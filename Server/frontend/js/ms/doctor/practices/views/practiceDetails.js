var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Form from 'ms/common/views/form.js';

var HomeView = Backbone.View.extend({

  render: function () {
    var f = new Form({
      model: app.currentPractice,
      saveButton: true,
      fields: [
        {
          attr: 'name',
          label: 'Praktijknaam',
          size: 6
        },
        {
          attr: 'city',
          label: 'Stad',
          size: 6
        },
      ]
    });
    this.$el.append(f.render().el);
    Backbone.Validation.bind(f);
    return this;
  }
});

module.exports = HomeView;
