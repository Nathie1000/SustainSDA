var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var Goal = require('../../models/goal');

router.get('/:id', function(req, res, next) {
  var p = req.params;
  knex.raw(`select * from progress where user_id = ${p.id}`)
  .then(arr=>{
    var p = arr[0];
    res.send(p);
  }).error(e=>{
    res.send('error', e);
  });
});

router.post('/',function(req,res,next){
  var p = req.body;
  var data = [];
  for(var i in p.dates){
    if(p.dates[i])
      data.push({
        patient_id: p.patient_id,
        steps: p.steps,
        date: p.dates[i]
      });
  }
  knex('goals').whereIn('date',p.dates).where('patient_id', p.patient_id).del().then(()=>{
    knex('goals').insert(data).then(arr=>{
      res.type('json');
      res.send(JSON.stringify(data));
    }).error(e=>{
      res.send(e);
    }).catch(function(err) {
      res.send(err);
    });
  });

});

router.put('/:id',function(req,res,next){
  var id = req.params.id;
  var p = req.body;
  var vals = '';
  for(var i in p){
    if(typeof p[i] === 'string' || p[i] instanceof String)
      vals += i + '= "' + p[i] + '",';
    else
      vals += i + '= ' + p[i] + ',';
  }
  vals = vals.replace(/(^,)|(,$)/g, "");
  knex.raw(`update patients set ${vals} where id = ${id}`).then(arr=>{
    res.type('json');
    res.send(JSON.stringify(req.body));
  }).error(e=>{
    res.send(e);
  });
});

module.exports = router;
