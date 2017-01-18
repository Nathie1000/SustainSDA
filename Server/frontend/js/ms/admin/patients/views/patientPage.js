var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Patient from 'ms/models/patients.js';
import User from 'ms/models/users.js';
import Form from 'ms/common/views/form.js';

var PatientPageView = Backbone.View.extend({
    template: Handlebars.templates['admin/patient/patientPage'],

    createForm: function(createNew){
      this.$el.html(this.template());
      $.ajax('admin/doctors/allDoctorsByPractice/'+app.currentPractice.id) 
      .done(res=>{
        console.log(res);
        var doctors = [];
        _.each(res,d=>{
          doctors.push({id:d.id,label:d.firstName + ' ' + d.lastName});
        });
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
            },  {
                attr: 'doctor_id',
                label: 'Lijst met doctoren',
                type: 'singleselect',
                size: 12,
                values: doctors
            }, {
                attr: 'emergencyNumber',
                label: 'Noodnummer',
                size: 4
            }, {
                attr: 'beltNumber',
                label: 'Riemnummer',
                size: 4
            }, {
                attr: 'SHDNumber',
                label: 'SHDNummer',
                size: 4
            }, ]
        });
        var m2 = new User({id:this.model.get('user_id')});
        var form2 = new Form({
          model: m2,
          fields:[
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
                type: 'password',
                showHidden: true,
                size: 6
            },
          ]
        });
        Backbone.Validation.bind(form);
        Backbone.Validation.bind(form2);

        this.$el.find('.patient-info-section').append(form.render().el);
        $('select').material_select();
        var add = $(Handlebars.templates['layout/button']({
            label: (createNew)?'Patient toevoegen':'Opslaan',
        }));
        this.$el.find('.patient-info-section').append(add);
        add.click(() => {
          if(this.model.get('password') === this.model.get('password_repeat')){
            model.unset('password_repeat');
            this.model.save(null, {
                success: () => {
                    m2.save(null, {
                        success: () => {
                            swal('Gebruiker succesvol opgeslagen', undefined, 'success');
                        }
                    });
                }
            });
          } else swal('Wachtwoorden zijn niet gelijk.', undefined,'error');
        });
      });
    },


    renderNew: function(){
      this.model = new Patient();
      // this.model.setUrl('admin/patients');
      this.createForm();
      return this;
    },

    render: function(id) {
      if(!id) return this.renderNew();
        this.model = new Patient({
            id: id
        });
        // this.model.setUrl('admin/patients');
        this.model.fetch({
            success: () => {
                this.createForm();
            }
        });


        return this;
    }
});

module.exports = PatientPageView;
