import Backbone from 'backbone';
import app from 'ms/app.js';
var _ = require('underscore');
import async from 'async';
import Practice from 'ms/models/practices.js';
import Doctor from 'ms/models/doctors.js';
import HomePage from 'ms/doctor/homePage.js';
// import PatientOverviewPage from 'ms/doctor/patients/views/patientOverview.js';
import PatientMasterPage from 'ms/doctor/patients/views/patientMaster.js';
import DoctorPage from 'ms/doctor/doctors/views/doctorPage.js';

module.exports = function(callback) {
  if (app.currentUser.get('role') !== app.ROLE_DOCTOR) {
    callback();
    return;
  }
  console.log('whieeee');
  app.setHome('home');

  app.addPath('home', HomePage, 'Hoofdmenu');
  // app.addPath('patients', PatientOverviewPage, 'Patientoverzicht');
  app.addPath('patient', PatientMasterPage,'Nieuwe Patient');
  app.addPath('patient/:id', PatientMasterPage,'Patient details');

  app.addPath('doctor', DoctorPage,'Nieuwe Fysiotherapeut');
  app.addPath('doctor/:id', DoctorPage,'Fysiotherapeut details');

  $('li.home').click(()=>{
    window.location = $('li.home > a').attr('href');
  });

  async.parallel([
      function(next) {
        $.ajax('/fysio/doctors/current')
        .done(response=>{
          app.currentFysio = new Doctor(response);
          $('.welcome-text').text('Welkom ' + response.firstName + ' ' + response.lastName);
          next();
        });
      },
      function(next) {
        $.ajax('/fysio/practices/current')
        .done(response=>{
          app.currentPractice = new Practice(response);
          next();
        });
      },
    ],
    function(err, results) {
      console.log('test');
        callback();
      }
    );

};
