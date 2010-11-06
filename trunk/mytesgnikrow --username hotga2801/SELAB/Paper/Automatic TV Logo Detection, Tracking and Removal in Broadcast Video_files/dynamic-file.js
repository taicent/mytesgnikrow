
(function(){window.onload=function(){var ot=document.getElementsByTagName('table')[0];var logoDiv=ot.getElementsByTagName('div')[0];var link=document.createElement('a');var img=new Image();link.href='/home/link-over.mpx';link.title='Try the new beta site.';link.appendChild(img);img.alt='Springerlink Beta';img.onload=function(){logoDiv.parentNode.insertBefore(link,logoDiv.nextSibling);};img.src='/images/TryNewBeta.gif';};})();

function AjaxRightsChecker()
{function RightsQuery(callback,context,query,args)
{this.callback=callback;this.context=context;this.query=query;this.args=args;}
function PostCheckEvent(callback,context,args)
{this.callback=callback;this.context=context;this.args=args;}
this.currentQueryIndex=0;this.queries=new Array();this.currentPostEventIndex=0;this.postCheckEvents=new Array();this.currentPostEventScriptIndex=0;this.postCheckEventScripts=new Array();this.addQuery=function(callback,context,query,args)
{this.queries[this.currentQueryIndex]=new RightsQuery(callback,context,query,args);this.currentQueryIndex++;}
this.addPostCheckEvent=function(callback,context,args)
{this.postCheckEvents[this.currentPostEventIndex]=new PostCheckEvent(callback,context,args);this.currentPostEventIndex++;}
this.addPostCheckEventScript=function(script)
{this.postCheckEventScripts[this.currentPostEventScriptIndex]=script;this.currentPostEventScriptIndex++;}
this.processQueries=function()
{var queries=new Array();for(k=0;k<this.currentQueryIndex;k++)
queries[k]=this.queries[k].query;MetaPress.Products.Reader.Application.RightsChecking.AjaxRightsChecker.ProcessQueries(queries,AjaxRightsChecker_ProcessCallback,this);}
this.processResults=function(results)
{var query;var k;if(results)
{for(k=0;k<this.currentQueryIndex;k++)
{query=this.queries[k];query.context[query.callback](results[k],query.args);}}
for(k=0;k<this.currentPostEventIndex;k++)
{postEvent=this.postCheckEvents[k];postEvent.context[postEvent.callback](postEvent.args);}
for(k=0;k<this.currentPostEventScriptIndex;k++)
{eval(this.postCheckEventScripts[k]);}}}
function AjaxRightsChecker_ProcessCallback(result)
{if(result.error)
alert(result.error.Message+' : '+result.error.Type);else
result.context.processResults(result.value);}

function latex_initClientSide()
{latex_showClientSide();try
{jsMath.Autoload.Check();jsMath.Process();}
catch(e)
{if(handleException)
handleException(e);}
jsMath.Synchronize('latex_hideErrors()');}
function latex_showClientSide()
{var tex=document.getElementsByTagName('SPAN');for(k=0;k<tex.length;k++)
{if(tex[k].className=='cs')
tex[k].style.display='inline';else if(tex[k].className=='ss')
tex[k].style.display='none';}}
function latex_hideErrors()
{var tex=document.getElementsByTagName('SPAN');for(k=0;k<tex.length;k++)
{if(tex[k].className=='error')
{var parent=getParent(tex[k]);if(parent&&parent.className=='typeset')
{parent=getParent(parent);if(parent&&parent.className=='cs')
{parent=getParent(parent);if(parent&&parent.className=='latex')
{parent.childNodes[0].style.display='inline';parent.childNodes[1].style.display='none';}}}}}}

