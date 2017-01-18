var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Chart from 'chart.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patient/patientProgressionTabWeek'],

  render: function () {
    this.$el.html(this.template());
    var data = {labels: this.model.chartData.month.dates,
    datasets: [{
        label: 'Stappen gezet',
        data: this.model.chartData.month.steps,
        backgroundColor:[
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
            'rgba(255, 99, 132, 0.4)',
        ],
        borderColor: [
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
            'rgba(255,99,132,1)',
        ],
        borderWidth: 1
    },
    {
        label: 'Stappen doel',
        data: this.model.chartData.month.goals,
        backgroundColor:[
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
            'rgba(132, 99, 255, 0.4)',
        ],
        borderColor: [
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
            'rgba(132, 99, 255,1)',
        ],
        borderWidth: 1
    }
  ]};
    var dataLine = {labels: this.model.chartData.month.dates,
    datasets: [{
      backgroundColor: [
        'rgba(255, 99, 132, 0.4)'
      ],
      label: 'Stappen gezet',
      data: this.model.chartData.month.stepsTotal,
    }, {
      backgroundColor: [
        'rgba(0, 0, 240, 0.4)'
      ],
      label: 'Stappen doel',
      data: this.model.chartData.month.goalsTotal,
  }]};
    setTimeout(()=>{
      var c = new Chart(this.$el.find('#firstChart'),{
        type: 'bar',
        data: data,
        options: {
          defaultColor: 'rgba(255,99,132,1)',
          scales: {
            yAxes: [{
              ticks: {
                beginAtZero:true
              }
            }]
          }
        }
      });
      var c2 = new Chart(this.$el.find('#secondChart'),{
        type: 'line',
        data: dataLine,
        options: {
          scales: {
            yAxes: [{
              ticks: {
                beginAtZero:true
              }
            }]
          }
        }
      });
    },1);
      return this;
  }
});

module.exports = HomeView;
