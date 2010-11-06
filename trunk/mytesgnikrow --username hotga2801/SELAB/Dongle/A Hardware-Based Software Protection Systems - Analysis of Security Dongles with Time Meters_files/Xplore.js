function showAdvancedSearch(){
	var url_string = '/search/advsearch.jsp';
	location.href = url_string;
}
function SearchwithinSearch() {
//To Be Reimplemented for Endeca	
/*	var qry = document.search_form.queryText.value;
	alert (qry);
	qry = qry.toLowerCase();
	var inLoc = qry.indexOf("<in>");
	var containLoc = qry.indexOf("<contains>");
	if(qry.length > 1 && inLoc== -1 && containLoc == -1){
	    qry = document.search_form.queryText.value;
//	    qry = "((" + document.search_form.queryText.value + ")<IN>metadata)";
    	}
	
	if(document.search_form.chkresult.checked == true) {

		document.search_form.chkresult.value = "true";
		if (document.search_form.oldqrytext.value.length >1){
			//qry = "((" + document.search_form.oldqrytext.value + ")<AND>" + qry.toLowerCase() + ")"; Verity code
			qry =  document.search_form.oldqrytext.value + " and " + qry.toLowerCase();
		}
	} else {
		document.search_form.chkresult.value = "false";
	}
	qry=fixsrchquote(qry);
    document.search_form.queryText.value = qry;*/
}

function fixsrchquote(oldstr) {
	
	if(oldstr.length < 1){
		nocriteria=="";
		//nocriteria="Enter one or more keywords";	
		//alert("Enter one or more keywords");
		return nocriteria;
	}
	
	nLoc = 0;
   	nStart = 0;
    	var  tempstr;
	tempstr = '"';
	nLoc = oldstr.indexOf(tempstr, nStart);
	sTemp = "";

	while(nLoc != -1) {
		sTemp += oldstr.substring(nStart, nLoc);
		sTemp += "~~";
		nStart = nLoc+1;
		nLoc = oldstr.indexOf(tempstr, nStart);
	}
	sTemp += oldstr.substring(nStart, oldstr.length);
	oldstr = sTemp;
	
	nLoc = 0;
	nStart = 0;
	tempstr = "'";
	nLoc = oldstr.indexOf(tempstr, nStart);
	sTemp = "";
	while(nLoc != -1) {
		sTemp += oldstr.substring(nStart, nLoc);
		sTemp += " ";
		nStart = nLoc+1;
		nLoc = oldstr.indexOf(tempstr, nStart);
	}
	sTemp += oldstr.substring(nStart, oldstr.length);
	
	return sTemp;
}
function resultCountAction(){
	document.getElementById("action").value="resultCount";	
	document.getElementById("random").value=new Date().getTime();
	document.searchResultsForm.submit();
}

function sortAction()
{
	document.getElementById("action").value="Sort";	
	document.getElementById("random").value=new Date().getTime();
	document.searchResultsForm.submit();
}

function resetQueryText()
{	
	if (document.getElementById('chkresult').checked){
//		document.getElementById('queryText').value="";	
		document.getElementById('queryText').focus();
	} //else {
		//document.getElementById('queryText').value = document.getElementById('oldqrytext').value;
	//}
	
	
}

/** Advanced Search functions 
 * 
 * @param Mform
 * @param Mmenu
 * @param Mvalue
 * @param Mtext
 * @return
 */
