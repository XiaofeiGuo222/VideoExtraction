// mpvdata.h : interface of data definition
//
// This file defines data for MPEG video sequence layer;
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

#if !defined MPVDATA_H
#define MPVDATA_H

#include "datatype.h"
#include "mpegdata.h"
#include "cstrmbuf.h"


// constants
const DWORD VIDEO_STREAM_BUF_SIZE=2048;
const WORD FAST_IDCT_COEF_NUM=1024;
const WORD MIN_SEQUENCE_HEADER_SIZE=8;  //for MPEG-1;(64 BITS)
const WORD MIN_GOP_HEADER_SIZE=8;		//MPEG-1 and MPEG-2 are the same (59 BITS);
const WORD MIN_PICTURE_HEADER_SIZE=8;	//for MPEG-1 (62 BITS);
const WORD MIN_SLICE_HEADER_SIZE=4;		//MPEG-1 and MPEG-2 are the same (38 BITS), must be 4(not 5), otherwise use 38 bits;
const WORD MPEG2_MIN_SEQUENCE_HEADER_SIZE=18; // (144 BITS)
const WORD MPEG2_MIN_GOP_HEADER_SIZE=8; // (59 BITS)
const WORD MPEG2_MIN_PICTURE_HEADER_SIZE=16;  // (126 BITS)

// data types and constants defined for sequence layer;
const BYTE BLOCK_SIZE=8;
const BYTE BLOCK_ITEM_NUM=BLOCK_SIZE*BLOCK_SIZE;
// profiles and levels;
const BYTE PROFILE_422=133;  // 128+5;
const BYTE MAIN_LEVEL=8;
// scalable_mode
const BYTE SC_NONE=0;
const BYTE SC_DP=1;
const BYTE SC_SPAT=2;
const BYTE SC_SNR=3;
const BYTE SC_TEMP=4;
// picture coding type
const BYTE I_TYPE=1;
const BYTE P_TYPE=2;
const BYTE B_TYPE=3;
const BYTE D_TYPE=4;
// picture structure
const BYTE TOP_FIELD=1;
const BYTE BOTTOM_FIELD=2;
const BYTE FRAME_PICTURE=3;
// macroblock type, 0 for invalid type;
const char MACROBLOCK_INTRA=1;
const char MACROBLOCK_PATTERN=2;
const char MACROBLOCK_MOTION_BACKWARD=4;
const char MACROBLOCK_MOTION_FORWARD=8;
const char MACROBLOCK_QUANT=16;
const char SPATIAL_TEMPORAL_WEIGHT_CODE_FLAG=32;
const char PERMITTED_SPATIAL_TEMPORAL_WEIGHT_CLASS=64;
// motion_type, 0 and -1 for invalid type;
const char MC_FIELD=1;
const char MC_FRAME=2;
const char MC_16X8=2;
const char MC_DMV=3;
// mv_format
const BYTE MV_FIELD=0;
const BYTE MV_FRAME=1;
// chroma_format
const BYTE CHROMA420=1;
const BYTE CHROMA422=2;
const BYTE CHROMA444=3;
// interpolation mode
const BYTE DUPLICATE_MODE=1;
const BYTE DOUBLE_MODE1=2;
const BYTE DOUBLE_MODE2=3;
const BYTE PROGRESSIVE_MODE=4;
const BYTE INTERLACE_MODE=5;
const BYTE FIELD_MODE=6;
// Ohter constants
const BYTE ZIG_ZAG=0;
const BYTE MB_WEIGHT=32;
const BYTE MB_CLASS4=64;
const long INVALID_FRAME_NUM=-1;

// frame rate table
const double RESERVED=-1;
const double FRAME_RATE_TABLE[16]=
{
  0.0,
  ((23.0*1000.0)/1001.0),
  24.0,
  25.0,
  ((30.0*1000.0)/1001.0),
  30.0,
  50.0,
  ((60.0*1000.0)/1001.0),
  60.0,
  RESERVED,
  RESERVED,
  RESERVED,
  RESERVED,
  RESERVED,
  RESERVED,
  RESERVED
};

