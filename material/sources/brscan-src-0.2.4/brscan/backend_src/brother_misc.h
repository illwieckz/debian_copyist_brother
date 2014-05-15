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
//	Source filename: brother_misc.h
//
//		Copyright(c) 1997-2000 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			各種関数群（主に文字列処理関数）・ヘッダー
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#ifndef _BROTHER_MISC_H_
#define _BROTHER_MISC_H_


//
// 関数のプロトタイプ宣言
//
void    GetPathFromFileName( LPSTR lpszFileName );
void    MakePathName( LPSTR lpszPathName, LPSTR lpszAddName );
LPSTR   GetToken( LPSTR *lppszData );
WORD    StrToWord( LPSTR lpszText );
LPSTR   WordToStr( WORD wValue, LPSTR lpszTextTop );
short   StrToShort( LPSTR lpszText );
LPSTR   ShortToStr( short nValue, LPSTR lpszTextTop );
DWORD   StrToDword( LPSTR lpszText );
LPSTR   DwordToStr( DWORD dwValue, LPSTR lpszTextTop );

#endif //_BROTHER_MISC_H_


//////// end of brother_misc.h ////////
