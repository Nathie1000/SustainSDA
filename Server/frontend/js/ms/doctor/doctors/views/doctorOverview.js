var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import TableSearch from 'ms/common/views/dynamicTable.js';
import DoctorCollection from 'ms/doctor/doctors/collections/doctorSearch.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patientOverview'],

  render: function () {
    this.$el.html(this.template());
    var col = new DoctorCollection();
    var table = new TableSearch({
      collection: col,
      search:true,
      defaultSort: 'firstName',
      lazyLoading: {
        rows: 12,
      },
      click: m=>{
        app.router.navigate('doctor/' + m.id, {
          trigger: true
        });
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
      ]
    });
    this.$el.find('.userTable').append(table.render().el);
    // table.on('click', m=>{
    //   console.log(m);
    //   app.router.navigate('doctor/' + m.id, {
    //     trigger: true
    //   });
    // });
    var newDoctor = $(Handlebars.templates['layout/button']({
        label: 'Fysiotherapeut toevoegen',
        class: 'right'
    }));
    this.$el.find('.topRow').append(newDoctor);
    newDoctor.click(()=>{
      app.router.navigate('doctor',{trigger:true});
    });
    return this;
  }
});

module.exports = HomeView;
