// cdlist.h : interface of a double list 
//
// Although some compliers (such as Visual C++) support List data type, 
// this class is still been developed to made the whole system can be used
// under those compliers which don't have such kind of class.
// Furthermore, this class has been developped to meet our specific requirement.
// It is faster than those versions designed for general purpose;
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

#if !defined CDLIST_H
#define CDLIST_H

#include <stdio.h>
#include "datatype.h"

template <class CItem, int iMaxItemNum> 
class CDList
{
// constructor and destructor
public:
	CDList(void);
	~CDList();

// attributes
protected:
	int iHeadPointer;
	int iTailPointer;
	int iFreePointer;
	CItem *pItemList;
	int *piPrevious;
	int *piNext;
	int iCurPointer;

// operation
public:
	int iStatus; //the value of iStatus has no effect on CDList, only used to indicate current status of CDList;
	void AddHead(const CItem &NewItem);
	void AddTail(const CItem &NewItem);
	CItem RemoveHead();
	CItem RemoveTail();
	CItem ShowHead() {return pItemList[iHeadPointer];};
	CItem ShowTail() {return pItemList[iTailPointer];};
	bool IsEmpty() { return (iHeadPointer==iFreePointer)? true:false;};
	int GetItemNum();
	void RemoveAll();
// must ensure the list is not empty before use the following functions;
	void ToHead() {iCurPointer=iHeadPointer;}; // initialize iCurPointer;
	void ToTail() {iCurPointer=iTailPointer;}; // initialize iCurPointer;
	inline bool ToNext();
	inline bool ToPrevious();
	CItem ShowItem() {return pItemList[iCurPointer];};
};

// constructor
template <class CItem, int iMaxItemNum>
CDList<CItem, iMaxItemNum>::CDList(void)
{
	pItemList=new CItem [iMaxItemNum];
	if (pItemList==NULL)
	{
		iStatus=MEM_ERROR;
		return;
	}
	piPrevious=new int [iMaxItemNum];
	if (piPrevious==NULL)
	{
		iStatus=MEM_ERROR;
		delete [] pItemList;
		return;
	}
	piNext=new int [iMaxItemNum];
	if (piNext==NULL)
	{
		iStatus=MEM_ERROR;
		delete [] piPrevious;
		delete [] pItemList;
		return;
	}
	iHeadPointer=iTailPointer=iFreePointer=0;  //point to the same place
	for (int i=0; i<iMaxItemNum-1; i++)
		piNext[i]=i+1;
	piNext[iMaxItemNum-1]=0;  // form a circle
	iStatus=MDC_SUCCESS;
}

// destructor
template <class CItem, int iMaxItemNum>
CDList<CItem, iMaxItemNum>::~CDList()
{
	if (pItemList!=NULL)
		delete [] pItemList;
	if (piPrevious!=NULL)
		delete [] piPrevious;
	if (piNext!=NULL)
		delete [] piNext;
}

// operations

template <class CItem, int iMaxItemNum>
void CDList<CItem, iMaxItemNum>::AddHead(const CItem &NewItem)
{
	int iPosition;

	//change free list
	iPosition=iFreePointer;   // get the free position
	iFreePointer=piNext[iFreePointer];  // change the free list
	//set this new item 
	pItemList[iPosition]=NewItem;  
	piPrevious[iPosition]=iPosition;  // point to itself  
	piNext[iPosition]=iHeadPointer;
	//change head pointer
	piPrevious[iHeadPointer]=iPosition;
	iHeadPointer=iPosition;
}

template <class CItem, int iMaxItemNum>
void CDList<CItem, iMaxItemNum>::AddTail(const CItem &NewItem)
{
	int iPosition;

	iPosition=iFreePointer;   // get the free position
	iFreePointer=piNext[iFreePointer];  // change the free list
	pItemList[iPosition]=NewItem;  
	piPrevious[iPosition]=iTailPointer;  
	piNext[iPosition]=iPosition;  // point to itself
	piNext[iTailPointer]=iPosition;
	iTailPointer=iPosition;
}

template <class CItem, int iMaxItemNum>
CItem CDList<CItem, iMaxItemNum>::RemoveHead()
{
	int iPosition;

	iPosition=iHeadPointer;
	iHeadPointer=piNext[iPosition];
	piPrevious[iHeadPointer]=iHeadPointer;  // point to itself
	piNext[iPosition]=iFreePointer;
	iFreePointer=iPosition;
	return pItemList[iPosition];
}

template <class CItem, int iMaxItemNum>
CItem CDList<CItem, iMaxItemNum>::RemoveTail()
{
	int iPosition;

	iPosition=iTailPointer;
	iTailPointer=piPrevious[iPosition];
	piNext[iTailPointer]=iTailPointer; // point to itself
	piNext[iPosition]=iFreePointer;
	iFreePointer=iPosition;
	return pItemList[iPosition];
}

template <class CItem, int iMaxItemNum>
int CDList<CItem, iMaxItemNum>::GetItemNum()
{
	int iPosition;
	int iCount;

	if (IsEmpty())
		return 0;
	iPosition=iHeadPointer;
	for (iCount=1; iCount<=iMaxItemNum+1; iCount++)
	{
		if (piNext[iPosition]==iPosition)
			break;
		iPosition=piNext[iPosition];
	}
	return iCount;  // if i=iMaxItemNum+1, there is an error;
}

template <class CItem, int iMaxItemNum>
void CDList<CItem, iMaxItemNum>::RemoveAll()
{
	int iCount;

	iHeadPointer=iTailPointer=iFreePointer=0;  //point to the same place
	for (iCount=0; iCount<iMaxItemNum-1; iCount++)
		piNext[iCount]=iCount+1;
	piNext[iMaxItemNum-1]=0;  // form a circle
}

template <class CItem, int iMaxItemNum>
inline bool CDList<CItem, iMaxItemNum>::ToNext()
{
	if (piNext[iCurPointer]==iCurPointer)  //tail of the list;
		return false;	
	iCurPointer=piNext[iCurPointer];
	return true;
};

template <class CItem, int iMaxItemNum>
inline bool CDList<CItem, iMaxItemNum>::ToPrevious()
{
	if (piPrevious[iCurPointer]==iCurPointer)//head of the list;
		return false;	
	iCurPointer=piPrevious[iCurPointer];
	return true;
};


#endif // CDLIST_H
