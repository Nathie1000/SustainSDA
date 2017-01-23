var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Form from 'ms/common/views/form.js';
import User from 'ms/models/users.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patient/patientDetails'],

  render: function () {
    this.$el.html(this.template());
    this.$el.parent().addClass('z-depth-2');
    this.$el.addClass('small-margin');
    var m;
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
          {
            label: 'Begeleider',
            attr: 'doctor_id',
            type: 'singleselect',
            values:app.doctors,
            size: 12
          },

        ]
      });
      Backbone.Validation.bind(f);
      this.$el.append(f.render().el);
      if(!this.model.id){
        m = new User();
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
        Backbone.Validation.bind(form2);
        this.$el.append(form2.render().el);
      }
      var saveSelf = $(Handlebars.templates['layout/button']({
          label: 'Opslaan',
          class: 'right'
      }));
      this.$el.append(saveSelf);
      saveSelf.click(()=>{
        if(!this.model.id){
          m.save(null,{success:()=>{
            this.model.set('user_id',m.id);
            this.model.save(null,{success:()=>{
              app.router.navigate('/patient/'+this.model.id,{trigger: true});
            }});
          }});
        } else {
          this.model.save(null,{success:()=>{
            swal('Succesvol opgeslagen!', null, 'success');
          }});
        }
        // app.currentFysio.save(null,{success:()=>{
        //   m.save(null,{success:()=>{
        //     swal('Succesvol opgeslagen!',null,'success');
        //   }})
        // }});
      });
      $('select').material_select();
    return this;
  }
});

module.exports = HomeView;