// zig-zag and alternate scan patterns
const BYTE SCAN[2][64]=
{
  { // Zig-Zag scan pattern;
    0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,
    12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,
    35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,
    58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63
  },
  { // Alternate scan pattern;
    0,8,16,24,1,9,2,10,17,25,32,40,48,56,57,49,
    41,33,26,18,3,11,4,12,19,27,34,42,50,58,35,43,
    51,59,20,28,5,13,6,14,21,29,36,44,52,60,37,45,
    53,61,22,30,7,15,23,31,38,46,54,62,39,47,55,63
  }
};

// default intra quantization matrix
const BYTE DEFAULT_INTRA_QUANTIZER_MATRIX[64]=
{
  8, 16, 19, 22, 26, 27, 29, 34,
  16, 16, 22, 24, 27, 29, 34, 37,
  19, 22, 26, 27, 29, 34, 34, 38,
  22, 22, 26, 27, 29, 34, 37, 40,
  22, 26, 27, 29, 32, 35, 40, 48,
  26, 27, 29, 32, 35, 40, 48, 58,
  26, 27, 29, 34, 38, 46, 56, 69,
  27, 29, 35, 38, 46, 56, 69, 83
};

// non-linear quantization coefficient table
const BYTE NON_LINEAR_QUANTIZER_SCALE[32]=
{
   0, 1, 2, 3, 4, 5, 6, 7,
   8,10,12,14,16,18,20,22,
  24,28,32,36,40,44,48,52,
  56,64,72,80,88,96,104,112
};

// ISO/IEC 13818-2 section 6.3.6 sequence_display_extension()
const long INVERSE_TABLE_6_9[8][4]=
{
  {117504, 138453, 13954, 34903}, /* no sequence_display_extension */
  {117504, 138453, 13954, 34903}, /* ITU-R Rec. 709 (1990) */
  {104597, 132201, 25675, 53279}, /* unspecified */
  {104597, 132201, 25675, 53279}, /* reserved */
  {104448, 132798, 24759, 53109}, /* FCC */
  {104597, 132201, 25675, 53279}, /* ITU-R Rec. 624-4 System B, G */
  {104597, 132201, 25675, 53279}, /* SMPTE 170M */
  {117579, 136230, 16907, 35559}  /* SMPTE 240M (1987) */
};

// compressed information;
typedef struct MB_block_tag
{
	short shDCTBlock[12][BLOCK_ITEM_NUM];
	BYTE nMBType;
	BYTE nQuantizerScale;
	short shPMV[2][2][2];
	BYTE nMotionVerticalFieldSelect[2][2];
} MBBLOCK;

typedef short DCTBLOCK [BLOCK_ITEM_NUM];

typedef struct compressedpic_tag
{
	WORD wMBWidth;
	WORD wMBHeight;
	BYTE *pnMBType;
	BYTE *pnQuantizerScale;
	short *pshSet1FwdPMVx;  //shPMV[0][0][0];
	short *pshSet1FwdPMVy; //shPMV[0][0][1];
	short *pshSet1BackPMVx;  //shPMV[0][1][0];
	short *pshSet1BackPMVy; //shPMV[0][1][1];
	short *pshSet2FwdPMVx;  //shPMV[1][0][0];
	short *pshSet2FwdPMVy; //shPMV[1][0][1];
	short *pshSet2BackPMVx;  //shPMV[1][1][0];
	short *pshSet2BackPMVy; //shPMV[1][1][1];
	BYTE *pnMotionVerticalFieldSelect00;
	BYTE *pnMotionVerticalFieldSelect01;
	BYTE *pnMotionVerticalFieldSelect10;
	BYTE *pnMotionVerticalFieldSelect11;
	WORD wYBlockWidth;
	WORD wYBlockHeight;
	DCTBLOCK *pshYDCTBlock;
	WORD wCbBlockWidth;
	WORD wCbBlockHeight;
	DCTBLOCK *pshCbDCTBlock;
	WORD wCrBlockWidth;
	WORD wCrBlockHeight;
	DCTBLOCK *pshCrDCTBlock;
} COMPRESSEDPIC;

