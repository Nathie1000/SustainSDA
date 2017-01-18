var Backbone = require('backbone');
var _ = require('underscore');
import app from 'ms/app.js';

var SortableCollection = Backbone.Collection.extend({

  sortableTable: true,

  initialize: function(options){
    this.options = options;
    this.name = '';
    this.lastPosition = 0;
    this.subCollection = new Backbone.Collection();
    if (!this.options){
      this.options = {};
      this.options.length = 20;
    }
  },
  sort: function(a){
    this.subCollection.models.sort(a);
    this.reset();
    this.add(this.subCollection.models.slice(0,this.options.length));
  },
  /**
   * Fetches the whole collection.
   * Resets the main en sub collections and adds the first part of models(0-10 for example) to the current collections
   * @param  {[type]} options [description]
   * @return {[type]}         [description]
   */
  fetch: function(options){
    if (!options){
      options = {};
    }
    var succ = options.success;
    options.success = (collection, response, options)=>{
      this.fetchCallback(succ,collection, response, options);
    }
    return Backbone.Collection.prototype.fetch.call(this, options);
  },
  /** PRIVATE
   * Resets the collections and calls for a refresh()
   * Also calls the normal succes callback
   */
  fetchCallback: function(succ,collection, response, options){
    this.subCollection.reset(this.models); //adding all fetched models to the subcollection
    this.refresh();
    if(succ && app.isFunction(succ)){
      succ(collection, response, options);
    }
  },

  getNext: function (options) {
    console.log(this);
    this.add(this.subCollection.models.slice(this.lastPosition,this.lastPosition+this.options.length));
    this.lastPosition += this.options.length;
  },
  url: function () {
    return this.name;
  },
  /**
   * Resets the collection and subcollection, resets lastPosition to 0 and
   * @return {[type]} [description]
   */
  refresh: function () {
    this.reset();
    this.lastPosition = 0;
    this.getNext();
  }
});

module.exports = SortableCollection;
