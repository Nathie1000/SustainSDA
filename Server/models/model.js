"use strict";
var knex = require('../backend/utils/knex.js');
var _ = require('underscore');
var isFunction = require('../backend/utils/utils').isFunction;
var base64 = require('base64-url');
var MCrypt = require('mcrypt').MCrypt;
var secretKey = 'jnjFNiETFCWXgFjH';
var rijnEcb = new MCrypt('rijndael-128', 'ecb');
rijnEcb.open(secretKey);
/*
* Model.fillable
*   This static variable contains all values which are being saved into the database.
*   The values 'id', 'created_at', 'updated_at' are automaticly created so they do not need to be added to this array
*
* Model.omit
*   This static variable contains all values which are not returned when calling getAllAttributes()
*/

/**
 * PRIVATE function used to convert binary to asci
 * @param  {String} str
 * @return {base64 string}
 */
function btoa(str) {
  var buffer;

  if (str instanceof Buffer) {
    buffer = str;
  } else {
    buffer = new Buffer(str.toString(), 'binary');
  }

  return buffer.toString('base64');
}
/**
 * PRIVATE function same as btoa only the other way around.
 */
function atob(str) {
  return new Buffer(str, 'base64').toString('binary');
}
/**
 * Checks if a string is base64, Note some normal strings like 'q' are recognized as base64, so this isn't 100% perfect
 * @param  {[type]}  str [description]
 * @return {Boolean}     [description]
 */
function isBase64(str) {
    try {
        return btoa(atob(str)) == str;
    } catch (err) {
        return false;
    }
}



class Model {
  /**
   * returns the database name of the model, each model has it's own implementation of this method.m
   */
  static getDBName(){
    return 'no name';
  }
  /**
   * Initializes the attributes if given, otherwise this.values becomes an empty object
   */
  constructor(attributes){
    if(this.constructor.isObject(attributes))
      this.values = attributes;
    else
      this.values= {};
  }
  /**
   * Returns a value based on the given key
   * @param  {String}  key [The string representation of a attribute key]
   * @return {String}     [Value of the corrosponding key]
   */
  get(key){
    return this.values[key];
  }
  /**
   * Sets multiple values to the specified key
   * @param  {JSON}  keyvals [JSON object with the key as name of the property and a value as value of the property]
   * @param optional  {Boolean}  overwrite [Deterines if a key will be overwritten]
   * @param  optional {int}  id [Determines if an id will be set]
   * @return {Object}     [Attributes of the model object]
   */
  setKeyVals(keyvals,overwrite,id){
    overwrite = (typeof overwrite === 'undefined')?true:overwrite;
    id = (typeof id === 'undefined')?false:id;
    _.each(keyvals,(val,key)=>{
      if((this.values[key] && overwrite) || !this.values[key]){
        if((key === 'id' && id) || key !== 'id'){
          this.set(key,val);
        }
      }
    });
    return this.values;
  }
  /**
   * Sets the value to a specific key.
   * @param  {String}  key [String representation of an attribute key]
   * @param  {mixed}  val [Value of a specific key]
   * @return {mixed}     [The attribute corrosponding to the specified key]
   */
  set(key,val){
    this.values[key] = val;
    return this.values[key];
  }
  /**
   * Returns all attribute of the model object, omits values that need to be omitted.
   * @return {Object}     [The attributes of the model object]
   */
  getAllAttributes(){
    if(!this.constructor.omit) return this.values[0] || this.values;
    var arr = {};
    _.each(this.values[0] || this.values,(val,key)=>{
      if(!this.constructor.isOmmitted(key)){
        arr[key] = val;
      }
    });
    return arr;
  }
  /**
   * Encrypts all encryptable values of the current model object.
   * @return {Object}     [The model instance]
   */
  encrypt(){
    if(this.values[0]) this.values = this.values[0];
    this.values = this.constructor.encryptKeys(this.values);
    return this;
  }

  /**
  * Decrypts all encryptable values of the current model object.
  * @return {Object}     [The model instance]
  */
  decrypt(){
    if(this.values[0]) this.values = this.values[0];
    this.values = this.constructor.decryptKeys(this.values);
    return this;
  }
  /**
   * Decrypts an array of models
   * @param  {Array}  arr [An array containing models]
   * @return {Array}     [The decrypt array containg models]
   */
  static decryptArray(arr){
    console.log(arr);
    _.each(arr,val=>{
      console.log(val);
      val = this.decryptKeys(val);
    });
    return arr;
  }

  /**
  * Encrypts an array of keys and values
  * @param  {Array}  arr [An array containing all attributes of a model]
  * @return {Array}     [The encrypt array the attributes of a model]
  */
  static encryptKeys(arr){
    _.each(arr,(val,key)=>{
      if(this.isEncryptable(key) && val){
        arr[key] = rijnEcb.encrypt(''+val).toString('base64');
      }
    });
    return arr;
  }

