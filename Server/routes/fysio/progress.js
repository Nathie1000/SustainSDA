var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var Progress = require('../../models/progress');

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
  p.progress_hour = new Date().getHours();
  Progress.create(p,{success:d=>{
    res.send(d.getAllAttributes());
  }});

  //
  res.type('json');
  res.send(JSON.stringify(req.body));
  return;
  var vals = '';
  $keys =
  for(var d in p.dates){
    vals += `(${p.patient_id},${p.}`
  }
  vals = vals.replace(/(^,)|(,$)/g, "");
  knex.raw(`insert into progress (${keys}) values ${vals}`).then(arr=>{
    res.type('json');
    res.send(JSON.stringify(req.body));
  }).error(e=>{
    res.send(e);
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