// Data structure
typedef struct sequence_header_tag
{
	WORD wHorizontalSize;   //frame horizontal size;
	WORD wVerticalSize;		//frame vertical size;
	BYTE nPelAspectRatio;
	BYTE nFrameRate;
	DWORD dwBitRate;
	DWORD dwVbvBufferSize;
	BYTE nConstrainedParametersFlag;
	// currently we just overlook user data;
} SEQUENCEHEADER;

typedef struct sequence_extension_tag
{  // ISO/IEC 13818-2 section 6.2.2.3:  sequence_extension()
	BYTE nProfileAndLevelIndication;
	BYTE nProgressiveSequence;
	BYTE nChromaFormat;
	BYTE nLowDelay;
	WORD wFrameRateExtension_n;
	WORD wFrameRateExtension_d;
} SEQUENCEEXTENSION;

typedef struct sequence_display_extension_tag
{  // ISO/IEC 13818-2 section 6.2.2.4:  sequence_display_extension()
	BYTE nVideoFormat;
	BYTE nColorDescription;
	BYTE nColorPrimaries;
	BYTE nTransferCharacteristics;
	BYTE nMatrixCoefficients;
	WORD wDisplayHorizontalSize;
	WORD wDisplayVerticalSize;
} SEQUENCEDISPLAYEXT;

typedef struct sequence_scalable_extension_tag
{  // ISO/IEC 13818-2 section 6.2.2.5: sequence_scalable_extension() header
	BYTE nScalableMode;
	BYTE nLayerID;
	WORD wLowerLayerPredHSize;  // Lower layer prediction horizontal size
	WORD wLowerLayerPredVSize;  // Lower layer prediction vertical size
	BYTE nHorizontalSubsamplingFactor_m;
	BYTE nHorizontalSubsamplingFactor_n;
	BYTE nVerticalSubsamplingFactor_m;
	BYTE nVerticalSubsamplingFactor_n;
} SEQUENCESCALABLEEXT;

typedef struct group_of_picture_header_tag
{  // ISO/IEC 13818-2 section 6.2.2.6: group_of_pictures_header()
	BYTE nDropFlag;
	BYTE nHour;
	BYTE nMinute;
	BYTE nSecond;
	BYTE nFrame;
	BYTE nClosedGOP;
	BYTE nBrokenLink;
} GOPHEADER;

// common for sequence header and picture extension;
typedef struct quant_matrix_tag
{
	BYTE nLoadIntraQuantizerMatrix;
	BYTE nIntraQuantizerMatrix[64];
	BYTE nLoadNonIntraQuantizerMatrix;
	BYTE nNonIntraQuantizerMatrix[64];
	BYTE nLoadChromaIntraQuantizerMatrix;  
	BYTE nChromaIntraQuantizerMatrix[64];  
	BYTE nLoadChromaNonIntraQuantizerMatrix; 
	BYTE nChromaNonIntraQuantizerMatrix[64];
} QUANTMATRIX;

typedef struct picture_header_tag
{  // ISO/IEC 13818-2 section 6.2.3: picture_header()
	short shTemporalReference;
	BYTE nPictureCodingType;
	WORD wVbvDelay;
	BYTE nFullPelForwardVector;
	BYTE nForwardFCode;
	BYTE nFullPelBackwardVector;
	BYTE nBackwardFCode;
} PICTUREHEADER;

typedef struct picture_extension_tag
{  // ISO/IEC 13818-2 section 6.2.3.1: picture_coding_extension() header
	BYTE nFCode[2][2];
	BYTE nIntraDCPrecision;
	BYTE nPictureStructure;
	BYTE nTopFieldFirst;
	BYTE nFramePredFrameDCT;
	BYTE nConcealmentMotionVectors;
	BYTE nQScaleType;
	BYTE nIntraVlcFormat;
	BYTE nAlternateScan;
	BYTE nRepeatFirstField;
	BYTE nChroma420Type;
	BYTE nProgressiveFrame;
	BYTE nCompositeDisplayFlag;
	BYTE nVAxis;
	BYTE nFieldSequence;
	BYTE nSubCarrier;
	BYTE nBurstAmplitude;
	BYTE nSubCarrierPhase;
} PICTUREEXTENSION;

