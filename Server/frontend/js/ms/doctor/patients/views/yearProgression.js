var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Chart from 'chart.js';

var HomeView = Backbone.View.extend({
    template: Handlebars.templates['doctor/patient/patientProgressionTabYear'],

    render: function() {
        this.$el.html(this.template());
        var data = {
            labels: [
                "Januari",
                "Februari",
                "Maart",
                "April",
                "Mei",
                "Juni",
                "juli",
                "Augustus",
                "September",
                "Oktober",
                "November",
                "December"
            ],
            datasets: [
                {
                    label: 'Stappen gezet',
                    data: this.model.chartData.year.steps,
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
                    ],
                    borderWidth: 1
                }, {
                    label: 'Stappen doel',
                    data: this.model.chartData.year.goals,
                    backgroundColor: [
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
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                        'rgba(132,99,255,1)',
                    ],
                    borderWidth: 1
                }
            ]
        }
    var dataLine = {
        labels: [
            "Januari",
            "Februari",
            "Maart",
            "April",
            "Mei",
            "Juni",
            "juli",
            "Augustus",
            "September",
            "Oktober",
            "November",
            "December"
        ],
        datasets: [
            {
                backgroundColor: ['rgba(255, 99, 132, 0.4)'],
                label: 'Totaal aantal stappen gezet',
                data: this.model.chartData.year.stepsTotal
            }, {
                backgroundColor: ['rgba(0, 0, 240, 0.4)'],
                label: 'Totaal aantal stappen doel',
                data: this.model.chartData.year.goalsTotal
            }
        ]
    };
    setTimeout(() => {
        var c = new Chart(this.$el.find('#firstChart'), {
            type: 'bar',
            data: data,
            options: {
                scales: {
                    yAxes: [
                        {
                            ticks: {
                                beginAtZero: true
                            }
                        }
                    ]
                }
            }
        });
        var c2 = new Chart(this.$el.find('#secondChart'), {
            type: 'line',
            data: dataLine,
            options: {
                scales: {
                    yAxes: [
                        {
                            ticks: {
                                beginAtZero: true
                            }
                        }
                    ]
                }
            }
        });
    }, 1);
    return this;
}});

module.exports = HomeView;
