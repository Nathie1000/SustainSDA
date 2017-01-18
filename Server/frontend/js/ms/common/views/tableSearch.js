var Backbone = require('backbone');
var _ = require('underscore');

import app from 'ms/app.js';
import Form from 'ms/common/views/form.js';
import Table from 'ms/common/views/table.js';

var TabView = Backbone.View.extend({
  template: Handlebars.templates['layout/tableSearchView'],
  searchList: null,
  content: null,
  current: null,


  initialize: function(s){
    this.col = s.collection;
    this.secondAttr = s.secondAttr || null;
    this.columns = s.columns;
    this.settings = s;
    console.log(this.col);
    try{
      if(this.secondAttr){
        console.log(2);
        this.col.setQuery('','');
      } else {
        console.log(1);
        this.col.setQuery('');
      }
    } catch(e){
      swal({
        title: 'Error found',
        text: 'collection has no correct setQuery function, please implement. If you have a secondAttr make sure the setQuery function requires 2 params.',
        type: 'error'
      });
    }
  },

  onSearch: function(model){
    if(this.secondAttr)
      this.collection.setQuery(model.get('search'), model.get('secondAttr'));
    else
      this.collection.setQuery(model.get('search'));
    this.collection.fetch();
  },

  render: function(){
    this.$el.html(this.template());
    this.searchList = this.$el.find('.tableViewList');
    var f = new Form({
      model: new Backbone.Model(),
      fields: [
        {
          icon: 'search',
          attr: 'search',
          keyup: true
        }
      ]
    });
    var timeout;
    if(this.secondAttr)
      f.model.set('secondAttr', this.secondAttr);
    f.model.on('change:search', ()=>{
      if(timeout){
        clearTimeout(timeout);
        timeout = null;
      }
      timeout = setTimeout(()=>{
        this.onSearch(f.model);
      },200);
    });
    this.$el.find('.topBarTable').append(f.render().el);
    this.table = new Table(this.settings);
    // this.table = new Table({
    //   collection: this.collection,
    //   columns: this.columns
    // });
    this.table.on('click', m=>{
      this.trigger('click',m);
    });
    this.searchList.append(this.table.render().el);
    this.onSearch(f.model);
    return this;
  }

});

module.exports = TabView;
