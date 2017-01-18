var Backbone = require('backbone');
import SortableCollection from 'ms/common/collections/sortable-collection.js';
import Practice from 'ms/models/practices.js';

var PracticeSearchCollection = SortableCollection.extend({
  model: Practice,

  setQuery: function(model) {
    var query = model.get('search');
    if(!query || query === "undefined") query= '-';
    this.name = 'admin/practices/searchPractice/' + query;
    return this;
  }
});
module.exports = PracticeSearchCollection;
