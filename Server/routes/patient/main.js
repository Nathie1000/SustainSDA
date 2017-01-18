var express = require('express');
var router = express.Router();
var patients = require('./patients');
// var progress = require('./progress');

router.use('/patients', patients);


module.exports = router;
