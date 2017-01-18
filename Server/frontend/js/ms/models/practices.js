import Backbone from 'backbone';
import app from 'ms/app.js';
import BaseModel from './baseModel.js';

var Practice = BaseModel.extend({

  initialize: function(){
    this.urlRoot = ((app.currentUser.get('role') === app.ROLE_ADMIN) ? 'admin/' : 'fysio/') + 'practices';
  },

  validation: {
    name: {
      minLength: 3,
      required: true,
      msg: 'Praktijknaam is vereist en moet bestaan uit minimaal 3 karakters.'
    },
    city: {
      minLength: 2,
      required: true,
      msg: 'Stad is vereist en moet bestaan uit minimaal 2 karakters.'
    }
  }
});

module.exports = Practice;
