#pragma once
#include "include\cmpvdeco.h"
#include <tchar.h>
#include <dshow.h>
#include "include\convert.h"
#using <system.dll>
#using <system.drawing.dll>


using namespace System;
using namespace System::Drawing;



int main1()
{
	System::Windows::Forms::Panel^ panel1 = gcnew System::Windows::Forms::Panel();;


	const HEADERDATA *pMpvHeaderData;
	const char * psFileName = "stefan.mpg";
	CMpvDecoder MpvDecoder;
	WORD wYPicWidth, wYPicHeight;
	long lResult;
	long lastnum;
	BYTE type1;
	long crt_num;
	double block_num;
	COMPRESSEDPIC *pCompressedPic;
	/*BYTE* pYBuff;
	BYTE* pCbBuff;
	BYTE* pCrBuff;*/
	int i;
	int i_number;
	
	//fail
	if ((lResult = MpvDecoder.Initialize(psFileName)) != MDC_SUCCESS)
	{
		return 0;
	}

	MpvDecoder.GetMaxFrameNum(&lastnum);

	pMpvHeaderData = MpvDecoder.GetMpvHeaders();
	SEQUENCEHEADER sequencehead = pMpvHeaderData->SequenceHeader;
	wYPicWidth = sequencehead.wHorizontalSize / 16;
	wYPicHeight = sequencehead.wVerticalSize / 16;
	block_num = wYPicWidth * wYPicHeight;
	i_number = 0;


	System::Drawing::Rectangle rc;
	rc = panel1->ClientRectangle;
	int num_row = (int)(rc.Height / (wYPicHeight * 2));
	int num_col = (int)(rc.Width / (wYPicWidth * 2));
	int num_pic = num_row * num_col;
	array<System::Windows::Forms::PictureBox^>^ picBox = gcnew array<System::Windows::Forms::PictureBox^>(num_pic);
	for (i = 0; i<num_pic; i++)
	{
		int heightLoc = (int)(num_pic / num_col);
		int widthLoc = num_pic % num_col;
		picBox[i] = gcnew System::Windows::Forms::PictureBox();
		picBox[i]->Location = System::Drawing::Point(heightLoc, widthLoc);
		picBox[i]->Size = System::Drawing::Size(wYPicHeight * 2, wYPicWidth * 2);
	}



	System::Drawing::Bitmap^ bitmap1 = gcnew System::Drawing::Bitmap((int)(wYPicWidth * 2), (int)(wYPicHeight * 2),
		System::Drawing::Imaging::PixelFormat::Format24bppRgb);

	/*	pYBuff = new BYTE[wYPicWidth*wYPicHeight*4];
	pCrBuff = new BYTE[wYPicWidth*wYPicHeight];
	pCbBuff = new BYTE[wYPicWidth*wYPicHeight];*/

	pCompressedPic = MpvDecoder.PrepareCompressedPic();

	for (i = 0; i < (int)(lastnum - 1); i++)
	{

		lResult = MpvDecoder.GetCompressedPic();
		pMpvHeaderData = MpvDecoder.GetMpvHeaders();
		type1 = pMpvHeaderData->PictureHeader.nPictureCodingType;

		//MpvDecoder.GetCurFrameNum(&crt_num);

		if (type1 == I_TYPE)
		{
			int j;
			int pos_frame;
			pos_frame = i_number % 12;

			System::Drawing::Rectangle^ rect2 = gcnew System::Drawing::Rectangle(0, 0, (int)(wYPicWidth * 2), (int)(wYPicHeight * 2));

			System::Drawing::Imaging::BitmapData^ bmData = bitmap1->LockBits(
				*rect2,
				System::Drawing::Imaging::ImageLockMode::ReadWrite,
				System::Drawing::Imaging::PixelFormat::Format24bppRgb);


			int m_stride = bmData->Stride;
			int m_scan0 = bmData->Scan0.ToInt32();
			BYTE* p = (BYTE*)(void*)m_scan0;

			BYTE* pYBuff = new BYTE[wYPicWidth*wYPicHeight * 4];
			BYTE* pCbBuff = new BYTE[wYPicWidth*wYPicHeight];
			BYTE* pCrBuff = new BYTE[wYPicWidth*wYPicHeight];
			BYTE* pRGBBuff = new BYTE[wYPicWidth*wYPicHeight * 12];
			ColorSpaceConversions conv;

			for (j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
			{
				pYBuff[j] = (BYTE)(pCompressedPic->pshYDCTBlock[j][0] >> 3) + 128;
			}

			for (j = 0; j<wYPicWidth*wYPicHeight; j++)
			{
				pCbBuff[j] = (BYTE)(pCompressedPic->pshCbDCTBlock[j][0] >> 3) + 128;
				pCrBuff[j] = (BYTE)(pCompressedPic->pshCrDCTBlock[j][0] >> 3) + 128;
			}

			//TODO DEFINE THIS FUNCTION
			conv.YV12_to_RGB24(pYBuff, pCbBuff, pCrBuff, pRGBBuff, wYPicWidth, wYPicHeight);
			int k;
			BYTE* q;
			q = pRGBBuff + wYPicWidth * 3 * (wYPicHeight * - 1);

			for (j = 0; j < wYPicHeight; j++)
			{
				for (k = 0; k<wYPicWidth; k++)
				{
					p[0] = q[k * 3];
					p[1] = q[k * 3 + 1];
					p[2] = q[k * 3 + 2];
					p = p + 3;

				}
				q = q - wYPicWidth * 3;
			}

			bitmap1->UnlockBits(bmData);
			picBox[pos_frame]->Image = bitmap1;
			picBox[pos_frame]->Update();

			i_number++;

			free(pYBuff);
			free(pCrBuff);
			free(pCbBuff);
			free(pRGBBuff);

		}

		if (MpvDecoder.MoveToNextFrame() != MDC_SUCCESS)
			return 0;

	}

	MpvDecoder.FreeCompressedPic();

	return 0;
}