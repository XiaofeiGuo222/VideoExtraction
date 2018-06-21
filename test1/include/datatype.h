// datatype.h : Defination of constants and data types
//
// many global data types and constants used in OOMPEG are 
// defined in this file.
//
//*****************************************************************************
// 
//					MPEG Developing Classes
//
// Copyright (C) 1998, Vision and Neural Networks Laboratory, Computer Science 
// Department, Wayne State University, All Rights Reserved; 
//
//					Disclaimer of Warranty
//
// MPEG Developing Classes, both binary and source (hereafter, Software)
// is copyrighted by Vision and Neural Networks Laboratory, Computer Science 
// Department, Wayne State University (WSU), and ownership remains with WSU.
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files to use, copy,
// and distribute the software, provided that no charge is associated with 
// such copies and that the copyright notice and this statement appears on 
// all copies.
//
// THE SOFTWARE IS AVAILABLE TO THE USER ON AN "AS IS" BASIS, WITHOUT WARRANTY
// OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING, WITHOUT LIMITATION, THE 
// IMPLIED WARRANTIES OR MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
// IN NO EVENT SHALL THE COPYRIGHT-HOLDER BE LIABLE FOR ANY CLAIMS, DAMAGES, OR
// OTHER LIABILITIES OF ANY KIND WHATSOEVER ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. THIS 
// DISCLAIMAER OF WARRANTY EXTENDS TO THE USER OF THIS SOFTWARE. ALSO THE WSU 
// DOES NOT REPRESENT OR WARRANT THAT THE SOFTWARE FURNISHED HEREUNDER ARE FREE OF
// INFRINGMENT OF ANY THIRD-PARTY PATENTS.
//
// Many patents of MPEG-1 and MPEG-2 are general enough such that commercial
// implementations, including shareware, are unavoidable subject to royalty fees to
// patent holders, regardless of implementation design.
//
//
// Vision and Neural Networks Laboratory,
// Computer Science Department,
// Wayne State University,
// Detroit, MI 48202.
// Dongge Li(dil@cs.wayne.edu) and Ishwar K. Sethi(isethi@oakland.edu).
//
/////////////////////////////////////////////////////////////////////////////

#if !defined DATATYPE_H
#define DATATYPE_H


#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
#endif
#define WB "wb"
#define RB "rb"
typedef unsigned char BYTE;   /*  8  bits  */
typedef unsigned short WORD;  /*  16 bits  */
typedef unsigned long DWORD;  /*  32 bits  */


const WORD MAX_PATH_LENGTH=244;

// status information
const int MDC_SUCCESS=1; //represents both NO_ERROR and SUCCESS, must larger than 0;
const int GENERAL_ERROR=-2;
const int USE_ERROR=-3;
const int FILE_ERROR=-4;
const int MEM_ERROR=-5;
const int STREAM_ERROR=-6;
const int BUF_ERROR=-7; // buffer operation error;
const int READ_ERROR=-8; // read failed or buffer error during reading;
const int NOT_SUPPORT=-9;
const int POTENTIAL_ERROR=-10;
const int UNMATCH_LAYERS=-21;  // for two stream
const int OUT_OF_SYNC=-22;  // for two stream
// not fatal status;
const int UNSUCCESS=0;  // USUALLY NOT FATAL
const int NOT_FATAL=UNSUCCESS;  // NOT FATAL ERROR
// channel status;
const int NOT_USED=-127;
//for cstreambuf
const int ENDLESS_WARNING=-126;

// Output Type
const BYTE Y=0;
const BYTE U=1;
const BYTE V=2;
const BYTE YUV=3;
const BYTE RGB=4;
const BYTE BMP=5;
const BYTE TGA=6;
const BYTE SIF=7;
const BYTE PPM=8;

const BYTE TGA_HEADER_LENGTH=14;
const BYTE TGA_HEADER24[TGA_HEADER_LENGTH]={0,0,2,0,0,0,0,0,0,0,0,0,24,32};
const BYTE MAX_PPM_HEADER_LENGTH=50;
const BYTE TRUE_BMP_HEADER_LENGTH=54;
// The following need to be calculated for CONST_BMP_HEADER:
//  1. CONST_BMP_HEADER[2,3,4,5]=Row*Col+54;
//  2. CONST_BMP_HEADER[18,19,20,21]=Col;
//  3. CONST_BMP_HEADER[22,23,24,25]=Row;
//  4. CONST_BMP_HEADER[34,35,36,37]=Row*Col;
const BYTE CONST_BMP_HEADER[TRUE_BMP_HEADER_LENGTH]= {0x42,0x4d, 0,0,0,0,
								0,0, 0,0, 54,0,0,0,
								/*InfoHeader*/40,0,0,0, 0,0,0,0, 0,0,0,0,
								1,0, 24,0, 0,0,0,0, 0,0,0,0, 200,0,0,0,
								200,0,0,0, 0,0,0,0, 0,0,0,0};


////////////////////////////////////////////////////////////////////
// The BMP header is organized as follows:
// 
// typedef struct Tbitmap_header_tag
// {
//    WORD  bfType;
//    DWORD bfSize; 
//    WORD  bfReserved1;
//    WORD  bfReserved2;
//    DWORD bfOffBits;
// } TBITMAPHEADER;
//
// typedef struct bitmap_info_header_tag
// {
//    DWORD biSize;
//    DWORD biWidth;
//    DWORD biHeight;
//    WORD  biPlanes;
//    WORD  biBitCount;
//    DWORD biCompress;
//    DWORD biSizeImage;
//    DWORD biXPelsPerMeter;
//    DWORD biYPelsPerMeter;
//    DWORD biClrUsed;
//    DWORD biClrImportant; 
// } TBITMAPINFOHEADER;
//
// typedef struct RGB_quad_tag
// {
//    BYTE rgbBlue;
//    BYTE rgbGreen;
//    BYTE rgbRed;
//    BYTE rgbReserved;
// } RGBQUAD;
//
// typedef struct TBMP_header_tag 
// {
//    TBITMAPHEADER BitMapHeader;
//    TBITMAPINFOHEADER BitMapInfoHeader;
//    RGBQUAD bmiColor[256];
// } TBMPHEADER;
//
/////////////////////////////////////////////////////////////////////

#endif //DATATYPE_H