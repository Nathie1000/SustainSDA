import Backbone from 'backbone';

var BaseModel = Backbone.Model.extend({

  setUrl: function(url){
    this.urlRoot = '/'+url;
  },

  // oldValidate: BaseModel.validate,
  // oldSave: this.save,

  validate: function(attrs,options){
    console.log(BaseModel);
    var errors = Backbone.Model.prototype.validate.call(this, attrs, options);
    console.log(errors);
    if(!errors) return true;
    return errors;
  },

  internalValidate: function(attrs,options){
    console.log(BaseModel);
    var errors = Backbone.Model.prototype.validate.call(this, attrs, options);
    console.log(errors);
    if(!errors) return true;
    console.log(123);
    return errors;
  },

  save: function(attrs,options){
    console.log(2,attrs,options);
    if(this.internalValidate(attrs,options) === true){
      console.log(1,options);
      Backbone.Model.prototype.save.call(this, attrs, options);
      // var temp = options.success;
      // options.success = (model,response)=>{
      //   console.log(123);
      //   temp(model,response);
      // }
    } else {
      console.log(this.internalValidate(attrs,options));
    }
  }

});

module.exports = BaseModel;
