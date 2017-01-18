var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import User from 'ms/models/users.js';
import Doctor from 'ms/models/doctors.js';
import Practice from 'ms/models/practices.js';
import Form from 'ms/common/views/form.js';
import {validateSwalModels} from 'ms/utils.js'

var PatientPageView = Backbone.View.extend({
    template: Handlebars.templates['admin/practice/practicePage'],

    createForm: function(model,createNew){
      this.$el.html(this.template());
      var form = new Form({
          model: model,
          fields: [{
              attr: 'name',
              label: 'Naam',
              size: 6
          }, {
              attr: 'city',
              label: 'Stad',
              size: 6
          },
        ]
      });
      this.$el.find('.practice-info-section').append(form.render().el);
      Backbone.Validation.bind(form);
      if(createNew){
        var m2 = new Doctor();
        var form2 = new Form({
          model: m2,
          fields: [
              {
                attr: 'firstName',
                label: 'Voornaam (hoofdgebruiker)',
                size: 6
            }, {
                attr: 'lastName',
                label: 'Achternaam (hoofdgebruiker)',
                size: 6
            }
          ]
        });
        var m3 = new User();
        var form3 = new Form({
          model: m3,
          fields: [
            {
                attr: 'username',
                label: 'Gebruikersnaam (hoofdgebruiker)',
                size: 12
            }, {
                attr: 'password',
                label: 'Wachtwoord (hoofdgebruiker)',
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
        })
        this.$el.find('.practice-info-section').append(form2.render().el);
        Backbone.Validation.bind(form2);
        this.$el.find('.practice-info-section').append(form3.render().el);
        Backbone.Validation.bind(form3);
      }
      var add = $(Handlebars.templates['layout/button']({
          label: (createNew)?'Praktijk aanmaken':'Opslaan',
          class: 'right'
      }));
      this.$el.find('.practice-info-section').append(add);
      add.click(() => {
        if(model.get('password') === model.get('password_repeat')){
          model.unset('password_repeat');
          console.log(1);
          if(validateSwalModels({model,m2,m3}) === true) //meaning model is valid
          {
            console.log(2);
              model.save(null,{success:()=>{
                console.log(3);
              if(createNew){
                console.log(4);
                m3.set('role', app.ROLE_DOCTOR);
                m3.save(null,{success:()=>{
                  m2.set('user_id', m3.id);
                  m2.set('practice_id',model.id);
                  m2.save(null,{success:()=>{
                    swal('Succesvol opgeslagen', undefined, 'success');
                  }});
                }});
              }
              else swal('Succesvol opgeslagen', undefined, 'success');
            }});
          } else console.log('help');
        } else swal('Wachtwoorden zijn niet gelijk.', undefined,'error');
      });
    },


    renderNew: function(){
      this.model = new Practice();
      this.createForm(this.model,true);
      return this;
    },

    render: function() {
      if(!app.currentPractice) return this.renderNew();
        this.$el.html(this.template());
        this.createForm(app.currentPractice);
        return this;
    }
});

module.exports = PatientPageView;
