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
//	Source filename: brother_cmatch.h
//
//		Copyright(c) 2004-2006 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			カラーマッチング処理モジュール・ヘッダー
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef _BROTHER_CMATCH_H_
#define _BROTHER_CMATCH_H_

#include "brcolor.h"

#define GRAY_TABLE_NO  1  // 1で固定

//
// ColorMatch/GrayTableのファイル名
//
#define COLORMATCHDLL  "Brcolm32.dll"
#define COLORMATCHLUT  "BrLutCm.dat"
#define GRAYTABLEFILE  "brmfgray.bin"


//
// ColorMatch処理の状態
//
#define COLORMATCH_NONE  0
#define COLORMATCH_GOOD  1
#define COLORMATCH_NG    2


//
// 関数のプロトタイプ宣言
//
#ifndef  NET_AND_ADVINI //for network and inifile extension (M-LNX16,17) kado
BOOL     LoadColorMatchDll( Brother_Scanner *this );
#else    //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
BOOL     LoadColorMatchDll( Brother_Scanner *this ,int index);
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado


void     FreeColorMatchDll( Brother_Scanner *this );
void     InitColorMatchingFunc( Brother_Scanner *this, WORD nColorType, int nRgbDataType );
void     ExecColorMatchingFunc( Brother_Scanner *this, LPBYTE lpRgbData, long lRgbDataLen, long lLineCount );
void     CloseColorMatchingFunc( Brother_Scanner *this );
BOOL     LoadGrayTable( Brother_Scanner *this, BYTE GrayTableNo );
void     FreeGrayTable( Brother_Scanner *this );
HANDLE   SetupGrayAdjust( Brother_Scanner *this );
LONG     AdjustContrast( LONG lGrayVal, int sdc );
LONG     AdjustBright( LONG lGrayVal, int sdc );


#endif //_BROTHER_CMATCH_H_


//////// end of brother_cmatch.h ////////
