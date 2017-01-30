var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Table from 'ms/common/views/dynamicTable.js';
import PracticeSearchCollection from 'ms/admin/home/collections/practiceSearch.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['admin/practiceOverview'],

  onTableClick: function(model){
    console.log(model);
    model.save();
    app.currentPractice = model;

    app.router.navigate('#practice/'+model.id,{trigger:true});
  },

  render: function () {
    this.$el.html(this.template());
    var col = new PracticeSearchCollection();
    var table = new Table({
      collection: col,
      click: this.onTableClick,
      defaultSort: 'name',
      search: true,
      lazyLoading: {
        rows: 12,
      },
      columns: [
        {
          label: "Praktijk",
          attr: 'name',
        },
        {
          label: "Stad",
          attr: 'city',
        },
      ]
    });
    this.$el.find('.practiceTable').append(table.render().el);
    var newPractice = $(Handlebars.templates['layout/button']({
        label: 'Praktijk aanmaken',
        class: 'right'
    }));
    this.$el.find('.practiceTopBar').append(newPractice);
    newPractice.click(()=>{
      app.currentPractice =null;
      app.router.navigate('#practice',{trigger:true});
    });
    return this;
  }
});

module.exports = HomeView;
