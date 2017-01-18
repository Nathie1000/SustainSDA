var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Tabs from 'ms/common/views/tabs.js';
import PatientOverviewPage from 'ms/doctor/patients/views/patientOverview.js';
import PracticeDetailPage from 'ms/doctor/practices/views/practiceDetails.js';
import SelfDetailPage from 'ms/doctor/self/views/selfDetails.js';
import DoctorOverviewPage from 'ms/doctor/doctors/views/doctorOverview.js';

var HomeView = Backbone.View.extend({

  render: function () {
    var t = new Tabs([
      {
            title: 'Patient Overzicht',
            view: function () {
              return new PatientOverviewPage();
            }
      },
      {
            title: 'Praktijk informatie',
            view: function () {
              var v = new PracticeDetailPage();
              return v;
            }
      },
      {
            title: 'Fysiotherapeuten overzicht',
            view: function () {
              var v = new DoctorOverviewPage();
              return v;
            }
      },
      {
            title: 'Eigen gegevens',
            view: function () {
              var v = new SelfDetailPage();
              return v;
            }
      },

    ]);
    this.$el.append(t.render().el);
    return this;
  }
});

module.exports = HomeView;
