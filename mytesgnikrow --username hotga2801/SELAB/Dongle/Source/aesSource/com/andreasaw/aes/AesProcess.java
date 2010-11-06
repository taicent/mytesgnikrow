/*
 * AesProcess.java
 * @author Andrea
 */

package com.andreasaw.aes;

import java.io.*;

public class AesProcess {
    
    private AesData data;
    private Aes myAES;
    private byte[] state = new byte[16];
    private byte[] result = new byte[16];
    
    /**
     * Creates a new instance of AesProcess
     */
    public AesProcess(AesData myData) {
        data = myData;
        myAES = new Aes();
    }
    
    protected String Decryptor() throws Exception{
    
          //FileInputStream in = new FileInputStream(file1);//test.txt
          //FileOutputStream out = new FileOutputStream(file3);//test2.txt

          ByteArrayInputStream  in = new ByteArrayInputStream(data.input);
          ByteArrayOutputStream out = new ByteArrayOutputStream();

          int len = data.input.length;
          while(len>16){
              in.read(state);
              result = myAES.start(state, data.keybytes, "decrypt");
              out.write(result);
              len-=16;
          }

          //process last block here
          in.read(state);
          result = myAES.start(state, data.keybytes, "decrypt");

          ByteArrayInputStream  in2 = new ByteArrayInputStream(result);

          int data = 16-(int)result[15];   //number of bytes of actual data
          if(data>0){
              byte[] temparray = new byte[data];
              in2.read(temparray);
              out.write(temparray);
          }   
          //end processing of last block
          String decResult = new String(out.toString());
          
          in.close();
          out.close();
          
          return decResult;
            
    }
    
    protected String Encryptor() throws Exception{
        
          //FileInputStream in = new FileInputStream(file1);
          //FileOutputStream out = new FileOutputStream(file2);

          ByteArrayInputStream  in = new ByteArrayInputStream(data.input);
          ByteArrayOutputStream out = new ByteArrayOutputStream();

          int len = data.input.length;

          while(len>=16){
              in.read(state);
              result = myAES.start(state, data.keybytes, "encrypt");
              out.write(result);
              len-=16;
          }

          if(len!=0){
              byte[] stateE = new byte[len];
              in.read(stateE);
              System.arraycopy(stateE,0,state,0,len);
              for(int i=len; i<16; i++)
              {
                state[i]=(byte)((byte)16 - len);
              }
          }
          else{   //pad one block with the value (byte)16
              for(int j=0; j<16; j++)
              {
                state[j] = (byte)16;
              }
          }

          result = myAES.start(state, data.keybytes, "encrypt");
          out.write(result);

          int outLength = out.size();
          byte[] a = new byte[outLength];
          a = out.toByteArray();

          String encResult = new String(Base64.encode(a));

          /*begin test
          //String another = new String(a);
          byte[] b = new byte[]{};
          b = encResult.getBytes();
          //jTextArea3.append("String another and encResult equals? " + another.equals(encResult) + "\n");//true
          jTextArea3.append("a and b Array.Equals? " + Arrays.equals(a, b) + "\n");//false
          */


          in.close();
          out.close();
          
          return encResult;
    }
    
    
}
