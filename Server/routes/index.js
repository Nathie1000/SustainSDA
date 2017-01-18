var express = require('express');
var router = express.Router();

var fs = require('fs');
var base64 = require('base64-url');

var MCrypt = require('mcrypt').MCrypt;
var rijnEcb = new MCrypt('rijndael-128', 'ecb');


/* GET home page. */
router.get('/', function(req, res, next) {
  if(req.session.user.role === app.ROLE_DOCTOR){
    res.render('doctor');
  } else if(req.session.user.role === app.ROLE_ADMIN) {
    res.render('admin');
  } else if(req.session.user.role === app.ROLE_PATIENT) {
    res.render('patient');
  } else {
    res.render('404');
    console.log('user role not doctor: ' + req.session.user.role + ' required: '+app.ROLE_DOCTOR);
  }
});

function processInput ( text )
{
  var date =new Date().toISOString().
  replace(/T/, ' ').split(' ')[0];
  fs.open('./log'+date+'.txt', 'a', 666, function( e, id ) {
   fs.write( id, text + "\n", null, 'utf8', function(){
    fs.close(id, function(){
     console.log('file is updated');
    });
   });
  });
}

router.post('/send',(req,res,next)=>{
  if(!app.sendData)app.sendData=[];
  app.sendData.push(req.body);
  var time = new Date().toISOString().replace(/T/, ' ').split('.')[0];
  console.log(req.body);
  processInput(time + ': '+JSON.stringify(req.body));
  res.send('a');
});

router.get('/data',(req,res,next)=>{
  var val = "admin";
  var key = "jnjFNiETFCWXgFjH";
  rijnEcb.open(key);

  var decrypt = rijnEcb.encrypt(val).toString('base64');
  console.log(decrypt);
  res.send(decrypt.trim());
});

module.exports = router;
