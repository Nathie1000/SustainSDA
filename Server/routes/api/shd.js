var _ = require('underscore');
var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var Patient = require('../../models/patient');
var Progress = require('../../models/progress');
var Goal = require('../../models/goal');
//calculate days in a month
Date.prototype.monthDays= function(){
    var d= new Date(this.getFullYear(), this.getMonth()+1, 0);
    return d.getDate();
}

router.get('/getChartDataHours/:id', (req, res) => {
  console.log('24');
  var id = req.params.id;
  var data = {};
  knex('progress').select('progress_hour as hour').sum('steps as steps').whereRaw('patient_id = '+id+' and datediff(created_at, CURDATE()) = 0').groupBy('progress_hour').then(d => {
      data.steps = [];
      _.each(d, p => {
          data.steps[p.hour - 1] = p.steps;
      });
      _.each(data.steps,st=>{
        if(st === null)
          st = 0;
      });
      for(var i = 0; i< 24;i++){
        if(!data.steps[i] && data.steps[i] !== 0){
          data.steps[i] = 0;
        }
      }
      res.send(data);

    }).catch(e=>{
      console.log(e);
    });

});

router.get('/getChartDataWeek/:id', (req, res) => {
  console.log('week');
  var id = req.params.id;
  var data = {};
  knex('progress').select('created_at').sum('steps as steps').whereRaw('patient_id = '+id+' and created_at >= DATE_ADD(CURDATE(), INTERVAL -6 DAY) and created_at < DATE_ADD(CURDATE(), INTERVAL 1 DAY) group by DAY(created_at)').orderBy('created_at').then(d => {
      data.week = {};
      data.week.dates = [];
      data.week.steps = [];
      data.week.goals = [];
      data.week.stepsTotal = [];
      data.week.goalsTotal = [];
      var lastSteps = 0;
      var lastGoal = 0;
      knex('goals').select("steps", "date").whereRaw('patient_id = '+id+' and date >= DATE_ADD(CURDATE(), INTERVAL -6 DAY) and date < DATE_ADD(CURDATE(), INTERVAL 1 DAY)').orderBy('date').then(g => {
          _.each(d, p => {
              p.created_at = p.created_at.toISOString().slice(0, 10);
              _.each(g, gs => {
                  if (p.created_at === gs.date.toISOString().slice(0, 10)) {
                      p.goal = gs.steps;
                  }
              });
              if (!p.goal)
                  p.goal = 0;
              lastGoal += p.goal;
              lastSteps += p.steps;
              data.week.stepsTotal.push(lastSteps);
              data.week.goalsTotal.push(lastGoal);
              data.week.dates.push(new Date(p.created_at).getDay());
              data.week.steps.push(p.steps);
              data.week.goals.push(p.goal);
          });
          _.each(data.week.stepsTotal,st=>{
            if(st === null)
              st = 0;
          });
          _.each(data.week.goalsTotal,st=>{
            if(st === null)
              st = 0;
          });
          _.each(data.week.steps,st=>{
            if(st === null)
              st = 0;
          });
          _.each(data.week.goals,st=>{
            if(st === null)
              st = 0;
          });
          for(var i = 0; i< 7;i++){
            if(!data.week.goals[i] && data.week.goals[i] !== 0){
              data.week.goals[i] = 0;
            }
            if(!data.week.steps[i] && data.week.steps[i] !== 0){
              data.week.steps[i] = 0;
            }
            if(!data.week.goalsTotal[i] && data.week.goalsTotal[i] !== 0){
              data.week.goalsTotal[i] = 0;
            }
            if(!data.week.stepsTotal[i] && data.week.stepsTotal[i] !== 0){
              data.week.stepsTotal[i] = 0;
            }
          }
          res.send(data.week);
      });
    });
});

router.get('/getChartDataMonth/:id', (req, res) => {
  console.log('maand');
  var id = req.params.id;
  var data = {};
  knex('progress').select("steps", "created_at").whereRaw('patient_id = '+id+' and MONTH(created_at) = MONTH(CURDATE()) group by DAY(created_at)').orderBy('created_at').then(d => {
      data.month = {};
      data.month.dates = [];
      data.month.steps = [];
      data.month.goals = [];
      lastSteps = 0;
      lastGoal = 0;
      data.month.stepsTotal = [];
      data.month.goalsTotal = [];
      knex('goals').select("steps", "date").whereRaw('patient_id = '+id+' and MONTH(date) = MONTH(CURDATE())').orderBy('date').then(g => {
        _.each(d, p => {
            p.created_at = p.created_at.toISOString().slice(0, 10);
            _.each(g, gs => {
                if (p.created_at === gs.date.toISOString().slice(0, 10)) {
                    p.goal = gs.steps;
                }
            });
            if (!p.goal)
                p.goal = 0;
            lastGoal += p.goal;
            lastSteps += p.steps;
            data.month.stepsTotal.push(lastSteps);
            data.month.goalsTotal.push(lastGoal);
            data.month.dates.push(p.created_at);
            data.month.steps.push(p.steps);
            data.month.goals.push(p.goal);
        });
        _.each(data.month.stepsTotal,st=>{
          if(st === null)
            st = 0;
        });
        _.each(data.month.goalsTotal,st=>{
          if(st === null)
            st = 0;
        });
        _.each(data.month.steps,st=>{
          if(st === null)
            st = 0;
        });
        _.each(data.month.goals,st=>{
          if(st === null)
            st = 0;
        });
        for(var i = 0; i< (new Date).monthDays();i++){
          if(!data.month.goals[i] && data.month.goals[i] !== 0){
            data.month.goals[i] = 0;
          }
          if(!data.month.steps[i] && data.month.steps[i] !== 0){
            data.month.steps[i] = 0;
          }
          if(!data.month.goalsTotal[i] && data.month.goalsTotal[i] !== 0){
            data.month.goalsTotal[i] = 0;
          }
          if(!data.month.stepsTotal[i] && data.month.stepsTotal[i] !== 0){
            data.month.stepsTotal[i] = 0;
          }
        }
      res.send(data.month);
    });
  });
});

