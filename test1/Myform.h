#pragma once
#include "include\cmpvdeco.h"
#include <tchar.h>
#include <dshow.h>
#include "include\convert.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <time.h>

#using <mscorlib.dll> 
#using <System.Drawing.dll>
//using namespace std;

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

		/*typedef enum _FilterState
		{
			State_Stopped,
			State_Paused,
			State_Running
		};

		face::_FilterState Current_State = face::State_Stopped;
*/


	private: System::ComponentModel::IContainer ^  components;
			 //private: System::Windows::Forms::PictureBox ^  pictureBox1;
	private: System::Windows::Forms::PictureBox ^  pictureBox;
	private: System::Windows::Forms::Button ^  videoExtractionButton;
	private: System::Windows::Forms::Button ^  motionVectorButton;
	private: System::Windows::Forms::OpenFileDialog ^  openFileDialog;
	private: System::Windows::Forms::RichTextBox ^  richTextBox;

	private:
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());

			this->richTextBox = gcnew System::Windows::Forms::RichTextBox();
			this->richTextBox->Location = System::Drawing::Point(50, 30);
			this->richTextBox->Name = "richTextBox1";
			this->richTextBox->Size = System::Drawing::Size(500, 500);
			this->richTextBox->TabIndex = 14;
			this->richTextBox->Text = "HI, this place is for displaying motion vector data";


			//picture box
			//this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			//pictureBox1->Location = Point(100, 120);
			//pictureBox1->Size = System::Drawing::Size(400, 400);
			this->pictureBox = (gcnew System::Windows::Forms::PictureBox());
			pictureBox->Location = Point(620, 120);
			pictureBox->Size = System::Drawing::Size(400, 400);


			//file dialog
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());

			//button
			this->videoExtractionButton = (gcnew System::Windows::Forms::Button());
			this->videoExtractionButton->Location = System::Drawing::Point(600, 600);
			this->videoExtractionButton->Size = System::Drawing::Size(271, 69);
			this->videoExtractionButton->TabIndex = 3;
			this->videoExtractionButton->Text = L"Extract video";
			this->videoExtractionButton->Click += gcnew System::EventHandler(this, &Form1::videoExtractionButton_click);

			this->motionVectorButton = (gcnew System::Windows::Forms::Button());
			this->motionVectorButton->Location = System::Drawing::Point(300, 600);
			this->motionVectorButton->Size = System::Drawing::Size(271, 69);
			this->motionVectorButton->TabIndex = 3;
			this->motionVectorButton->Text = L"Motion vector";
			this->motionVectorButton->Click += gcnew System::EventHandler(this, &Form1::motionVectorpsButton_click);

			//this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->pictureBox);
			this->Controls->Add(this->videoExtractionButton);
			this->Controls->Add(this->motionVectorButton);
			this->Controls->Add(this->richTextBox);

			this->AutoScaleBaseSize = System::Drawing::Size(8, 19);
			this->ClientSize = System::Drawing::Size(1104, 857);
		}


	private:
		System::Void motionVectorButton_click(System::Object ^  sender, System::EventArgs ^  e)
		{
			const HEADERDATA *pMpvHeaderData;
			char pFileName[200];
			CMpvDecoder MpvDecoder;
			WORD PicWidth, PicHeight, CrPicWidth, CrPicHeight, wYPicWidth, wYPicHeight;
			long lResult;
			long lastnum;
			BYTE type1;
			int Num_Inter;
			double block_num;
			COMPRESSEDPIC *pCompressedPic;
			long crt_num;
			int zoomin_count;
			int Pframe_num;
			int closeup_count;
			int count;

			int i0, j0, ik, jk;
			double sxk1, syk1, sxk, syk, mvi, mvj, Avx1, Avy1, Avx, Avy, s_nu, s_de, s, smax, smin;
			double* xk1Buff;
			double* yk1Buff;
			double* xkBuff;
			double* ykBuff;
			double* zincountBuff;
			double* zinframeBuff;
			double* zinsBuff;

			double* zinsmaxBuff;
			double* zinsminBuff;

			int* cframeBuff;


			if (this->openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				int i = this->openFileDialog->FileName->Length;
				for (int j = 0; j < i; j++)
				{
					pFileName[j] = this->openFileDialog->FileName->ToCharArray()[j];
				}
				for (int j = i; j < 200; j++)
				{
					pFileName[j] = 0;
				}
				if ((lResult = MpvDecoder.Initialize(pFileName)) != MDC_SUCCESS)
					return;
			}

			HRESULT hr;

			//if (Current_State == face::State_Stopped)
			//{
			//	// CloseInterfaces();						 		 
			//	// OpenInterfaces();

			//	hr = pControl->Run();
			//	Current_State = face::State_Running;

			//}

			this->richTextBox->ResetText();


			MpvDecoder.GetMaxFrameNum(&lastnum);

			pMpvHeaderData = MpvDecoder.GetMpvHeaders();
			SEQUENCEHEADER sequencehead = pMpvHeaderData->SequenceHeader;
			i0 = sequencehead.wVerticalSize / 2;
			j0 = sequencehead.wHorizontalSize / 2;

			PicWidth = sequencehead.wHorizontalSize;
			PicHeight = sequencehead.wVerticalSize;
			CrPicWidth = PicWidth / 2;
			CrPicHeight = PicHeight / 2;


			wYPicWidth = sequencehead.wHorizontalSize / 16;
			wYPicHeight = sequencehead.wVerticalSize / 16;
			block_num = wYPicWidth * wYPicHeight;
			zoomin_count = 0;
			closeup_count = 0;

			smax = 0;
			smin = 0;

			BYTE* mbtype = new BYTE[wYPicWidth*wYPicHeight];
			zincountBuff = new double[lastnum];
			zinframeBuff = new double[lastnum];

			Pframe_num = 0;

			cframeBuff = new int[lastnum];

			zinsBuff = new double[lastnum];

			zinsmaxBuff = new double[lastnum];
			zinsminBuff = new double[lastnum];

			pCompressedPic = MpvDecoder.PrepareCompressedPic();

			for (int i = 0; i < (int)lastnum; i++)
			{
				lResult = MpvDecoder.GetCompressedPic();
				pMpvHeaderData = MpvDecoder.GetMpvHeaders();
				type1 = pMpvHeaderData->PictureHeader.nPictureCodingType;

				if (type1 == P_TYPE)
				{

					Num_Inter = 0;
					sxk1 = 0;
					syk1 = 0;
					sxk = 0;
					syk = 0;
					mvi = 0;
					mvj = 0;
					Avx1 = 0;
					Avy1 = 0;
					Avx = 0;
					Avy = 0;
					s_nu = 0;
					s_de = 0;
					s = 0;

					xk1Buff = new double[block_num];
					yk1Buff = new double[block_num];
					xkBuff = new double[block_num];
					ykBuff = new double[block_num];

					//get Intercoded macroblock in P-frame
					//检测帧的类型
					//一帧一个循环
					richTextBox->AppendText("At Frame ");
					richTextBox->AppendText(i.ToString() + ": \n");
					count = 0;
					for (int j = 0; j < block_num; j++)
					{

						//get type
						mbtype[j] = pCompressedPic->pnMBType[j];

						//检测宏块类型
						//type：MACROBLOCK_INTRA（I）， MACROBLOCK_MOTION_BACKWARD & MACROBLOCK_MOTION_FORWARD（B），MACROBLOCK_MOTION_FORWARD（P）
						//如果该宏块有前向预测变量
						if (mbtype[j] & MACROBLOCK_MOTION_FORWARD)
						{
							//(jk, ik)是每个16*16 y block里的中心点，也就是（3，3）那个点
							ik = (int)(j / wYPicWidth) * 16 + 8;
							jk = (int)(j % wYPicWidth) * 16 + 8;

							//得到xy方向运动矢量
							mvi = pCompressedPic->pshSet1FwdPMVy[Num_Inter];
							mvj = pCompressedPic->pshSet1FwdPMVx[Num_Inter];

							//sxk1, syk1, sxk, syk里的s是sum
							//(j0, i0)是每个帧的中心点
							//计算每个宏块运动前与帧中心点的差值
							xk1Buff[Num_Inter] = jk - j0;
							sxk1 = sxk1 + jk - j0;
							yk1Buff[Num_Inter] = i0 - ik;
							syk1 = syk1 + i0 - ik;
							//计算每个宏块运动后与帧中心点的差值
							xkBuff[Num_Inter] = (jk + mvj) - j0;
							sxk = sxk + (jk + mvj) - j0;
							ykBuff[Num_Inter] = i0 - (ik + mvi);
							syk = syk + i0 - (ik + mvi);
							count++;
							//显示motion vector
							richTextBox->AppendText("(" + mvj + "," + mvi + ") ");
							//一排结束以后换行
							if (count == wYPicWidth) {
								count = 0;
								richTextBox->AppendText("\n");
							}
							Num_Inter++;
						}
					}

					richTextBox->AppendText("\n");
					richTextBox->AppendText("\n");
					richTextBox->Update();

					//A表示average
					Avx1 = sxk1 / Num_Inter;
					Avy1 = syk1 / Num_Inter;
					Avx = sxk / Num_Inter;
					Avy = syk / Num_Inter;

					////////////////////

					//后面都是用motion vector检测特写镜头
					/*for (int k = 0; k < Num_Inter; k++)
					{
					s_nu = s_nu + (xk1Buff[k] - Avx1) * (xkBuff[k] - Avx) + (yk1Buff[k] - Avy1) * (ykBuff[k] - Avy);
					s_de = s_de + (xkBuff[k] - Avx) *(xkBuff[k] - Avx) + (ykBuff[k] - Avy) * (ykBuff[k] - Avy);
					}

					s = s_nu / s_de;*/

					//MpvDecoder.GetCurFrameNum(&crt_num);


					//if (s>1)
					//{
					//	zoomin_count++;

					//	if (zoomin_count == 1)
					//	{
					//		smax = s;
					//		smin = s;
					//	}

					//	if (s>smax) smax = s;
					//	if (s<smin) smin = s;

					//	//  this->richTextBox1->AppendText("Zoom in is detected at the ");
					//	this->richTextBox1->AppendText("Zoom in is detected: s=");
					//	this->richTextBox1->AppendText(s.ToString());
					//	this->richTextBox1->AppendText("   at");

					//	this->richTextBox1->AppendText(crt_num.ToString());
					//	this->richTextBox1->AppendText(" Frame\n");
					//	this->richTextBox1->Update();
					//}

					//if (s == 1)
					//{
					//	zoomin_count = 0;

					//	smax = 0;
					//	smin = 0;

					//	this->richTextBox1->AppendText("No Zoom is detected at the ");
					//	this->richTextBox1->AppendText(crt_num.ToString());
					//	this->richTextBox1->AppendText(" Frame\n");
					//	this->richTextBox1->Update();
					//}


					//if (s<1)
					//{
					//	zoomin_count = 0;

					//	smax = 0;
					//	smin = 0;

					//	this->richTextBox1->AppendText("Zoom Out is detected at the ");
					//	this->richTextBox1->AppendText(crt_num.ToString());
					//	this->richTextBox1->AppendText(" Frame\n");
					//	this->richTextBox1->Update();
					//}

					//zincountBuff[Pframe_num] = zoomin_count;
					//zinframeBuff[Pframe_num] = crt_num;
					//zinsBuff[Pframe_num] = s;
					//zinsmaxBuff[Pframe_num] = smax;
					//zinsminBuff[Pframe_num] = smin;


					////  if(zincountBuff[Pframe_num-1]>=1 && zincountBuff[Pframe_num] == 0 && zinsBuff[Pframe_num-1]>1.001)
					//if (zincountBuff[Pframe_num - 1] == 1 && zincountBuff[Pframe_num] == 0 && zinsBuff[Pframe_num - 1]>1.00109)
					//{
					//	cframeBuff[closeup_count] = zinframeBuff[Pframe_num - 1];

					//	closeup_count++;

					//	/*this->richTextBox1->AppendText("\nClose-up is detected at the ");
					//	this->richTextBox1->AppendText(zinframeBuff[Pframe_num - 1].ToString());
					//	this->richTextBox1->AppendText(" Frame\n\n");
					//	this->richTextBox1->Update();*/

					//}

					//if (zincountBuff[Pframe_num - 1]>1 && zincountBuff[Pframe_num] == 0 && ((zinsmaxBuff[Pframe_num - 1] - 1) / (zinsminBuff[Pframe_num - 1] - 1))>4)
					//{
					//	cframeBuff[closeup_count] = zinframeBuff[Pframe_num - 1];

					//	closeup_count++;

					//	/*this->richTextBox1->AppendText("\nClose-up is detected at the ");
					//	this->richTextBox1->AppendText(zinframeBuff[Pframe_num - 1].ToString());
					//	this->richTextBox1->AppendText(" Frame\n\n");
					//	this->richTextBox1->Update();*/

					//}

					//Pframe_num++;


					free(xk1Buff);
					free(yk1Buff);
					free(xkBuff);
					free(ykBuff);

				}


				if (MpvDecoder.MoveToNextFrame() != MDC_SUCCESS)
					return;
			}


			free(mbtype);
			free(zincountBuff);
			free(zinframeBuff);
			free(zinsBuff);

			free(zinsmaxBuff);
			free(zinsminBuff);

			free(cframeBuff);
			MpvDecoder.FreeCompressedPic();


		}

	////////////////////////////
	// MOTION VECTOR
	///////////////////////////


	private:
		System::Void motionVectorpsButton_click(System::Object ^  sender, System::EventArgs ^  e)
		{
			const HEADERDATA *pMpvHeaderData;
			char pFileName[200];
			CMpvDecoder MpvDecoder;
			WORD PicWidth, PicHeight, CrPicWidth, CrPicHeight, wYPicWidth, wYPicHeight;
			long lResult;
			long lastnum;
			BYTE type1;
			int Num_Inter;
			double block_num;
			COMPRESSEDPIC *pCompressedPic;
			int i0, j0;

			int set1, set2;
			double* alphasBuff;
			double* psBuff;

			if (this->openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
			{
				int i = this->openFileDialog->FileName->Length;
				for (int j = 0; j < i; j++)
				{
					pFileName[j] = this->openFileDialog->FileName->ToCharArray()[j];
				}
				for (int j = i; j < 200; j++)
				{
					pFileName[j] = 0;
				}
				if ((lResult = MpvDecoder.Initialize(pFileName)) != MDC_SUCCESS)
					return;

			}

			HRESULT hr;

			MpvDecoder.GetMaxFrameNum(&lastnum);

			pMpvHeaderData = MpvDecoder.GetMpvHeaders();
			SEQUENCEHEADER sequencehead = pMpvHeaderData->SequenceHeader;
			i0 = sequencehead.wVerticalSize / 2;
			j0 = sequencehead.wHorizontalSize / 2;

			PicWidth = sequencehead.wHorizontalSize;
			PicHeight = sequencehead.wVerticalSize;
			CrPicWidth = PicWidth / 2;
			CrPicHeight = PicHeight / 2;


			wYPicWidth = sequencehead.wHorizontalSize / 16;
			wYPicHeight = sequencehead.wVerticalSize / 16;
			block_num = wYPicWidth * wYPicHeight;

			pCompressedPic = MpvDecoder.PrepareCompressedPic();
			alphasBuff = new double[lastnum];
			psBuff = new double[lastnum];
			int k = 0;
			double alpha_i = 0;
			double alpha_j = 0;

			for (int i = 0; i < (int)lastnum; i++)
			{
				set1 = 0;
				set2 = 0;
				lResult = MpvDecoder.GetCompressedPic();
				pMpvHeaderData = MpvDecoder.GetMpvHeaders();
				type1 = pMpvHeaderData->PictureHeader.nPictureCodingType;

				if (type1 == P_TYPE)
				{
					//算阿尔法i
					alpha_i = calculateAlpha(pCompressedPic, block_num);
					alphasBuff[k] = alpha_i;
					for (int j = 0; j < k + 1; j++) {
						//比较alpha_j和alpha_i的大小
						alpha_j = alphasBuff[j];
						if (alpha_j > alpha_i) {
							set1++;
						}
						if (alpha_j == alpha_i) {
							set2++;
						}
					}
					srand(time(NULL));
					double random = rand() / double(RAND_MAX);
					psBuff[k] = (set1 + random * set2) / (k + 1);
					std::cout << "alphasBuff[" << k << "] = " << alphasBuff[k]
						<< "      psBuff[" << k << "] = " << psBuff[k] << "\n";
					k++;
				}
				if (MpvDecoder.MoveToNextFrame() != MDC_SUCCESS)
					return;
			}
			MpvDecoder.FreeCompressedPic();
			delete[] alphasBuff;
			delete[] psBuff;
		}

		double calculateAlpha(COMPRESSEDPIC *pCompressedPic, int block_num) {
			int Num_Inter = 0;
			double mvi = 0;
			double mvj = 0;
			double mv = 0;
			double avg_mv = 0;
			for (int k = 0; k < block_num; k++)
			{
				//如果该宏块有前向预测变量
				if (pCompressedPic->pnMBType[k] & MACROBLOCK_MOTION_FORWARD)
				{
					//得到xy方向运动矢量
					mvi = pCompressedPic->pshSet1FwdPMVy[Num_Inter];
					mvj = pCompressedPic->pshSet1FwdPMVx[Num_Inter];
					mv += sqrt(mvi * mvi + mvj * mvj);
					Num_Inter++;
				}
			}
			avg_mv = mv / block_num;
			return avg_mv;
		};



		/////////////////////////////////////////
		// VIDEO EXTRACTION
		/////////////////////////////////////////



		System::Void videoExtractionButton_click(System::Object ^  sender, System::EventArgs ^  e)
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

			//for (int i = 0; i < (int)(lastnum - 1); i++)
			//{
			lResult = MpvDecoder.GetCompressedPic();
			pMpvHeaderData = MpvDecoder.GetMpvHeaders();
			Byte type1 = pMpvHeaderData->PictureHeader.nPictureCodingType;

			if (type1 == I_TYPE)
			{
				//原大小重构 
				//因为太慢了 先把它注释掉好了
				//System::Drawing::Bitmap^ bitmap3 = gcnew System::Drawing::Bitmap((int)PicWidth, (int)PicHeight,
				//	System::Drawing::Imaging::PixelFormat::Format24bppRgb);
				//System::Drawing::Rectangle^ rect3 = gcnew System::Drawing::Rectangle(0, 0, (int)PicWidth, (int)PicHeight);

				//System::Drawing::Imaging::BitmapData^ bmData3 = bitmap3->LockBits(
				//	*rect3,
				//	System::Drawing::Imaging::ImageLockMode::ReadWrite,
				//	System::Drawing::Imaging::PixelFormat::Format24bppRgb);

				//m_stride = bmData3->Stride;
				//m_scan0 = bmData3->Scan0.ToInt32();
				//BYTE* pc = (BYTE*)(void*)m_scan0;

				//BYTE* pYBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 4];
				//BYTE* pCbBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				//BYTE* pCrBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				//BYTE* pRGBBuff3 = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 12];

				//Y = pCompressedPic->pshYDCTBlock;
				////wYPicWidth, wYPicHeight表示横竖方向有多少个block?
				//for (int j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
				//{
				//	int x = (j % (wYPicWidth * 2)) * 8;
				//	int y = (j / (wYPicWidth * 2)) * 8;
				//	assignPixels(pYBuff3, x, y, PicWidth, Y, j);
				//}

				//Cb = pCompressedPic->pshCbDCTBlock;
				//Cr = pCompressedPic->pshCrDCTBlock;
				//for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
				//{
				//	int x = (j % wYPicWidth) * 8;
				//	int y = (j / wYPicWidth) * 8;
				//	assignPixels(pCbBuff3, x, y, PicWidth / 2, Cb, j);
				//	assignPixels(pCrBuff3, x, y, PicWidth / 2, Cr, j);
				//}

				//conv.YV12_to_RGB24(pYBuff3, pCbBuff3, pCrBuff3, pRGBBuff3, wYPicWidth * 16, wYPicHeight * 16);

				//BYTE* qc = pRGBBuff3 + wYPicWidth * 8 * 6 * (wYPicHeight * 8 * 2 - 1);

				//for (int j = 0; j < wYPicHeight * 8 * 2; j++)
				//{
				//	for (int k = 0; k < wYPicWidth * 8 * 2; k++)
				//	{
				//		pc[0] = qc[k * 3];
				//		pc[1] = qc[k * 3 + 1];
				//		pc[2] = qc[k * 3 + 2];
				//		pc = pc + 3;

				//	}
				//	qc = qc - wYPicWidth * 8 * 6;
				//}

				//bitmap3->UnlockBits(bmData3);

				//this->pictureBox1->Image = bitmap3;
				//this->pictureBox1->Update();

				//delete[] pYBuff3;
				//delete[] pCbBuff3;
				//delete[] pCrBuff3;
				//delete[] pRGBBuff3;

				//原大小抽取9个系数
				System::Drawing::Bitmap^ bitmap = gcnew System::Drawing::Bitmap((int)PicWidth, (int)PicHeight,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb);
				System::Drawing::Rectangle^ rect = gcnew System::Drawing::Rectangle(0, 0, (int)PicWidth, (int)PicHeight);

				System::Drawing::Imaging::BitmapData^ bmData = bitmap->LockBits(
					*rect,
					System::Drawing::Imaging::ImageLockMode::ReadWrite,
					System::Drawing::Imaging::PixelFormat::Format24bppRgb);

				int m_stride = bmData->Stride;
				int m_scan0 = bmData->Scan0.ToInt32();
				BYTE* pc = (BYTE*)(void*)m_scan0;

				BYTE* pYBuff = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 4];
				BYTE* pCbBuff = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				BYTE* pCrBuff = new BYTE[wYPicWidth * 8 * wYPicHeight * 8];
				BYTE* pRGBBuff = new BYTE[wYPicWidth * 8 * wYPicHeight * 8 * 12];

				DCTBLOCK* Y = pCompressedPic->pshYDCTBlock;
				for (int j = 0; j < wYPicWidth*wYPicHeight * 4; j++)
				{
					int x = (j % (wYPicWidth * 2)) * 8;
					int y = (j / (wYPicWidth * 2)) * 8;
					//printf("Y block %d: \n", j);
					//assign4m4Pixels(pYBuff, x, y, PicWidth, Y, j);
					//assign2m2Pixels(pYBuff, x, y, PicWidth, Y, j);
					assign4m4_16DctPixels(pYBuff, x, y, PicWidth, Y, j);
				}

				DCTBLOCK* Cb = pCompressedPic->pshCbDCTBlock;
				DCTBLOCK* Cr = pCompressedPic->pshCrDCTBlock;
				for (int j = 0; j < wYPicWidth*wYPicHeight; j++)
				{
					int x = (j % wYPicWidth) * 8;
					int y = (j / wYPicWidth) * 8;
					//printf("Cb block %d: \n", j);
					//assign4m4Pixels(pCbBuff, x, y, PicWidth / 2, Cb, j);
					//printf("Cr block %d: \n", j);
					//assign4m4Pixels(pCrBuff, x, y, PicWidth / 2, Cr, j);
					//assign2m2Pixels(pCbBuff, x, y, PicWidth / 2, Cb, j);
					//assign2m2Pixels(pCrBuff, x, y, PicWidth / 2, Cr, j);
					assign4m4_16DctPixels(pCbBuff, x, y, PicWidth / 2, Cb, j);
					assign4m4_16DctPixels(pCrBuff, x, y, PicWidth / 2, Cr, j);
				}
				ColorSpaceConversions conv;
				conv.YV12_to_RGB24(pYBuff, pCbBuff, pCrBuff, pRGBBuff, wYPicWidth * 16, wYPicHeight * 16);

				BYTE* qc = pRGBBuff + wYPicWidth * 8 * 6 * (wYPicHeight * 8 * 2 - 1);

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

				bitmap->UnlockBits(bmData);

				this->pictureBox->Image = bitmap;
				this->pictureBox->Update();

				delete[] pYBuff;
				delete[] pCbBuff;
				delete[] pCrBuff;
				delete[] pRGBBuff;

			}
			if (MpvDecoder.MoveToNextFrame() != MDC_SUCCESS) {
				return;
				//}
			}

			MpvDecoder.FreeCompressedPic();
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





		void assign2m2Pixels(Byte * pbuff, int x, int y, int num_per_row, DCTBLOCK * C, int blocknum)
		{
			int index = 0;
			short A00 = (short)((C[blocknum][0] + C[blocknum][8] + C[blocknum][1] + C[blocknum][9]) >> 3);
			short A01 = (short)((C[blocknum][0] + C[blocknum][8] - C[blocknum][1] - C[blocknum][9]) >> 3);
			short A10 = (short)((C[blocknum][0] - C[blocknum][8] + C[blocknum][1] - C[blocknum][9]) >> 3);
			short A11 = (short)((C[blocknum][0] - C[blocknum][8] - C[blocknum][1] + C[blocknum][9]) >> 3);
			for (int i = 0; i < 8; i++)
			{
				for (int j = 0; j < 8; j++)
				{
					index = x + i + (y + j) * num_per_row;
					switch (i)
					{
					case 0:
					case 1:
					case 2:
					case 3:
						switch (j)
						{
						case 0:
						case 1:
						case 2:
						case 3:
							pbuff[index] = (Byte)(A00 + 128);
							break;
						case 4:
						case 5:
						case 6:
						case 7: //x30
							pbuff[index] = (Byte)(A10 + 128);
							break;
						}
						break;
					case 4:
					case 5:
					case 6:
					case 7:
						switch (j)
						{
						case 0:
						case 1:
						case 2:
						case 3: //x13
							pbuff[index] = (Byte)(A01 + 128);
							break;
						case 4:
						case 5:
						case 6:
						case 7: //x33
							pbuff[index] = (Byte)(A11 + 128);
						}
						break;
					}
				}
			}

		}

		void assign4m4Pixels(Byte * pbuff, int x, int y, int num_per_row, DCTBLOCK * C, int blocknum)
		{
			int index = 0;
			ValueBlocks VB;
			assign4m4_9Values(C, blocknum, VB);
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

		void assign4m4_16DctPixels(Byte * pbuff, int x, int y, int num_per_row, DCTBLOCK * C, int blocknum) {
			int index = 0;
			ValueBlocks VB;
			assign4m4_9Values(C, blocknum, VB);
			assign4m4_7Values(C, blocknum, VB);
			//short temp[64];
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
							pbuff[index] = (VB.x00 + 128);
							//temp[i * 8 + j] = (C[j][24] + C[j][26] + C[j][3] + C[j][19] + C[j][27]) >> 4 + (C[j][25] + C[j][1]) >> 3 + 128;
							break;
						case 2:
						case 3: //x10
							pbuff[index] = (VB.x10 + 128);
							break;
						case 4:
						case 5: //x20
							pbuff[index] = (VB.x20 + 128);
							break;
						case 6:
						case 7: //x30
							pbuff[index] = (VB.x30 + 128);
							break;
						}
						break;
					case 2:
					case 3:
						switch (j)
						{
						case 0:
						case 1: //x01
							pbuff[index] = (VB.x01 + 128);
							break;
						case 2:
						case 3: //x11
							pbuff[index] = (VB.x11 + 128);
							break;
						case 4:
						case 5: //x21
							pbuff[index] = (VB.x21 + 128);
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

		void assign4m4_9Values(DCTBLOCK * C, int j, ValueBlocks& VB) {
			ABBlocks AB;
			assignBlock(C, AB, j);
			VB.x00 = ((AB.A1 + AB.A3 + AB.B3 + AB.B1 + AB.C11) >> 3) + ((AB.B3 + AB.B1 + AB.C11) >> 5);
			VB.x10 = ((AB.A2 + AB.A4 + AB.B2) >> 3) + (AB.B2 >> 5) + (AB.B3 + AB.C11 >> 4);
			VB.x20 = ((AB.A2 + AB.A4 + AB.B2) >> 3) + (AB.B2 >> 5) - (AB.B3 + AB.C11 >> 4);
			VB.x30 = ((AB.A1 + AB.A3) >> 3) - ((AB.B3 - AB.B1 + AB.C11) >> 4);
			VB.x01 = ((AB.A1 - AB.A3 + AB.B3) >> 3) + (AB.B3 >> 5) + (AB.B1 + AB.C11 >> 4);
			VB.x11 = ((AB.A2 - AB.A4) >> 3) + ((AB.B4 + AB.B2) >> 4);
			VB.x21 = ((AB.A2 - AB.A4) >> 3) - ((AB.B4 - AB.B2) >> 4);
			VB.x31 = ((AB.A1 - AB.A3 - AB.B4) >> 3) - (AB.B4 >> 5) + (AB.B1 + AB.C11 >> 4);
			VB.x02 = ((AB.A1 - AB.A3 + AB.B4) >> 3) + (AB.B4 >> 5) - (AB.B1 + AB.C11 >> 4);
			VB.x12 = ((AB.A2 - AB.A4) >> 3) + (AB.B4 - AB.B2 >> 4);
			VB.x22 = ((AB.A2 - AB.A4) >> 3) - ((AB.B4 + AB.B2) >> 4);
			VB.x32 = ((AB.A1 - AB.A3 - AB.B4) >> 3) - (AB.B4 >> 5) - ((AB.B1 - AB.C11) >> 4);
			VB.x03 = ((AB.A1 + AB.A3 + AB.B4 - AB.B1 - AB.C11) >> 3) + ((AB.B4 - AB.B1 - AB.C11) >> 5);
			VB.x13 = ((AB.A2 + AB.A4 - AB.B2) >> 3) - (AB.B2 >> 5) + (AB.B3 - AB.C11 >> 4);
			VB.x23 = ((AB.A2 + AB.A4 - AB.B2) >> 3) - (AB.B2 >> 5) - (AB.B3 - AB.C11 >> 4);
			VB.x33 = ((AB.A1 + AB.A3 - (AB.B4 + AB.B1 - AB.C11)) >> 3) - ((AB.B4 + AB.B1 - AB.C11) >> 5);
		}

		void assign4m4_7Values(DCTBLOCK * C, int j, ValueBlocks& VB)
		{
			VB.x00 += ((C[j][24] + C[j][26] + C[j][3] + C[j][19] + C[j][27]) >> 4) + ((C[j][25] + C[j][11]) >> 3);
			VB.x10 += -((C[j][24] + C[j][26] + C[j][27]) >> 3) + ((C[j][3] - C[j][19]) >> 4) - (C[j][25] >> 2) + (C[j][11] >> 5);
			VB.x20 += ((C[j][24] + C[j][26] + C[j][27]) >> 3) + ((C[j][3] - C[j][19]) >> 4) + (C[j][25] >> 2) - (C[j][11] >> 5);
			VB.x30 += -((C[j][24] + C[j][26] - C[j][3] - C[j][19] + C[j][27]) >> 4) + ((C[j][25] + C[j][11]) >> 3);
			VB.x01 += ((C[j][24] - C[j][26]) >> 4) - ((C[j][3] + C[j][19] - C[j][27]) >> 3) + (C[j][25] >> 5) - (C[j][11] >> 2);
			VB.x11 += -((C[j][24] - C[j][26] + C[j][3] - C[j][19]) >> 3) - ((C[j][25] + C[j][11]) >> 4) + (C[j][27] >> 2);
			VB.x21 += ((C[j][24] - C[j][26] - C[j][3] + C[j][19]) >> 3) + ((C[j][25] + C[j][11]) >> 4) - (C[j][27] >> 2);
			VB.x31 += -((C[j][24] - C[j][26]) >> 4) - ((C[j][3] + C[j][19] - C[j][27]) >> 3) - (C[j][25] >> 5) + (C[j][11] >> 2);
			VB.x02 += ((C[j][24] - C[j][26]) >> 4) + ((C[j][3] + C[j][19] + C[j][27]) >> 3) - (C[j][25] >> 5) + (C[j][11] >> 2);
			VB.x12 += -((C[j][24] - C[j][26] - C[j][3] + C[j][19]) >> 3) + ((C[j][25] + C[j][11]) >> 4) - (C[j][27] >> 2);
			VB.x22 += ((C[j][24] - C[j][26] + C[j][3] - C[j][19]) >> 3) - ((C[j][25] + C[j][11]) >> 4) + (C[j][27] >> 2);
			VB.x32 += -((C[j][24] - C[j][26]) >> 4) + ((C[j][3] + C[j][19] - C[j][27]) >> 3) + (C[j][25] >> 5) - (C[j][11] >> 2);
			VB.x03 += ((C[j][24] + C[j][26] - C[j][3] - C[j][19] - C[j][27]) >> 4) + ((C[j][25] + C[j][11]) >> 3);
			VB.x13 += -((C[j][24] + C[j][26] - C[j][27]) >> 3) - ((C[j][3] - C[j][19]) >> 4) + (C[j][25] >> 2) - (C[j][11] >> 5);
			VB.x23 += ((C[j][24] + C[j][26] - C[j][27]) >> 3) - ((C[j][3] - C[j][19]) >> 4) - (C[j][25] >> 2) + (C[j][11] >> 5);
			VB.x33 += -((C[j][24] + C[j][26] + C[j][3] + C[j][19] - C[j][27]) >> 4) + ((C[j][25] + C[j][11]) >> 3);

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
			//printf("C00 %d, C10 %d, C01 %d, C20 %d\n", C[i][0], C[i][8], C[i][1], C[i][16]);
		}


	};


}