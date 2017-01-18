var express = require('express');
var router = express.Router();
var patients = require('./patients');
var practices = require('./practices');
var doctors = require('./doctors');

router.use('/patients', patients);
router.use('/practices', practices);
router.use('/doctors', doctors);


module.exports = router;
