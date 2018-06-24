#pragma once
#include "include\cmpvdeco.h"
#include <tchar.h>
#include <dshow.h>
#include "include\convert.h"
#define _USE_MATH_DEFINES
#include <math.h>

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
	private: System::Windows::Forms::PictureBox ^  pictureBox3;
	private: System::Windows::Forms::PictureBox ^  pictureBox4;
	private: System::Windows::Forms::Button ^  startButton;
	private: System::Windows::Forms::OpenFileDialog ^  openFileDialog;

	private:
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			//picture box
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			pictureBox1->Location = Point(266, 16);
			pictureBox1->Size = System::Drawing::Size(90, 90);
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			pictureBox2->Location = Point(606, 16);
			pictureBox2->Size = System::Drawing::Size(90, 90);
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			pictureBox3->Location = Point(100, 120);
			pictureBox3->Size = System::Drawing::Size(400, 400);
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			pictureBox4->Location = Point(600, 120);
			pictureBox4->Size = System::Drawing::Size(400, 400);


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
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->startButton);

			this->AutoScaleBaseSize = System::Drawing::Size(8, 19);
			this->ClientSize = System::Drawing::Size(1104, 857);
		}

	private:
		System::Void startButton_click(System::Object ^  sender, System::EventArgs ^  e)
		{
			const HEADERDATA *pMpvHeaderData;
			char psFileName[200];
			CMpvDecoder MpvDecoder;
			WORD PicWidth, PicHeight, CrPicWidth, CrPicHeight, wYPicWidth, wYPicHeight;
			long lResult;
			long lastnum;
			COMPRESSEDPIC *pCompressedPic;

			if (this->openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				int i = this->openFileDialog->FileName->Length;
				for (int j = 0; j < i; j++)
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
			PicWidth = sequencehead.wHorizontalSize;
			PicHeight = sequencehead.wVerticalSize;
			CrPicWidth = PicWidth / 2;
			CrPicHeight = PicHeight / 2;
			int blocknum = wYPicHeight * wYPicWidth;


			pCompressedPic = MpvDecoder.PrepareCompressedPic();
			MpvDecoder.GetMaxFrameNum(&lastnum);

			/*for (int i = 0; i < (int)(lastnum - 1); i++)
			{*/
				lResult = MpvDecoder.GetCompressedPic();
				pMpvHeaderData = MpvDecoder.GetMpvHeaders();
				Byte type1 = pMpvHeaderData->PictureHeader.nPictureCodingType;

				//MpvDecoder.GetCurFrameNum(&crt_num);

				if (type1 == I_TYPE)
				{
				//	//重构缩略图
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

					for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
					{
						pCbBuff[j] = (BYTE)(pCompressedPic->pshCbDCTBlock[j][0] >> 3) + 128;
						pCrBuff[j] = (BYTE)(pCompressedPic->pshCrDCTBlock[j][0] >> 3) + 128;
					}

					conv.YV12_to_RGB24(pYBuff, pCbBuff, pCrBuff, pRGBBuff, wYPicWidth * 2, wYPicHeight * 2);


					BYTE* q;
					q = pRGBBuff + wYPicWidth * 6 * (wYPicHeight * 2 - 1);

					for (int j = 0; j < wYPicHeight * 2; j++)
					{
						for (int k = 0; k < wYPicWidth * 2; k++)
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

					delete [] pYBuff;
					delete [] pCbBuff;
					delete [] pCrBuff;
					delete [] pRGBBuff;

					//抽取9个系数重构缩略图
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

				DCTBLOCK* Y = pCompressedPic->pshYDCTBlock;

					ValueBlocks VB;

					for (int j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
					{
						//还原一个y block
						assignValues(Y, j, VB);
						//取平均数存在buffer里
						pYBuff2[j] = (BYTE)getBlockAverage(VB) + 128;
					}

					//CbDCT block
				DCTBLOCK* Cb = pCompressedPic->pshCbDCTBlock;
					for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
					{
						assignValues(Cb, j, VB);
						pCbBuff2[j] = (BYTE)getBlockAverage(VB) + 128;

					}

				DCTBLOCK* Cr = pCompressedPic->pshCrDCTBlock;
					for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
					{
						assignValues(Cr, j, VB);
						pCrBuff2[j] = (BYTE)getBlockAverage(VB) + 128;
					}
					conv.YV12_to_RGB24(pYBuff2, pCbBuff2, pCrBuff2, pRGBBuff2, wYPicWidth * 2, wYPicHeight * 2);

					BYTE* q2 = pRGBBuff2 + wYPicWidth * 6 * (wYPicHeight * 2 - 1);

					for (int j = 0; j < wYPicHeight * 2; j++)
					{
						for (int k = 0; k < wYPicWidth * 2; k++)
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

					delete [] pYBuff2;
					delete [] pCbBuff2;
					delete [] pCrBuff2;
					delete [] pRGBBuff2;

					//原大小重构
				System::Drawing::Bitmap^ bitmap3 = gcnew System::Drawing::Bitmap((int)PicWidth, (int)PicHeight,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb);
				System::Drawing::Rectangle^ rect3 = gcnew System::Drawing::Rectangle(0, 0, (int)PicWidth, (int)PicHeight);

				System::Drawing::Imaging::BitmapData^ bmData3 = bitmap3->LockBits(
					*rect3,
					System::Drawing::Imaging::ImageLockMode::ReadWrite,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb);

				m_stride = bmData3->Stride;
				m_scan0 = bmData3->Scan0.ToInt32();
				BYTE* pc = (BYTE*)(void*)m_scan0;

				BYTE* pYBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 4];
				BYTE* pCbBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				BYTE* pCrBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				BYTE* pRGBBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 12];

				Y = pCompressedPic->pshYDCTBlock;
				//wYPicWidth, wYPicHeight表示横竖方向有多少个block?
				for (int j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
				{
					int x = (j % (wYPicWidth * 2)) * 8;
					int y = (j / (wYPicWidth * 2)) * 8;
					assignPixels(pYBuff3, x, y, PicWidth, Y, j);
				}

				Cb = pCompressedPic->pshCbDCTBlock;
				Cr = pCompressedPic->pshCrDCTBlock;
				for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
				{
					int x = (j % wYPicWidth) * 8;
					int y = (j / wYPicWidth) * 8;
					assignPixels(pCbBuff3, x, y, PicWidth / 2, Cb, j);
					assignPixels(pCrBuff3, x, y, PicWidth / 2, Cr, j);
				}

				conv.YV12_to_RGB24(pYBuff3, pCbBuff3, pCrBuff3, pRGBBuff3, wYPicWidth * 16, wYPicHeight * 16);

				BYTE* qc = pRGBBuff3 + wYPicWidth * 8 * 6 * (wYPicHeight * 8 * 2 - 1);

				for (int j = 0; j < wYPicHeight * 8 * 2; j++)
				{
					for (int k = 0; k < wYPicWidth * 8 * 2; k++)
					{
						pc[0] = qc[k * 3];
						pc[1] = qc[k * 3 + 1];
						pc[2] = qc[k * 3 + 2];
						pc = pc + 3;

					}
					qc = qc - wYPicWidth * 8 * 6;
				}

				bitmap3->UnlockBits(bmData3);

				this->pictureBox3->Image = bitmap3;
				this->pictureBox3->Update();

				delete[] pYBuff3;
				delete[] pCbBuff3;
				delete[] pCrBuff3;
				delete[] pRGBBuff3;

				//原大小抽取9个系数
				System::Drawing::Bitmap^ bitmap4 = gcnew System::Drawing::Bitmap((int)PicWidth, (int)PicHeight,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb);
				System::Drawing::Rectangle^ rect4 = gcnew System::Drawing::Rectangle(0, 0, (int)PicWidth, (int)PicHeight);

				System::Drawing::Imaging::BitmapData^ bmData4 = bitmap4->LockBits(
					*rect3,
					System::Drawing::Imaging::ImageLockMode::ReadWrite,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb);

				m_stride = bmData4->Stride;
				m_scan0 = bmData4->Scan0.ToInt32();
				BYTE* pc2 = (BYTE*)(void*)m_scan0;

				BYTE* pYBuff4 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 4];
				BYTE* pCbBuff4 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				BYTE* pCrBuff4 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				BYTE* pRGBBuff4 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 12];

				Y = pCompressedPic->pshYDCTBlock;
				for (int j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
				{
					//哪一列取余数
					int x = (j % (wYPicWidth * 2)) * 8;
					//哪一行取除数
					int y = (j / (wYPicWidth * 2)) * 8;
					assign4m4Pixels(pYBuff4, x, y, PicWidth, Y, j);
				}

				Cb = pCompressedPic->pshCbDCTBlock;
				Cr = pCompressedPic->pshCrDCTBlock;
				for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
				{
					int x = (j % wYPicWidth) * 8;
					int y = (j / wYPicWidth) * 8;
					assign4m4Pixels(pCbBuff4, x, y, PicWidth / 2, Cb, j);
					assign4m4Pixels(pCrBuff4, x, y, PicWidth / 2, Cr, j);
				}

				conv.YV12_to_RGB24(pYBuff4, pCbBuff4, pCrBuff4, pRGBBuff4, wYPicWidth * 16, wYPicHeight * 16);

				BYTE* qc2 = pRGBBuff4 + wYPicWidth * 8 * 6 * (wYPicHeight * 8 * 2 - 1);

				for (int j = 0; j < wYPicHeight * 8 * 2; j++)
				{
					for (int k = 0; k < wYPicWidth * 8 * 2; k++)
					{
						pc2[0] = qc2[k * 3];
						pc2[1] = qc2[k * 3 + 1];
						pc2[2] = qc2[k * 3 + 2];
						pc2 = pc2 + 3;

					}
					qc2 = qc2 - wYPicWidth * 8 * 6;
				}

				bitmap4->UnlockBits(bmData4);

				this->pictureBox4->Image = bitmap4;
				this->pictureBox4->Update();

				delete[] pYBuff4;
				delete[] pCbBuff4;
				delete[] pCrBuff4;
				delete[] pRGBBuff4;

			}
				if (MpvDecoder.MoveToNextFrame() != MDC_SUCCESS) {
					return;
				}
				//}

				MpvDecoder.FreeCompressedPic();
			
		}

		int IDCT(int x, int y, DCTBLOCK * C, int blocknum)
		{
			double alphau, alphav;
			double ret = 0;
			double temp = 0;
			for (int u = 0; u <= 7; u++)
			{
				for (int v = 0; v <= 7; v++)
				{
					if (u == 0)
						alphau = 1 / sqrt(2);
					else
						alphau = 1;

					if (v == 0)
						alphav = 1 / sqrt(2);
					else
						alphav = 1;
					int a = C[blocknum][u + 8 * v];
					temp += alphau * alphav * C[blocknum][u + 8 * v] * 
							cos(((2 * x + 1) * u * M_PI) / 16) * cos(((2 * y + 1) * v * M_PI) / 16);

				}
			}
			ret = temp / 4;
			
			return round(ret);
		}


		void assignPixels(Byte * pbuff, int x, int y, int num_per_row, DCTBLOCK * C, int blocknum)
		{
			int index = 0;
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					index = x + j + (y + i) * num_per_row;
					pbuff[index] = IDCT(j, i, C, blocknum) + 128;
				}
			}	
		}


		void assign4m4Pixels(Byte * pbuff, int x, int y, int num_per_row, DCTBLOCK * C, int blocknum)
		{
			int index = 0;
			ValueBlocks VB;
			assignValues(C, blocknum, VB);
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					index = x + i + (y + j) * num_per_row;
					switch (i)
					{
					case 0:
					case 1:
						switch (j)
						{
						case 0:
						case 1: //x00
							pbuff[index] = VB.x00 + 128;
							break;
						case 2:
						case 3: //x10
							pbuff[index] = VB.x10 + 128;
							break;
						case 4:
						case 5: //x20
							pbuff[index] = VB.x20 + 128;
							break;
						case 6:
						case 7: //x30
							pbuff[index] = VB.x30 + 128;
							break;
						}
						break;
					case 2:
					case 3:
						switch (j)
						{
						case 0:
						case 1: //x01
							pbuff[index] = VB.x01 + 128;
							break;
						case 2:
						case 3: //x11
							pbuff[index] = VB.x11 + 128;
							break;
						case 4:
						case 5: //x21
							pbuff[index] = VB.x21 + 128;
							break;
						case 6:
						case 7: //x31
							pbuff[index] = VB.x31 + 128;
						}
						break;
					case 4:
					case 5:
						switch (j)
						{
						case 0:
						case 1: //x02
							pbuff[index] = VB.x02 + 128;
							break;
						case 2:
						case 3: //x12
							pbuff[index] = VB.x12 + 128;
							break;
						case 4:
						case 5: //x22
							pbuff[index] = VB.x22 + 128;
							break;
						case 6:
						case 7: //x32
							pbuff[index] = VB.x32 + 128;
						}
						break;
					case 6:
					case 7:
						switch (j)
						{
						case 0:
						case 1: //x03
							pbuff[index] = VB.x03 + 128;
							break;
						case 2:
						case 3: //x13
							pbuff[index] = VB.x13 + 128;
							break;
						case 4:
						case 5: //x23
							pbuff[index] = VB.x23 + 128;
							break;
						case 6:
						case 7: //x33
							pbuff[index] = VB.x33 + 128;
						}
						break;
					}
				}
			}

		}


		void assignValues(DCTBLOCK * C, int j, ValueBlocks& VB) {
			ABBlocks AB;
			assignBlock(C, AB, j);
			VB.x00 = ((AB.A1 + AB.A3) >> 3) + ((AB.B3 + AB.B1 + AB.C11) >> 3) + ((AB.B3 + AB.B1 + AB.C11) >> 5);
			VB.x10 = ((AB.A2 + AB.A4) >> 3) + (AB.B2 >> 3) + (AB.B2 >> 5) + (AB.B3 >> 4) + (AB.C11 >> 4);
			VB.x20 = ((AB.A2 + AB.A4) >> 3) + (AB.B2 >> 3) + (AB.B2 >> 5) - (AB.B3 >> 4) - (AB.C11 >> 4);
			VB.x30 = ((AB.A1 + AB.A3) >> 3) - ((AB.B3 - AB.B1) >> 4) - (AB.C11 >> 4);
			VB.x01 = ((AB.A1 - AB.A3) >> 3) + (AB.B3 >> 3) + (AB.B3 >> 5) + (AB.B1 >> 4) + (AB.C11 >> 4);
			VB.x11 = ((AB.A2 - AB.A4) >> 3) + ((AB.B4 + AB.B2) >> 4);
			VB.x21 = ((AB.A2 - AB.A4) >> 3) - ((AB.B4 - AB.B2) >> 4);
			VB.x31 = ((AB.A1 - AB.A3) >> 3) - (AB.B4 >> 3) - (AB.B4 >> 5) + (AB.B1 >> 4) + (AB.C11 >> 4);
			VB.x02 = ((AB.A1 - AB.A3) >> 3) + (AB.B4 >> 3) + (AB.B4 >> 5) - (AB.B1 >> 4) - (AB.C11 >> 4);
			VB.x12 = ((AB.A2 - AB.A4) >> 3) + (AB.B4 >> 4) - (AB.B2 >> 4);
			VB.x22 = ((AB.A2 - AB.A4) >> 3) - ((AB.B4 + AB.B2) >> 4);
			VB.x32 = ((AB.A1 - AB.A3) >> 3) - (AB.B4 >> 3) - (AB.B4 >> 5) - (AB.B1 >> 4) + (AB.C11 >> 4);
			VB.x03 = ((AB.A1 + AB.A3) >> 3) + ((AB.B4 - AB.B1 - AB.C11) >> 3) + ((AB.B4 - AB.B1 - AB.C11) >> 5);
			VB.x13 = ((AB.A2 + AB.A4) >> 3) - (AB.B2 >> 3) - (AB.B2 >> 5) + (AB.B3 >> 4) - (AB.C11 >> 4);
			VB.x23 = ((AB.A2 + AB.A4) >> 3) - (AB.B2 >> 3) - (AB.B2 >> 5) - (AB.B3 >> 4) + (AB.C11 >> 4);
			VB.x33 = ((AB.A1 + AB.A3) >> 3) - ((AB.B4 + AB.B1 - AB.C11) >> 3) - ((AB.B4 + AB.B1 - AB.C11) >> 5);
		}

		void assignBlock(DCTBLOCK * C, ABBlocks& AB, int i)
		{
			AB.A1 = C[i][0] + C[i][16];
			AB.A2 = C[i][0] - C[i][16];
			AB.A3 = C[i][2] + C[i][18];
			AB.A4 = C[i][2] - C[i][18];
			AB.B1 = C[i][1] + C[i][17];
			AB.B2 = C[i][1] - C[i][17];
			AB.B3 = C[i][8] + C[i][10];
			AB.B4 = C[i][8] - C[i][10];
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