// cpicdedo.h : interface of MPEG-2/1 Video picture, slice, 
//                     macroblock block layer decoder;
//
// This class provides a set of MPEG picture decode functions
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

#if !defined CPICDECODER_H
#define CPICDECODER_H

#include <memory.h>
#include <string.h>
#include "datatype.h"
#include "mpegdata.h"
#include "cstrmbuf.h"
#include "mpvdata.h"
#include "clayerpa.h"


const BYTE TABLE_6_20[3] = {6,8,12};
const BYTE STWC_TABLE[3][4] = { {6,3,7,4}, {2,1,5,4}, {2,5,7,4} };
const BYTE STWCLASS_TABLE[9] = {0, 1, 2, 1, 1, 2, 3, 3, 4};

class CPicDecoder
{
// constructor and destructor
public:
	CPicDecoder();
	~CPicDecoder();

// attributes
protected:
	LAYERTYPE CurLayerType;
	DECODERINFO PicDecoderInfo;
	CStreamBuf *pStreamBuf[MAX_LAYER_NUM];
	const HEADERDATA *pHeaderData[MAX_LAYER_NUM];
	const PICTUREINFO *pCurPicInfo[MAX_LAYER_NUM];

	// pointers to picture buffer;
	BYTE *pnBackRefFrame[3];
	BYTE *pnFwdRefFrame[3];
	BYTE *pnAuxFrame[3];
	BYTE *pnCurFrame[3];
	// pointers for temporal data buffer;
	BYTE *pnComTempBuf[3];//used by several functions, and will be reallocated whenever a larger buffer needed;
	DWORD dwCurTempBufSize[3];
	// pointers for spatial scalability picture buffer
	short *pshLowerLayerTemp;
	// variables derived from normative elements
	WORD wCodedFrameWidth;
	WORD wCodedFrameHeight;
	WORD wCodedChromaWidth;
	WORD wCodedChromaHeight;
	bool bMpeg2[MAX_LAYER_NUM]; // get from LayerParser during initialization;
	// normative derived variables
	BYTE nMBBlockNum;
	WORD wMBWidth;
	WORD wMBHeight;
	DWORD dwMBMaxNum;  //Max MB Num for frame picture, it will be one half of this number for field picture;
	// IDCT coefficients;
	double dfIDCTCoef[BLOCK_SIZE][BLOCK_SIZE]; // for reference IDCT, cosine transform matrix for 8x1 IDCT
	short *pshFastIDCTCoef;  // for fast IDCT;
	// buffer for multiuse purposes
	BYTE *pnClip;
	// exception variable
	bool bWarningFlag;

private:
	BYTE pnBuf[1024];
	short pshFastIDCTBuf[FAST_IDCT_COEF_NUM];
	// slice data;
	BYTE nSliceVerticalPositionExtension;
	// slice/block data;
	BYTE nMBType[MAX_LAYER_NUM];
	BYTE nPriorityBreakpoint[MAX_LAYER_NUM];
	BYTE nQuantizerScale[MAX_LAYER_NUM];
	BYTE nIntraSlice[MAX_LAYER_NUM];
	short shBlock[MAX_LAYER_NUM][12][64];
	// block data;
	short shPMV[2][2][2];

// operation
public:
	int Initialize(CLayerParser *pBaseLayerParser, CLayerParser *pEnhanLayerParser);
	void Reset(void);
	int GetStatus(void) { return PicDecoderInfo.iStatus; };
	inline void ClearStatus(void);
	inline void SetSpeed(bool bIsHighSpeed) { PicDecoderInfo.bSpeedFlag=bIsHighSpeed; };
	// decode picture;
	int DecodePicture(void);
	int DecodePicture(BYTE *pnBaseFrameData[3], WORD wYRowIncr, WORD wChromaRowIncr);
	// get information;
	inline void GetCodingPicPara(CODINGPICPARA *pCodingPicPara);
	long GetCurFrame(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,BYTE nOutputType);
	long GetCurPic(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,BYTE nOutputType);
	int StoreCurPic(const char *psOutFileName, BYTE nOutputType);
	int StoreCurFrame(const char *psOutFileName, BYTE nOutputType);
	inline int GetCurYUV(BYTE *pnOutData[3],WORD pwOutPicWidth[3],WORD pwOutPicHeight[3], bool bFrameOnly);
	inline int GetCurRGB(BYTE *pnOutData[3],WORD *pwOutPicWidth,WORD *pwOutPicHeight, bool bFrameOnly);

