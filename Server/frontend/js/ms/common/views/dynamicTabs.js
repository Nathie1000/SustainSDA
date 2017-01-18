var Backbone = require('backbone');
var _ = require('underscore');

import app from 'ms/app.js';

var dtabCid = 0;
var TabView = Backbone.View.extend({
  template: Handlebars.templates['layout/dtabs'],
  templateTab: Handlebars.templates['layout/dtab'],
  currentSelect: null,
  dtabs: [],
  lastSize: 12,
  initialize: function(s){
    if(s){
      this.doTrigger = s.trigger || null;
      this.buttonLabel = s.buttonLabel || '';
      this.standardTabs = s.tabs;
    }
  },

  add: function(tab){
    console.log(tab);
    dtabCid++;
    tab.cid = dtabCid;
    this.$el.find('ul.dtabs').append(this.templateTab({title: tab.title, cid: tab.cid}));
    tab.titleContent = this.$el.find('li.dtab-'+tab.cid);
    this.$el.find('li.dtab-'+tab.cid).click(()=>{
      this.click(tab);
    });
    this.$el.find('ul.dtabs').parent().parent().append('<div class="dtab-content dtab-'+tab.cid+'"></div>');
    console.log(this.$el.find('div.dtab-'+tab.cid));
    var a = this.$el.find('div.dtab-'+tab.cid).append((tab.view()).render().el);
    tab.content = a;
    this.dtabs.push(tab);
    var size = Math.floor(12/this.dtabs.length);
    if(size < 1) size = 1;
    _.each(this.dtabs, t=>{
      t.titleContent.removeClass('s'+this.lastSize);
      t.titleContent.addClass('s'+size);
    });
    this.lastSize = size;
    tab.content.hide();
    this.refresh();
  },

  click: function(tab){
    var cur = this.getByCid(this.currentSelect);
    this.currentSelect = tab.cid;
    cur.content.hide();
    cur.titleContent.removeClass('active');
    tab.content.show();
    tab.titleContent.addClass('active');
  },

  refresh: function(){
    if(!this.currentSelect){
      this.currentSelect = this.dtabs[0].cid;
    }
    var tab = this.getByCid(this.currentSelect);
    tab.titleContent.addClass('active');
    tab.content.show();
  },

  getByCid: function(cid){
    var r = null;
    _.each(this.dtabs,m=>{
      if(m.cid === cid && !r){
        r = m;
      }
    });
    return r;
  },

  render: function(){
    this.$el.html(this.template());
    if(this.standardTabs){
      _.each(this.standardTabs,t=>{
        this.add(t);
      });
    }


    return this;
  }

});

module.exports = TabView;
