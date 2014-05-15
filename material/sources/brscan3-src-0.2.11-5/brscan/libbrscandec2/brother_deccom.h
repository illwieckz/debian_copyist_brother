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

/*******************************************************************************
 *
 *	Scan Decode 共通ヘッダーファイル
 *
 *	Copyright: 2000 brother Industries , Ltd.
 *
 *	ver 1.0.0 : 2000.04.13 : 稲田 : 新規作成
 ******************************************************************************/

/* カラー種別 */
#define	SC_BW		0x01
#define	SC_ED		0x02
#define	SC_DTH		0x04
#define	SC_TG		0x08
#define	SC_256		0x10
#define	SC_FUL		0x20
#define	SC_FULNOCM	0x40

#define	SC_2BIT		(0x01<<8)
#define	SC_8BIT		(0x02<<8)
#define	SC_24BIT	(0x04<<8)


#define	SCCLR_TYPE_BW		(SC_BW | SC_2BIT)		// Black & White
#define	SCCLR_TYPE_ED		(SC_ED | SC_2BIT)		// Error Diffusion Gray
#define	SCCLR_TYPE_DTH		(SC_DTH | SC_2BIT)		// Dithered Gray
#define	SCCLR_TYPE_TG		(SC_TG | SC_8BIT)		// True Gray
#define	SCCLR_TYPE_256		(SC_256 | SC_8BIT)		// 256 Color
#define	SCCLR_TYPE_FUL		(SC_FUL | SC_24BIT)		// 24bit Full Color
#define	SCCLR_TYPE_FULNOCM	(SC_FULNOCM | SC_24BIT)	// 24bit Full Color(do not colormatch)

/* 入力データ種別 */
#define	SCIDK_MONO		1	/* モノクロ */
#define	SCIDK_R			2	/* Rデータ */
#define	SCIDK_G			3	/* Gデータ */
#define	SCIDK_B			4	/* Bデータ */
#define	SCIDK_RGB		5	/* RGB画素順 */
#define	SCIDK_BGR		6	/* BGR画素順 */
#define	SCIDK_256		7	/* 256色カラー */

/* 24BitColor時のRGB出力形式 */
#define	SCODK_PIXEL_RGB		1	/* RGB RGB RGB画素順 */
#define	SCODK_PLANE			2	/* RRR GGG BBB画素順 */
#define	SCODK_PIXEL_BGR		3	/* BGR BGR BGR画素順 */

