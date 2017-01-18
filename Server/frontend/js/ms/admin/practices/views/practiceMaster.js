var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Practice from 'ms/models/practices.js';
import Tabs from 'ms/common/views/tabs.js';

import PracticeInfoPage from 'ms/admin/practices/views/practiceInfoPage.js';
import PatientOverviewPage from 'ms/admin/patients/views/patientOverview.js';
import DoctorOverviewPage from 'ms/admin/doctors/views/doctorOverview.js';

var PatientPageView = Backbone.View.extend({

    render: function(id) {
      if(!app.currentPractice){
        app.currentPractice = new Practice({id:id});
        // app.currentPractice.setUrl('admin/practices');
        app.currentPractice.fetch();
      }
      var t = new Tabs([
        {
          title: 'Patiënten',
          view: function () {
            var v = new PatientOverviewPage();
            return v;
          }
        },
        {
          title: 'Fysiotherapeuten',
          view: function () {
            var v = new DoctorOverviewPage();
            return v;
          }
        },
        {
          title: 'Praktijk informatie',
          view: function () {
            var v = new PracticeInfoPage();
            return v;
          }
        },
      ]);
      this.$el.append(t.render().el);
      return this;
    }
});

module.exports = PatientPageView;
