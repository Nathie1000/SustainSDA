var _ = require('underscore');
var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var Patient = require('../../models/patient');
var Progress = require('../../models/progress');

router.post('/sendPatientData',(req,res)=>{
  var data = req.body;
  Patient.getByKeyVal('beltNumber',data.ccid,{success:d=>{
    d.set('battery', data.battery);
    d.save();
    var p = new Progress();
    p.set('patient_id', d.get('id'));
    p.set('steps',data.steps);
    p.set('progress_hour',(new Date()).getHours());
    p.save();
    res.send("success");
  }, error:e=>{
    console.log(e);
    res.send(e);
  }});


});



module.exports = router;
