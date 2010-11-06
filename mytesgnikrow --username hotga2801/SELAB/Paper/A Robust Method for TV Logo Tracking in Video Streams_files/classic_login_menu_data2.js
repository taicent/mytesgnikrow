fixMozillaZIndex=true; //Fixes Z-Index problem  with Mozilla browsers but causes odd scrolling problem, toggle to see if it helps
_menuCloseDelay=500;
_menuOpenDelay=150;
_subOffsetTop=0;
_subOffsetLeft=0;

horizontalMenuDelay="1";


with(mainbarStyle=new mm_style()){
borderwidth=0;
bgimage="/cms/Computer.org/Menus/ClassicImages/background.png";
fontfamily="arial, tahoma";
fontsize="12px";
fontstyle="normal";
fontweight="bold";
offcolor="#ffffff";
overbgimage="/cms/Computer.org/Menus/ClassicImages/activebackground.png";
oncolor="#ffffff";
padding=7;
separatorimage="/cms/Computer.org/Menus/ClassicImages/separator.png";
separatorsize=1;
subimage="/cms/Computer.org/Menus/ClassicImages/arrow.gif";
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
fontsize="12px";
fontstyle="normal";
headerbgcolor="#f8f7f0";
headerborder=0;
headercolor="#ce6500";
offbgcolor="#f8f7f0";
offcolor="#000000";
onbgcolor="#ffffff";
//onbgcolor="#f3f1e7";
onborder="none";
//onborder="1px solid #000000";
ondecoration="underline";
oncolor="#000000";
padding="2px";
itemheight="20px";
separatoralign="right";
separatorcolor="#555555";
separatorwidth="85%";
orientation="vertical";
subimage="/cms/Computer.org/Menus/ClassicImages/right_black_menu_arrow.gif";
}


with(milonic=new menuname("Main Top Menu Bar")){
alwaysvisible=1;
top=69;
left='50%';
margin=0;
style=mainbarStyle;
orientation="horizontal";
menuwidth=1002;
menuheight=30;
menualign="left";


aI("text=Home;url=/portal/web/guest/home;");
aI("text=Join;title=Join the Computer Society;url=/portal/web/membership/join;");

aI("showmenu=Publications;text=Publications;url=/portal/web/publications");
aI("showmenu=Conferences;text=Conferences;url=/portal/web/conferences/home");
aI("showmenu=Digital Library;align=center;text=Digital Library;url=/portal/web/csdl");
aI("showmenu=Build Your Career;align=center;text=Build Your Career;url=/portal/web/buildyourcareer/home");
aI("showmenu=elearning;align=center;text=e-Learning;url=/portal/web/e-learning/home");
aI("showmenu=Certification;text=Certification &amp; Training;url=/portal/web/getcertified");
aI("showmenu=Communities;text=Communities;");
aI("text=Store;title=Store;url=/store;");

aI("text=&nbsp;&nbsp;Login&nbsp;&nbsp;&nbsp;;title=login with web account;url=/portal/c/portal/login;");



}


with(milonic=new menuname("Publications")){
margin=7;
style=submenuStyle;

aI("text=Publications Home;url=/portal/web/publications;");
aI("showmenu=Publications_CN;text=Computing Now;url=/portal/web/computingnow");
aI("showmenu=Publications_Magazines;text=Magazines;url=/portal/web/publications/magazines");
aI("showmenu=Publications_Transactions;text=Transactions;url=/portal/web/publications/journals");
aI("text=Letters;url=http://www.computer.org/cal;");
aI("text=Conference Publications;url=http://www.computer.org/csdl;");
aI("showmenu=Publications_CS_Press;text=CS Press;url=/portal/web/cspress/home");
aI("text=Pubs Blog;url=/portal/web/publications/out-of-print;");
}

with(milonic=new menuname("Publications_CN")){
margin=7;
style=submenuStyle;

aI("text=Computing Now Home;url=/portal/web/computingnow/home;");
aI("text=Archive;url=/portal/web/computingnow/articles;");
aI("showmenu=CN Departments;text=Departments;url=/portal/web/computingnow/news;");
aI("text=Survey;url=/portal/web/computingnow/survey;");
aI("text=Newsletter;url=/portal/web/computingnow/newsletter;");
aI("text=Subscribe;url=/portal/web/computingnow/subscribe;target='blank';");
aI("text=About Computing Now;url=/portal/web/computingnow/about;");
}

