/*
 * Aes.java
 * @author Andrea
 */

package com.andreasaw.aes;

import java.io.*;
import java.security.*;
import javax.crypto.*;

public class Aes {
    
    /**
     * Creates a new instance of Aes
     */
    public Aes() {
    }

    private static byte[] key = new byte[176];
    private static byte[] dw = new byte[176];

    private static byte[] cipherkey = new byte[16];

    private static byte[] sbox =
    {	0x63, 0x7c, 0x77, 0x7b, (byte)0xf2, 0x6b, 0x6f, (byte)0xc5, 0x30, 0x01, 0x67, 0x2b, (byte)0xfe, (byte)0xd7, (byte)0xab, 0x76,

            (byte)0xca, (byte)0x82, (byte)0xc9, 0x7d, (byte)0xfa, 0x59, 0x47, (byte)0xf0, (byte)0xad, (byte)0xd4, (byte)0xa2, (byte)0xaf, (byte)0x9c, (byte)0xa4, 0x72, (byte)0xc0,

            (byte)0xb7, (byte)0xfd, (byte)0x93, 0x26, 0x36, 0x3f, (byte)0xf7, (byte)0xcc, 0x34, (byte)0xa5, (byte)0xe5, (byte)0xf1, 0x71, (byte)0xd8, 0x31, 0x15,

            0x04, (byte)0xc7, 0x23, (byte)0xc3, 0x18, (byte)0x96, 0x05, (byte)0x9a, 0x07, 0x12, (byte)0x80, (byte)0xe2, (byte)0xeb, 0x27, (byte)0xb2, 0x75,

            0x09, (byte)0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, (byte)0xa0, 0x52, 0x3b, (byte)0xd6, (byte)0xb3, 0x29, (byte)0xe3, 0x2f, (byte)0x84,

            0x53, (byte)0xd1, 0x00, (byte)0xed, 0x20, (byte)0xfc, (byte)0xb1, 0x5b, 0x6a, (byte)0xcb, (byte)0xbe, 0x39, 0x4a, 0x4c, 0x58, (byte)0xcf,

            (byte)0xd0, (byte)0xef, (byte)0xaa, (byte)0xfb, 0x43, 0x4d, 0x33, (byte)0x85, 0x45, (byte)0xf9, 0x02, 0x7f, 0x50, 0x3c, (byte)0x9f, (byte)0xa8,

            0x51, (byte)0xa3, 0x40, (byte)0x8f, (byte)0x92, (byte)0x9d, 0x38, (byte)0xf5, (byte)0xbc, (byte)0xb6, (byte)0xda, 0x21, 0x10, (byte)0xff, (byte)0xf3, (byte)0xd2,

            (byte)0xcd, 0x0c, 0x13, (byte)0xec, 0x5f, (byte)0x97, 0x44, 0x17, (byte)0xc4, (byte)0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,

            0x60, (byte)0x81, 0x4f, (byte)0xdc, 0x22, 0x2a, (byte)0x90, (byte)0x88, 0x46, (byte)0xee, (byte)0xb8, 0x14, (byte)0xde, 0x5e, 0x0b, (byte)0xdb,

            (byte)0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, (byte)0xc2, (byte)0xd3, (byte)0xac, 0x62, (byte)0x91, (byte)0x95, (byte)0xe4, 0x79,

            (byte)0xe7, (byte)0xc8, 0x37, 0x6d, (byte)0x8d, (byte)0xd5, 0x4e, (byte)0xa9, 0x6c, 0x56, (byte)0xf4, (byte)0xea, 0x65, 0x7a, (byte)0xae, 0x08,

            (byte)0xba, 0x78, 0x25, 0x2e, 0x1c, (byte)0xa6, (byte)0xb4, (byte)0xc6, (byte)0xe8, (byte)0xdd, 0x74, 0x1f, 0x4b, (byte)0xbd, (byte)0x8b, (byte)0x8a,

            0x70, 0x3e, (byte)0xb5, 0x66, 0x48, 0x03, (byte)0xf6, 0x0e, 0x61, 0x35, 0x57, (byte)0xb9, (byte)0x86, (byte)0xc1, 0x1d, (byte)0x9e,

            (byte)0xe1, (byte)0xf8, (byte)0x98, 0x11, 0x69, (byte)0xd9, (byte)0x8e, (byte)0x94, (byte)0x9b, 0x1e, (byte)0x87, (byte)0xe9, (byte)0xce, 0x55, 0x28, (byte)0xdf,

            (byte)0x8c, (byte)0xa1, (byte)0x89, 0x0d, (byte)0xbf, (byte)0xe6, 0x42, 0x68, 0x41, (byte)0x99, 0x2d, 0x0f, (byte)0xb0, 0x54, (byte)0xbb, 0x16

    };

