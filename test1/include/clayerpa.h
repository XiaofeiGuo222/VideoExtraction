// clayerpa.h : interface of MPEG-2 and MPEG-1 video sequence
//
// This class provides a set of MPEG video (for one layer) sequence 
// operation functions for sequence, GOP, and picture syntax parsing.
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

#if !defined CLAYERPARSER_H
#define CLAYERPARSER_H

#include "datatype.h"
#include "mpegdata.h"
#include "cstrmbuf.h"
#include "mpvdata.h"

const GOPHEADER DEFAULT_GOP_HEADER={0,0,0,0,0,0,0};

class CLayerParser
{
// constructor and destructor
public:
	CLayerParser();
	~CLayerParser();

// attributes
protected:
	FILE *pfFile;
	CStreamBuf StreamBuf;
	HEADERDATA HeaderData;
	// sequence related variables;
	long lMaxFrameNum;
	long lMinFrameNum;
	long lAvgPictureScale;
	long lMinPictureSize;
	double dfFrameRate; // variables derived from normative elements;
	// flags;
	bool bMpeg2;
	// status;
	VIDEOCHNUM VideoChNum;
	PICTUREINFO CurPicInfo;
	int iParserStatus;

// operation
public:
	int Initialize(const char *psFileName, VIDEOCHNUM ChannelNum, DWORD dwBufSize=VIDEO_STREAM_BUF_SIZE);
	int Initialize(const char *psFileName, bool bIsSystemStream, VIDEOCHNUM ChannelNum, DWORD dwBufSize=VIDEO_STREAM_BUF_SIZE);
	void Reset(void);
	int PrepareRandomAccess(void);
	int GetStatus(void) { return iParserStatus; };
	inline void ClearStatus(void);
	//sequential access;
	int MoveToNextPicture(void);
	int MoveToPreviousPicture(void);
	int ResetCurPicture(void);
	int MoveToNextFrame(void);
	int MoveToPreviousFrame(void);
	int ResetCurFrame(void);
	// random access;
	int JumpToHeadingPicture(void);
	int JumpToEndingPicture(void);
	int JumpToFrame(DWORD dwFrameNum); //use offset from its own layer's lMinFrameNum;
	int UpdateGOPInfo(void);
	inline int GetCurFrameNum(long *plCurFrameNum); //use offset from lMinFrameNum, different from GetFrameNum();
	inline int GetMaxFrameNum(long *plMaxFrameNum); //use offset from lMinFrameNum
	// picture record and restore;
	inline void RecordCurPicture(PICTURERECORD *pPicRecord);
	inline int RestorePicture(const PICTURERECORD *pPicRecord);

	// Get structured data from LayerParser;
	CStreamBuf *GetLayerParserBuf(void) { return &StreamBuf; };
	const HEADERDATA *GetHeaderData(void) { return &HeaderData; };
	const PICTUREINFO *GetCurPicInfo(void) { return &CurPicInfo; };
	//Get information of video stream;
	bool IsMpeg2(void) { return bMpeg2; };
	inline void GetProfileLevel(short *pshProfile, short *pshLevel);
	// for advanced user;
	inline int SetMinPicSize(long lNewMinPicSize);

protected:
	void NextStartCode(void);
	void PreviousStartCode(void);
	void SetDefaultStatus(void); // only for constructor and reset
	inline void MarkerBit(void);
	int TestStream(bool *pbIsSystemStream);
	// sequence and GOP header parsers
	int ReadSequenceHeader(void);
	int ReadGOPHeader(void);
	int ReadExtensionAndUserDataV(void);
	int ReadSequenceExtension(void);
	int ReadSequenceDisplayExtension(void);
	int ReadSequenceScalableExtension(void);
	// picture header parsers
	int ReadPictureHeader(void);
	int ReadExtensionAndUserDataP(void);
	int ReadQuantMatrixExtension(void);
	int ReadPictureExtension(void);
	int ReadPictureDisplayExtension(void);
	int ReadPictureSpatialScalableExtension(void);
	int ReadPictureTemporalScalableExtension(void);
	int ReadCopyrightExtension(void);
	inline void ReadExtraBitInfo(void);
	inline void ReadUserData(void);
	// access operations
	int GetFrameNum(long *plCurFrameNum);
	int RestorePicture(const PACKETPOS &PictureBasePckPos, long lPictureBasePckOffset, char cFieldValue=-1);
	int LocateToNextPicture(char cDefaultField, bool bDefaultInvalidGOP, long lDefaultUnwrappedNum);
	void LocateToNextPicUpdate(bool bGOPChanged, char cDefaultField,
								bool bDefaultInvalidGOP, long lDefaultUnwrappedNum);
	int LocateCurrentPicture(char cDefaultField, bool bInvalidGOP, long lUnwrappedNum);
	void LocateCurrentPicUpdate(bool bGOPChanged, char cDefaultField,
								bool bInvalidGOP, long lUnwrappedNum);
	void MoveToNextPicUpdate(bool bGOPChanged, BYTE nPreviousPicStructure);
	void MoveToPreviousPicUpdate(bool bGOPChanged);
	int CurFrameGOPUpdate(void);
};