	// other facilities
	inline void RelocateBuf(bool bSwapRefBuf);
	void DataSequential(BYTE *pnSourceData, WORD wDataWidth, WORD wDataHeight,
						long lIncreasement, BYTE *pnTargetData);
	int HorizontalInterpolation(BYTE *pnSourceData, WORD wDataWidth, WORD wDataHeight,
						long lIncreasement, BYTE *pnTargetData, BYTE nWorkMode);
	int VerticalInterpolation(BYTE *pnSourceData, WORD wDataWidth, WORD wDataHeight,
						long lIncreasement, BYTE *pnTargetData, BYTE nWorkMode);
	
	//compressed information;
	int GetMBBlockPic(MBBLOCK *pOutMBBlockPic,WORD *pwOutWidth,WORD *pwOutHeight);
	int GetCompressedPic(COMPRESSEDPIC *pOutCompressedPic);

protected: //call by above functions, CurLayerType must be set properly for them;
	void SetDefaultStatus(void);  //only for constructor and reset;
	int CompareTwoStreamInfo(void);
	void NextStartCode(CStreamBuf *pStreamBuf);
	inline int PrepareComTempBuf(BYTE nComponent, DWORD dwNewSize);
	// fast IDCT
	void InitializeFastIDCT(void);
	void FastIDCT(short *pshBlock);
	void IDCTRow(short *pshBlock);
	void IDCTCol(short *pshBlock);
	// reference IDCT
	void InitializeReferenceIDCT(void);
	void ReferenceIDCT(short *pshBlock);
	//Read header; all use CurLayerType!!!!	
	int ReadSliceHeader(void);
	inline void ReadExtraBitInfo(void);
	inline void MarkerBit(void);
	// spatial scalability
	void SpatialPrediction(BYTE *pnBaseFrameData[3], WORD wYRowIncr, WORD wChromaRowIncr, BYTE *pnDestFrame[3]);
	void MakeSpatialPredictionFrame(BYTE *pSourceFrm, WORD wSourceWidth,
									BYTE *pnTempFrm, short *pshTemp, BYTE *pnDestination,
									short shHOffset, short shVOffset, WORD wPredWidth,
									WORD wPredHeight, WORD wPicWidth, WORD wPicHeight,
									BYTE nAperture);
	void Deinterlace(BYTE *pnSource, WORD wSourceWidth, BYTE nFieldValue,
							  BYTE *pnDest, WORD wDestWidth, WORD wDestHeight, BYTE nAperture);
	void SubsampleVertical(BYTE *pnSource, WORD wSourceWidth, WORD wSourceHeight,
									short *pshDestination, WORD wDestWidth, WORD wDestHeight,
									BYTE nFactorM, BYTE nFactorN, BYTE nFieldValue,
									BYTE nIncLineNum);
	void SubsampleHorizontal(short *pshSource, BYTE *pnDestination, WORD wHOffset, 
									WORD wWidth, WORD wSourceWidth, WORD wDestWidth,
									WORD wHeight, BYTE nFactorM, BYTE nFactorN);
	// picture decoding
	void DecodePictureData(void);
	int MoveToNextSlice(DWORD &dwMBA, DWORD &dwMBAInc, short shDctDcPred[3]);
	int DecodeSlice(DWORD dwPicMBMaxNum,DWORD dwMBA,DWORD dwMBAInc,short shDctDcPred[3]);
	void MacroblockModes(BYTE *pnStwType, BYTE *pnStwClass, char *pcMotionType,
				BYTE *pnMotionVectorCount,BYTE *pnMvFormat,bool *pbDmv,bool *pbMvScale,char *pcDctType);
	void MacroblockModes(BYTE *pnStwType, char *pcDctType);
	void AddBlock(BYTE nComponent,WORD wMBCol,WORD wMBRow,char cDctType,bool bAddFlag);
	void DecodeSNRMacroblock(DWORD *pdwSNRMBA,DWORD *pdwSNRMBAInc,DWORD dwMBA,
							DWORD dwPicMBMaxNum,char *pcDctType);
	inline void ClearBlock(BYTE nComponent);
	void SumBlock(BYTE nComponent);
	void Saturate(short *pshCurBlock);
	void MotionCompensation(DWORD dwMBA,char cMotionType,BYTE nMotionVerticalFieldSelect[2][2],
							char cDmVector[2],BYTE nStwType,char cDctType);
	void SkipMacroblock(short shDctDcPred[3],char *pcMotionType,
							BYTE nMotionVerticalFieldSelect[2][2],BYTE *pnStwType);
	int DecodeMacroblock(BYTE *pnStwType, BYTE *pnStwClass, char *pcMotionType, 
							char *pcDctType,short *shDctDcPred, BYTE nMotionVerticalFieldSelect[2][2],
							char cDmVector[2]);

