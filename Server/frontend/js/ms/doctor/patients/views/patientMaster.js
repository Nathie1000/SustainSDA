var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Tabs from 'ms/common/views/tabs.js';
import Patient from 'ms/models/patients.js';
import PatientDetailPage from 'ms/doctor/patients/views/patientDetails.js';
import PatientProgressionPage from 'ms/doctor/patients/views/patientProgression.js';
import PatientGoalPage from 'ms/doctor/patients/views/patientGoals.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patientOverview'],

  renderTabs: (self,patient)=>{
    var t = new Tabs([
      {
            title: 'Patient details',
            view: function () {
              var v = new PatientDetailPage();
              v.model = patient;
              return v;
            }
      },
      {
            title: 'Patient Vorderingen',
            view: function () {
              var v = new PatientProgressionPage();
              v.model = patient;
              return v;
            }
      },
      {
            title: 'Patient Doelstellingen',
            view: function () {
              var v = new PatientGoalPage();
              v.model = patient;
              return v;
            }
      },

    ]);
    self.$el.append(t.render().el);
  },

  render: function (id) {
    if(id){

      var patient = new Patient({id: id});
      patient.fetch({success:()=>{
        this.renderTabs(this,patient);
      }});
    } else {
      var patient = new Patient();
      var page = new PatientDetailPage();
      page.model = patient;
      this.$el.html(page.render().el);
    }


    return this;
  }
});

module.exports = HomeView;
