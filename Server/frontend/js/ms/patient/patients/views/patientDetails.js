var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import User from 'ms/models/users.js';
import Form from 'ms/common/views/form.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patient/patientDetails'],

  render: function () {
    this.$el.html(this.template());
    this.$el.parent().addClass('z-depth-2');
    this.$el.addClass('small-margin');
      var f = new Form({
        model: this.model,
        fields: [
          {
            label: 'Voornaam',
            attr: 'firstName',
            size: 6
          },
          {
            label: 'Achternaam',
            attr: 'lastName',
            size: 6
          },
        ]
      });
      var m = new User(this.model.attributes);
      m.set('id',m.get('user_id'));
      var f2 = new Form({
        model: m,
        fields: [
          {
            label: 'Gebruikersnaam',
            attr: 'username',
            size: 12
          },
          {
            label: 'Wachtwoord',
            attr: 'password',
            type: 'password',
            size: 6
          },
          {
            label: 'Herhaal wachtwoord',
            attr: 'password_repeat',
            type: 'password',
            size: 6
          },
        ]
      });

      Backbone.Validation.bind(f);
      Backbone.Validation.bind(f2);
      this.$el.append(f.render().el);
      this.$el.append(f2.render().el);
      var add = $(Handlebars.templates['layout/button']({
          label: 'Opslaan',
      }));
      add.css('margin-bottom', '10px');
      this.$el.append(add);
      add.click(() => {
        this.model.save(null,{success:()=>{
          console.log(456);
          // m.save(null, {
          //     success: function (model, response) {
          //         console.log("success");
          //     },
          //     error: function (model, response) {
          //         console.log("error");
          //     }
          // });
          m.save(null,{success:()=>{
            console.log(1235555);
            swal('Succesvol opgeslagen', null,'success');
          },error:e=>{
            console.log(e);
          }});
        }});
      });
    return this;
  }
});

module.exports = HomeView;
