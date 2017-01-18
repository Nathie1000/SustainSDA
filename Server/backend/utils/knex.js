var Knex = require('knex');
var config = require('../../knex.json');
module.exports = Knex(config);
