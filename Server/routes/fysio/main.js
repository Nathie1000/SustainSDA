var express = require('express');
var router = express.Router();
var patients = require('./patients');
var goals = require('./goals');
var fysio = require('./doctors');
var practices = require('./practices');
// var progress = require('./progress');

router.use('/patients', patients);
router.use('/goals', goals);
router.use('/doctors', fysio);
router.use('/practices', practices);
// router.use('/progress', progress);


module.exports = router;
