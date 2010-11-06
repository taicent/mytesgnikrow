
function toggleNotice() {
	var notice = document.getElementById('centralNotice');
	if (!wgNoticeToggleState) {
		notice.className = notice.className.replace('collapsed', 'expanded');
		toggleNoticeCookie('0');
	} else {
		notice.className = notice.className.replace('expanded', 'collapsed');
		toggleNoticeCookie('1');
	}
	wgNoticeToggleState = !wgNoticeToggleState;
}
function toggleNoticeStyle(elems, display) {
	if(elems)
		for(var i=0;i<elems.length;i++)
			elems[i].style.display = display;
}
function toggleNoticeCookie(state) {
	var e = new Date();
	e.setTime( e.getTime() + (7*24*60*60*1000) ); // one week
	var work='hidesnmessage='+state+'; expires=' + e.toGMTString() + '; path=/';
	document.cookie = work;
}
function pickTemplate(templates, weights) {
	var weightedTemplates = new Array();
	var currentTemplate = 0;
	var totalWeight = 0;

	if (templates.length == 0)
		return '';

	while (currentTemplate < templates.length) {
		totalWeight += weights[currentTemplate];
		for (i=0; i<weights[currentTemplate]; i++) {
			weightedTemplates[weightedTemplates.length] = templates[currentTemplate];
		}
		currentTemplate++;
	}

	if (totalWeight == 0)
		return '';

	var randomnumber=Math.floor(Math.random()*totalWeight);
	return weightedTemplates[randomnumber];
}


var wgNoticeToggleState = (document.cookie.indexOf('hidesnmessage=1')==-1);
document.writeln("\x3cstyle type=\"text/css\"\x3e\n#centralNotice .siteNoticeSmall{display:none;}\n#centralNotice .siteNoticeSmallAnon{display:none;}\n#centralNotice .siteNoticeSmallUser{display:none;}\n#centralNotice.collapsed .siteNoticeBig{display:none;}\n#centralNotice.collapsed .siteNoticeSmall{display:block;}\n#centralNotice.collapsed .siteNoticeSmallUser{display:block;}\n#centralNotice.collapsed .siteNoticeSmallAnon{display:block;}\n#centralNotice.anonnotice .siteNoticeSmallUser{display:none !important;}\n#centralNotice.usernotice .siteNoticeSmallAnon{display:none !important;}\n\x3c/style\x3e");

wgNotice=pickTemplate(["\x3c!-- empty --\x3e", "\x3cstyle type=\"text/css\"\x3e\n/*\nStyles for Notices\n*/\na { text-decoration: none; }\na:hover { text-decoration: underline; }\n\na:hover table tr td #no-link-underline { text-decoration: none; }\n\n\na table\n{\n	text-decoration: none;\n	color: #6e98c2;\n}\n\na:hover table tr td { text-decoration: underline; }\na:hover table tr td #use-underline { text-decoration: underline; }\n\na:hover #no-link-underline {text-decoration: none;}\n\n.text-one-line\n{\n	font-size: 3.08em;\n	font-weight: bold;\n}\n\ndiv.grayBorder\n{\n	border: 1px solid #bbb;\n	background-color: #fbfbfb;\n	font-family: helvetica, impact, sans-serif;\n	overflow: hidden;\n}\n\n.nobr { white-space: nowrap; }\n#centralNotice.collapsed .central-notice-banner {\n display: none;\n}\n\n.central-notice-banner\n{\n	position: relative;\n	width: 100%;\n	height: 100px;\n	border: 1px solid #bbb;\n	background-color: #fbfbfb;\n	font-family: helvetica, impact, sans-serif;\n	overflow: hidden;\n}\n\n#toggle-box\n{\n	position: absolute;\n	top: 3px;\n	right: 4px;\n	font-size: 0.8em;\n	text-align: right;\n}\n\n.blue-text\n{\n	font-weight: bold;\n	font-family: helvetica, impact, sans-serif;\n	color: #6e98c2;\n	text-align: left;\n	margin:auto;\n	width:100%;\n}\n\n/* notice 2 and up */\n\n.quote-text { }\n\n.quote-data\n{\n	font-size: .8em;\n	color: black;\n	font-weight: normal;\n}\n\n.center1\n{\n	font-weight: bold;\n	color: #6e98c2;\n	vertical-align: middle;\n}\n\n.lines-1 { font-size: 2.25em; }\n\x3c/style\x3e\n\x3chtml\x3e\n\n\x3cdiv class=\"central-notice-banner\"\x3e\n\x3cdiv id=\"toggle-box\"\x3e\n	[\x3ca href=\"#\" onclick=\"toggleNotice();return false\"\x3eHide\x3c/a\x3e]\x3cbr /\x3e\n	[\x3ca href=\"http://meta.wikimedia.org/wiki/CentralNotice/Work_at_Wikimedia\"\x3eHelp us with translations!\x3c/a\x3e]\n\x3c/div\x3e\n\n\x3ctable class=\"blue-text\" style=\"height: 100px;\"\x3e\n\x3ctr\x3e\n\x3ctd class=\"lines-1\"\x3e\x3ca href=\"http://wikimediafoundation.org/wiki/Special:CommunityHiring\"\x3e\x3c/html\x3e\x3ccenter\x3eWork at Wikipedia.\x3c/center\x3e\x3chtml\x3e\x3c/a\x3e\x3c/td\x3e\n\x3c/tr\x3e\n\x3c/table\x3e\n\n\x3c/a\x3e\n\x3c/div\x3e"],[90, 10]);
if (wgNotice != '')
wgNotice='<div id="centralNotice" class="' + (wgNoticeToggleState ? 'expanded' : 'collapsed') + ' ' + (wgUserName ? 'usernotice' : 'anonnotice' ) + '">' + wgNotice+'</div>';
