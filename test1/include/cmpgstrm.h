// cmpgstrm.h : interface of MPEG-2 and MPEG-1 system layer
//
// CMpegStream provides several MPEG operation functions on system layer, 
// such as parsing system header, managing multiple channels. Currently 
// this class only provides several simple facilities and is still under 
// developing, but already stand alone.
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

#if !defined CMPEGSTREAM_H
#define CMPEGSTREAM_H

#include "mpegdata.h"
#include "datatype.h"
#include "cmpvdeco.h"
#include "cobfbuf.h"

const long STREAM_BUF_SIZE=4096;

typedef struct mpegvideo_ch_status_tag
{
	CMpvDecoder *pMpvDecoder;
	int iStatus;	
} MVIDEOCHSTATUS;

typedef struct stream_status_tag
{
	int iStatus;
	bool bSystemStream[MAX_LAYER_NUM];  // Flag to mark if it is a system stream;
	bool bTwoStream;  // has second stream for Data Partitioning (DP) or SNR Scalability enhancment
} STREAMSTATUS;

class CMpegStream
{
// constructor and destructor
public:
	CMpegStream();
	~CMpegStream();

// attributes
protected:
	FILE *pfBaseFile;
	FILE *pfEnhanFile;
	char *psBaseFileName, *psEnhanFileName;
	COBitFileBuf BitBuf[MAX_LAYER_NUM];
	SYSTEMHEADER SystemHeader[MAX_LAYER_NUM];
	STREAMSTATUS StreamStatus;
	MVIDEOCHSTATUS VideoChStatus[MAX_VIDEOCH_NUM];
//	MAUDIOCHSTATUS AudioChStatus[MAX_AUDIOCH_NUM];
//	PRIVATECHSTATUS PrivateChStatus[MAX_PRIVATECH_NUM];

// operation
public:
	int Initialize(const char *psBaseLayerName, const char *psEnhanLayerName=NULL);
	void Reset(void);
	int TurnOnChannel(VIDEOCHNUM ChannelNum, VIDEOCHNUM EnhanChannelNum=VIDEOCH0);
	void TurnOffChannel(VIDEOCHNUM ChannelNum);
	CMpvDecoder *GetChannel(VIDEOCHNUM ChannelNum) {return VideoChStatus[ChannelNum].pMpvDecoder;};
	SYSTEMHEADER GetSystemHeader(LAYERTYPE LayerType=BASELAYER) { return SystemHeader[LayerType]; };
	bool IsSystemStream(LAYERTYPE LayerType=BASELAYER) { return StreamStatus.bSystemStream[LayerType]; };
	int GetStatus(void) { return StreamStatus.iStatus; };

protected:
	void NextStartCode(COBitFileBuf *pBitFileBuf);
	int ReadSystemHeader(LAYERTYPE LayerType);
	inline void MarkerBit(LAYERTYPE LayerType);
};


inline void CMpegStream::MarkerBit(LAYERTYPE LayerType)
{
#ifdef ERR_INFO
	if (!BitBuf[LayerType].GetByteBits(1))  // must be 1;
		StreamStatus.iStatus=STREAM_ERROR;
#else
	BitBuf[LayerType].GetByteBits(1); //'1'
#endif
}

#endif  //CMPEGSTREAM_H