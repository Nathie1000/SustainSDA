var _ = require('underscore');
import app from 'ms/app.js';


function AppPage (path, View) {
  this.path = path;
  this.View = View;
  this.cache = [];
}

var main = $('main');
var currentPage = null;

AppPage.prototype = {
  activate: function (path, args) {
    var self = this;
    document.body.scrollTop = document.documentElement.scrollTop = 0;
    app.triggerd = false;
      var item = {
        view: new self.View(),
        args: args,
        path: path,
        container: $('<div></div>')
      };
      self.cache.push(item);
      item.container
        .append(item.view.el)
        .appendTo(main);
      app.showView(item.view,args);
      if (item.view.pageLoaded && typeof item.view.pageLoaded === 'function') {
        item.view.pageLoaded();
      }
      currentPage = item;
    }
};

module.exports = AppPage;
