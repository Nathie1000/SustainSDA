import Backbone from 'backbone';
import app from 'ms/app.js';
var _ = require('underscore');
import async from 'async';
import Patient from 'ms/models/patients.js';
import PatientMasterPage from 'ms/patient/patients/views/patientMaster.js';
var aesjs = require('aes-js');
var base64url = require('base64-url');
var crypto = require('crypto-js');

module.exports = function(callback) {
  if (app.currentUser.get('role') !== app.ROLE_PATIENT) {
    callback();
    return;
  }
  app.setHome('home');
  app.addPath('home', PatientMasterPage, 'Hoofdmenu');

  $('li.home').click(()=>{
    window.location = $('li.home > a').attr('href');
  });

  async.parallel([
      function(next) {
        $.ajax('/patient/patients/current')
        .done(response=>{
          app.currentPatient = new Patient(response);
          $('.welcome-text').text('Welkom ' + response.firstName + ' ' + response.lastName);
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
