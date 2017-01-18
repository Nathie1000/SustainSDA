// EXAMPLE:
// new TabView([
//   {
//     title: 'Subsidie wachtrij',
//     view: function () {
//       return new Backbone.View();
//     }
//   }, {
//     title: 'Zoeken',
//     view: function () {
//       return new Backbone.View();
//     }
//   }, {
//     title: 'Toevoegen',
//     view: function () {
//       return new Backbone.View();
//     }
//   }
// ]);


var Backbone = require('backbone');
var _ = require('underscore');

import app from 'ms/app.js';

var tabCid = 0;

var TabView = Backbone.View.extend({
  template: Handlebars.templates['layout/tabs'],
  initialize: function (tabs) {
    var self = this;
    console.log(tabs);
    var mainC = '';
    if(tabs.tabsextra){
      mainC = tabs.mainclass;
      tabs = tabs.tabsextra;
    }
    this.model = {
      mainclass: mainC,
      tabs: tabs
    };
    this.map = {};
    var col = Math.floor(12 / tabs.length);
    _.each(this.model.tabs, function(tab){
      tab.col = col;
      tabCid++;
      tab.index = tabCid;
      tab.cid = 't-' + tabCid;
      self.map[tab.cid] = {
        initiated: false,
        view: tab.view,
        tab: tab
      };
    });
  },
  events: {
    'click .tabs > .tab > a': 'click'
  },
  click: function (event) {
    this.$('.tab-content').hide();
    var cid = $(event.target).closest('li').data('cid');
    this.activate(cid);
  },
  select: function (index) {
		var el = this.$('.tabs > .tab > a')[index];
    this.code = true;
    if (el.fireEvent) {
    	el.fireEvent('onclick');
    } else {
    	var evObj = document.createEvent('Events');
    	evObj.initEvent('click', true, false);
    	el.dispatchEvent(evObj);
    }
  },
  activate: function(cid) {
    var item = this.map[cid];
    var self = this;
    var t = 0;
    if(this.code){
      this.code = false;
      t = cid.split('t-')[1];
      while(t > self.model.tabs.length){
        t -= self.model.tabs.length;
      }
      app.currentTab = t;
    } else {
      t = cid.split('t-')[1];
      while(t > self.model.tabs.length){
        t -= self.model.tabs.length;
      }
      app.currentTab = t;
    }
    if (!item.initiated) {
      var view = item.view();
      this.$('.tab' + cid).append(view.el);
      view.render();

      item.initiated = true;
    }
    if(self.curTab)
      if(self.curTab.hasClass('active'))
        self.curTab.removeClass('active');
    var tab = this.$('.tab'+cid);
    if(!tab.hasClass('active')){
      self.curTab = tab;
      tab.addClass('active');
    }
    this.$('.tab-content.tab' + cid).show();
  },


  render: function () {
    var self = this;
    this.$el.html(this.template(this.model));
    var tab = location.hash;
    var tabindex = 0;
    if(tab.indexOf("t=") > -1){
      tab = tab.split("t=");
      tabindex = tab[1].split(';')[0];
      // if(!$.isNumeric(tabindex))
      //   tabindex = tab[1].split('?')[0];
      while(tabindex > self.model.tabs.length){
        tabindex -= self.model.tabs.length;
      }
      tabindex--;
    }
    var cid = this.model.tabs[tabindex].cid;
    // app.once('page:loaded', function () {
    //   console.log($(self.el).find('ul.tabs'));
    //   if($(self.el).find('ul.tabs'))
    //     $(self.el).find('ul.tabs').tabs().tabs('select_tab', '#tab' + cid);
    // });
      // self.curTab = $(self.$('.tabs > .tab > a')[tabindex]);
      self.curTab = $(self.model.tabs[tabindex]);
      // self.activate(self.model.tabs[tabindex].cid);
      self.currentIndex = tabindex;
      setTimeout(function(){
        self.select(tabindex);
      },50);
    return this;
  }
});

module.exports = TabView;
