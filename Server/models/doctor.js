"use strict";
var Model = require('./model');
class Doctor extends Model{

  static getDBName(){
    return 'doctors';
  }

}
Doctor.fillable = [
  'user_id',
  'practice_id',
  'firstName',
  'lastName',
];
Doctor.encryptable = [
  'firstName',
  'lastName',
];
module.exports = Doctor;
