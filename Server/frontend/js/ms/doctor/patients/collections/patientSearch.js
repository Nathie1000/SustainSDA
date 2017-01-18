var Backbone = require('backbone');
import SortableCollection from 'ms/common/collections/sortable-collection.js';

var PatientSearchCollection = SortableCollection.extend({


  setQuery: function(model) {
    console.log(model);
    var query = model.get('search');
    console.log(query);
    if(!query || query === 'undefined') query= '-';
    console.log(query);
    this.name = 'fysio/patients/searchPatient/' + query;
    return this;
  }
});
module.exports = PatientSearchCollection;
