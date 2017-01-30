"use strict";
var Model = require('./model');
var _ = require('underscore');
var isFunction = require('../backend/utils/utils').isFunction;
class Progress extends Model{

  static getDBName(){
    return 'progress';
  }

  static stepsByDay(id,date,options){
    if(!options) return null;
    Progress.where('DATEDIFF(created_at,"'+date+'") = 0 and patient_id = '+id,{
      success:d=>{
        var steps = 0;
        _.each(d, p=>{
          steps += p.steps;
        });
        if(isFunction(options.success)){
          options.success(steps);
        }
      },
      error:e=>{
        if(isFunction(options.error)){
          options.error(e);
        }
      }
    });
  }

}
Progress.fillable = [
  'patient_id',
  'steps',
  'progress_hour',
];

module.exports = Progress;
