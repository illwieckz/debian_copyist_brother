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
/*
;==============================================================================
;	file name		: brother_modelinf.h
;	create-date		: 2003.08.07
;       memo	        	: 
;==============================================================================
;	history
;	date		modifier	 comment
;       2005/04/18      Kazzy            modify for ALL,BH3
;==============================================================================
*/

#ifndef _H_BROTHER_MODELINF
#define _H_BROTHER_MODELINF

#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
#include "brother_advini.h"        //for OEM  kado
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado


/*======================*/
/* fundamental constant	*/
/*======================*/

/* define the section names */
#define		SUPORT_MODEL_SECTION	"Support Model"
#define		MODEL_TYPE_NAME_SECTION	"ModelTypeName"
#define		DRIVER_SECTION		"Driver"

/* define the error information */
#define		ERR_STRING		"ERR"
#define		ERROR_INT		255

/* define the key-name */
#define		FAX_RESO_KEY	"scanfast24"
#define		NO_USE_CM_KEY	"NoUseCM"
#define		COMPRESS_KEY	"compression"
#define		LOGFILE_KEY	"LogFile"
#define		IN_BUF_KEY      "Inqueue"


#if       BRSANESUFFIX == 2
/* define the ini-file name */
#define		INIFILE_NAME	"Brsane2.ini"

/* define the series-name */
#define		MAX_SERIES_NO	19	/* the number of model-series */

#define		GENERIC_RGB_MODEL	1
#define         GENERIC_RGB_NOADF	2
#define		GENERIC_RGB_NOFB	3

#define		ALL_SF_TYPE		5
#define		ALL_FB_DCP		6
#define		ALL_FB_ONLY		7

#define         AL_FB_DCP               8
#define         AL_DUPLEX               9 //06/02/28

#define		BH3_FB_ONLY		10
#define		BH3_SF_TYPE		11
#define		BH3_FB_DCP		12

#define		GENERIC_YCBCR_MODEL	13
#define		GENERIC_YCBCR_NOADF	14
#define		GENERIC_YCBCR_NOFB	15

#define         L4CFB                   16

/* Define generic model for Another endpoiont */
#define		GENERIC_YCBCR_MODEL_2	17
#define		GENERIC_YCBCR_NOADF_2	18
#define		GENERIC_YCBCR_NOFB_2	19

/* define the breakpoint of modelname that must be converted color mode */
#define         MUST_CONVERT_MODEL      10  

/* define models that must be chenaged endpoint */
#define         ANOTHERENDPOINT         6
int ChangeEndpoint[] = {AL_FB_DCP,AL_DUPLEX,L4CFB,GENERIC_YCBCR_MODEL_2,
                        GENERIC_YCBCR_NOADF_2,GENERIC_YCBCR_NOFB_2};

/* define the GRAY-LEVEL data file name */
/*
#define		ALL_FB_DCP_NAME		"ALL/brmfgray.bin"
#define		ALL_SF_TYPE_NAME	"ALL/brmsl07.bin"
#define		ALL_FB_DCP_NAME		"ALL/brmsl07f.bin"
#define		BH3_FB_ONLY_NAME	"BH3/brmfgray.bin"
#define		BH3_SF_TYPE_NAME	"BH3/brmsi06.bin"
#define		BH3_FB_DCP_NAME		"BH3/brmsi06f.bin"
*/

/* define the color-matching data file name */
#define		ALL_FB_DCP_CM_NAME	"ALL/brmsl08f.cm"
#define         AL_FB_DCP_CM_NAME       "AL/brmsl09f.cm"


#elif  BRSANESUFFIX == 1
/* define the ini-file name */
#define		INIFILE_NAME	"Brsane.ini"

/* define the series-name */
#define		MAX_SERIES_NO	12	/* the number of model-series */

#define		YL4_SF_TYPE		1
#define		YL4_FB_DCP		2
#define		ZLE_SF_TYPE		3
#define		ZLE_FB_DCP		4
#define		ZL2_SF_TYPE		5
#define		ZL2_FB_DCP		6
#define		BHL_SF_TYPE		7
#define		BHL_FB_DCP		8
#define		BHM_FB_TYPE		9
#define		BHMINI_FB_ONLY	10
#define		BHL2_SF_TYPE	11
#define		BHL2_FB_DCP		12

