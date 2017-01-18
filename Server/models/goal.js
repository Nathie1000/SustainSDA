"use strict";
var isFunction = require('../backend/utils/utils').isFunction;
var Model = require('./model');
class Goal extends Model{

  static getDBName(){
    return 'goals';
  }

}
Goal.fillable = [
  'patient_id',
  'steps',
  'date',
  'achieved', //0=unknown, 1=true, 2=false
];
Goal.encryptable = [
  'steps',
];
module.exports = Goal;
