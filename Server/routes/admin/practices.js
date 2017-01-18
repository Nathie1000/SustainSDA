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
  }});
});

router.get('/searchPractice/:query', (req,res,next)=>{
  var query = '';
  if(req.params.query !== '-'){query = `where LOWER(name) like '%${(req.params.query.toLowerCase())}%'
  or LOWER(city) like '%${req.params.query.toLowerCase()}%'`}
  knex.raw(`select * from practices ${query}`)
  .then(arr=>{
    _.each(arr[0],a=>{
      a = Practice.decryptKeys(a);
    })
    res.send(arr[0]);
  })
  .error(e=>{
    console.log(e);
    res.send('error', e);
  });
});

module.exports = router;
