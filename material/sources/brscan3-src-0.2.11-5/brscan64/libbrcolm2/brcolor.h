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
//#include "build.h"

// TWAIN ¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½èÍÂý ¥Ø¥Ã¥À¥Õ¥¡¥¤¥Âë 
// Create 97.1.8  Brother Systems T.YUGUCHI

#ifndef _H_BRCOLOR_
#define _H_BRCOLOR_

#define HWND void *

//---------------------------------------------------------------------- Äê¿ôÄêµÂÁ
// TWAIN¼Â¹Ô¥â¡¼¥É(¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½é´ü²½½èÍý¼Â¹Ô»þ¤Ë»ØÄê¡Ë
#define	TWAIN_SCAN_MODE				0	//TWAIN¼Â¹Ô  ÄÌ¾ï¥¹¥­¥ã¥Ê¡¼¥â¡¼¥ÂÉ
#define TWAIN_COPY_MODE				1	//TWAIN¼Â¹Ô  £Ð£Ã £Ã£Ï£Ð£ÂÙ ¥â¡¼¥É

// ¥Þ¥Ã¥Á¥ó¥°ÂÐ¾Ý¥Ç¡¼¥Â¿ £Ò£Ç£Â¥Ç¡¼¥¿ÊÂ¤Ó
#define CMATCH_DATALINE_RGB			0	// Red , Green , Blue
#define CMATCH_DATALINE_BGR			1	// Blue , Green, Red

// ¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¼ïÊÂÌ(¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½èÍý¼Â¹Ô»þ¤Ë»ØÄê¡Ë
#define CMATCH_KIND_GAMMA			0	//¦Ã¥«¡¼¥ÖÊäÀµ
#define CMATCH_KIND_LUT				1	//LUT¤Ë¤è¤ë¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°
#define CMATCH_KIND_MONITOR			2	//¥â¥Ë¥¿¡¼¥­¥ã¥ê¥Ö¥ì¡¼¥·¥ç¥Âó

// ¥Þ¥Ã¥Á¥ó¥°ÍÑ¥Æ¡¼¥Ö¥ëÎÎ°è¥µ¥¤¥º
#define	GAMMA_TABLE_NAME			"BrGamma.dat"	
													//¦Ã¥«¡¼¥ÖÄêµÁ¥Æ¡¼¥Ö¥ëÌ¾
#define GAMMA_TABLE_ID				"BSGT"			//¦Ã¥«¡¼¥ÖÄêµÁ¥Æ¡¼¥Ö¥ÂëID
#define TABLE_GAMMA_SIZE			768 			//¦Ã¥«¡¼¥ÖÄêµÁ¥Æ¡¼¥Ö¥Âë
													//¦Ã¥Ç¡¼¥¿¥µ¥¤¥Âº 256*3(byte)	

#define MONITOR_GAMMA_TABLE_NAME	"BrMonCal.dat"
													//¥â¥Ë¥¿¡¼¥­¥ã¥ê¥Ö¥ì¡¼¥·¥ç¥ó¦ÃÃÍ¥Æ¡¼¥Ö¥ëÌ¾
#define MONITOR_GAMMA_ID			"BRMC"			//¥â¥Ë¥¿¡¼¥­¥ã¥ê¥Ö¥ì¡¼¥·¥ç¥ó¦ÃÃÍ¥Æ¡¼¥Ö¥ÂëID

#define LUT_FILE_NAME				"BrLutCm.dat"
													//LUT¥Ç¡¼¥¿ÄêµÁ¥Õ¥¡¥¤¥ëÌÂ¾
#define LUT_FILE_ID					"BLCM"			//LUT¥Ç¡¼¥¿ÄêµÁ¥Õ¥¡¥¤¥ë

#define LUT_KIND_SCAN				"SLUT"			//ÄÌ¾ï¥¹¥­¥ã¥Ê¡¼½èÍýÍÑ£Ì£Õ£ÂÔ
#define LUT_KIND_COPY				"CLUT"			//PC COPYÍÑ£Ì£Õ£ÂÔ

#ifdef	TABLE_00
	#define	LUT_RGB						0				//LUT ¥Ç¡¼¥¿¼ïÊÌ¥³¡¼¥É RGB
//  not used
//	#define LUT_CMY						1				//LUT ¥Ç¡¼¥¿¼ïÊÌ¥³¡¼¥É CMY
//	#define	LUT_LAB						2				//LUT ¥Ç¡¼¥¿¼ïÊÌ¥³¡¼¥É LAB
#endif	//TABLE_00
#ifdef	TABLE_VER
	#define	LUT_00						0				//grid only
	#define LUT_01						1				//grid , gamma , gray
