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
//	Source filename: brother_dtype.h
//
//		Copyright(c) 2004-2006 Brother Industries, Ltd.  
//               All Rights Reserved.
//
//
//	Abstract:
//		
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


#ifndef _H_BROTHER_DTYPE_
#define _H_BROTHER_DTYPE_

/*==========*/
/* 基本定数	*/
/*==========*/

typedef char	CHAR;
typedef short SHORT;
typedef long	LONG;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned char       *LPBYTE;
typedef unsigned short      WORD;
typedef unsigned long	DWORD;
typedef float             FLOAT;
typedef int               INT;
typedef unsigned int      UINT;
typedef unsigned int      *PUINT;
typedef char	   *LPSTR;
//typedef char	   *PSTR;
typedef char	   *LPTSTR;
//typedef const char *PCSTR;
//typedef const char *LPCSTR;
typedef	const char *LPCTSTR;

typedef	void *HANDLE;

#define MAX_PATH          260

#define		NOFIND	0
#define		FIND	1

#define		NULL_C	'\0'
#define		NULL_S	"\0"
#define		LF		'\n'

#define		END		1
#define		NOEND	0

#define		ON		1
#define		OFF		0

#define		FALSE		0
#define		TRUE		1

//
// 解像度用構造体
//
typedef struct tagRESOLUTION {
	WORD  wResoX;
	WORD  wResoY;
} RESOLUTION, *LPRESOLUTION;

//
// 範囲／用紙サイズ用構造体
//
typedef struct tagAREASIZE {
	LONG  lWidth;
	LONG  lHeight;
} AREASIZE, *LPAREASIZE; 

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} AREARECT, *LPAREARECT;

#endif // _H_BROTHER_DTYPE_
