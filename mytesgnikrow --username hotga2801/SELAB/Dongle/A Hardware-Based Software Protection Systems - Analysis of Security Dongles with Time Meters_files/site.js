Prototype.Browser.IE6 = (function(){
	return (/msie 6/i.test(navigator.userAgent)) && !(/opera/i.test(navigator.userAgent)) && (/win/i.test(navigator.userAgent));
})();

Prototype.Browser.IE7 = (function(){
	return (/msie 7/i.test(navigator.userAgent)) && !(/opera/i.test(navigator.userAgent)) && (/win/i.test(navigator.userAgent));
})();

Element.addMethods({
	getDimensions: function(element) {
		element = $(element);
		var display = $(element).getStyle('display');
		if (display != 'none' && display != null)
			return {width: element.offsetWidth, height: element.offsetHeight};
		var els = element.style;
		var originalVisibility = els.visibility;
		var originalPosition = els.position;
		var originalDisplay = els.display;
		els.visibility = 'hidden';
		els.position = 'absolute';
		els.display = 'block';
		var originalHeight = element.clientHeight;
		var originalWidth = element.clientWidth;
		if (els.width.empty()){
			var container = element.ancestors().find(function(ancestor){ return (ancestor.clientWidth > 0);	}, this);
			element.setStyle({ 'width' : ((container.clientWidth - (parseInt(element.getStyle('padding-left'), 10)) + parseInt(element.getStyle('padding-right'), 10)) + 'px') });
			originalHeight = element.clientHeight - (parseInt(element.getStyle('padding-top'), 10) + parseInt(element.getStyle('padding-bottom'), 10));
			element.setStyle({ 'width' : 'auto' });
		}
		els.display = originalDisplay;
		els.position = originalPosition;
		els.visibility = originalVisibility;
		return { width : originalWidth, height : originalHeight };
	}
});

Object.extend(document.viewport, {
	getDimensions: function() {
    var dimensions = { };
    var B = Prototype.Browser;
    $w('width height').each(function(d) {
      var D = d.capitalize();
      dimensions[d] = (B.WebKit && !document.evaluate) ? self['inner' + D] :
        (B.Opera && parseFloat(opera.version)<9.5) ? document.body['client' + D] : document.documentElement['client' + D];
    });
    return dimensions;
  }
});

try { document.execCommand('BackgroundImageCache', false, true); } catch(e) {}

String.prototype.getHash = function() {
	var idx = this.indexOf("#");
	return (idx >= 0 ) ? this.substring(idx + 1) : null;
};

var PageWidget = Behavior.create({
	initialize : function(options) {
		this.setOptions(options);
	},
	setOptions : function(config) {
		this.CONFIG = Object.extend(Object.clone(this.constructor.CONFIG), config || {});
	}
});

PageWidget.CONFIG = { };