with(milonic=new menuname("CN Departments")){
margin=7;
style=submenuStyle;

aI("text=Book Reviews;url=/portal/web/cnbooks;");
aI("text=News;url=/portal/web/computingnow/news;");
aI("text=Computing Then;url=/portal/web/computingthen;");
aI("text=Education;url=/portal/web/computingnow/education;");
}

with(milonic=new menuname("Publications_Magazines")){
margin=7;
style=submenuStyle;

aI("text=Computer Magazine;title=Computer Magazine;url=http://www.computer.org/computer;");
aI("text=Computing in Science &amp; Engineering;title=Computing in Science &amp; Engineering;url=http://www.computer.org/cise;");
aI("text=IEEE Annals of the History of Computing;title=IEEE Annals of the History of Computing;url=http://www.computer.org/annals;");
aI("text=IEEE Computer Graphics and Applications;title=IEEE Computer Graphics and Applications;url=http://www.computer.org/cga;");
aI("text=IEEE Concurrency (out of print);title=IEEE Concurrency (out of print);url=/portal/web/csdl/magazines/concurrency#1;");
aI("text=IEEE Design &amp; Test of Computers;title=IEEE Design &amp; Test of Computers;url=http://www.computer.org/dt;");
aI("text=IEEE Distributed Systems Online (out of print);title=IEEE Distributed Systems Online (out of print);url=http://www.computer.org/dsonline;");
aI("text=IEEE Intelligent Systems;title=IEEE Intelligent Systems;url=http://www.computer.org/intelligent;");
aI("text=IEEE Internet Computing;title=IEEE Internet Computing;url=http://www.computer.org/internet;");
aI("text=IEEE Micro;title=IEEE Micro;url=http://www.computer.org/micro;");
aI("text=IEEE MultiMedia;title=IEEE MultiMedia;url=http://www.computer.org/multimedia;");
aI("text=IEEE Pervasive Computing;title=IEEE Pervasive Computing;url=http://www.computer.org/pervasive;");
aI("text=IEEE Security &amp; Privacy;title=IEEE Security &amp; Privacy;url=http://www.computer.org/security;");
aI("text=IEEE Software;title=IEEE Software;url=http://www.computer.org/software;");
aI("text=IT Professional;title=IT Professional;url=http://www.computer.org/itpro;");
}

with(milonic=new menuname("Publications_Transactions")){
margin=7;
style=submenuStyle;

aI("text=IEEE/ACM Transactions on Computational Biology and Bioinformatics;title=IEEE/ACM Transactions on Computational Biology and Bioinformatics;url=http://www.computer.org/tcbb;");
aI("text=IEEE Transactions on Affective Computing;title=IEEE Transactions on Affective Computing;url=http://www.computer.org/tac;");
aI("text=IEEE Transactions on Computers;title=IEEE Transactions on Computers;url=http://www.computer.org/tc;");
aI("text=IEEE Transactions on Dependable and Secure Computing;title=IEEE Transactions on Dependable and Secure Computing;url=http://www.computer.org/tdsc;");
aI("text=IEEE Transactions on Haptics;title=IEEE Transactions on Haptics;url=http://www.computer.org/th;");
aI("text=IEEE Transactions on Knowledge &amp; Data Engineering;title=IEEE Transactions on Knowledge &amp; Data Engineering;url=http://www.computer.org/tkde;");
aI("text=IEEE Transactions on Learning Technologies;title=IEEE Transactions on Learning Technologies;url=http://www.computer.org/tlt;");
aI("text=IEEE Transactions on Mobile Computing;title=IEEE Transactions on Mobile Computing;url=http://www.computer.org/tmc;");
aI("text=IEEE Transactions on Parallel &amp; Distributed Systems;title=IEEE Transactions on Parallel &amp; Distributed Systems;url=http://www.computer.org/tpds;");
aI("text=IEEE Transactions on Pattern Analysis &amp; Machine Intelligence;title=IEEE Transactions on Pattern Analysis &amp; Machine Intelligence;url=http://www.computer.org/tpami;");
aI("text=IEEE Transactions on Services Computing;title=IEEE Transactions on Services Computing;url=http://www.computer.org/tsc;");
aI("text=IEEE Transactions on Software Engineering;title=IEEE Transactions on Software Engineering;url=http://www.computer.org/tse;");
aI("text=IEEE Transactions on Visualization &amp; Computer Graphics;title=IEEE Transactions on Visualization &amp; Computer Graphics;url=http://www.computer.org/tvcg;");
}

