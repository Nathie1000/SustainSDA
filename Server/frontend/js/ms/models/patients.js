import Backbone from 'backbone';
import app from 'ms/app.js';
import BaseModel from './baseModel.js';

var Patient = BaseModel.extend({

  initialize: function(){
    var role = app.currentUser.get('role');
    if(role === app.ROLE_ADMIN) this.urlRoot = 'admin/patients'; else if(role === app.ROLE_DOCTOR) this.urlRoot = 'fysio/patients'; else this.urlRoot = 'patient/patients';
  },
  validation: {
    firstName: {
      minLength: 2,
      required: true,
      msg: 'Voornaam is vereist en moet langer zijn dan 1 karakter.'
    },
    lastName: {
      required: true,
      minLength: 2,
      msg: 'Achternaam is vereist en moet langer zijn dan 1 karakter.'
    }
  }
});

module.exports = Patient;
