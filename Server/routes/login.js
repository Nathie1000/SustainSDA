var express = require('express');
var router = express.Router();
var hash = require('../backend/utils/password').hash;
var User = require('../models/user');
var knex = require('../backend/utils/knex.js');

function authenticate(username, pass, success){
  if(!username) {username = '';success(null,'Voer een gebruikersnaam in.');}
  knex.raw('select * from users where username ="'+User.encryptValue(username)+'"')
    .then(arr=>{
      var user = arr[0][0];
      console.log('user: ');
      console.log(user);
      if(!user){success(null,'Incorrecte gebruikersnaam of wachtwoord');return;}
      hash(pass,user.salt,
        (err,passw)=>{
          if(err){success(null,err);return}
          console.log(passw);
          console.log(user.password);
          if(passw === user.password)
            success(user);
          else
            success(null,'Incorrecte gebruikersnaam of wachtwoord!');
        });
    })
    .error(e=>{
      console.log(e);
      success(false,null,e);
    });
}

function logout(req){
  req.session.user = null;
  req.session.fysio = null;
  req.session.practice = null;
}
router.get('/logout',function(req,res,next){
  logout(req);
  res.render('login', { title: 'Login Sustain' });
});



/* GET home page. */
router.get('/', function(req, res, next) {
  logout(req);
  res.render('login', { title: 'Login Sustain' });
});
router.post('/', function(req, res, next) {
  authenticate(req.body.username,req.body.password,(model,error)=>{
    console.log('authenticating');
    if(error){res.render('login',{title: 'Login Sustain', error: error});return;}
    if(model){
      req.session.user = model;
      if(model.role === app.ROLE_DOCTOR){
        knex('doctors').where('user_id', model.id).then(arr=>{
          req.session.fysio = arr[0];
          res.redirect('/#home');
        });
      } else {
        res.redirect('/#home')
      }
    }
  });
  // res.send({req: req, res: res});
});

module.exports = router;
