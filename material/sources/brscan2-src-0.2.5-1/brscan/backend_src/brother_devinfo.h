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
//	Source filename: brother_devinfo.h
//
//		Copyright(c) 2004-2006 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:Header of getting device information  module
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef _BROTHER_DEVINFO_H_
#define _BROTHER_DEVINFO_H_

#include "brother_mfcinfo.h"

//
// prototype
//
BOOL  ExecQueryThread( Brother_Scanner *this, void *lpQueryProc );
BOOL  QueryDeviceInfo( Brother_Scanner *this );
BOOL  QueryScannerInfo( Brother_Scanner *this );
void  SetDefaultScannerInfo( Brother_Scanner *this );
void  CnvResoNoToDeviceResoValue( Brother_Scanner *this, WORD nResoNo, WORD nColorType );

DWORD QCommandProc( void *lpParameter );
DWORD QueryScanInfoProc( void *lpParameter );

#endif //_BROTHER_DEVINFO_H_


//////// end of brother_devinfo.h ////////
