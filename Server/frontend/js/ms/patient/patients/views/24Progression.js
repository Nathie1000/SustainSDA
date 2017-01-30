var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Chart from 'chart.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patient/patientProgressionTab24'],

  render: function () {
    console.log(123);
    this.$el.html(this.template());
      var data = {labels: ["Om 1 uur","Om 2 uur","Om 3 uur","Om 4 uur","Om 5 uur","Om 6 uur",
      "Om 7 uur","Om 8 uur","Om 9 uur","Om 10 uur","Om 11 uur","Om 12 uur",
      "Om 13 uur","Om 14 uur","Om 15 uur","Om 16 uur","Om 17 uur","Om 18 uur",
      "Om 19 uur","Om 20 uur","Om 21 uur","Om 22 uur","Om 23 uur","Om 24 uur"],
      datasets: [{
          label: 'Stappen gezet',
          data: this.model.chartData.tf,
          backgroundColor: [
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
          ],
          borderWidth: 1
      },
    ]};
      setTimeout(()=>{
        var c = new Chart(this.$el.find('#firstChart'),{
          type: 'bar',
          data: data,
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
    // });

      return this;
  }
});

module.exports = HomeView;
