var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Table from 'ms/common/views/dynamicTable.js';
import DoctorSearchCollection from 'ms/admin/doctors/collections/doctorSearch.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['admin/patientOverview'],

  onTableClick: function(model){
    console.log(model);
    app.router.navigate('#fysio/'+model.id,{trigger:true});
  },

  render: function () {
    this.$el.html(this.template());
    var col = new DoctorSearchCollection();
    var table = new Table({
      collection: col,
      click: this.onTableClick,
      search:true,
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
          label: 'Praktijk',
          attr: 'practiceName'
        }
      ]
    });
    this.$el.find('.patientTable').append(table.render().el);
    var newPatient = $(Handlebars.templates['layout/button']({
        label: 'Fysiotherapeut toevoegen',
        class: 'right'
    }));
    this.$el.find('.patientTopBar').append(newPatient);
    newPatient.click(()=>{
      app.router.navigate('#fysio',{trigger:true});
    });
    return this;
  }
});

module.exports = HomeView;
