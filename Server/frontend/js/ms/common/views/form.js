// var form = new FormView({
//   model: model,
//   fields: [
//     {
//       attr: 'id',
//       label: 'id',
//       type: 'text'
//     }, {
//       attr: 'name',
//       label: 'Naam',
//       type: 'textField'
//     }
//   ]
// });

var Backbone = require('backbone');
var _ = require('underscore');

import app from 'ms/app.js';
import {isFunction} from 'ms/utils.js';

var fieldId = 0;

var fields = {
    textarea: Handlebars.templates['layout/textArea'],
    text: Handlebars.templates['layout/textField'],
    singleselect: Handlebars.templates['layout/singleSelect'],
    range: Handlebars.templates['layout/range'],
    password: Handlebars.templates['layout/textField'],
    email: Handlebars.templates['layout/textField'],
    date: Handlebars.templates['layout/date']
};

var FormView = Backbone.View.extend({
  templateButton: Handlebars.templates['layout/button'],
    initialize: function (settings) {
        var self = this;
        this.class = settings.class || undefined;
        this.saveButton = settings.saveButton;
        this.settings = [];
        _.each(settings.fields, function (attr) {
            self.settings.push(_.extend({
                type: 'text'
            }, attr));
        });
    },
    render: function () {
        var self = this;
        this.$el.empty();
        if(self.class){
          self.$el.addClass(self.class);
        }
        self.fields = {};
        _.each(this.settings, function (setting) {
            var details = _.extend(setting, {
                field: FormView.getFieldId(),
                value: self.model.get(setting.attr),
                disable: setting.disable || false,
                limit: setting.limit || undefined,
                class: setting.class || undefined,
                size: setting.size || undefined,
                hide: setting.hide || false,
                showHidden: setting.showHidden || false,
                errormsg: setting.errormsg || '',
                successmsg: setting.successmsg || '',
                unrequire: setting.unrequire || false,
            });
            var template = fields[details.type];
            var field;

              if (!template && details.type !== 'button')
              throw 'Field ' + JSON.stringify(setting, undefined, 3) + ' needs a valid "type" attribute.';
              if(details.type !== 'password'){
                if(details.showHidden !== false)
                  details.showHidden = false;
              }
              if(details.type !== 'button'){
                field = $(template(details));
                self.$el.append(field);
              }
              if(details.class){
                if($.isArray(details.class)){
                  _.each(details.class, function(cl){
                    field.addClass(cl);
                  });
                } else {
                  field.addClass(details.class);
                }
              }
            details.$el = field;
            self.fields[details.attr] = details;
            console.log(JSON.stringify(self.model.get(setting.attr)));
            self.model.set(setting.attr,(self.model.get(setting.attr) + '').replace(/\0/g, ''));
            console.log(JSON.stringify(self.model.get(setting.attr)));

            if (details.type === 'text' || details.type === 'textarea' || details.type === 'email' || details.type === 'password') {
                self.model.on('change:' + details.attr, function () {
                    var val = self.model.get(setting.attr);
                    field.find('[name="' + setting.attr + '"]').val(self.model.get(setting.attr).replace(/\0/g, ''));
                    field.find('label[for="ms_' + details.field + '"]')[val ? 'addClass' : 'removeClass']('active');
                });
                if(details.type === 'password' && details.showHidden){
                  details.$el.find('.show-hidden-password').click(function(){
                    (details.$el.find('input').attr('type') === 'text') ? details.$el.find('input').attr('type','password') : details.$el.find('input').attr('type','text');
                    (details.$el.find('input').attr('type') === 'text') ? details.$el.find('.show-hidden-password').text('visibility_off') : details.$el.find('.show-hidden-password').text('visibility');
                  });
                }
                if (details.type === 'textarea' && details.value) {
                    field.find('textarea').val(details.value);
                    $('#limit_' + setting.field+'').text(details.limit-(field.find('textarea').val().length));
                    field.find('textarea').trigger('autoresize');
                }
                  self.model.on('change:' + details.attr, function () {
                    if(field.find('textarea').val())
                      $('#limit_' + setting.field+'').text(details.limit-(field.find('textarea').val().length));
                  });
                  if(details.type === 'textarea' && details.limit){
                    field.keyup(function () {
                      var left = details.limit-(field.find('textarea').val().length);

                      $('#limit_' + setting.field+'').text(left);
                    });
                  }
            } else if (details.type === 'range') {
                self.model.on('change:' + details.attr, function () {

                    var val = self.model.get(setting.attr);
                    field.find('[name="' + setting.attr + '"]').val(self.model.get(setting.attr));
                });
            }
            else if (details.type === 'button'){
              var b = $(self.templateButton({
                label: self.model.get(details.attr),
                logo: details.logo,
                icon: details.icon,
                href: details.href,
                tooltip: details.tooltip
              }));
              var but = self.$el.append(b);
              details.$el = b;
              if(details.trigger){
                b.on('click', function() {
                  self.trigger(details.trigger, self.model);
                });
              }
              but.href = details.href;
              self.model.set('button', but);
              self.model.set('row', self);
            }
            else if (details.type === 'singleselect') {
                var selectField = field.find('select');
                if (details.value) {
                    selectField.val(details.value);
                } else {
                    self.model.set(setting.attr, selectField.val());
                }
                console.log(selectField);
                selectField.material_select();
                self.model.on('change:' + details.attr, function () {
                    selectField.val(self.model.get(setting.attr));
                    selectField.material_select();
                });
            } else if (details.type === 'date') {
                var $input = field.find('input').pickadate({
                  monthsFull: 'Januari Februari Maart April Mei Juni Juli Augustus September Oktober November December'.split(' '),
                  monthsShort: 'Jan Feb Mrt Apr Mei Jun Jul Aug Sep Okt Nov Dec'.split(' '),
                  today: 'Vandaag',
                  clear: 'Legen',
                  close: 'Sluiten',
                  weekdaysFull: 'Zondag Maandag Dinsdag Woensdag Donderdag Vrijdag Zaterdag'.split(' '),
                  weekdaysShort: 'Zo Ma Di Wo Do Vr Za'.split(' '),
                  labelMonthNext: 'Volgende maand',
                  labelMonthPrev: 'Vorige maand',
                  labelMonthSelect: 'Selecteer een maand',
                  labelYearSelect: 'Selecteer een jaar',
                  selectMonths: true, // Creates a dropdown to control month
                  selectYears: 100, // Creates a dropdown of 15 years to control year
                  format: 'yyyy-mm-dd'
                });
                var picker = $input.pickadate('picker');
                var updateDate = function () {
                    var val = self.model.get(setting.attr);
                    if (val && val !== '0000-00-00') {
                        picker.set('select', val, {
                            format: 'yyyy-mm-dd'
                        });
                        field.find('label[for="ms_' + details.field + '"]').addClass('active');
                    } else {
                        picker.set('select', null);
                        field.find('label[for="ms_' + details.field + '"]').removeClass('active');
                    }
                };
                self.model.on('change:' + details.attr, updateDate);
                updateDate();
            }
            var fieldListener = function () {
                self.model.set(details.attr, $(this).val());
            };
            if(field)
            field.find('input:not([type=radio]),textarea,select').change(fieldListener);
            if (details.keyup) {
                field.find('input:not([type=radio]),textarea,select').keyup(fieldListener);
            }
            if(details.size){
              if(!details.$el.parent().hasClass('row'))
                details.$el.parent().addClass('row');
              if(!details.$el.hasClass('col'))
                details.$el.addClass('col');
              var i = 1;
              while(i<=12){
                if(details.$el.hasClass('s'+i)){
                  details.$el.removeClass('s'+i);
                }
                i++;
              }
              details.$el.addClass('s'+details.size);
            }
          if(details.hide){
            details.$el.hide();
          }
          });
          if(this.saveButton){
            var add = $(Handlebars.templates['layout/button']({
              label: 'Opslaan',
            }));
            this.$el.append('<div class="save-button-div z-depth-2"></div>');
            this.$el.find('.save-button-div').append(add);
            add.click(()=>{
              if(this.saveButton.click && isFunction(this.saveButton.click)){
                this.saveButton.click();
              } else {
                var pre;
                var post;
                if(this.saveButton.preSave && isFunction(this.saveButton.preSave)) pre = this.saveButton.preSave(self.model);
                if(pre !== false)
                {
                  self.model.save(null, {success:()=>{
                    if(this.saveButton.postSave && isFunction(this.saveButton.postSave)) post = this.saveButton.postSave(self.model);
                    if(post !== false){
                      swal({
                        title: 'Succesvol opgeslagen',
                        type: 'success'
                      });
                    }
                  }});
                }
              }
            });
          }
        return this;
    }
}, {
    getFieldId: function () {
        fieldId++;
        return fieldId;
    }
});

module.exports = FormView;
