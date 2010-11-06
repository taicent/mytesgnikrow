var activeControl = null;

function manageActiveControl() {
  if (activeControl != null && activeControl != this) {
    activeControl.onClearButton.bindAsEventListener(activeControl)();
    activeControl.removeAllRefinements();
    activeControl = this;
  } else if (activeControl == null) {
    activeControl = this;
  }
  return true;
}

var IEEEDateSlider = Class.create(Control.Slider, {
  onClearButton: Prototype.emptyFunction,

  removeAllRefinements: function () {
    this.setValue(this.options.values.first(), 0);
    this.setValue(this.options.values.last(), 1);
    $('text_startyear').value = this.values[0];
    $('text_endyear').value = this.values[1];
    this.options.updateVisible = false;
    new Effect.BlindUp(this.track.up('table').next('.update-button'), {
      duration: 0.25
    });
  },

  handleSelectionTypeChoice: function(radioButton) {
    if (radioButton.value != this.options.selectionType) {
      if (radioButton.value == 'single') {
        new Effect.BlindUp('end-year-input', { duration: 0.25 });
        $('start-year-input').firstDescendant().update('Year:');
        $('end-date-slider').hide();
        this.setValue(this.options.values[this.options.values.length - 1], 1);
        this.options.selectionType = radioButton.value;
      } else if (radioButton.value == 'range') {
        $('start-year-input').firstDescendant().update('Min&nbsp;Year:');
        $('end-date-slider').show();
        new Effect.BlindDown('end-year-input', { duration: 0.25 });
        this.options.selectionType = radioButton.value;
      }
      if (!this.options.updateVisible) {
          this.options.updateVisible = true;
          new Effect.BlindDown(Element.down(Element.up(radioButton,'div'), '.update-button'), {
            duration: 0.25
          });
        }
    }
    return true;
  },

  selectClosestValue: function(valueBox, handleIndex) {
    manageActiveControl.bind(this)();
    var bestDiff = null;
    var bestValue = null;
    for (var i = 0; i < this.options.values.length; i++) {
      if (handleIndex == 0 && this.options.values[i] > valueBox.value) break;
      if (handleIndex == 1 && this.options.values[i] < valueBox.value) continue;
      var diff = Math.abs(valueBox.value - this.options.values[i]);
      if (bestDiff == null || bestDiff > diff) {
        bestDiff = diff;
        bestValue = this.options.values[i];
      }
    }
    if (handleIndex == 0 && valueBox.value < this.options.values.first()) {
      bestValue = this.options.values.first();
    } else if (handleIndex == 1 && valueBox.value > this.options.values.last()) {
      bestValue = this.options.values.last();
    }
    if (bestValue != null) {
      if (handleIndex - 1 >= 0 &&
        bestValue < this.values[handleIndex - 1]) {
        bestValue = this.values[handleIndex - 1];
        }
        if (handleIndex + 1 < this.values.length &&
          bestValue > this.values[handleIndex + 1]) {
          bestValue = this.values[handleIndex + 1];
          }
          this.setValue(bestValue, handleIndex);
          valueBox.value = bestValue;
    }
    if (!this.options.updateVisible) {
      this.options.updateVisible = true;
      new Effect.BlindDown(Element.down(Element.up(valueBox, 'div', 1), '.update-button'), {
        duration: 0.25
      });
    }
    return false;
  }
});