/* define the GRAY-LEVEL data file name */
#define		YL4_SF_TYPE_NAME	"YL4/brmfgray.bin"
#define		YL4_FB_DCP_NAME		"YL4FB/brmfgray.bin"
#define		ZLE_SF_TYPE_NAME	"ZLe/brmfgray.bin"
#define		ZLE_FB_DCP_NAME		"ZLeFB/brmfgray.bin"
#define		ZL2_SF_TYPE_NAME	"ZL2/brmsl07.bin"
#define		ZL2_FB_DCP_NAME		"ZL2FB/brmsl07f.bin"
#define		BHL_SF_TYPE_NAME	"BHL/brmfgray.bin"
#define		BHL_FB_DCP_NAME		"BHLFB/brmfgray.bin"
#define		BHM_FB_TYPE_NAME	"BHMFB/brmfgray.bin"
#define		BHMINI_FB_ONLY_NAME	"BHminiFB/brmfgray.bin"
#define		BHL2_SF_TYPE_NAME	"BHL2/brmsi06.bin"
#define		BHL2_FB_DCP_NAME	"BHL2FB/brmsi06f.bin"

/* define the color-matching data file name */
#define		YL4_FB_DCP_CM_NAME	"YL4FB/brlutcm.dat"
#define		ZLE_FB_DCP_CM_NAME	"ZLeFB/brlutcm.dat"
#define		ZL2_FB_DCP_CM_NAME	"ZL2FB/brmsl07f.cm"


int ChangeEndpoint[] = {};
#define         ANOTHERENDPOINT         (0)


#else    //BRSANESUFFIX
  force causing compile error
#endif   //BRSANESUFFIX



/* define the resolution label */
#define DPI100x100  100
#define DPI150x150  150
#define DPI200x200  200
#define DPI300x300  300
#define DPI400x400  400
#define DPI600x600  600
#define DPI1200x1200  1200
#define DPI2400x2400  2400
#define DPI4800x4800  4800
#define DPI9600x9600  9600


//////////////////////////////////////////////////////////
///////  constants for read_ini_file function      ///////
//////////////////////////////////////////////////////////

#define		BUF_SIZE	1000	/* buffer size to store the records */
#define		SECTION_CHECK	0	/* state-label */


/////////////////////////////////////////////////////////
///////  constant for get_Suport_inf function     ///////
/////////////////////////////////////////////////////////

#define		MAX_STRING				30				/* max charactor number of LABEL */

#define		COLOR_TYPE_COUNT		5				/* the number of color types  */
#define		RESO_COUNT				10				/* the number of resolution type  */
#define		SCAN_SRC_COUNT			3				/* the number of ScanSrc */

/////////////////////////////////////
///////  other constants      ///////
/////////////////////////////////////

#define		WORD_MAX			65535
/*
#define		NOFIND		0
#define		FIND		1

#define		NULL_C		'\0'
#define		NULL_S		"\0"
#define		LF			'\n'

#define		END			1
#define		NOEND		0

#define		ON			1
#define		OFF			0

#define		FALSE		0
#define		TRUE		1
*/



/*==========================================*/
/* 	 definition of structures           */
/*==========================================*/

/*** structure of model information ***/
struct model_inf{
	struct	model_inf *next;		/* pointer to the next model information structure */
#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
        int             index;
        int             expcaps;                /* expanded capability flags */ 
	int		vendorID;		/* Vendor ID					 */
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
	int		productID;				/* Product ID					 */
	int		seriesNo;				/* series Number					 */
	char	*modelName;				/* Model name						 */
	char	*modelTypeName;			/* Model type			 */
};
typedef struct model_inf MODELINF;
typedef struct model_inf * PMODELINF;

