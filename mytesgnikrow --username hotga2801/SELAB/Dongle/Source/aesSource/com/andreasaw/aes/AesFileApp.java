/*
 * AesFileApp.java
 * @author Andrea
 */

package com.andreasaw.aes;

import javax.swing.*;
import java.io.*;
import java.util.*; //StringTokenizer

public class AesFileApp extends JFrame{
    
    private String LS;  //this string tells the program if I want to load a file or save a file
    private File fFile; //the selected file, or just the filename if it doesn't exist
    private AesFileFilter textFilter;   //filters out the other file types other than .txt
    private AesFileFilter aesFilter;    //filters out the other file types other than .aes
    private boolean exists;//whether the filename that the user just selected exists or not
    private String userExt; //the string representation of the description of the file filter that the user selected.
                            //  If the filename doesn't exist, a new file will be created,
                            //  but more often than not the system doesn't append the file type;
                            //  ie C:\test instead of C:\test.txt
                            //  and to think I used to take those save dialogs for granted... 
    private String fileName;
    
    /** Creates a new instance of AesFileApp */
    public AesFileApp(String myLS) {
	        LS = new String(myLS);
	        textFilter = new AesFileFilter("txt", "Plaintext Files (*.txt)");
	        aesFilter = new AesFileFilter("aes", "Ciphertext Files (*.aes)");
	        exists = false;
	        fileName = new String("");
    }
   
    protected boolean openFileChooser(){
        
	      JFileChooser fc = new JFileChooser();
	      fc.setDialogTitle("Select File");
	
	      //choose only files, not directories
	      fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
	
	      //start in current directory
	      fc.setCurrentDirectory(new File("."));
	
	      //set file filter to .txt and .aes only
	      fc.addChoosableFileFilter(textFilter);
	      fc.addChoosableFileFilter(aesFilter);
	      
	      //does as it says (-:)
	      fc.setMultiSelectionEnabled(false);
	
	      //now open chooser
	      int result = fc.showDialog(this, "Select");   //this is a custom dialog
	
	      if (result == JFileChooser.CANCEL_OPTION) {
	          return false;
	      } 
	      else if (result == JFileChooser.APPROVE_OPTION) {
	          fFile = fc.getSelectedFile();
	          
	          if (LS.equals("save")){
	             //insert code to retrieve selected filter
	             userExt = new String(fc.getFileFilter().getDescription()); 
	             //end code to retrieve selected filter
	             if (fFile.exists()) {
	                int response = JOptionPane.showConfirmDialog (null,"Overwrite existing file?","Confirm Overwrite",JOptionPane.OK_CANCEL_OPTION,JOptionPane.QUESTION_MESSAGE);
	                if (response == JOptionPane.CANCEL_OPTION) 
	                    return false;
	                exists = true;
	             }
	          }
	          fileName = new String(fFile.getAbsolutePath());
	          return true; 
	          
	      } 
	 
	      return false; //if this line is reached it means the user just closed the dialog... :-P
      
    }
    
    protected String readFile() throws Exception{
	        StringBuilder fileBuffer;
	        String fileString=null;
	        String line;
	        
	        FileReader in = new FileReader(fFile);
	        BufferedReader dis = new BufferedReader(in);
	        fileBuffer = new StringBuilder() ;
	
	        while ((line = dis.readLine())!=null) {
	            fileBuffer.append(line + System.getProperty("line.separator"));
	        }
	
	        in.close ();
	        fileString = fileBuffer.toString ();
	       
	        return fileString;

    }
    
    protected String writeFile(byte[] textBytes) throws Exception{
	       //PrintWriter out;
	       BufferedOutputStream out;
	       File newFile;
	       
	       if (exists == false){
	           //rename file to add an extension based on user selected filter
	           if(userExt.startsWith("Plaintext")){
	               String newName = new String(fFile.getAbsolutePath()+".txt");
	               newFile = new File(newName);
	               fileName = newName;
	           }
	           else{
	               String newName = new String(fFile.getAbsolutePath()+".aes");
	               newFile = new File(newName);
	               fileName = newName;
	           }
	           
	           out = new BufferedOutputStream (new FileOutputStream (newFile));
	            
	           //out = new PrintWriter(new BufferedWriter(new FileWriter(newFile)));
	       }
	       else{
	           out = new BufferedOutputStream (new FileOutputStream (fFile));
	           //out = new PrintWriter(new BufferedWriter(new FileWriter(fFile)));
	           fileName = new String(fFile.getAbsolutePath());
	        }
	       out.write(textBytes);
	       //out.print(textBytes);
	       out.flush ();
	       out.close ();
	       
	       return fileName;
    }
    
    protected String getFileName(){
        
        	return fileName;
    }
    
    //re-append the EOL depending on the platform
    protected String formatEOL(String original) throws Exception{
	        StringBuilder sb = new StringBuilder();
	        StringTokenizer st = new StringTokenizer(original,"\r\n");
	
	        while(st.hasMoreTokens()) {
	            sb.append(st.nextToken());
	            sb.append(System.getProperty("line.separator") );
	        }
	        
	        return sb.toString();
	        
	  }
    
}
