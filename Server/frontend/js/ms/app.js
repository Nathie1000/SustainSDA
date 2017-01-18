import 'babel-polyfill';
import Backbone from 'backbone';
var _ = require('underscore');
import async from 'async';
import AppPage from 'ms/common/helpers/app-page.js';
import User from 'ms/models/users.js';
import Breadcrumb from 'ms/common/helpers/breadcrumbs.js';

import {isFunction} from 'ms/utils.js';

Backbone.$ = $;
global.Backbone = Backbone;
require('./lib/backbone.collectionView.js');
require('./lib/owl.carousel.min.js');
require('./lib/spectrum.js');
const app = {};
app.ROLE_DOCTOR = 1;
app.ROLE_ADMIN = 2;
app.ROLE_PATIENT = 3;
export default app;
_.extend(app, Backbone.Events);
app.isFunction = isFunction;

app.history = [];
var Validation = require('backbone-validation');
_.extend(Backbone.Model.prototype, Validation.mixin);
//TODO Remove this:
window.app = app;


app.paths = { routes: {} };
app.showView = function (view, args){
      if (app.currentView){
        app.currentView.close();
      }
      app.currentView = view;
      if(args && args[0] !== null)
        app.currentView.render.apply(view,args);
      else{
        app.currentView.render();
      }

      $(".main-content").html(app.currentView.el);
      app.bread.refresh();
    };

    Backbone.View.prototype.close = function(){
      this.remove();
      this.unbind();
      if(this.model instanceof Backbone.Model)
        this.model.unbind("change", this.modelChanged);
     };


var loading = 0;
$.ajaxSetup({
  beforeSend: function () {
    if (loading === 0) $(".main-progress-bar").show();
		loading++;
  },
  complete: function (xhr) {
		if (loading > 0) loading--;
    if (loading === 0) $(".main-progress-bar").hide();
    if(xhr.responseText.contains('Login Sustain')) {
      swal({
        title: 'U bent of niet ingelogd of incorrect ingelogd.',
        text: 'U wordt doorverwezen naar de login pagina.',
        type: 'info'
      },()=>{
        location.reload();
      });
    }
  }
});
var mainProgressCircular = $('.main-progress-circular');
var loaderIsShown = false;
app.showLoader = function (hideLoader) {
  if (hideLoader) $('main').hide();
  if (loaderIsShown) return;
  loaderIsShown = true;
	mainProgressCircular.openModal({
		dismissible: false
	});
  $(".main-progress-bar").css({
    zIndex: 0
  });
};
app.hideLoader = function () {
  loaderIsShown = false;
	mainProgressCircular.closeModal();
  $(".main-progress-bar").removeAttr('style');
  $('main').show();
};

app.bread = new Breadcrumb();
$('div.breadcrumb').append(app.bread.render().el);
$('.logo').click(()=>{app.router.navigate('home',{trigger:true});})
app.addToHistory = function(path,name,args){
  if(args.length===0 || !path.contains(':')) {app.history.push({path: path,name:name});return;}
  var i = 1;
  var split = path.split(':');
  var newPath = split[0];
  _.each(args,a=>{
    if(a){
      newPath += ((i!==1)?'/':'') + a;
      i++;
    }
  });
  app.history.push({path: newPath,name:name});
};

app.getLastHistory = function(){
  //the current path is already in the history array so the last item is the current path,
  //meaning last-1 is the previous path
  return app.history[app.history.length-2];
};

app.getHistory = function(length=3){
  var arr = [];
  if(app.history.length-1 <= length) {var reverse = (app.history.slice()).reverse(); console.log(reverse); reverse.removeKey(0); return reverse;}
  for (var i = app.history.length-2; i > app.history.length-2-length; i--) {
    arr.push(app.history[i]);
  }

  return arr;
};

app.getCurrentPath = function(){
  return app.history[app.history.length-1];
}

var pathId = 0;
app.addPath = function (path, View, pathName) {
  console.log(arguments);
	pathId++;
	var page = new AppPage(path, View);
	var name = 'func' + pathId;
	app.paths.routes[path] = name;
	app.paths[name] = function () {
		app.trigger('page:change');
    console.log(path,arguments,pathName);
    app.addToHistory(path,pathName,arguments);
		page.activate.apply(page, [path, arguments]);
		app.trigger('page:loaded');
	};
};

app.setHome = function (url) {
	app.on('home', function() {
	  app.router.navigate(url, {trigger: true});
	});
	$('#logo-container').attr('href', '#' + url);
};

app.goHome = function () {
	app.trigger('home');
};
$.ajax('/user/users/current')
.done(response=>{

  console.log(1);
  console.log(response);
  app.currentUser = new User(response);
  async.parallel([
  	function (callback) {
      var func = require('./doctor/init');
      func(callback);
  	},
  	function (callback) {
      var func = require('./admin/init');
      func(callback);
  	},
  	function (callback) {
      var func = require('./patient/init');
      func(callback);
  	},
  ],
  function (err, results) {
  	if (err) {
      alert(err.message);
  		throw err;
  	} else {
      app.paths.routes['*path'] = 'goHome';
      app.paths.goHome = function () {
      	app.goHome();
      };
      var AppRouter = Backbone.Router.extend(app.paths);
      app.router = new AppRouter();
      Backbone.history.start();
      try {
      	var fragment = Backbone.history.getFragment();
      	if (!fragment) throw 1;
      	app.router.navigate(fragment, { trigger: true });
      } catch(e) {
      	app.goHome();
      }
  	}
  });
});