router.get('/getChartDataYear/:id', (req, res) => {
  console.log('jaar');
  var id = req.params.id;
  var data = {};
  knex('progress').select("steps", "created_at").whereRaw('patient_id = '+id+' and YEAR(created_at) = YEAR(CURDATE()) group by MONTH(created_at)').orderBy('created_at').then(d => {
    data.year = {};
    data.year.steps = [];
    data.year.goals = [];
    lastSteps = 0;
    lastGoal = 0;
    data.year.stepsTotal = [];
    data.year.goalsTotal = [];
    data.year.months = [];
    knex.raw('select sum(steps) as steps,MONTH(date) as month from goals where YEAR(date) = YEAR(CURDATE()) group by MONTH(date)').then(g => {
      _.each(d, p => {
          p.created_at = p.created_at.getMonth()+1;
          _.each(g[0], gs => {
              if (p.created_at === gs.month) {
                  p.goal = gs.steps;
              }
          });
          if (!p.goal)
              p.goal = 0;
          lastGoal += p.goal;
          lastSteps += p.steps;
          data.year.stepsTotal[p.created_at-1] = (lastSteps);
          data.year.goalsTotal[p.created_at-1] = (lastGoal);
          data.year.months.push(p.created_at-1);
          data.year.steps[p.created_at-1] = (p.steps);
          data.year.goals[p.created_at-1] = (p.goal);
      });
      _.each(data.year.stepsTotal,st=>{
        if(st === null)
          st = 0;
      });
      _.each(data.year.goalsTotal,st=>{
        if(st === null)
          st = 0;
      });
      _.each(data.year.steps,st=>{
        if(st === null)
          st = 0;
      });
      _.each(data.year.goals,st=>{
        if(st === null)
          st = 0;
      });
      for(var i = 0; i< 12;i++){
        if(!data.year.goals[i] && data.year.goals[i] !== 0){
          data.year.goals[i] = 0;
        }
        if(!data.year.steps[i] && data.year.steps[i] !== 0){
          data.year.steps[i] = 0;
        }
        if(!data.year.goalsTotal[i] && data.year.goalsTotal[i] !== 0){
          data.year.goalsTotal[i] = 0;
        }
        if(!data.year.stepsTotal[i] && data.year.stepsTotal[i] !== 0){
          data.year.stepsTotal[i] = 0;
        }
      }
      res.send(data.year);
    });
  });
});

router.get('/getProgressData/:id', (req, res) => {
    var id = req.params.id;
    // console.log('progress',id);
    var data = {};
    var curDate = (new Date()).toISOString().substring(0, 10);
    Progress.stepsByDay(id, curDate, {
        success: d => {
            data.steps = d;
            Goal.where('date = CURDATE() and patient_id = ' + id, {
                success: d => {
                    data.stepGoal = d.length > 0
                        ? d[0].steps
                        : 0;
                    Goal.where('"' + curDate + '" > date and achieved = 0 and patient_id = ' + id, {
                        success: d => {
                            _.each(d, g => {
                                Progress.stepsByDay(id, (new Date(g.date)).toISOString().substring(0, 10), {
                                    success: d => {
                                        g.achieved = (d >= g.steps
                                            ? 1
                                            : 2);
                                        Goal.update(g);
                                    }
                                });
                            });
                            Goal.countWhere('patient_id = ' + id, {
                                success: c => {
                                    data.totalGoals = c;
                                    Progress.where('patient_id = ' + id, {
                                        success: d => {
                                            data.totalSteps = 0;
                                            _.each(d, p => {
                                                data.totalSteps += p.steps;
                                            });
                                            Goal.where('achieved = 1 and patient_id = ' + id, {
                                                success: d => {
                                                    data.goalsCompleted = d.length;
                                                    if (data.steps >= data.stepGoal)
                                                        data.goalsCompleted++;
                                                    res.send(data);
                                                }
                                            });
                                        }
                                    });
                                }
                            });
                        }
                    });
                }
            });
        }
    });
});


router.get('/patient/:shdNumber',(req,res)=>{
  var param = req.params;
  Patient.getByKeyVal('SHDNumber', param.shdNumber,{success:d=>{
    res.send(d.getAllAttributes());
  }})
});

module.exports = router;