  /**
  * Encrypts a value
  * @param  {mixed}  val [The value which will be encrypted]
  * @return {String}     [The encrypted value]
  */
  static encryptValue(val){
    if(!val) return "Cannot encrypt value";
    return rijnEcb.encrypt(''+val).toString('base64');
  }
  /**
  * Decrypts a value
  * @param  {mixed}  val [The value which will be decrypted]
  * @return {String}     [The decrypted value]
  */
  static decryptValue(val){
    if(!val) return val;
    if(!isBase64(val)) return "Cannot decrypt value is not valid base64";
    val = rijnEcb.decrypt(new Buffer(''+val, 'base64'));
    val = val.toString('base64');
    val = base64.decode(val);
    if(typeof val === 'string'){
      while(val.indexOf('\0') > -1){
        val = val.replace('\0', '');
      }
    }
    return val;
  }

  /**
  * Decrypts an array of keys and values
  * @param  {Array}  arr [An array containing all attributes of a model]
  * @return {Array}     [The decrypt attributes]
  */
  static decryptKeys(arr){
    _.each(arr,(val,key)=>{
      try{
        if(val && this.isEncryptable(key) && isBase64(val)){
          arr[key] = rijnEcb.decrypt(new Buffer(''+val, 'base64'));
          arr[key] = arr[key].toString('base64');
          arr[key] = base64.decode(arr[key]);
          if(typeof arr[key] === 'string'){
            while(arr[key].indexOf('\0') > -1){
              arr[key] = arr[key].replace('\0', '');
            }
          }
        }
      } catch(e){
        console.log(val);
        console.log(e);
      }
    });
    return arr;
  }
  /**
  * Executes a where query on the models database tabel.
  * @param  {String}  where [The where part of a query]
  * @param  {Object}  options [Can contain a success call with the returned query results and/or the error call with the error]
  */
  static where(where, options){
    knex(this.getDBName()).select().whereRaw(where).then(d=>{
      if(options && isFunction(options.success)){
        options.success(d);
      }
    }).error(e=>{
      if(options && isFunction(options.error)){
        options.error(e);
      }
    });
  }
  /**
  * Executes a count where query on the models database tabel.
  * @param  {String}  where [The where part of a query]
  * @param  {Object}  options [Can contain a success call with the returned query results and/or the error call with the error]
  */
  static countWhere(where, options){
    knex(this.getDBName()).count('* as c').whereRaw(where).then(d=>{
      if(options && isFunction(options.success)){
        options.success(d[0].c);
      }
    }).error(e=>{
      if(options && isFunction(options.error)){
        options.error(e);
      }
    });
  }
  /**
  * Checks if key is encryptable
  * @param  {String}  key [The string representation of an attribute key]
  * @return {Boolean}     [Returns true if the key is encryptable, false if not]
  */
  static isEncryptable(key){
    if(!this.encryptable) return false
    return this.encryptable.indexOf(key) !== -1;
  }
  /**
  * Checks if key is fillable
  * @param  {String}  key [The string representation of an attribute key]
  * @return {Boolean}     [Returns true if the key is fillable, false if not]
  */
  static isFillable(key){
    return this.fillable.indexOf(key) !== -1;
  }
  /**
  * Checks if key is omitted
  * @param  {String}  key [The string representation of an attribute key]
  * @return {Boolean}     [Returns true if the key is omitted, false if not]
  */
  static isOmmitted(key){
    return this.omit.indexOf(key) !== -1;
  }

  /**
  * PRIVATE function to check if something is an object
  */
  static isObject(data,options){
    if(typeof data !== 'object' && !Array.isArray(data)){
      if(options && isFunction(options.error)){
        options.error({error: 'Attributes must be an object', data: data});
      }
      console.error('Attributes must be an object');
      console.error(data)
      return false;
    }
    return true;
  }
  /**
  * The none static version of save
  * @param  {Object}  options [Success and error callbacks for saving/updating]
  */
  save(options){
    this.constructor.save(this.getAllAttributes(),options);
  }

  /**
  * The static version of save, saving or updating depending on if it has an id or not.
  * @param  {Object}  data [The data which needs to be saved, if no id it's created else it's updated]
  * @param  {Object}  options [Success and error callbacks for saving/updating]
  */
  static save(data,options){
    if(!this.isObject(data,options)) return;
    if(data.id){
      this.update(data,options);
    } else {
      this.create(data,options);
    }
  }

  /**
  * Retrieves an object by id from the database.
  * @param  {int}  id [The id of the wanted model]
  * @param  {Object}  options [Success and error callbacks]
  */
  static find(id,options){
    if(!id || isNaN(id)){
      if(options && isFunction(options.error)){
        options.error({error: 'Cannot find object with no id', data: id});
      }
      console.error('Cannot find object with no id: ', id);
      return;
    }
    knex(this.getDBName()).where('id',id).then(d=>{
      // arr.id = d[0];
      if(options && isFunction(options.success)){
        var model = new this(d[0]);
        options.success(model.decrypt());
      }
    }).error(e=>{
      if(options && isFunction(options.error))
        options.error(e);
    });
  }
  /**
  * Retrieves an object by id from the database.
  * @param  {Object}  options [Success and error callbacks]
  * @param  {int}  offset [The offset from which we will retrieve]
  * @param  {int}  limit [The limit of object we will retrieve]
  */
  static all(options,offset,limit){
    if(!offset) offset = 0;
    if(!limit) limit =100000000;
    knex(this.getDBName()).select('*').offset(offset).limit(limit).then(d=>{
      if(options && isFunction(options.success))
        options.success(d);
    }).error(e=>{
      if(options && isFunction(options.error))
        options.error(e);
    });
  }

