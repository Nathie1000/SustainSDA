var express = require('express');
var router = express.Router();
var Practice = require('../../models/practice');
var knex = require('../../backend/utils/knex.js');
var _ = require('underscore');

router.get('/:id', function(req, res, next) {
  var p = req.params;
  Practice.find(p.id,{success:d=>{
    res.send(d.getAllAttributes());
  }});
});

router.put('/:id',function(req,res,next){
  var data = req.body;
  Practice.update(data,{success:d=>{
    res.send(d.getAllAttributes());
  }});
});

router.post('/', (req,res,next)=>{
  var data = req.body;
  Practice.create(data,{success:d=>{
    res.send(d);
  }, error:e=>{
    res.send(e);
  }});
});

router.get('/searchPractice/:query', (req,res,next)=>{
  var query = '';
  // if(req.params.query !== '-')
  //   req.params.query = Practice.encryptValue(req.params.query);
  // if(req.params.query !== '-'){query = `where LOWER(name) like '%${(req.params.query.toLowerCase())}%'
  // or LOWER(city) like '%${req.params.query.toLowerCase()}%'`}
  // console.log(query);
  knex.raw(`select * from practices`)
  .then(arr=>{
    _.each(arr[0],a=>{
      a = Practice.decryptKeys(a);
    });
    if(req.params.query !== '-'){
      var ret = [];
      _.each(arr[0],a=>{
        if(a.name.toLowerCase().contains(req.params.query) || a.city.toLowerCase().contains(req.params.query)){
          ret.push(a);
        }
      })
      res.send(ret);
    } else {
      res.send(arr[0]);
    }
  })
  .error(e=>{
    console.log(e);
    res.send('error', e);
  });
});

module.exports = router;
