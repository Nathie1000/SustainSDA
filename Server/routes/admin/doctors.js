var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var hash = require('../../backend/utils/password').hash;
var Doctor = require('../../models/doctor');
var User = require('../../models/user');
var isFunction = require('../../backend/utils/utils').isFunction;

router.get('/:id', function(req, res, next) {
  var p = req.params;
  Doctor.find(p.id,{success:d=>{
    User.find(d.get('user_id'),{success:u=>{
      d.set('username', u.get('username'));
      res.send(d.getAllAttributes());
    }});
  }});
});

router.get('/allDoctorsByPractice/:id', (req,res,next)=>{
  var p = req.params;
  Doctor.where('practice_id = ' +p.id,{success:d=>{
    res.send(d);
  }});
});

router.post('/', (req,res,next)=>{
  var data = req.body;
  Doctor.create(data,{success:d=>{
    res.send(d.getAllAttributes());
  }});
});

router.put('/:id',function(req,res,next){
  var id = req.params.id;
  var data = req.body;
  Doctor.update(data,{success:d=>{
    res.send(d.getAllAttributes());
  }});
});

router.get('/searchDoctor/:query/:pid', (req,res,next)=>{
  var query = '';
  if(req.params.query !== '-'){query = `and LOWER(firstName) like '%${(req.params.query.toLowerCase())}%'
  || LOWER(lastName) like '%${req.params.query.toLowerCase()}%'`}
  knex.raw(`select *, (SELECT p.name from practices p where p.id = ${req.params.pid}) practiceName
  from doctors where practice_id = ${req.params.pid} ${query}`)
  .then(arr=>{
    res.send(Doctor.decryptArray(arr[0]));
  })
  .error(e=>{
    console.log(e);
    res.send('error', e);
  });
});

module.exports = router;
