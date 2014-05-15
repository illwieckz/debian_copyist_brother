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
//		Copyright(c) 2005-2009 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef _BROTHER_MFCINFO_H_
#define _BROTHER_MFCINFO_H_

#include "brother_dtype.h"

//
// 
//


typedef struct tagMFCMODELINFO {
	WORD  wModelType;		// 
	WORD  wDialogType;		// 
	BOOL  bColorModel;		// 
	BOOL  b3in1Type;		// 
	BOOL  bDither;			// 
	BOOL  bVideoCap;		// 
	BOOL  bQcmdEnable;		// 
} MFCMODELINFO, *LPMFCMODELINFO;


#define TWDSUI_3IN1  1
#define TWDSUI_NOVC  3


#pragma pack(1)
//
// 
//
typedef union tagMFCCOLORTYPE {
	struct {
		BYTE  bBlackWhite:1;	// 
		BYTE  bErrorDiffusion:1;// 
		BYTE  bTrueGray:1;	// 
		BYTE  b256Color:1;	// 
		BYTE  b24BitColor:1;	// 
		BYTE  b256IndexColor:1;	// 
	} bit;
	BYTE val;
} MFCCOLORTYPE, *LPMFCCOLORTYPE;

#define MFCDEVINFMONO   0x07		// 
					//   (B&W, ErrorDiff, TrueGray)
#define MFCDEVINFCOLOR  0x1F		// 
					//   (B&W, ErrorDiff, TrueGray, 256Color, 24bit)


//
//
//
typedef struct tagMFCDEVICEHEAD {
	WORD  wDeviceInfoID;		// 
	BYTE  nInfoSize;		// 
	BYTE  nProtcolType; 		// DS<->MFC
					// 
} MFCDEVICEHEAD, *LPMFCDEVICEHEAD;

#define MFCDEVICEINFOID  0x00C1		// 
#define MFCPROTCOL1998   0		// 
#define MFCPROTCOL2000   1		// 


//
//
//
typedef struct tagMFCDEVICEINFO {
	BYTE          nVideoSignalType; // 
					// 
	MFCCOLORTYPE  nColorType;	// 
					//   00h=Reserve, MSB|0:0:256ix:24c:256c:TG:ED:BW|LSB
	WORD          nVideoNtscSignal; // 
					//   0=Reserve 1:B,2:G,3:H,4:I,5:D,6:K,7:K1,8:L,9:M,10:N
	WORD          nVideoPalSignal;	// 
					//   0=Reserve 1:B,2:G,3:H,4:I,5:D,6:K,7:K1,8:L,9:M,10:N
	WORD          nVideoSecamSignal;// 
					//   0=Reserve 1:B,2:G,3:H,4:I,5:D,6:K,7:K1,8:L,9:M,10:N
	BYTE          nHardwareType;	// 
					//   00h=Reserve, 01h=NTSC
	BYTE          nHardwareVersion; // 
					//   00h=Reserve, 
	BYTE          nMainScanDpi;	// 
					//   00h=Reserve, 01h=200dpi, 02h=200,300dpi, 03h=100,200,300dpi

#if 0 // support A3 M-LNX-98
	BYTE          nPaperSizeMax;	// 
#else // support A3 M-LNX-98
	BYTE          nPaperSizeMax;	//   00h=Reserve, 01h=A4, 02h=B4
                                        //   03h=A3
        BYTE          nXposFB;          //   00h=NotSuppot
	                                //   01h=adjust to left position
	                                //   02h=adjust to centor position
	                                //   03h=adjust to right position
        BYTE          nYposFB;          //   00h=NotSuppot
	                                //   01h=adjust to top position
	                                //   02h=adjust to centor position
	                                //   03h=adjust to bottom position
        BYTE          nXposADF;         //   00h=NotSuppot
	                                //   01h=adjust to left position
	                                //   02h=adjust to centor position
	                                //   03h=adjust to right position
        BYTE          nYposADF;         //   00h=NotSuppot
	                                //   01h=adjust to top position
	                                //   02h=adjust to centor position
	                                //   03h=adjust to bottom position
#endif // support A3 M-LNX-98

					//   00h=Reserve, 01h=A4, 02h=B4
} MFCDEVICEINFO, *LPMFCDEVICEINFO;

#define MFCVIDEONONE     0xFF		// 
#define MFCVIDEONTSC     1		// 
#define MFCVIDEOPAL      2		// 
#define MFCVIDEOSECAM    3		// 

#define MFCHARDNTSC      1		// 
#define MFCHARDLEX       2		// 
#define MFCHARDPAL       0x81		// 

#define MFCMAINSCAN200   1		// 
#define MFCMAINSCAN300   2		// 
#define MFCMAINSCAN100   3		// 

#pragma pack()
//
// 
//
#define MFCDEVINFOFULLSIZE  sizeof( BYTE ) * 2 + sizeof( MFCDEVICEINFO )


//
// 
//
typedef struct tagDEVSCANINFO {
	WORD        wResoType;		// 
	WORD        wColorType;		// 
	RESOLUTION  DeviceScan; 	// 
	WORD        wScanSource;	// 
	DWORD       dwMaxScanWidth; 	// 
	DWORD       dwMaxScanPixels;	// 
	DWORD       dwMaxScanHeight;	// 
	DWORD       dwMaxScanRaster;	// 
	AREARECT    ScanAreaDot;	// 
	AREASIZE    ScanAreaSize;	// 
	AREASIZE    ScanAreaByte;	// 
} DEVSCANINFO, *LPDEVSCANINFO;

#define MFCSCANSRC_ADF  1		// 
#define MFCSCANSRC_FB   2		// 
#define MFCSCANSRC_ADF_DUP  3           //

#define MFCSCANMAXWIDTH      2080	//
#define MFCSCAN200MAXPIXEL   1664	//
#define MFCSCAN300MAXPIXEL   2464	//
#define MFCSCANMAXHEIGHT     3476	//
#define MFCSCAN400MAXRASTER  5474	//
#define MFCSCAN600MAXRASTER  8210	//
#define MFCSCANFBHEIGHT      2910	//
#define MFCSCAN400FBRASTER   4582	//
#define MFCSCAN600FBRASTER   6874	//

#endif //_BROTHER_MFCINFO_H_

//////// end of brother_mfcinfo.h ////////
