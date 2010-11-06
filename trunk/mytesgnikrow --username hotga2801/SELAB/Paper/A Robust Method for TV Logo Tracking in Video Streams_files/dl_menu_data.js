fixMozillaZIndex=true; //Fixes Z-Index problem  with Mozilla browsers but causes odd scrolling problem, toggle to see if it helps
_menuCloseDelay=500;
_menuOpenDelay=200;
_subOffsetTop=0;
_subOffsetLeft=0;
horizontalMenuDelay="1";

with(mainbarStyle=new mm_style()){
borderwidth=0;
bordercolor="#a5a6a5";
bgimage="/cms/Computer.org/Menus/DigitalLibraryImages/background.png";
fontfamily="arial, tahoma";
fontsize=12;
fontstyle="normal";
fontweight="bold";
offcolor="#4a4d4a";
overbgimage="/cms/Computer.org/Menus/DigitalLibraryImages/activebackground.png";
oncolor="#000000";
padding=7;
separatorimage="/cms/Computer.org/Menus/DigitalLibraryImages/separator.png";
separatorsize=3;
subimage="/cms/Computer.org/Menus/DigitalLibraryImages/arrow.gif";
subimagepadding=0;
imagealign="right";
imageposition="middle";
itemheight=16;
}


with(submenuStyle=new mm_style()){
bordercolor="#888888";
borderstyle="solid";
borderwidth=1;
fontfamily="arial, tahoma";
fontsize=11;
fontstyle="normal";
headerbgcolor="#f8f7f0";
headerborder=0;
headercolor="#ce6500";
offbgcolor="#f8f7f0";
offcolor="#000000";
onbgcolor="#f3f1e7";
onborder="1px solid #000000";
oncolor="#000000";
padding=0;
separatoralign="right";
separatorcolor="#555555";
separatorwidth="85%";
}


with(milonic=new menuname("Main Top Menu Bar")){
alwaysvisible=1;
top=216;
left='50%';
margin=0;
style=mainbarStyle;
orientation="horizontal";
menuwidth=1002;
menuheight=30;
menualign="left";

aI("image=/cms/Computer.org/Menus/DigitalLibraryImages/endcap.png;text=Digital Library Home;title=Homepage of the Digital Library;url=/portal/web/csdl/home;");
aI("text=Subscribe;title=Subscribe to the Digital Library;url=/portal/web/csdl/subscribe;");
aI("showmenu=Search;align=center;text=Search;url=http://search3.computer.org/search/advancedSearch.jsp;");
aI("showmenu=Resources;align=center;text=Resources;");
aI("text=Institutional and Library Resources;url=/portal/web/csdl/libsubscription");
aI("text=Newsletter;title=Digital Library Newsletter;url=/portal/web/csdl/subscribe/newsletter;");
aI("text=About the Digital Library;title=About the Digital Library;url=/portal/web/csdl/about;");
aI("image=/cms/Computer.org/Menus/DigitalLibraryImages/endcap.png;itemwidth=300;");
}


with(milonic=new menuname("Search")){
margin=7;
style=submenuStyle;

aI("text=Simple Search;url=http://search3.computer.org/search/simpleSearch.jsp;");
aI("text=Advanced Search;url=http://search3.computer.org/search/advancedSearch.jsp;");
aI("text=Author Search;url=http://search3.computer.org/search/authorSearch.jsp;");
aI("text=Proceedings Search;url=http://search3.computer.org/search/proceedingsSearch.jsp;");
aI("text=CS Store Search;url=http://www.computer.org/portal/web/store/shopping?p_p_id=ofbizintegration_WAR_pluginstore_INSTANCE_ZnHI&p_p_lifecycle=1&p_p_state=normal&p_p_mode=view&p_p_col_id=column-1&p_p_col_count=1&_ofbizintegration_WAR_pluginstore_INSTANCE_ZnHI_org.computer.ofbizurl=http%3A%2F%2Fstore.computer.org%2Fcsstore%2Fcontrol%2Fadvancedsearch;");
}

with(milonic=new menuname("Resources")){
margin=7;
style=submenuStyle;

aI("text=RSS Feeds;url=/portal/web/csdl/dlrss;");
aI("text=CSDL Newsletter;url=/portal/web/csdl/subscribe/newsletter;");
aI("text=Terms of Use;url=/portal/web/csdl/terms;");
aI("text=Peer Review Notice;url=/portal/web/csdl/peerreview;");
}

with(milonic=new menuname("LibResources")){
margin=7;
style=submenuStyle;

aI("text=OPAC;url=/portal/web/csdl/opac;");
aI("text=Library/Institutional Newsletter;url=/portal/web/csdl/subscribe/librarynewsletter;");
aI("text=Library/Institutional Subscriptions;url=/portal/web/csdl/libsubscription;");
}



drawMenus();

/*
   Milonic DHTML Menu Image Pre-loading Module preloadmenuimages.js version 1.3 June 27 2006
   This module is only compatible with the Milonic DHTML Menu version 5.16 or higher

   Copyright 2004 (c) Milonic Solutions Limited. All Rights Reserved.
   This is a commercial software product, please visit http://www.milonic.com/ for more information.
   
   USE: This module does not need any parameters but needs to be placed after the menus have been built
   
   Version 1.2 added ability to load images and give control back to the menu at the same time, the system is now more thread like
   
   SYNTAX: <script language="JavaScript1.2" type="text/javascript" src="preloadmenuimages.js"></script> 
*/

_mIms=new Array();
_mIms2=new Array();

function _mPLF(i)
{
	for(var x=0;x<_mIms.length;x++){
		if(i==_mIms[x])return
		x++
	}
	_mIms[_mIms.length]=i
}

function mmPLIs(i,n){
	
	if(n){	
		_mIms2[i]=new Image()
		if(_mIms[i])_mIms2[i].src=_mIms[i]
	}
	
	if(!_mIms2[i].complete){
		_StO("mmPLIs("+(i)+",0)",5)
		return
	}
	
	if(i<_mIms.length)_StO("mmPLIs("+(i+1)+",1)",20)
}

function mmenuPreLoadImages(){
	for(var x=0;x<_mi.length;x++){
		var I=_mi[x]
		if(I[24])_mPLF(I[24])
		if(I[29])_mPLF(I[29])
		if(I[32])_mPLF(I[32])
		if(I[46])_mPLF(I[46])
		if(I[47])_mPLF(I[47])
		if(I[48])_mPLF(I[48])
		if(I[56])_mPLF(I[56])
		if(I[69])_mPLF(I[69])
		if(I[71])_mPLF(I[71])
		if(I[73])_mPLF(I[73])
		if(I[79])_mPLF(I[79])
		if(I[82])_mPLF(I[82])
		if(I[83])_mPLF(I[83])
		if(I[85])_mPLF(I[85])
		if(I[88])_mPLF(I[88])
		if(I[90])_mPLF(I[90])
		if(I[91])_mPLF(I[91])
		if(I[92])_mPLF(I[92])
		
	}
	mmPLIs(0,1)
}

mmenuPreLoadImages()
