var Backbone = require('backbone');
var _ = require('underscore');

var PaginationCollection = Backbone.Collection.extend({
  getNext: function (options) {
    var self = this;
    var obj = {};
    if(self.autofetch === undefined)
      self.autofetch = true;

    _.extend(obj, options);
    _.extend(obj, {
      success: function (collection, response) {
        if (options && options.success) options.success.apply(undefined, arguments);
      if(self.success && typeof self.success === 'function'){
        self.success(collection);
      }
        if (response.length < self.req.length) {
          self.trigger('ended');

        }
      }
    });
      this.fetch(_.extend({
        remove: false
      }, obj));
      this.req.offset += this.req.length;
  },
  url: function () {
    return this.req.name + '/' + this.req.offset + '/' + this.req.length;
  },
  refresh: function () {
    this.req.offset = 0;
    this.reset();
    this.getNext.apply(this, arguments);
  }
});

module.exports = PaginationCollection;