#endif	//TABLE_VER

#define LUT_DEFINE_FILE				"BrLutDef.Dat"
													//LUTÁªÂòÍÑÄêµÁ¥Õ¥¡¥¤¥ë
#define LUT_DEFINE_FILE_ID			"BRLD"			//LUTÁªÂòÍÑÄêµÁ¥Õ¥¡¥¤¥ëID

//¥á¥Ç¥£¥¢¥¿¥¤¥×ÄêµÁ
//¸½ºß»ÈÍÑ¤·¤Æ¤¤¤Ê¤¤»È¤¤Êý
#define MEDIA_PLAIN					0				//PLAIN PAPER
#define	MEDIA_COTED					1				//COTED PAPER 320
#define	MEDIA_COTED720				2				//COTED PAPER 720
#define	MEDIA_GROSSY				3				//GROSSY
#define	MEDIA_OHP					4				//OHP
#define MEDIA_OHPMIRROR				5				//OHP MIRROR
#define	MEDIA_PHOTO					6				//PHOTO
//¤³¤Á¤é¤¬¸½ºß¤Î»È¤¤ÊÂý
#define	MEDIA_STD					0				//standard paper
//#define	MEDIA_PHOTO					6				//photo paper
#define	MEDIA_FB_STD				MEDIA_STD		//FB	standard paper
#define	MEDIA_FB_PHOTO				MEDIA_PHOTO		//FB	photo paper
#define	MEDIA_ADF_STD				7				//ADF	standard paper
#define	MEDIA_ADF_PHOTO				8				//ADF	photo paper

//¥É¥­¥å¥á¥ó¥È¥â¡¼¥ÉÄêµÁ
#define	DOCMODE_AUTO				0				//Document Mode AUTO
#define	DOCMODE_GRAPH				1				//Document Mode GRAPH
#define	DOCMODE_PHOTO				2				//Document Mode PHOTO
#define	DOCMODE_CUSTOM				3				//Document Mode Custom

//¥«¥é¡¼¥Þ¥Ã¥È¥ó¥°¥â¡¼¥É			
#define	COLOR_VIVID					0				//VIVID
#define	COLOR_MATCHSCREEN			1				//MATCH_SCREEN

//ICM(Win95)À©¸æÍÑ
//#define ICM_OFF					0				//ICM Off
#define ICM_THROUGH					1				//ICM On
#define	ICM_MANUAL					2				//ICM Manual				

//¥×¥ê¥ó¥¿¡¼¥É¥é¥¤¥Ð¡¼Ì¾
#define MFC_PRINTER_NAME			"Brother MFC-7000 Series"
#define MC_PRINTER_NAME				"Brother MC3000"

//INI¥Õ¥¡¥¤¥ëÌ¾
#define MFC_INIFILE_NAME			"Brmfc97c.ini"
#define MC_INIFILE_NAME				"Brmc97c.ini"

//µ¡¼ï£É£ÂÄ
#define	MFC7000						0				//MFC-7000¥·¥ê¡¼¥Âº
#define MC3000						1				//MC3000 (3in1)

//É¸½à£Ì£Õ£Ô ID
#define DEFAULT_LUT					"DLUT"
#define DEFAULT_PHOTO_LUT			"PHTO"
#define ADF_STD_LUT					"DADF"
#define ADF_PHOTO_LUT				"PADF"
//--------------------------------------------------------------------- ¹½Â¤ÂÎÄêµÁ
#pragma pack(1)

typedef struct{					//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½é´ü²½¥â¡¼¥É»ØÄê¹½Â¤ÂÎ¡§¥Õ¥¡¥¤¥ëÌ¾ÉÕ¤­
	int		nRgbLine;			//RGB¥Ç¡¼¥¿ÊÂ¤Ó	  (BGR  or RGB)
	int		nPaperType;			//¸¶¹Æ »æ¼ïÊÌ
	int		nMachineId;			//µ¡¼ïID
	LPSTR	lpLutName;			//Lut Name
}CMATCH_INIT;	

typedef struct{										//¥â¥Ë¥¿¡¼¥­¥ã¥ê¥Ö¥ì¡¼¥·¥ç¥ó¦ÃÃÍ¹½Â¤ÂÎ
	char	FileID[4];			//'BRMC'
	short	BlackPoint[4];		//index[0,1,2,3]=[r,g,b,reserved]
	float	Gamma[4];			//index[0,1,2,3]=[r,g,b,reserved]
	short	Flag;				//default Flag
}BRMONCALDAT; 

