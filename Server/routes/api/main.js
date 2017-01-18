var express = require('express');
var router = express.Router();
var belt = require('./belt');
var shd = require('./shd');

router.use('/belt', belt);
router.use('/shd', shd);


module.exports = router;
