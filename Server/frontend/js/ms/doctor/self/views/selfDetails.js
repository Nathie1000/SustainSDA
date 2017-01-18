var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import User from 'ms/models/users.js';
import Form from 'ms/common/views/form.js';

var HomeView = Backbone.View.extend({

  render: function () {
    var m = new User(app.currentFysio.toJSON());
    var form = new Form({
      model: app.currentFysio,
      fields: [
        {
            attr: 'firstName',
            label: 'Voornaam',
            size: 6
        }, {
            attr: 'lastName',
            label: 'Achternaam',
            size: 6
        }
      ]
    });
    var form2 = new Form({
      model: m,
      fields: [
        {
            attr: 'username',
            label: 'Gebruikersnaam',
            size: 12
        }, {
            attr: 'password',
            label: 'Wachtwoord',
            showHidden: true,
            type: 'password',
            size: 6
        }, {
            attr: 'password_repeat',
            label: 'Herhaal wachtwoord',
            showHidden: true,
            type: 'password',
            size: 6
        }
      ]
    });
    this.$el.append(form.render().el);
    this.$el.append(form2.render().el);
    Backbone.Validation.bind(form);
    Backbone.Validation.bind(form2);
    return this;
  }
});

module.exports = HomeView;