typedef struct{										//LUT¥Ç¡¼¥¿¹½Â¤ÂÎ
	short	sLutVer;			//lut version
}BRLUT_HEAD_DATA_VER;

typedef struct{										//LUT¥Ç¡¼¥¿¹½Â¤ÂÎ
//	short	sLutDataKind;		//LUT¥Ç¡¼¥¿¼±ÊÌ¥³¡¼¥É

	float	s3D_Xmin;			//3D DATA XºÇ¾®ÃÍ
	float	s3D_Xmax;			//3D DATA XºÇÂçÃÍ
	float	s3D_Ymin;			//3D DATA YºÇ¾®ÃÍ
	float	s3D_Ymax;			//3D DATA YºÇÂçÃÍ
	float	s3D_Zmin;			//3D DATA ZºÇ¾®ÃÍ
	float	s3D_Zmax;			//3D DATA ZºÇÂçÃÍ

	short	s3D_Xsplit;			//3D DATA XÊý¸þÊ¬³ä¿ô
	short	s3D_Ysplit;			//3D DATA YÊý¸þÊ¬³ä¿ô
	short	s3D_Zsplit;			//3D DATA ZÊý¸þÊ¬³ä¿ô

//	float	f3D_Xspase;			//3D¾®Î©ÊýÂÎ¶õ´Ö £ØÊý¸þ´Ö³Ö
//	float	f3D_Yspase;			//3D¾®Î©ÊýÂÎ¶õ´Ö £ÙÊý¸þ´Ö³Ö
//	float	f3D_Zspase;			//3D¾®Î©ÊýÂÎ¶õ´Ö £ÚÊý¸þ´Ö³Ö

}BRLUT_HEAD_DATA_00;

#ifdef	TABLE_01
typedef struct{										//LUT¥Ç¡¼¥¿¹½Â¤ÂÎ
//	short	sLutDataKind;		//LUT¥Ç¡¼¥¿¼±ÊÌ¥³¡¼¥É

	short	sLutnum;			//LUT data ¿Âô
	short	sGamnum;			//Gamma data ¿Âô
	short	sGraynum;			//Gray data ¿Âô

	short	s3D_Xmin;			//3D DATA XºÇ¾®ÃÍ
	short	s3D_Xmax;			//3D DATA XºÇÂçÃÍ
	short	s3D_Ymin;			//3D DATA YºÇ¾®ÃÍ
	short	s3D_Ymax;			//3D DATA YºÇÂçÃÍ
	short	s3D_Zmin;			//3D DATA ZºÇ¾®ÃÍ
	short	s3D_Zmax;			//3D DATA ZºÇÂçÃÍ

	short	s3D_Xgrid;			//3D DATA XÊý¸þgrid¿Âô
	short	s3D_Ygrid;			//3D DATA YÊý¸þgrid¿Âô
	short	s3D_Zgrid;			//3D DATA ZÊý¸þgrid¿Âô
	short	s3D_Xspace;			//3D DATA XÊý¸þgrid size
	short	s3D_Yspace;			//3D DATA YÊý¸þgrid size
	short	s3D_Zspace;			//3D DATA ZÊý¸þgrid size
}BRLUT_HEAD_DATA_01;
#endif	//TABLE_01

typedef struct{										//LUT¥Ç¡¼¥¿ÄêµÁ¥Õ¥¡¥¤¥ë¥Æ¡¼¥Ö¥ë¥Ø¥Ã¥À¾ðÊó¹½Â¤ÂÎ
	char	TableID[4];			//¥Æ¡¼¥Ö¥ë¼±ÊÌ¥³¡¼¥É
	long	lTableOffset;		//LUT¥Ç¡¼¥¿³«»Ï¥ª¥Õ¥»¥Ã¥ÈÃÍ
}BRLUT_FILE_HEAD;

