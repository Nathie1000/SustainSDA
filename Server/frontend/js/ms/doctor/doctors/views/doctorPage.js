var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import DoctorCollection from 'ms/doctor/doctors/collections/doctorSearch.js';
import Form from 'ms/common/views/form.js';
import User from 'ms/models/users.js';
import Doctor from 'ms/models/doctors.js';
import {saveModels} from 'ms/utils.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patientOverview'],

  renderForm(self,fysio){
    var m = new User(fysio.toJSON());
    m.set('id', m.get('user_id'));
    var form = new Form({
      model: fysio,
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
    self.$el.append(form.render().el);
    self.$el.append(form2.render().el);
    Backbone.Validation.bind(form);
    Backbone.Validation.bind(form2);
    var add = $(Handlebars.templates['layout/button']({
        label: (!fysio.id)?'Fysiotherapeut toevoegen':'Opslaan',
    }));
    this.$el.append(add);
    add.click(() => {
      fysio.save(null,{success:()=>{
        m.save(null,{success:()=>{
          swal('Succesvol opgeslagen!',null,'success');
        }})
      }});
    });
  },

  render: function (id) {
    if(id){
      var fysio = new Doctor({id: id});
      fysio.fetch({success:()=>{
        this.renderForm(this,fysio);
      }});
    } else {
      var fysio = new Doctor();
      this.renderForm(this,fysio)
    }


    return this;
  }
});

module.exports = HomeView;
