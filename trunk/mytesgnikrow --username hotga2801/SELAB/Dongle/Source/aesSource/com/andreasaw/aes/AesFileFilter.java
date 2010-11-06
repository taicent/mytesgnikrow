/*
 * AesFileFilter.java
 * @author Andrea
 */

package com.andreasaw.aes;

import javax.swing.filechooser.*;
import java.io.File;

public class AesFileFilter extends FileFilter{
    
    /** Creates a new instance of AesFileFilter */
    public AesFileFilter() {
    }
    
	  private String[] extensions;
	  private String description;
	
	  public AesFileFilter(String ext, String descr) {
	    		this (new String[] {ext}, descr); 
	  }
	
	  public AesFileFilter(String[] exts, String descr) {
			    //clone and lowercase the extensions
			    extensions = new String[exts.length];
			    for (int i = exts.length - 1; i >= 0; i--) {
			      extensions[i] = exts[i].toLowerCase();
			    }
			    //make sure we have a valid (if simplistic) description
			    description = (descr == null ? exts[0] + " files" : descr);
	  }
	
	  public boolean accept(File f) {
			    //always allow directories, regardless of their extension
			    if (f.isDirectory()) { return true; }
			
			    //ok, it's a regular file so check the extension
			    String name = f.getName().toLowerCase();
			    for (int i = extensions.length - 1; i >= 0; i--) {
			      if (name.endsWith(extensions[i])) {
			        return true;
			      }
			    }
			    return false;
	  }
	
	  public String getDescription() { return description; }
    
    
}
