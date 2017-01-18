var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Form from 'ms/common/views/form.js';
import Tabs from 'ms/common/views/tabs.js';
import DTabs from 'ms/common/views/dynamicTabs.js';
import Chart from 'chart.js';
import ProgressionsTF from 'ms/doctor/patients/views/24Progression.js';
import ProgressionsWeek from 'ms/doctor/patients/views/weekProgression.js';
import ProgressionsMonth from 'ms/doctor/patients/views/monthProgression.js';
import ProgressionsYear from 'ms/doctor/patients/views/yearProgression.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patient/patientProgression'],

  render: function () {
    $.ajax('/patient/patients/getProgressData/'+this.model.id)
    .done(response=>{
      console.log(response);
      console.log(response);
      this.$el.html(this.template({
        patientName: 'Pietje puk',
        steps: response.steps,
        requiredSteps: response.stepGoal,
        daysCompleted: response.goalsCompleted,
        daysTotal: response.totalGoals,
        totalSteps: response.totalSteps
      }));
      // this.$el.addClass('container section');
      var patient = this.model

      $.ajax('/patient/patients/getChartData/'+this.model.id)
      .done(response=>{
        patient.chartData = response;
        var t = new DTabs({
          tabs: [
          {
                title: '24 uur',
                view: function () {
                  var v = new ProgressionsTF();
                  v.model = patient;
                  return v;
                }
          },
          {
                title: 'Week',
                view: function () {
                  var v = new ProgressionsWeek();
                  v.model = patient;
                  return v;
                }
          },
          {
                title: 'Maand',
                view: function () {
                  var v = new ProgressionsMonth();
                  v.model = patient;
                  return v;
                }
          },
          {
                title: 'Jaar',
                view: function () {
                  var v = new ProgressionsYear();
                  v.model = patient;
                  return v;
                }
          },

        ]});
        this.$el.find('.progression-stats').append(t.render().el);
      });
    });
      return this;
  }
});

module.exports = HomeView;
