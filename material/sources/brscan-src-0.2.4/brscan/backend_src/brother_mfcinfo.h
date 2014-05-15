/*

 This file is part of the Brother MFC/DCP backend for SANE.

 This program is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2 of the License, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place, Suite 330, Boston, MA  02111-1307  USA

*/
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//	Source filename: brother_mfcinfo.h
//
//		Copyright(c) 1997-2000 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			デバイス情報に関する構造体／シンボル定義
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef _BROTHER_MFCINFO_H_
#define _BROTHER_MFCINFO_H_

#include "brother_dtype.h"

//
// MFCモデル・タイプ
//
#define MODEL_YL1_1      0		// 4550
#define MODEL_YL1_2      1		// 6550,7550
#define MODEL_BY1_US     2		// 7000,7200
#define MODEL_BY1_EUR    3		// BY Eurlope
#define MODEL_BY1_3IN1   4		// MC3000
#define MODEL_YL2_US    10		// YL2 USA model
#define MODEL_YL2_EUR   11		// YL2 Eurlope model
#define MODEL_BY2_US    12		// BY2 USA Color model
#define MODEL_BY2_EUR   13		// BY2 Eurlope Color model
#define MODEL_YL3       14		// YL3 series
#define MODEL_BY4       15		// BY4 series
#define MODEL_YL4       16		// YL4 series
#define MODEL_ZLe       17		// ZLe series
#define MODEL_BHL       18		// BH-low series

#define BIT_YL		0x100		// YL bit
#define BIT_BY		0x200		// BY bit
#define BIT_YL2		0x400		// YL2 bit
#define BIT_BY2		0x800		// BY2 bit
#define BIT_3IN1	0x1000		// 3in1 bit
#define BIT_EUR		0x2000		// Eurlope

typedef struct tagMFCMODELINFO {
	WORD  wModelType;		// MFCモデル・タイプ
	WORD  wDialogType;		// Dialog(UI)タイプ
	BOOL  bColorModel;		// カラースキャナ・モデル
	BOOL  b3in1Type;		// 3in1モデル(MC3000)
	BOOL  bDither;			// Ditherサポート・モデル
	BOOL  bVideoCap;		// VideoCaptureサポート・モデル
	BOOL  bQcmdEnable;		// Q-commandサポート・モデル
} MFCMODELINFO, *LPMFCMODELINFO;

#define TWDSUI_YL    0
#define TWDSUI_3IN1  1
#define TWDSUI_BY    2
#define TWDSUI_NOVC  3


#pragma pack(1)
//
// デバイスがサポートするカラータイプ
//
typedef union tagMFCCOLORTYPE {
	struct {
		BYTE  bBlackWhite:1;	// ２値（白黒）
		BYTE  bErrorDiffusion:1;// 誤差拡散
		BYTE  bTrueGray:1;	// グレースケール
		BYTE  b256Color:1;	// 256色カラー
		BYTE  b24BitColor:1;	// 24bitカラー
		BYTE  b256IndexColor:1;	// 256色カラー（MFCからパレットテーブル取得）
	} bit;
	BYTE val;
} MFCCOLORTYPE, *LPMFCCOLORTYPE;

#define MFCDEVINFMONO   0x07		// 白黒モデルのデフォルトカラータイプ
					//   (B&W, ErrorDiff, TrueGray)
#define MFCDEVINFCOLOR  0x1F		// カラーモデルのデフォルトカラータイプ
					//   (B&W, ErrorDiff, TrueGray, 256Color, 24bit)


//
// デバイスのスキャナ／ビデオ能力情報ヘッダー
//
typedef struct tagMFCDEVICEHEAD {
	WORD  wDeviceInfoID;		// デバイス情報のID
	BYTE  nInfoSize;		// デバイス情報のサイズ（IDはサイズに含まない）
	BYTE  nProtcolType; 		// DS<->MFC間のプロトコル種別
					// 00h=〜1999年モデル, 01h=2000年モデル
} MFCDEVICEHEAD, *LPMFCDEVICEHEAD;

#define MFCDEVICEINFOID  0x00C1		// デバイス情報のID値
#define MFCPROTCOL1998   0		// 〜1999年モデル・プロトコル
#define MFCPROTCOL2000   1		// 2000年モデル・プロトコル


