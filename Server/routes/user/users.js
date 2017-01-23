var express = require('express');
var router = express.Router();
var hash = require('../../backend/utils/password').hash;
var User = require('../../models/user');
var knex = require('../../backend/utils/knex.js');
/* GET users listing. */
router.get('/', function(req, res, next) {
    knex.raw('select * from users')
        .then(arr => {
            res.send(arr[0]);
        }).
    error(e => {
        console.log(arguments);
        res.send('error', e);
    });
});

function updateUser(data,id,res){
  data.id = id;
  console.log(55555,data);
  User.update(data,{success:d=>{
    // res.type('json');
    res.send(d.getAllAttributes());
  },error:e=>{
    res.send(e);
  }});
}

router.post('/', (req,res,next)=>{
  console.log('we maken een nieuwe user aan');
  var data = req.body;
  hash(data.password, (err, pass,salt) => {
    data.password = pass;
    data.salt = salt;
    User.create(data,{success:d=>{
      delete d['salt'];
      delete d['password'];
      res.send(d);
    }});
  });
});

router.put('/:id', function(req, res, next) {
    var id = req.params.id;
    var p = req.body;
    console.log(123333,p);
    var data = {
        username: req.body.username
    };
    if (req.body.password && req.body.password !== 'undefined') {
      knex('users').select('salt').where('id',id).then(arr=>{
        hash(req.body.password,arr[0].salt, (err, pass) => {
          data.password = pass;
          updateUser(data,id,res);
        });
      });
    } else
      updateUser(data,id,res);

});

router.get('/current', function(req, res, next) {
    console.log(req.session.user);
    res.send(req.session.user || null);
});

module.exports = router;
