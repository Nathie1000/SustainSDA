var express = require('express');
var router = express.Router();
var knex = require('../../backend/utils/knex.js');
var hash = require('../../backend/utils/password').hash;
var Doctor = require('../../models/doctor');
var User = require('../../models/user');
var isFunction = require('../../backend/utils/utils').isFunction;

router.get('/current',(req,res,next)=>{
  Doctor.getByKeyVal('user_id',req.session.user.id,{success:d=>{
    User.find(d.get('user_id'),{success:u=>{
      d.setKeyVals(u.getAllAttributes());
      req.session.fysio = d.getAllAttributes();
      res.send(d.getAllAttributes());
    }, error:e=>{
      res.send(e);
    }});
  }});
});

router.get('/:id', function(req, res, next) {
  var p = req.params;
  Doctor.find(p.id,{success:d=>{
    User.find(d.get('user_id'),{success:u=>{
      d.set('username', u.get('username'));
      res.send(d.getAllAttributes());
    }});
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

router.get('/searchDoctor/:query', (req,res,next)=>{
  var query = '';
  if(req.params.query !== '-'){query = `and LOWER(firstName) like '%${(req.params.query.toLowerCase())}%'
  || LOWER(lastName) like '%${req.params.query.toLowerCase()}%'`}
  if(!req.session.practice){
    app.runMiddleware('/fysio/practices/current',{});
  }
  knex.raw(`select *, (SELECT u.username from users u where u.id = doctors.user_id) practiceName
  from doctors where practice_id = ${req.session.practice.id} ${query}`)
  .then(arr=>{
    res.send(Doctor.decryptArray(arr[0]));
  })
  .error(e=>{
    console.log(e);
    res.send('error', e);
  });
});

module.exports = router;