typedef struct{										//LUT¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°ÂÐ¾Ý¥Ç¡¼¥¿¾ðÊó
#ifdef	TABLE_00
	BYTE	*pbIndex_X;			//ÆþÎÏ¥Ç¡¼¥¿ £ÂØ
	BYTE	*pbIndex_Y;			//ÆþÎÏ¥Ç¡¼¥¿ £ÂÙ
	BYTE	*pbIndex_Z;			//ÆþÎÏ¥Ç¡¼¥¿ £ÂÚ

	short	sColorType_X;		//ÆþÎÏ¥Ç¡¼¥¿ £Ø¥«¥é¡¼¥¿¥¤¥Â×  (Red = 0, Green = 1, Blue = 2) 
	short	sColorType_Y;		//ÆþÎÏ¥Ç¡¼¥¿ £Ù¥«¥é¡¼¥¿¥¤¥Â×
	short	sColorType_Z;		//ÆþÎÏ¥Ç¡¼¥¿ £Ú¥«¥é¡¼¥¿¥¤¥Â×
#endif	//TABLE_00
#ifdef	TABLE_01
	short	sData_X;			//ÆþÎÏ¥Ç¡¼¥¿ £ÂØ
	short	sData_Y;			//ÆþÎÏ¥Ç¡¼¥¿ £ÂÙ
	short	sData_Z;			//ÆþÎÏ¥Ç¡¼¥¿ £ÂÚ
#endif	//TABLE_01

	short	sIndex_X;			//LUT»²¾È¥¤¥ó¥Ç¥Ã¥¯¥¹ £ÂØ
	short	sIndex_Y;			//LUT»²¾È¥¤¥ó¥Ç¥Ã¥¯¥¹ £ÂÙ
	short	sIndex_Z;			//LUT»²¾È¥¤¥ó¥Ç¥Ã¥¯¥¹ £ÂÚ
}BRLUT_INPUT_DATA;

typedef struct{										//LUTÂåÉ½ÅÀ(Â¬¿§¥Ç¡¼¥¿¡Ë³ÊÇ¼¹½Â¤ÂÎ
	float	fExpPoint_X[8];		//ÂåÉ½ÅÀ £ÂÐ £Ø£°¡Á£Ø£·
	float	fExpPoint_Y[8];		//ÂåÉ½ÅÀ £ÂÐ £Ù£°¡Á£Ù£·
	float	fExpPoint_Z[8];		//ÂåÉ½ÅÀ £ÂÐ £Ú£°¡Á£Ú£·
}BRLUT_EXP_POINT;

typedef struct{										//LUTÂåÉ½ÅÀ¡ÊÏÀÍý¥Ç¡¼¥¿¡Ë³ÊÇ¼¹½Â¤ÂÎ
	float	fLogicPoint_X[8];
	float	fLogicPoint_Y[8];
	float	fLogicPoint_Z[8];
}BRLUT_LOGIC_POINT;

typedef struct{										//LUTÂåÉ½ÅÀ¥Ç¡¼¥¿³ÊÇ¼¹½Â¤ÂÎ
	float	fLutXData;			//ÂåÉ½ÅÀ £ØÊý¸þ¥Ç¡¼¥¿
	float	fLutYData;			//ÂåÉ½ÅÀ £ÙÊý¸þ¥Ç¡¼¥¿
	float	fLutZData;			//ÂåÉ½ÅÀ £ÚÊý¸þ¥Ç¡¼¥¿
}BRLUT_DATA;

#ifdef	GAMMA_ADJ
//typedef struct{										//LUT¥Ç¡¼¥¿Adjust³ÊÇ¼¹½Â¤ÂÎ
//	float	fGammaXData;			//£ØÊý¸þ¥Ç¡¼¥¿
//	float	fGammaYData;			//£ÙÊý¸þ¥Ç¡¼¥¿
//	float	fGammaZData;			//£ÚÊý¸þ¥Ç¡¼¥¿
//}BRLUT_GAMMA;
#endif	//GAMMA_ADJ

#ifdef	GRAY_ADJ
//typedef struct{										//LUT¥Ç¡¼¥¿Adjust³ÊÇ¼¹½Â¤ÂÎ
//	float	fAdjXData;			//£ØÊý¸þ¥Ç¡¼¥¿
//	float	fAdjYData;			//£ÙÊý¸þ¥Ç¡¼¥¿
//	float	fAdjZData;			//£ÚÊý¸þ¥Ç¡¼¥¿
//}BRLUT_GRAY;
#endif	//GRAY_ADJ

typedef struct{										//LUTÁªÂòÍÑ¥Ç¡¼¥¿¹½Â¤ÂÎ
	short	nScanMode;			//¥¹¥­¥ã¥Ê¡¼¥â¡¼¥ÉŽÞ¡ÂÊSCAN or COPY)
	short	nInputMedia;		//ÆþÎÏ¥á¥Ç¥£¥¢¡Ê»æ¥¿¥¤¥×¡Ë
	short	nOutputMedia;		//½ÐÎÏ¥á¥Ç¥£¥¢
	short	nDocMode;			//¥É¥­¥å¥á¥ó¥È¥â¡¼¥É
	short	nColorMatch;		//VIVID or MATCHSCREEN
	short	nIcmControl;		//ICM ¥â¡¼¥É
}BRLUT_SELECT;	

