var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Form from 'ms/common/views/form.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patient/patientDetails'],

  render: function () {
    this.$el.html(this.template());
    this.$el.parent().addClass('z-depth-2');
    this.$el.addClass('small-margin');
      var f = new Form({
        saveButton: {
          preSave: ()=>{
            if(!this.model.id)
              this.newModel = true;
          },
          postSave: ()=>{
            if(this.newModel)
              app.router.navigate('/patient/'+this.model.id,{trigger: true});
          }
        },
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
            attr: 'doctor',
            size: 6
          },
          {
            label: 'test',
            attr: 'test',
            size: 6,
          },
        ]
      });
      Backbone.Validation.bind(f);
      this.$el.append(f.render().el);
    return this;
  }
});

module.exports = HomeView;
