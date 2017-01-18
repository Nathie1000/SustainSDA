var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import TableSearch from 'ms/common/views/dynamicTable.js';
import PatientCollection from 'ms/doctor/patients/collections/patientSearch.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patientOverview'],

  render: function () {
    this.$el.html(this.template());
    var col = new PatientCollection();
    var table = new TableSearch({
      collection: col,
      search: true,
      click: m=>{
        app.router.navigate('patient/' + m.id, {
          trigger: true
        });
      },
      defaultSort: 'firstName',
      lazyLoading: {
        rows: 12,
      },
      rowClass: {class:'table-row'},
      columns: [
        {
          attr: 'firstName',
          label: 'Voornaam'
        },
        {
          attr: 'lastName',
          label: 'Achternaam'
        },
        {
          attr: 'doctor',
          label: 'Behandelaar'
        },
      ]
    });
    this.$el.find('.userTable').append(table.render().el);
    var newPractice = $(Handlebars.templates['layout/button']({
        label: 'Patient toevoegen',
        class: 'right'
    }));
    this.$el.find('.topRow').append(newPractice);
    newPractice.click(()=>{
      app.router.navigate('#patient',{trigger:true});
    });
    return this;
  }
});

module.exports = HomeView;
