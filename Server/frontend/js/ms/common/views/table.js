// var table = new TableView({
//   collection: collection,
//   columns: [
//     {
//       attr: 'id',
//       label: 'id'
//     }, {
//       attr: 'name',
//       label: 'Naam'
//     }
//   ]
// });
// table.on('click', function (model, event, column, rowView) {
// });

var Backbone = require('backbone');
var _ = require('underscore');
import RowView from 'ms/common/views/row.js';

var TableView = Backbone.View.extend({
  template: Handlebars.templates['layout/table'],
  initialize: function(settings) {
    this.columns = settings.columns;
    this.collection = settings.collection;
    this.forbidden = settings.forbidden;
    this.onClick = settings.onClick;
    this.rowClass = settings.rowClass || null;
    this.RowView = RowView(this.columns, this);
    this.emptyListCaption = settings.emptyListCaption || 'Geen resultaten gevonden';
  },
  setColumns: function (columns) {
    this.columns = columns;
  },
  render: function () {
    var self = this;
    this.$el.html(this.template({columns: this.columns}));
    this.$el.find('.tooltipped').tooltip({delay: 100});
    console.log(this.$el);
    var moreBtn = this.$('.more-btn')
      .click(function () {
        moreBtn.addClass('disabled');
        self.collection.getNext({
          success: function () {
            moreBtn.removeClass('disabled');
          }
        });
      });
    // this.$el.append(moreBtn);

    if (!this.collection.length) {
      moreBtn.addClass('disabled');
      this.collection.getNext({
        success: function (collection, response) {
          moreBtn.removeClass('disabled');
        }
      });
    }
    this.collection.on('ended', function () {
      // moreBtn.hide();
    });
    this.collectionView = new Backbone.CollectionView({
      emptyListCaption: this.emptyListCaption,
      el: this.$('table'),
      collection: this.collection,
      modelView: this.RowView
    }).render();
    return this;
  }
});

module.exports = TableView;