	// generic picture macroblock type processing functions
	BYTE GetIMBType(void);
	BYTE GetPMBType(void);
	BYTE GetBMBType(void);
	BYTE GetDMBType(void);
	// spatial picture macroblock type processing functions
	BYTE GetISpatialMBType(void);
	BYTE GetPSpatialMBType(void);
	BYTE GetBSpatialMBType(void);
	BYTE GetSNRMBType(void);

	void GetMacroblockType(void);
	char GetMotionCode(void);
	char GetDmVector(void);
	BYTE GetCodedBlockPattern(void);
	DWORD GetMacroblockAddressIncrement(void);
	short GetLumaDctDcDiff(void);
	short GetChromaDctDcDiff(void);

	//block decoding
	void DecodeMPEG1IntraBlock(BYTE nComponent, short shDctDcPred[]);
	void DecodeMPEG1NonIntraBlock(BYTE nComponent);
	void DecodeMPEG2IntraBlock(BYTE nComponent,short shDctDcPred[]);
	void DecodeMPEG2NonIntraBlock(BYTE nComponent);
	
	// motion
	void DecodeMPEG2MotionVector(char cDmVector[2],BYTE nMotionVerticalFieldSelect[2][2],
					BYTE nPredDirection,BYTE nMotionVectorCount,BYTE nMvFormat,BYTE nHorizontalRSize,
					BYTE nVerticalRSize,bool bDmv,bool bMvScale);
	void DecodeMPEG1MotionVector(short *pshPMVVector, char *pcDmVector, BYTE nHorizontalRSize,
					BYTE nVerticalRSize, bool bDmv, bool bMvScale, BYTE nFullPelVector);
	void DecodeMotionVector(short *pshPMVValue, BYTE nRSize, char cMotionCode,
					WORD wMotionResidual, BYTE nFullPelVector);
	void DualPrimeArithmetic(short shDMV[][2],char *pcDmVector,short shHorizontalMV,short shVerticalMV);
	void GetPredictionPic(WORD wMBCol, WORD wMBRow, char cMotionType,
					BYTE nMotionVerticalFieldSelect[2][2], char cDmVector[2], BYTE nStwType);
	void FormPrediction(BYTE *pnRefSource[],BYTE nSourceFieldValue,BYTE *pnDestination[],
					BYTE nDstFieldValue,WORD wPicIncreasement,WORD wBlkIncreasement,
					BYTE nPredBlkWidth,BYTE nPredBlkHeight,WORD wCol,WORD wRow,short shDH,
					short shDV,BYTE nAverageFlag);
	void FormComponentPrediction(BYTE *pnRefSource,BYTE *pnDestination,WORD wPicIncreasement,
					WORD wBlkIncreasement,BYTE nPredBlkWidth,BYTE nPredBlkHeight,
					WORD wCol,WORD wRow,short shDH,short shDV,BYTE nAverageFlag);
	//picture output
	long GetBMPPic(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,
					BYTE *pnSourceData[3], bool bFrameOnly=false);
	long GetTGAPic(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,
					BYTE *pnSourceData[3], bool bFrameOnly=false);
	long GetSIFPic(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,
					BYTE *pnSourceData[3], bool bFrameOnly=false);
	long GetPPMPic(BYTE *pnOutData,WORD *pwOutPicWidth,WORD *pwOutPicHeight,
					BYTE *pnSourceData[3], bool bFrameOnly=false);
	int StoreYUVPic(const char *psOutFileName,BYTE *pnSourceData[3], bool bFrameOnly=false);
	int StoreComponent(const char *psOutFileName,BYTE *pnSourceData,WORD wStoredPicWidth,
					WORD wStoredPicHeight,long lIncreasement);
	int GetYUVPic(BYTE *pnOutData[3],WORD pwOutPicWidth[3],WORD pwOutPicHeight[3],
					BYTE *pnSourceData[3], bool bFrameOnly=false);
	int GetRGBPic(BYTE *pnOutData[3],WORD *pwOutPicWidth,WORD *pwOutPicHeight,
					BYTE *pnSourceData[3], bool bFrameOnly=false);
	// get compressed information;
	int GetMBBlock(DWORD dwPicMBMaxNum,DWORD dwMBA,DWORD dwMBAInc,short shDctDcPred[3],
								 MBBLOCK *pOutMBBlockPic);
	int GetCompressedBlock(DWORD dwPicMBMaxNum,DWORD dwMBA,DWORD dwMBAInc,short shDctDcPred[3],
								 COMPRESSEDPIC *pOutCompressedPic);

// only for advanced users 
public:
	inline BYTE *GetCurFrameBuf(BYTE nComponent);
	inline BYTE *GetBackRefFrameBuf(BYTE nComponent);
	inline BYTE *GetFwdRefFrameBuf(BYTE nComponent);
};