var IEEERefinementAutocompleter = Class.create(Ajax.Autocompleter, {

  addRefinement: function(refinement) {
    manageActiveControl.bind(this)();
    if (!this.options.selectedItems.any(function(id) { return id == refinement.id; })) {
      this.options.selectedItems.push(refinement.id);
    }
    var selectedList = this.options.selectedList;
    var header = selectedList.previous('h5');
    var updateButton = selectedList.next('.' + this.options.updateButtonClass);
    var backgroundColor = this.options.backgroundColor;
    var highlightColor = this.options.highlightColor;
    var afterEffect = function() {
      if (!selectedList.visible()) selectedList.show();
      refinement.down('.' + this.options.refinementClass).style.backgroundColor = backgroundColor;
      var effects = [];
      effects.push(new Effect.BlindDown(refinement));
      if (!header.visible())
        effects.push(new Effect.BlindDown(header));
      if (!updateButton.visible())
        effects.push(new Effect.BlindDown(updateButton));
      selectedList.appendChild(refinement);
      refinement.down('.' + this.options.selectorClass).checked = true;
      new Effect.Parallel(effects, { duration: 0.25 });
    };
    new Effect.BlindUp(refinement, {
        duration: 0.25,
        afterFinish: afterEffect.bind(this)
    });
  },

  removeRefinement: function(refinement) {
    var val = null;
    var length = this.options.selectedItems.length;
    for (var i = 0; i < this.options.selectedItems.length; i++) {
      val = this.options.selectedItems.shift();
      if (val != refinement.id) this.options.selectedItems.push(val);
    }
    var header = this.options.selectedList.previous('h5');
    var updateButton = this.options.selectedList.next('.' + this.options.updateButtonClass);
    var effects = [];
    if (this.options.selectedList.childElements().length == 1) {
      effects = [
        new Effect.BlindUp(this.options.selectedList),
        new Effect.BlindUp(header),
        new Effect.BlindUp(updateButton)
      ];
    }
    effects.push(new Effect.BlindUp(refinement));
    var refresh = this.refreshRefinementListContents.bind(this);
    var cleanSelected = this.cleanSelectedListContents.bind(this);
    new Effect.Parallel(effects, {
      duration: 0.25,
      afterFinish: function() {
        refresh();
        cleanSelected();
      }
    });
  },

  removeAllRefinements: function() {
    var length = this.options.selectedItems.length;
    for (var i = 0; i < length; i++) {
      this.options.selectedItems.shift();
    }
    var header = this.options.selectedList.previous('h5');
    var updateButton = this.options.selectedList.next('.' + this.options.updateButtonClass);
    var effects = [
      new Effect.BlindUp(this.options.selectedList),
      new Effect.BlindUp(header),
      new Effect.BlindUp(updateButton)
    ];
    var items = this.options.selectedList.childElements();
    for (var i = 0; i < items.length; i++) {
      effects.push(new Effect.BlindUp(items[i]));
    }
    var refresh = this.refreshRefinementListContents.bind(this);
    var cleanSelected = this.cleanSelectedListContents.bind(this);
    new Effect.Parallel(effects, {
      duration: 0.25,
      afterFinish: function() {
        refresh();
        cleanSelected();
      }
    });
  },

  checkboxHandler: function(event) {
    var checkbox = Event.element(event);
    var refinement = checkbox.up('li');
    if (checkbox.checked) {
      this.addRefinement(refinement);
    } else {
      this.removeRefinement(refinement);
    }
  },

  initializeItem: function(item) {
    item.show();
    checkbox = item.down('.' + this.options.selectorClass);
    checkbox.stopObserving('click');
    checkbox.observe('click', this.checkboxHandler.bindAsEventListener(this));
    checkbox.checked = false;
  },

  replaceRefinementListContents: function(newItems) {
    while (!this.options.refinementList.empty()) {
      this.options.refinementList.down().remove();
    }
    var newItem = null;
    for (var i = 0; i < newItems.length; i++) {
      newItem = newItems[i];
      newItem.down('.' + this.options.refinementClass).style.backgroundColor = this.options.backgroundColor;
      if (!this.isItemSelected(newItem)) {
        this.initializeItem(newItem);
        this.options.refinementList.appendChild(newItem);
      }
    }
    this.listContents = newItems;
  },

  refreshRefinementListContents: function() {
    var item = null;
    var refinementItems = this.options.refinementList.select('li');
    var nextRefinementItem = null;
    var j = 0;
    for (var i = 0; i < this.listContents.length; i++) {
      item = this.listContents[i];
      if (!this.isItemSelected(item)) {
        this.initializeItem(item);
        if (j < refinementItems.length) {
          nextRefinementItem = refinementItems[j];
          if (item.id != nextRefinementItem.id) {
            Element.insert(nextRefinementItem, { before: item });
            this.revealRefinementItem(item);
          } else j++;
        } else {
          Element.insert(this.options.refinementList, { bottom: item });
          this.revealRefinementItem(item);
        }
      }
    }
  },

  cleanSelectedListContents: function() {
    var selected = this.options.selectedList.select('li');
    for (var i = 0; i < selected.length; i++) {
      if (!selected[i].visible()) {
        selected[i].remove();
      }
    }
  },

  revealRefinementItem: function(item) {
    var highlightColor = this.options.highlightColor;
    var afterEffect = function() {
      item.down('.' + this.options.refinementClass).style.backgroundColor = highlightColor;
    }
    new Effect.BlindDown(item, {
        duration: 0.25,
        afterFinish: afterEffect.bind(this)
    });
    item.down('.' + this.options.refinementClass).style.backgroundColor = highlightColor;
  },

  isItemSelected: function(item) {
    return this.options.selectedItems.any(function(id) { return id == item.id });
  },

  onComplete: function(request) {
    this.updateChoices(request.responseText);
    this.replaceRefinementListContents(this.update.select('li'));
    if(this.getToken().length>=this.options.minChars) {
      this.element.next('.' + this.options.clearButtonClass).show();
    }
  },

  onKeyUp: function () {
    if(this.getToken().length>=this.options.minChars) {
      this.element.next('.' + this.options.clearButtonClass).show();
    } else {
      this.element.next('.' + this.options.clearButtonClass).hide();
      this.replaceRefinementListContents(this.options.defaultItems);
    }
  },

  onClearButton: function() {
    this.element.value = '';
    this.element.next('.' + this.options.clearButtonClass).hide();
    this.replaceRefinementListContents(this.options.defaultItems);
  },

  onHover: Prototype.emptyFunction,
  onClick: Prototype.emptyFunction,
  onBlur: Prototype.emptyFunction,
  updateElement: function(selectedElement) {}
});