typedef struct{										//Hash Table DataÍÑ¹½Â¤ÂÂÎ
	BYTE	yColorX;
	BYTE	yColorY;
	BYTE	yColorZ;
}COLOR_HASH_DAT;

typedef struct{										//Hash Table ¥­¡¼ÍÑ¹½Â¤ÂÎ
	long			nHashKey;	//Hash Key
	COLOR_HASH_DAT	*pColorDat;	//Hash Data Pointer
}COLOR_HASH_KEY;
#pragma pack()

//--------------------------------------------------------------------- ´Ø¿ôÀë¸ÂÀ
#ifdef BRCOLOR

//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½é´ü²½½èÍÂý
BOOL ColorMatchingInit(CMATCH_INIT matchingInitDat);

//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½ªÎ»½èÍý
void ColorMatchingEnd(void);

//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½èÍÂý
BOOL ColorMatching(BYTE *pRgbData, long lRgbDataLength, long lLineCount);


//¦Ã¥«¡¼¥ÖÊäÀµÍÑ¥Æ¡¼¥Ö¥ë½é´ü²½½èÍÂý
BOOL InitGammaTable(void);

//¥â¥Ë¥¿¡¼¥­¥ã¥ê¥Ö¥ì¡¼¥·¥ç¥ó½é´ü²½½èÍý
BOOL InitMonitorGamma(void);

//LUT½é´ü²½½èÍý
BOOL InitLUT(char* pcLutKind);

//¦Ã¥«¡¼¥ÖÊäÀµ½èÍý		
void GammaCurveMatching(BYTE *pRgbData, long lRgbDataLength, long lLineCount);

//LUT ¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½èÍÂý
BOOL LutColorMatching(BYTE *pRgbData, long lRgbDataLength, long lLineCount);

//¥â¥Ë¥¿¡¼¥­¥ã¥ê¥Ö¥ì¡¼¥·¥ç¥ó½èÍý
void MonitorCalibration(BYTE *pRgbData, long lRgbDataLength, long lLineCount);

//LUT»²¾È¥¤¥ó¥Ç¥Ã¥¯¥¹¼èÆÀ½èÍý
BOOL LutIndexGet(float fInPutData, short nLutColor, short *psIndex);

//LUTÂåÉ½ÅÀÂ¬¿§¼èÆÀ½èÍý(Â¬¿§ÃÍ¡ÂË
BOOL LutExampleDataGet(BRLUT_INPUT_DATA	lutMatchInputData,
						BRLUT_EXP_POINT *plutExampleData);

//LUTÂåÉ½ÅÀ¼èÆÀ½èÍÂý(ÏÀÍýÃÍ¡ÂË
BOOL LutLogicalDataGet(BRLUT_INPUT_DATA	lutMatchInputData,
						BRLUT_LOGIC_POINT *plutLogicData);

//¥Þ¥Ã¥Á¥ó¥°¥Ç¡¼¥¿ ·×»»½èÍÂý
BOOL CreateMatchData(BRLUT_INPUT_DATA lutMatchInputData, BRLUT_EXP_POINT lutExampleData,
					BRLUT_LOGIC_POINT lutLogicData);

//LUTÁªÂò½èÍÂý
BOOL LutSelect(BRLUT_SELECT lutSelectData, char * lutTableID);

//¥×¥ê¥ó¥¿¡¼¥â¡¼¥É¼èÆÀ½èÍý
BOOL GetPrinterMode(BRLUT_SELECT* plutSelectData);

//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¥Ï¥Ã¥·¥å½èÍý ¥Ï¥Ã¥·¥å¥Æ¡¼¥Ö¥ëºîÀ®½èÍý
BOOL StoreColorHash(void);

//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¥Ï¥Ã¥·¥å½èÍý ¥Ï¥Ã¥·¥å¸¡º÷½èÍý
BOOL LookupColorHash(BRLUT_INPUT_DATA lutMatchInputData);

//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¥Ï¥Ã¥·¥å½èÍý ½ªÎ»½èÍÂý
BOOL EndColorHash(void);  

