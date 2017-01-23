var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import User from 'ms/models/users.js';
import Form from 'ms/common/views/form.js';

var HomeView = Backbone.View.extend({

  render: function () {
    var m = new User(app.currentFysio.toJSON());
    m.set('id', m.get('user_id'));
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
    var saveSelf = $(Handlebars.templates['layout/button']({
        label: 'Opslaan',
        class: 'right'
    }));
    this.$el.append(saveSelf);
    saveSelf.click(()=>{
      app.currentFysio.save(null,{success:()=>{
        m.save(null,{success:()=>{
          swal('Succesvol opgeslagen!',null,'success');
        }})
      }});
    });
    return this;
  }
});

module.exports = HomeView;