function menuChange(Mform, Mmenu, Mvalue, Mtext) {
	 document.getElementById(Mmenu).value = Mtext
	 if(Mmenu=='menu1'){
	    document.getElementById("scope1").value = Mvalue;
	 }
	 if(Mmenu=='menu2'){
	    document.getElementById("scope2").value = Mvalue;
	 }
	 if(Mmenu=='menu3'){
	    document.getElementById("scope3").value = Mvalue;
	 }
	 var dropMenu = 'Drop' + Mmenu
	 MM_showHideLayers(dropMenu,'','hide')
	 menu1 = Mvalue;
	 
	}


	function chk_range(val){

	    if(val=='recentyr'){
	         document.bsearch.py1.value='1950';
	          document.bsearch.py1.disabled=true;
	          document.bsearch.py2.value='<%=YY%>';
	          document.bsearch.py2.disabled=true;
	     }  
	    if(val=='allyr'){
	          document.bsearch.py1.disabled=false;
	          //document.bsearch.py1.checked=false;
	          document.bsearch.py2.disabled=false;
	          //document.bsearch.py2.checked=false;
	     }

	}

	function changePublications() {
	 if (document.bsearch.std_status.selectedIndex > 0) {
	    document.bsearch.coll1.checked=false;
	    document.bsearch.coll2.checked=false;
	    document.bsearch.coll3.checked=false;
	    document.bsearch.coll4.checked=false;
	    //document.bsearch.coll5.checked=true; 
	    document.bsearch.coll1.disabled=true;
	    document.bsearch.coll2.disabled=true;
	    document.bsearch.coll3.disabled=true;
	    document.bsearch.coll4.disabled=true;
	    //document.bsearch.coll5.disabled=true;
	    document.bsearch.srchlist[0].checked=true;
	 } else {
	    document.bsearch.coll1.checked=true;
	    document.bsearch.coll2.checked=true;
	    document.bsearch.coll3.checked=true;
	    document.bsearch.coll4.checked=true;
	    //document.bsearch.coll5.checked=true; 
	    document.bsearch.coll1.disabled=false;
	    document.bsearch.coll2.disabled=false;
	    document.bsearch.coll3.disabled=false;
	    document.bsearch.coll4.disabled=false;
	    //document.bsearch.coll5.disabled=false;
	    document.bsearch.srchlist[0].checked=true;
	  }
	 }

	// For advsearch.jsp
	function isvalidsearch(f,yr) {
	    var q1 = "true";
	    var q2 = "true";
	    for (var i = 0; i < f.length; i++) {
	        var c = f.elements[i];
	        if (c.name == "query1") {
	            if ((c.value == null) || (c.value == "") || isblank(c.value)) {
	                q1 = "false";
	            }
	        } 
	        if (c.name == "queryblock") {
	            if ((c.value == null) || (c.value == "") || isblank(c.value)) {
	                q2 = "false";
	            }
	        }
	    }
	    if ((q1 == "false") && (q2 == "false")) {
	    alert("Please enter at least one search criteria!");
	    return false;
	    }
	    BuildSrchQuery(yr);
	    return true;
	}


	
	function BuildSrchQuery(YY){
	    
	    var queryText;
	    
	    var qry1 = document.bsearch.query1.value;
	    var len1 = qry1.length;
	            
	    var qry2 = document.bsearch.query2.value;
	    var len2 = qry2.length;
	            
	    var qry3 = document.bsearch.query3.value;
	    var len3 = qry3.length;
	    
	    var sc1 = document.bsearch.scope1.value;
	    var sc2 = document.bsearch.scope2.value;
	    var sc3 = document.bsearch.scope3.value;
	    
	    var op1 = document.bsearch.op1.value;
	    var op2 = document.bsearch.op2.value;

	    var p1 = document.bsearch.py1.value;
	    var p2 = document.bsearch.py2.value;
	    	    
	    var queryText = document.bsearch.queryblock.value;
	    queryText = queryText.toLowerCase();
	    if(queryText.length > 0){
	    	var nLoc = queryText.indexOf("<in>");
	    	var containLoc = queryText.indexOf("<contains>");
	    	if(nLoc==-1 && containLoc == -1 ){
	    	  queryText = "(("+ queryText +")<in>metadata)";
	    	}
	    	document.bsearch.reqloc.value = "adv";
	    	
	    }

	    if( len1 > 0 || len2 > 0 || len3 > 0) {
	       
	        if(sc1.length > 0 && qry1.length > 0){
	            qry1 = " ((" + qry1 + ")<in>" + sc1 + " ) ";
	        }
	        
	        if(sc2.length > 0 && qry2.length > 0){
	            qry2 = " ((" + qry2 + ")<in>" + sc2 + " ) ";
	        }
	        
	        if(sc3.length > 0 && qry3.length > 0){
	            qry3 = " ((" + qry3 + ")<in>" + sc3 + " ) ";
	        }
		
			
	        
	        if(len1 > 0 && len2 < 1 && len3 < 1){
	         
	          queryText = qry1;
	        }
	        
	        if(len1 > 0 && len2 > 0 && len3 < 1){
	          queryText = "("+ qry1 + "<" + op1 +">" + qry2 +")";
	        }
	        
	        if(len1 > 0 && len2 > 0 && len3 > 0){
	          queryText = "("+ qry1 + "<" + op1 +">" + qry2 +")"+ "<" + op2 + ">" +qry3;
	        }
	    
	    }
	    queryText=fixsrchquote(queryText);
	    
	    
	    if(p1 == '1950' && p2 == YY && queryText.length > 0){
		    queryText = queryText;
	    }else{
	        queryText = "(" + queryText +") <and> (pyr >= "+ p1 + " <and> pyr <= " + p2 + ")";
	    }
	  	
	    document.bsearch.queryText.value = queryText.toLowerCase();    
	    
	    var ipel = SelectRadio();
	    
	    if(ipel=='ipellist'){
	    	
	    	document.bsearch.queryText.value = "("+queryText.toLowerCase() + ")<and>(ipel_flag<in>packages)";
	    	
	    }
	    
	    if(document.bsearch.coll1.checked ==true) {
		    document.bsearch.coll6.value = "preprint";  
	    }
	    if(document.bsearch.coll1.checked ==false) {
		    document.bsearch.coll6.value = "";
	    }
	   
	}
	
	function buildQuery(YY)
	{
		
		 var queryText;
		 var Ntk, Ntt, Nf;
		 
		 var qry1 = document.advancedSearchForm.query1.value;
		 var len1 = qry1.length;
		            
		 var qry2 = document.advancedSearchForm.query2.value;
		 var len2 = qry2.length;
		            
		 var qry3 = document.advancedSearchForm.query3.value;
		 var len3 = qry3.length;
		    
		 var sc1 = document.advancedSearchForm.scope1.value;
		 var sc2 = document.advancedSearchForm.scope2.value;
		 var sc3 = document.advancedSearchForm.scope3.value;
		    
		 var op1 = document.advancedSearchForm.op1.value;
		 var op2 = document.advancedSearchForm.op2.value;
		    
		 var p1 = document.advancedSearchForm.py1.value;
		 var p2 = document.advancedSearchForm.py2.value;
		    
		 if( len1 > 0 || len2 > 0 || len3 > 0) {
			       
		     if(sc1.length > 0 && qry1.length > 0){
		    	queryText = qry1;
		     	Ntk = sc1;
		      	Ntt = qry1;
		     }
		        
		     if(sc2.length > 0 && qry2.length > 0){
		       	if(Ntk.length>0){
			       	Ntk = Ntk+"|"+sc2;
			       	Ntt = Ntt+"|"+qry2;
		       	}else{
			        Ntk = sc2;
			       	Ntt = qry2;
		       	}
		     }
		        
		     if(sc3.length > 0 && qry3.length > 0){
		       	if(Ntk.length>0){
			       	Ntk = Ntk+"|"+sc3;
			       	Ntt = Ntt+"|"+qry3;
		       	}else{
			        Ntk = sc3;
			       	Ntt = qry3;
		       	}
		     }
		 }
		 if(p1 == '1950' && p2 == YY && queryText.length > 0){
		 }else if(p1.length>0&&p2.length>0){
		    Nf = "year|BTWN "+ p1+" "+p2;
		 }else
			Nf ="";
		var N="";
		for(i=1; i<document.advancedSearchForm.paramN.length;i++){
			if(document.advancedSearchForm.paramN[i].checked){
				if(N.length>=1){					
					N = N+"+"+document.advancedSearchForm.paramN[i].value;
				}else{
					N = document.advancedSearchForm.paramN[i].value;
				}
			}
		}
		if(N.length<1){
			N = "0";
		}	
		document.advancedSearchForm.Ntk.value = Ntk; 
		document.advancedSearchForm.Ntt.value = Ntt; 
		document.advancedSearchForm.Nf.value = Nf; 
		document.advancedSearchForm.N.value = N; 
		document.advancedSearchForm.queryText.value = Ntt; 
	}
	
