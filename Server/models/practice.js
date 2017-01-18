"use strict";
var Model = require('./model');
class Practice extends Model{

  static getDBName(){
    return 'practices';
  }

}
Practice.fillable = [
  'name',
  'city',
];
Practice.encryptable = [
  'name',
  'city',
];
module.exports = Practice;
