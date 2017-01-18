import Backbone from 'backbone';
import app from 'ms/app.js';
import BaseModel from './baseModel.js';

var Doctor = BaseModel.extend({

  initialize: function(){
    this.urlRoot = ((app.currentUser.get('role') === app.ROLE_ADMIN) ? 'admin/' : 'fysio/') + 'doctors';
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

module.exports = Doctor;