//LUTÂåÉ½ÅÀ(¾®¶õ´Ö £¸¥Ý¥¤¥ó¥È)Â¬¿§¥Ç¡¼¥¿¼èÆÂÀ
//LUTÂåÉ½ÅÀ(¾®¶õ´Ö £¸¥Ý¥¤¥ó¥È)ÏÀÍý¥Ç¡¼¥¿ÃÍ¼èÆÀ
//¥Þ¥Ã¥Á¥ó¥°¥Ç¡¼¥¿ ·×»»½èÍÂý
BOOL MatchDataGet(BRLUT_INPUT_DATA	*lutMatchInputData);

//-------------------------------------------------------------------¥Þ¥Ã¥Á¥ó¥°ÍÑ¥Æ¡¼¥Ö¥Âë
BRMONCALDAT		gMonitorGammaDat;		//¥â¥Ë¥¿¡¼¥­¥ã¥ê¥Ö¥ì¡¼¥·¥ç¥ó¦ÃÃÍ¹½Â¤ÂÎ


HANDLE			ghGammaCurveTable;		//¦Ã¥«¡¼¥ÖÄêµÁ¥Æ¡¼¥Ö¥Âë
BYTE			*gpbGammaCurveTable;

BRLUT_HEAD_DATA_00	gLutDataHead00;		//LUT ¥Ç¡¼¥¿¥Ø¥Ã¥À¾ðÊó
#ifdef	TABLE_01
BRLUT_HEAD_DATA_01	gLutDataHead;		//LUT ¥Ç¡¼¥¿¥Ø¥Ã¥À¾ðÊó
#endif	//TABLE_01


HANDLE			ghLutDataTable;			//LUT ¥Æ¡¼¥Ö¥Âë
BRLUT_DATA		*gpfLutDataTable;		//LUT DATA
#ifdef	GAMMA_ADJ

HANDLE			ghLutGammaTable;		//Gamma table
BRLUT_DATA		*gpfLutGammaTable;		//Gamma data
#endif	//GAMMA_ADJ
#ifdef	GRAY_ADJ

HANDLE			ghLutGrayTable;			//Gray table
BRLUT_DATA		*gpfLutGrayTable;		//Gray data
#endif	//GRAY_ADJ

CMATCH_INIT		gMatchingInitDat;		//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½é´ü²½¾ðÊó¹½Â¤ÂÂÎ


BRLUT_SELECT 	glutSelectData;			//LUTÁªÂò¾ðÊÂó

char			szgWinSystem[145];		//Windows System ¥Ç¥£¥ì¥¯¥È¥ê
#ifdef	INIT_LUT_NAME
char			szgLutDir[145];			//Lut Data ¥Ç¥£¥ì¥¯¥È¥ê
#endif	//INIT_LUT_NAME
char			szgOpenFile[160];


HANDLE			ghHashTable;			//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¹âÂ®²½ÍÑ¥Ï¥Ã¥·¥å¥Æ¡¼¥Ö¥ë
COLOR_HASH_DAT	*gpHashTable;
//COLOR_HASH_KEY	colorHashKey[1000];		//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¹âÂ®²½ÍÑ¥Ï¥Ã¥·¥å¥­¡¼¥Æ¡¼¥Ö¥Âë
//COLOR_HASH_KEY	colorHashKey[3375];		//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¹âÂ®²½ÍÑ¥Ï¥Ã¥·¥å¥­¡¼¥Æ¡¼¥Ö¥Âë
int				gnColorHashFlag;		//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°¹âÂ®²½ÍÑ¥Ï¥Ã¥·¥å½èÍý¥Õ¥é¥°ÂŽÞ(=0 :OFF, =1 :ON)				

#else	//BRCOLOR
//---------------------------------------------------------------³°Éô¸ø³Â« ´Ø¿ôÀë¸ÂÀ
//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½é´ü²½½èÍÂý
BOOL ColorMatchingInit(CMATCH_INIT matchingInitDat);
//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½ªÎ»½èÍý
void ColorMatchingEnd(void);
//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½èÍÂý
BOOL ColorMatching(BYTE *pRgbData, long lRgbDataLength, long lLineCount);

//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½é´ü²½½èÍÂý
typedef BOOL (*COLORINIT)(CMATCH_INIT);
//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½ªÎ»½èÍý
typedef void (*COLOREND)(void);
//¥«¥é¡¼¥Þ¥Ã¥Á¥ó¥°½èÍÂý
typedef BOOL (*COLORMATCHING)(BYTE *, long , long );

#endif	//BRCOLOR

#endif // _H_BRCOLOR_

