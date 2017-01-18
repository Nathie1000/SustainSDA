var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Table from 'ms/common/views/dynamicTable.js';
import PatientSearchCollection from 'ms/admin/patients/collections/patientSearch.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['admin/patientOverview'],

  onTableClick: function(model){
    console.log(model);
    app.router.navigate('#patient/'+model.id,{trigger:true});
  },

  render: function () {
    this.$el.html(this.template());
    var col = new PatientSearchCollection();
    var table = new Table({
      collection: col,
      click: this.onTableClick,
      search: true,
      defaultSort: 'firstName',
      lazyLoading: {
        rows: 12,
      },
      columns: [
        {
          label: "Voornaam",
          attr: 'firstName',
        },
        {
          label: "Achternaam",
          attr: 'lastName',
        },
        {
          label: 'Behandelaar',
          attr: 'doctorName'
        }
      ]
    });
    this.$el.find('.patientTable').append(table.render().el);
    var newPatient = $(Handlebars.templates['layout/button']({
        label: 'Patient toevoegen',
        class: 'right'
    }));
    this.$el.find('.patientTopBar').append(newPatient);
    newPatient.click(()=>{
      app.router.navigate('#patient',{trigger:true});
    });
    return this;
  }
});

module.exports = HomeView;
