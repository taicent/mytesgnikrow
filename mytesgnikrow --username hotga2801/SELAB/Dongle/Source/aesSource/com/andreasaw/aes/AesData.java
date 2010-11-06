/*
 * AesData.java
 * @author Andrea
 */

package com.andreasaw.aes;

public class AesData {
    
    protected byte[] keybytes;
    protected byte[] input;
    
    /** Creates a new instance of AesData */
    public AesData(byte[] myInput, byte[] myKeyBytes) {
	        input = new byte[myInput.length];
	        keybytes = new byte[myKeyBytes.length];
	                
	        System.arraycopy(myInput,0,input,0,myInput.length);
	        System.arraycopy(myKeyBytes,0,keybytes,0,myKeyBytes.length);
    }
    
    
    
}