////////////////////////////////////////////////////////////////////
//inline public functions:

inline void CPicDecoder::ClearStatus(void)
{
	if (PicDecoderInfo.iStatus!=NOT_USED)  // NOT_USED CAN NOT be erased;
		PicDecoderInfo.iStatus=MDC_SUCCESS;
}

inline void CPicDecoder::GetCodingPicPara(CODINGPICPARA *pCodingPicPara)
{
	pCodingPicPara->wCodedFrameWidth=wCodedFrameWidth;
	pCodingPicPara->wCodedFrameHeight=wCodedFrameHeight;
	pCodingPicPara->wCodedChromaWidth=wCodedChromaWidth;
	pCodingPicPara->wCodedChromaHeight=wCodedChromaHeight;
	pCodingPicPara->wMBWidth=wMBWidth;
	pCodingPicPara->wMBHeight=wMBHeight;
}

inline int CPicDecoder::GetCurYUV(BYTE *pnOutData[3],WORD pwOutPicWidth[3],WORD pwOutPicHeight[3], bool bFrameOnly)
{
	BYTE *pnCurDataBuf[3];

	if (bFrameOnly)
	{
		pnCurDataBuf[0]=GetCurFrameBuf(0);
		pnCurDataBuf[1]=GetCurFrameBuf(1);
		pnCurDataBuf[2]=GetCurFrameBuf(2);
		return GetYUVPic(pnOutData,pwOutPicWidth,pwOutPicHeight,pnCurDataBuf,true);
	}
	else
		return GetYUVPic(pnOutData,pwOutPicWidth,pwOutPicHeight,pnCurFrame);
}

inline int CPicDecoder::GetCurRGB(BYTE *pnOutData[3],WORD *pwOutPicWidth,WORD *pwOutPicHeight, bool bFrameOnly)
{
	BYTE *pnCurDataBuf[3];

	if (bFrameOnly)
	{
		pnCurDataBuf[0]=GetCurFrameBuf(0);
		pnCurDataBuf[1]=GetCurFrameBuf(1);
		pnCurDataBuf[2]=GetCurFrameBuf(2);
		return GetRGBPic(pnOutData,pwOutPicWidth,pwOutPicHeight,pnCurDataBuf,true);
	}
	else
		return GetRGBPic(pnOutData,pwOutPicWidth,pwOutPicHeight,pnCurFrame);
}

// other facilities

