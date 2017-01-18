import Backbone from 'backbone';
import _ from 'underscore';

if (!Array.prototype.last){
    Array.prototype.last = function(){
        return this[this.length - 1];
    };
}

Array.prototype.contains = function(obj) {
    var i = this.length;
    while (i--) {
        if (this[i] === obj) {
            return true;
        }
    }
    return false;
}; //

jQuery.fn.reverse = [].reverse;

Array.prototype.removeKey = function(key){
  this.splice(key, 1);
}

String.prototype.count=function(s1) {
    return (this.length - this.replace(new RegExp(s1,"g"), '').length) / s1.length;
};

String.prototype.contains=function(string){
    return this.indexOf(string) !== -1;
};

export function generateRandomString(len)
{
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    for( var i=0; i < len; i++ )
        text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
}

//Only returns the arguments of the last model saved, this might need changing someday if required.
export function saveModels(models,options){
  models[0].save(
    {success:()=>{
      delete models[0];
      if(models.length>0){
        saveModels(models,options);
      } else {
        if(options.success && isFunction(options.success)){
          options.success.apply(arguments);
        }
      }
    },
    error:()=>{
        if(options.error && isFunction(options.error)){
          options.error.apply(arguments);
        }
    }
  });
}

// Backbone.Model.prototype.oldValidate = Backbone.Model.prototype.validate;
// Backbone.Model.prototype.oldSave = Backbone.Model.prototype.save;
//
// Backbone.Model.prototype.validate = (a,b)=>{
//   var errors = this.oldValidate(a,b);
//   if(!errors) return true;
//   return errors;
// };
//
// Backbone.Model.prototype.save = (a,b)=>{
//   if(this.validate() === true){
//     this.oldSave(a,b);
//   }
// };

Backbone.Model.prototype.validateSwal = function(){
  var errors = this.validate();
  if(errors === true) return true;
  console.log(errors);
  var msg = '';
  _.each(errors,val=>{
    msg += val + '\n';
  });
  swal('De volgende fouten zijn gevonden.', msg, 'error');
};

export function validateSwalModels(models){
var msg = '';
  _.each(models,m=>{
    var errors = m.validate();
    _.each(errors, val=>{
      msg += val + '\n';
    });
  });
  if(msg === '') return true;
  swal('De volgende fouten zijn gevonden.', msg, 'error');
}

export function getMonthFromString(mon){
   return new Date(Date.parse(mon +" 1, 2012")).getMonth()+1
}

export function validURL(str) {
  var pattern = new RegExp('(http|ftp|https)://[a-z0-9\-_]+(\.[a-z0-9\-_]+)+([a-z0-9\-\.,@\?^=%&;:/~\+#]*[a-z0-9\-@\?^=%&;/~\+#])?', 'i'); // fragment locator
  console.log(pattern.test(str));
  return pattern.test(str);
}

export function getBaseURI(){
  return location.origin + location.pathname;
}
export function getFullURI(){
  return location.href;
}

export function isFunction(functionToCheck){
 var getType = {};
 return functionToCheck && getType.toString.call(functionToCheck) === '[object Function]';
}

export function removeElementFromArray(obj, arr){
  if(!obj || !arr) return arr;
  var index = arr.indexOf(obj);
  while(index> -1){
    arr.splice(index,1);
    console.log('removing at index ' + index);
    index = arr.indexOf(obj);
  }
  return arr;
}

export function arrToObj(array){
    var thisEleObj = new Object();
    if(typeof array == "object"){
        for(var i in array){
            var thisEle = app.arrToObj(array[i]);
            thisEleObj[i] = thisEle;
        }
    }else {
        thisEleObj = array;
    }
    return thisEleObj;
}

export function getStackTrace() {

  var stack;

  try {
    throw new Error('');
  }
  catch (error) {
    stack = error.stack || '';
  }

  stack = stack.split('\n').map(function (line) { return line.trim(); });
  return stack.splice(stack[0] == 'Error' ? 2 : 1);
}