  /**
  * Retrieves all object by a key with a value, if the values is encrypted in the DB,
  * just input the none encrypted value on which you want to find
  * @param  {String}  key [The string representation of the key to be searched]
  * @param  {mixed}  val [The value which we want to find]
  * @param  {Object}  options [Success and error callbacks]
  */
  static getAllByKeyVal(key,val,options){
    if(!key) {
      if(options && isFunction(options.error)){
        options.error({error: 'Trying to get objects with key undefined', data: key});
      }
      console.warn('Trying to get objects with key undefined');
    }
    if(!val) {
      if(options && isFunction(options.error)){
        options.error({error: 'Trying to get objects with val undefined', data: key});
      }
      console.warn('Trying to get objects with val undefined');
    }
    val = (this.isEncryptable(key)) ? this.encryptValue(val) : val;
    knex(this.getDBName()).select('*').where(key, val).then(d=>{
      if(options && isFunction(options.success))
        options.success(d);
    }).error(e=>{
      if(options && isFunction(options.error))
        options.error(e);
    })
  }
  /**
  * Retrieves first object by a key with a value, if the values is encrypted in the DB,
  * just input the none encrypted value on which you want to find
  * @param  {String}  key [The string representation of the key to be searched]
  * @param  {mixed}  val [The value which we want to find]
  * @param  {Object}  options [Success and error callbacks]
  */
  static getByKeyVal(key,val,options){
    if(!key) {
      if(options && isFunction(options.error)){
        options.error({error: 'Trying to get objects with key undefined', data: key});
      }
      console.warn('Trying to get objects with key undefined');
    }
    if(!val) {
      if(options && isFunction(options.error)){
        options.error({error: 'Trying to get objects with val undefined', data: key});
      }
      console.warn('Trying to get objects with val undefined');
    }
    val = (this.isEncryptable(key)) ? this.encryptValue(''+val) : val;
    knex(this.getDBName()).select('*').where(key, val).then(d=>{
      if(options && isFunction(options.success))
        options.success((new this(d[0])).decrypt());
    }).error(e=>{
      if(options && isFunction(options.error))
        options.error(e);
    })
  }
  //creates a new instance of a model in the database
  //possible options:
  //success: function that gets called on success
  //error: function that gets called on error
  /**
  * Creates a new object with attributes
  * @param  {Array}  attributes [The attributes of the to be created model]
  * @param  {Object}  options [Success and error callbacks]
  */
  static create(attributes,options){
    if(!this.isObject(attributes,options)) return;
    if(attributes.id) {
      if(options && isFunction(options.error)){
        options.error({error: 'WARNING: Trying to create object with id, id will be auto set', data: attributes});
      }
      console.warn('WARNING: Trying to create object with id, id will be auto set');
    }
    var arr = {};
    //loop through the list of attributes that want to be saved
    _.each(attributes,(val,key)=>{
      //check if the key is fillable
      if(this.isFillable(key)){
        arr[key] = val;
      }
    });
    knex(this.getDBName()).insert(this.encryptKeys(arr)).then(d=>{
      arr.id = d[0];
      arr = this.decryptKeys(arr);
      if(options && isFunction(options.success))
        options.success(new this(arr));
    }).error(e=>{
      console.log(e);
      if(options && isFunction(options.error))
        options.error(e);
    });
  }
  /**
  * Updates an existing model
  * @param  {Array}  attributes [The attributes of the to be updated model, should contain an ID]
  * @param  {Object}  options [Success and error callbacks]
  */
  static update(attributes,options){
    if(!this.isObject(attributes,options)) return;
    if(!attributes.id) {
      if(options && isFunction(options.error)){
        options.error({error: 'Trying to update object with no id', data: attributes});
      }
      console.error('Trying to update object with no id');
      return;
    }
    var arr = {};
    //loop through the list of attributes that want to be saved
    _.each(attributes,(val,key)=>{
      //check if the key is fillable
      if(this.isFillable(key)){
        arr[key] = val;
      }
    });
    knex(this.getDBName()).update(this.encryptKeys(arr)).where('id',attributes.id).then(d=>{
      arr.id = attributes.id;
      arr = this.decryptKeys(arr);
      if(options && isFunction(options.success))
        options.success(new this(arr));
    }).error(e=>{
      console.log(e);
      if(options && isFunction(options.error))
        options.error(e);
    });
  }
}
module.exports = Model;
