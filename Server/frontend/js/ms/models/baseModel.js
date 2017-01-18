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
    console.log(this);
    if(this.internalValidate(attrs,options) === true){
      Backbone.Model.prototype.save.call(this, attrs, options);
    } else {
      console.log(this.internalValidate(attrs,options));
    }
  }

});

module.exports = BaseModel;