//
// デバイスのスキャナ／ビデオ能力情報
//
typedef struct tagMFCDEVICEINFO {
	BYTE          nVideoSignalType; // ビデオの信号種別
					//   00h=Reserve, FFh=ビデオ無し, 01h=NTSC, 02h=PAL
	MFCCOLORTYPE  nColorType;	// スキャナの対応カラータイプ
					//   00h=Reserve, MSB|0:0:256ix:24c:256c:TG:ED:BW|LSB
	WORD          nVideoNtscSignal; // NTSC信号規格
					//   0=Reserve 1:B,2:G,3:H,4:I,5:D,6:K,7:K1,8:L,9:M,10:N
	WORD          nVideoPalSignal;	// PAL信号規格
					//   0=Reserve 1:B,2:G,3:H,4:I,5:D,6:K,7:K1,8:L,9:M,10:N
	WORD          nVideoSecamSignal;// SECAM信号規格
					//   0=Reserve 1:B,2:G,3:H,4:I,5:D,6:K,7:K1,8:L,9:M,10:N
	BYTE          nHardwareType;	// 基板の種類
					//   00h=Reserve, 01h=NTSC基板 02h=NTSC/Lexmark基板 81h=PAL基板
	BYTE          nHardwareVersion; // 基板のバージョン
					//   00h=Reserve, 01h〜=バージョン番号
	BYTE          nMainScanDpi;	// スキャナ主走査解像度の能力
					//   00h=Reserve, 01h=200dpi, 02h=200,300dpi, 03h=100,200,300dpi
	BYTE          nPaperSizeMax;	// スキャナ対応用紙サイズ
					//   00h=Reserve, 01h=A4, 02h=B4
} MFCDEVICEINFO, *LPMFCDEVICEINFO;

#define MFCVIDEONONE     0xFF		// ビデオ無し
#define MFCVIDEONTSC     1		// ビデオの信号種別はNTSC
#define MFCVIDEOPAL      2		// ビデオの信号種別はPAL
#define MFCVIDEOSECAM    3		// ビデオの信号種別はSecam

#define MFCHARDNTSC      1		// 基板の種類はNTSC基板
#define MFCHARDLEX       2		// 基板の種類はNTSC/Lexmark基板
#define MFCHARDPAL       0x81		// 基板の種類はPAL基板

#define MFCMAINSCAN200   1		// 主走査解像度の能力は200dpiのみ
#define MFCMAINSCAN300   2		// 主走査解像度の能力は200,300dpi
#define MFCMAINSCAN100   3		// 主走査解像度の能力は100,200,300dpi

#pragma pack()
//
// デバイスのスキャナ／ビデオ能力情報のサイズ（デバイス情報のIDはサイズに含まない）
//
#define MFCDEVINFOFULLSIZE  sizeof( BYTE ) * 2 + sizeof( MFCDEVICEINFO )


//
// デバイスのスキャナ情報／スキャンパラメータ
//
typedef struct tagDEVSCANINFO {
	WORD        wResoType;		// スキャン解像度タイプ番号
	WORD        wColorType;		// スキャンするカラータイプ
	RESOLUTION  DeviceScan; 	// スキャンする実解像度
	WORD        wScanSource;	// スキャンソース
	DWORD       dwMaxScanWidth; 	// 読み取り最大幅（0.1mm単位）
	DWORD       dwMaxScanPixels;	// 読み取り最大幅（Pixel数）
	DWORD       dwMaxScanHeight;	// 読み取り最大長（0.1mm単位）
	DWORD       dwMaxScanRaster;	// 読み取り最大長（ラスタ数）
	AREARECT    ScanAreaDot;	// スキャン範囲指定（dot単位）
	AREASIZE    ScanAreaSize;	// 読み取り範囲（ドット数）
	AREASIZE    ScanAreaByte;	// 読み取り範囲（バイト数）
} DEVSCANINFO, *LPDEVSCANINFO;

#define MFCSCANSRC_ADF  1		// スキャンソースはADF
#define MFCSCANSRC_FB   2		// スキャンソースはFB
#define MFCSCANSRC_ADF_DUP  3           //06/02/27 スキャンソースはADFの両面

#define MFCSCANMAXWIDTH      2080	// 最大読み取り幅（0.1mm単位）
#define MFCSCAN200MAXPIXEL   1664	// 最大読み取りドット数（200dpi）
#define MFCSCAN300MAXPIXEL   2464	// 最大読み取りドット数（300dpi）
#define MFCSCANMAXHEIGHT     3476	// ADF最大読み取り長（0.1mm単位）
#define MFCSCAN400MAXRASTER  5474	// ADF最大読み取りラスタ数（400dpi）
#define MFCSCAN600MAXRASTER  8210	// ADF最大読み取りラスタ数（600dpi）
#define MFCSCANFBHEIGHT      2910	// FB最大読み取り長（0.1mm単位）
#define MFCSCAN400FBRASTER   4582	// FB最大読み取りラスタ数（400dpi）
#define MFCSCAN600FBRASTER   6874	// FB最大読み取りラスタ数（600dpi）

#endif //_BROTHER_MFCINFO_H_

//////// end of brother_mfcinfo.h ////////
