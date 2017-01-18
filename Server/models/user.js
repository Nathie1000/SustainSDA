"use strict";
var Model = require('./model');
class User extends Model{

  static getDBName(){
    return 'users';
  }
}
User.fillable = [
  'username',
  'password',
  'salt',
  'role',
];
User.encryptable = [
  'username',
];
User.omit = [
  'password',
  'salt',
]
module.exports = User;
