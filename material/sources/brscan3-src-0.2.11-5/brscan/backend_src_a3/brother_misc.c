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
//	Source filename: brother_misc.c
//
//		Copyright(c) 2005-2009 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			各種関数群
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "brother_misc.h"


//-----------------------------------------------------------------------------
//
//	Function name:	GetPathFromFileName
//
//
//	Abstract:
//		ファイルのフルパス名からパス名に変換する
//
//
//	Parameters:
//		lpszLogStr
//			in:  ファイルのフルパス名へのポインタ
//			out: パス名が格納される
//
//
//	Return values:
//		なし
//
//-----------------------------------------------------------------------------
//
void
GetPathFromFileName( LPSTR lpszFileName )
{
	int  nFileNameSize;
	LPSTR  lpszNameTop;


	lpszNameTop = lpszFileName;
	nFileNameSize = strlen( lpszFileName );
	lpszFileName += nFileNameSize;

	for( ; nFileNameSize > 0; --nFileNameSize ){
		if( *(--lpszFileName) == '\\')
			break;
	}
	if( nFileNameSize <= 0 ){
		*lpszNameTop = '\0';
	}else{
		*lpszFileName = '\0';
	}
}


//-----------------------------------------------------------------------------
//
//	Function name:	MakePathName
//
//
//	Abstract:
//		パス名にファイル／フォルダ名を追加する
//
//
//	Parameters:
//		lpszPathName
//			in:  パス名へのポインタ
//			out: ファイル／フォルダのフルパス名が格納される
//
//		lpszAddName
//			in:  ファイル／フォルダ名へのポインタ
//
//
//	Return values:
//		なし
//
//-----------------------------------------------------------------------------
//
void
MakePathName( LPSTR lpszPathName, LPSTR lpszAddName )
{
	strcat( lpszPathName, "\\" );
	strcat( lpszPathName, lpszAddName );
}


//-----------------------------------------------------------------------------
//
//	Function name:	GetToken
//
//
//	Abstract:
//		入力文字列からTokenを取り出す
//
//
//	Parameters:
//		lppszData
//			in:  文字列へのポインタへのポインタ
//			out: Token取り出し後の文字列へのポインタが格納される
//
//
//	Return values:
//		取り出したTokenへのポインタ
//
//-----------------------------------------------------------------------------
//
LPSTR
GetToken( LPSTR *lppszData )
{
	LPSTR  lpszToken;
	LPSTR  lpszNextTop;


	lpszToken = lpszNextTop = *lppszData;

	if( lpszNextTop != NULL && *lpszNextTop ){
		while( *lpszNextTop ){
			if( *lpszNextTop == ',' ){
				*lpszNextTop++ = '\0';
				*lppszData = lpszNextTop;
				break;
			}else{
				lpszNextTop++;
			}
		}
	}else{
		lpszToken = NULL;
	}
	return lpszToken;
}


//-----------------------------------------------------------------------------
//
//	Function name:	StrToWord
//
//
//	Abstract:
//		文字列を数値(WORD)に変換
//
//
//	Parameters:
//		lpszText
//			文字列へのポインタ
//
//
//	Return values:
//		変換された数値
//
//-----------------------------------------------------------------------------
//
WORD
StrToWord( LPSTR lpszText )
{
	WORD   wResult = 0;
	char   chData = *lpszText;


	if( lpszText != NULL ){
		while( chData ){
			if( '0' <= chData && chData <= '9' ){
				wResult = wResult * 10 + ( chData - '0' );
			}else{
				wResult = 0;
				break;
			}
			lpszText++;
			chData = *lpszText;
		}
	}
	return wResult;
}


//-----------------------------------------------------------------------------
//
//	Function name:	WordToStr
//
//
//	Abstract:
//		数値(WORD)を文字列に変換
//
//
//	Parameters:
//		wValue
//			数値(WORD)
//
//		lpszTextTop
//			文字列を格納するバッファへのポインタ
//
//
//	Return values:
//		変換された文字列へのポインタ
//
//-----------------------------------------------------------------------------
//
LPSTR
WordToStr( WORD wValue, LPSTR lpszTextTop )
{
	LPSTR  lpszText;
	char   szTemp[ 8 ];
	LPSTR  lpszTemp;
	int    nTextLen;

 
	lpszTemp = szTemp;
	do{
		*lpszTemp++ = '0' + ( wValue % 10 );
		wValue /= 10;
	}while( wValue > 0 );
	*lpszTemp = '\0';

	lpszText = lpszTextTop;
	nTextLen = strlen( szTemp );
	for( ; nTextLen > 0; nTextLen-- ){
		*lpszText++ = *(--lpszTemp);
	}
	*lpszText = '\0';

	return lpszTextTop;
}