    private static byte[] invsbox =
    {
            0x52, 0x09, 0x6a, (byte)0xd5, 0x30, 0x36, (byte)0xa5, 0x38, (byte)0xbf, 0x40, (byte)0xa3, (byte)0x9e, (byte)0x81, (byte)0xf3, (byte)0xd7, (byte)0xfb,

            0x7c, (byte)0xe3, 0x39, (byte)0x82, (byte)0x9b, 0x2f, (byte)0xff, (byte)0x87, 0x34, (byte)0x8e, 0x43, 0x44, (byte)0xc4, (byte)0xde, (byte)0xe9, (byte)0xcb,

            0x54, (byte)0x7b, (byte)0x94, 0x32, (byte)0xa6, (byte)0xc2, 0x23, 0x3d, (byte)0xee, 0x4c, (byte)0x95, 0x0b, 0x42, (byte)0xfa, (byte)0xc3, 0x4e,

            0x08, 0x2e, (byte)0xa1, 0x66, 0x28, (byte)0xd9, 0x24, (byte)0xb2, 0x76, 0x5b, (byte)0xa2, 0x49, 0x6d, (byte)0x8b, (byte)0xd1, 0x25,

            0x72, (byte)0xf8, (byte)0xf6, 0x64, (byte)0x86, 0x68, (byte)0x98, 0x16, (byte)0xd4, (byte)0xa4, 0x5c, (byte)0xcc, 0x5d, 0x65, (byte)0xb6, (byte)0x92,

            0x6c, 0x70, 0x48, 0x50, (byte)0xfd, (byte)0xed, (byte)0xb9, (byte)0xda, 0x5e, 0x15, 0x46, 0x57, (byte)0xa7, (byte)0x8d, (byte)0x9d, (byte)0x84,

            (byte)0x90, (byte)0xd8, (byte)0xab, 0x00, (byte)0x8c, (byte)0xbc, (byte)0xd3, 0x0a, (byte)0xf7, (byte)0xe4, 0x58, 0x05, (byte)0xb8, (byte)0xb3, 0x45, 0x06,

            (byte)0xd0, 0x2c, 0x1e, (byte)0x8f, (byte)0xca, 0x3f, 0x0f, 0x02, (byte)0xc1, (byte)0xaf, (byte)0xbd, 0x03, 0x01, 0x13, (byte)0x8a, 0x6b,

            0x3a, (byte)0x91, 0x11, 0x41, 0x4f, 0x67, (byte)0xdc, (byte)0xea, (byte)0x97, (byte)0xf2, (byte)0xcf, (byte)0xce, (byte)0xf0, (byte)0xb4, (byte)0xe6, 0x73,

            (byte)0x96, (byte)0xac, 0x74, 0x22, (byte)0xe7, (byte)0xad, 0x35, (byte)0x85, (byte)0xe2, (byte)0xf9, 0x37, (byte)0xe8, 0x1c, 0x75, (byte)0xdf, 0x6e,

            0x47, (byte)0xf1, 0x1a, 0x71, 0x1d, 0x29, (byte)0xc5, (byte)0x89, 0x6f, (byte)0xb7, 0x62, 0x0e, (byte)0xaa, 0x18, (byte)0xbe, 0x1b,

            (byte)0xfc, 0x56, 0x3e, 0x4b, (byte)0xc6, (byte)0xd2, 0x79, 0x20, (byte)0x9a, (byte)0xdb, (byte)0xc0, (byte)0xfe, 0x78, (byte)0xcd, 0x5a, (byte)0xf4,

            0x1f, (byte)0xdd, (byte)0xa8, 0x33, (byte)0x88, 0x07, (byte)0xc7, 0x31, (byte)0xb1, 0x12, 0x10, 0x59, 0x27, (byte)0x80, (byte)0xec, 0x5f,

            0x60, 0x51, 0x7f, (byte)0xa9, 0x19, (byte)0xb5, 0x4a, 0x0d, 0x2d, (byte)0xe5, 0x7a, (byte)0x9f, (byte)0x93, (byte)0xc9, (byte)0x9c, (byte)0xef,

            (byte)0xa0, (byte)0xe0, 0x3b, 0x4d, (byte)0xae, 0x2a, (byte)0xf5, (byte)0xb0, (byte)0xc8, (byte)0xeb, (byte)0xbb, 0x3c, (byte)0x83, 0x53, (byte)0x99, 0x61,

            0x17, 0x2b, 0x04, 0x7e, (byte)0xba, 0x77, (byte)0xd6, 0x26, (byte)0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
    };

