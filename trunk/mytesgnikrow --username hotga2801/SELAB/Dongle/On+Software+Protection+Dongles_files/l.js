var Domain='thefreelibrary.com';
var URLPref='http://www2.'+Domain+'/';
var Ads=[];var kw='';
var CurrentAd=0;
var IE=(navigator.appName=='Microsoft Internet Explorer' && navigator.userAgent.indexOf("Opera")==-1?1:0);
function dw(t){document.write(t)}
var loc=encodeURIComponent(document.location);
var dttl=encodeURIComponent(document.title);
function ById(id){return document.getElementById(id)}
var click_block=0;
function dictionary(){
if (click_block){
click_block=0;
return;
}
if (!IE)
	od(document.getSelection());
else{
	var t=document.selection.createRange();
	if(document.selection.type=='Text' && t.text>''){
		document.selection.empty();
		od(t.text);}
	}
function od(t){
while (t.substr(t.length-1,1)==' ')
	t=t.substr(0,t.length-1)
while (t.substr(0,1)==' ')
	t=t.substr(1)
if (t) window.open('http://www.thefreedictionary.com/'+encodeURIComponent(t), 'dict', 'width=750,height=550,resizable=1,menubar=1,scrollbars=1,status=1,titlebar=1,toolbar=1,location=1,personalbar=1');
}}
if (!document.all) {
	document.captureEvents(Event.MOUSEMOVE);
	document.onmousemove = get_mouse;
}
function get_mouse(e) {
	nsx = e.pageX - 10;
	nsy = e.pageY - 160;
}
function t_i(id){
	var hlp=ById('Tp'+id);
	if (hlp) {
		if (document.all) {
			nsy = event.y //+ document.body.scrollTop;
			nsx = event.x //+ document.body.scrollLeft;
		}
		hlp.style.top = nsy + 20;
		hlp.style.left = nsx -20;
		if (hlp.innerHTML.indexOf('Click the link for more information.')<0) hlp.innerHTML+='<br><b>.....</b> <span class=flw>Click the link for more information.</span>'
		hlp.style.display='block';
}}
function t_o(id){
	var hlp=ById('Tp'+id);
	if(hlp) hlp.style.display='none';
}
function m_over(url){
window.status=url;
return true;
}
function m_out(){
window.status='';
}
var g_kw=0;
function google_ad_request_done(google_ads){
	Ads=google_ads;
	if (Ads.length==0 && g_kw){
		g_kw=0;
		google_ad_width=468;
		google_ad_height=60;
		google_ad_format="468x60_as";
		google_ad_output="js";
		google_prev_ad_formats_by_region=null;
		google_encoding="utf-8";
		google_ad_channel=ad_channel;
		dw('<script language="JavaScript1.1" src="http://pagead2.googlesyndication.com/pagead/show_ads.js"></script>');
	}
}
function write_ad(ad){
	dw('<a href="'+ad.url+'" onMouseOver="return m_over(\'go to '+ad.visible_url+'\')" onMouseOut="m_out()">');
	dw('<span class=OvTitle>'+highlight(ad.line1)+'</span><br><span class=OvDescr>'+highlight(ad.line2+' '+ad.line3)+'</span><br><span class=OvURL>'+ad.visible_url+'</span></a>');
}
function write_ads(AdsNum, Br){
	if (Ads.length && Ads.length>CurrentAd){
	dw('<div class="Ov"><span class=by><a href=http://www.google.com/ads_by_google.html>Ads by Google</a></span>');
	for (i=0;CurrentAd<Ads.length && i<AdsNum;i++){
		if (i) dw('<div class=AdSep></div>');
		write_ad(Ads[CurrentAd]);
		CurrentAd+=1;
		}
	dw('</div>');
	if (Br) dw('<br>');
	}
}
function highlight(t) {
	var k = kw;
	if (k) k = k.replace(/[,.:;"()\[\]]/g, '');
	if (k && t > '') {
		var q = k.split(' ');
		for (var i = 0;i<q.length;i++)
			if (q[i].length>2) {
				var rexp = new RegExp(q[i],'ig');
				t = t.replace(rexp, '<b>'+q[i]+'</b>');
			}
}
return t;
}
function myerror(){
window.onerror=null;
return true;
}
function TestSB(){
if (window.external && ((typeof window.external.AddSearchProvider=="unknown") || (typeof window.external.AddSearchProvider=="function"))) return 1;
return 0;
}
function WriteSB(){
if (TestSB())
dw('<a href="#" title="Add The Free Library to your search bar" onclick="window.external.AddSearchProvider(\'http://www.thefreelibrary.com/_/static/open-search.xml\');return false"><img src=http://img.tfd.com/m/add2SB.gif border=0 align=absbottom style="width:26px;"></a>');
}
function getCookie(name){
	var dc=document.cookie;
	var prefix=name+'=';
	var s=dc.indexOf('; '+prefix);
	if (s==-1){
		s=dc.indexOf(prefix);
		if (s !=0) return null;
	}
	else
		s+=2;
	var e=dc.indexOf(';', s);
	if (e==-1) e=dc.length;
	return unescape(dc.substring(s+prefix.length, e));
}
function setCookie(name, value){
	document.cookie=name+'='+escape(value)+'; expires=Thu, 1 Jan 2015 05:00:00 UTC; path=/; domain='+Domain;
}
function changeTextSize(s){
	if (s){	
	var TD=Rules[0].style;
	TD.fontSize=s+'pt';
	setCookie('fontSize', s);
}}
function WF(s,l){
dw('<a title="Change Text Size" style="text-decoration:none;font-size:'+(s-2)+'pt" href="javascript:changeTextSize('+s+')">'+l+'&nbsp;</a>')
}
function FontSize(){
WF(8,'T');WF(10,'E');WF(11,'X');WF(13,'T');
changeTextSize(getCookie('fontSize'));
}
function showDatePopUp(i){
	var o;
	if (eval('typeof PopUp'+i)=='undefined'){
		eval('PopUp'+i+'=new Object()');
		o=eval('PopUp'+i);
		o.e=ById('datePopUp'+i);
		o.t=null;
	}
	else o=eval('PopUp'+i);
	if (o.t) clearTimeout(o.t);	
	o.e.style.visibility='visible';		
}
function outDatePopUp(i){
	var o=eval('PopUp'+i);
	if (o.t) clearTimeout(o.t);
	o.t=setTimeout('hideDatePopUp('+i+')', 150);
}
function hideDatePopUp(i){
	var o=eval('PopUp'+i);
	o.e.style.visibility='hidden';
}
function digg(c){return ''; '<area shape=rect coords="'+c+'" target=_blank href="http://digg.com/submit?phase=2&url='+loc+'&title='+dttl+'" title="Digg it!">'}
function deli(c){return '<area shape=rect coords="'+c+'" target=_blank href="http://del.icio.us/post?v=4;url='+loc+';title='+dttl+'" title="Bookmark on del.icio.us">'}
function yh(c){return '<area shape=rect coords="'+c+'" target=_blank href="http://myweb2.search.yahoo.com/myresults/bookmarklet?u='+loc+'&t='+dttl+'" title="Bookmark on Yahoo!">'}
function TopLine(){
FontSize();
dw('<map name=buttons>'+digg('0,0,13,14')+deli('16,0,28,14')+yh('31,0,44,14')+
'<area shape=rect coords="46,0,60,14" href="'+URLPref+'webmasters.aspx" title="For bloggers and webmasters">'+
'</map><img src=http://www.thefreelibrary.com/_/static/TFLbuttons.gif style="width:60px" align=absbottom usemap=#buttons>')
if (window.external && (typeof window.external.AddFavorite !="undefined" ))
	dw(' <a title="Add to favorites" href="javascript:window.external.AddFavorite(\'http://www.thefreelibrary.com/\',\'Free Online Library\')"><img src=http://img.tfd.com/m/favorites.gif align=absbottom></a>')
WriteSB();
}
function LinkToThis(){
var dttl=encodeURIComponent(document.title);
dw('<p><a href="javascript:LinkToThisShow()">Link to this page</a><br><textarea id=link2this cols=50 rows='+(4+Math.floor(kw.length*2/50))+' style="display:none" readonly=readonly>&lt;a href=&quot;'+document.location+'&quot;&gt;'+kw+'&lt;/a&gt;</textarea></p>');
}
function LinkToThisShow(){ById('link2this').style.display='inline';return;}
function SocialNetworks(){
dw('<map name=socialNetworks>'+digg('0,0,33,32')+
'<area shape=rect coords="34,0,67,32" target=_blank href="http://www.facebook.com/share.php?u='+loc+'" title="Add to Facebook">'+
deli('69,0,102,32')+
'<area shape=rect coords="106,0,136,32" target=_blank href="http://www.google.com/bookmarks/mark?op=add&bkmk='+loc+'&title='+dttl+'" title="Bookmark on Google History Bookmarks">'+
yh('138,0,174,32')+'<area shape=rect coords="177,0,211,32" href="http://reddit.com/submit?url='+loc+'&title='+dttl+'" title="Reddit">'+
'</map><br><b>Please bookmark with social media, your votes are noticed and appreciated:</b><br><br><IMG SRC="/_/static/social-networks.gif" width=211 height=32 usemap="#socialNetworks" border=0><br><br>');
}
function Translate(){
var i=location.href.indexOf('&u=http://');
if (i>-1){
	var url=decodeURIComponent(location.href.substring(i+3));
	if (url.indexOf('&')>0) url=url.substring(0, url.indexOf('&'));
	dw('<br><b><a href="'+url+'" target="_top">Back to English</a></b>');
	return;
}
function l(x,c,n){
	dw('<area shape=rect coords="'+x+'" target=_top href="http://www.google.com/translate?hl=en&ie=UTF8&langpair=en%7C'+c+'&u='+loc+'" title="'+n+'">');
}
dw('<map name=translate>');
l('0,0,17,11','fr','Francais');
l('20,0,37,11','de','Deutsch');
l('40,0,57,11','es','Espa&#241;ol');
l('60,0,77,11','it','Italiano');
l('80,0,97,11','pt','Portugu&#234;s');
l('0,14,17,25','zh-CN','Chinese');
l('20,14,37,25','ar','Arabic');
l('40,14,57,25','ru','Russian');
l('60,14,77,25','ja','Japanese');
l('80,14,97,25','ko','Korean');
dw('</map><img src="/_/static/translate.gif" style="width:97px;height:25px;margin-top:6px" usemap=#translate>')
}
var userRate=-1;
var curRating=0.0;
var totalVotes=0;
var artId=0;
var usrAuthenticated=0;
var usrSelf=false;
function SendRating(eImg){
    if (usrSelf) return false;
	var nurl;
	if (artId > 0){
		if (/*usrAuthenticated && */userRate==-1){
			userRate=getVote(eImg);
			nurl=URLPref+'_/Users/AddRating.aspx?artId='+artId+'&vote='+userRate;
			nurl+='&t='+Math.random();
			AsyncRequest(nurl);
		}/* else if (!usrAuthenticated){
			userRate=getVote(eImg);
			nurl=URLPref+ '_/Users/Registration.aspx?cmd=Rating&srcUrl='+escape(location.href)+'&arg0='+artId+'&arg1='+userRate;
			location=nurl;
		}*/
	}
	return false;
}
function onStarOver(eImg){
	if (userRate==-1 && !usrSelf){
		for (i=1; i<=5; i++){
			var rate_star=ById('rate_star_'+i);
			if (i <=getVote(eImg))	rate_star.src='_/static/FilledStar.png';
			else rate_star.src='_/static/EmptyStar.png';
		}
	}
}
function drwRating(){
	var cl=Math.ceil(curRating);
	var gl=Math.floor(curRating);
	for (i=1; i<=5; i++){
		var rate_star=ById('rate_star_'+i);
		if (i <=gl){
			if (userRate > 0 || usrSelf){
				rate_star.className='rating';
				rate_star.src='/_/static/SavedStar.png';
			} else{
				rate_star.className='usr_rating';
				rate_star.src='/_/static/FilledStar.png';
			}
		} else if (i==cl){
			if (userRate > 0 || usrSelf){
				rate_star.className='rating';
				rate_star.src='/_/static/SavedHalfStar.png';
			} else{
				rate_star.className='usr_rating';
				rate_star.src='/_/static/HalfStar.png';
			}
		} else{
			rate_star.src='_/static/EmptyStar.png';
			if (userRate > 0 || usrSelf) rate_star.className='rating';
			else rate_star.className='usr_rating';
		}
	}
	var htxt=curRating.toString()+' ['+totalVotes.toString()+' vote(s)]';
	if (!usrSelf)
		htxt+=(userRate==-1) ? ' You can rate this article by selecting 1 to 5 stars on the left.' : ' You have rated this article as '+userRate;
	else
		htxt+=' You can not rate your own article.'
	ById('rating_value').innerHTML=htxt;
}
function SetInitRating(rating, votes, usrVote){
	function w(a,b){dw('<img id="rate_star_'+a+'" class="usr_rating" src="/_/static/EmptyStar.png" alt="Vote: '+a+' star" title="'+b+'" onclick="SendRating(this);" onmouseover="onStarOver(this);" onmouseout="drwRating();">')};
	dw('<div class="rating">Reader&nbsp;ratings:&nbsp;');
	w(1,'Poor');w(2,'Below Average');w(3,'Average');w(4,'Good');w(5,'Excellent');
	dw('&nbsp;<span id="rating_value" class="rating" >0 [0 vote(s)]</span> </div>');
	SetRating(rating, votes, usrVote);
}
function SetRating(rating, votes, usrVote){
	curRating=rating; totalVotes=votes; userRate=usrVote;
	drwRating();
}
function getVote(elm){
	var vt=elm.id.substr(elm.id.length-1);
	return parseInt(vt);
}
function AsyncRequest(url){
	var scr=document.createElement('script');
	scr.type='text/javascript';
	scr.defer=true;
	scr.src=url;
	document.getElementsByTagName('head').item(0).appendChild(scr);
}
var abused_span_callback;
function abusedComment(e, comm_id){
	abused_span_callback=e;
	var sUrl=URLPref+'_/Users/Abuse.aspx?cmd=Abuse&commId='+comm_id;
	AsyncRequest(sUrl);
	return false;
}
var comm_done=false;
function onCommKey(evt){
	if (evt.keyCode==13){
		onCommSubmit();
		return true;
	} else	return false;
}
function onCommSubmit(){
	if (comm_done) return false;
	var comm=ById('comm_text');
	var rnd=ById('comm_rnd');
	var err=ById('comm_error');
	if (comm.value.length==0){err.innerHTML='Comment text is required.'; return false;}
	if (rnd && rnd.value.length==0){err.innerHTML='Human validation code is required.'; return false;}
	err.innerHTML='Please wait ...';
	if ( artId > 0 ){
		err.innerHTML='&nbsp;';
		var nurl=URLPref;
		if (usrAuthenticated){
			nurl+='_/Users/SubmitComment.aspx?artId='+artId+'&artTitle='+artTitle+'&title='+encodeURIComponent(ById('comm_title').value)+'&comment='+encodeURIComponent(comm.value)+'&usrRnd='+rnd.value;
			nurl+='&t='+Math.random();
			comm_done=true;
			AsyncRequest(nurl);
		} else{
			nurl+='_/Users/Registration.aspx?cmd=Comment&srcUrl='+encodeURIComponent(location.href)+'&arg0='+artId+'&arg1='+encodeURIComponent(ById('comm_title').value)+'&arg2='+encodeURIComponent(comm.value)+'&t='+Math.random();
			location=nurl;
		}
	}
	return false;
}
function clearControl(){
	comm_done=false;
	var comm_captcha=ById('comm_captcha');
	comm_captcha.src=URLPref+'/_/Users/image-rnd.aspx?len=4&' +Math.random();
	setTimeout('ById("comm_title").value="";ById("comm_text").value="";ById("comm_rnd").value=""',20);
}
function onLogin(){
	var ok=true;
	if (ById('userName').value.length==0){
		ById('err_userName').innerHTML='*';
		ok=false;
	}
	else ById('err_userName').innerHTML='&nbsp;';
	if (ById('userPwd').value.length==0){
		ById('err_userPwd').innerHTML='*';
		ok=false;
	} else
		ById('err_userPwd').innerHTML='&nbsp;';
	ById('srcUrl').value=document.URL;
	return ok;
}
function CacheBuster(c){
	var a=getCookie('SID2');
	if (a==null) a='';
	if (a!=c) location.reload();
}
function diggit(){
//dw('<div style="float:right"><a href="http://digg.com/submit?phase=2&url='+loc+'&title='+dttl+'"><img src="http://digg.com/img/badges/100x20-digg-button.gif" width="100" height="20" alt="Digg!"></a></div>')
}
// ==============================================================================
function toggle(id){
	var img=ById('toggle_img_'+id);
	var td=ById('toggle_td_'+id).style;
	if (td.display=='none'){
		td.display='';
		toggle_store(id,0);
		img.src=img.src.replace('right','down');
	}
	else {
		td.display='none';
		toggle_store(id,1);
		img.src=img.src.replace('down','right');
	}
}
function toggle_store(id, hide){
	var c='0'+getCookie('t2');
	if (hide)
		c=c|(1<<(id-1));
	else
		c=c & (~(1<<(id-1)));
	setCookie('t2',c);
}
function toggle_retrieve(id){
	var c=getCookie('t2');
	if (c) c=Number(c);
	else c=1;
	if (c&(1<<(id-1))) toggle(id);
}
function homepage(root){
var ua=navigator.userAgent;
if (IE && ua.indexOf("Win")!=-1/* && !eval('shp.is'+'Homepage(\'http://www.'+Domain+'/\')')*/){
	dw('<a href="/" title="Set as home page" onMouseOver="return m_over(\'set www.'+Domain+' as home page\')" onMouseOut="m_out()" onClick="style.b'+'ehavior=\'url(#'+'default#'+'homepage)\';setHome'+'Page(\'http://www.'+Domain+'/\');">');
	dw((root?'Set as Home Page</a> |':'<img src=http://img.tfd.com/m/home.gif align=absbottom></a>'));
	}
}



var _CLIENTTIMEOUTERROR="Problem loading data.";
var _ASK_SAVE_PARAMETER_DATA="You have unsaved data. Do you want to save it also?";
var _ASK_SAVE_PARAMETER_DATA_GLOBAL="You have unsaved data. Do you want to save it?";
var _ASK_DISCARD_PARAMETER_DATA="You have unsaved data in parameters. Do you want to discard your changes?"
////////////GENERAL/////////////
function getBrowserEvent(e){
	var evt=new Object();
    if(navigator.userAgent.indexOf('MSIE')>=0){
		evt.target=event.srcElement;
		evt.event=event;
	}
	else{
		evt.target=e.target;
		evt.event=e;
	}
	return evt;
}
///////FOCUS//////
function checkSubmitOnEnter(e){
	var evt=getBrowserEvent(e);
	var obj=evt.target;
	if(evt.event && obj){	
		var btnId=obj.getAttribute("CSsubmitControlId");
		if(btnId){
			var btn=ById(btnId);
			if(btn && (evt.event.keyCode==13)){
				if(obj.value !=null && obj.value.length>0)
					btn.click();
				return false;
			}
		}
	}
	return true;
}
function registerClickEvent(txtId, btnId){
	var txt=ById(txtId);
	var btn=ById(btnId);
	if(txt){
		txt.setAttribute("CSsubmitControlId", btnId)
		txt.onkeypress=new Function("e", "return checkSubmitOnEnter(e);");
	}	
}
////FOCUS///////
var CSChangedParameters='\t';
var CSParametersSaveButton=new Array();
function CSParameterChange(obj, propName, defValue){
	eval('var curValue=obj.'+propName+'.toString();');
	defValue=unescape(defValue)
	if(defValue !=curValue)
		CSChangedParameters=CSChangedParameters+obj.id+'\t';
	else
		CSChangedParameters.replace('\t'+obj.id+'\t', '\t');
}
function CSIsAnyUnsavedParams(sbmtid){
	if(CSChangedParameters !='\t'){
		var unsaved=CSChangedParameters.split('\t');
		for(var i=1; i<unsaved.length - 1; i++){	
			var sbid=CSParametersSaveButton[unsaved[i]];
			if(sbid && sbid !=sbmtid)
				return true
		}
	}
	return false;
}
function checkForUnsavedParameters(saveButtonId){
	if(CSChangedParameters !='\t')
	{
		if(saveButtonId){
			if(CSIsAnyUnsavedParams(saveButtonId))
				CSSaveAll(_ASK_SAVE_PARAMETER_DATA);
		}
		else
			CSSaveAll(_ASK_SAVE_PARAMETER_DATA_GLOBAL);
	}
	return true;
}
function CSSaveAll(msg){
	var res=true //confirm(msg);
	var hd=ById('saveAllParameters');
	hd.value=res ? "1" : "0";
	return res;
}
///////Parameters Saving/////////////
function ShowEditBlock(objid,id){
	var param=ById(id);
	var obj=ById(objid);
	if (param.style.display=='none'){
		param.style.display=''
		obj.innerHTML=obj.innerHTML.replace("edit", "close")
	}
	else{
		param.style.display='none'
		obj.innerHTML=obj.innerHTML.replace("close", "edit")
	}	
	var s="s";
}
///////////Customization Pane////////
function togglecp(cid, catId){
	var el=ById(cid+'_dvChild');
	if(el){
		var cp=el.style;
		var invis=(cp.display=='none');
		cp.display=invis ? '': 'none';
		var img=ById(cid+'_img')
		if (img)
			if (invis)
				img.src=img.src.replace('right', 'down')
			else
				img.src=img.src.replace('down', 'right')
		var str=getCookie("ts");
		if(!str)str="0";
		str=updateHEXBit(str, catId, invis);
		setCookie("ts", str);
		var Load=el.getAttribute('Load');
		LoadMenu(el);
	}
}
function LoadMenu(el){
	var Load=el.getAttribute('Load');
	if (Load)
		if (el.getAttribute('DataLoaded')=='0'){
			ById('CA_dataTransfer').src='/_/hp/Controls/AsyncMenu.aspx?CID='+el.id+'&Menu='+Load;
			setTimeout("_TFAIL('"+el.id+"')", 5000);
		}
}
function addBlock(obj,id){
	if(!CSIsAnyUnsavedParams(null) || confirm(_ASK_DISCARD_PARAMETER_DATA)){
		var hnb=getServerElement('hnb');
		hnb.value=obj+(id?'|'+id:'');
		submitForm(obj);	
	}
	return false;
}
function addCustomRSSBlock(){
	if(!CSIsAnyUnsavedParams(null) || confirm(_ASK_DISCARD_PARAMETER_DATA)){
		var hdCust=getServerElement('hdCustomRSS');
		hdCust.value="1";
		submitForm(hdCust);	
	}
}
//////////Customization Pane//////
function updateHEXBit(str, n, value){
	var hexN=parseInt(n / 4);
	var pHex=n % 4;
	while(str.length<=hexN)
		str="0"+str;
	var strInd=str.length - hexN - 1;
	var hexPart=str.substr(strInd, 1);
	var st=parseInt(hexPart, 16);
	st=value ? (st | (1<<pHex)) : (st & (~(1<<pHex)));
	str=str.substr(0, strInd)+st.toString(16)+str.substr(strInd+1);
	while(str.length>1 && str.substr(0, 1)=="0")
		str=str.substr(1);
	return str;
}
////////ASYNC LOADING////////
function _CP(destId, sourceID){
	var dest=ById(destId);
	var source=ById(sourceID);
	if(source && dest){
		dest.setAttribute("DataLoaded", "1");
		dest.innerHTML=source.innerHTML;
	}
}
function _CPD(destId, data){
	var dest=ById(destId);
	if(dest){
		dest.setAttribute("DataLoaded", "1");
		dest.innerHTML=data;
	}
}
function _TFAIL(id){
	var ctrl=ById(id);
	if(ctrl){	
		var attr=ctrl.getAttribute("DataLoaded");
		if(attr !="1")
			ctrl.innerHTML=_CLIENTTIMEOUTERROR
	}
}


///////Customization Usage///////
function _CUP(val, maxval){
	var ctrl=ById("CustUsedText");
	var pcnt=parseInt(val * 100 / maxval).toString();
	if(ctrl && maxval) ctrl.innerHTML='Customization Used '+pcnt+'%';
	ById('CustUsedSlider').style.width=parseInt(val * 163 / maxval + 2)+'px';
	var ectrl=ById("currentCustomizationErrorMsg");
	var isError=(val<0 || val>maxval);
	if(ectrl){ectrl.style.display=isError ? "inline" : "none";}
}
function _CUSE(msg){
	var ctrl=ById("currentCustomizationErrorMsg");
	if(ctrl)ctrl.innerHTML=msg;
}

// drag drop.js
var dragEvent = null;
var isMacIE5 = (navigator.platform.indexOf ("Win32") == -1 && navigator.appName == "Microsoft Internet Explorer")?1:0;
function DADSaveConfig(de){
	if(dragEvent == null || !dragEvent.control.Dragged)
		return;
	var strdata = "";
	var colIndex = 0;
	for(var i in dragEvent.columns){
		var col = dragEvent.columns[i];
		var rowIndex = 0;
		if(col != null)
			for(var j = 0; j < col.childNodes.length; j++){
				var el = col.childNodes[j];
				if(dragEvent.control.isSupported(el)){
					var acid=el.getAttribute("ACID")
					strdata+=acid+"="+colIndex.toString()+rowIndex.toString()+";";
					rowIndex++;
				}
			}
		colIndex ++;
	}
	var frame = _getE(dragEvent.Params["frame"])
	if(frame)
		frame.src = dragEvent.Params["path"] + "/UpdateClientConfiguration.aspx?data=" + escape(strdata);;
}
//////////////////////////////////////////////////////////////////////////////////
function DADInitControls(cols, headers, path, frameID){
	dragEvent = new DragMouseEvent(headers);
	dragEvent.Params["path"] = path;
	dragEvent.Params["frame"] = frameID;
	dragEvent.OnFinish = new Function("de", "DADSaveConfig(de)");  	
	var i;
	for(i in cols)
		dragEvent.columns[cols[i]] = _getE(cols[i]);
	for(i in headers){
		var id = headers[i];
		var hdr_obj = document.getElementById(id);		
		var dc_id = hdr_obj.getAttribute('DC');		
		dragEvent.blocks[dc_id] = _getE(dc_id);
		hdr_obj.style.cursor="move";
		hdr_obj.onmousedown = new Function("e", "return dragEvent.Start(e);");  	
	}
}
function _offsetE(a, b){
	var c = 0;
	while(a != null){
		c += a["offset"+(b?"Left":"Top")];
		a = a.offsetParent
	}
	return c;
}

function _getE(id){return document.getElementById(id);}
function _getX(obj){
	var x = obj.offsetLeft + obj.parentNode.offsetLeft;
	return x;
}
function _getY(obj){
	var y = obj.offsetTop + obj.parentNode.offsetTop;
	return y;
}
function DragPoint(col, row){
	this.Col = col;
	this.Row = row;
}
function DragControl(e){
	this.dragNodeType = 'DIV';
	this.dragEvent = e;
	this.mc = null;
	this.dragContainer = e.getDragContainer();
	this.dx = 0;
	this.dy = 0;
	this.mcPos = null;
}
DragControl.prototype={
	"getCoordinates":function(obj){
		var colIndex = 0;
		for(var i in this.dragEvent.columns){
			var col = this.dragEvent.columns[i];
			var rowIndex = 0;
			if(col != null)
				for(var j = 0; j < col.childNodes.length; j++){
					var el = col.childNodes[j];
					if(el == obj)
						return new DragPoint(colIndex, rowIndex);
					else
					if(this.isSupported(el))
						rowIndex ++;
				}
			colIndex ++;
		}
		return null;
	},
	"PrepareDrag":function(){
		var obj = this.dragEvent.target;
		var e = this.dragEvent.event;
		this.mc = this.getContainer(obj);
		this.mcPos = this.getCoordinates(this.mc);
		var dc = this.dragContainer 
		this.dx = e.clientX - _offsetE(this.mc, true);
		this.dy = e.clientY - _offsetE(this.mc, false);
		dc.innerHTML = this.mc.innerHTML;
		dc.className = this.mc.className;
		dc.backgroundColor = this.mc.backgroundColor
		var cc = e.clientX;
		dc.style.left =  _offsetE(this.mc, true);
		dc.style.top =   _offsetE(this.mc, false);		
		dc.style.height = this.mc.offsetHeight;
		dc.style.width = this.mc.offsetWidth;
		if(navigator.platform == "Win32" || navigator.userAgent.indexOf('Gecko') >= 0){
			dc.style.filter="alpha(Opacity=75)"
			dc.style.opacity=0.75;	
		}
	
	},
	"Drag":function(applyDrag){
		var obj = this.dragEvent.target;
		var e = this.dragEvent.event;
		var dc = this.dragContainer;
		dc.style.left = e.clientX - this.dx;
		dc.style.top = e.clientY - this.dy;
		if(applyDrag)this.DragOver();
	},
	"DragOver":function(){	
		var dc = this.dragContainer;
		var e = this.dragEvent.event;
		var mode ;
		var neighbour;
		var colparent;
		if(e.clientX == this.cacheX && e.clientY == this.cacheY)
			return;
		else{
			this.cacheX = e.clientX;
			this.cacheY = e.clientY;
		}
		var canAppend = false;		
		var l1 = dc.offsetLeft;
		var t1 = dc.offsetTop;
		var l2 = _offsetE(dc,true);
		var t2 = _offsetE(dc,false);		
		var minx = 1000000;
		var miny = 1000000;
		var nearestCol = null;
		for(var i in this.dragEvent.columns){
			var col = this.dragEvent.columns[i];
			if(col != null){
				var xx = l1-_offsetE(col,true);
				if(xx < 0) xx = -xx;
				if(xx<minx){
					minx = xx;
					nearestCol = col;
				}
			}
		}
		if(nearestCol != null){
				for(var j = 0; j < nearestCol.childNodes.length; j++){
					var el = nearestCol.childNodes[j];
					if(!this.isSupportedSimple(el))
						continue;
					var yy = t1-_offsetE(el,false);
					if(yy < 0) yy = -yy;
					if(yy<miny){
						miny=yy;
						neighbour=el;
					}
				}
		}	
		if(neighbour!=null  && (this.mc.nextSibling!=neighbour && this.mc!=neighbour))
			neighbour.parentNode.insertBefore(this.mc, neighbour);
	},
	"Contains": function(parent, child){
		var i = 0;
		for(i = 0; i < parent.childNodes.length; i++)
			if(parent.childNodes[i] == child)
				return true;
		return false;
	},
	"EndDrag":function(){
		var mcPos = this.getCoordinates(this.mc);
		this.Dragged = false;
		if(this.mcPos != null && mcPos != null)
			this.Dragged = this.mcPos.Col != mcPos.Col || this.mcPos.Row != mcPos.Row;
		this.mc = null;
		this.dragContainer.innerHTML = "";
		this.dragContainer.style.height = 0;
		this.dragContainer.style.width = 0;
	},
	"getContainer":function(obj){
		var headers = this.dragEvent.headers;
		var tmpObj = obj;
		do{
			for(var i = 0; i < headers.length; i++)
				if(tmpObj.id == headers[i]){
					var pid = tmpObj.getAttribute('DC')
					return _getE(pid);
				}
			tmpObj = tmpObj.parentNode;
		}
		while(tmpObj.parentNode);
		return null;
	},
	"isSupported":function(obj){	
		return obj.nodeName == this.dragNodeType && this.dragEvent.blocks[obj.id] == obj;
	},
	"isSupportedSimple":function(obj){	
		return obj.nodeName == this.dragNodeType;
	}		
}
//////////////////////////EVENT////////////////////////////////////////////////////////
function DragMouseEvent(headers){
	this.headers = headers;
	this.columns = new Array();
	this.blocks = new Array();
	this.DC = null;
	this.Params = new Array();
}
DragMouseEvent.prototype={
	"getDragContainer":function (){
		if(this.DC == null){
			var n = document.createElement("DIV");
			n.style.position="absolute";
			n.style.cursor="move";
			n.style.backgroundColor="#ffffff";
			n.style.paddingBottom="0px";
			n.style.left="0px";
			n.style.top="0px";
			document.body.appendChild(n)
			this.DC = n;		
		}
		return this.DC;
	},
	"Init":function (e){
		var evt = getBrowserEvent(e);
		this.target = evt.target;
		this.event = evt.event; 
	},
	"Start" : function(e){
		this.Init(e);
		this.oldMouseMove = document.onmousemove;		
		this.oldMouseUp = document.onmouseup;
		this.control = new DragControl(this);
		this.control.PrepareDrag();
		document.onmousemove = new Function("e", "return dragEvent.Move(e);");  	
		document.onmouseup = new Function("e", "return dragEvent.End(e);");  
		return false;
	},
	"Move" : function(e){
		this.Init(e);
		this.control.Drag(!isMacIE5);
		if(document.selection)document.selection.empty();
		return false;
	},
	"End" : function(e){
		document.onmousemove = this.oldMouseMove 	
		document.onmouseup = this.oldMouseUp
		if(isMacIE5)this.control.Drag(true);
		this.control.EndDrag();
		if(typeof(this.OnFinish) == "function")
			this.OnFinish(this);
		this.control = null;
		this.event = null;
		this.target = null;
		if(document.selection)document.selection.empty();
		return false;
	}
}

//bookmarks.js
var ccbContentContainer=null;
var ccbTemplateContainer=null;
var ccbHiddenForSerialize=null;
var ccbBookmarksArray=new Array();

function cbHtmlEncode(a){var b=a;return b.replace(/</g,"&lt;").replace(/>/g,"&gt;");}

function cbAddClick(nid, uid){
	var nm=ById(nid);
	var url=ById(uid);
	if(nm && url){
		if(nm.value.length==0) nm.value=url.value;
		if(ccbBookmarksArray.push)
			ccbBookmarksArray.push(new Array(cbHtmlEncode(nm.value), cbHtmlEncode(url.value)));
		else
			ccbBookmarksArray[ccbBookmarksArray.length]=new Array(cbHtmlEncode(nm.value), cbHtmlEncode(url.value));
		nm.value=""
		url.value="http://"
		cbUpdateContent(false);
	}
}
function cbDeleteClick(index){
	if(confirm('Delete this bookmark?')){
		if(ccbBookmarksArray.splice)
			ccbBookmarksArray.splice(index, 1);
		else	{
			var tmpArray=new Array();
			var j=0;
			for(var i=0; i<ccbBookmarksArray.length; i++)
				if(i !=index) tmpArray[j++]=ccbBookmarksArray[i];
			ccbBookmarksArray=tmpArray;
		}
		cbUpdateContent(false);
	}
}

function cbInitalize(cid, tid, hid, arr){
	ccbContentContainer=ById(cid);
	ccbTemplateContainer=ById(tid);
	ccbHiddenForSerialize=ById(hid);
	ccbBookmarksArray=arr;
	cbUpdateContent(true);
}
function cbUpdateContent(first){
	//Clear Old items
	var content="";
	var hValue="";
	var tmplt=ccbTemplateContainer.innerHTML;
	for(var i=0; i<ccbBookmarksArray.length; i++){
		var name=ccbBookmarksArray[i][0];
		var url=ccbBookmarksArray[i][1];
		var itemTmplt=tmplt;		
		itemTmplt=itemTmplt.replace("__name__", name);
		itemTmplt=itemTmplt.replace("__url__", url);
		itemTmplt=itemTmplt.replace("__click__", "cbDeleteClick("+i+");return false;");
		//Seriazliing
		hValue+="<"+name+">"+url;
		content+=itemTmplt;		
	}	
	if(first) ccbHiddenForSerialize.setAttribute("defValue", escape(hValue));
	ccbHiddenForSerialize.value=hValue;
	if(!first){
		var defValue=ccbHiddenForSerialize.getAttribute("defValue");
		CSParameterChange(ccbHiddenForSerialize, 'value', defValue);
	}	
	ccbContentContainer.innerHTML=content;
}
function DisplayBookmarks(){
	for(var i=0; i<cbArr.length; i++){
		var name=cbArr[i][0];
		var url=cbArr[i][1];
		dw('<a href="'+url+'">'+name+'</a><br>')
	}
}
function BGColorPal(id) {
var colors=['White','#F8F8F8','#F0F0F0','#FEF2F2','#FFF6F1','#FFFBF1','#FEFFF1','#F9FFF1','#F4FFF1','#F1FFF3','#F1FFF8','#F1FFFD','#F1FDFF','#F1F8FF','#F1F3FF','#F4F1FF','#F9F1FF','#FEF1FF','#FFF1FB','#FFF1F6'];
dw('<table class=BGColorPal border=1 cellpadding=0 cellspacing=0 style="float:left"><tr>');
for (var i=0;i<colors.length;i++){
	if (i==colors.length/2) dw('</tr><tr>');
	dw('<td bgcolor="'+colors[i]+'" onclick="ById(\''+id+'\').value=\''+colors[i]+'\';ById(\''+id+'\').style.backgroundColor=\''+colors[i]+'\'">&nbsp;</td>');
}
dw('</tr></table>');
}
function ExtLink(u){
open(u);
return ;
}
function rsslink(l,f){
switch(f){
case 'pub':
dw('<a href="'+l+'" target="_blank"><img class="rss_small" alt="Rss Feed" src="/_/static/feed-icon-10x10.png"></a>');
break;
case 'subcat':
dw('<a href="'+l+'"  target="_blank"><img class="rss_big" alt="Rss Feed" src="/_/static/feed-icon-16x16.png"></a>');
break;default:	dw('<a href="' + l + '" target="_blank"><img class="rss_small" align=right alt="Rss Feed" src="/_/static/feed-icon-10x10.png"></a>');	if (l.indexOf('RssFeeds.aspx?type=category') > 0) addSlice(l.replace('RssFeeds.aspx', 'slice.aspx'));
}}
function addSlice(l) {
	if (IE) {
		var ua = navigator.userAgent;
		var re = new RegExp("MSIE ([0-9]{1,}[\.0-9]{0,})");
		if (re.exec(ua) != null) {
			var rv = parseFloat(RegExp.$1);
			if (rv < 8) return
		}
		else return;
		var n = decodeURIComponent(l.substring(40).replace(/[+]/g, ' '));
		dw('<a target=_blank href="#" onclick="javascript:window.external.AddToFavoritesBar(\'http://' + document.location.host + l + '#SLICE\', \'' + n.replace("'", "\\'") + '\', \'slice\');return false" title="Add ' + n + ' slice"><img class="rss_small" align=right alt="Add ' + n + ' slice" src="/_/static/slice-icon-10x10.png"></a>')
	} 
}