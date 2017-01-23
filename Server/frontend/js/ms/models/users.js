import Backbone from 'backbone';
import app from 'ms/app.js';
import BaseModel from './baseModel.js';

var User = BaseModel.extend({

  initialize: function(){
    this.urlRoot = 'user/users';
  },

  validation: {
    username: {
      required: true,
      msg: 'Gebruikersnaam is vereist'
    },
    password: {
      minLength: 6,
      maxLength: 32,
      required: false,
      msg: 'Wachtwoord is vereist en moet tussen de 6 en 32 karakters bevatten.'
    },
    password_repeat: {
      required: false,
      equalTo: 'password',
      msg: 'Herhaalde wachtwoord moet gelijk zijn aan wachtwoord'
    },

  }
});

module.exports = User;
