var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Tabs from 'ms/common/views/tabs.js';
import Patient from 'ms/models/patients.js';
import PatientDetailPage from 'ms/patient/patients/views/patientDetails.js';
import PatientProgressionPage from 'ms/patient/patients/views/patientProgression.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patientOverview'],

  renderTabs: (self,patient)=>{
    var t = new Tabs([
      {
            title: 'Jouw Vorderingen',
            view: function () {
              var v = new PatientProgressionPage();
              v.model = patient;
              return v;
            }
      },
      {
        title: 'Persoonsgegevens',
        view: function () {
          var v = new PatientDetailPage();
          v.model = patient;
          return v;
        }
      },

    ]);
    console.log('trolololol');
    self.$el.append(t.render().el);
  },

  render: function () {
    this.renderTabs(this,app.currentPatient);
    console.log('b lalalala');
    return this;
  }
});

module.exports = HomeView;
