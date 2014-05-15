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
//	Source filename: brother_scanner.h
//
//		Copyright(c) 2004-2006 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef _BROTHER_SCANNER_H_
#define _BROTHER_SCANNER_H_

//
// 関数のプロトタイプ宣言
//
BOOL   LoadScanDecDll( Brother_Scanner *this );
void   FreeScanDecDll( Brother_Scanner *this );
BOOL   ScanStart( Brother_Scanner *this );
BOOL   PageScanStart( Brother_Scanner *this );
int    PageScan( Brother_Scanner *this, char *lpFwBuf, int nMaxLen, int *lpFwLen );
void   AbortPageScan( Brother_Scanner *this );
void   ScanEnd( Brother_Scanner *this );
void   GetScanAreaParam( Brother_Scanner *this );
void   GetDeviceScanArea( Brother_Scanner *this, LPAREARECT lpScanAreaDot );
BOOL   StartDecodeStretchProc( Brother_Scanner *this );
int    ProcessMain(Brother_Scanner *this, WORD wByte, WORD wDataLineCnt, char * lpFwBuf, int *lpFwBufcnt, WORD *lpProcessSize);
void   SetupImgLineProc( BYTE chLineHeader );
int    GetStatusCode(BYTE Header);
void   CnvResoNoToUserResoValue( LPRESOLUTION pUserSelect, WORD nResoNo );


#define SCAN_GOOD		0
#define SCAN_EOF		1
#define SCAN_MPS		2
#define SCAN_CANCEL		3
#define SCAN_NODOC		4
#define SCAN_DOCJAM		5
#define SCAN_COVER_OPEN		6
#define SCAN_SERVICE_ERR	7
//06/02/28
#define SCAN_DUPLEX_NORMAL      8
#define SCAN_DUPLEX_REVERSE     9

#define SANE_STATUS_DUPLEX_ADVERSE  128


#endif //_BROTHER_SCANNER_H_


//////// end of brother_scanner.h ////////
