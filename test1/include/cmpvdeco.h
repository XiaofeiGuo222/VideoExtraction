// cmpvdeco.h : interface of MPEG-2 and MPEG-1 video sequence
//
// CMpvDecoder provides a set of MPEG video operation functions to 
// access, parse, and decode MPEG video sequence. Becuase CMpvDecoder 
// can automatically choose to work on system layer or sequence layer,
// most users will only need CMpvDecoder for MPEG video operations.
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

#if !defined CMPVDECODER_H
#define CMPVDECODER_H

#include "clayerpa.h"
#include "cpicdeco.h"
#include "datatype.h"
#include "mpegdata.h"
#include "cstrmbuf.h"
#include "mpvdata.h"

class CMpvDecoder
{
// constructor and destructor
public:
	CMpvDecoder();
	~CMpvDecoder();

// attributes
protected:
	CLayerParser *pLayerParser[MAX_LAYER_NUM];
	CPicDecoder PicDecoder; // for base layer and SNR, data partitioning scalability;
	// compressed picture output buffer pointer;
	COMPRESSEDPIC *pCompressedPic;
	// status data
	DECODERINFO DecoderInfo;

private: // just for speedup;
	const HEADERDATA *pHeaderData[MAX_LAYER_NUM];
	const PICTUREINFO *pCurPicInfo[MAX_LAYER_NUM];

// operation
public:
	int Initialize(const char *psBaseFileName, const char *psEnhanFileName=NULL,
							VIDEOCHNUM BaseChannelNum=VIDEOCH0, VIDEOCHNUM EnhanChannelNum=VIDEOCH0,
							DWORD dwBaseBufSize=VIDEO_STREAM_BUF_SIZE, DWORD dwEnhanBufSize=VIDEO_STREAM_BUF_SIZE);
	int Initialize(const char *psBaseFileName, const char *psEnhanFileName, bool bIsBaseSystemStream, bool bIsEnhanSystemStream,
					VIDEOCHNUM BaseChannelNum, VIDEOCHNUM EnhanChannelNum,
					DWORD dwBaseBufSizee=VIDEO_STREAM_BUF_SIZE, DWORD dwEnhanBufSizee=VIDEO_STREAM_BUF_SIZE);
	void Reset(void);
	inline DECODERSTATUS GetStatus(void);
	inline void ClearStatus(void);
	inline void SetSpeed(bool bIsHighSpeed);
	//sequential access;
	int MoveToNextPicture(void);
	int MoveToPreviousPicture(void);
	int ResetCurPicture(void);
	int MoveToNextFrame(void);
	int MoveToPreviousFrame(void);
	int ResetCurFrame(void);
	// random access;
	int PrepareRandomAccess(void);
	int JumpToHeadingPicture(void);
	int JumpToEndingPicture(void);
	int JumpToFrame(DWORD dwFrameNum);
	inline int GetCurFrameNum(long *plCurFrameNum, LAYERTYPE LayerType=BASELAYER); //use offset from lMinFrameNum;
	inline int GetMaxFrameNum(long *plMaxFrameNum, LAYERTYPE LayerType=BASELAYER); //use offset from lMinFrameNum;
	inline int UpdataGOPInfo(LAYERTYPE LayerType=BASELAYER);

	// get deoded data;
	inline int DecodePicture(void);
	inline int DecodeSpatPic(BYTE *pnLowLayerFrameData[3], WORD wYRowIncr, WORD wChromaRowIncr);

	// Decoded Information Output Functions:
	inline long GetCurFrame(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,BYTE nOutputType);
	inline long GetCurPic(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,BYTE nOutputType);
	inline int StoreCurPic(const char *psOutFileName, BYTE nOutputType);
	inline int StoreCurFrame(const char *psOutFileName, BYTE nOutputType);
	inline int GetCurYUV(BYTE *pnOutData[3],WORD pwOutPicWidth[3],WORD pwOutPicHeight[3], bool bFrameOnly=false);
	inline int GetCurRGB(BYTE *pnOutData[3],WORD *pwOutPicWidth,WORD *pwOutPicHeight, bool bFrameOnly=false);

	//Get information of video stream;
	const HEADERDATA *GetMpvHeaders(LAYERTYPE LayerType=BASELAYER) { return pHeaderData[LayerType]; };
	inline void GetProfileLevel(short *pshProfile, short *pshLevel, LAYERTYPE LayerType=BASELAYER);
	void GetCodingPicPara(CODINGPICPARA *pCodingPicPara) { PicDecoder.GetCodingPicPara(pCodingPicPara); };
	inline bool IsMpeg2(LAYERTYPE LayerType=BASELAYER);

