"use strict";
var Model = require('./model');
class Patient extends Model{

  static getDBName(){
    return 'patients';
  }

}
Patient.fillable = [
  'firstName',
  'lastName',
  'user_id',
  'doctor_id',
  'emergencyNumber',
  'beltNumber',
  'SHDNumber',
  'battery',
];
Patient.encryptable = [
  'firstName',
  'lastName',
  'emergencyNumber',
  'beltNumber',
  'SHDNumber',
];
module.exports = Patient;
