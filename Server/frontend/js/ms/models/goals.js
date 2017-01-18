import Backbone from 'backbone';
import app from 'ms/app.js';
import BaseModel from './baseModel.js';

var Goal = BaseModel.extend({

  initialize: function(){
    this.urlRoot = ((app.currentUser.get('role') === app.ROLE_ADMIN) ? 'admin/' : 'fysio/') + 'goals';
  },

});

module.exports = Goal;