with(milonic=new menuname("Publications_CS_Press")){
margin=7;
style=submenuStyle;

aI("text=CS Press Home;title=CS Press Home;url=/portal/web/cspress/home;");
aI("text=Books;title=CS Press Books;url=/portal/web/cspress/books;");
aI("text=ReadyNotes;title=ReadyNotes;url=/portal/web/readynotes;");
aI("text=Essential Sets;title=TechSets;url=/portal/web/buildyourcareer/techsets;");
}

with(milonic=new menuname("Conferences")){
margin=7;
style=submenuStyle;

aI("text=Conferences Home;title=Conferences Home;url=/portal/web/conferences/home;");
aI("text=Conference Calendar;title=Conference Calendar;url=/portal/web/conferences/calendar;");
aI("text=Conference Organizer Resources;title=Conference Organizer Resources;url=/portal/web/conferences/organizer-tools;");
aI("text=Conference Publishing Services;title=Conference Publishing Services;url=/portal/web/cscps/home;");
aI("text=Conference Publications;title=Conference Publications;url=/portal/web/csdl;");
aI("text=Call for Papers / Submission Instructions;title=Call for Papers / Submission Instructions;url=/portal/web/conferences/cfps;");
aI("text=Search IEEE Conferences;title=Search IEEE Conferences;url=http://www.ieee.org/web/conferences/search/index.html;");
}

with(milonic=new menuname("Digital Library")){
margin=7;
style=submenuStyle;

aI("text=Digital Library Home;url=/portal/web/csdl;");
aI("text=Search;url=http://search3.computer.org/search/advancedSearch.jsp;");
aI("text=Subscribe;url=/portal/web/csdl/subscribe;");
aI("text=About the Digital Library;url=/portal/web/csdl/about;");
}



with(milonic=new menuname("Build Your Career")){
margin=7;
style=submenuStyle;

aI("text=Build Your Career Home;url=/portal/web/buildyourcareer/home;");
aI("text=Career Watch;url=/portal/web/buildyourcareer/careerwatch;");
aI("text=CS Jobs Board;url=/portal/web/careers;");
aI("text=Career News;url=/portal/web/buildyourcareer/news;");
aI("text=Tutorials;url=/portal/web/buildyourcareer/techsets;");
aI("text=Free Articles;url=/portal/web/buildyourcareer/articles;");
aI("text=Special Offers;url=/portal/web/buildyourcareer/offers;");
aI("text=About Build Your Career;url=/portal/web/buildyourcareer/about;");
}


with(milonic=new menuname("elearning")){
margin=7;
style=submenuStyle;

aI("text=e-Learning Campus;url=/portal/web/e-learning/home;");
aI("text=Course Catalog;url=/portal/web/e-learning/ekcourses;");
aI("text=Online Books;url=/portal/web/e-learning/ekbooks;");
aI("text=Safari Books Online;url=/portal/web/membership/safari;");
aI("text=Books 24x7;url=/portal/web/e-learning/books24x7;");
aI("text=Certification;url=/portal/web/e-learning/brainbench;");
}


with(milonic=new menuname("Certification")){
margin=7;
style=submenuStyle;
aI("text=About Certification;title=About Certification;url=/portal/web/getcertified;");
aI("text=CSDA Home;title=CSDA Home;url=/portal/web/csda;");
aI("text=CSDP Home;title=CSDP Home;url=/portal/web/certification;");
aI("text=Success Stories;title=Success Stories;url=http://www.computer.org/certification/successstories;");
aI("text=Certification News;title=Certification News;url=/portal/web/getcertified/news;");
aI("text=About SWEBOK;title=About SWEBOK;url=/portal/web/swebok;");
}


with(milonic=new menuname("Communities")){
margin=7;
style=submenuStyle;


aI("text=Chapter Activities;url=/portal/web/chapters;");
aI("text=Educational Activities;url=/portal/web/education;");
aI("text=Professional Activities;url=/portal/web/pab;");
aI("showmenu=Communities_StandardsActivities;text=Standards Activities&nbsp;url=/portal/web/standards/home");
aI("showmenu=Communities_Technical Activities;text=Technical Activities;url=/portal/web/tandc");
aI("text=Publications Board;url=/portal/web/publicationsboard/home;");
aI("text=Boards and Committees;url=/portal/web/volunteercenter;");
aI("text=Volunteer Resource Center;url=/portal/web/volunteercenter;");
aI("text=Student Activities;url=/portal/web/studentactivities/home;");
aI("text=Awards;url=http://awards.computer.org/ana/award/viewHomepage.action;");
aI("text=Get Involved;url=/portal/web/about/getinvolved;");
}


