// cobfBuf.h : interface of bidirectional bit reading through file buffer
//
// This class provides a set of bit-based and byte-based functions
// to read information from bufferred file in two direction and manage buffer.
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

#if !defined COBITFILEBUF_H
#define COBITFILEBUF_H

#include <stdio.h>
#include "datatype.h"
#include "cobitbuf.h"

class COBitFileBuf : public COBitBuf
{
// constructor and destructor
public:
	COBitFileBuf();
	~COBitFileBuf();

// attributes
protected:
	FILE *pfFile;
	long lBufStartFPos, lBufEndFPos;
	long lFileLength;

// operation
public:
	int InitBuffer(FILE *pfTheFile, DWORD dwBufferSize);
	long GetCurFilePos(void);
	long FlushBuf(long lFilePos);

// override
public:
	void ResetBuffer(void);

protected: 
	virtual long FwdLoadData(long lLoadStartPos, DWORD dwItemNum);  // used by other functions
	virtual long BackLoadData(long lLoadEndPos, DWORD dwItemNum);	
	virtual long FlushBuf(long lOffset, BUFPOS *pBufPos);
};

#endif // COBITFILEBUF_H