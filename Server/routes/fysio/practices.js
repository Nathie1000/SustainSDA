var express = require('express');
var router = express.Router();
var Practice = require('../../models/practice');
var knex = require('../../backend/utils/knex.js');

router.get('/current',(req,res,next)=>{
  Practice.find(req.session.fysio.practice_id,{success: d=>{
    req.session.practice = d.getAllAttributes();
    res.send(d.getAllAttributes());
  }});
});

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

module.exports = router;