/*	function buildQuery(YY)
	{
		
		 var queryText;
		    
		    var qry1 = document.advancedSearchForm.query1.value;
		    var len1 = qry1.length;
		            
		    var qry2 = document.advancedSearchForm.query2.value;
		    var len2 = qry2.length;
		            
		    var qry3 = document.advancedSearchForm.query3.value;
		    var len3 = qry3.length;
		    
		    var sc1 = document.advancedSearchForm.scope1.value;
		    var sc2 = document.advancedSearchForm.scope2.value;
		    var sc3 = document.advancedSearchForm.scope3.value;
		    
		    var op1 = document.advancedSearchForm.op1.value;
		    var op2 = document.advancedSearchForm.op2.value;
		    
		    var p1 = document.advancedSearchForm.py1.value;
		    var p2 = document.advancedSearchForm.py2.value;
		    
		    if( len1 > 0 || len2 > 0 || len3 > 0) {
			       
		        if(sc1.length > 0 && qry1.length > 0){
		            qry1 = " ((" + qry1 + ")<in>" + sc1 + " ) ";
		        }
		        
		        if(sc2.length > 0 && qry2.length > 0){
		            qry2 = " ((" + qry2 + ")<in>" + sc2 + " ) ";
		        }
		        
		        if(sc3.length > 0 && qry3.length > 0){
		            qry3 = " ((" + qry3 + ")<in>" + sc3 + " ) ";
		        }
			
				
		        
		        if(len1 > 0 && len2 < 1 && len3 < 1){
		         
		          queryText = qry1;
		        }
		        
		        if(len1 > 0 && len2 > 0 && len3 < 1){
		          queryText = "("+ qry1 + "<" + op1 +">" + qry2 +")";
		        }
		        
		        if(len1 > 0 && len2 > 0 && len3 > 0){
		          queryText = "("+ qry1 + "<" + op1 +">" + qry2 +")"+ "<" + op2 + ">" +qry3;
		        }
		    
		    }
		    if(p1 == '1950' && p2 == YY && queryText.length > 0){
			    queryText = queryText;
		    }else{
		        queryText = "(" + queryText +") <and> (pyr >= "+ p1 + " <and> pyr <= " + p2 + ")";
		    }
		    queryText=fixsrchquote(queryText);
		    document.advancedSearchForm.queryText.value = queryText.toLowerCase(); 
	}
*/
	function chk_range(val,yy){

	    if(val=='recentyr'){
	         document.advancedSearchForm.py1.value='1950';
	          document.advancedSearchForm.py1.disabled=true;
	          document.advancedSearchForm.py2.value=yy;
	          document.advancedSearchForm.py2.disabled=true;
	     }  
	    if(val=='allyr'){
	          document.advancedSearchForm.py1.disabled=false;
	          document.advancedSearchForm.py2.disabled=false;
	     }

	}
	

	
	
