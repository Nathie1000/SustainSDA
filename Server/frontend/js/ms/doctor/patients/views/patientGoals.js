var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';
import Form from 'ms/common/views/form.js';
import Goal from 'ms/models/goals.js';
import {getMonthFromString} from 'ms/utils.js';

var HomeView = Backbone.View.extend({
  template: Handlebars.templates['doctor/patient/patientGoals'],


  /*
    This method changes all text to dutch
  */
  setLocale(){
    this.$el.find('.ui-datepicker-prev').text('Vorige maand');
    this.$el.find('.ui-datepicker-next').text('Volgende maand');
    var headers = this.$el.find('th');
    var daysShort = 'Zo Ma Di Wo Do Vr Za'.split(' ');
    var months = 'Januari Februari Maart April Mei Juni Juli Augustus September Oktober November December'.split(' ');
    var i = 0;
    _.each(headers, h=>{
      h = $(h);
      h.text(daysShort[i]);
      i++;
    });
    var month = getMonthFromString()
    this.$el.find('.ui-datepicker-month').text(months[getMonthFromString(this.$el.find('.ui-datepicker-month').text())-1])
    console.log();
  },

  /*
    This method gives all date fields a step value if they have one.
    It also binds a click event to the prev, next and all TD(dates) elements and if they are clicked this method is called again.

  */

  setStepsByDate(){
    this.setLocale();
    this.$el.find('.ui-datepicker-prev').click(()=>{
      this.setStepsByDate();
    });
    this.$el.find('.ui-datepicker-next').click(()=>{
      this.setStepsByDate();
    });
    var arr = this.$el.find('.goalDates').find('td');
    _.each(arr, val=>{
      var $val = $(val);
      $val.click(()=>{
        this.setStepsByDate();
      })
      var day = '' + $val.find('a').text();
      day = (day.length === 1) ? '0'+day:day;
      var month = '' + ($val.data('month')+1);
      month = (month.length === 1) ? '0'+month:month;
      var dateVal = $val.data('year') + '-'+month + '-' + day;
      $val.removeClass('steps-value');
      if(this.dates[dateVal]){
        $val.addClass('steps-value');
          $val.append('</br><a class="steps-show">('+this.dates[dateVal]+')</a>');
      }
    });
  },

  render: function () {
    var self = this;
    var dates = []; //only date
    this.dates = {}; //steps + date
    _.each(this.model.get('goals'), g=>{
      var date = new Date(g.date);
      date.setDate(date.getDate()+1)
      dates.push(date.toISOString().slice(0, 10));
      this.dates[''+date.toISOString().slice(0, 10)] = g.steps;
    });
    this.$el.html(this.template(this.model.toJSON()));
      this.$el.find('.goalDates').multiDatesPicker({
        separator: '-',
        dateFormat: 'yy-mm-dd',
      });

      self.setStepsByDate();
      var form = new Form({
        model: this.model,
        fields: [
          {
            attr: 'steps',
            label: 'Stappen per geselecteerde dag',
          }
        ]
      })
      this.$el.find('.stepsForm').append(form.render().el);
      var add = $(Handlebars.templates['layout/button']({
        label: 'Opslaan',
      }));
      this.$el.find('.stepsButton').append(add);
      add.click(()=>{
        if(!this.model.get('steps')){
          swal('Aantal stappen niet ingevuld', 'Vul een geldig aantal stappen in', 'error');
          return;
        }
        //the method to returns the selected dates returns false for none existing dates so we loop through and check
        //if the value exists
        var arr = this.$el.find('.goalDates').multiDatesPicker('getDates');
        _.each(arr,(val,key)=>{
          if(!val){
            delete arr[key];
          } else {
            this.dates[val] = parseInt(this.model.get('steps'));
          }
        });
        //to make sure the values are refreshed we doubleclick a td element which contains an <a>
        //additionally we call the setStepsByDate method to re-set all fields that have steps.
        var tdrefresh = $(this.$el.find('.goalDates').find('td').find('a')[0]).parent();
        // tdrefresh.click();
        tdrefresh.click();
        tdrefresh = $(this.$el.find('.goalDates').find('td').find('a')[0]).parent();
        tdrefresh.click();
        // setTimeout(()=>{
        //   console.log('lololol');
        //   // this.setStepsByDate();
        //   console.log(tdrefresh);
        //   tdrefresh.removeClass('ui-state-highlight ui-datepicker-current-day');
        //   tdrefresh.click();
        // },1000);
        this.setStepsByDate();

        var obj = {
          dates: arr,
          steps: this.model.get('steps'),
          patient_id: this.model.id
        }
        var goal = new Goal(obj);
        goal.save(null,{success:()=>{
          swal((obj.dates.length > 1?'Doelstellingen':'Doelstelling') +' succesvol toegevoegd',undefined,'success');
        }});
      });

    return this;
  }
});

module.exports = HomeView;