	//get compressed picture
	inline int GetMBBlockPic(MBBLOCK *pOutMBBlockPic,WORD *pwOutWidth,WORD *pwOutHeight);
	COMPRESSEDPIC *PrepareCompressedPic(void);
	void FreeCompressedPic(void); //reset and destructor will call it automatically;
	inline int GetCompressedPic(void);

	//////////////////////////////////////////////////////////////////
	//for advance users;
	int SetMinPicSize(long lNewMinPicSize, LAYERTYPE LayerType=BASELAYER);
	BYTE *GetBackRefFrameBuf(BYTE nComponent) { return PicDecoder.GetBackRefFrameBuf(nComponent); };
	BYTE *GetFwdRefFrameBuf(BYTE nComponent) { return PicDecoder.GetFwdRefFrameBuf(nComponent); };
	BYTE *GetCurFrameBuf(BYTE nComponent) { return PicDecoder.GetCurFrameBuf(nComponent); };
	//sequential access for each layer;
	int MoveToNextPicture(LAYERTYPE LayerType);
	int MoveToPreviousPicture(LAYERTYPE LayerType);
	int ResetCurPicture(LAYERTYPE LayerType);
	int MoveToNextFrame(LAYERTYPE LayerType);
	int MoveToPreviousFrame(LAYERTYPE LayerType);
	int ResetCurFrame(LAYERTYPE LayerType);
	// random access for each layer;
	int JumpToHeadingPicture(LAYERTYPE LayerType);
	int JumpToEndingPicture(LAYERTYPE LayerType);
	int JumpToFrame(DWORD dwFrameNum, LAYERTYPE LayerType);
	// record and restore picture for each layer;
	void RecordCurPicture(PICTURERECORD *pCurPicRecord, LAYERTYPE LayerType);
	int RestorePicture(const PICTURERECORD *pPicRecord, LAYERTYPE LayerType);

protected:
	int CompareTwoStreamInfo(void);
};

//////////////////////////////////////
//inline public functions:

inline DECODERSTATUS CMpvDecoder::GetStatus(void)
{
	DECODERSTATUS DecoderStatus;

	DecoderStatus.iMpvDecoderStatus=DecoderInfo.iStatus;
	DecoderStatus.iBaseParserStatus=pLayerParser[BASELAYER]->GetStatus();
	if (DecoderInfo.bTwoStream)
		DecoderStatus.iEnhanParserStatus=pLayerParser[ENHANLAYER]->GetStatus();
	else
		DecoderStatus.iEnhanParserStatus=MDC_SUCCESS;
	DecoderStatus.iPicDecoderStatus=PicDecoder.GetStatus();
	return DecoderStatus;
}

inline void CMpvDecoder::ClearStatus(void)
{
	if (DecoderInfo.iStatus!=NOT_USED)  // NOT_USED CAN NOT be erased;
	{
		DecoderInfo.iStatus=MDC_SUCCESS;
		pLayerParser[BASELAYER]->ClearStatus();
		if (DecoderInfo.bTwoStream)
			pLayerParser[ENHANLAYER]->ClearStatus();
		PicDecoder.ClearStatus();
	}
}

inline void CMpvDecoder::SetSpeed(bool bIsHighSpeed)
{
	DecoderInfo.bSpeedFlag=bIsHighSpeed;
	PicDecoder.SetSpeed(bIsHighSpeed);
}

// get picture information in random access;
inline int CMpvDecoder::GetCurFrameNum(long *plCurFrameNum, LAYERTYPE LayerType) //use offset from lMinFrameNum;
{
#ifdef DEBUG
	if (pLayerParser[LayerType]==NULL)
	{
		DecoderInfo.iStatus=USE_ERROR;
		return UNSUCCESS;
	}
#endif		
	return pLayerParser[LayerType]->GetCurFrameNum(plCurFrameNum);
}

inline int CMpvDecoder::GetMaxFrameNum(long *plMaxFrameNum, LAYERTYPE LayerType) //use offset from lMinFrameNum;
{
#ifdef DEBUG
	if (pLayerParser[LayerType]==NULL)
	{
		DecoderInfo.iStatus=USE_ERROR;
		return UNSUCCESS;
	}
#endif		
	return pLayerParser[LayerType]->GetMaxFrameNum(plMaxFrameNum);
}

inline int CMpvDecoder::UpdataGOPInfo(LAYERTYPE LayerType)
{
#ifdef DEBUG
	if (pLayerParser[LayerType]==NULL)
	{
		DecoderInfo.iStatus=USE_ERROR;
		return UNSUCCESS;
	}
#endif		
	if (pHeaderData[LayerType]->bGOPValid)
		return MDC_SUCCESS; //already up-to-date;
	else
		return pLayerParser[LayerType]->UpdateGOPInfo();
}