//-----------------------------------------------------------------------------
//
//	Function name:	StrToShort
//
//
//	Abstract:
//		文字列を数値(short)に変換
//
//
//	Parameters:
//		lpszText
//			文字列へのポインタ
//
//
//	Return values:
//		変換された数値
//
//-----------------------------------------------------------------------------
//
short
StrToShort( LPSTR lpszText )
{
	short  nSign = 1;
	short  nResult = 0;
	char   chData = *lpszText;


	if( lpszText != NULL ){
		if( *lpszText == '-' ){
			nSign = -1;
			lpszText++;
		}
		while( chData ){
			if( '0' <= chData && chData <= '9' ){
				nResult = nResult * 10 + ( chData - '0' );
			}else{
				nResult = 0;
				break;
			}
			lpszText++;
			chData = *lpszText;
		}
		nResult *= nSign;
	}
	return nResult;
}


//-----------------------------------------------------------------------------
//
//	Function name:	ShortToStr
//
//
//	Abstract:
//		数値(short)を文字列に変換
//
//
//	Parameters:
//		nValue
//			数値(short)
//
//		lpszTextTop
//			文字列を格納するバッファへのポインタ
//
//
//	Return values:
//		変換された文字列へのポインタ
//
//-----------------------------------------------------------------------------
//
LPSTR
ShortToStr( short nValue, LPSTR lpszTextTop )
{
	LPSTR  lpszText;
	char   szTemp[ 8 ];
	LPSTR  lpszTemp;
	int    nTextLen;
	BOOL   bSign = FALSE;


	if( nValue < 0 ){
		nValue *= -1;
		bSign = TRUE;
	}
	lpszTemp = szTemp;
	do{
		*lpszTemp++ = '0' + ( nValue % 10 );
		nValue /= 10;
	}while( nValue > 0 );
	*lpszTemp = '\0';

	lpszText = lpszTextTop;
	if( bSign ){
		*lpszText++ = '-';
	}
	nTextLen = strlen( szTemp );
	for( ; nTextLen > 0; nTextLen-- ){
		*lpszText++ = *(--lpszTemp);
	}
	*lpszText = '\0';

	return lpszTextTop;
}


//-----------------------------------------------------------------------------
//
//	Function name:	StrToDword
//
//
//	Abstract:
//		文字列を数値(DWORD)に変換
//
//
//	Parameters:
//		lpszText
//			文字列へのポインタ
//
//
//	Return values:
//		変換された数値
//
//-----------------------------------------------------------------------------
//
DWORD
StrToDword( LPSTR lpszText )
{
	DWORD  dwResult = 0;
	char  chData = *lpszText;


	if( lpszText != NULL ){
		while( chData ){
			if( '0' <= chData && chData <= '9' ){
				dwResult = dwResult * 10 + ( chData - '0' );
			}else{
				dwResult = 0;
				break;
			}
			chData = *lpszText++;
		}
	}
	return dwResult;
}


//-----------------------------------------------------------------------------
//
//	Function name:	DwordToStr
//
//
//	Abstract:
//		数値(DWORD)を文字列に変換
//
//
//	Parameters:
//		dwValue
//			数値(DWORD)
//
//		lpszTextTop
//			文字列を格納するバッファへのポインタ
//
//
//	Return values:
//		変換された文字列へのポインタ
//
//-----------------------------------------------------------------------------
//
LPSTR
DwordToStr( DWORD dwValue, LPSTR lpszTextTop )
{
	LPSTR  lpszText;
	char   szTemp[ 16 ];
	LPSTR  lpszTemp;
	int    nTextLen;


	lpszTemp = szTemp;
	do{
		*lpszTemp++ = '0' + (BYTE)( dwValue % 10 );
		dwValue /= 10;
	}while( dwValue > 0 );
	*lpszTemp = '\0';

	lpszText = lpszTextTop;
	nTextLen = strlen( szTemp );
	for( ; nTextLen > 0; nTextLen-- ){
		*lpszText++ = *(--lpszTemp);
	}
	*lpszText = '\0';

	return lpszTextTop;
}


//////// end of brother_misc.c ////////
