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


// brscan3 - brother sane backend
// Copyright(c) 2005-2008 Brother Industries, Ltd.  
//          All Rights Reserved.
/*******************************************************************************
 *
 *	Scan Decode ヘッダーファイル
 *
 *	Copyright: 2000 brother Industries , Ltd.
 *
 *	ver 1.0.0 : 2000.04.13 : 稲田 : 新規作成
 ******************************************************************************/
#ifndef	__BROTHER_SCAN_DECODE_H
#define	__BROTHER_SCAN_DECODE_H

typedef	struct {
	INT		nInResoX ;		/* 入力解像度 */
	INT		nInResoY ;		/* 入力解像度 */
	INT		nOutResoX ;		/* 出力解像度 */
	INT		nOutResoY ;		/* 出力解像度 */
	INT		nColorType ;	/* カラー種別 */
	DWORD	dwInLinePixCnt ;	/* 1ライン入力Pixel数 */
	INT		nOutDataKind ;	/* 24BitColor時のRGB出力形式 */
#if 0	/* brother_deccom.hに記述 */
#define	SCODK_PIXEL		1	/* RGB RGB RGB画素順 */
#define	SCODK_PLANE		2	/* RRR GGG BBB画素順 */
#endif
	BOOL	bLongBoundary ;	/* Long Boundary指定 */

/* ScanDecOpen内で設定される */
	DWORD	dwOutLinePixCnt ;	/* 1ライン出力Pixel数 */
	DWORD	dwOutLineByte ;		/* 1ライン出力Byte数 */
	DWORD	dwOutWriteMaxSize ;	/* 最大書き込みバッファサイズ */

} SCANDEC_OPEN ;

typedef	struct {
	INT		nInDataComp ;		/* 入力データ圧縮種別 */
#define	SCIDC_WHITE		1	/* 全白ライン */
#define	SCIDC_NONCOMP	2	/* 非圧縮 */
#define	SCIDC_PACK		3	/* パックビッツ圧縮 */

	INT		nInDataKind ;		/* 入力データ格納情報 */
#if 0	/* brother_deccom.hに記述 */
#define	SCIDK_MONO		1	/* モノクロ */
#define	SCIDK_R			2	/* Rデータ */
#define	SCIDK_G			3	/* Gデータ */
#define	SCIDK_B			4	/* Bデータ */
#define	SCIDK_RGB		5	/* RGB画素順 */
#define	SCIDK_BGR		6	/* BGR画素順 */
#define	SCIDK_256		7	/* 256色カラー */
#endif
	BYTE	*pLineData ;	/* 入力１ラインデータ格納先 */
	DWORD	dwLineDataSize ;	/* 入力１ラインデータサイズ */
	BYTE	*pWriteBuff ;	/* 出力データ格納先 */
	DWORD	dwWriteBuffSize ;	/* 出力データバッファサイズ */
	BOOL	bReverWrite ;	/* 出力データ格納方法 */

} SCANDEC_WRITE ;

BOOL	ScanDecOpen( SCANDEC_OPEN * );
void	ScanDecSetTblHandle( HANDLE, HANDLE );
BOOL	ScanDecPageStart( void );
DWORD	ScanDecWrite( SCANDEC_WRITE *, INT * );
DWORD	ScanDecPageEnd( SCANDEC_WRITE *, INT * );
BOOL	ScanDecClose( void );

typedef BOOL  (*SCANDECOPEN) (SCANDEC_OPEN * );
typedef void  (*SCANDECSETTBL) ( HANDLE, HANDLE );
typedef BOOL  (*SCANDECPAGESTART) ( void );
typedef DWORD (*SCANDECWRITE) ( SCANDEC_WRITE *, INT * );
typedef DWORD (*SCANDECPAGEEND) ( SCANDEC_WRITE *, INT * );
typedef BOOL  (*SCANDECCLOSE) ( void );


#endif	/* ! __BROTHER_SCAN_DECODE_H */
