var _ = require('underscore');
var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var Patient = require('../../models/patient');
var Progress = require('../../models/progress');
var Goal = require('../../models/goal');
var User = require('../../models/user');

router.get('/current',(req,res,next)=>{
  // if(req.session.patient){res.send(req.session.patient);return;}
  console.log('userid:');
  console.log(req.session.user.id);
  Patient.getByKeyVal('user_id',req.session.user.id,{success:d=>{
    console.log(d.getAllAttributes());
    User.find(d.get('user_id'),{success:u=>{
      console.log("user found with current patient: ");
      console.log(u);
      d.setKeyVals(u.getAllAttributes());
      req.session.patient = d.getAllAttributes();
      res.send(req.session.patient);
    }, error:e=>{
      res.send(e);
    }});
  }});
});

router.get('/:id', function(req, res, next) {
    var p = req.params;
    Patient.find(p.id, {
        success: d => {
            d.set('name', d.get('firstName') + ' ' + d.get('lastName'));
            Goal.getAllByKeyVal('patient_id', p.id, {
                success: g => {
                    d.set('goals', g);
                    res.send(d.getAllAttributes());
                }
            });
        }
    })
});

router.get('/getChartData/:id', (req, res) => {
    var days = 'Zondag Maandag Dinsdag Woensdag Donderdag Vrijdag Zaterdag'.split(' ');
    var id = req.params.id;
    var data = {};
    knex('progress').select('progress_hour as hour').sum('steps as steps').whereRaw('patient_id = '+id+' and datediff(created_at, CURDATE()) = 0').groupBy('progress_hour').then(d => {
        data.tf = [];
        _.each(d, p => {
            data.tf[p.hour - 1] = p.steps;
        });
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
                    data.week.dates.push(days[new Date(p.created_at).getDay()]);
                    data.week.steps.push(p.steps);
                    data.week.goals.push(p.goal);
                });
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
                        knex('progress').select("steps", "created_at").whereRaw('patient_id = '+id+' and YEAR(created_at) = YEAR(CURDATE()) group by MONTH(created_at)').orderBy('created_at').then(d => {
                          data.year = {};
                          data.year.steps = [];
                          data.year.goals = [];
                          lastSteps = 0;
                          lastGoal = 0;
                          data.year.stepsTotal = [];
                          data.year.goalsTotal = [];
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
                                data.year.steps[p.created_at-1] = (p.steps);
                                data.year.goals[p.created_at-1] = (p.goal);
                            });
                            res.send(data);
                          });
                        });
                    });
                });
            });
        });
    });
});

router.get('/getProgressData/:id', (req, res) => {
    var id = req.params.id;
    var data = {};
    var curDate = (new Date()).toISOString().substring(0, 10);
    Progress.stepsByDay(id, curDate, {
        success: d => {
            data.steps = d;
            Goal.where('date = CURDATE() and patient_id = ' + id, {
                success: d => {
                    data.stepGoal = d.length > 0
                        ? d[0].steps
                        : 'Geen doelstelling voor vandaag.';
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

router.put('/:id', function(req, res, next) {
    var id = req.params.id;
    var data = req.body;
    Patient.update(data, {
        success: d => {
            res.send(d.getAllAttributes());
        }
    });
});

router.post('/', function(req, res, next) {
    var id = req.params.id;
    var data = req.body;
    console.log('posting body');
    console.log(req.body);
    console.log(req.body.toString());
    Patient.create(data, {
        success: d => {
            res.send(d.getAllAttributes());
        }
    });
});

module.exports = router;