function getElement(id)
{return document.getElementById(id);}
function getRoot(element,key,tagName)
{while(element!=null)
{if(element.tagName)
{if(element.tagName==tagName||(tagName=='DIV'&&element.tagName=='TABLE'))
{if(getAttributeValue(element,'key')==key)
return element;}}
element=getParent(element);}
return null;}
function getElementByClassAndTagName(parent,className,tagName)
{var elements=parent.getElementsByTagName(tagName);var matches=new Array;var index=0;for(var k=0;k<elements.length;k++)
{if(elements[k].className==className)
{return elements[k];}}
return null;}
function getElementsByClassAndTagName(parent,className,tagName)
{var elements=parent.getElementsByTagName(tagName);var matches=new Array;var index=0;for(var k=0;k<elements.length;k++)
{if(elements[k].className==className)
{matches[index]=elements[k];index++;}}
return matches;}
function getElementByKeyAndTagName(parent,key,tagName)
{var elements=parent.getElementsByTagName(tagName);for(var k=0;k<elements.length;k++)
{if(getAttributeValue(elements[k],'key')==key)
return elements[k];}
if(tagName=='DIV')
{elements=parent.getElementsByTagName('TABLE');for(var k=0;k<elements.length;k++)
{if(getAttributeValue(elements[k],'key')==key)
return elements[k];}}
return null;}
function getElementsByKeyAndTagName(parent,key,tagName)
{var elements=parent.getElementsByTagName(tagName);var matches=new Array;var index=0
for(var k=0;k<elements.length;k++)
{if(getAttributeValue(elements[k],'key')==key)
{matches[index]=elements[k];index++;}}
if(tagName=='DIV')
{elements=parent.getElementsByTagName('TABLE');for(var k=0;k<elements.length;k++)
{if(getAttributeValue(elements[k],'key')==key)
{matches[index]=elements[k];index++;}}}
return matches;}
function getParent(element)
{if(element.parentNode)
return element.parentNode
else if(element.parentElement)
return element.parentElement;else
return null;}
function getAttributeValue(element,attributeName)
{if(element==null)
return null;if(element.attributes)
{var attribute=element.attributes.getNamedItem(attributeName);if(attribute)
return attribute.value;}
else if(element.getAttribute)
{return element.getAttribute(attributeName);}
return'';}
function setAttributeValue(element,attributeName,attributeValue)
{if(element&&element.attributes)
{if(attributeValue==null)
{element.removeAttribute(attributeName);}
else
{var attribute=document.createAttribute(attributeName);attribute.value=attributeValue;element.attributes.setNamedItem(attribute);}}}
function getInnerText(element)
{if(element.innerText)
return element.innerText
else if(document.createRange)
{var range=document.createRange();this.range.selectNodeContents(this.element);return range.toString();}
else
return'';}
function setInnerText(element,value)
{if(element.innerText)
element.innerText=value;else if(document.createRange)
{var range=document.createRange();range.selectNodeContents(element);range.deleteContents();element.appendChild(document.createTextNode(value));}}
function setDefaultFocus(elementId)
{var element=getElement(elementId);if(element==null)
return false;else
element.focus();}
function click(element)
{if(element.click)
element.click();else
{var evt=document.createEvent('MouseEvents');evt.initMouseEvent('click',true,true,window,0,0,0,0,0,false,false,false,false,0,null);element.dispatchEvent(evt);}}
function addHandler(element,eventName,eventHandler,owner)
{if(!element)
return false;var func
if(owner)
func=function(){owner[eventHandler](element,owner);};else
func=eventHandler;if(element.addEventListener)
{if(eventName.substring(0,2)=='on')
eventName=eventName.substring(2);element.addEventListener(eventName,func,false);}
else if(element.attachEvent)
element.attachEvent(eventName,func);else
element[eventName]=func;}
function removeHandler(element,eventName,eventHandler)
{if(!element)
return false;if(element.removeEventListener)
{if(eventName.substring(0,2)=='on')
eventName=eventName.substring(2);element.removeEventListener(eventName,eventHandler,false);}
else if(element.detachEvent)
element.detachEvent(eventName,eventHandler);else
element[eventName]=null;}
function clearHandlers(element,eventName)
{element[eventName]=null;}
function getEvent(e)
{if(e)
return e;else
return window.event;}
function getSender(e)
{if(e.srcElement)
return e.srcElement
else
return e.currentTarget;}
function abortEvent(e)
{if(isIE)
{e.cancelBubble=true;e.returnValue=false;}
else
{e.stopPropagation();e.preventDefault();}}
function getCurrentStyle(element)
{if(element==null)
return null;if(element.currentStyle)
return element.currentStyle
else if(window.getComputedStyle)
return window.getComputedStyle(element,'');else
return null;}
function copyPrototype(descendant,parent)
{var sConstructor=parent.toString();var aMatch=sConstructor.match(/\s*function (.*)\(/);if(aMatch!=null){descendant.prototype[aMatch[1]]=parent;}
for(var m in parent.prototype){descendant.prototype[m]=parent.prototype[m];}};function pendMethodCall(method,delay,element,timerKey)
{var key=getAttributeValue(element,timerKey);if(key!='')
clearTimeout(key);key=setTimeout(method,delay);setAttributeValue(element,timerKey,key);}
function asyncMethodCall(method,delay,owner,timerKey)
{if(owner.getTimer&&owner.setTimer)
{var timer=owner.getTimer(timerKey);if(timer!='')
clearTimeout(key);timer=setTimeout(method,delay);owner.setTimer(timerKey,timer);}
else
setTimeout(method,delay);}
function getQueryStringValue(key)
{var query=window.location.search.substring(1);var vars=query.split("&");for(var i=0;i<vars.length;i++)
{var pair=vars[i].split("=");if(pair[0]==key)
return pair[1];}
return'';}
function setFocus(element)
{if(element!=null&&canAcceptFocus(element))
{element.focus();switch(element.tagName)
{case'SELECT':if(element.options.length>0)
element.selectedIndex=0;break;case'INPUT':switch(getAttributeValue(element,'type'))
{case'','text','password':element.select();break;}
break;}}}
function canAcceptFocus(element)
{if(element.style.display=='none')
return false;return true;}
function isDefined(variableName)
{return window[variableName]!=null;}
function toggleVisibility(element,visibleStyle)
{if(element==null)
return;if(visibleStyle==null)
visibleStyle='';if(getCurrentStyle(element).display=='none')
element.style.display=visibleStyle;else
element.style.display='none';}
function trim(text)
{return text;while(text.substring(0,1)==' ')
{text=text.substring(1,text.length);}
while(text.substring(text.length-1,text.length)==' ')
{text=text.substring(0,text.length-1);}}
function startsWith(string,value)
{if(string==null||value==null)
return false;if(string==''||value=='')
return false;if(string.length<value.length)
return false;return string.substring(string,value.length)==value;}
function initValidators()
{if(typeof ValidatorOnLoad=='undefined')
return;ValidatorOnLoad();var i;for(i=0;i<Page_Validators.length;i++){ValidatorValidate(Page_Validators[i]);}
ValidatorUpdateIsValid();Page_BlockSubmit=!Page_IsValid;return Page_IsValid;}
function beginAsync(object,method,delay,timer)
{if(timer&&timer!='')
clearTimeout(timer);var parameters=[];for(var k=4;k<arguments.length;k++)
parameters.push(arguments[k]);var func=function(){method.apply(object,parameters);};timer=setTimeout(func,delay);return timer;}

function selectedItemsHyperLink_onLoad(collection)
{selectedItemsHyperLink_attachControls(collection);selectedItems_updateSelectedItemsHyperLinkCounts(collection);}
function selectedItemsHyperLink_attachControls(collection)
{var boxes=selectedItems_getControls(collection);for(k=0;k<boxes.length;k++)
{addHandler(boxes[k],'onclick',selectedItemsHyperLink_onCheckChanged);selectedItemLinkButton_onLoad(boxes[k]);}}
function selectedItemsHyperLink_getCount(collection)
{return window[collection+'Count'];}
function selectedItemsHyperLink_setCount(collection,count)
{window[collection+'Count']=count;}
function selectedItemsHyperLink_onCheckChanged(e)
{sender=getSender(e);if(sender.tagName!='A')
sender=getParent(sender);var collection=getAttributeValue(sender,'collection');if(selectedItems_getCount(collection)>=100)
{alert('Cannot have more than 100 items');return false;}
selectedItems_updateSelectedItemsHyperLinkCounts(collection);}

function selectedItems_getCount(collection)
{var c=getCookie(collection);if(c)
{return c.split(',').length-1;}
else
return 0;}
function selectedItems_setSelected(collection,value,select)
{if(select)
{if(getCookie(collection)==null)
setCookie(collection,",'"+value+"'");else
setCookie(collection,getCookie(collection)+",'"+value+"'");}else{setCookie(collection,getCookie(collection).replace(",'"+value+"'",""));}}
function selectedItems_isSelected(collection,value)
{var c=getCookie(collection);if(c==null)
return false;else
return(getCookie(collection).indexOf(",'"+value+"'")>-1);}
function getCookie(key)
{if(document.cookie.length>0)
{begin=document.cookie.indexOf(key+"=");if(begin!=-1){begin+=key.length+1;end=document.cookie.indexOf(";",begin);if(end==-1)
end=document.cookie.length;return unescape(document.cookie.substring(begin,end));}}
return null;}
function setCookie(key,value)
{document.cookie=key+"="+escape(value)+";path=/";}
function delCookie(key)
{if(getCookie(key))
{document.cookie=key+"="+"; expires=Thu, 01-Jan-70 00:00:01 GMT";}}
function selectedItems_getControls(collection)
{var controls=new Array();var boxes=document.getElementsByTagName('A');for(k=0;k<boxes.length;k++)
{if(getAttributeValue(boxes[k],'collection')==collection&&getAttributeValue(boxes[k],'isSelectedItemControl')=='true')
{controls=controls.concat(boxes[k]);}}
return controls;}
function selectedItems_setSelectedItemsHyperLinkText(link,count)
{var span=link.childNodes[1];if(count==0||count==null)
{setInnerText(span,'');link.className=link.className.replace('markedItemsHasItems','markedItemsNoItems');}
else
{span.innerHTML='&nbsp;('+count+')';link.className=link.className.replace('markedItemsNoItems','markedItemsHasItems');}}
function selectedItems_updateSelectedItemsHyperLinkCounts(collection)
{var links=document.getElementsByTagName('a');var count=selectedItems_getCount(collection);for(var k=0;k<links.length;k++)
{if(getAttributeValue(links[k],'isSelectedItemsLink')=='true'&&getAttributeValue(links[k],'displayCount')=='true'&&getAttributeValue(links[k],'collection')==collection)
selectedItems_setSelectedItemsHyperLinkText(links[k],count);}}

function getWidth(element)
{var width;if(element.offsetWidth)
width=element.offsetWidth;else if(element.clientWidth)
width=element.clientWidth;else
width=element.scrollWidth;width=parseInt(width);if(isNaN(width))
return 0;else
return width;}
function getHeight(element)
{var height;if(element.offsetHeight)
height=element.offsetHeight;else if(element.clientHeight)
height=element.clientHeight;else
height=element.scrollHeight;height=parseInt(height);if(isNaN(height))
return 0;else
return height;}
function getLeft(element)
{var curLeft=0;if(element.offsetParent)
{while(element.offsetParent)
{curLeft+=element.offsetLeft;element=element.offsetParent;}}
else if(element.x)
curLeft=element.x;return curLeft;}
function getLeftRelative(element)
{if(element.offsetParent)
return element.offsetLeft;else if(element.x)
return element.x;return 0.0;}
function getTop(element)
{var curTop=0;if(element.offsetParent)
{while(element.offsetParent)
{curTop+=element.offsetTop;element=element.offsetParent;}}
else if(element.y)
curTop=element.y;return curTop;}
function getTopRelative(element)
{if(element.offsetParent)
return element.offsetTop;else if(element.y)
return element.y;return 0.0;}
function getRightAlignOffset(element,target)
{var offset=getLeft(target);offset+=getWidth(target);offset-=getWidth(element);return offset;}
function getStringWidth(string)
{if(string=='')
return 0;var div=createHiddenDiv();div.innerHTML=string;return getWidth(div);}
function getWindowWidth()
{if(window.innerWidth)
return window.innerWidth;else if(document.documentElement&&document.documentElement.clientWidth)
return document.documentElement.clientWidth;else if(document.body&&document.body.clientWidth)
return document.body.clientWidth;else
return-1;}
function createHiddenDiv()
{var div=getElement('hiddenDiv');if(div==null)
{div=document.createElement('div');div.style.position="absolute";div.style.left=-10000;div.id='hiddenDiv';div.style.padding='0px';div.style.margin='0px';document.body.appendChild(div);}
return div;}
function getScrollBarSize(){var scr=null;var inn=null;var wNoScroll=0;var wScroll=0;scr=document.createElement('div');scr.style.position='absolute';scr.style.top='-1000px';scr.style.left='-1000px';scr.style.width='100px';scr.style.height='50px';scr.style.overflow='hidden';inn=document.createElement('div');inn.style.width='100%';inn.style.height='200px';scr.appendChild(inn);document.body.appendChild(scr);wNoScroll=inn.offsetWidth;scr.style.overflow='auto';wScroll=inn.offsetWidth;document.body.removeChild(document.body.lastChild);return(wNoScroll-wScroll);}
function getViewportHeight()
{if(typeof window.innerHeight!='undefined')
return window.innerHeight;else if(typeof document.documentElement!='undefined'&&typeof document.documentElement.clientHeight!='undefined'&&document.documentElement.clientHeight!=0)
return document.documentElement.clientHeight;else
return document.getElementsByTagName('BODY')[0].clientHeight;}

function popOutMenuItem_init()
{var items=document.getElementsByTagName('A');for(var k=0;k<items.length;k++)
{if(items[k].className=='noScriptLink')
items[k].style.cursor='default';}}
function popOutMenuItem_addEventHandlers(itemId)
{var item=getElement(itemId);if(document.getElementById)
{item.onmouseover=popOutMenuItem_expand;item.onmouseout=popOutMenuItem_collapse;var children=item.getElementsByTagName('LI');for(var k=0;k<children.length;k++)
{item=children[k];if(item.attributes.getNamedItem('direction'))
{item.onmouseover=popOutMenuItem_expand;item.onmouseout=popOutMenuItem_collapse;}}}}
function popOutMenuItem_collapse(e)
{e=getEvent(e);var sender=getSender(e);this.className=this.className.replace('expanded','collapsed');if(this.iFrame)
{this.iFrame.className='iFrameCollapsed';this.iFrame.style.display='none';}}
function popOutMenuItem_getPopOutDirection(item)
{var direction=item.attributes.getNamedItem('direction')
if(direction&&direction.value=='vertical')
return'vertical';else
return'horizontal';}
function popOutMenuItem_expand(e)
{e=getEvent(e);var sender=getSender(e);this.className=this.className.replace('collapsed','expanded');if(this.iFrame)
{this.iFrame.style.left=getLeft(this);this.iFrame.style.top=getTop(this);this.iFrame.style.width=getIFrameWidth(this)+'px';this.iFrame.style.height=getIFrameHeight(this)+'px';this.iFrame.className='iFrameExpanded';this.iFrame.style.display='block';}
if(popOutMenuItem_getPopOutDirection(this)=='vertical')
popOutMenuItem_popOutVertical(this);else
popOutMenuItem_popOutHorizontal(this);}
function popOutMenuItem_popOutHorizontal(item)
{var width=getWidth(getParent(item));if(isIE)
{var padding=parseInt(getCurrentStyle(item).paddingLeft);padding-=parseInt(getCurrentStyle(getParent(getParent(item))).paddingRight);width-=padding;}
var children=item.childNodes;var child;for(var k=0;k<children.length;k++)
{child=children[k];if(child.style)
{child.style.top=0;child.style.left=width;}}}
function popOutMenuItem_popOutVertical(item)
{var height=getHeight(item);height-=1;var children=item.childNodes;var child;for(var k=0;k<children.length;k++)
{child=children[k];if(child.style)
{child.style.top=height;child.style.left=0;}}}
function getIFrameWidth(sender)
{var maxWidth=getWidth(sender);var children=sender.childNodes;for(k=0;k<children.length;k++)
{var item=children[k];var width=getWidth(item);if(width>maxWidth)
maxWidth=width;}
return maxWidth;}
function getIFrameHeight(sender)
{var totalHeight=getHeight(sender);var children=sender.childNodes;for(k=0;k<children.length;k++)
totalHeight+=getHeight(children[k]);return totalHeight;}
function addIFrames()
{var items=document.getElementsByTagName('LI');for(var k=0;k<items.length;k++)
{if(getAttributeValue(items[k],'direction')!='')
{var iFrame=document.createElement('IFRAME');iFrame.frameBorder=0;iFrame.src='javascript:;';iFrame.className='collapsed';iFrame.style.position='absolute';iFrame.style.width='0px';iFrame.style.height='0px';iFrame.style.display='none';iFrame.style.filter='progid:DXImageTransform.Microsoft.Alpha(style=0,opacity=0)';items[k].iFrame=items[k].parentNode.insertBefore(iFrame,items[k]);iFrame.zIndex=1000;items[k].zIndex=10000;}}}

function PrimitiveHeadingControl(id)
{this.element=getElement(id);this.requestSampleDiv=getElement(id+'_RequestSample');this.toggleRequestSampleDiv=function(permission)
{if(permission!=3)
return false;this.requestSampleDiv.style.display='none';}
this.processAccessQuery=function(permission)
{if(this.requestSampleDiv)
this.toggleRequestSampleDiv(permission);}}
function primitiveHeadingControl_markItem(sender)
{var value=getAttributeValue(sender,'value');var selected=selectedItems_isSelected('SelectedPrimitives',value);if(selected)
{selectedItems_setSelected('SelectedPrimitives',value,false);var text=getAttributeValue(sender,'unselectedText');}
else
{selectedItems_setSelected('SelectedPrimitives',value,true);var text=getAttributeValue(sender,'selectedText');}
setInnerText(sender.childNodes[0],text);selectedItems_updateSelectedItemsHyperLinkCounts('SelectedPrimitives');}
function primitiveHeadingControl_toggleTocAlert(sender)
{var code=getAttributeValue(sender,'value');if(!MetaPress.Products.Reader.Web.UI.PrimitiveHeadingControl.ToggleTocAlert(code).value)
return true;var eText=getAttributeValue(sender,'enabledText');var dText=getAttributeValue(sender,'disabledText');if(getInnerText(sender)==eText)
setInnerText(sender,dText);else
setInnerText(sender,eText);return false;}

function hyperLinkButton_getTrueSender(sender)
{var isSender=false;isSender=(sender.tagName=='A'&&getAttributeValue(sender,'value'));while(sender&&!isSender)
{sender=getParent(sender);isSender=(sender.tagName=='A'&&getAttributeValue(sender,'value'));}
return sender;}
function hyperLinkButton_onClick(e)
{e=getEvent(e);var sender=hyperLinkButton_getTrueSender(getSender(e));var methodName=getAttributeValue(sender,'onClickMethod');if(methodName!=''&&window[methodName])
var ret=window[methodName](sender);return ret||false;}

function advancedTextBox_onLoad(senderId)
{var sender=getElement(senderId);var expandButton=getElementByKeyAndTagName(sender,'expand','INPUT');if(expandButton)
expandButton.style.display='inline';if(getAttributeValue(sender,'gdm')>0)
getElementByKeyAndTagName(sender,'go','INPUT').style.display='none';advancedTextBox_position(sender);addHandler(getElementByKeyAndTagName(sender,'text','INPUT'),'onblur',advancedTextBox_onBlur);addHandler(getElementByKeyAndTagName(sender,'go','INPUT'),'onclick',advancedTextBox_onGo);}
function advancedTextBox_position(sender)
{var textBox=getElementByKeyAndTagName(sender,'text','INPUT');var div=getElementByKeyAndTagName(sender,'contents','DIV');if(getAttributeValue(sender,'da')=='1')
div.style.left=(getLeft(textBox)+getWidth(textBox))-getWidth(div);else
div.style.left=getLeft(textBox);div.style.top=getTop(textBox)+getHeight(textBox);}
function advancedTextBox_collapse(sender)
{sender.style.display='none';sender.style.left=1000000;}
function advancedTextBox_expand(sender)
{sender.style.display='block';advancedTextBox_position(getParent(sender));}
function advancedTextBox_toggle(e)
{e=getEvent(e);var sender=getParent(getSender(e));var div=getElementByKeyAndTagName(sender,'contents','DIV');if(getCurrentStyle(div).display=='block')
advancedTextBox_collapse(div);else
advancedTextBox_expand(div);getElementByKeyAndTagName(sender,'text','INPUT').focus();return false;}
function advancedTextBox_onBlur(e)
{if(!document.activeElement||document.activeElement==document.body)
return false;e=getEvent(e);var sender=getSender(e);var parent=getParent(sender);var current=document.activeElement;while(current)
{if(current==parent)
return true;current=getParent(current);}
var div=getElementByKeyAndTagName(parent,'contents','DIV');advancedTextBox_collapse(div);}
function advancedTextBox_onGo(e)
{e=getEvent(e);var sender=getSender(e);var parent=getParent(sender);var div=getElementByKeyAndTagName(parent,'contents','DIV');advancedTextBox_collapse(div);}

function defaultTextBox_captureEnter(e)
{e=getEvent(e);var sender=getSender(e);var target=getElement(getAttributeValue(sender,'target'));if(target==null)
{alert('target for '+sender.id+' is null');return false;}
if(e.keyCode==13)
{abortEvent(e);switch(getAttributeValue(sender,'action'))
{case'click':defaultTextBox_click(target);break;case'focus':setFocus(target);break;default:defaultTextBox_click(target);}}}
function defaultTextBox_click(element)
{if(element!=null)
element.click();}
function defaultTextBox_setFocus(element)
{if(element!=null)
{element.focus();switch(element.tagName)
{case'SELECT':if(element.options.length>0)
element.selectedIndex=0;break;case'INPUT':switch(getAttributeValue(element,type))
{case'','text','password':element.select();break;}
break;}}}

function queryBuilder_wireUpEvents(senderId)
{var sender=queryBuilder_getRoot(getElement(senderId));var textBox=queryBuilder_getTextBox(sender);var operatorList=queryBuilder_getOperatorList(sender);var fieldList=queryBuilder_getFieldList(sender);for(var k=0;k<fieldList.childNodes.length;k++)
{if(fieldList.childNodes[k].className!='empty')
{addHandler(fieldList.childNodes[k].childNodes[0],'onclick',queryBuilder_onFieldClick);addHandler(fieldList.childNodes[k],'onclick',queryBuilder_onFieldClick);addHandler(fieldList.childNodes[k],'onmouseover',queryBuilder_onMouseOver);addHandler(fieldList.childNodes[k],'onmouseout',queryBuilder_onMouseOut);}}
for(var k=0;k<operatorList.childNodes.length;k++)
{if(operatorList.childNodes[k].className!='empty')
{if(getAttributeValue(operatorList.childNodes[k],'arg')=='*')
{addHandler(operatorList.childNodes[k].childNodes[0],'onclick',queryBuilder_onWildcardClick);addHandler(operatorList.childNodes[k],'onclick',queryBuilder_onWildcardClick);}
else
{addHandler(operatorList.childNodes[k].childNodes[0],'onclick',queryBuilder_onOperatorClick);addHandler(operatorList.childNodes[k],'onclick',queryBuilder_onOperatorClick);}
addHandler(operatorList.childNodes[k],'onmouseover',queryBuilder_onMouseOver);addHandler(operatorList.childNodes[k],'onmouseout',queryBuilder_onMouseOut);}}
addHandler(textBox,'onclick',queryBuilder_updateSelection);addHandler(textBox,'onselect',queryBuilder_updateSelection);addHandler(textBox,'onfocus',queryBuilder_updateSelection);addHandler(textBox,'onkeyup',queryBuilder_updateSelection);addHandler(textBox,'onkeydown',queryBuilder_onKeyPress);var div=getElementByKeyAndTagName(sender,'contents','DIV');advancedTextBox_collapse(div);}
function queryBuilder_getRoot(sender)
{while(sender!=null&&getAttributeValue(sender,'key')!='advancedTextBox')
sender=getParent(sender);return sender;}
function queryBuilder_getTextBox(sender)
{return getElementByKeyAndTagName(queryBuilder_getRoot(sender),'text','INPUT');}
function queryBuilder_getOperatorList(sender)
{return getElementByKeyAndTagName(queryBuilder_getRoot(sender),'operatorList','UL');}
function queryBuilder_getFieldList(sender)
{return getElementByKeyAndTagName(queryBuilder_getRoot(sender),'fieldList','UL');}
function queryBuilder_getSelectionStart(sender)
{var start=getAttributeValue(queryBuilder_getRoot(sender),'ss');if(start=='')
return 0;else
return start;}
function queryBuilder_setSelectionStart(sender,selectionStart)
{setAttributeValue(queryBuilder_getRoot(sender),'ss',selectionStart);}
function queryBuilder_getSelectionEnd(sender)
{var end=getAttributeValue(queryBuilder_getRoot(sender),'se');if(end=='')
return 0;else
return end;}
function queryBuilder_setSelectionEnd(sender,selectionEnd)
{setAttributeValue(queryBuilder_getRoot(sender),'se',selectionEnd);}
function queryBuilder_onFieldClick(e)
{e=getEvent(e);var sender=getSender(e);if(sender.tagName!='LI')
sender=getParent(sender);var textBox=queryBuilder_getTextBox(sender);var text=getAttributeValue(sender,'arg');var selectionStart=queryBuilder_getSelectionStart(sender);var selectionEnd=queryBuilder_getSelectionEnd(sender);if(selectionStart==selectionEnd||!isIE)
{textBox.value+=' '+text;queryBuilder_setCaretPos(textBox,textBox.value.length-1);queryBuilder_updateSelection(e);}
else
{textBox.focus();var range=document.selection.createRange();range.moveStart('character',selectionStart);range.moveEnd('character',selectionEnd-selectionStart);range.text=text;queryBuilder_updateSelection(e);range.select();}
abortEvent(e);return false;}
function queryBuilder_onOperatorClick(e)
{e=getEvent(e);var sender=getSender(e);if(sender.tagName!='LI')
sender=getParent(sender);var textBox=queryBuilder_getTextBox(sender);var text=getAttributeValue(sender,'arg');var selectionStart=queryBuilder_getSelectionStart(sender);var selectionEnd=queryBuilder_getSelectionEnd(sender);textBox.focus();if(selectionStart==selectionEnd||!isIE)
{textBox.value+=' '+text;queryBuilder_setCaretPos(textBox,textBox.value.length-1);queryBuilder_updateSelection(e);}
else
{var range=document.selection.createRange();range.moveStart('character',selectionStart);range.moveEnd('character',selectionEnd-selectionStart);range.text=text;queryBuilder_updateSelection(e);range.select();}
abortEvent(e);return false;}
function queryBuilder_onWildcardClick(e)
{e=getEvent(e);var sender=getSender(e);if(sender.tagName!='LI')
sender=getParent(sender);var textBox=queryBuilder_getTextBox(sender);var text=getAttributeValue(sender,'arg');var selectionStart=queryBuilder_getSelectionStart(sender);var selectionEnd=queryBuilder_getSelectionEnd(sender);textBox.focus();if(!isIE)
{textBox.value+=text;queryBuilder_setCaretPos(textBox,textBox.value.length-1);queryBuilder_updateSelection(e);}
else if(selectionStart==selectionEnd)
{var range=document.selection.createRange();range.text=text;queryBuilder_updateSelection(e);range.select();}
else
{var range=document.selection.createRange();range.moveStart('character',selectionStart);range.moveEnd('character',selectionEnd-selectionStart);range.text=text;queryBuilder_updateSelection(e);range.select();}
abortEvent(e);return false;}
function queryBuilder_setCaretPos(textBox,position)
{if(textBox.selectionStart)
{textBox.selectionStart=position;textBox.selectionEnd=position;}
else
{var range=textBox.createTextRange();range.moveStart('character',position);range.collapse();range.select();}}
function queryBuilder_findSelectionStart(textBox)
{if(isIE)
{var range=document.selection.createRange();var isCollapsed=range.compareEndPoints("StartToEnd",range)==0;if(!isCollapsed)
range.collapse(true);var b=range.getBookmark();return b.charCodeAt(2)-2;}
else
return textBox.selectionStart;}
function queryBuilder_findSelectionEnd(textBox)
{if(isIE)
{var range=document.selection.createRange();var isCollapsed=range.compareEndPoints("StartToEnd",range)==0;if(!isCollapsed)
range.collapse(false);var b=range.getBookmark();return b.charCodeAt(2)-2;}
else
return textBox.selectionEnd;}
function queryBuilder_updateSelection(e)
{e=getEvent(e);var sender=getSender(e);var textBox=queryBuilder_getTextBox(sender)
if(document.activeElement==textBox||sender==textBox)
{queryBuilder_setSelectionStart(sender,queryBuilder_findSelectionStart(textBox));queryBuilder_setSelectionEnd(sender,queryBuilder_findSelectionEnd(textBox));}}
function queryBuilder_clear(e)
{e=getEvent(e);var sender=getSender(e);var textBox=queryBuilder_getTextBox(sender);textBox.value='';textBox.focus();}
function queryBuilder_close(e)
{e=getEvent(e);var sender=getSender(e);var root=queryBuilder_getRoot(sender);var div=getElementByKeyAndTagName(root,'contents','DIV');advancedTextBox_collapse(div);getElementByKeyAndTagName(root,'text','INPUT').focus();}
function queryBuilder_onMouseOver(e)
{e=getEvent(e);var sender=getSender(e);if(sender.tagName!='LI')
sender=getParent(sender);sender.className='hover';}
function queryBuilder_onMouseOut(e)
{e=getEvent(e);var sender=getSender(e);if(sender.tagName!='LI')
sender=getParent(sender);sender.className='';}
function queryBuilder_onKeyPress(e)
{e=getEvent(e);var sender=getSender(e);if(e.keyCode==27)
{abortEvent(e);queryBuilder_close(e);}
var key=String.fromCharCode(e.keyCode);if(key=='')
return false;key=key.toLowerCase();var needsAlt=false;var needsCtrl=false;var target='';if(e.altKey||e.ctrlKey)
{var links=queryBuilder_getRoot(sender).getElementsByTagName('LI');for(var k=0;k<links.length;k++)
{var hotkey=getAttributeValue(links[k],'hotkey');if(hotkey!='')
{needsAlt=false;needsCtrl=false;target='';var keys=hotkey.split('+');for(i=0;i<keys.length;i++)
{switch(keys[i].toLowerCase())
{case'alt':needsAlt=true;break;case'ctrl':needsCtrl=true;break;default:target=keys[i].toLowerCase();break;}}
if(needsAlt==e.altKey&&needsCtrl==e.ctrlKey&&key==target)
{links[k].childNodes[0].click();abortEvent(e);return true;}}}}}
