import Backbone from 'backbone';
import app from 'ms/app.js';
var _ = require('underscore');
import async from 'async';
import PracticeOverviewPage from 'ms/admin/home/views/adminPractices.js';
import PatientPage from 'ms/admin/patients/views/patientPage.js';
import PracticePage from 'ms/admin/practices/views/practiceMaster.js';
import NewPracticePage from 'ms/admin/practices/views/practiceInfoPage.js';
import DoctorPage from 'ms/admin/doctors/views/doctorPage.js';

module.exports = function(callback) {
  if (app.currentUser.get('role') !== app.ROLE_ADMIN) {
    callback();
    return;
  }
  console.log('in admin');
  app.setHome('home');

  app.addPath('home', PracticeOverviewPage, 'Praktijken overzicht');

  app.addPath('patient', PatientPage, 'Nieuwe patient');
  app.addPath('patient/:id', PatientPage, 'Patient informatie');

  app.addPath('practice', NewPracticePage, 'Nieuwe praktijk');
  app.addPath('practice/:id', PracticePage, 'Praktijk informatie');

  app.addPath('fysio', DoctorPage, 'Nieuwe Fysiotherapeut');
  app.addPath('fysio/:id', DoctorPage, 'Fysiotherapeut informatie');

  $('li.home').click(()=>{
    window.location = $('li.home > a').attr('href');
  });


  async.parallel([
      function(next) {
        next();
      }
    ],
    function(err, results) {
      console.log('test');
        callback();
      }
    );

};
