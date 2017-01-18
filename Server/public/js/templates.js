this["Handlebars"] = this["Handlebars"] || {};
this["Handlebars"]["templates"] = this["Handlebars"]["templates"] || {};
this["Handlebars"]["templates"]["admin/patientOverview"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\">\r\n  <div class=\"row patientTopBar\">\r\n\r\n  </div>\r\n  <div class=\"row patientTable\">\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["admin/practiceOverview"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\">\r\n  <div class=\"row practiceTopBar\">\r\n\r\n  </div>\r\n  <div class=\"row practiceTable\">\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/patientOverview"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\">\r\n  <div class=\"topRow\">\r\n\r\n  </div>\r\n\r\n  <div class=\"row userTable\">\r\n\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/basic-table"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container subsidy-tool\">\r\n	<div class=\"section\">\r\n		<div class=\"row\">\r\n\r\n      <div class=\"basictable\">\r\n      </div>\r\n		</div>\r\n	</div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/breadcrumb"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<ul class=\"breadcrumb-list row\">\r\n  <div class=\"breadcrumb-div col s2 first-breadcrumb\">\r\n    <span class=\"breadcrumb-span\"><</span>\r\n    <li class=\"breadcrumb-item breadcrumb-first\"></li>\r\n  </div>\r\n  <div class=\"breadcrumb-div col s2 second-breadcrumb\">\r\n    <span class=\"breadcrumb-span\"><</span>\r\n    <li class=\"breadcrumb-item breadcrumb-second\"></li>\r\n  </div>\r\n  <div class=\"breadcrumb-div col s2 third-breadcrumb\">\r\n    <li class=\"breadcrumb-item breadcrumb-third\"></li>\r\n  </div>\r\n</ul>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/button"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.tooltip : depth0),{"name":"if","hash":{},"fn":this.program(2, data, 0),"inverse":this.program(4, data, 0),"data":data})) != null ? stack1 : "");
},"2":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "    <a href=\""
    + alias3(((helper = (helper = helpers.href || (depth0 != null ? depth0.href : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"href","hash":{},"data":data}) : helper)))
    + "\" target=\""
    + alias3(((helper = (helper = helpers.target || (depth0 != null ? depth0.target : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"target","hash":{},"data":data}) : helper)))
    + "\" class=\"waves-effect waves-light btn tooltipped "
    + alias3(((helper = (helper = helpers['class'] || (depth0 != null ? depth0['class'] : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"class","hash":{},"data":data}) : helper)))
    + "\" data-tooltip=\""
    + alias3(((helper = (helper = helpers.tooltip || (depth0 != null ? depth0.tooltip : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"tooltip","hash":{},"data":data}) : helper)))
    + "\" data-delay=\"50\" data-position=\"bottom\">\r\n";
},"4":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "    <a href=\""
    + alias3(((helper = (helper = helpers.href || (depth0 != null ? depth0.href : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"href","hash":{},"data":data}) : helper)))
    + "\" target=\""
    + alias3(((helper = (helper = helpers.target || (depth0 != null ? depth0.target : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"target","hash":{},"data":data}) : helper)))
    + "\" class=\"waves-effect waves-light btn "
    + alias3(((helper = (helper = helpers['class'] || (depth0 != null ? depth0['class'] : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"class","hash":{},"data":data}) : helper)))
    + "\">\r\n";
},"6":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.tooltip : depth0),{"name":"if","hash":{},"fn":this.program(7, data, 0),"inverse":this.program(9, data, 0),"data":data})) != null ? stack1 : "");
},"7":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "    <a class=\"waves-effect waves-light btn tooltipped "
    + alias3(((helper = (helper = helpers['class'] || (depth0 != null ? depth0['class'] : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"class","hash":{},"data":data}) : helper)))
    + "\" data-tooltip=\""
    + alias3(((helper = (helper = helpers.tooltip || (depth0 != null ? depth0.tooltip : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"tooltip","hash":{},"data":data}) : helper)))
    + "\" data-delay=\"50\" data-position=\"bottom\">\r\n";
},"9":function(depth0,helpers,partials,data) {
    var helper;

  return "    <a class=\"waves-effect waves-light btn "
    + this.escapeExpression(((helper = (helper = helpers['class'] || (depth0 != null ? depth0['class'] : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"class","hash":{},"data":data}) : helper)))
    + "\">\r\n";
},"11":function(depth0,helpers,partials,data) {
    var helper;

  return "  <i class=\"material-icons\">"
    + this.escapeExpression(((helper = (helper = helpers.icon || (depth0 != null ? depth0.icon : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"icon","hash":{},"data":data}) : helper)))
    + "</i>\r\n";
},"13":function(depth0,helpers,partials,data) {
    var helper;

  return "  <i class=\"material-icons\" src=\""
    + this.escapeExpression(((helper = (helper = helpers.logo || (depth0 != null ? depth0.logo : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"logo","hash":{},"data":data}) : helper)))
    + "\"></i>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1, helper;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.href : depth0),{"name":"if","hash":{},"fn":this.program(1, data, 0),"inverse":this.program(6, data, 0),"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.icon : depth0),{"name":"if","hash":{},"fn":this.program(11, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.logo : depth0),{"name":"if","hash":{},"fn":this.program(13, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "  "
    + this.escapeExpression(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "\r\n</a>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/checkbox"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var helper;

  return "		<div class= \"flip-icon "
    + this.escapeExpression(((helper = (helper = helpers.iconClass || (depth0 != null ? depth0.iconClass : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"iconClass","hash":{},"data":data}) : helper)))
    + "\">\r\n		</div>\r\n";
},"3":function(depth0,helpers,partials,data) {
    return "		<div class=\"flip-name col s12\">\r\n";
},"5":function(depth0,helpers,partials,data) {
    return "				<div class=\"flip-name col s10\">\r\n";
},"7":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "				<a  class=\"tooltipped tooltip-tag\" style=\"width: 100%; heigth: 70px; display:inline-block\" data-position=\"right\" data-delay=\"50\" data-tooltip=\""
    + alias3(((helper = (helper = helpers.tooltip || (depth0 != null ? depth0.tooltip : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"tooltip","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.name || (depth0 != null ? depth0.name : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"name","hash":{},"data":data}) : helper)))
    + "</a>\r\n";
},"9":function(depth0,helpers,partials,data) {
    var helper;

  return "				"
    + this.escapeExpression(((helper = (helper = helpers.name || (depth0 != null ? depth0.name : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"name","hash":{},"data":data}) : helper)))
    + "\r\n";
},"11":function(depth0,helpers,partials,data) {
    return "		<div class=\"col s12\">\r\n";
},"13":function(depth0,helpers,partials,data) {
    return "				<div class=\"col s2\">\r\n";
},"15":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "						<!--<<input class=\"flip-value\" type=\"checkbox\" disabled checked=\"true\">-->\r\n\r\n					<input checked=\"checked\" disabled type=\"checkbox\" class=\"filled-in\"  id=\"box_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" />\r\n					<label for=\"box_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\"></label>\r\n";
},"17":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "					<!--<input class=\"flip-value\" type=\"checkbox\" checked=\"true\">-->\r\n					<input type=\"checkbox\"  class=\"filled-in\"  id=\"box_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" />\r\n					<label for=\"box_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\"></label>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1;

  return "	<div class=\"col s12 switch secondary-content\">\r\n	<div class=\"col s12 flip-main\">\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.icon : depth0),{"name":"if","hash":{},"fn":this.program(1, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.icon : depth0),{"name":"if","hash":{},"fn":this.program(3, data, 0),"inverse":this.program(5, data, 0),"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.tooltip : depth0),{"name":"if","hash":{},"fn":this.program(7, data, 0),"inverse":this.program(9, data, 0),"data":data})) != null ? stack1 : "")
    + "		</div>\r\n\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.icon : depth0),{"name":"if","hash":{},"fn":this.program(11, data, 0),"inverse":this.program(13, data, 0),"data":data})) != null ? stack1 : "")
    + "			<label>\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.disable : depth0),{"name":"if","hash":{},"fn":this.program(15, data, 0),"inverse":this.program(17, data, 0),"data":data})) != null ? stack1 : "")
    + "			</label>\r\n		</div>\r\n	</div>\r\n</div>\r\n<br>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/date"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var helper;

  return "    <i class=\"material-icons prefix\">"
    + this.escapeExpression(((helper = (helper = helpers.icon || (depth0 != null ? depth0.icon : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"icon","hash":{},"data":data}) : helper)))
    + "</i>\r\n";
},"3":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "    <input name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" type=\"text\" disabled>\r\n";
},"5":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "    <input name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" type=\"text\">\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1, helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "<div class=\"input-field col s12 attr-"
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\">\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.icon : depth0),{"name":"if","hash":{},"fn":this.program(1, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.disable : depth0),{"name":"if","hash":{},"fn":this.program(3, data, 0),"inverse":this.program(5, data, 0),"data":data})) != null ? stack1 : "")
    + "  <label class=\"active\" for=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/dtab"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "<li class=\"dtab col dtab-"
    + alias3(((helper = (helper = helpers.cid || (depth0 != null ? depth0.cid : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"cid","hash":{},"data":data}) : helper)))
    + "\">\r\n  <span class=\"dtab-title\">"
    + alias3(((helper = (helper = helpers.title || (depth0 != null ? depth0.title : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"title","hash":{},"data":data}) : helper)))
    + "</span>\r\n</li>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/dtabs"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"col s12 dtab-div\">\r\n  <ul class=\"tabs col s12 dtabs row\">\r\n\r\n  </ul>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/dynamicTable"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var stack1;

  return "            "
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.isOptionsColumn : depth0),{"name":"if","hash":{},"fn":this.program(2, data, 0),"inverse":this.program(4, data, 0),"data":data})) != null ? stack1 : "");
},"2":function(depth0,helpers,partials,data) {
    return " <!-- not if basicly -->\r\n              <th class=\"header-options\">\r\n              </th>\r\n";
},"4":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "              <th class=\"header-"
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + " "
    + alias3(((helper = (helper = helpers.headerClass || (depth0 != null ? depth0.headerClass : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"headerClass","hash":{},"data":data}) : helper)))
    + " dynamic-table-header\">\r\n                "
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "\r\n                <i class=\"material-icons\"></i>\r\n              </th>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1;

  return "<div class=\"topBarTable row\">\r\n</div>\r\n<div class=\"row\">\r\n  <div class=\"col s12\">\r\n    <table class=\"responsive-table striped\">\r\n      <thead>\r\n        <tr>\r\n"
    + ((stack1 = helpers.each.call(depth0,(depth0 != null ? depth0.columns : depth0),{"name":"each","hash":{},"fn":this.program(1, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "        </tr>\r\n      </thead>\r\n      <tbody></tbody>\r\n    </table>\r\n  </div>\r\n</div>\r\n<!-- <div class='tablebuttons'>\r\n<a class=\"btn-floating btn-large waves-effect waves-light more-btn\"><i class=\"material-icons\">expand_more</i></a>\r\n</div> -->\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/dynamicTableRow"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var helper;

  return "<tr class=\"dynamic-table-row "
    + this.escapeExpression(((helper = (helper = helpers.rowClass || (depth0 != null ? depth0.rowClass : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"rowClass","hash":{},"data":data}) : helper)))
    + "\">\r\n \r\n</tr>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/row"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\">\r\n	<div class=\"section\">\r\n		<div class=\"row\">\r\n    </div>\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/singleRadio"] = Handlebars.template({"1":function(depth0,helpers,partials,data,blockParams,depths) {
    var stack1, helper, alias1=this.escapeExpression, alias2=helpers.helperMissing, alias3="function";

  return "      <div class=\"col s12\">\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.disable : depth0),{"name":"if","hash":{},"fn":this.program(2, data, 0, blockParams, depths),"inverse":this.program(4, data, 0, blockParams, depths),"data":data})) != null ? stack1 : "")
    + "        <label for=\"ms_"
    + alias1(this.lambda((depths[1] != null ? depths[1].field : depths[1]), depth0))
    + "_"
    + alias1(((helper = (helper = helpers.id || (depth0 != null ? depth0.id : depth0)) != null ? helper : alias2),(typeof helper === alias3 ? helper.call(depth0,{"name":"id","hash":{},"data":data}) : helper)))
    + "\">"
    + alias1(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias2),(typeof helper === alias3 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n      </div>\r\n";
},"2":function(depth0,helpers,partials,data,blockParams,depths) {
    var helper, alias1=this.lambda, alias2=this.escapeExpression, alias3=helpers.helperMissing, alias4="function";

  return "          <input name=\""
    + alias2(alias1((depths[2] != null ? depths[2].field : depths[2]), depth0))
    + "_"
    + alias2(alias1((depths[2] != null ? depths[2].attr : depths[2]), depth0))
    + "\" value=\""
    + alias2(((helper = (helper = helpers.id || (depth0 != null ? depth0.id : depth0)) != null ? helper : alias3),(typeof helper === alias4 ? helper.call(depth0,{"name":"id","hash":{},"data":data}) : helper)))
    + "\" type=\"radio\" id=\"ms_"
    + alias2(alias1((depths[2] != null ? depths[2].field : depths[2]), depth0))
    + "_"
    + alias2(((helper = (helper = helpers.id || (depth0 != null ? depth0.id : depth0)) != null ? helper : alias3),(typeof helper === alias4 ? helper.call(depth0,{"name":"id","hash":{},"data":data}) : helper)))
    + "\" disabled/>\r\n";
},"4":function(depth0,helpers,partials,data,blockParams,depths) {
    var helper, alias1=this.lambda, alias2=this.escapeExpression, alias3=helpers.helperMissing, alias4="function";

  return "          <input name=\""
    + alias2(alias1((depths[2] != null ? depths[2].field : depths[2]), depth0))
    + "_"
    + alias2(alias1((depths[2] != null ? depths[2].attr : depths[2]), depth0))
    + "\" value=\""
    + alias2(((helper = (helper = helpers.id || (depth0 != null ? depth0.id : depth0)) != null ? helper : alias3),(typeof helper === alias4 ? helper.call(depth0,{"name":"id","hash":{},"data":data}) : helper)))
    + "\" type=\"radio\" id=\"ms_"
    + alias2(alias1((depths[2] != null ? depths[2].field : depths[2]), depth0))
    + "_"
    + alias2(((helper = (helper = helpers.id || (depth0 != null ? depth0.id : depth0)) != null ? helper : alias3),(typeof helper === alias4 ? helper.call(depth0,{"name":"id","hash":{},"data":data}) : helper)))
    + "\" />\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data,blockParams,depths) {
    var stack1, helper;

  return "<div class=\"col s12\">\r\n  <label>"
    + this.escapeExpression(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n  <div class=\"row\">\r\n"
    + ((stack1 = helpers.each.call(depth0,(depth0 != null ? depth0.values : depth0),{"name":"each","hash":{},"fn":this.program(1, data, 0, blockParams, depths),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "  </div>\r\n</div>\r\n";
},"useData":true,"useDepths":true});
this["Handlebars"]["templates"]["layout/singleSelect"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <select id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\"disabled>\r\n";
},"3":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <select id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\">\r\n";
},"5":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.disabled : depth0),{"name":"if","hash":{},"fn":this.program(6, data, 0),"inverse":this.program(8, data, 0),"data":data})) != null ? stack1 : "");
},"6":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "        <option disabled value=\""
    + alias3(((helper = (helper = helpers.id || (depth0 != null ? depth0.id : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"id","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</option>\r\n";
},"8":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "        <option value=\""
    + alias3(((helper = (helper = helpers.id || (depth0 != null ? depth0.id : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"id","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</option>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1, helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "<div class=\"input-field single-select-container col s12\">\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.disable : depth0),{"name":"if","hash":{},"fn":this.program(1, data, 0),"inverse":this.program(3, data, 0),"data":data})) != null ? stack1 : "")
    + "      <option disabled selected value=\"-1\">Kies een optie</option>\r\n"
    + ((stack1 = helpers.each.call(depth0,(depth0 != null ? depth0.values : depth0),{"name":"each","hash":{},"fn":this.program(5, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "  </select>\r\n  <label for=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/table"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var helper;

  return "            <th>"
    + this.escapeExpression(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</th>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1;

  return "<div class=\"row\">\r\n  <div class=\"col s12\">\r\n    <table class=\"responsive-table striped\">\r\n      <thead>\r\n        <tr class=\"table-row-head\">\r\n"
    + ((stack1 = helpers.each.call(depth0,(depth0 != null ? depth0.columns : depth0),{"name":"each","hash":{},"fn":this.program(1, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "        </tr>\r\n      </thead>\r\n      <tbody></tbody>\r\n    </table>\r\n  </div>\r\n</div>\r\n<div class='tablebuttons'>\r\n<a class=\"btn-floating btn-large waves-effect waves-light more-btn\"><i class=\"tooltipped material-icons\" data-position=\"bottom\" data-delay=\"50\" data-tooltip=\"Klik voor meer resultaten\">expand_more</i></a>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/tableSearchView"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"topBarTable row\">\r\n</div>\r\n<div class=\"col s12 tableViewMain\">\r\n  <div class=\"col s12 tableViewList\">\r\n  </div>\r\n</div>\r\n<div class=\"bottomBarTable row\">\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/tabs"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var stack1, helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "    <li class=\"tab col s"
    + alias3(((helper = (helper = helpers.col || (depth0 != null ? depth0.col : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"col","hash":{},"data":data}) : helper)))
    + " "
    + alias3(((helper = (helper = helpers['class'] || (depth0 != null ? depth0['class'] : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"class","hash":{},"data":data}) : helper)))
    + " tab"
    + alias3(((helper = (helper = helpers.cid || (depth0 != null ? depth0.cid : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"cid","hash":{},"data":data}) : helper)))
    + "\" data-cid=\""
    + alias3(((helper = (helper = helpers.cid || (depth0 != null ? depth0.cid : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"cid","hash":{},"data":data}) : helper)))
    + "\">\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.hover : depth0),{"name":"if","hash":{},"fn":this.program(2, data, 0),"inverse":this.program(4, data, 0),"data":data})) != null ? stack1 : "")
    + "      <!-- <a class=\"btn tooltipped\" data-position=\"bottom\" data-delay=\"50\" data-tooltip=\"I am tooltip\">Hover me!</a> -->\r\n    </li>\r\n\r\n";
},"2":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "        <a href=\"javascript:void(0)\" class=\"tooltipped\" data-position=\""
    + alias3(((helper = (helper = helpers.hoverPosition || (depth0 != null ? depth0.hoverPosition : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"hoverPosition","hash":{},"data":data}) : helper)))
    + "\" data-delay=\"50\" data-tooltip=\""
    + alias3(((helper = (helper = helpers.hover || (depth0 != null ? depth0.hover : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"hover","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.title || (depth0 != null ? depth0.title : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"title","hash":{},"data":data}) : helper)))
    + "</a>\r\n";
},"4":function(depth0,helpers,partials,data) {
    var helper;

  return "      <a href=\"javascript:void(0)\">"
    + this.escapeExpression(((helper = (helper = helpers.title || (depth0 != null ? depth0.title : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"title","hash":{},"data":data}) : helper)))
    + "</a>\r\n";
},"6":function(depth0,helpers,partials,data) {
    var helper;

  return "<div class=\"col s12 tab"
    + this.escapeExpression(((helper = (helper = helpers.cid || (depth0 != null ? depth0.cid : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"cid","hash":{},"data":data}) : helper)))
    + " tab-content\"></div>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1, helper;

  return "<div class=\"col s12 tab-div\">\r\n  <ul class=\"tabs row "
    + this.escapeExpression(((helper = (helper = helpers.mainclass || (depth0 != null ? depth0.mainclass : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"mainclass","hash":{},"data":data}) : helper)))
    + "\">\r\n"
    + ((stack1 = helpers.each.call(depth0,(depth0 != null ? depth0.tabs : depth0),{"name":"each","hash":{},"fn":this.program(1, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "  </ul>\r\n</div>\r\n"
    + ((stack1 = helpers.each.call(depth0,(depth0 != null ? depth0.tabs : depth0),{"name":"each","hash":{},"fn":this.program(6, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "");
},"useData":true});
this["Handlebars"]["templates"]["layout/textArea"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var helper;

  return "    <i class=\"material-icons prefix\">"
    + this.escapeExpression(((helper = (helper = helpers.icon || (depth0 != null ? depth0.icon : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"icon","hash":{},"data":data}) : helper)))
    + "</i>\r\n";
},"3":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.limit : depth0),{"name":"if","hash":{},"fn":this.program(4, data, 0),"inverse":this.program(6, data, 0),"data":data})) != null ? stack1 : "");
},"4":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <textarea name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" maxlength=\""
    + alias3(((helper = (helper = helpers.limit || (depth0 != null ? depth0.limit : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"limit","hash":{},"data":data}) : helper)))
    + "\" disabled class=\"materialize-textarea\"></textarea>\r\n";
},"6":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <textarea name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" disabled class=\"materialize-textarea\"></textarea>\r\n";
},"8":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.limit : depth0),{"name":"if","hash":{},"fn":this.program(9, data, 0),"inverse":this.program(11, data, 0),"data":data})) != null ? stack1 : "");
},"9":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <textarea name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" maxlength=\""
    + alias3(((helper = (helper = helpers.limit || (depth0 != null ? depth0.limit : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"limit","hash":{},"data":data}) : helper)))
    + "\" class=\"materialize-textarea\"></textarea>\r\n";
},"11":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <textarea name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" class=\"materialize-textarea\"></textarea>\r\n";
},"13":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "  <label class=\"active\" for=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n";
},"15":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "  <label for=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n";
},"17":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "  <br>\r\n    <label>Aantal characters over: </label> <label id=\"limit_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\"> "
    + alias3(((helper = (helper = helpers.limit || (depth0 != null ? depth0.limit : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"limit","hash":{},"data":data}) : helper)))
    + "</label>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1, helper;

  return "<div>\r\n<div class=\"input-field col s12 attr-"
    + this.escapeExpression(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\">\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.icon : depth0),{"name":"if","hash":{},"fn":this.program(1, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.disable : depth0),{"name":"if","hash":{},"fn":this.program(3, data, 0),"inverse":this.program(8, data, 0),"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.value : depth0),{"name":"if","hash":{},"fn":this.program(13, data, 0),"inverse":this.program(15, data, 0),"data":data})) != null ? stack1 : "")
    + "</div>\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.limit : depth0),{"name":"if","hash":{},"fn":this.program(17, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["layout/textField"] = Handlebars.template({"1":function(depth0,helpers,partials,data) {
    var helper;

  return "    <i class=\"material-icons prefix\">"
    + this.escapeExpression(((helper = (helper = helpers.icon || (depth0 != null ? depth0.icon : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"icon","hash":{},"data":data}) : helper)))
    + "</i>\r\n";
},"3":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.unrequire : depth0),{"name":"if","hash":{},"fn":this.program(4, data, 0),"inverse":this.program(6, data, 0),"data":data})) != null ? stack1 : "");
},"4":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <input name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" type=\""
    + alias3(((helper = (helper = helpers.type || (depth0 != null ? depth0.type : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"type","hash":{},"data":data}) : helper)))
    + "\" class=\"validate\" value=\""
    + alias3(((helper = (helper = helpers.value || (depth0 != null ? depth0.value : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"value","hash":{},"data":data}) : helper)))
    + "\" disabled readonly onfocus=\"this.removeAttribute('readonly');\" style=\"cursor:text\">\r\n";
},"6":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <input name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" type=\""
    + alias3(((helper = (helper = helpers.type || (depth0 != null ? depth0.type : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"type","hash":{},"data":data}) : helper)))
    + "\" required class=\"validate\" value=\""
    + alias3(((helper = (helper = helpers.value || (depth0 != null ? depth0.value : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"value","hash":{},"data":data}) : helper)))
    + "\" disabled readonly onfocus=\"this.removeAttribute('readonly');\" style=\"cursor:text\">\r\n";
},"8":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.unrequire : depth0),{"name":"if","hash":{},"fn":this.program(9, data, 0),"inverse":this.program(11, data, 0),"data":data})) != null ? stack1 : "");
},"9":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <input name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" type=\""
    + alias3(((helper = (helper = helpers.type || (depth0 != null ? depth0.type : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"type","hash":{},"data":data}) : helper)))
    + "\" class=\"validate\" value=\""
    + alias3(((helper = (helper = helpers.value || (depth0 != null ? depth0.value : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"value","hash":{},"data":data}) : helper)))
    + "\" readonly onfocus=\"this.removeAttribute('readonly');\" style=\"cursor:text\">\r\n";
},"11":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <input name=\""
    + alias3(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\" id=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\" required type=\""
    + alias3(((helper = (helper = helpers.type || (depth0 != null ? depth0.type : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"type","hash":{},"data":data}) : helper)))
    + "\" class=\"validate\" value=\""
    + alias3(((helper = (helper = helpers.value || (depth0 != null ? depth0.value : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"value","hash":{},"data":data}) : helper)))
    + "\" readonly onfocus=\"this.removeAttribute('readonly');\" style=\"cursor:text\">\r\n";
},"13":function(depth0,helpers,partials,data) {
    return "    <i class=\"material-icons show-hidden-password\">visibility</i>\r\n";
},"15":function(depth0,helpers,partials,data) {
    var stack1;

  return ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.value : depth0),{"name":"if","hash":{},"fn":this.program(16, data, 0),"inverse":this.program(18, data, 0),"data":data})) != null ? stack1 : "");
},"16":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <label class=\"active\" data-error=\""
    + alias3(((helper = (helper = helpers.errormsg || (depth0 != null ? depth0.errormsg : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"errormsg","hash":{},"data":data}) : helper)))
    + "\" data-success=\""
    + alias3(((helper = (helper = helpers.successmsg || (depth0 != null ? depth0.successmsg : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"successmsg","hash":{},"data":data}) : helper)))
    + "\" for=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n";
},"18":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "      <label data-error=\""
    + alias3(((helper = (helper = helpers.errormsg || (depth0 != null ? depth0.errormsg : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"errormsg","hash":{},"data":data}) : helper)))
    + "\" data-success=\""
    + alias3(((helper = (helper = helpers.successmsg || (depth0 != null ? depth0.successmsg : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"successmsg","hash":{},"data":data}) : helper)))
    + "\" for=\"ms_"
    + alias3(((helper = (helper = helpers.field || (depth0 != null ? depth0.field : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"field","hash":{},"data":data}) : helper)))
    + "\">"
    + alias3(((helper = (helper = helpers.label || (depth0 != null ? depth0.label : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"label","hash":{},"data":data}) : helper)))
    + "</label>\r\n";
},"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var stack1, helper;

  return "<div class=\"input-field col s12 attr-"
    + this.escapeExpression(((helper = (helper = helpers.attr || (depth0 != null ? depth0.attr : depth0)) != null ? helper : helpers.helperMissing),(typeof helper === "function" ? helper.call(depth0,{"name":"attr","hash":{},"data":data}) : helper)))
    + "\">\r\n"
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.icon : depth0),{"name":"if","hash":{},"fn":this.program(1, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.disable : depth0),{"name":"if","hash":{},"fn":this.program(3, data, 0),"inverse":this.program(8, data, 0),"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.showHidden : depth0),{"name":"if","hash":{},"fn":this.program(13, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + ((stack1 = helpers['if'].call(depth0,(depth0 != null ? depth0.label : depth0),{"name":"if","hash":{},"fn":this.program(15, data, 0),"inverse":this.noop,"data":data})) != null ? stack1 : "")
    + "</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["admin/user/patientOverview"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\" style=\"max-width:100%\">\r\n  <div class=\"section\">\r\n    <div class=\"row userTable\">\r\n    </div>\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["admin/patient/patientPage"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\">\r\n  <div class=\"section\">\r\n    <div class=\"patient-info-section\">\r\n\r\n    </div>\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["admin/practice/practicePage"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\">\r\n  <div class=\"section\">\r\n    <div class=\"practice-info-section\">\r\n\r\n    </div>\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/patient/patientDetails"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"row patientDetails\">\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/patient/patientGoals"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "<div class=\"row patientGoals z-depth-2\">\r\n  <div class=\"col s12\">\r\n      <!-- <div class=\"col s6\">\r\n        <h3>Doelstellingen overzicht "
    + alias3(((helper = (helper = helpers.name || (depth0 != null ? depth0.name : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"name","hash":{},"data":data}) : helper)))
    + "</h3>\r\n      </div> -->\r\n      <div class=\"col s12\">\r\n      <!-- <div class=\"col s6\"> -->\r\n        <h3>Nieuwe doelstellingen toevoegen "
    + alias3(((helper = (helper = helpers.name || (depth0 != null ? depth0.name : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"name","hash":{},"data":data}) : helper)))
    + "<h3>\r\n      </div>\r\n  </div>\r\n    <div class=\"col s12 divider\"></div>\r\n  <!-- <div class=\"col s6 info z-depth-1\">\r\n    <div class=\"col s12 goalsInfo\">\r\n    </div>\r\n    <div class=\"col s12 infoButton\">\r\n\r\n    </div>\r\n  </div> -->\r\n  <div class=\"col s12 dates z-depth-1\">\r\n    <div class=\"col s12 goalDates\">\r\n    </div>\r\n    <div class=\"col s12 datesButton\">\r\n      <div class=\"col s8 stepsForm\">\r\n\r\n      </div>\r\n      <div class=\"col s4 stepsButton\">\r\n\r\n      </div>\r\n    </div>\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/patient/patientProgression"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    var helper, alias1=helpers.helperMissing, alias2="function", alias3=this.escapeExpression;

  return "<div class=\"row progression-div z-depth-2\">\r\n  <div class=\"patient-info small-margin\">\r\n    <h3>"
    + alias3(((helper = (helper = helpers.patientName || (depth0 != null ? depth0.patientName : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"patientName","hash":{},"data":data}) : helper)))
    + " Voorderingen</h3></br>\r\n    <!-- <div class=\"col s12 divider\"></div> -->\r\n    <div class=\"col s7 offset-s3\">\r\n      <blockquote>\r\n\r\n        <span class=\"label-large\"><span class=\"bold\">Aantal stappen vandaag:</span> "
    + alias3(((helper = (helper = helpers.steps || (depth0 != null ? depth0.steps : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"steps","hash":{},"data":data}) : helper)))
    + "</span></br></br>\r\n        <span class=\"label-large\"><span class=\"bold\">Doelstelling vandaag:</span> "
    + alias3(((helper = (helper = helpers.requiredSteps || (depth0 != null ? depth0.requiredSteps : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"requiredSteps","hash":{},"data":data}) : helper)))
    + "</span></br></br>\r\n        <span class=\"label-large\"><span class=\"bold\">Aantal dagen doelstelling behaald:</span> "
    + alias3(((helper = (helper = helpers.daysCompleted || (depth0 != null ? depth0.daysCompleted : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"daysCompleted","hash":{},"data":data}) : helper)))
    + "/"
    + alias3(((helper = (helper = helpers.daysTotal || (depth0 != null ? depth0.daysTotal : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"daysTotal","hash":{},"data":data}) : helper)))
    + "</span></br></br>\r\n        <span class=\"label-large\"><span class=\"bold\">Totaal aantal stappen:</span> "
    + alias3(((helper = (helper = helpers.totalSteps || (depth0 != null ? depth0.totalSteps : depth0)) != null ? helper : alias1),(typeof helper === alias2 ? helper.call(depth0,{"name":"totalSteps","hash":{},"data":data}) : helper)))
    + "</span></br></br>\r\n      </blockquote>\r\n    </div>\r\n  </div>\r\n  <!-- <div class=\"col s12 divider\"></div> -->\r\n  <div class=\"container progression-stats z-depth-1\">\r\n\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/patient/patientProgressionTab24"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"col s12 progression-stats z-depth-1\">\r\n  <canvas id=\"firstChart\" width=\"300\" height=\"300\"></canvas>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/patient/patientProgressionTabWeek"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"col s12 progression-stats z-depth-1\">\r\n  <div class= \"col s6 stepProgress\">\r\n    <canvas id=\"firstChart\" width=\"300\" height=\"300\"></canvas>\r\n  </div>\r\n  <div class= \"col s6 stepProgress\">\r\n    <canvas id=\"secondChart\" width=\"300\" height=\"300\"></canvas>\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/patient/patientProgressionTabYear"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"col s12 progression-stats z-depth-1\">\r\n  <div class= \"col s6 stepProgress\">\r\n    <canvas id=\"firstChart\" width=\"300\" height=\"300\"></canvas>\r\n  </div>\r\n  <div class= \"col s6 stepProgress\">\r\n    <canvas id=\"secondChart\" width=\"300\" height=\"300\"></canvas>\r\n  </div>\r\n</div>\r\n";
},"useData":true});
this["Handlebars"]["templates"]["doctor/user/patientOverview"] = Handlebars.template({"compiler":[6,">= 2.0.0-beta.1"],"main":function(depth0,helpers,partials,data) {
    return "<div class=\"container\" style=\"max-width:100%\">\r\n  <div class=\"section\">\r\n    <div class=\"row userTable\">\r\n    </div>\r\n  </div>\r\n</div>\r\n";
},"useData":true});