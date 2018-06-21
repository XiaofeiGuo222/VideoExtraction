// mpegdata.h : Defination of constants and data types
//
// most global data types and constants used in MPEG system and layer
// are defined in this file.
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

#if !defined MPEGDATA_H
#define MPEGDATA_H

#include "datatype.h"


// system layer data types and constants;
const BYTE MAX_STREAM_NUM=51;  //not count reserved_id
const BYTE MAX_VIDEOCH_NUM=16;
const BYTE MAX_AUDIOCH_NUM=32;
const BYTE MAX_PADCH_NUM=1;
const BYTE MAX_PRIVATECH_NUM=2;
const BYTE MAX_RESERVECH_NUM=17;

const BYTE MAX_LAYER_NUM=2;
enum LAYERTYPE {BASELAYER, ENHANLAYER};

enum VIDEOCHNUM {VIDEOCH0,VIDEOCH1,VIDEOCH2,VIDEOCH3,VIDEOCH4,VIDEOCH5,VIDEOCH6,
				VIDEOCH7,VIDEOCH8,VIDEOCH9,VIDEOCH10,VIDEOCH11,VIDEOCH12,
				VIDEOCH13,VIDEOCH14,VIDEOCH15};
const BYTE VIDEO_STREAM_ID[16]= {0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6,
							0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed,
							0xee, 0xef};
enum AUDIOCHNUM {AUDIOCH0,AUDIOCH1,AUDIOCH2,AUDIOCH3,AUDIOCH4,AUDIOCH5,AUDIOCH6,
				AUDIOCH7,AUDIOCH8,AUDIOCH9,AUDIOCH10,AUDIOCH11,AUDIOCH12,AUDIOCH13,
				AUDIOCH14,AUDIOCH15,AUDIOCH16,AUDIOCH17,AUDIOCH18,AUDIOCH19,AUDIOCH20,
				AUDIOCH21,AUDIOCH22,AUDIOCH23,AUDIOCH24,AUDIOCH25,AUDIOCH26,AUDIOCH27,
				AUDIOCH28,AUDIOCH29,AUDIOCH30,AUDIOCH31};
const BYTE AUDIO_STREAM_ID[32]= {0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6,
								0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd,
								0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
								0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xdb, 
								0xdc, 0xdd, 0xde, 0xdf};
enum PRIVATECHNUM {PRIVATECH1,PRIVATECH2};
enum PADCHNUM {PADCH0};
enum RESERVECHNUM {RESERVESTR,RESERVECH0,RESERVECH1,RESERVECH2,RESERVECH3,RESERVECH4,
				RESERVECH5,RESERVECH6,RESERVECH7,RESERVECH8,RESERVECH9,RESERVECH10,
				RESERVECH11,RESERVECH12,RESERVECH13,RESERVECH14,RESERVECH15};

typedef struct pack_header_tag
{
	BYTE nHighSystemClockReference;
	DWORD dwLowSystemClockReference;
	DWORD dwMuxRate;
} PACKHEADER;

typedef struct packet_header_tag
{
	BYTE nStreamID;
	WORD wPacketLength;
	DWORD dwSTDBufferSize;
	DWORD dwPresentationTimeStampH;
	DWORD dwPresentationTimeStampL;
	DWORD dwDecodingTimeStampH;
	DWORD dwDecodingTimeStampL;
	WORD wPacketHeaderLength;   // not in the ISO/IEC 13818
} PACKETHEADER;

typedef struct system_header_tag
{
	WORD wHeaderLength;
	DWORD dwRateBound;
	BYTE nAudioBound;
	bool bFixedFlag;
	bool bCSPCFlag;
	bool bSystemAudioLockFlag;
	bool bSystemVideoLockFlag;
	BYTE nVideoBound;
	BYTE nStreamID[MAX_STREAM_NUM];
	bool bSTDBufferBoundScale[MAX_STREAM_NUM+2];//two extra space for 0xB8, 0xB9, to apply to all audio ,video
	WORD wSTDBufferSizeBound[MAX_STREAM_NUM+2];
	BYTE nUsedIDNum; // not in standard;
} SYSTEMHEADER;


// data types and constants defined in ISO 13818;
const DWORD PICTURE_START_CODE=0x100;
const DWORD SLICE_START_CODE_MIN=0x101;
const DWORD SLICE_START_CODE_MAX=0x1AF;
const DWORD USER_DATA_START_CODE=0x1B2;
const DWORD SEQUENCE_HEADER_CODE=0x1B3;
const DWORD SEQUENCE_ERROR_CODE=0x1B4;
const DWORD EXTENSION_START_CODE=0x1B5;
const DWORD SEQUENCE_END_CODE=0x1B7;
const DWORD GROUP_START_CODE=0x1B8;
// system code IDs;
const DWORD SYSTEM_START_CODE_MIN=0x1B9;
const DWORD SYSTEM_START_CODE_MAX=0x1FF;
const DWORD ISO_END_CODE=0x1B9;
const DWORD PACK_START_CODE=0x1BA;
const DWORD SYSTEM_START_CODE=0x1BB;
// extension start code IDs;
const BYTE SEQUENCE_EXTENSION_ID=1;
const BYTE SEQUENCE_DISPLAY_EXTENSION_ID=2;
const BYTE QUANT_MATRIX_EXTENSION_ID=3;
const BYTE COPYRIGHT_EXTENSION_ID=4;
const BYTE SEQUENCE_SCALABLE_EXTENSION_ID=5;
const BYTE PICTURE_DISPLAY_EXTENSION_ID=7;
const BYTE PICTURE_CODING_EXTENSION_ID=8;
const BYTE PICTURE_SPATIAL_SCALABLE_EXTENSION_ID=9;
const BYTE PICTURE_TEMPORAL_SCALABLE_EXTENSION_ID=10;
// other IDs;
const DWORD VIDEO_ELEMENTARY_STREAMID=0x1e0; // ID of video channel 0;

#endif  //MPEGDATA_H