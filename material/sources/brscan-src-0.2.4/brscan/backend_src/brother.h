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

#ifndef _H_BROTHER
#define _H_BROTHER

/* ======================================================================

brother.h

SANE backend master module.

Definitions ported from "scantool.h" 5.4.2001.

(C) Marian Matthias Eichholz 2001

Start: 2.4.2001

====================================================================== */

#define DEBUG_SCAN     0x0001
#define DEBUG_COMM     0x0002
#define DEBUG_ORIG     0x0004
#define DEBUG_BASE     0x0011
#define DEBUG_DEVSCAN  0x0012
#define DEBUG_REPLAY   0x0014
#define DEBUG_BUFFER   0x0018
#define DEBUG_SIGNALS  0x0020
#define DEBUG_CALIB    0x0040

#define DEBUG_CRITICAL 1
#define DEBUG_VERBOSE  2
#define DEBUG_INFO     3
#define DEBUG_JUNK     5

#define SCANNER_VENDOR     0x04F9		// Brother Vendor ID

/* ====================================================================== */

#include "sane/sane.h"

#include "brother_dtype.h"

#include "brother_scandec.h"
#include "brcolor.h"

#include "brother_modelinf.h"
#include "brother_mfcinfo.h"

typedef SANE_Status TState;


typedef struct TScanState {
  BOOL           bEOF;         /* EOF marker for sane_read */
  BOOL           bCanceled;    /* Cancel flag */ 
  BOOL           bScanning;    /* block is active? */
  int		 iProcessEnd;  /* End marker for ProcessMain */
  BOOL           bReadbufEnd;  /* Read buffer Receive End */
  int            nPageCnt;     /* Page count */
} TScanState;


#ifndef INSANE_VERSION

typedef enum { optCount,		// 0
	       optGroupMode, 		// 1
	       optMode, optResolution,  // 2, 3
	       optScanSrc,              // 4
	       optBrightness, optContrast,  // 5, 6
	       optGroupGeometry,optTLX, optTLY, optBRX, optBRY, // 7, 8, 9, 10, 11
	       optLast } TOptionIndex; // 12

#define NUM_OPTIONS optLast

typedef union
  {  
    SANE_Word w;
    SANE_Word *wa;              /* word array */
    SANE_String s;
  }
TOptionValue;

typedef struct TDevice {
  struct TDevice        *pNext;
  struct usb_device     *pdev;
#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
  int                   index;
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
  SANE_Device            sane;
  MODELINF               modelInf;
} TDevice;

#endif


//
// UI（Frontend）設定
//
typedef struct tagUISETTING {
	WORD          wResoType;		// 解像度タイプ
	WORD          wColorType;		// カラータイプ
	int           nBrightness;		// Brightness設定
	int           nContrast;		// Contrast設定
	RESOLUTION    UserSelect;		// 選択された解像度
	AREARECT      ScanAreaMm;		// スキャン範囲指定（0.1mm単位）
	AREARECT      ScanAreaDot;		// スキャン範囲指定（dot単位）
	RESOLIST      ResoList;			// UI上の解像度設定項目リスト
	SCANMODELIST  ScanModeList;		// UI上のスキャンモード設定項目リスト
	SCANSRCLIST   ScanSrcList;   		// UI上のスキャンソース設定項目リスト
	int           nSrcType;			// 06/02/28  選択されたスキャンソース
} UISETTING, *LPUISETTING;

//
// スキャン時の設定情報／スキャンパラメータ
//
typedef struct tagSCANINFO {
	RESOLUTION  UserSelect;			// 選択された解像度
	AREARECT    ScanAreaMm;			// スキャン範囲指定（mm単位）
	AREARECT    ScanAreaDot;		// スキャン範囲指定（dot単位）
	AREASIZE    ScanAreaSize;		// 読み取り範囲（ドット数）
	AREASIZE    ScanAreaByte;		// 読み取り範囲（バイト数）
} SCANINFO, *LPSCANINFO;

