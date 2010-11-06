var ppLegal='PageFlap : Copyright Visual Steel, all rights reserved (http://visualsteel.com)'; // v5.5.6

var ppName='ddj_intel_100802';

function pp_init(){
	var ppVars='clickstream='+clickstream+'&ppAdClick='+ppAdClick+'&n='+ppAdd+'&w='+ppSite+'&legal='+escape(ppLegal);
	var ppObject='<style type="text/css"> #pp_l{position:absolute;top:0px;width:100px;height:75px;right:0px;z-index:999999;} </style><div id="pp_l"></div>';
	if(ppCkfs){
		ppObject=ppObject+'<script language=\"VBScript\"\>\n'+'On Error Resume Next\n'+'Sub pObj_FSCommand(ByVal command, ByVal args)\n'+'Call pp_fs(command, args)\n'+'End Sub\n'+'</script\>\n';
	}
	var ppCont='<object classid="clsid:d27cdb6e-ae6d-11cf-96b8-444553540000" codebase="http://fpdownload.macromedia.com/pub/shockwave/cabs/flash/swflash.cab#version=8,0,0,0" id="pObj" width="100%" height="100%"><param name="allowScriptAccess" value="always"><param name="movie" value="'+ppPath+ppName+'_pp.swf?keyLock='+ppLck+'"><param name="wmode" value="transparent"><param name="FlashVars" value="'+ppVars+'"><embed src="'+ppPath+ppName+'_pp.swf?keyLock='+ppLck+'" wmode="transparent" width="100%" height="100%" id="pObj" name="pObj" flashvars="'+ppVars+'" allowscriptaccess="always" type="application/x-shockwave-flash" pluginspage="http://www.macromedia.com/go/getflashplayer"></embed></object>';
	document.write(ppObject);
	if(ppCkfs){
		document.getElementById('pp_l').style.display='inline'
	}
	document.getElementById('pp_l').innerHTML=ppCont;
	if(ppImp){
		ppTag.src=ppImp
	}
}
function pp_call(ppArs){
	var ppAr=ppArs.split(',');
	if(ppCksaf){
		document.getElementById('pp_l').style.top="-1000px";
	}
	ppP1=eval(ppAr[0]);
	ppP2=eval(ppAr[1]);
	if(ppCkfs&&eval(ppAr[6]+'.length')){
		for(var ppFc=0;ppFc<eval(ppAr[6]+'.length');ppFc++){
			for(var ppSfcount=0;ppSfcount<eval(ppAr[6]+'[ppFc].length');ppSfcount++){
				if(eval(ppAr[6]+'[ppFc]'+ppAr[7]+'[ppSfcount]'+ppAr[8])){
					ppV1=eval(ppAr[6]+'[ppFc]'+ppAr[7]+'[ppSfcount]'+ppAr[2]+'ppAr[3]')
				}
			}
		}
	}
	if(ppCkwn&&!ppCkfs&&eval(ppAr[4]+'(ppAr[5]).length')){
		for(var ppIc=0;ppIc<eval(ppAr[4]+'(ppAr[5]).length');ppIc++){
			ppV2=eval(ppAr[4]+'(ppAr[5])[ppIc]'+ppAr[2]+'ppAr[3]')
		}
	}
	ppP1=null;ppP2=null;ppV1=null;ppV2=null;
	if(ppCksaf){
		pprdlTO=setTimeout(pp_rdl,1)
	}
}
function pp_fs(ppCOMV){
	var obj_pp=ppCkfs?document.all.pObj:document.pObj;
	ppCOM=eval(ppCOMV);
	ppCOM=null;
}
function pp_rdl(ppArs){
	document.getElementById('pp_l').style.top="0px"
}
function pp_ccreate(name,value,minutes){
	if(minutes){
		var date=new Date();
		date.setTime(date.getTime()+(minutes*60000));
		var expire="; expires="+date.toGMTString();
	}else{
		var expire='';
	}
	document.cookie=name+"="+value+expire+"; path=/; domain="+ppSite;
}
function pp_cread(name){
	var nameEQ=name+"=";
	var CAK=document.cookie.split(';');
	for(var ppFpcount=0;ppFpcount<CAK.length;ppFpcount++){
		var CK=CAK[ppFpcount];
		while(CK.charAt(0)==' ')CK=CK.substring(1,CK.length);
		if(CK.indexOf(nameEQ)==0) return CK.substring(nameEQ.length,CK.length);
	}
	return null;
}
function pp_TLink(ppArs){
	/*eval(ppArs)*/
}
if(!ppCount){
	var ppLck=Math.floor(Math.random()*10000000000);
	var ppCount=true;
	var ppTagLink=new Image(0,0);		
	var ppTag=new Image(0,0);
	var ppImp=false;
	var ppPath='http://i.cmpnet.com/ads/graphics/as5/pagepeel/';
	var ppUA=navigator.userAgent.toLowerCase();
	var ppAN=navigator.appName.toLowerCase();
	var ppAdClick;	
	var clickstream;
	var clickstream=escape(clickstream);
	var ppURL=(document.location.href.split('/'))[2].toLowerCase();
	if(ppURL.indexOf('www.')!=-1){
		var ppSite=ppURL.substring(4,ppURL.length)
	}else{
		var ppSite=ppURL
	}
	if(ppUA.indexOf('nd')!=-1){
		var ppCkwn=true
	}
	if(ppUA.indexOf('afa')!=-1&&!ppCkwn){
		var ppCksaf=true
	}else{
		var ppCksaf=false
	}	
	if(ppAN.indexOf('ro')!=-1&&!ppCkwn){
		var ppTerm=true
	}
	if(ppUA&&ppAN.indexOf('ro')!=-1&&ppCkwn&&ppUA.indexOf('3.1')==-1){
		var ppCkfs=true
	}
	var ppCcheck=pp_cread(ppName+'_pf');
	if(!ppCcheck){
		pp_ccreate(ppName+'_pf',ppLck,525600);
		var ppCcheck2=pp_cread(ppName+'_pf');
		if(ppCcheck2){
			var ppAdd=ppCcheck2
		}else{
			var ppAdd=ppLck
		}
	}else{
		var ppAdd=ppCcheck
	}		
	if(!ppTerm){
		pp_init()
	}
}