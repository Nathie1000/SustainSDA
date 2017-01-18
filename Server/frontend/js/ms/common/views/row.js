var Backbone = require('backbone');
var _ = require('underscore');

import {isFunction} from 'ms/utils.js';


function toLocalDate(date) {
  var local = new Date(date);
  local.setMinutes(date.getMinutes() - date.getTimezoneOffset());
  return local.toJSON().slice(0, 16).replace('T', ' ');
}

var RowView = function(columns, tableView) {

  return Backbone.View.extend({
    tagName: 'tr',
    templateButton: Handlebars.templates['layout/button'],
    templateRange: Handlebars.templates['layout/range'],
    templateLabel: Handlebars.templates['layout/label'],

    render: function() {
      var self = this;
      this.$el.empty();
      if(tableView.rowClass !== null){
        console.log(1);
        if((tableView.rowClass.attr && self.model.get(tableView.rowClass.attr)) || !tableView.rowClass.attr){
          console.log(2);
          this.$el.addClass(tableView.rowClass.class);
        }
      }
      _.each(columns, function(column) {
        if(tableView.onClick){
          var $el = $('<td style="cursor: pointer"></td>');
        } else {
          var $el = $('<td></td>');
        }
        self.$el.append($el);
        if (column.width) {
          $el.css('width', column.width);
        }
        if (column.render && typeof column.render === 'function') {
          self.$el.append($el);
          if (column.render($el, self.model, column)) {
            $el.addClass('reduced-space');
          }
          if (column.update && typeof column.update === 'function') {
            self.model.on('change:' + column.attr, column.update);
          }
        } else {
          var setText = function() {
            if (column.default && !self.model.get(column.attr)) {
              $el.text(column.default);
            } else {
              if (!column.type) {
                var value = self.model.get(column.attr);
                if (column.translate) {
                  value = column.translate[value] || value;
                }
                if (column.attr === 'active') {
                  if (value === 0)
                    value = 'Nee';
                  else
                    value = 'Ja';
                }
                if (!value) {
                  value = column.empty
                }
                if(column.percentage)
                  if(value) value += '%'; else value = '0%';
                $el.text(value);
              } else if (column.type === 'logo') {
                if ((tableView.forbidden && tableView.forbidden.type === 'logo') || !tableView.forbidden) {
                  if (!tableView.forbidden || (self.model.get(tableView.forbidden.attr) !== tableView.forbidden.value)) {
                    $el.css({
                      backgroundImage: 'url(\'/partnerprogramma/' + self.model.get(column.attr) + '\')',
                      backgroundSize: '75px 50px',
                      backgroundRepeat: 'no-repeat'
                    });
                  }
                }
              } else if (column.type === 'button') {
                if ((tableView.forbidden && tableView.forbidden.type === 'button') || !tableView.forbidden) {
                  if (!tableView.forbidden || (self.model.get(tableView.forbidden.attr) !== tableView.forbidden.value)) {
                    var but = $el.html(self.templateButton({
                      label: self.model.get(column.attr),
                      logo: column.logo,
                      icon: column.icon,
                      href: column.href,
                      tooltip: column.tooltip
                    }));
                    but.on('click', function() {
                      tableView.trigger(column.trigger, self.model);
                    });
                    but.href = column.href;
                    self.model.set('button', but);
                    self.model.set('row', self);
                  }
                }
              } else if (column.type === 'range') {
                if ((tableView.forbidden && tableView.forbidden.type === 'range') || !tableView.forbidden) {
                  if (!tableView.forbidden || (self.model.get(tableView.forbidden.attr) !== tableView.forbidden.value)) {
                    if (!self.rng)
                      self.rng = $el.html(self.templateRange({
                        label: '0',
                        min: column.values[0].min,
                        max: column.values[0].max
                      }));


                    if (!self.model.get(column.attr))
                      self.model.set(column.attr, 0);
                    var val = self.model.get('skill');
                    self.$('.rangeSlider').val(val);
                    self.$('.label-show').text(val);
                    self.rng.on('change', function() {
                      var rn = self.$el.find('.rangeSlider');
                      var lbl = self.$el.find('.label-show');
                      self.undelegateEvents();
                      self.model.set(column.attr, rn.val());
                      self.delegateEvents();
                      tableView.trigger('slide', self.model, rn, lbl);
                      lbl.text(rn.val());

                    });
                  }
                }
              }
            }
          };
          if (column.type !== 'range') {
            self.model.on('change:' + column.attr, setText);
          }
          setText();
        }
        if (!column.type || (column.type !== 'button')) {
          $el.click(function(event) {
            if(tableView.onClick && isFunction(tableView.onClick)) tableView.onClick(self.model);
            tableView.trigger('click', self.model, event, column, self);
          });
        }
      });
      return this;
    }
  });
};

module.exports = RowView;
