var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var hash = require('../../backend/utils/password').hash;
var isFunction = require('../../backend/utils/utils').isFunction;
var Patient = require('../../models/patient');
var User = require('../../models/user');
var _ = require('underscore');

router.post('/',function(req,res,next){
  var id = req.params.id;
  var data = req.body;
  Patient.create(data,{success:d=>{
    res.send(d.getAllAttributes());
  }});
});

router.get('/:id', function(req, res, next) {
  var p = req.params;
  Patient.find(p.id,{success:d=>{
    User.find(d.get('user_id'),{success:u=>{
      d.set('username',u.get('username'));
      res.send(d.getAllAttributes());
    }});
  }});
});

router.put('/:id',function(req,res,next){
  var id = req.params.id;
  var data = req.body;
  Patient.update(data,{success:d=>{
    res.send(d.getAllAttributes());
  }});
});

router.get('/searchPatient/:query/:pid', (req,res,next)=>{
  var query = '';
  if(req.params.query !== '-'){query = `and LOWER(firstName) like '%${(req.params.query.toLowerCase())}%'
  || LOWER(lastName) like '%${req.params.query.toLowerCase()}%'`}
  knex.raw(`select *, (select firstName from doctors d where d.id =  doctor_id) as doctorFName, (select lastName from doctors d where d.id =  doctor_id) as doctorLName
  from patients where doctor_id in (select d.id from doctors d where d.practice_id = ${req.params.pid}) ${query}`)
  .then(arr=>{
    var ret = Patient.decryptArray(arr[0]);
    _.each(ret,r=>{
      r.doctorName = Patient.decryptValue(r.doctorFName) + ' ' + Patient.decryptValue(r.doctorLName);
    });
    res.send(ret);
  })
  .error(e=>{
    console.log(e);
    res.send('error', e);
  });
});

module.exports = router;