inline void CPicDecoder::RelocateBuf(bool bSwapRefBuf)
{
	BYTE nCount;
	BYTE *pnBufTemp;

	if (bSwapRefBuf)
	{ // swap forward and backward reference buffer;
		for (nCount=0; nCount<3; nCount++)
		{
			pnBufTemp=pnFwdRefFrame[nCount];
			pnFwdRefFrame[nCount]=pnBackRefFrame[nCount];
			pnBackRefFrame[nCount]=pnBufTemp;//pnCurFrame never points to pnFwdRefFrame;
		}
	}
	// restore buffer pointers for current pictures;
	if (pHeaderData[BASELAYER]->PictureHeader.nPictureCodingType==B_TYPE)
	{ // B pictures do not need to be saved for future reference;
		for (nCount=0; nCount<3; nCount++)
			pnCurFrame[nCount]=pnAuxFrame[nCount];
	}
	else  //I, P or D frame
	{ //reset pointers of current frame to the start position of backward reference;
		for (nCount=0; nCount<3; nCount++)
			pnCurFrame[nCount]=pnBackRefFrame[nCount];
	}
	if (pHeaderData[BASELAYER]->PictureExtension.nPictureStructure==BOTTOM_FIELD)
	{
		pnCurFrame[0]+=wCodedFrameWidth;
		pnCurFrame[1]+=wCodedChromaWidth;
		pnCurFrame[2]+=wCodedChromaWidth;
	}
}

// Get data pointers, for advanced user
inline BYTE *CPicDecoder::GetCurFrameBuf(BYTE nComponent)
{
	if (nComponent>=3)
	{
		PicDecoderInfo.iStatus=USE_ERROR;
		return NULL;
	}
	if (pHeaderData[BASELAYER]->PictureHeader.nPictureCodingType==B_TYPE)
		return pnAuxFrame[nComponent];
	else
		return pnBackRefFrame[nComponent];
}

inline BYTE *CPicDecoder::GetBackRefFrameBuf(BYTE nComponent)
{
	if (nComponent>=3)
	{
		PicDecoderInfo.iStatus=USE_ERROR;
		return NULL;
	}
	return pnBackRefFrame[nComponent];
}

inline BYTE *CPicDecoder::GetFwdRefFrameBuf(BYTE nComponent)
{
	if (nComponent>=3)
	{
		PicDecoderInfo.iStatus=USE_ERROR;
		return NULL;
	}
	return pnFwdRefFrame[nComponent];
}

////////////////////////////////////////////////////////////////
// inline protected functions:

// decode extra bit information, ISO/IEC 13818-2 section 6.2.3.4.
inline void CPicDecoder::ReadExtraBitInfo(void)
{
	while (pStreamBuf[CurLayerType]->GetByteBits(1))
		pStreamBuf[CurLayerType]->GetByteBits(8);
}

inline void CPicDecoder::MarkerBit(void)
{
#ifdef ERR_INFO
	if (!pStreamBuf[CurLayerType]->GetByteBits(1))  // must be 1;
	{
		PicDecoderInfo.iStatus=STREAM_ERROR;
	}
#else
	pStreamBuf[CurLayerType]->GetByteBits(1); //'1'
#endif
}

// set scratch pad macroblock to zero;
inline void CPicDecoder::ClearBlock(BYTE nComponent)
{
	memset(shBlock[CurLayerType][nComponent],0,64*sizeof(short));
}

// prepare shared temp buf;
inline int CPicDecoder::PrepareComTempBuf(BYTE nComponent, DWORD dwNewSize)
{
#ifdef DEBUG
	if (nComponent>=3)
	{
		PicDecoderInfo.iStatus=USE_ERROR;
		return UNSUCCESS;
	}
#endif
	if (dwCurTempBufSize[nComponent]<dwNewSize)
	{ // need to allocate a larger buffer;
		if (pnComTempBuf[nComponent]!=NULL)
			delete [] pnComTempBuf[nComponent];
		pnComTempBuf[nComponent]=new BYTE [dwNewSize];
		if (pnComTempBuf[nComponent]==NULL)
		{ // allocation failed;
			dwCurTempBufSize[nComponent]=0;
			PicDecoderInfo.iStatus=MEM_ERROR;
			return UNSUCCESS;
		}
		dwCurTempBufSize[nComponent]=dwNewSize;
	}
	return MDC_SUCCESS;
}

#endif  //CPICDECODER_H