typedef struct TScanDec {
  void              *hScanDec;               // 圧縮データ展開ライブラリのハンドル
  SCANDECOPEN        lpfnScanDecOpen;        // 圧縮データ展開ライブラリのオープン関数
  SCANDECSETTBL      lpfnScanDecSetTbl;      // 圧縮データ展開ライブラリのテーブルセット関数
  SCANDECPAGESTART   lpfnScanDecPageStart;   // 圧縮データ展開ライブラリの開始処理関数
  SCANDECWRITE       lpfnScanDecWrite;       // 圧縮データ展開ライブラリの書き込み関数
  SCANDECPAGEEND     lpfnScanDecPageEnd;     // 圧縮データ展開ライブラリの終了処理関数
  SCANDECCLOSE       lpfnScanDecClose;       // 圧縮データ展開ライブラリのクローズ関数
} TScanDec;


// DSCMATCH.Cに定義されている外部変数を構造体として定義
typedef struct TCorlorMATCH {
  void              *hColorMatch;           // カラーマッチングライブラリのハンドル
  COLORINIT          lpfnColorMatchingInit; // カラーマッチングライブラリの初期化関数
  COLOREND           lpfnColorMatchingEnd;  // カラーマッチングライブラリの終了関数
  COLORMATCHING      lpfnColorMatchingFnc;  // カラーマッチングライブラリの実行関数
  int                nColorMatchStatus;     // カラーマッチングステータス
  char               szTwdsColorMatch[ MAX_PATH ]; // カラーマッチングライブラリのフルパス名
  char               szLutFilePathName[ MAX_PATH ];// カラーマッチングのLUTファイルフルパス名
  void              *hGrayTbl;              // GrayTableのハンドル
} TCorlorMATCH;





#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
typedef struct tagDEVHANDLE {
  br_net_dev_handle      net;
  usb_dev_handle   *usb;
  int              device;
  int              net_device_index;
  int              usb_w_ep;
  int              usb_r_ep;
} dev_handle;
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado




typedef struct Brother_Scanner {
#ifndef INSANE_VERSION
  struct Brother_Scanner    *pNext;
  SANE_Option_Descriptor     aoptDesc[NUM_OPTIONS];
  TOptionValue               aoptVal[NUM_OPTIONS];
#endif
  MODELINF                   modelInf;		// オープンされたデバイス情報
  MODELCONFIG                modelConfig;       // オープンされたデバイスの各種設定情報

#ifndef  NET_AND_ADVINI //for network and inifile extension (M-LNX16,17) kado
  usb_dev_handle            *hScanner;		// USBスキャナのハンドル
#else    //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
  dev_handle                 *hScanner;		// USBスキャナのハンドル
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado

  UISETTING                  uiSetting;         // 
  TCorlorMATCH               cmatch;            // カラーマッチング用構造体
  TScanDec                   scanDec;           // 圧縮データ展開用構造体
  SCANINFO                   scanInfo;          // スキャン時の情報
  DEVSCANINFO                devScanInfo;       // デバイスのスキャン情報
  MFCMODELINFO               mfcModelInfo;      // 
  MFCDEVICEHEAD              mfcDevInfoHeader;  // 
  MFCDEVICEINFO              mfcDeviceInfo;     // 
  TScanState                 scanState;         // スキャン時のステータス情報
} Brother_Scanner;


#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
#define  usb_dev_handle   dev_handle
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado

// from DS_INFO.H
//
// 解像度タイプ用シンボル定義
//
#define RESOTYPECNT   13				// 解像度タイプ総数
#define RES100X100     0				//  100 x  100 dpi
#define RES150X150     1				//  150 x  150 dpi
#define RES200X100     2				//  200 x  100 dpi
#define RES200X200     3				//  200 x  200 dpi
#define RES200X400     4				//  200 x  400 dpi
#define RES300X300     5				//  300 x  300 dpi
#define RES400X400     6				//  400 x  400 dpi
#define RES600X600     7				//  600 x  600 dpi
#define RES800X800     8				//  800 x  800 dpi
#define RES1200X1200   9				// 1200 x 1200 dpi
#define RES2400X2400  10				// 2400 x 2400 dpi
#define RES4800X4800  11				// 4800 x 4800 dpi
#define RES9600X9600  12				// 9600 x 9600 dpi
#define DEF_RESOTYPE  RES200X200		// 解像度タイプ初期値

