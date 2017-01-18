var Backbone = require('backbone');
import SortableCollection from 'ms/common/collections/sortable-collection.js';

var DoctorSearchCollection = SortableCollection.extend({

  setQuery: function(model) {
    var query = model.get('search');
    if(!query || query === 'undefined') query= '-';
    this.name = 'fysio/doctors/searchDoctor/' + query;
    return this;
  }
});
module.exports = DoctorSearchCollection;
