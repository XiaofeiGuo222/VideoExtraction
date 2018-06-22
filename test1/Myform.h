#pragma once
#include "include\cmpvdeco.h"
#include <tchar.h>
#include <dshow.h>
#include "include\convert.h"
#using <mscorlib.dll> 
#using <System.Drawing.dll>


struct DCTBlockStruct
{
	short A1, A2, A3, A4, B1, B2, B3, B4, C11;
} typedef ABBlocks;

struct ValueBlockStruct
{
	short x00, x01, x02, x03, x10, x11, x12, x13, x20, x21, x22, x23, x30, x31, x32, x33;
} typedef ValueBlocks;

namespace face
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	using namespace System::Runtime::InteropServices;

	WCHAR wpFileName[200];


	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
		}
		~Form1(void)
		{
			if (components)
				delete components;
		}

	private: System::ComponentModel::IContainer ^  components;
	private: System::Windows::Forms::PictureBox ^  pictureBox1;
	private: System::Windows::Forms::PictureBox ^  pictureBox2;
	private: System::Windows::Forms::Button ^  startButton;
	private: System::Windows::Forms::OpenFileDialog ^  openFileDialog;

	private:
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			//picture box
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			pictureBox1->Location = Point(266, 16);
			pictureBox1->Size = System::Drawing::Size(300, 300);
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			pictureBox2->Location = Point(606, 16);
			pictureBox2->Size = System::Drawing::Size(300, 300);

			//file dialog
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			
			//button
			this->startButton = (gcnew System::Windows::Forms::Button());
			this->startButton->Location = System::Drawing::Point(470, 600);
			this->startButton->Size = System::Drawing::Size(271, 69);
			this->startButton->TabIndex = 3;
			this->startButton->Text = L"Extract video";
			this->startButton->Click += gcnew System::EventHandler(this, &Form1::startButton_click);

			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->startButton);

			this->AutoScaleBaseSize = System::Drawing::Size(8, 19);
			this->ClientSize = System::Drawing::Size(1104, 857);
		}



	private: System::Void startButton_click(System::Object ^  sender, System::EventArgs ^  e)
	{
		const HEADERDATA *pMpvHeaderData;
		char psFileName[200];
		CMpvDecoder MpvDecoder;
		WORD PicWidth, PicHeight, CrPicWidth, CrPicHeight, wYPicWidth, wYPicHeight;
		long lResult;
		COMPRESSEDPIC *pCompressedPic;

		if (this->openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			int i = this->openFileDialog->FileName->Length;
			for (int j = 0; j<i; j++)
			{
				psFileName[j] = this->openFileDialog->FileName->ToCharArray()[j];
			}
			for (int j = i; j < 200; j++)
			{
				psFileName[j] = 0;
			}
			if ((lResult = MpvDecoder.Initialize(psFileName)) != MDC_SUCCESS)
				return;
		}
		

		pMpvHeaderData = MpvDecoder.GetMpvHeaders();
		SEQUENCEHEADER sequencehead = pMpvHeaderData->SequenceHeader;
		wYPicWidth = sequencehead.wHorizontalSize / 16;
		wYPicHeight = sequencehead.wVerticalSize / 16;
		
		pCompressedPic = MpvDecoder.PrepareCompressedPic();


		//重构第零帧
		lResult = MpvDecoder.GetCompressedPic();
		pMpvHeaderData = MpvDecoder.GetMpvHeaders();

		System::Drawing::Bitmap^ bitmap1 = gcnew System::Drawing::Bitmap((int)(wYPicWidth * 2), (int)(wYPicHeight * 2),
			System::Drawing::Imaging::PixelFormat::Format24bppRgb);
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

		for (int j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
		{
			pYBuff[j] = (BYTE)(pCompressedPic->pshYDCTBlock[j][0] >> 3) + 128;
		}

		for (int j = 0; j<wYPicWidth*wYPicHeight; j++)
		{
			pCbBuff[j] = (BYTE)(pCompressedPic->pshCbDCTBlock[j][0] >> 3) + 128;
			pCrBuff[j] = (BYTE)(pCompressedPic->pshCrDCTBlock[j][0] >> 3) + 128;
		}

		conv.YV12_to_RGB24(pYBuff, pCbBuff, pCrBuff, pRGBBuff, wYPicWidth * 2, wYPicHeight * 2);


		BYTE* q;
		q = pRGBBuff + wYPicWidth * 6 * (wYPicHeight * 2 - 1);

		for (int j = 0; j < wYPicHeight * 2; j++)
		{
			for (int k = 0; k<wYPicWidth * 2; k++)
			{
				p[0] = q[k * 3];
				p[1] = q[k * 3 + 1];
				p[2] = q[k * 3 + 2];
				p = p + 3;

			}
			q = q - wYPicWidth * 6;
		}

		bitmap1->UnlockBits(bmData);
		
		this->pictureBox1->Image = bitmap1;
		this->pictureBox1->Update();



		//尝试抽取9个系数重构
		//失败
		System::Drawing::Bitmap^ bitmap2 = gcnew System::Drawing::Bitmap((int)(wYPicWidth * 2), (int)(wYPicHeight * 2),
			System::Drawing::Imaging::PixelFormat::Format24bppRgb);
		System::Drawing::Imaging::BitmapData^ bmData2 = bitmap2->LockBits(
			*rect2,
			System::Drawing::Imaging::ImageLockMode::ReadWrite,
			System::Drawing::Imaging::PixelFormat::Format24bppRgb);


		m_stride = bmData2->Stride;
		m_scan0 = bmData2->Scan0.ToInt32();

		BYTE* p2 = (BYTE*)(void*)m_scan0;
		BYTE* pYBuff2 = new BYTE[wYPicWidth*wYPicHeight * 4];
		BYTE* pCbBuff2 = new BYTE[wYPicWidth*wYPicHeight];
		BYTE* pCrBuff2 = new BYTE[wYPicWidth*wYPicHeight];
		BYTE* pRGBBuff2 = new BYTE[wYPicWidth*wYPicHeight * 12];

		DCTBLOCK *C = pCompressedPic->pshYDCTBlock;

		ValueBlocks VB;

		for (int j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
		{
			//还原一个y block
			assignValues(C, j, VB);
			//取平均数存在buffer里
			pYBuff2[j] = (BYTE)getBlockAverage(VB) + 128;
		}

		//CbDCT block
		C = pCompressedPic->pshCbDCTBlock;
		for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
		{
			assignValues(C, j, VB);
			pCbBuff2[j] = (BYTE)getBlockAverage(VB) + 128;

		}

		C = pCompressedPic->pshCrDCTBlock;
		for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
		{
			assignValues(C, j, VB);
			pCrBuff2[j] = (BYTE)getBlockAverage(VB) + 128;
		}
		conv.YV12_to_RGB24(pYBuff2, pCbBuff2, pCrBuff2, pRGBBuff2, wYPicWidth * 2, wYPicHeight * 2);

		BYTE* q2 = pRGBBuff2 + wYPicWidth * 6 * (wYPicHeight * 2 - 1);

		for (int j = 0; j < wYPicHeight * 2; j++)
		{
			for (int k = 0; k<wYPicWidth * 2; k++)
			{
				p2[0] = q2[k * 3];
				p2[1] = q2[k * 3 + 1];
				p2[2] = q2[k * 3 + 2];
				p2 = p2 + 3;

			}
			q2 = q2 - wYPicWidth * 6;
		}


		bitmap2->UnlockBits(bmData2);

		this->pictureBox2->Image = bitmap2;
		this->pictureBox2->Update();

		MpvDecoder.FreeCompressedPic();
	}

	void assignValues(DCTBLOCK * C, int j, ValueBlocks& VB) {
		ABBlocks AB;
		assignBlock(C, AB, j);
		VB.x00 = ((AB.A1 + AB.A3) >> 3) + ((AB.B3 + AB.B1 + AB.C11) >> 3) + ((AB.B3 + AB.B1 + AB.C11) >> 5);
		VB.x10 = ((AB.A2 + AB.A4) >> 3) + (AB.B2 >> 3) + (AB.B2 >> 5) + (AB.B3 >> 4) + (AB.C11 >> 4);
		VB.x20 = ((AB.A2 + AB.A4) >> 3) - (AB.B2 >> 3) + (AB.B2 >> 5) + (AB.B3 >> 4) - (AB.C11 >> 4);
		VB.x30 = ((AB.A1 + AB.A3) >> 3) - ((AB.B3 - AB.B1) >> 4) - (AB.C11 >> 4);
		VB.x01 = ((AB.A1 - AB.A3) >> 3) + (AB.B3 >> 3) + (AB.B3 >> 5) + (AB.B1 >> 4) + (AB.C11 >> 4);
		VB.x11 = ((AB.A2 - AB.A4) >> 3) + ((AB.B4 + AB.B2) >> 4);
		VB.x21 = ((AB.A2 - AB.A4) >> 3) - ((AB.B4 - AB.B2) >> 4);
		VB.x31 = ((AB.A1 - AB.A3) >> 3) - (AB.B4 >> 3) + (AB.B4 >> 5) + (AB.B1 >> 4) + (AB.C11 >> 4);
		VB.x02 = ((AB.A1 - AB.A3) >> 3) + (AB.B4 >> 3) + (AB.B4 >> 5) - (AB.B1 >> 4) - (AB.C11 >> 4);
		VB.x12 = ((AB.A2 - AB.A4) >> 3) + (AB.B4 >> 4) - (AB.B2 >> 4);
		VB.x22 = ((AB.A2 - AB.A4) >> 3) - ((AB.B4 + AB.B2) >> 4);
		VB.x32 = ((AB.A1 - AB.A3) >> 3) - (AB.B4 >> 3) + (AB.B4 >> 5) - (AB.B1 >> 4) + (AB.C11 >> 4);
		VB.x03 = ((AB.A1 + AB.A3) >> 3) + ((AB.B4 - AB.B1 - AB.C11) >> 3) + ((AB.B4 - AB.B1 - AB.C11) >> 5);
		VB.x13 = ((AB.A2 + AB.A4) >> 3) - (AB.B2 >> 3) + (AB.B2 >> 5) + (AB.B3 >> 4) - (AB.C11 >> 4);
		VB.x23 = ((AB.A2 + AB.A4) >> 3) - (AB.B2 >> 3) + (AB.B2 >> 5) - (AB.B3 >> 4) + (AB.C11 >> 4);
		VB.x33 = ((AB.A1 + AB.A3) >> 3) + ((AB.B4 - AB.B1 - AB.C11) >> 3) + ((AB.B4 - AB.B1 - AB.C11) >> 5);
	}

	void assignBlock(DCTBLOCK * C, ABBlocks& AB, int i)
	{
		AB.A1 = C[i][0] + C[i][16];
		AB.A2 = C[i][0] - C[i][16];
		AB.A3 = C[i][2] + C[i][18];
		AB.A4 = C[i][2] - C[i][18];
		AB.B1 = C[i][1] + C[i][17];
		AB.B2 = C[i][1] - C[i][17];
		AB.B3 = C[i][1] + C[i][10];
		AB.B4 = C[i][1] - C[i][10];
		AB.C11 = C[i][9];
	}

	short getBlockAverage(ValueBlocks VB) {
		short sum;
		int size = 16;
		//the average may not necessarily be integer
		float avg = 0.0;  //or double for higher precision
		sum = VB.x00 + VB.x01 + VB.x02 + VB.x03 + VB.x10 + VB.x11 + VB.x12 + VB.x13 + VB.x20 + VB.x21 + VB.x22 + VB.x23 + VB.x30 + VB.x31 + VB.x32 + VB.x33;
		avg = (((float)sum) / size); //or cast sum to double before division
		return (short)avg;

	}
	};


}