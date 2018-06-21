// cobitbuf.h : interface of bidirectional bit buffer management
//
// This class provides a set of bit-based and byte-based functions
// to read information from buffer in two direction and manage buffer.
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

#if !defined COBITBUF_H
#define COBITBUF_H

#include <stdio.h>
#include "datatype.h"

const long MIN_SIZE_LIMIT=sizeof(DWORD)+1; // minimum number of data remained in the buffer, must larger than 5(DWORD+BitOffset);


enum DIRECTION {FORWARD, BACKWARD};
typedef struct bufpos_tag
{
	long lCurBytePos;
	BYTE nCurBitOffset;
	long lStartPos; // data start position in the buffer;
	long lEndPos; // data end position in the buffer;
	long lTotalItemNum;
} BUFPOS;

class COBitBuf
{
// constructor and destructor
public:
	COBitBuf();
	~COBitBuf();

// attributes
protected:
	BYTE *pnBuf;
	DWORD dwBufSize;
	long lActualBufSize;  // just for speedup, it equals to dwBufSize-1;
	BUFPOS BufPos;
	long lFwdLoadBlkSize, lBackLoadBlkSize;
	int iStatus;

// operation
public:
	int InitBuffer(DWORD dwBufferSize);
	void ResetBuffer(void);
	DWORD GetBufferSize(void) { return dwBufSize; };
	BYTE GetCurBitOffset() { return BufPos.nCurBitOffset; };
	bool ByteAligned() { return (BufPos.nCurBitOffset==0)? true : false; };
	int SetLoadSize(long lNewLoadSize, DIRECTION eDirection);
	int SetLoadSize(long lFwdLoadSize, long lBackLoadSize);
	long GetLoadSize(DIRECTION eDirection);
	int FwdJumpBytes(DWORD dwOffset);
	int BackJumpBytes(DWORD dwOffset);
	BYTE ShowByte(int *piException);
	BYTE ShowByte(void);
	BYTE GetByte(int *piException);
	BYTE GetByte(void);
	WORD ShowWord(int *piException);
	WORD ShowWord(void);
	WORD GetWord(int *piException);
	WORD GetWord(void);
	DWORD ShowDWord(int *piException);
	DWORD ShowDWord(void);
	DWORD GetDWord(int *piException);
	DWORD GetDWord(void);
	inline int FwdJumpBits(DWORD dwOffset);
	inline int BackJumpBits(DWORD dwOffset);
	DWORD ShowBits(BYTE nBitNum, int *piException);
	DWORD ShowBits(BYTE nBitNum);
	inline DWORD GetBits(BYTE nBitNum, int *piException);
	inline DWORD GetBits(BYTE nBitNum);
	int GetStatus(void) {return iStatus;};
	void ClearStatus(void) {iStatus=MDC_SUCCESS;};
	//just for advance user to slightly improve performance;
	inline BYTE ShowByteBits(BYTE nBitNum, int *piException);
	inline BYTE ShowByteBits(BYTE nBitNum);
	inline BYTE GetByteBits(BYTE nBitNum);


protected:
	void SetBufPos(const BUFPOS &dNewBufPos) {BufPos=dNewBufPos;};

// virtual methods;
protected: 
	//Unsuccess represents by 0; and fatal errors <0;
	virtual long FwdLoadData(long lLoadStartPos, DWORD dwItemNum)=0;// used by other functions
	//Unsuccess represents by 0; and fatal errors <0;
	virtual long BackLoadData(long lLoadEndPos, DWORD dwItemNum)=0;	
	//Unsuccess represents by values smaller than 0; No fatal error;
	virtual long FlushBuf(long lOffset, BUFPOS *pBufPos)=0;
};

int COBitBuf::FwdJumpBits(DWORD dwOffset)
{
	DWORD dwBitOffset;
	int iStatus;

	dwBitOffset=dwOffset+BufPos.nCurBitOffset;
	if (dwBitOffset>=8)
	{
		iStatus=FwdJumpBytes(dwBitOffset>>3);
		if (iStatus!=MDC_SUCCESS)
			return iStatus;
		BufPos.nCurBitOffset=(BYTE)dwBitOffset&7;  // module 8
	}
	else
		BufPos.nCurBitOffset=(BYTE)dwBitOffset;
	return MDC_SUCCESS;
}