with(milonic=new menuname("Communities_Chapters")){
margin=7;
style=submenuStyle;

aI("text=Chapters Activities;title=Chapters Activities;url=http://www.computer.org/portal/site/ieeecs/menuitem.c5efb9b8ade9096b8a9ca0108bcd45f3/index.jsp?&pName=ieeecs_level1&path=ieeecs/communities/chapter&file=index.xml&xsl=generic.xsl&;");
aI("text=Chapters Activities Board;title=Chapters Activities Board;url=http://www.computer.org/portal/site/ieeecs/menuitem.c5efb9b8ade9096b8a9ca0108bcd45f3/index.jsp?&pName=ieeecs_level1&path=ieeecs/communities/chapter&file=board.xml&xsl=generic.xsl&;");
aI("text=How to form a chapter;title=How to form a chapter;url=http://www.computer.org/portal/site/ieeecs/menuitem.c5efb9b8ade9096b8a9ca0108bcd45f3/index.jsp?&pName=ieeecs_level1&path=ieeecs/communities/chapter&file=howtoform.xml&xsl=generic.xsl&;");
aI("text=Distinguished Visitor Program;title=Distinguished Visitor Program;url=http://www.computer.org/portal/site/ieeecs/menuitem.c5efb9b8ade9096b8a9ca0108bcd45f3/index.jsp?&pName=ieeecs_level1&path=ieeecs/communities/chapter/DVP&file=index.xml&xsl=generic.xsl&;");
aI("text=Chapter Awards;title=Chapter Awards;url=http://www.computer.org/portal/site/ieeecs/menuitem.c5efb9b8ade9096b8a9ca0108bcd45f3/index.jsp?&pName=ieeecs_level1&path=ieeecs/communities/chapter&file=assistance.xml&xsl=generic.xsl&;");
aI("text=CAB Handbook;title=CAB Handbook;url=http://www.computer.org/portal/site/ieeecs/menuitem.c5efb9b8ade9096b8a9ca0108bcd45f3/index.jsp?&pName=ieeecs_level1&path=ieeecs/communities/chapter&file=CABhandbook.xml&xsl=generic.xsl&;");
aI("text=Student Clubs;title=Student Clubs;url=http://www.computer.org/portal/site/ieeecs/menuitem.c5efb9b8ade9096b8a9ca0108bcd45f3/index.jsp?&pName=ieeecs_level1&path=ieeecs/Communities/chapter&file=ClubsList.xml&xsl=generic.xsl&;");
}


with(milonic=new menuname("Communities_StandardsActivities")){
margin=7;
style=submenuStyle;

aI("text=Standards Activities Board;title=Standards Activities Board;url=/portal/web/standards/home");
aI("text=Standards Speakers Bureau;title=Standards Speakers Bureau;url=/portal/web/standards/speakers;");
}


with(milonic=new menuname("Communities_Technical Activities")){
margin=7;
style=submenuStyle;

aI("text=T&amp;C Mission and Vision;title=T&amp;C Mission and Vision;url=/portal/web/tandc/mission_vision;");
aI("text=Technical &amp; Conference Activities Board;title=Technical &amp; Conference Activities Board;url=/portal/web/tandc;");
aI("text=Technical Committees, Councils, and Task Forces;title=Technical Committees, Councils, and Task Forces;url=/portal/web/tandc/tclist;");
aI("text=Join a Technical Committee, Council, or Task Force;title=Join a Technical Committee, Council, or Task Force;url=https://cima.computer.org/TECA_Login.htm;");
aI("text=TAB Handbook;title=TAB Handbook;url=/portal/c/document_library/get_file?uuid=5c868790-c397-4f69-ba9e-5fa90f92393d&groupId=155406;");
aI("text=T&amp;C Executive Committee and Chairs;title=T&amp;C Executive Committee and Chairs;url=/portal/web/tandc/board;");
aI("text=New Chair Reference Page;title=New Chair Reference Page;url=/portal/web/tandc/newchair;");
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
