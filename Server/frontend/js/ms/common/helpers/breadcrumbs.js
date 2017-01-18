var _ = require('underscore');
var Backbone = require('backbone');
import app from 'ms/app.js';
var template = Handlebars.templates['layout/breadcrumb'];

module.exports = Backbone.View.extend({

  render: function(){
    this.$el.html(template());
    this.$el.find('li').each(function(){
      $(this).hover(()=>{
        //on hover enter
        $(this).addClass('breadcrumb-hover');
      }, ()=>{
        //on hover leave
        $(this).removeClass('breadcrumb-hover');
      })
    });

    // this.$el.find('li').reverse().each(function(){
    //
    //   $(this).click(()=>{
    //     self.onClick($(this), hist);
    //   });
    // });
    return this;
  },

  refresh: function(){
    var self = this;
    var history = app.getHistory();
    if(history.length === 0) {this.$el.find('li').parent().hide();return;}
    var i = 0;
    this.$el.find('li').reverse().each(function(){
      if(!history[i]) {console.log('temp123');$(this).parent().hide();return;}
      $(this).removeClass('current-view');
      console.log(app.getCurrentPath(), history[i]);
      if(app.getCurrentPath().path === history[i].path) {$(this).addClass('current-view'); console.log('adding class');}
      $(this).parent().show();
      $(this).text(history[i].name);
      $(this).history = history[i];
      var hist = history[i];
      $(this).off('click');
      $(this).click(()=>{
        self.onClick($(this), hist);
      })
      i++;
    });
  },

  onClick: function(el, history){
    app.router.navigate(history.path, {trigger:true});
  }
  // app.history;
});