int COBitBuf::BackJumpBits(DWORD dwOffset)
{
	long lBitOffset;
	int iStatus;

	lBitOffset=BufPos.nCurBitOffset-dwOffset;
	if (lBitOffset>=0)   // still the same byte;
	{
		BufPos.nCurBitOffset=(BYTE)lBitOffset;
	}
	else   // backward
	{
		iStatus=BackJumpBytes(-(lBitOffset+1)/8+1); // Bit -1 to -8 is Byte -1 position.
		if (iStatus!=MDC_SUCCESS)
			return iStatus;
		BufPos.nCurBitOffset=7+(lBitOffset+1)%8; 
	}
	return MDC_SUCCESS;
}

DWORD COBitBuf::GetBits(BYTE nBitNum, int *piException)
{
	DWORD dwResult;
	BYTE nBitEnd;

	dwResult=ShowBits(nBitNum,piException);
	if (*piException!=MDC_SUCCESS)
		return 0;
	// the data has actually be loaded in ShowBits successfully
	nBitEnd=nBitNum+BufPos.nCurBitOffset;
	BufPos.lCurBytePos+=(nBitEnd>>3);  // divided by 8
	if (BufPos.lCurBytePos>=(long)dwBufSize)
		BufPos.lCurBytePos-=dwBufSize;
	BufPos.nCurBitOffset=nBitEnd&7;  // module 8
	return dwResult;
}

DWORD COBitBuf::GetBits(BYTE nBitNum)
{
	DWORD dwResult;
	BYTE nBitEnd;
	int iException;

	dwResult=ShowBits(nBitNum,&iException);
	if (iException!=MDC_SUCCESS)
		return 0;
	// the data has actually be loaded in ShowBits successfully
	nBitEnd=nBitNum+BufPos.nCurBitOffset;
	BufPos.lCurBytePos+=(nBitEnd>>3);  // divided by 8
	if (BufPos.lCurBytePos>=(long)dwBufSize)
		BufPos.lCurBytePos-=dwBufSize;
	BufPos.nCurBitOffset=nBitEnd&7;  // module 8
	return dwResult;
}

BYTE COBitBuf::ShowByteBits(BYTE nBitNum, int *piException)
{
#ifdef DEBUG
	if (nBitNum>8)
	{
		if (iStatus>0)
			iStatus=UNSUCCESS;
		*piException=UNSUCCESS;
		return 0;
	}
#endif
	if (nBitNum+BufPos.nCurBitOffset<=8)
		return ( (BYTE)(ShowByte(piException)<<BufPos.nCurBitOffset) )>>( 8-nBitNum );
	else
		return (BYTE)(( (WORD)(ShowWord(piException)<<BufPos.nCurBitOffset) )>>( 16-nBitNum ));
}

BYTE COBitBuf::ShowByteBits(BYTE nBitNum)
{
#ifdef DEBUG
	if (nBitNum>8)
	{
		if (iStatus>0)
			iStatus=UNSUCCESS;
		return 0;
	}
#endif
	if (nBitNum+BufPos.nCurBitOffset<=8)
		return ( (BYTE)(ShowByte()<<BufPos.nCurBitOffset) )>>( 8-nBitNum );
	else
		return (BYTE)(( (WORD)(ShowWord()<<BufPos.nCurBitOffset) )>>( 16-nBitNum ));
}

BYTE COBitBuf::GetByteBits(BYTE nBitNum)
{
	BYTE nResult;
	BYTE nBitEnd;
	int iException;

	nResult=ShowByteBits(nBitNum,&iException);
	if (iException!=MDC_SUCCESS)
		return 0;
	// the data has actually be loaded in ShowBits successfully
	nBitEnd=nBitNum+BufPos.nCurBitOffset;
	BufPos.lCurBytePos+=(nBitEnd>>3);  // divided by 8
	if (BufPos.lCurBytePos>=(long)dwBufSize)
		BufPos.lCurBytePos-=dwBufSize;
	BufPos.nCurBitOffset=nBitEnd&7;  // module 8
	return nResult;
}

#endif // COBITBUF_H