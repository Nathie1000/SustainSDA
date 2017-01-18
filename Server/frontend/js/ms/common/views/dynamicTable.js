/**
 self.table = new TableView({
   collection: col,
   search: true,
   selectable: true,
   options: [
     {
       label: 'Informatie wijzigen',
       function: (model,optionObject)=>{
         swal('Je gaat door naar informatie wijzigen',undefined,'info');
       },
       data: {test:'blabla',whiee:'hoalsl'}
     },
     {
       template: 'delete',
     },
   ],
   columns: [
     {
       attr: 'name',
       label: 'Naam'
     },
     {
       attr: 'displayRole',
       label: 'Functie'
     },
     {
       attr: 'officeName',
       label: 'Vestiging'
     },
     {
       attr: 'plaats',
       label: 'Plaats'
     },
     {
       attr: 'active',
       label: 'Actief'
     },
     {
       attr: 'offline',
       label: 'Laatst ingelogd'
     },
   ]
 });
 */



var Backbone = require('backbone');
var firstBy = require('thenby');
import app from 'ms/app.js';
var _ = require('underscore');
import Form from 'ms/common/views/form.js';
import {generateRandomString} from 'ms/utils.js';

var i = 0;
var sort = {};
var icons = [
  '',
  'expand_more',
  'expand_less'
];
var Collapsible = Backbone.View.extend({
    template: Handlebars.templates['layout/dynamicTable'],
    templateRow: Handlebars.templates['layout/dynamicTableRow'],

    randomNames: [],

    initialize: function(settings){
      this.collection = settings.collection;
      if(!this.collection.sortableTable){
        swal({
          title: 'Collection type moet sortableCollection zijn!',
          type: 'error'
        });
        return;
      }
      this.collection.on('reset',()=>{this.emptyRows()})
      this.settings = settings;
      this.rowId = 0;
      var name = 'tr'+generateRandomString(8);
      while(this.randomNames.contains(name)){
        name = 'tr'+generateRandomString(8);
      }
      this.name = name;
      this.settings.NoResults = this.settings.NoResults || 'Geen resultaten gevonden';
      this.settings.ErrorResults = this.settings.ErrorResults || 'Er is een fout opgetreden, probeer de webpagina te verversen. Als dit het probleem niet oplost kunt u Mijnsubsidie bellen.';
      if(this.settings.search){
        try{
          this.collection.setQuery(new Backbone.Model());
        } catch(e){
          console.error(e);
          swal({
            title: 'Error found',
            text: 'collection has no correct setQuery function, please implement. The setQuery function is given the model containing all search values, so logic needs to be implemented there.',
            type: 'error'
          });
        }
      }
      if(this.settings.lazyLoading !== false){
        this.lazyLoadingOptions = [];
      }
      this.settings.sortFunctions = {};
      _.each(this.settings.columns,col=>{
        if(col.sortFunction && app.isFunction(col.sortFunction))
          this.settings.sortFunctions[col.attr] = col.sortFunction;
      });
    },

    /**
     * Initializes lazy loading for the collection, based on the options given.
     * Through materializes scrollFire feature we enable easy on scroll detection
     * @return {[type]} [description]
     */
    addLazyLoading: function(rowClass){
      var offset = (this.settings.lazyLoading && this.settings.lazyLoading.offset)?this.settings.lazyLoading.offset:200;
      this.lazyLoadingOptions.push({
        selector: '.'+rowClass, offset: offset, callback: ()=>{
          this.triggerLazyLoading();
        }
      });
        Materialize.scrollFire(this.lazyLoadingOptions);
    },

    triggerLazyLoading: function(){
      this.collection.getNext();
    },
    /**
     * Empties all rows including resetting the row numbers
     * @return {[type]} [description]
     */
    emptyRows: function(){
      this.body.empty();
      this.rowId = 0;
    },
    /**
     * Searches based on the values given. In the setQuery method from the collection all values need to be retrieved and the actual logic is handled.
     * @param  {[Model]} model [The search model containing all values from the search fields]
     */
    onSearch: function(model){
      console.log('pre search: ', model);
      this.collection.setQuery(model);
      this.collection.reset();
      this.collection.fetch({reset:true, success:()=>{
        this.emptyRows()
        _.each(this.collection.models,m=>{
          this.renderRow(m);
        });
      }});
    },
    /**
     * Sets the value of a specified column, according to the attr connected to it.
     * @param {[model]} model  [The column model]
     * @param {[object]} column [The column which we are using]
     * @param {[$object]} $col   [The jquery object of the column]
     */
    setColumnValue: function(model,column,$col){
      $col.text(model.get(column.attr));
    },
    /**
     * Renders a specific row with all its columns, with a model to retrieve the values from.
     * Has an on change event on each columns value, that if it changes we update the column automaticly
     * @param  {[model]} model [description]
     */
    renderRow: function(model){
      var $el = $(this.templateRow({rowClass: this.settings.rowClass}));
      this.rowId++;
      $el.addClass(this.name + '-' + this.rowId);
      var rows = (this.settings.lazyLoading && this.settings.lazyLoading.rows)?this.settings.lazyLoading.rows:5;
      if(this.settings.lazyLoading !== false && this.rowId % rows === 0){
        var cl = this.name + '-' + this.rowId;
        setTimeout(()=>{
          this.addLazyLoading(cl);
        },10);
      }
      this.body.append($el);
      var count = 0;
      if(this.settings.click){
        $el.addClass('clickable');
      }
      // $el.click(()=>{this.trigger('click',model);});
      _.each(this.settings.columns,column=>{
        if(!column.rowValues) column.rowValues = [];
        if(!column.class) column.class = "";
        var valign = (this.settings.collapsible)?'style="vertical-align:top;"':'';
        var $col = $('<td class=" '+column.class+'"'+valign+'></td>');
        i++;
        count++;
        $el.append($col);
        model.set('col', $col);
        $col.click(()=>{
          if(this.settings.collapsible && !$el.$rcontent.animating){
            var $content = $el.$rcontent;
            var $contentdiv = $el.$rcontent.find('.dynamic-table-row-content');
            $el.$rcontent.animating = true;
            if (!$content.hasClass('hidden')) {
              $contentdiv.height($content.showHeight);
              $contentdiv.slideUp(()=>{
                $el.$rcontent.animating = false;
                $el.$rcontent.hide();
                $content.addClass('hidden');
              });
            } else {
              $el.$rcontent.show();
              $content.height(0);
              $content.removeClass('hidden');
              $contentdiv.slideDown(()=>{
                $el.$rcontent.animating = false;
              });
            }
          }
          if(column.click){
            if(app.isFunction(column.click))column.click(model);
          } else if(this.settings.click){
            if(app.isFunction(this.settings.click)) this.settings.click(model);
          }
        });
        column.rowValues.push($col);
        if(column.attr){
          this.setColumnValue(model,column,$col);
          model.on('change:'+column.attr,()=>{
            this.setColumnValue(model,column,$col);
          });
        }
        else $col.text('No attr defined');

      });
    },
    /**
     * Compares values based on an attribute in a model, private function is used internally.
     * @param  {[string]} attr          [The string representation of an attribute]
     * @param  {[null]} requiredParam [Is required to make the correct "sort" run in javascript, can always be ignored]
     */
    compareValues: function(attr,requiredParam){
      return function(a,b){
        var ax = [], bx = [];
        a = ''+a.get(attr);
        b = ''+b.get(attr);
        if(!a || !b) return 0;
        a.replace(/(\d+)|(\D+)/g, function(_, $1, $2) { ax.push([$1 || Infinity, $2 || ""]); });
        b.replace(/(\d+)|(\D+)/g, function(_, $1, $2) { bx.push([$1 || Infinity, $2 || ""]); });

        while(ax.length && bx.length) {
            var an = ax.shift();
            var bn = bx.shift();
            var nn = (an[0] - bn[0]) || an[1].localeCompare(bn[1]);
            if(nn) return nn;
        }
        return ax.length - bx.length;
      };
   },

   defaultSortFunction: function(v1,v2,attr){
     var ax = [], bx = [];
     var a = ''+v1.get(attr);
     var b = ''+v2.get(attr);
     if(!a || !b) return 0;
     a.replace(/(\d+)|(\D+)/g, function(_, $1, $2) { ax.push([$1 || Infinity, $2 || ""]); });
     b.replace(/(\d+)|(\D+)/g, function(_, $1, $2) { bx.push([$1 || Infinity, $2 || ""]); });

     while(ax.length && bx.length) {
         var an = ax.shift();
         var bn = bx.shift();
         var nn = (an[0] - bn[0]) || an[1].localeCompare(bn[1]);
         if(nn) return nn;
     }
     console.log("iets dergelijks",ax.length,bx.length);
     return ax.length - bx.length;
   },
    /**
     * Sorts a collection based on an attribute, using the function compareValues as comparator.
     * The collection can be sorted on ASC or DESC order, depending on the sort.type that is currently required.
     * @param  {[string]} attr       [The string representation of an attribute]
     * @param  {[collection]} collection [A collection containing Backbone models]
     */
    sort: function(attr){ //TODO sort werkt nog niet volledig perfect, eerste keer dat je sorteert doet die niks...
      var self = this;
      this.emptyRows()
      if(sort.type === 0) attr = 'id';
      var temp;
      if(sort.type === 2)
        var sorting = firstBy(function(v1,v2){return self.defaultSortFunction(v1,v2,attr);},-1);
      else
        var sorting = firstBy(function(v1,v2){return self.defaultSortFunction(v1,v2,attr);});
      _.each(this.settings.extraSorts,sort=>{
        if(sort.reverse)
          sorting.thenBy(sort.sortFunction || function(v1,v2){return self.defaultSortFunction(v1,v2,attr);},sort.reverse);
        else
          sorting.thenBy(sort.sortFunction || function(v1,v2){return self.defaultSortFunction(v1,v2,attr);});
      });
      this.collection.sort(sorting);
    },


    /**
     * In this function we add the visualisation for the sorting type for a header, we call the actual sorting.
     * @param  {[$object]} $th          [Contains the jquery object of the table head]
     * @param  {[string]} attr         [The name of the attribute on which we sort]
     * @param  {[optional function]} sortFunction [The function which gets called when we call the sorting, sortFunction(attr,collection)]
     */
    clickHeader: function($th,attr,sortFunction){
      if(sortFunction && !app.isFunction(sortFunction)) {console.error("The parameter sortFunction is not of type function");return;}
      var first;
      //The attribute on which it gets sorted
      if(!sort.attr) sort.attr = attr;
      //The table header which we need for the visualisation of the sorting.
      if(!sort.$th) sort.$th = $th;
      //The type of sorting. 0 = no sorting, 1 = low>high sorting, 2 = high>low sorting.
      if(!sort.type) {sort.type = 0; first = true; sort.$th.addClass('sorting');}
      if(sort.attr !== attr){
        sort.$th.removeClass('sorting');
        sort.$th.find('i').text('');
        sort.$th = $th;
        sort.attr = attr;
        sort.type = 0;
        sort.$th.addClass('sorting');

      }

      sort.type++;
      if(sort.type>2) {sort.type = 0;sort.$th.removeClass('sorting');}
      $th.find('i').text(icons[sort.type]);
      if(sortFunction){
        sortFunction(attr,this.collection);
        if(first) sortFunction(attr,this.collection);
      } else {this.sort(attr);if(first) this.sort(attr);}
    },

    /**
     * Renders the base of the table, calling all required functions to render all of the table.
     * @return {[this]}            [Returns an instance of this object]
     */
    render: function(){
      var self = this;
      this.$el.html(this.template(this.settings));
      this.body = this.$el.find('tbody');

      _.each(this.$el.find('th'),th=>{ //looping through all table headers, so we can register a on click for sorting
        var $th = $(th);
        var classes = $th.attr('class');
        var attr;
        _.each(classes.split(" "),cl=>{ //loop through all classes found on the table header. We do this so we know what attribute we need to sort.
          if(cl.contains('header-')){
            if(attr) console.warn('Warning a class containing header- was already detected on the table header, this might destroy the sorting functionality');
            attr = cl.replace('header-',"");
          }
        });
        if(this.settings.defaultSort){
          if($th.attr("class").contains('header-'+this.settings.defaultSort)){
            this.clickHeader($th,attr,this.settings.sortFunctions[this.settings.defaultSort] || this.settings.sortFunction);
          }

        }
        if(!$th.attr('class').contains('select-column-field-auto')){
          $th.click(()=>{
            this.clickHeader($th,attr,this.settings.sortFunctions[attr] || this.settings.sortFunction);
          });
        }
      });
      this.collection.fetch({
        success:()=>{
          if(this.collection.subCollection.models.length === 0) this.emptyFetched();
        },
        error:e=>{
          console.log(e);
          this.emptyFetched(true);
        }});
      this.collection.on('add', m=>{
        if(this.noresults) {this.emptyRows(); this.noresults = false;}
        this.renderRow(m);
      });

      if(this.settings.search){
        var f = new Form({
          model: new Backbone.Model(),
          fields: [
            {
              icon: 'search',
              attr: 'search',
              keyup: true
            }
          ]
        });
        var timeout;
        if(this.secondAttr)
          f.model.set('secondAttr', this.secondAttr);
        f.model.on('change:search', ()=>{
          if(timeout){
            clearTimeout(timeout);
            timeout = null;
          }
          timeout = setTimeout(()=>{
            this.onSearch(f.model);
          },200);
        });
        ((this.settings.search.searchClass)? $(this.settings.search.searchClass) : this.$el.find('.topBarTable')).append(f.render().el);
      }


      return this;
    },

    emptyFetched: function(err = false){
      this.emptyRows();
      var errclass = (err)?'error-results-dynamic-table':'empty-results-dynamic-table';
      var errmsg = (err)?this.settings.ErrorResults:this.settings.NoResults;
      var $el = $('<div class="'+errclass+'">'+errmsg+'</div>');
      console.log($el);
      $el.insertAfter(this.$el.find('table'));
      // this.body.append($el);
      this.noresults = true;
    }
});

module.exports = Collapsible;
