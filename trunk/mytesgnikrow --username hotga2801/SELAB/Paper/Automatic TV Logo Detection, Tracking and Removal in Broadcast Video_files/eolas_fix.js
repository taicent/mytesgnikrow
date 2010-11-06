function vc_obj_refresh( id ) {
    var d = document;
    var obj = d.getElementById(id);
    var agt = navigator.userAgent.toLowerCase();
	
    if ( agt.indexOf('msie') != -1 && obj && 
        ( typeof obj == 'object' || typeof obj == 'function' ) ) {

        var elt = d.createElement('div');
        elt.innerHTML = obj.outerHTML;
        obj.parentNode.replaceChild( elt, obj );
    }
}

// let the main script know we're ready
EOLASloaded = 1;