typedef struct picture_display_extension_tag
{  // ISO/IEC 13818-2 section 6.2.3.3: picture_display_extension() header
	long lFrameCenterHorizontalOffset[3]; //range: -32768 to 32767;
	long lFrameCenterVerticalOffset[3]; //range: -32768 to 32767;
} PICTUREDISPLAYEXT;

typedef struct picture_spatial_scalable_extension_tag
{  // ISO/IEC 13818-2 section 6.2.3.5: picture_spatial_scalable_extension() header
	short shLowerLayerTemporalReference;
	short shLowerLayerHorizontalOffset; //range: -16384 to 16383;
	short shLowerLayerVerticalOffset; //range: -16384 to 16383;
	BYTE nSpatialTemporalWeightCodeTableIndex;
	BYTE nLowerLayerProgressiveFrame;
	BYTE nLowerLayerDeinterlacedFieldSelect;
} PICTURESPATIALEXT;

typedef struct copyright_extension_tag
{  // ISO/IEC 13818-2 section 6.2.3.6: copyright_extension() header
	BYTE nCopyrightFlag;
	BYTE nCopyrightIdentifier;
	BYTE nOriginalOrCopy;
	DWORD dwCopyrightNumber1;
	DWORD dwCopyrightNumber2;
	DWORD dwCopyrightNumber3;
} COPYRIGHTEXT;

typedef struct header_data_tag
{
	bool bGOPValid;
	// for sequence extension headers
	bool bSequenceExtension;
	bool bSequenceDisplayExt;
	bool bSequenceScalableExt;
	// for picture extension headers;
	bool bPictureExtension;
	bool bPictureDisplayExt;
	bool bPictureSpatialExt;
	bool bCopyrightExt;
	bool bQuantMatrixExt;
	// headers;
	SEQUENCEHEADER SequenceHeader;
	SEQUENCEEXTENSION SequenceExtension;
	SEQUENCEDISPLAYEXT SequenceDisplayExt;
	SEQUENCESCALABLEEXT SequenceScalableExt;
	GOPHEADER GOPHeader;
	QUANTMATRIX QuantMatrix;
	PICTUREHEADER PictureHeader;
	PICTUREEXTENSION PictureExtension;
	PICTUREDISPLAYEXT PictureDisplayExt;
	PICTURESPATIALEXT PictureSpatialExt;
	COPYRIGHTEXT CopyrightExt;
} HEADERDATA;

typedef struct decoder_info_tag
{
	bool bTwoStream;  // has second stream for Data Partitioning (DP) or SNR Scalability enhancment;
	bool bSpeedFlag; //higher speed but lower quality;
	int iStatus;
} DECODERINFO;

typedef struct picture_info_tag
{
	// picture variables;
	bool bSecondField;  //to indicate if current field is the second field of the same picture (frames are always first fields);	
	PACKETPOS CurPicturePckPos;//packet position;
	long lCurPicturePckOffset;//packet position, not include picture_start_code(four bytes);
	// GOP related variables;
	long lUnwrappedFrameNum; //associated with bGOPValid, for counting picture number;
} PICTUREINFO;

typedef struct picture_record_tag
{
	bool bGOPValid;
	PICTUREINFO PicInfo;
	GOPHEADER GOPHeader;
} PICTURERECORD;

typedef struct coding_picture_parameter_tag
{
	WORD wCodedFrameWidth; // Y component row increasement;
	WORD wCodedFrameHeight; // Y component (row) height;
	WORD wCodedChromaWidth; // Cb/Cr component row increasement;
	WORD wCodedChromaHeight; // Cb/Cr component (row) height;
	WORD wMBWidth; // MB row increasement;
	WORD wMBHeight; // MB (row) height;
} CODINGPICPARA;

typedef struct decoder_status_tag
{
	int iMpvDecoderStatus;
	int iBaseParserStatus;
	int iEnhanParserStatus;
	int iPicDecoderStatus;
} DECODERSTATUS;

#endif  //MPVDATA_H