var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Doctor from 'ms/models/doctors.js';
import User from 'ms/models/users.js';
import Form from 'ms/common/views/form.js';

var PatientPageView = Backbone.View.extend({
    template: Handlebars.templates['admin/patient/patientPage'],

    createForm: function(createNew){
      this.$el.html(this.template());
      var form = new Form({
          model: this.model,
          fields: [{
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
      var m2 = new User({id: this.model.get('user_id')});
      var form2 = new Form({
        model: m2,
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
        }]
      });
      this.$el.find('.patient-info-section').append(form.render().el);
      this.$el.find('.patient-info-section').append(form2.render().el);
      Backbone.Validation.bind(form);
      Backbone.Validation.bind(form2);

      $('select').material_select();
      var add = $(Handlebars.templates['layout/button']({
          label: (createNew)?'Fysiotherapeut toevoegen':'Opslaan',
      }));
      this.$el.find('.patient-info-section').append(add);
      add.click(() => {
        // if(this.model.get('password') === this.model.get('password_repeat')){
          this.model.unset('password_repeat');
          this.model.save(null, {
              success: () => {
                  m2.save(null, {
                      success: () => {
                          swal('Gebruiker succesvol opgeslagen', undefined, 'success');
                      }
                  });
              }
          });
        // } else swal('Wachtwoorden zijn niet gelijk.', undefined,'error');
      });
    },


    renderNew: function(){
      this.model = new Doctor();
      this.createForm();
      return this;
    },

    render: function(id) {
      if(!id) return this.renderNew();
        this.model = new Doctor({
            id: id
        });
        this.model.fetch({
            success: () => {
                this.createForm();
            }
        });


        return this;
    }
});

module.exports = PatientPageView;