// decode picture data;
// even return value is MDC_SUCCESS, not fatal errors may still exist 
// and can be reported by DecoderInfo.iStatus; 
inline int CMpvDecoder::DecodePicture(void)
{
	// decode picture;
	return PicDecoder.DecodePicture();
}

// decode picture data;
// even return value is MDC_SUCCESS, not fatal errors may still exist 
// and can be reported by DecoderInfo.iStatus; 
inline int CMpvDecoder::DecodeSpatPic(BYTE *pnLowLayerFrameData[3], WORD wYRowIncr, WORD wChromaRowIncr)
{
#ifdef DEBUG
	if ( (wYRowIncr<pHeaderData[BASELAYER]->SequenceScalableExt.wLowerLayerPredHSize)
				||(wChromaRowIncr<(pHeaderData[BASELAYER]->SequenceScalableExt.wLowerLayerPredHSize>>1)) )
	{ //spatial scalable layer is always the base layer;
		DecoderInfo.iStatus=USE_ERROR;
		return UNSUCCESS;
	}
#endif
	// decode picture;
	return PicDecoder.DecodePicture(pnLowLayerFrameData,wYRowIncr,wChromaRowIncr);
}

inline long CMpvDecoder::GetCurFrame(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,BYTE nOutputType)
{
	return PicDecoder.GetCurFrame(pnOutData,pwOutPicWidth,pwOutPicHeight,nOutputType);
}

inline long CMpvDecoder::GetCurPic(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,BYTE nOutputType)
{
	return PicDecoder.GetCurPic(pnOutData,pwOutPicWidth,pwOutPicHeight,nOutputType);
}

inline int CMpvDecoder::StoreCurPic(const char *psOutFileName, BYTE nOutputType)
{
	return PicDecoder.StoreCurPic(psOutFileName,nOutputType);
}

inline int CMpvDecoder::StoreCurFrame(const char *psOutFileName, BYTE nOutputType)
{
	return PicDecoder.StoreCurFrame(psOutFileName,nOutputType);
}

inline int CMpvDecoder::GetCurYUV(BYTE *pnOutData[3],WORD pwOutPicWidth[3],WORD pwOutPicHeight[3], bool bFrameOnly)
{
	return PicDecoder.GetCurYUV(pnOutData, pwOutPicWidth, pwOutPicHeight, bFrameOnly);
}

inline int CMpvDecoder::GetCurRGB(BYTE *pnOutData[3],WORD *pwOutPicWidth,WORD *pwOutPicHeight, bool bFrameOnly)
{
	return PicDecoder.GetCurRGB(pnOutData, pwOutPicWidth, pwOutPicHeight, bFrameOnly);
}

inline void CMpvDecoder::GetProfileLevel(short *pshProfile, short *pshLevel, LAYERTYPE LayerType)
{
#ifdef DEBUG
	if (pLayerParser[LayerType]==NULL)
	{
		DecoderInfo.iStatus=USE_ERROR;
		*pshProfile=-1;
		*pshLevel=-1;
		return;
	}
#endif		
	pLayerParser[LayerType]->GetProfileLevel(pshProfile, pshLevel);
}

inline bool CMpvDecoder::IsMpeg2(LAYERTYPE LayerType)
{
#ifdef DEBUG
	if (pLayerParser[LayerType]==NULL)
	{
		DecoderInfo.iStatus=USE_ERROR;
		return false;
	}
#endif		
	return pLayerParser[LayerType]->IsMpeg2();
}

// get compressed picture;
// even return value is MDC_SUCCESS, not fatal errors may still exist 
// and can be reported by DecoderInfo.iStatus; 
inline int CMpvDecoder::GetMBBlockPic(MBBLOCK *pOutMBBlockPic,WORD *pwOutWidth,WORD *pwOutHeight)
{
	// parse picture;
	return PicDecoder.GetMBBlockPic(pOutMBBlockPic,pwOutWidth,pwOutHeight);
}

// get compressed picture;
// even return value is MDC_SUCCESS, not fatal errors may still exist 
// and can be reported by DecoderInfo.iStatus; 
inline int CMpvDecoder::GetCompressedPic(void)
{
	// parse picture;
	return PicDecoder.GetCompressedPic(pCompressedPic);
}

////////////////////////////////////////////////
// advanced user facilities; (public functions)

inline int CMpvDecoder::SetMinPicSize(long lNewMinPicSize, LAYERTYPE LayerType)
{
	if (pLayerParser[LayerType]==NULL)
	{
		DecoderInfo.iStatus=USE_ERROR;
		return UNSUCCESS;
	}
	return pLayerParser[LayerType]->SetMinPicSize(lNewMinPicSize);
}

#endif  //CMPVDECODER_H