/*** supported resolution ***/
typedef union tagRESOLIST {
	struct {
		WORD  bDpi100x100:1;		/*  100 x  100 dpi */
		WORD  bDpi150x150:1;		/*  150 x  150 dpi */
		WORD  bDpi200x100:1;		/*  200 x  100 dpi */
		WORD  bDpi200x200:1;		/*  200 x  200 dpi */
		WORD  bDpi200x400:1;		/*  200 x  400 dpi */
		WORD  bDpi300x300:1;		/*  300 x  300 dpi */
		WORD  bDpi400x400:1;		/*  400 x  400 dpi */
		WORD  bDpi600x600:1;		/*  600 x  600 dpi */
		WORD  bDpi1200x1200:1;		/* 1200 x 1200 dpi */
		WORD  bDpi2400x2400:1;		/* 2400 x 2400 dpi */
		WORD  bDpi4800x4800:1;		/* 4800 x 4800 dpi */
		WORD  bDpi9600x9600:1;		/* 9600 x 9600 dpi */
	} bit;
	WORD val;
} RESOLIST, *PRESOLIST;

/*** supported ScanMode ***/
typedef union tagSCANMODELIST {
	struct {
		WORD  bBlackWhite:1;		/* Mono						 */
		WORD  bErrorDiffusion:1;	/* error-diffusion				 */
		WORD  bTrueGray:1;		/* gray scale					 */
		WORD  b24BitColor:1;		/* 24 bit color			       		 */
	  WORD  b24BitNoCMatch:1;		/* fast 24 bit color ¡Êwithout ColorMatch¡Ë 	 */
	} bit;
	WORD val;
} SCANMODELIST, *PSCANMODELIST;

/*** ScanSrc ***/
typedef union tagSCANSRCLIST {
	struct {
		WORD  FB	:1;				/* FlatBed			  */
		WORD  ADF	:1;				/* AutoDocumentFeeder */
		WORD  ADF_DUP	:1;				/* Duplex capability */
	} bit;
	WORD val;
} SCANSRCLIST, *PSCANSRCLIST;

/*** structure of MODEL_CONFIG ***/
typedef struct tagModelConfig {
	RESOLIST	 SupportReso;				/* supported resolution				 */
	SCANMODELIST     SupportScanMode;			/* supported ScanMode				 */
	SCANSRCLIST	 SupportScanSrc;			/* supported ScanSrc				 */
	double		 SupportScanAreaHeight;			/* supported length of scan area (x0.1mm) */
	double		 SupportScanAreaWidth;			/* supported width of scan area  (x0.1mm) */
	char		 szGrayLebelName[30];			/* data name of gray level	 */
	char		 szColorMatchName[30];			/* data name of color matching	 */
	BOOL		 bFaxResoEnable;			/* flag for FAX resolution			 */
	BOOL		 bNoUseColorMatch;			/* flag for disabling the ColorMatching			 */
	BOOL		 bCompressEnbale;			/* flag of enabling the compression				 */
	BOOL		 bLogFile;				/* flag of log file	 */
	WORD		 wInBuffSize;				/* size of input buffer			 */
} MODELCONFIG, *PMODELCONFIG;


/*==========================================*/
/*	 prototype difinision of exported function		*/
/*==========================================*/
int init_model_info(void);						/* Initialize the model information 		 */
int get_model_info(PMODELINF);					/* acquire the model information	 */
int exit_model_info(void);						/* terminate the process for model information	 */
int get_model_config(PMODELINF,PMODELCONFIG);	/* acquire the various information		 */

int ReadInitFileInt( LPCTSTR, LPCTSTR, int, LPCTSTR);
int ReadInitFileString( LPCTSTR, LPCTSTR, LPCTSTR, char *, int, LPCTSTR);
int ReadModelInfoSize(LPCTSTR, LPCTSTR, int *, int *, LPCTSTR);
int ReadModelInfo(LPCTSTR, LPTSTR, int, LPCTSTR);

int get_scanmode_string(SCANMODELIST, const char **);
int get_reso_int(RESOLIST, int *);
int get_scansrc_string(SCANSRCLIST, const char **);
int get_scanmode_id(const char *);
int get_reso_id(const int );
int get_scansrc_id(const char *);
int get_scanmode_listcnt(const char **, int);
int get_reso_listcnt(int *, int);
int get_scansrc_listcnt(const char **, int);

#endif	//_H_BROTHER_MODELINF
