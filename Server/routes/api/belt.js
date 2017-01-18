var _ = require('underscore');
var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var Patient = require('../../models/patient');
var Progress = require('../../models/progress');

router.post('/sendPatientData',(req,res)=>{
  var data = req.body;
  console.log(data);
  Patient.getByKeyVal('beltNumber',data.beltNumber,{success:d=>{
    console.log(1);
    d.set('battery', data.battery);
    d.save();
    console.log(2);
    var p = new Progress();
    p.set('patient_id', d.get('id'));
    console.log(data.steps);
    p.set('steps',data.steps);
    p.set('progress_hour',(new Date()).getHours());
    p.save();
    console.log(3);
    res.send(p.getAllAttributes());
  }, error:e=>{
    res.status(500);
    // res.send(e);
  }});


});



module.exports = router;
