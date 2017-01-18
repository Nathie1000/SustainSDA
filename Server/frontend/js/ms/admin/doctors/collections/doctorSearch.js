var Backbone = require('backbone');
import SortableCollection from 'ms/common/collections/sortable-collection.js';
import Doctor from 'ms/models/doctors.js';

var PatientSearchCollection = SortableCollection.extend({
  model: Doctor,

  setQuery: function(model) {
    var query = model.get('search');
    if(!query || query === 'undefined') query= '-';
    this.name = 'admin/doctors/searchDoctor/' + query + '/'+app.currentPractice.id;
    return this;
  }
});
module.exports = PatientSearchCollection;