var AdvancedSearch = Behavior.create(PageWidget, {
	initialize : function(config){
		this.setOptions(config);
		this.primary_fields = this.element.select(this.CONFIG['primrary_fields_selector']);
		this.search_interface = $(this.CONFIG['search_interface']);
		this.srchIntElements = this.search_interface.select('input');
		if (this.primary_fields.length < 1)
			return;
		this.add_new_line = $(this.CONFIG['add_new_line_id']);
		this.reset_all_fields = $(this.CONFIG['reset_all_fields_id']);
		this.element.select('.Invisible').invoke('removeClassName','Invisible');
		this.new_field_template = this.primary_fields.last().cloneNode(true);
		this.primary_fields.first().select(".button").invoke("remove");
		this.primary_fields.first().select('select').last().setStyle({ marginRight : "56px" });
		this.primary_fields.each(function(field){ 
			this.initializePrimaryField(field); 
		}, this);
		this.add_new_line.observe('click', this.addNewPrimaryFieldClickHandler.bindAsEventListener(this));
		this.reset_all_fields.observe('click', this.resetAllFieldsClickHandler.bindAsEventListener(this));
		for (var i = 0; i < this.srchIntElements.length; i++) {
			var srchIntElement = this.srchIntElements[i];
			srchIntElement.observe('click', this.srchInterfaceRadioClickHandler.bindAsEventListener(this,srchIntElement));
			if (srchIntElement.checked)
				this.srchInterfaceRadioClickHandler(null, srchIntElement);
		}
	},
	initializePrimaryField : function(field){
		var button = field.down(this.CONFIG['move_field_button_selector']);
		var select_menus = field.getElementsByTagName('SELECT');
		if (select_menus.length > 1 && button) {
			button.observe('click', this.moveFieldUpClickHandler.bindAsEventListener(this, button));
			var removeLineButton = button.next('IMG');
			if (removeLineButton)
				removeLineButton.observe('click', this.removeLineClickHandler.bindAsEventListener(this, removeLineButton));
		} else {
			if (button)
				button.addClassName(this.CONFIG['move_field_disabled_button_class']).src = this.CONFIG['disabled_button_image'];
		}
	},
	addNewPrimaryFieldClickHandler : function(){
		this.addNewPrimaryField();
	},
	srchInterfaceRadioClickHandler : function(e, srchIntElement){
		var field_group = this.primary_fields[0];
		var fields = field_group.childElements();
		var srchKeyElement = fields[1].childElements()[1];
		srchKeyElement.options[0].text = srchIntElement.value;
		srchKeyElement.options[0].value = srchIntElement.id;
		for(var i = 1; i < this.primary_fields.length; i++) {
			field_group = this.primary_fields[i];
			fields = field_group.childElements();
			srchKeyElement = fields[2].childElements()[1];
			srchKeyElement.options[0].text = srchIntElement.value;
			srchKeyElement.options[0].value = srchIntElement.id;
		}
		if(this.new_field_template==null||typeof(this.new_field_template) === 'undefined');
		else{
			var templateOptions = this.new_field_template.childElements()[2].childElements()[1].options;				
			if(typeof(templateOptions) === 'undefined');
			else{
				templateOptions[0].text = srchIntElement.value;
				templateOptions[0].value = srchIntElement.id;
			}
		}
	},
	moveFieldUpClickHandler : function(e, button){
		if (!button.hasClassName(this.CONFIG['move_field_disabled_button_class']))
			this.moveFieldUp(e, button);
	},
	removeLineClickHandler : function(e,button){
		var cElement = button.up();
		var idx = this.primary_fields.indexOf(cElement);
		if (idx >= 0){
			this.primary_fields.splice(idx,1);
			cElement.remove();
		}
	},
	resetAllFieldsClickHandler : function(){
		this.resetAllFields();
	},
	validateSearchTerms:function(searchTerms){
		if (searchTerms == null)
			return null;
		var splitTerms=searchTerms.split(" ");		
		if(splitTerms==null||splitTerms.length<2){
			return searchTerms;
		}
		var returnTerms = splitTerms[0];
		for(i=1; i<splitTerms.length;i++){
			returnTerms = returnTerms+ " "+splitTerms[i];
		}
		return returnTerms;
	},
	prepareNewPrimaryFieldTemplate : function(){
		var num = this.primary_fields.length + 1;
		return this.new_field_template.innerHTML.replace(/(name|id|for)\=\"?(\w+_)\d+\"?/g, '$1="$2' + num + '"');
	},
	addNewPrimaryField : function(){
		if (this.primary_fields.length > 10)
			return;
		var wrapper = new Element('P', { 'class' : 'field-group' }).update(this.prepareNewPrimaryFieldTemplate());
		this.primary_fields.last().insert({ 'after' : wrapper });
		this.primary_fields.push(wrapper);
		this.initializePrimaryField(wrapper);
	},
	moveFieldUp : function(e, button){
		var original_field = button.up();
		var target_field = original_field.previous();
		var original_selects = original_field.getElementsByTagName('SELECT');
		var target_selects = target_field.getElementsByTagName('SELECT');
		if (target_selects.length > 1) 
			this.swapSelectMenu(original_selects[0], target_selects[0]);
		this.swapTextField(original_field.down('INPUT'), target_field.down('INPUT'));
		this.swapSelectMenu(original_selects[1], target_selects.last());
	},
	swapTextField : function(first_field, second_field){
		var savedvalue = $F(second_field);
		second_field.value = $F(first_field);
		first_field.value = savedvalue;
	},
	swapSelectMenu : function(first_menu, second_menu){
		var savedvalue = second_menu.selectedIndex;
		second_menu.selectedIndex = first_menu.selectedIndex;
		first_menu.selectedIndex = savedvalue;
	},
	resetAllFields : function(){
		var search_wrapper = $("AdvancedSearchPage");
		search_wrapper.select("input[type=text]").each(function(textbox){
			textbox.clear();
		});
		search_wrapper.select("input[type=checkbox]").each(function(checkbox){
			checkbox.checked = false;
		});
		search_wrapper.select("select").each(function(selecct){
			select.selectedIndex = 0;
		});
	}
});
AdvancedSearch.CONFIG = {
	disabled_button_image : ASSETS_RELATIVE_PATH+'/img/icon.to-top.disabled.gif',
	conditional_prefix : 'op',
	term_prefix : 'query',
	search_interface : 'search-interface',	
	primrary_fields_selector : '.primary-fields .field-group',
	add_new_line_id : 'add-new-line',
	reset_all_fields_id : 'reset-all-fields',
	move_field_button_selector : 'IMG.button',
	move_field_disabled_button_class : 'disabled'
};

var AjaxImageSubmit = Behavior.create(PageWidget, {
	initialize : function(config){
		this.ajax_form = this.element.up('FORM');
		if (!this.ajax_form || (!this.element.tagName.toLowerCase() == 'input'))
			return;
		this.setOptions(config);
		this.replaceInputElement();
		this.trigger_observer = this.clickHandler.bindAsEventListener(this);
		this.element.observe('mousedown', this.trigger_observer);
		this.click_handler_callback = this.CONFIG['click_callback'].bind(this);
		this.success_handler_callback = this.CONFIG['success_handler'].bind(this);
		this.failure_handler_callback = this.CONFIG['failure_handler'].bind(this);
		this.parameters = this.CONFIG['ajax_parameters'];
	},
	replaceInputElement : function(){
		var button = new Element('IMG', { 'id' : this.element.id, 'class' : this.element.className, 'src' : this.element.src });
		this.element.insert({ after : button }).remove();
		this.element = button;
	},
	clickHandler : function(e){
		e.stop();
		this.submitAjaxForm();
		this.click_handler_callback();
	},
	submitAjaxForm : function(){
		var formAction = this.ajax_form.action;
		new Ajax.Request(formAction, {
			method : this.ajax_form.method,
			evalJSON : 'force',
			parameters : this.parameters,
			onSuccess : this.successHandler.bind(this),
			onFailure : this.failureHandler.bind(this)
		});
	},
	successHandler : function(transport){
		if (this.validateResponseJSON(transport)) {
			this.success_handler_callback();
			this.toggleTriggerImage();
		}
	},
	failureHandler : function(transport){
		this.element.stopObserving('click', this.trigger_observer);
		this.failure_handler_callback();
	},
	validateResponseJSON : function(transport){
		if (this.CONFIG['require_json_validation']) {
			try {
				return transport.responseJSON[this.CONFIG['valid_json_key']] == this.CONFIG['valid_json_value'];
			} catch(e) {
				return false;
			}
		} else {
			return true;
		}
	},
	toggleTriggerImage : function(){
		if (!this.CONFIG['trigger_image_1'].empty() && !this.CONFIG['trigger_image_2'].empty())
			this.element.src =  this.CONFIG[this.element.src.split('/').last() == this.CONFIG['trigger_image_1'].split('/').last() ? 'trigger_image_2' : 'trigger_image_1'];
	}
});

AjaxImageSubmit.CONFIG = {
	ajax_parameters : {},
	trigger_image_1 : '',
	trigger_image_2 : '',
	require_json_validation : false,
	valid_json_key : '',
	valid_json_value : '',
	click_callback : Prototype.emptyFunction,
	success_handler : Prototype.emptyFunction,
	failure_handler : Prototype.emptyFunction
};

var DHTMLMenu = Behavior.create(PageWidget, {
	initialized       : false,
	menu_hide_timeout : null,
	menu_show_timeout : null,
	last_menu_on      : null,
  initialize: function(config){
		this.setOptions(config);
		this.submenus = { };
		this.element.immediateDescendants().each(function(item){
			item.observe("mouseover", this.mouseoverHandler.bindAsEventListener(this, item));
			item.observe("mouseout", this.mouseoutHandler.bindAsEventListener(this, item));
			this.submenus[item.id] = item.down().next();
			if (this.submenus[item.id] == undefined && item.select('UL').length > 0)
					this.submenus[item.id] = item.down('UL');
		}, this);
	},
	mouseoverHandler : function(e, item) {
		clearTimeout(this.menu_hide_timeout);
		clearTimeout(this.menu_show_timeout);
		if (this.last_menu_on == null) {
			this.menu_show_timeout = setTimeout(this.showMenu.bind(this, item),	this.CONFIG["menu_show_time"]);
		} else if (this.last_menu_on != item) {
			this.showMenu(item);
		}
	},
	mouseoutHandler : function(e, item) {
		clearTimeout(this.menu_hide_timeout);
		clearTimeout(this.menu_show_timeout);
		if (this.submenus[item.id]) {
			this.menu_hide_timeout = setTimeout(this.hideMenu.bind(this, item),	this.CONFIG["menu_hide_time"]);
		} else {
			this.hideMenu(item);
		}
	},
	showMenu : function(item) {
		if (this.last_menu_on != null)
			this.hideMenu(this.last_menu_on);
		item.addClassName(this.CONFIG['hover_class']);
		if (Prototype.Browser.IE6) {
			var submenu = this.submenus[item.id];
			if (submenu) {
				var iframe = submenu.next();
				if (submenu && !iframe)
					this.createIframe(item, { 'left'   : submenu.offsetLeft + "px", 'height' : submenu.offsetHeight + "px", 'width'  : submenu.offsetWidth + "px" });
			}
		}
		this.last_menu_on = item;
	},
	hideMenu : function(item) {
		if (item == null)
			return;
		item.removeClassName(this.CONFIG['hover_class']);
		if (Prototype.Browser.IE6) {
			var submenu = this.submenus[item.id];
			if (submenu) {
				var iframe = submenu.next();
				if (submenu && iframe)
					item.removeChild(iframe);
			}
		}
		this.last_menu_on = null;
	},
	createIframe : function(node, style) {
		new Insertion.Bottom(node, (new Template('<iframe class="' + this.CONFIG['iframe_class'] + '" frameborder="0" scrolling="no" style="width: #{width}; height: #{height}; left: #{left};"><\/iframe>')).evaluate(style));
	}
});

DHTMLMenu.CONFIG = {
	submenu_class  : "SubMenu",
	hover_class    : "Hover",
	menu_hide_time : 500,
	menu_show_time : 100,
	iframe_class   : "IframeFix"
};

var ExpertSearch = Behavior.create(PageWidget, {
	initialize : function(expression){
		this.expression = $(expression);
		if (!this.expression)
			return;
		this.sortType = $('sortType');
		this.rowsPerPage = $('rowsPerPage');
				
		this.search_interface = $('search-interface');
 		this.srchIntElements = this.search_interface.select('input');

		this.element.setStyle({ display : 'block' });
		this.keyValueArray = [];
		this.expOffset = -1;
		this.filterOpKey = '';
		for (var x = 0; x < 2; x++)
			this.initializeDropDownMenu(this.element.down('.DropDown', x));
		$('reset-all').setStyle({ display : 'block' }).observe('click', this.resetTextArea.bindAsEventListener(this));
		$('submit-search').observe('click', this.submitCommandClickHandler.bindAsEventListener(this));
		this.expression.observe('click', this.expressioneEventHandler.bindAsEventListener(this));
		this.expression.observe('keyup', this.expressioneEventHandler.bindAsEventListener(this));
	},
	initializeDropDownMenu : function(menu){
		menu.select('LI').each(function(li){
			li.observe('click', this.dropDownItemClickHandler.bindAsEventListener(this, li));
		}, this);
	},
	dropDownItemClickHandler : function(e, li){
		if(li.id.length<1)return;
		string = li.id.split('-').last();
		this.filterOpKey = li.hasClassName("Operator") ? "op" : "srchkey";
		this.addItem(li, string);
		this.expression.focus();
	},
	expressioneEventHandler:function(){
		if (this.expression.selectionStart || this.expression.selectionStart == '0') {
			var startPos = this.expression.selectionStart;
			var endPos = this.expression.selectionEnd;
			this.expOffset = startPos;
		} else if (document.selection) {
			var sel = document.selection.createRange();
			if (sel.text.length > 1)
				return;
			var c= "\001";
			var dul = sel.duplicate();
			var len = 0;
			dul.moveToElementText(this.expression);
			sel.text = c;
			len = dul.text.indexOf(c);
			sel.moveStart('character', -1);
			sel.text = "";
			this.expOffset = len;
		} else {
			this.expOffset = -1;
		}
	},
	submitCommandClickHandler:function(){
		if (this.expression.value == null || this.expression.value.length <= 0) {
			alert("Please enter one or more keywords");
			return;
		}
		var url_string = '/search/searchresult.jsp?action=search' + '&' + this.sortType.id + '=' + this.sortType.value + '&' + this.rowsPerPage.id + '=' + this.rowsPerPage.value + '&' + 'matchBoolean=true';

		for(var i=0; i<this.srchIntElements.length;i++){
			if(this.srchIntElements[i].checked){
				url_string=url_string+'&searchField='+this.srchIntElements[i].id;
			}
		}
		url_string = url_string + '&queryText='+getFilteredTermsExt(this.expression.value,true);
		window.location=encodeURI(url_string);
	},
	addItem : function(li, string){
		if (string == null || string.length < 1)
			return;
		if (this.filterOpKey == 'srchkey')
			string = '"' + string+ '"' + ':';
		if (this.filterOpKey == 'op'&&string!=null&&(string=="NEAR/"||string=="ONEAR/")){
			string='("<term1> "' +string+'<#> "<term2>")';
		}		
		this.hideParentMenu(li);
		var existing = $F(this.expression);
		if (this.expOffset < 0){
			if (existing.endsWith(' ')) {
				this.expression.value = existing + string;
			} else {
				if (existing.empty()) {
					this.expression.value = string;
				} else {
					this.expression.value = existing + ' ' + string;
				}
			}
			this.expOffset = this.expression.value.length;
		} else {
			this.expression.value  = this.expression.value.substring(0,this.expOffset)+' '+string+this.expression.value.substring(this.expOffset, this.expression.value.length);
			this.expOffset = this.expOffset + string.length + 1;
		}
		this.setSelRange(this.expression, this.expOffset, this.expOffset);
	},
	setSelRange : function (inputEl, selStart, selEnd) {
		if (this.expression.selectionStart || this.expression.selectionStart == '0'){
			this.expression.selectionStart = selStart;
			this.expression.selectionEnd = selEnd;
		} else if (document.selection) {
			if (inputEl.setSelectionRange) {
				inputEl.focus();
				inputEl.setSelectionRange(selStart, selEnd);
			} else if (inputEl.createTextRange) {
				var range = inputEl.createTextRange();
				range.collapse(true);
				range.moveEnd('character', selEnd);
				range.moveStart('character', selStart);
				range.select();
			} 
		}
	},
	hideParentMenu : function(li){
		li.up('.DropDown').removeClassName('Hover');
	},
	resetTextArea : function(li){
		this.expression.clear();
	}
});

var HoverHighlighter = Behavior.create(PageWidget, {
	initialize : function(items_css_selector, config){
		this.setOptions(config);
		this.highlightable_items = this.element.select(items_css_selector);
		if (this.highlightable_items.size() < 1) 
			return;
		this.highlightable_items.each(function(item){
			item.observe('mouseover', this.itemMouseHandler.bindAsEventListener(this, item, true));
			item.observe('mouseout', this.itemMouseHandler.bindAsEventListener(this, item, false));
		}, this);
		this.mouseOverCallback = this.CONFIG['mouseover_callback'];
		this.mouseOutCallback = this.CONFIG['mouseout_callback'];
	},
	itemMouseHandler : function(event, li, hovering){
		if (hovering) {
			li.addClassName(this.CONFIG['hover_class']);
			this.mouseOverCallback();
		} else {
			li.removeClassName(this.CONFIG['hover_class']);
			this.mouseOutCallback();
		}
	}
});

HoverHighlighter.CONFIG = {
	hover_class : 'Hover',
	mouseover_callback : Prototype.emptyFunction,
	mouseout_callback : Prototype.emptyFunction
};

var MaintenanceMessage = Behavior.create(PageWidget, {
	initialize : function(config){
		this.setOptions(config);
		this.form_input = this.element.down('INPUT');
		this.close_link = this.element.down('SPAN');
		if (!this.form_input || !this.close_link)
			return;
		this.form_input.setStyle({ display : 'none' });
		this.close_link.setStyle({ display : 'block', zoom : '1' }).observe('click', this.closeClickHandler.bindAsEventListener(this));
	},
	closeClickHandler : function(e){
		e.stop();
		new Ajax.Request(this.element.action, {
			method : this.element.method,
			evalJSON : 'force',
			onSuccess : this.closeMaintenanceSuccessHandler.bind(this),
			onFailure : this.closeMaintenanceFailureHandler.bind(this)
		});
	},
	closeMaintenanceSuccessHandler : function(transport){
		if (transport.responseJSON.response) {
			new Effect.Fade(this.element.id);
		} else {
			this.closeMaintenanceFailureHandler();
		}
	},
	closeMaintenanceFailureHandler : function(){
		this.removeMaintenanceMessage(!this.CONFIG['failure_message_text'].empty());
	},
	removeMaintenanceMessage : function(delay){
		if (delay) {
			this.element.innerHTML = this.CONFIG['failure_message_text'];
			new Effect.Fade(this.element.id, { delay : this.CONFIG['failure_message_auto_dismiss_time'] });
		} else {
			new Effect.Fade(this.element.id);
		}
	}
});

MaintenanceMessage.CONFIG = {
	failure_message_text : '',
	failure_message_auto_dismiss_time : 3
};

var MasterCheckbox = Behavior.create(PageWidget, {
	initialize : function(master_checkbox, config){
		this.setOptions(config);
		this.master_checkbox = $(master_checkbox);
		this.checkbox_nodes = this.element.select('input[type="checkbox"]').without(this.master_checkbox);
		if (!this.master_checkbox || this.checkbox_nodes.size() < 1) 
			return;
		this.master_checkbox.checked = true;
		this.masterCheckboxClickHandler();
		this.master_checkbox.observe('click', this.masterCheckboxClickHandler.bindAsEventListener(this));
		this.checkbox_nodes.each(function(checkbox){
			checkbox.observe('click', this.checkboxClickHandler.bindAsEventListener(this));
		}, this);
	},
	masterCheckboxClickHandler : function(){
		var none_checked = true;
		if (this.master_checkbox.checked) {
			this.checkbox_nodes.each(function(checkbox){
				checkbox.checked = false;
			});
		} else {
			this.checkbox_nodes.each(function(checkbox){
				if (checkbox.checked)
					none_checked = false;
			});
		}
		this.checkMasterCheckbox(none_checked);
	},
	checkboxClickHandler : function(){
		if (this.master_checkbox.checked)
			this.master_checkbox.checked = false;
		var none_checked = true;
		this.checkbox_nodes.each(function(checkbox){
			if (checkbox.checked)
				none_checked = false;
		});
		this.checkMasterCheckbox(none_checked);
	},
	checkMasterCheckbox : function(none_checked){
		if (none_checked)
			this.master_checkbox.checked = true;
	}
});

var OverLabel = Behavior.create({
	initialize : function(){
		this.element.hide();
		this.overlabel = this.element.innerHTML;
		this.input = $(this.element.htmlFor);
		if ($F(this.input) == "" || ($F(this.input) == this.overlabel))
			this.initializeOverLabel();
	},
	initializeOverLabel : function(){
		this.input.value = this.overlabel;
		this.input.observe('click', this.overLabelFocusHandler.bindAsEventListener(this));
	},
	overLabelFocusHandler : function(){
		if ($F(this.input) == this.overlabel || ($F(this.input) == "")) {
			this.input.clear();
			this.input_blur_handler = this.overLabelBlurHandler.bindAsEventListener(this);
			this.input.observe('blur', this.input_blur_handler);
		}
	},
	overLabelBlurHandler : function(){
		Event.stopObserving(this.input, 'blur', this.input_blur_handler);
		if ($F(this.input).empty())
			this.replaceOverLabel();
	},
	replaceOverLabel : function(){
		this.input.value = this.overlabel;
	}
});

var Popup = Behavior.create(PageWidget, {
	initialize : function(location, config){
		this.setOptions(config);
		this.popup_location = location;
		this.layout_wrapper = $(this.CONFIG['layout_wrapper']);
		if (!this.layout_wrapper) 
			return;
		this.defineBrowserVariables();
		this.definePopupType();
		this.createOverlay();
		this.element.observe('click', this.popUpClickHandler.bindAsEventListener(this));
		this.overlay_observer = this.overlayClickHandler.bindAsEventListener(this);
		this.initializeCallback = this.CONFIG['initialize_callback'].bind(this);
		this.validateCallback = this.CONFIG['validate_callback'].bind(this);
		this.showCallback = this.CONFIG['show_callback'].bind(this);
		this.hideCallback = this.CONFIG['hide_callback'].bind(this);
		this.initializeCallback();
	},
	defineBrowserVariables : function(){
		var view = document.viewport;
		this.document_height = this.layout_wrapper.getHeight();
		this.vertical_scroll = view.getScrollOffsets()[1];
		this.window_height = view.getHeight();
		this.window_width = view.getWidth();
		this.element_position = this.element.positionedOffset();
	},
	definePopupType : function(){
		if (this.element.tagName.toLowerCase() == 'a')
			this.element.setStyle({ cursor : 'pointer' }).removeAttribute('href');
		if (!this.CONFIG['use_ajax'] && this.element.hasClassName(this.CONFIG['dhtml_popup_class']))
			this.element.addClassName(this.CONFIG['dhtml_trigger_class']).removeClassName(this.CONFIG['dhtml_popup_class']);
	},
	createOverlay : function(){
		if (!$('popup_overlay')) {
			var overlay = new Element('DIV', { 'id' : 'popup_overlay', 'style' : 'width: ' + this.window_width + 'px;' });
			$(document.body).insert({ 'bottom' : overlay });
		}
		this.overlay = $('popup_overlay');
	},
	popUpClickHandler : function(e){
		this.defineBrowserVariables();
		(!this.popup_container) ? this.fetchPopup() : this.displayPopup();
	},
	overlayClickHandler : function(e){
		if (e.target == this.overlay)
			this.hidePopup();
	},
	fetchPopup : function(){
		if (this.CONFIG['use_ajax']) {
			new Ajax.Request(this.popup_location, {	method : 'get',	onSuccess : this.popUpAjaxSuccessHandler.bind(this), onFailure : this.popUpAjaxFailureHandler.bind(this) });
		} else {
			if ($(this.popup_location)) {
				this.createPopup($(this.popup_location).innerHTML);
				this.displayPopup();
			}
		}
	},
	popUpAjaxSuccessHandler : function(transport){
		this.createPopup(transport.responseText);
		this.displayPopup();
	},
	popUpAjaxFailureHandler : function(transport){
		alert('The AJAX request failed and the popup cannot load.');
	},
	createPopup : function(html){
		if (!$(this.element.id + '_container')) {
			this.popup_container = new Element('DIV', { 'id' : this.element.id + '_container', 'class' : 'popup_container', 'style' : 'position: absolute; width: ' + this.popup_width + 'px; display: none; z-index: 999;' });
			this.overlay.insert({ 'bottom' : this.popup_container });
			Element.extend(this.popup_container);
			if (this.CONFIG['use_ajax']) {
				this.popup_container.update(html);
			} else {
				this.popup_container.insert($(this.popup_location));
			}
			this.calculatePopupDimensions();
		}
	},
	calculatePopupDimensions : function(){
		this.displayOverlay();
		this.popup_container.addClassName('Calculating');
		this.popup_width = this.popup_container.getWidth();
		this.popup_height = this.popup_container.getHeight();
		this.popup_container.removeClassName('Calculating');
		this.hideOverlay();
	},
	displayPopup : function(){
		if (Prototype.Browser.IE)
			this.hideAllContainers();
		this.validated = true;
		this.validateCallback();
		if (this.validated) {
			this.positionPopup();
			this.displayOverlay();
			this.insertIframe();
			this.popup_container.setStyle({ display : 'block' });
			this.overlay.observe('click', this.overlay_observer);
			this.initializePopupElements();
			this.showCallback();
		}
	},
	initializePopupElements : function(){
		if (this.CONFIG['window_submit_selector']){
			this.popup_form = this.popup_container.down('FORM');
			if (this.popup_form) {
				if (this.popup_form.findFirstElement())
					this.popup_form.focusFirstElement();
				this.popup_form_location = this.popup_form.action;
				this.popup_form_method = this.popup_form.method;
				this.popup_form.observe('submit', this.submitPopupForm.bindAsEventListener(this));
			}
		}
		if (this.CONFIG['window_close_selector']){
			var closes = this.popup_container.select(this.CONFIG['window_close_selector']);
			if (closes.size() > 0) {
				closes.each(function(close){
					close.observe('click', this.hidePopup.bind(this));
				}, this);
			}
		}
	},
	submitPopupForm : function(e){
		if (this.CONFIG['form_uses_ajax']) {
			e.stop();
			if (!this.validation)
				this.validation = new Validation(this.popup_form);
			if (this.validation.validate()) {
				new Ajax.Request(this.popup_form_location, { parameters : this.popup_form.serialize(), onSuccess : this.CONFIG['form_onsuccess_callback'], onFailure : this.CONFIG['form_onfailure_callback'] });
				this.validation = null;
				this.hidePopup();
			}
		}
	},
	displayOverlay : function(){
		this.overlay.setStyle({ height : this.document_height + 'px', display : 'block', visibility : 'visible' });
	},
	hideOverlay : function(){
		if (this.popup_container.visible())
			this.hidePopup();
		this.overlay.setStyle({ display : 'none', visibility : 'hidden' });
	},
	insertIframe : function(){
		if (Prototype.Browser.IE6)
			this.createIframe(this.popup_container, { 'top'    : this.popup_container.style.top, 'left'   : this.popup_container.style.left, 'height' : this.popup_height + "px", 'width'  : this.popup_width + "px" });
	},
	removeIframe : function(){
		if (Prototype.Browser.IE6) {
			var iframe = $('popup-iframe');
			if (iframe)
				iframe.remove();
		}
	},
	createIframe : function(node, style) {
		new Insertion.Bottom(node.up(), (new Template('<iframe id="popup-iframe" frameborder="0" scrolling="no" style="width: #{width}; height: #{height}; left: #{left}; top: #{top}; position: absolute; z-index: 502;"><\/iframe>')).evaluate(style));
	},
	positionPopup : function(){
		switch (this.CONFIG['position']) {
			case 'auto' :
				this.popup_container.setStyle({
					top : (((this.window_height / 2) - (this.popup_height / 2)) + this.vertical_scroll) + 'px',
					left : ((this.window_width - this.popup_width) / 2) + 'px'
				});
			break;
			case 'vertical' : 
				this.popup_container.setStyle({
					top : (this.element_position[1] + this.element.getHeight()) + 'px',
					left : (this.element_position[0]) + 'px'
				});
			break;
			case 'horizontal' :
				this.popup_container.setStyle({
					top : (((this.window_height / 2) - (this.popup_height / 2)) + this.vertical_scroll) + 'px',
					left : ((this.element_position[0] + this.element.getWidth()) + 1) + 'px'
				});
			break;
		}
	},
	hidePopup : function(){
		this.removeIframe();
		this.hideAllContainers();
		this.overlay.setStyle({	display : 'none' }).stopObserving('click', this.overlay_observer);
		this.hideCallback();
	},
	hideAllContainers : function(){
		this.removeIframe();
		this.overlay.select('.popup_container').each(function(popup){
			popup.style.display = 'none';
		}, this);
	},
	attachCheckboxObserver : function(target, popup, image_states) {
		var checkbox_container = this.layout_wrapper.down(target);
		if (checkbox_container) {
			var checkboxes = checkbox_container.select('INPUT[type="checkbox"]');
			checkboxes.each(function(box){
				box.observe('click', this.toggleCheckboxDisabledPopup.bindAsEventListener(this, checkboxes, popup, image_states));
			}, this);
		}
		this.select_all = $('select-all');
		this.deselect_all = $('deselect-all');
		if (this.select_all && this.deselect_all) {
			this.select_all.observe('click', this.toggleDisabledPopup.bindAsEventListener(this, true, popup, image_states));
			this.deselect_all.observe('click', this.toggleDisabledPopup.bindAsEventListener(this, false, popup, image_states));
		}
		if (checkboxes)
		 this.toggleCheckboxDisabledPopup(this, checkboxes, popup, image_states);
	},
	toggleCheckboxDisabledPopup : function(e, checkboxes, popup, image_states) {
		var something_checked = false;
		checkboxes.each(function(box){
			if (box.checked)
				something_checked = true;
		});
		this.toggleDisabledPopup(e, something_checked, popup, image_states);
	},
	toggleDisabledPopup : function(e, toggle_status, popup, image_states){
		var container = $(popup);
		if (container) {
			if (toggle_status) {
				container.removeClassName('disabled');
				this.element.src = image_states[0];
			} else {
				container.addClassName('disabled');
				this.element.src = image_states[1];
			}
		}
	},
	displayUnlessDisabled : function(){
		if (this.element.hasClassName('disabled'))
			this.validated = false;
	}
});

Popup.CONFIG = {
	use_ajax : true,
	position : 'auto',
	window_submit_selector : '.form-action',
	window_close_selector : '.close-popup',
	layout_wrapper : 'LayoutWrapper',
	dhtml_popup_class : 'DHTMLPopup',
	dhtml_trigger_class : 'DHTMLTrigger',
	initialize_callback : Prototype.emptyFunction,
	validate_callback : Prototype.emptyFunction,
	show_callback : Prototype.emptyFunction,
	hide_callback : Prototype.emptyFunction,
	form_uses_ajax : true,
	form_onsuccess_callback : Prototype.emptyFunction,
	form_onfailure_callback : Prototype.emptyFunction
};

var SaveSearch = Class.create(PageWidget, {
	initialize : function(config){
		this.setOptions(config);
		this.results_header = $("results-hdr");
		this.div_display_query = '';
		if (this.results_header) {
			var chunks = this.results_header.select(".refinement");
			chunks = chunks.collect(function(i) { return i.innerHTML.stripTags(); });
			chunks.each(function(chunk){
				this.div_display_query = this.div_display_query.concat("<p align=\"left\">" + chunk + "<br /> </p>");
			}, this);
		}
		this.ss_display_query = ($("hiddenBreadcrumbContent").innerHTML.stripTags()).strip();
		this.ss_title= $("savedSearchTitle").value;
		this.ss_qry_str = $("oqs").value;
		this.ss_endeca_query =  ($("hiddenEndecaQuery").innerHTML.stripTags()).strip();
		this.renderLabels();
	},
	renderLabels : function(){
		var ss_title = $(this.CONFIG['ss_title']);
		var ss_display_query = $(this.CONFIG['ss_display_query']);
		var ss_qry_str = $(this.CONFIG['ss_qry_str']);
		var div_display_query = $(this.CONFIG['div_display_query']);
		var ss_endeca_query = $(this.CONFIG['ss_endeca_query']);
		if (ss_title)
			ss_title.value = this.ss_title;
		if (ss_display_query)
			ss_display_query.value = this.ss_display_query;
		if (div_display_query)
			div_display_query.innerHTML = this.div_display_query;
		if (ss_qry_str)
			ss_qry_str.value  = this.ss_qry_str;
		if (ss_endeca_query)
			ss_endeca_query.value = this.ss_endeca_query;
	}
});

SaveSearch.CONFIG = {
	wrapper_id : 'save-this-search',
	ajax_href : '/search/jsp/personalization/save_this_search.jsp?random='+ new Date().getTime(),
	ss_title : 'ss_title',
	ss_display_query : 'ss_display_query',
	ss_qry_str : 'ss_qry_str',
	div_display_query : 'div_display_query',
	ss_endeca_query : 'ss_endeca_query'
};

var DownloadBrowseCitations = Class.create(PageWidget, {
	initialize : function(wrapper_target, config){
		this.setOptions(config);
		this.node = $(wrapper_target);
		this.fetchUserPref();
	},
	fetchUserPref : function(){
		new Ajax.Request(this.CONFIG['ajax_href'], {
			method : 'get',
			evalJSON : 'force',
			onSuccess : this.fetchSuccessHandler.bind(this),
			onFailure : this.fetchFailureHandler.bind(this)
		});
	},
	fetchFailureHandler : function(transport){
		$(this.CONFIG['wrapper_id']).setStyle({ display : 'none' });
	},
	fetchSuccessHandler : function(transport){
		var json = transport.responseText.evalJSON();
		this.DL_Format = json.DL_Format;
		this.CIT_Format = json.CIT_Format;
		this.renderLabels();
	},
	renderLabels : function(){
		var popup_holder = this.node.up("div").id;
		if (popup_holder == 'popup-download-browse-citations_container') {
			var checkboxes = $("tocresultform").select("input[type='checkbox']");
			var checked = checkboxes.collect(function(box){
				return box.checked ? box.id : '';
			});
			$("recordIds").value = checked.join(' ').strip().gsub(' ', ',');
		} else {
			var recordId = $("recordId").innerHTML;
			$("recordIds").value = recordId;
		}
		$(this.CIT_Format == '2' ? "citation-abstract" : "citation-only").checked = true;
		switch (this.DL_Format) {
			case '1' : $("download-ascii").checked = true; break;
			case '2' : $("download-bibtex").checked = true; break;
			case '3' : $("download-refworks").checked = true; break;
			case '4' : $("download-endnote-procite-refman").checked = true; break;
			default : $("download-ascii").checked = true;
		}
	}
});

DownloadBrowseCitations.CONFIG = {
	wrapper_id : 'download-citations',
	ajax_href : '/search/jsp/personalization/userpref.jsp',
	DL_Format : 'DL_Format',
	CIT_Format : 'CIT_Format'
};

var DownloadSearchResultCitations = Class.create(PageWidget, {
	initialize : function(wrapper_target, config){
		this.setOptions(config);
		this.node = $(wrapper_target);
		this.fetchUserPref();
	},
	fetchUserPref : function(){
		new Ajax.Request('/search/jsp/personalization/userpref.jsp', {
			method : 'get',
			evalJSON : 'force',
			onSuccess : this.fetchSuccessHandler.bind(this),
			onFailure : this.fetchFailureHandler.bind(this)
		});
	},
	fetchFailureHandler : function(transport){
		$(this.CONFIG['wrapper_id']).setStyle({ display : 'none' });
	},
	fetchSuccessHandler : function(transport){
		var json = transport.responseText.evalJSON();
		this.DL_Format = json.DL_Format;
		this.CIT_Format = json.CIT_Format;
		this.renderLabels();
	},
	renderLabels : function(){
		var popup_holder = this.node.up("div").id;
		if (popup_holder == 'popup-download-searchresult-citations_container'){
			var checkboxes = $$("UL.Results")[0].select("INPUT");
			var checked = checkboxes.collect(function(box){ 
				return box.checked ? box.id : '';
			});
			$("recordIds").value = checked.join(' ').strip().gsub(' ', ',');
		} else {
			var recordId = $("recordId").innerHTML;
			$("recordIds").value = recordId;
		}
		$(this.CIT_Format == '2' ? "citation-abstract" : "citation-only").checked = true;
		switch (this.DL_Format) {
			case '1' : $("download-ascii").checked = true; break;
			case '2' : $("download-bibtex").checked = true; break;
			case '3' : $("download-refworks").checked = true; break;
			case '4' : $("download-endnote-procite-refman").checked = true; break;
			default : $("download-ascii").checked = true;
		}
	}
});

DownloadBrowseCitations.CONFIG = {
	wrapper_id : 'download-citations',
	ajax_href : '/search/jsp/personalization/userpref.jsp',
	DL_Format : 'DL_Format',
	CIT_Format : 'CIT_Format'
};

var UserPreferences = Class.create(PageWidget, {
	initialize : function(wrapper_target, config){
		this.setOptions(config);
		this.node = $(wrapper_target);
		this.fetchUserPref();
	},
	fetchUserPref : function(){
		new Ajax.Request('/search/jsp/personalization/userpref.jsp', {
			method : 'post',
			evalJSON : 'force',
			onSuccess : this.fetchSuccessHandler.bind(this),
			onFailure : this.fetchFailureHandler.bind(this)
		});
	},
	fetchFailureHandler : function(transport){
		$(this.CONFIG['wrapper_id']).setStyle({ display : 'none' });
	},
	fetchSuccessHandler : function(transport){
		var json = transport.responseText.evalJSON();
		this.DL_Format = json.DL_Format;
		this.CIT_Format = json.CIT_Format;
		this.Email_Format = json.EMAIL_Format;
		this.Email_Address = json.EMAIL_Addr;
		this.Sort_Type = json.Sort_Type;
		this.ResultsPerPage = json.Result_Num;
		this.Results_Layout = json.Result_Layout;
		this.Search_Type = json.Search_Type;
		this.Return_Type = json.Return_Type;
		this.hideNonSubscribeContent = json.hideNonSubscribeContent;
		if (json.ERROR) {
			$(this.CONFIG['wrapper_id']).setStyle({ display : 'none' });
			alert('Error saving search term due to session expiration. PLEASE LOGIN AGAIN');
			return;
		}
		this.renderLabels();
	},
	renderLabels : function(){
		if (this.hideNonSubscribeContent) {
			$('Search_Type').setStyle({ display : 'none' });
			$('Download_Options').setStyle({ display : 'none' });
		} 

		$(this.CIT_Format == '2' ? "include-citation-abstract" : "include-citation-only").checked = true;

		switch (this.DL_Format) {
			case '1' : $("format-ascii").checked = true; break;
			case '2' : $("format-bibtext").checked = true; break;
			case '3' : $("format-refworks").checked = true; break;
			case '4' : $("format-endnote").checked = true; break;
			default : $("format-ascii").checked = true;
		}

		$(this.Email_Format == 'html' ? "email_format_html" : "email_format_text").checked = true;

		if (this.Email_Address)
			$("email_address").value = this.Email_Address; 

		switch (this.Results_Layout) {
			case '1' : $("layout-title").checked = true; break;
			case '2' : $("layout-title-citation").checked = true; break;
			case '3' : $("layout-title-citation-abstract").checked = true; break;
			default : $("layout-title").checked = true;
		}

		$(this.Search_Type == '2' ? "full-text-all-fields" : "search-all-fields").checked = true;

		if (this.Return_Type.match("ALL")) {
			$("return-all").checked = true; 
			$("return-ieee").checked = false; 
			$("return-iet").checked = false; 
			$("return-aip").checked = false; 
			$("return-avs").checked = false; 
			$("return-ibm").checked = false; 
		} else {
			if (this.Return_Type.match(/IEEE_\w+/)) {
				$("return-ieee").checked = true; 
				$("return-all").checked = false;
			} 
			if (this.Return_Type.match(/IET_\w+/)) {
				$("return-iet").checked = true; 
				$("return-all").checked = false;
			} 
			if (this.Return_Type.match(/AIP_\w+/)) {
				$("return-aip").checked = true;
				$("return-all").checked = false;
			} 
			if (this.Return_Type.match(/AVS_\w+/)) {
				$("return-avs").checked = true;
				$("return-all").checked = false;
			}
			if (this.Return_Type.match(/IBM_\w+/)) {
				$("return-ibm").checked = true;
				$("return-all").checked = false;
			}
		}

		var results = $('results_page');
		switch (this.ResultsPerPage) {
			case 10 : results.selectedIndex = 0; break;
			case 25 : results.selectedIndex = 1; break;
			case 50 : results.selectedIndex = 2; break;
			case 100 : results.selectedIndex = 3; break;
			default : results.selectedIndex = 0;
		}

		$('sort_by').selectedIndex = parseInt(this.Sort_Type, 10) - 1;
	}
});

UserPreferences.CONFIG = {
	wrapper_id : 'search-preferences',
	ajax_href : '/search/jsp/personalization/userpref.jsp',
	DL_Format : 'DL_Format',
	CIT_Format : 'CIT_Format',
	Email_Format : 'Email_Format',
	Email_Address : 'Email_Address',
	Sort_Type: 'Sort_Type',
	ResultsPerPage : 'ResultsPerPage',
	Results_Layout : 'Results_Layout',
	Search_Type : 'Search_Type',
	Return_Type : 'Return_Type',
	hideNonSubscribeContent : 'hideNonSubscribeContent'
};

var Revealer = Behavior.create(PageWidget, {
	initialize: function(config){
		this.setOptions(config);
		this.nodes = {};
		this.nodes['controller'] = this.element.down(this.CONFIG['controller_class']);
		this.nodes['content'] = this.element.down(this.CONFIG['content_class']);
		if (!this.nodes['controller'] || !this.nodes['content']) {
			this.CONFIG['noRevealedContentCallback'](this);
			return;
		}
		if (this.CONFIG['hidden_by_default'])
			this.nodes['content'].hide();
		this.initializeCallback = this.CONFIG['initialize_callback'].bind(this);
		this.controlCallback = this.CONFIG['control_callback'].bind(this);
		this.nodes['controller'].observe('click', this.revealerControlClickHandler.bindAsEventListener(this));
		this.initializeCallback();
	},
	revealerControlClickHandler : function(){
		this.revealOrConcealContent();
		this.controlCallback();
	},
	revealOrConcealContent : function() {
		if (this.CONFIG['effect']) {
			new Effect.toggle(this.nodes['content'], 'blind', this.CONFIG['effect_options']);
		} else {
			this.nodes['content'].toggle();
		}
	}
});

Revealer.CONFIG = {
	controller_class : '.RevealControl',
	content_class : '.RevealContent',
	effect : false,
	effect_options : { duration : 1 },
	hidden_by_default: true,
	initialize_callback : Prototype.emptyFunction,
	control_callback : Prototype.emptyFunction,
	noRevealedContentCallback : Prototype.emptyFunction
};

var SelectAll = Behavior.create(PageWidget, {
	initialize : function(controls, config){
		this.setOptions(config);
		this.checkbox_nodes = this.element.select('input[type="checkbox"]');
		this.controls_container = $(controls);
		if (this.checkbox_nodes.size() < 1 || !this.controls_container)
			return;
		this.select_all_node = this.controls_container.down('.' + this.CONFIG['select_class']);
		this.deselect_all_node = this.controls_container.down('.' + this.CONFIG['deselect_class']);
		if (this.select_all_node && this.deselect_all_node) {
			this.select_all_node.observe('click', this.toggleAllSelections.bindAsEventListener(this, 'checked'));
			this.deselect_all_node.observe('click', this.toggleAllSelections.bindAsEventListener(this, ''));
			this.controls_container.setStyle({ 'display' : 'block' });
		}
	},
	toggleAllSelections : function(event, state) {
		this.checkbox_nodes.each(function(checkbox){
			checkbox.checked = state;
		});
	}
});

SelectAll.CONFIG = {
	select_class : 'SelectAll',
	deselect_class : 'DeSelectAll'
};

var SelectObserver = Behavior.create({
	initialize : function(){
		this.select_fieldset = this.element.up('FIELDSET');
		this.select_form = this.select_fieldset.up('FORM');
		if (!this.select_fieldset || !this.select_form)
			return;
		this.select_fieldset.down('INPUT').setStyle({ display : 'none' });
		this.element.observe('change', this.selectChangeHandler.bindAsEventListener(this));
	},
	selectChangeHandler : function(){
		this.select_form.submit();
	}
});

var TabSet = Behavior.create(PageWidget, {
	initialize : function(config) {
		this.setOptions(config);
		if (this.element.hasClassName(this.CONFIG['static_class']))
			return;
		this.tabs = [];
		this.element.select(this.CONFIG['tab_selector']).each(function(anchor, index){
			this.tabs.push(new TabSet.Tab(anchor, this));
		}, this);
		var desired_tab, desired_tab_name = window.location.hash.substring(1);
		if (!desired_tab_name.blank()) {
			desired_tab = this.tabs.find(function(tab) {
				return tab.element.href.getHash() == desired_tab_name;
			}) || this.tabs[0];
		}
		if (!desired_tab) {
			desired_tab = this.tabs.find(function(tab){
				return tab.element.up().hasClassName('Active');
			}) || this.tabs[0];
		}
		desired_tab.activate(true);
		this.initializeCallback = this.CONFIG['initialize_callback'];
		this.initializeCallback();
	}
});

TabSet.CONFIG = {
	tab_selector : ".TabNav A",
	tab_prefix : "tab_",
	active_class : "Active",
	static_class : "Static",
	initialize_callback : Prototype.emptyFunction
};

TabSet.Tab = Class.create({
	initialize : function(anchor, tab_set) {
		this.element = $(anchor);
		this.tab_set = tab_set;
		this.node_target_id = this.element.href.getHash();
		this.nav_node = this.element.up();
		this.tab_node = $(this.tab_set.CONFIG['tab_prefix'] + this.node_target_id);
		anchor.observe('click', this.handleAnchorClick.bindAsEventListener(this));
	},
	handleAnchorClick : function(e) {
		e.stop();
		e.element().blur();
		this.activate();
	},
	activate : function(force) {
		var ACTIVE_CLASS = this.tab_set.CONFIG['active_class'];
		if (this.nav_node.hasClassName(ACTIVE_CLASS) && !force)
			return;
		this.tab_set.tabs.each(function(tab){
			if (this.node_target_id == tab.node_target_id) {
				tab.nav_node.addClassName(ACTIVE_CLASS);
				tab.tab_node.addClassName(ACTIVE_CLASS);
				tab.tab_node.show();
			} else {
				tab.nav_node.removeClassName(ACTIVE_CLASS);
				tab.tab_node.removeClassName(ACTIVE_CLASS);
				tab.tab_node.hide();
			}
		}, this);
	}
});



// Implementation

Event.addBehavior({

	'.advanced-search .primary-search' : AdvancedSearch,

	'#save-this-search-button1' : AjaxImageSubmit({
		require_json_validation : false,
		valid_json_key : 'response',
		valid_json_value : true,
		trigger_image_1 : ASSETS_RELATIVE_PATH+'/img/btn.save-this-search.gif',
		trigger_image_2 : ASSETS_RELATIVE_PATH+'/img/btn.search-saved.gif'
	}),
	'#set-alert-on-this-search' : AjaxImageSubmit({
		trigger_image_1 : ASSETS_RELATIVE_PATH+'/img/btn.set-alert-on-search.gif',
		trigger_image_2 : ASSETS_RELATIVE_PATH+'/img/btn.set-alert.alert-set.gif'
	}),
	'#set-alert-on-this-journal' : AjaxImageSubmit({
		trigger_image_1 : ASSETS_RELATIVE_PATH+'/img/btn.alert-on-journal.gif',
		trigger_image_2 : ASSETS_RELATIVE_PATH+'/img/btn.alert-on-journal-set.gif'
	}),
	'#favorite-journal' : AjaxImageSubmit({
		trigger_image_1 : ASSETS_RELATIVE_PATH+'/img/btn.favorite-journal.gif',
		trigger_image_2 : ASSETS_RELATIVE_PATH+'/img/btn.journal-favorited.gif'
	}),

	'.DHTMLMenu': DHTMLMenu({ menu_hide_time : 300 }),
	  
	'#expression-tools' : ExpertSearch('expression-textarea'),

	'UL.Results' : HoverHighlighter('LI'),
	'#BrowseContent' : HoverHighlighter('UL.Browsing > LI'),
	'UL.Profiles' : HoverHighlighter('LI'),
	'UL.listview-highlight' : HoverHighlighter('LI'),
	'#expression-tools .SubMenu' : HoverHighlighter('LI'),
	'UL.Searches' : HoverHighlighter('LI.Hoverable'),
	'UL.AlertsFeeds' : HoverHighlighter('LI.Hoverable'),

	'#maintenance-message': MaintenanceMessage({
		failure_message_text : 'Error: Unable to save maintenance message preferences.',
		failure_message_auto_dismiss_time : 1
	}),

	'#choose-sources .layout' : MasterCheckbox('select-all-checkboxes'),

	'.overlabel' : OverLabel,

	'#popup-search-preferences' : Popup('/search/jsp/personalization/search_preferences.jsp', {
		show_callback : function() { 
			new UserPreferences();
		},
		form_onsuccess_callback : function(transport) {
			var json = transport.responseText.evalJSON();
			if (!json.validates){
				var alert_message = new Element("p", { "class" : "validation-failed validation-advice", "style" : "padding: .5em;margin-top:1em;" });
			} else {
				var alert_message = new Element("p", { "class" : "validation-advice" });
			}
			alert_message.update(json.message);
			var wrapper = $$(".Home #search-field");
			if (wrapper[0]) {
				wrapper[0].insert({ before : alert_message });
			} else {
				$("Body").down("div").insert({ "top" : alert_message });
			}
			new Effect.Fade(alert_message, { delay : 3 });
		},
		validate_callback : function(){
			this.displayUnlessDisabled();
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		}
	}),
	'.popup-save-to-project' : Popup('/xpl/ajax/save_to_project.html', {
		show_callback : function(){
			this.element.up('LI').addClassName('HoverStatic');
		},
		hide_callback : function(){
			this.element.up('LI').removeClassName('HoverStatic');
		}
	}),
	'#popup-email-selected-results' : Popup('/xpl/ajax/email_selected_results.html', { 
		position : 'vertical',
		initialize_callback : function(){
			this.attachCheckboxObserver('UL.Results', 'popup-email-selected-results', [ASSETS_RELATIVE_PATH+'/img/btn.email-selected-results.gif',ASSETS_RELATIVE_PATH+'/img/btn.email-selected-results.disabled.gif']);
		},
		validate_callback : function(){
			this.displayUnlessDisabled();
		},
		show_callback : function() {
			var frompage = $("fromPage").innerHTML;
			var formname = $("formName").innerHTML;
			var checkboxes = $(formname).select("input[type='checkbox']");
			var checked = checkboxes.collect(function(box){
				if (box.checked)
					return box.id;
			});
			$("hidden_input").value = checked.join(";");
			$("from_page").value = frompage;
			if (frompage == 'tocresults') {
				   $("abs_link").value =  '/xpls/abs_all.jsp?';
			}			
			else if(frompage == 'searchresults'){
				$("abs_link").value =  '/search/srchabstract.jsp?navigation=no&';
			}
		},
		form_onsuccess_callback : function(transport) {
			var json = transport.responseText.evalJSON();
			if (!json.validates){
				var alert_message = new Element("p", { "class" : "validation-failed validation-advice", "style" : "padding: .5em;margin-top:1em;" });
			} else {
				var alert_message = new Element("p", { "class" : "validation-advice" });
			}
			alert_message.update(json.message);
			$("Body").down("div").insert({ "top" : alert_message });
			new Effect.Fade(alert_message, { delay : 3 });
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		}
	}),
	'#popup-email-document' : Popup('/xpl/ajax/email_document.html', { 
		position : 'vertical',
		show_callback : function() {
			var fromPage = $("fromPage").innerHTML;
			var recordId = $("recordId").innerHTML;
			if (fromPage == 'searchabstract') {
				$("abs_link").value =  '/search/srchabstract.jsp?navigation=no&';
				$("from_page").value = 'searchresult';
			} else if (fromPage == 'moduleabstract') {
				$("abs_link").value =  '/xpl/modulesabstract.jsp?';
			} else if (fromPage == 'bookAbstract') {
				$("abs_link").value =  '/xpl/bkabstractplus.jsp?';
			}
			else {
				$("abs_link").value =  '/xpls/abs_all.jsp?';
			}
			$("hidden_input").value = recordId;
		},
		form_onsuccess_callback : function(transport) {
			var json = transport.responseText.evalJSON();
			if (!json.validates){
				var alert_message = new Element("p", { "class" : "validation-failed validation-advice", "style" : "padding: .5em;margin-top:1em;" });
			} else {
				var alert_message = new Element("p", { "class" : "validation-advice" });
			}
			alert_message.update(json.message);
			$("Body").down("div").insert({ "top" : alert_message });
			new Effect.Fade(alert_message, { delay : 3 });
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		}
	}),
	'#popup-download-browse-citations' : Popup('/xpl/ajax/download_citations.html', { 
		position : 'vertical',
		form_uses_ajax : false,
		initialize_callback : function(){ 
			this.attachCheckboxObserver('UL.Results', 'popup-download-browse-citations', [ASSETS_RELATIVE_PATH+'/img/btn.dl-citations.gif', ASSETS_RELATIVE_PATH+'/img/btn.dl-citations.disabled.gif']);
		},
		validate_callback : function(){
			this.displayUnlessDisabled();
		},
		show_callback : function() {
			var download_citations = $("download-citations");
			if (!download_citations.hasClassName('Activated')) {
				new DownloadBrowseCitations('download-citations');
				download_citations.addClassName('Activated');
			}
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		}
	}),
	'#popup-download-searchresult-citations' : Popup('/xpl/ajax/download_citations.html', { 
		position : 'vertical',
		form_uses_ajax : false,
		initialize_callback : function(){ 
			this.attachCheckboxObserver('UL.Results', 'popup-download-searchresult-citations', [ASSETS_RELATIVE_PATH+'/img/btn.dl-citations.gif', ASSETS_RELATIVE_PATH+'/img/btn.dl-citations.disabled.gif']);
		},
		validate_callback : function(){
			this.displayUnlessDisabled();
		},
		show_callback : function() {
			var download_citations = $("download-citations");
			if (!download_citations.hasClassName('Activated')) {
				new DownloadSearchResultCitations('download-citations');
				download_citations.addClassName('Activated');
			}
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		}
	}),
	'#popup-download-document-citations' : Popup('/xpl/ajax/download_citations.html', { 
		position : 'vertical',
		form_uses_ajax : false,
		initialize_callback : function(){ 
			this.attachCheckboxObserver('UL.Results', 'popup-download-search-citations', [ASSETS_RELATIVE_PATH+'/img/btn.dl-citations.gif', ASSETS_RELATIVE_PATH+'/img/btn.dl-citations.disabled.gif']);
		},
		show_callback : function() {
			
			var recordId = $("recordId").innerHTML;
			var fromPage = $("fromPage").innerHTML;
			var download_citations = $("download-citations");
			if (!download_citations.hasClassName('Activated')) {
				new DownloadBrowseCitations('download-citations');
				download_citations.addClassName('Activated');
			}
			
			$("recordIds").value = recordId;
			$("fromPageName").value = fromPage;
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		},
		validate_callback : function(){
			this.displayUnlessDisabled();
		}
	}),
	'#save-this-search-button' : Popup('/search/jsp/personalization/save_this_search.jsp?'+"&random="+ new Date().getTime(), {
		position : 'horizontal',
		show_callback : function() {
			var save = $("save-this-search");
			if (!save.hasClassName('Activated')) {
				new SaveSearch({});
				save.addClassName('Activated');
			}
		},
		form_onsuccess_callback : function(transport) {
			var json = transport.responseText.evalJSON();
			if (!json.validates){
				var alert_message = new Element("p", { "class" : "validation-failed validation-advice", "style" : "padding: .5em;margin-top:1em;" });
			} else {
				var alert_message = new Element("p", { "class" : "validation-advice" });
			}
			alert_message.update(json.message);
			$("Body").down("div").insert({ "top" : alert_message });
			new Effect.Fade(alert_message, { delay : 10 });
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		}
	}),
	'LI.EditSavedSearch' : Popup('/search/jsp/personalization/save_this_search.jsp'+"?random="+ new Date().getTime(), {
		initialize_callback : function(){
			var id = this.element.id.replace("edit-saved-search-","");
			this.popup_location = this.popup_location + "?" + id;
			this.popup_data_li = $("saved-search-" + id);
			this.ss_display_query = this.popup_data_li.down(".ss_display_query").innerHTML.stripTags();
			this.ss_title = this.popup_data_li.down(".ss_title").innerHTML;
			this.ss_query_str = this.popup_data_li.down(".ss_query_str").innerHTML;
			this.search_id = this.popup_data_li.down(".search_id").innerHTML;
			this.div_display_query = this.popup_data_li.down(".div_display_query").innerHTML;
			this.ss_alert = this.popup_data_li.down(".ss_alert").innerHTML;
		},
		form_onsuccess_callback : function(transport) {
			var json = transport.responseText.evalJSON();
			if (!json.validates){
				var alert_message = new Element("p", { "class" : "validation-eror" }).update(json.message);
			} else {
				var alert_message = new Element("p", { "class" : "validation-advice" }).update(json.message);
			}
			if (json.ID){
				this.popup_data_li = $("saved-search-" + json.ID);
				if (json.queryTitle){
					this.popup_data_li.down(".ss_title").innerHTML = json.queryTitle;
					this.popup_data_li.down("div").down("a").innerHTML = json.queryTitle;
				}
				var alertElement = this.popup_data_li.down("ul").down("li.alert").down(".link");
				if (json.alert == 'on'){
					this.popup_data_li.down(".ss_alert").innerHTML = 'Y';
					alertElement.replace('<a href="/search/noAlert?searchID='+json.ID+ '"> <span class="link"> Remove Alert </span></a>');
				} else {
					this.popup_data_li.down(".ss_alert").innerHTML = 'N';
					alertElement.replace('<a href="/search/setAlert?searchID='+json.ID+ '"> <span class="link">Set Alert </span></a>');
				}
			}
			$("Body").down("div").insert({ "top" : alert_message });
			new Effect.Fade(alert_message, { delay : 3 });
			window.location.reload( false );
		},
		show_callback : function(){
			$("ss_display_query").value = this.ss_display_query.strip();
			$("div_display_query").innerHTML = this.div_display_query;
			$("ss_title").value = this.popup_data_li.down(".ss_title").innerHTML;
			$("ss_qry_str").value = this.ss_query_str;
			$("search_id").value = this.search_id;
			this.ss_alert = this.popup_data_li.down(".ss_alert").innerHTML;
			$("ss_alerts_boolean").checked = this.ss_alert == "Y";
		},
		hide_callback : function(){
			if (this.popup_container) {
				this.popup_container.remove();
				this.popup_container = null;
			}
		}
	}),
	'#search-tips-popup' : Popup('/xpl/ajax/search_tips.html'),

	'UL.Results LI.noAbstract' : Revealer({
		effect : true,
		hidden_by_default : true,
		initialize_callback : function(){
			this.nodes['controller'].writeAttribute('title', 'Click to Reveal');
		},
		control_callback : function(){
			this.nodes['controller'].src = (this.nodes['content'].visible()) ? ASSETS_RELATIVE_PATH+'/img/btn.quick-abstract.collapsed.gif' : ASSETS_RELATIVE_PATH+'/img/btn.quick-abstract.expanded.gif';
			this.nodes['controller'].title = (this.nodes['content'].visible()) ? 'Click to Reveal' : 'Click to Close';
		}
	}),
	'UL.Results LI.showAbstract' : Revealer({
		effect : false,
		hidden_by_default : false,
		initialize_callback : function(){
			this.nodes['controller'].writeAttribute('title', 'Click to Close');
		},
		control_callback : function(){
			this.nodes['controller'].src = (this.nodes['content'].visible()) ? ASSETS_RELATIVE_PATH+'/img/btn.quick-abstract.collapsed.gif' : ASSETS_RELATIVE_PATH+'/img/btn.quick-abstract.expanded.gif';
			this.nodes['controller'].title = (this.nodes['content'].visible()) ? 'Click to Reveal' : 'Click to Close';
		}
	}),
	'UL.Browsing LI.noAbstract' : Revealer({
		effect : true,
		hidden_by_default : true,
		initialize_callback : function(){
			this.nodes['controller'].writeAttribute('title', 'Click to Reveal');
			this.element.setStyle({ position: "relative" });
			var arrow = new Element("span");
			this.element.insert({ bottom : arrow });
			arrow.addClassName("ArrowToggler");
			arrow.observe("click", this.revealerControlClickHandler.bindAsEventListener(this));
		},
		control_callback : function(){
			this.nodes['controller'].src = (this.nodes['content'].visible()) ? ASSETS_RELATIVE_PATH+'/img/btn.view-details.collapsed.gif' : ASSETS_RELATIVE_PATH+'/img/btn.view-details.expanded.gif';
			this.nodes['controller'].up('.header').toggleClassName('open');
			this.nodes['controller'].title = (this.nodes['content'].visible()) ? 'Click to Reveal' : 'Click to Close';
		},
		noRevealedContentCallback : function(scope){
			var header = scope.element.down(".header");
			if (header)
				header.setStyle({ background : "none" });
		}
	}),
	'UL.Browsing LI.showAbstract' : Revealer({
		effect : true,
		hidden_by_default : false,
		initialize_callback : function(){
			this.nodes['controller'].writeAttribute('title', 'Click to Reveal');
		},
		control_callback : function(){
			this.nodes['controller'].src = (this.nodes['content'].visible()) ? ASSETS_RELATIVE_PATH+'/img/btn.view-details.collapsed.gif' : ASSETS_RELATIVE_PATH+'/img/btn.view-details.expanded.gif';
			this.nodes['controller'].up('.header').toggleClassName('open');
			this.nodes['controller'].title = (this.nodes['content'].visible()) ? 'Click to Reveal' : 'Click to Close';
		}
	}),
	'DIV.advanced-search .DHTML' : Revealer({
		effect : true,
		hidden_by_default : false,
		initialize_callback : function(){
			this.nodes['controller'].addClassName('link');
			this.nodes['controller'].writeAttribute('title', 'Click to Reveal');
		},
		control_callback : function(){
			this.nodes['controller'].up('.header').toggleClassName('open');
			this.nodes['controller'].title = (this.nodes['content'].visible()) ? 'Click to Reveal' : 'Click to Close';
		}
	}),
	'ul.refinement LI.reveal .DHTML' : Revealer({
		effect : true,
		hidden_by_default : true,
		initialize_callback : function(){
			this.nodes['controller'].addClassName('link');
			this.nodes['controller'].writeAttribute('title', 'Click to Reveal');
		},
		control_callback : function(){
			this.nodes['controller'].up('.header').toggleClassName('open');
			this.nodes['controller'].title = (this.nodes['content'].visible()) ? 'Click to Reveal' : 'Click to Close';
		}
	}),
	'DIV.column-side-left DIV.box .DHTML' : Revealer({
		effect : true,
		hidden_by_default : true,
		initialize_callback : function(){
			this.nodes['controller'].addClassName('link');
			this.nodes['controller'].writeAttribute('title', 'Click to Reveal');
		},
		control_callback : function(){
			this.nodes['controller'].toggleClassName('open');
			this.nodes['controller'].title = (this.nodes['content'].visible()) ? 'Click to Reveal' : 'Click to Close';
		}
	}),

	'UL.listview' : SelectAll('toggle-all-checkboxes'),

	'#search-results-sort-by' : SelectObserver,

	'.tabbed-content' : TabSet({
		tab_selector : '.tab-menu LI A',
		tab_prefix : '',
		active_class : 'selected'
	}),

	'#highlights' : TabSet({
		tab_selector : '.pagination LI A',
		tab_prefix : '',
		active_class : 'selected',
		initialize_callback : function(){
			var footer = this.element.down('.footer');
			if (footer)
				footer.setStyle({ display : 'block' });
		}
	})

});

Event.addBehavior({
	'UL.Results' : SelectAll('toggle-all-checkboxes')
});

document.observe("dom:loaded", function(){

	function fixDisabledOptions() { $A(this.options).each(function(opt) { if ($(opt).disabled) opt.selected = false; }); }
	$$("SELECT").each(function(sel){ 
		Event.observe(sel, 'change', fixDisabledOptions.bind(sel));
	});

	if (Prototype.Browser.IE || Prototype.Browser.WebKit) {
		$$("LABEL IMG").each(function(image) {
			var el = $(image.parentNode.getAttribute('for') || image.parentNode.attributes['for'].nodeValue);
			if (el) { image.observe('click', function(){ el.focus(); }); }
		});
	}

});