//////////////////////////////////////
//inline public functions:

inline void CLayerParser::ClearStatus(void)
{
	if (iParserStatus!=NOT_USED)  // NOT_USED CAN NOT be erased;
	{
		iParserStatus=MDC_SUCCESS;
//		StreamBuf.ClearStatus();
	}
}

inline void CLayerParser::GetProfileLevel(short *pshProfile, short *pshLevel)
{
	// special case for 422 VideoStatus.iProfile & VideoStatus.iLevel must be made;
	if((HeaderData.SequenceExtension.nProfileAndLevelIndication>>7) & 1)
	{ // escape bit of HeaderData.SequenceExtension.nProfileAndLevelIndication set;
		// 4:2:2 Profile @ Main Level;
		if((HeaderData.SequenceExtension.nProfileAndLevelIndication&15)==5)
		{
			*pshProfile=PROFILE_422;
			*pshLevel=MAIN_LEVEL;  
		}
		else
		{	//undefined;
			*pshProfile=0;
			*pshLevel=0;
		}
	}
	else //default value of nProfileAndLevelIndication (for MPEG-1) is 0;
	{
		*pshProfile=HeaderData.SequenceExtension.nProfileAndLevelIndication >> 4;// Profile is upper nibble;
		*pshLevel=HeaderData.SequenceExtension.nProfileAndLevelIndication & 0xF;// Level is lower nibble;
	}
}

 // use offset from lMinFrameNum, different from GetFrameNum();
inline int CLayerParser::GetCurFrameNum(long *plCurFrameNum)
{
	int iStatus;

	StreamBuf.ClearStatus();
	iStatus=GetFrameNum(plCurFrameNum);
	if (iStatus!=MDC_SUCCESS)
	{ // UNSUCCESS or ERROR;
		return iStatus;
	}
	*plCurFrameNum-=lMinFrameNum;
	if (*plCurFrameNum<0)
	{
		iParserStatus=STREAM_ERROR;
		return UNSUCCESS;
	}
	return MDC_SUCCESS;
}

// if random access can not be supported, lMaxFrameNum will be smaller than zero;
inline int CLayerParser::GetMaxFrameNum(long *plMaxFrameNum)
{
	int iStatus;

	if (lMaxFrameNum<0)
		iStatus=PrepareRandomAccess();
	*plMaxFrameNum=lMaxFrameNum-lMinFrameNum; // convert to offset;
	return iStatus;
}
	
// picture record and restore;
inline void CLayerParser::RecordCurPicture(PICTURERECORD *pPicRecord)
{
	pPicRecord->PicInfo=CurPicInfo;
	pPicRecord->bGOPValid=HeaderData.bGOPValid;
	if (HeaderData.bGOPValid)
		pPicRecord->GOPHeader=HeaderData.GOPHeader;
}

inline int CLayerParser::RestorePicture(const PICTURERECORD *pPicRecord)
{
	int iStatus;

	iStatus=RestorePicture(pPicRecord->PicInfo.CurPicturePckPos,pPicRecord->PicInfo.lCurPicturePckOffset,
					pPicRecord->PicInfo.bSecondField);
	if (iStatus!=MDC_SUCCESS)
		return iStatus;
	CurPicInfo.lUnwrappedFrameNum=pPicRecord->PicInfo.lUnwrappedFrameNum;
	HeaderData.bGOPValid=pPicRecord->bGOPValid;
	if (pPicRecord->bGOPValid)
		HeaderData.GOPHeader=pPicRecord->GOPHeader;
	return MDC_SUCCESS;
}

////////////////////////////////////////////////
// advanced user facilities; (public functions)

inline int CLayerParser::SetMinPicSize(long lNewMinPicSize)
{
	if (lNewMinPicSize<MIN_GOP_HEADER_SIZE)
	{ //lMinPictureSize must be not smaller than MIN_GOP_HEADER_SIZE;
		iParserStatus=USE_ERROR;
		return UNSUCCESS;
	}
	lMinPictureSize=lNewMinPicSize;
	return MDC_SUCCESS;
}

//////////////////////////////
// inline protected functions:

inline void CLayerParser::MarkerBit(void)
{
#ifdef ERR_INFO
	if (!StreamBuf.GetByteBits(1))  // must be 1;
	{
		iParserStatus=STREAM_ERROR;
	}
#else
	StreamBuf.GetByteBits(1); //'1'
#endif
}

// decode extra bit information, ISO/IEC 13818-2 section 6.2.3.4.
inline void CLayerParser::ReadExtraBitInfo(void)
{
	while (StreamBuf.GetByteBits(1))
		StreamBuf.GetByteBits(8);
}

// ISO/IEC 13818-2  sections 6.3.4.1 and 6.2.2.2.2;
inline void CLayerParser::ReadUserData(void)
{
	// skip ahead to the next start code;
	NextStartCode();
}

#endif //CLAYERPARSE_H