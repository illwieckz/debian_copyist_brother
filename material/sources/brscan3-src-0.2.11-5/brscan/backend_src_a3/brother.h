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
//	Source filename: brother_devaccs.h
//
//		Copyright(c) 2005-2008 Brother Industries, Ltd.  
//               All Rights Reserved.
//
//
//	Abstract:
//		
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



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
  int                   index;
  SANE_Device            sane;
  MODELINF               modelInf;
} TDevice;

#endif


//
//
//
typedef struct tagUISETTING {
	WORD          wResoType;		//
	WORD          wColorType;		//
	int           nBrightness;		//
	int           nContrast;		//
	RESOLUTION    UserSelect;		//
	AREARECT      ScanAreaMm;		//
	AREARECT      ScanAreaDot;		//
	RESOLIST      ResoList;			//
	SCANMODELIST  ScanModeList;		//
	SCANSRCLIST   ScanSrcList;   		//
	int           nSrcType;			//
	int           nSrcAlign;       		//M-LNX-103
} UISETTING, *LPUISETTING;

//
//
//
typedef struct tagSCANINFO {
	RESOLUTION  UserSelect;			//
	AREARECT    ScanAreaMm;			//
	AREARECT    ScanAreaDot;		//
	AREASIZE    ScanAreaSize;		//
	AREASIZE    ScanAreaByte;		//
} SCANINFO, *LPSCANINFO;

typedef struct TScanDec {
  void              *hScanDec;               //
  SCANDECOPEN        lpfnScanDecOpen;        //
  SCANDECSETTBL      lpfnScanDecSetTbl;      //
  SCANDECPAGESTART   lpfnScanDecPageStart;   //
  SCANDECWRITE       lpfnScanDecWrite;       //
  SCANDECPAGEEND     lpfnScanDecPageEnd;     //
  SCANDECCLOSE       lpfnScanDecClose;       //
} TScanDec;


// DSCMATCH.C
typedef struct TCorlorMATCH {
  // M-LNX-111
  //   delete color matching function
  void              *hGrayTbl;              //
} TCorlorMATCH;





typedef struct tagDEVHANDLE {
  br_net_dev_handle      net;
  usb_dev_handle   *usb;
  int              device;
  int              net_device_index;
  int              usb_w_ep;
  int              usb_r_ep;
} dev_handle;




typedef struct Brother_Scanner {
#ifndef INSANE_VERSION
  struct Brother_Scanner    *pNext;
  SANE_Option_Descriptor     aoptDesc[NUM_OPTIONS];
  TOptionValue               aoptVal[NUM_OPTIONS];
#endif
  MODELINF                   modelInf;		//
  MODELCONFIG                modelConfig;       //

  dev_handle                 *hScanner;		//

  UISETTING                  uiSetting;         // 
  TCorlorMATCH               cmatch;            //
  TScanDec                   scanDec;           //
  SCANINFO                   scanInfo;          //
  DEVSCANINFO                devScanInfo;       //
  MFCMODELINFO               mfcModelInfo;      // 
  MFCDEVICEHEAD              mfcDevInfoHeader;  // 
  MFCDEVICEINFO              mfcDeviceInfo;     //
  TScanState                 scanState;         //
} Brother_Scanner;



#define  usb_dev_handle   dev_handle

// from DS_INFO.H
//
//
//
#define RESOTYPECNT   13				//
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
#define DEF_RESOTYPE  RES200X200		//

//
//
//
#define COLORTYPECNT   7				//
#define COLOR_BW       0				// Black & White
#define COLOR_ED       1				// Error Diffusion Gray
#define COLOR_DTH      2				// Dithered Gray
#define COLOR_TG       3				// True Gray
#define COLOR_256      4				// 256 Color
#define COLOR_FUL      5				// 24bit Full Color
#define COLOR_FUL_NOCM 6				// 24bit Full Color(ColorMatch¤Ê¤·)
#define DEF_COLORTYPE  COLOR_BW			//

//
//
//
//06/02/28
#define SCANSRCCNT      3				//
#define SCANSRC_FB      0				//
#define SCANSRC_ADF     1				//
#define SCANSRC_ADF_DUP 2				//
// M-LNX-103 measure for the center aligned document
#define SCANSRC_ADFC     3				//
#define SCANSRC_ADF_DUPC 4				//
#define SCANSRC_ADF_CTRALN     0			//
#define SCANSRC_ADF_LFTALN     1			//
#define DEF_SCANSRC    SCANSRC_ADF			//

//
//
//
#define DEF_BRIGHTNESS     0			//
#define MIN_BRIGHTNESS   -50			//
#define MAX_BRIGHTNESS    50			//
#define DEF_CONTRAST       0			//
#define MIN_CONTRAST     -50			//
#define MAX_CONTRAST      50			//

//
//
//
#define PAPERTYPECNT   8				//
#define PAPER_A4       0				//
#define PAPER_B5       1				//
#define PAPER_LETTER   2				//
#define PAPER_LEGAL    3				// Legal
#define PAPER_A5       4				// A5
#define PAPER_EXEC     5				// Executive
#define PAPER_BCARD    6				//
#define PAPER_USER     7				//

//
//
//
#define PSIZE_A4_X      2100			//
#define PSIZE_A4_Y      2970			//
#define PSIZE_B5_X      1820			//
#define PSIZE_B5_Y      2570			//
#define PSIZE_LETTER_X  2159			//
#define PSIZE_LETTER_Y  2794			//
#define PSIZE_LEGAL_X   2159			//
#define PSIZE_LEGAL_Y   3556			//
#define PSIZE_A5_X      1480			//
#define PSIZE_A5_Y      2100			//
#define PSIZE_EXEC_X    1842			//
#define PSIZE_EXEC_Y    2667			//
#define PSIZE_BCARD_X    900			//
#define PSIZE_BCARD_Y    600			//
#define PSIZE_12INCH    3048			//

#define PSIZE_MIN_X       89			// 
#define PSIZE_MIN_Y       89			// 
#define PSIZE_MAX_X     PSIZE_LEGAL_X	//
#define PSIZE_MAX_Y     PSIZE_LEGAL_Y	//
#define PSIZE_BCARDWID  1200			//

#define PSIZE_VMARGIN     60			//
#define PSIZE_HMARGIN     20			//

// M-LNX-104  #define MAX_PATH 256
#define BROTHER_SANE_DIR "/usr/local/Brother/sane/"
#define BROTHER_GRAYCMDATA_DIR "GrayCmData/"

#endif