    private static byte[] RC = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, (byte)(0x80), 0x1b, 0x36, 0x6c, (byte)(0xd8), (byte)(0xab)};
    // RC is obtained from fips draft page 22. starts with 0x01, xtime(0x01) = 0x02, xtime(0x02) = 0x04, xtime(0x04) = 0x08, xtime(0x08) = 0x10,
    // xtime(0x10) = 0x20, xtime(0x20) = 0x40, xtime(0x40) = 0x80, xtime(0x80) = 0x1b, xtime(0x1b)=0x36 etc...

    private static void ByteSub (byte[] state)
    {

            short temp1, temp2;	//"short" bcoz byte can only accept values to from -127 to 128 but s box index is 0-255
            short subs;		//subs => array SUBScript

            for (byte i = 0; i<16; i++)	// one block is 128 bits. 128/8 = 16 so 16 times bytesub
            {
                    temp1 = temp2 = (short) (state[i]);	//copy state[i] to temp1 and temp2
                                                            //example fips page 19, state[i] = {53}
                    temp1>>>=4;				//temp1 is now 0x0005
                    temp1&=0x000f;				//clear all bits except last 4 bits
                    temp2&=0x000f;				//temp2 = 0x0053 , 0x0053 & 0x000f = 0x0003

                                                                    //fips draft, page 19.
                                                                    //sbox array index (subscript) for single dimensional array is:
                                                                    //0 : 0-15
                                                                    //1 : 16-31
                                                                    //2 : 32-47
                                                                    //3 : 48-63
                                                                    //4 : 64-79
                                                                    //5 : 80-95
                                                                    //    :
                                                                    //    :
                                                                    //f : 240-255
                    subs = (short) ( temp1*16+temp2 );	//so, intersection of '5' and '3' is '5'*16 + '3' = subscript 83

                    state[i] = sbox[subs];
            }

    }

    private static byte ByteSub (byte inputbyte) 		//function overloading accepts one byte instead of byte array
    {
            short temp1, temp2;
            short subs;

            temp1 = temp2 = (short)inputbyte;

            temp1>>>=4;
            temp1&=0x000f;
            temp2&=0x000f;
            subs = (short)(temp1*16+temp2);

            inputbyte = sbox[subs];

            return inputbyte;
    }

    public static void InvByteSub (byte[] state)
    {

            short temp1, temp2;
            short subs;

            for (byte i = 0; i<16; i++)
            {
                    temp1 = temp2 = (short) (state[i]);
                    temp1>>>=4;
                    temp1&=0x000f;
                    temp2&=0x000f;
                    subs = (short) (temp1*16+temp2);

                    state[i] = invsbox[subs];
            }

    }

    private static void ShiftRow3(byte[] state)	//fips draft page 19 and page 20
                                                    // S[0][0] => state[0]; S[0][1] => state[4] etc...
                                                    // S[1][0] => state[1]; S[1][1] => state[5] etc...
                                                    // S[2][0] => state[2]; S[2][1] => state[6] etc...
                                                    // S[3][0] => state[3]; S[3][1] => state[7] etc...
    {
            byte[] temp = new byte [2];

            //first row shift state[1], state[5], state[9], state[13] => state[5], state[9], state[13], state[1]
            //left cyclic shift of 1 byte
            temp[0] = state[1];
            for(byte i = 1; i<10; i+=4)
            {
                    state[i] = state[i+4];
            }
            state[13] = temp[0];

            //second row shift state[2], state[6], state[10], state[14] => state[10], state[14], state[2], state[6]
            //left cyclic shift of 2 bytes
            temp[0] = state[2];
            temp[1] = state[6];
            for(byte j = 2; j<7; j+=4)
            {
                    state[j] = state[j+8];
            }
            state[10] = temp[0];
            state[14] = temp[1];

            //third row shift state[3], state[7], state[11], state[15] => state[15], state[3], state[7], state[11]
            //left cyclic shift of 3 bytes, equivalent to right cyclic shift of 1 byte
            temp[0] = state[15];
            for(byte k = 15; k>6; k-=4)
            {
                    state[k] = state[k-4];
            }
            state[3] = temp[0];
    }

    private static void InvShiftRow (byte[] state)
    {

            byte[] temp = new byte[2];

            // 1st row shift, left cyclic shift of 3 bytes, equivalent to 1 shift to the right

            temp[0] = state[13];

            for(byte i=13; i>4; i-=4)
            {
                    state[i]=state[i-4];
            }
            state[1] = temp[0];


            // 2nd row shift, left cyclic shift of 2 bytes

            temp[0] = state[2];
            temp[1] = state[6];

            for(byte j=2; j<7; j+=4)
            {
                    state[j] = state[j+8];
            }
            state[10]=temp[0];
            state[14]=temp[1];


            // 3rd row shift, left cyclic shift of 1 byte

            temp[0] = state[3];

            for(byte k=3; k<12; k+=4)
            {
                    state[k] = state[k+4];
            }
            state[15] = temp[0];

    }


    private static void MixColumn2 (byte[] state)
    {

            byte[] temp = new byte[16];

            for(byte i = 0; i<16; i++)
            {
                    temp[i] = state[i];
            }

            //concept: from fips draft page 20
            //s[0] = ({02} * s[0]) ^ ({03} * s[1]) ^ s[2] ^ s[3]
            //s[0] = (XTime(s[0]) ^ ({02 ^ 01} * s[1]) ^ s[2] ^ s[3]
            //s[0] = (XTime(s[0]) ^ ( ({02}*s[1]) ^ ({01}*s[1]) ) ^ s[2] ^ s[3]
            //s[0] = (XTime(s[0]) ^ ( XTime(s[1]) ^ s[1] ) ^ s[2] ^ s[3]

            //and so on for s[1], s[2], s[3]...

            for(byte k=0; k<16; k+=4)		//increment in 4 so that from 0 it will go to 4 in the next iteration completing all 16 bytes
            {
                    state[0+k] = (byte) ( XTime(temp[0+k]) ^ ( temp[1+k] ^ XTime(temp[1+k]) ) ^ temp[2+k] ^ temp[3+k] );

                    state[1+k] = (byte) ( temp[0+k] ^ ( XTime(temp[1+k]) ) ^ ( temp[2+k] ^ XTime(temp[2+k]) ) ^ temp[3+k] );

                    state[2+k] = (byte) ( temp[0+k] ^ temp[1+k] ^ XTime(temp[2+k]) ^ ( temp[3+k] ^ XTime(temp[3+k]) ) );

                    state[3+k] = (byte) ( ( temp[0+k] ^ XTime(temp[0+k]) ) ^ temp[1+k] ^ temp[2+k] ^ XTime(temp[3+k]) );
            }

    }

    // InvMixColumn according to polynomial multiplication (d(x) = 0bx3 + 0dx2 + 09x + 0e)
    // please bear in mind tht this function is also used by GenerateKey (for Equivalent Inverse Cipher)
    private static void InvMixColumn (byte[] state)
    {

            byte[] xarray = new byte[48];	//array for storing multiplication with 0x02, 0x04, 0x08 values

            byte count = 0;

            //basically, for every element in array a, there are 3 values corresponding to multiplication by 0x02, 0x04 and 0x08
            //eg for state[0], xarray[0] is state[0]*0x02, xarray[1] is state[0]*0x04, xarray[2] is state[0]*0x08 and so on

            for(byte k=0; k<16; k++)
            {
                    xarray[count] = XTime(state[k]);		//multiplication by 0x02
                    count++;					//2 lines instead of 1 ie {count++ and then xarray[count]} instead of xarray[count++] for clarity
                    xarray[count] = XTime(xarray[count-1]);	//multiplication by 0x04
                    count++;
                    xarray[count] = XTime(xarray[count-1]);	//multiplication by 0x08
                    count++;
            }

            //store the state into a temp array first because the 3 subsequent statements in the for loop need to use the original array

            byte[] temp = new byte[16];

            for(byte i = 0; i<16; i++)
            {
                    temp[i] = state[i];
            }


            //InvMixColumn according to section 5.3.3 FIPS draft

            //from fips draft page 25
            //0B => 08 ^ 02 ^ 01
            //0D => 08 ^ 04 ^ 01
            //09 => 08 ^ 01
            //0E => 08 ^ 04 ^ 02

            //a[0] = 	( (a[0]*08) ^ (a[0]*04) ^ (a[0]*02) ) ^	:: xarray[2] ^ xarray[1] ^ xarray[0]
            //		( (a[1]*08) ^ (a[1]*02) ^ (a[1]*01) ) ^	:: xarray[5] ^ xarray[3] ^ a[1]
            //		( (a[2]*08) ^ (a[2]*04) ^ (a[2]*01) ) ^	:: xarray[8] ^ xarray[7] ^ a[2]
            //		( (a[3]*08) ^ (a[3]*01) )		:: xarray[11] ^a[3]

            //a[1] = 	( (a[0]*08) ^ (a[0]*01) )		:: xarray[2] ^a[0]
            //		( (a[1]*08) ^ (a[1]*04) ^ (a[1]*02) ) ^	:: xarray[5] ^ xarray[4] ^ xarray[3]
            //		( (a[2]*08) ^ (a[2]*02) ^ (a[2]*01) ) ^	:: xarray[8] ^ xarray[6] ^ a[2]
            //		( (a[3]*08) ^ (a[3]*04) ^ (a[3]*01) ) ^	:: xarray[11] ^ xarray[10] ^ a[3]

            //a[2] = 	( (a[0]*08) ^ (a[0]*04) ^ (a[0]*01) ) ^	:: xarray[2] ^ xarray[1] ^ a[0]
            //		( (a[1]*08) ^ (a[1]*01) )		:: xarray[5] ^a[1]
            //		( (a[2]*08) ^ (a[2]*04) ^ (a[2]*02) ) ^	:: xarray[8] ^ xarray[7] ^ xarray[6]
            //		( (a[3]*08) ^ (a[3]*02) ^ (a[3]*01) ) ^	:: xarray[11] ^ xarray[9] ^ a[3]

            //a[3] =	( (a[0]*08) ^ (a[0]*02) ^ (a[0]*01) ) ^	:: xarray[2] ^ xarray[0] ^ a[0]
            // 		( (a[1]*08) ^ (a[1]*04) ^ (a[1]*01) ) ^	:: xarray[5] ^ xarray[4] ^ a[1]
            //		( (a[2]*08) ^ (a[2]*01) )		:: xarray[8] ^a[2]
            //		( (a[3]*08) ^ (a[3]*04) ^ (a[3]*02) ) ^	:: xarray[11] ^ xarray[10] ^ xarray[9]


            for(byte y=0; y<16; y+=4)
            {
                    state[0+y] = (byte) ( (xarray[2+ ( y*3 )]^xarray[1+ ( y*3 )]^xarray[0+ ( y*3 )]) ^ (xarray[5+ ( y*3 )]^xarray[3+ ( y*3 )]^temp[1+y]) ^ (xarray[8+ ( y*3 )]^xarray[7+ ( y*3 )]^temp[2+y]) ^ (xarray[11+ ( y*3 )]^temp[3+y]) );


                    state[1+y] = (byte) ( (xarray[2+ ( y*3 )]^temp[0+y]) ^ (xarray[5+ ( y*3 )]^xarray[4+ ( y*3 )]^xarray[3+ ( y*3 )]) ^ (xarray[8+ ( y*3 )]^xarray[6+ ( y*3 )]^temp[2+y]) ^ (xarray[11+ ( y*3 )]^xarray[10+ ( y*3 )]^temp[3+y]) );


                    state[2+y] = (byte) ( (xarray[2+ ( y*3 )]^xarray[1+ ( y*3 )]^temp[0+y]) ^ (xarray[5+ ( y*3 )]^temp[1+y]) ^ (xarray[8+ ( y*3 )]^xarray[7+ ( y*3 )]^xarray[6+ ( y*3 )]) ^ (xarray[11+ ( y*3 )]^xarray[9+ ( y*3 )]^temp[3+y]) );


                    state[3+y] = (byte) ( (xarray[2+ ( y*3 )]^xarray[0+ ( y*3 )]^temp[0+y]) ^ (xarray[5+ ( y*3 )]^xarray[4+ ( y*3 )]^temp[1+y]) ^ (xarray[8+ ( y*3 )]^temp[2+y]) ^ (xarray[11+ ( y*3 )]^xarray[10+ ( y*3 )]^xarray[9+ ( y*3 )]) );
            }



    }

    private static byte XTime (byte Tm)
    {
            // m(x) is x8 + x4 + x3 + x1 + x0

            byte temp = Tm;

            temp &= (byte)0x80;

            Tm <<= 1;

            if (temp == (byte)0x80)
            {
                    Tm = (byte)(Tm^0x1b);
            }
            return Tm;
    }

    private static void AddRoundKey (byte[] state, byte[] key, byte current_round)
    {
            short count = (short) (current_round*16);

            for(byte i = 0; i<16; i++)
            {
                    state[i] ^= key[count];
                    count++;
            }
    }

    // GenerateKey function from fips draft 5.2 (page 22)
    private static void GenerateKey (byte[] cipherkey, byte[] key, byte Nk, byte Nr)
    {

            short i;


            for( i = (short)(0); i < (short)( Nk * 4 ); i++) 	//work with byte arrays (8) instead of word arrays (32)
            {
                    key[i] = cipherkey[i];
            }

            byte[] temp = new byte[4];

            byte temp2;			//for rotbyte, am not creating a separate function called RotByte coz i don't want to
                                            //initialize a new element each time it's executed
            byte count = (byte)0;	//counter for if condition below


            for ( i = (short)((Nk*4) - 4) ; i < (short)(16 * (Nr + 1) - 4) ; i += 4 )	//if we work with words, it would b 4, not 16
                                                                                                                    //for Nk = 4, i = 12 to 172 increment by 4
            {
                    //word temp = w[i-1];

                    for (byte j = 0; j<4; j++)
                    {
                            temp[j] = key[i];
                            i++;
                    }

                    //following : RotByte

                    //if(i mod Nk = 0)

                    if ( ( (i/4) % Nk ) == 0 )	// fips draft deals with words, which is 32 bytes. 8*4 = 32
                                                            // in the first loop (execution), fips draft => 4, our loop => 16.
                                                            //  16/4 = 4
                    {

                            temp2 = temp[0];

                            for (byte k = 0; k<3; k++)
                            {
                                    temp[k] = temp [ (k+1)%4 ];
                            }

                            temp[3] = temp2;

                            //following: ByteSub

                            for (byte ctr = 0; ctr<4; ctr++)
                            {
                                    temp[ctr] = ByteSub(temp[ctr]);
                            }


                            //following: XOR with RC
                            temp[0] ^= RC[count];	// fips draft => rcon[i/Nk] (page 22, line number 8 from figure 12 => i = Nk
                                                            // meaning i starts from 4, 5, 6, 7...
                                                            // line number 11 => if (i mod Nk = 0)
                                                            // meaning only 4, 8, 12, 16...
                                                            // [i/nk] is always 1, 2, 3, 4, 5...
                                                            // "count" is 1, 2, 3, 4, 5...
                                                            // we only need to XOR the first byte (temp has 4 bytes)

                            count++;


                    }

                    else if ( ( Nk == 8 ) && ( ( (i/4) % Nk ) == 4) )
                    {
                            for (byte ctr2 = 0; ctr2 < 4; ctr2++)
                            {
                                    temp[ctr2] = ByteSub(temp[ctr2]);
                            }
                    }

                    // w[i] = w[i-Nk] xor temp

                    for (byte ctr3 = 0; ctr3 < 4; ctr3++)
                    {
                                                                                                    //in the first loop (execution), now i is 16
                            key[i] = (byte) ( key[ i - (Nk*4) ] ^ temp[ctr3] ); 	//key[ i - (Nk*4) ] => 16 - (4*4) = 0
                            i++;
                    }


                    i-=8;		//in the first loop (execution), now i is 20. 20-8 = 12
                                    //second loop (execution), loop increment => i+=4, so i becomes 16

            }


    }

    private static void Cipher (byte[] state, byte[] key)
    {
            //Nk=4 and Nr=10

            AddRoundKey(state, key, (byte)0);


            for(byte rnd=1; rnd<10; rnd++)
            {
                    ByteSub(state);
                    ShiftRow3(state);
                    MixColumn2(state);
                    AddRoundKey(state, key, rnd);
            }


            ByteSub(state);
            ShiftRow3(state);
            AddRoundKey(state, key, (byte)10);

    }

    private static void InvCipher(byte[] state, byte[] key)
    {
            //Nk=4 and Nr=10

            AddRoundKey(state, key, (byte)10);

            for(byte rnd=9; rnd>0; rnd--)
            {
                    InvShiftRow(state);
                    InvByteSub(state);
                    AddRoundKey(state, key, rnd);
                    InvMixColumn(state);
            }

            InvShiftRow(state);
            InvByteSub(state);
            AddRoundKey(state, key, (byte)0);

    }

    protected static byte[] start(final byte[] myState, final byte[] b, final String encDEC)
    {

            byte[] state = new byte[16];

            System.arraycopy(myState, 0, state, 0, 16);


            //create the key bytes from the pass phrase; this will use int
            //	for demonstrative purposes only.
            //In the java card implementation this step is unnecessary 
            //	as the key will have been properly initialized.	 

            int len = b.length;
            if (len > cipherkey.length) 
                    len = cipherkey.length;

            System.arraycopy(b, 0, cipherkey, 0, len);


            GenerateKey(cipherkey, key, (byte)4, (byte)10);		//assume Nk=4, Nr=10


            if(encDEC.equals("encrypt"))
                    Cipher(state, key);				
            else
                    InvCipher(state, key);

            return state;

    }

	
    
}
