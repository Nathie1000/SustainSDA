var Backbone = require('backbone');
import SortableCollection from 'ms/common/collections/sortable-collection.js';
import Patient from 'ms/models/patients.js';

var PatientSearchCollection = SortableCollection.extend({
  model: Patient,

  setQuery: function(model) {
    console.log(123,model);
    var query = model.get('search');
    if(!query || query === 'undefined') query= '-';
    this.name = 'admin/patients/searchPatient/' + query + '/'+app.currentPractice.id;

    return this;
  }
});
module.exports = PatientSearchCollection;