function applyRefinements(selectionsUl, submitUrl, param) {
  var url_string =  submitUrl + document.getElementById("oqs").value;
  var childRefinements = $(selectionsUl).childElements();
  var i;		var j=0;		var vals= new Array();
  for(i = 0; i < childRefinements.length; i++) {
    url_string+='&' + param + '='+ childRefinements[i].id;
  }
   url_string += '&pageNumber=1&resultAction=REFINE&history=no';
   
  window.location = url_string;
}

function applyDateRange(submitUrl, param, propertyName) {
  var url_string =  submitUrl + document.getElementById("oqs").value ;
  var start_year = document.getElementById('text_startyear').value;
  var end_year = start_year;
  if (document.getElementById("radio_range").checked) {
  	end_year = document.getElementById('text_endyear').value;
  }
  url_string += param + "=" + start_year + "_" + end_year + "_" + propertyName;
  url_string += '&pageNumber=1&resultAction=REFINE&history=no';
  
  window.location = url_string;
}

var selectionType;
var DateValues;
var DateSlider;
var DateSliderValues;

function addDateSlider(minYear, maxYear) {
  var handles = [$('start-date-slider'), $('end-date-slider')];
  DateSliderValues = [minYear, maxYear];
  DateSlider = new IEEEDateSlider(handles, 'Date-slider', {
    range: $R(DateValues.first(), DateValues.last()),
    values: DateValues, restricted: true,
    sliderValue: [DateValues.first(), DateValues.last()],
    handleImage: ['start-date-slider-image','end-date-slider-image'],
    alignX: -5, updateVisible: false,
    onSlide: function(values, slider) {
    	currentStart = $('text_startyear');
    	currentEnd = $('text_endyear');
    	if (currentStart.value != values[0] || currentEnd.value != values[1]) {
    		if (!manageActiveControl.bind(slider)(function() {
    			this.setValues($('text_startyear').value, 0);
    			this.setVlaues($('text_endyear').value, 1);
    		} ) ) {
    			return false;
    		}
    		currentStart.value = values[0];
    		currentEnd.value = values[1];
    		var updateButton = slider.track.up('table').next('.update-button');
    		if (!slider.options.updateVisible) {
    			slider.options.updateVisible = true;
    			new Effect.BlindDown(updateButton, { duration: 0.25 });
    		}
    	}
    }, 
    selectionType: 'range'
  });
}
function getFilteredTermsExt(terms, ignoreBrackets) {
	// Replace multiple adjacent wildcards or space with singles.
	terms = terms.replace(/\*+/, "*").replace(/\s+/, " ");
	
	// Replace & with .AND., = with .EQ.: these are used to split request
	// parameters/values by the servlet.
	for (var i = 0; i < SPECIAL_CHARACTERS.length; i++) {
		if(ignoreBrackets && (SPECIAL_CHARACTERS[i] == '(' || SPECIAL_CHARACTERS[i] == ')')) {
			continue;
		}
		var index = terms.indexOf(SPECIAL_CHARACTERS[i]);
		while (index > -1) {
			terms = terms.replace(SPECIAL_CHARACTERS[i], SPECIAL_CHARACTER_REPLACEMENTS[i]);
			index = terms.indexOf(SPECIAL_CHARACTERS[i]);
		}
	}
	// Trim the string
	terms = terms.replace(/^\s+|\s+$/g, "");
	return terms;
}
/*
 * Filter the input string and return.
 */
function getFilteredTerms(terms) {
	return getFilteredTermsExt(terms, false);
}

/*
 * Validate and filter the text in the input element.
 */
function filterQueryElement(textField) {
	var terms = textField.value;
	if (!terms.match(/\w/)) {
		alert("Please enter one or more keywords")
		return false;
	}
	terms = getFilteredTerms(terms);
	
	// Check that there are no more than 2 wildcarded words.
	var stars = terms.match(/\*/g);
	if (stars != null && stars.length > 2) {
		alert("Please limit the number of wildcarded words to 2")
		return false;
	}
	
	// Check that wild-carded words contain at least 3 valid characters.
	parts = terms.split(/[^*_a-z_A-Z_0-9]/);
	for (var i = 0; i < parts.length; i++) {
		if (parts[i].indexOf("*") > -1) {
			var validChars = parts[i].match(/\w/g);
			if (validChars == null || validChars.length < 3) {
				alert("Please provide at least 3 valid characters in wildcarded word (" + parts[i] + ")");
				return false;		
			}
		}
	}
		
	// Replace the text field value;
	textField.value = terms;
	return true;
}

/*
 * Validate and filter the text in element with id textFieldId.
 */
function filterQuery(textFieldId) {
	var textField = document.getElementById(textFieldId);
	return filterQueryElement(textField);
}