//
// カラータイプ用シンボル定義
//
#define COLORTYPECNT   7				// カラータイプ総数
#define COLOR_BW       0				// Black & White
#define COLOR_ED       1				// Error Diffusion Gray
#define COLOR_DTH      2				// Dithered Gray
#define COLOR_TG       3				// True Gray
#define COLOR_256      4				// 256 Color
#define COLOR_FUL      5				// 24bit Full Color
#define COLOR_FUL_NOCM 6				// 24bit Full Color(ColorMatchなし)
#define DEF_COLORTYPE  COLOR_BW			// カラータイプ初期値

//
// スキャンソース用シンボル定義
//
//06/02/28 Duplexスキャン用にSCANSRC_ADF_DUP追加
#define SCANSRCCNT      3				// スキャンソース総数
#define SCANSRC_FB      0				// Flatbed
#define SCANSRC_ADF     1				// Automatic document feeder
#define SCANSRC_ADF_DUP 2				// ADF Duplex
#define DEF_SCANSRC    SCANSRC_ADF			// スキャンソース初期値

//
// Brightness/Contrast用シンボル定義
//
#define DEF_BRIGHTNESS     0			// Brightness初期値
#define MIN_BRIGHTNESS   -50			// Brightness最小値
#define MAX_BRIGHTNESS    50			// Brightness最大値
#define DEF_CONTRAST       0			// Contrast初期値
#define MIN_CONTRAST     -50			// Contrast最小値
#define MAX_CONTRAST      50			// Contrast最大値

//
// 原稿サイズタイプ用シンボル定義
//
#define PAPERTYPECNT   8				// 原稿サイズタイプ総数
#define PAPER_A4       0				// A4
#define PAPER_B5       1				// B5
#define PAPER_LETTER   2				// US-Letter
#define PAPER_LEGAL    3				// Legal
#define PAPER_A5       4				// A5
#define PAPER_EXEC     5				// Executive
#define PAPER_BCARD    6				// 名刺
#define PAPER_USER     7				// ユーザー指定

//
// 原稿サイズ(0.1mm単位)用シンボル定義
//
#define PSIZE_A4_X      2100			// A4幅(0.1mm単位)
#define PSIZE_A4_Y      2970			// A4長さ(0.1mm単位)
#define PSIZE_B5_X      1820			// B5幅(0.1mm単位)
#define PSIZE_B5_Y      2570			// B5長さ(0.1mm単位)
#define PSIZE_LETTER_X  2159			// Letter幅(0.1mm単位)
#define PSIZE_LETTER_Y  2794			// Letter長さ(0.1mm単位)
#define PSIZE_LEGAL_X   2159			// Legal幅(0.1mm単位)
#define PSIZE_LEGAL_Y   3556			// Legal長さ(0.1mm単位)
#define PSIZE_A5_X      1480			// A5幅(0.1mm単位)
#define PSIZE_A5_Y      2100			// A5長さ(0.1mm単位)
#define PSIZE_EXEC_X    1842			// Executive幅(0.1mm単位)
#define PSIZE_EXEC_Y    2667			// Executive長さ(0.1mm単位)
#define PSIZE_BCARD_X    900			// 名刺幅(0.1mm単位)
#define PSIZE_BCARD_Y    600			// 名刺長さ(0.1mm単位)
#define PSIZE_12INCH    3048			// 12inch(0.1mm単位)

#define PSIZE_MIN_X       89			// 最小幅(0.1mm単位)
#define PSIZE_MIN_Y       89			// 最小長(0.1mm単位)
#define PSIZE_MAX_X     PSIZE_LEGAL_X	// 最大幅(0.1mm単位)
#define PSIZE_MAX_Y     PSIZE_LEGAL_Y	// 最大長(0.1mm単位)
#define PSIZE_BCARDWID  1200			// 名刺モード最大幅

#define PSIZE_VMARGIN     60			// 上下マージン(0.1mm単位)
#define PSIZE_HMARGIN     20			// 左右マージン(0.1mm単位)

#define MAX_PATH 256
#define BROTHER_SANE_DIR "/usr/local/Brother/sane/"
#define BROTHER_GRAYCMDATA_DIR "GrayCmData/"

#endif
