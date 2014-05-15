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
//	Source filename: brother_modelinfo.c
//
//		Copyright(c) 2004-2008 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			ログファイル処理モジュール
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*
;==============================================================================
;	Filename		: brother_modelinf.c
;	Summary of capability  	: Get model information
;	Created			: 2003.08.06
;	Notes    		:
;==============================================================================
;	Histry
;	Date		Fixer	comments
;==============================================================================
*/
/*==========================================*/
/*		include 					*/
/*==========================================*/

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>

#include	"brother.h"
#include	"brother_dtype.h"
#include	"brother_modelinf.h"
#include	"brother_advini.h"                             // L-LNX-20

/*==========================================*/
/* 		prototype		*/
/*==========================================*/

static int GetHexInfo(char *,int *);
static int GetDecInfo(char *,int *);
static int GetModelNo(char *,char *);
static int NextPoint(char *);
static int GetSeriesNo(PMODELINF,int *);			/* Get series Number */
static void GetSupportReso(int,PMODELCONFIG);			/* Get information of supported resolution */
static void GetSupportScanMode(int,PMODELCONFIG);		/* Get information of supported scan mode */
#if 0  //M-LNX-20
static void GetSupportScanSrc(int,PMODELCONFIG);		/* Get information of supported scan source */
static void GetSupportScanAreaHeight(int,PMODELCONFIG);	        /* Get information of supported scan height */
static void GetSupportScanAreaWidth(int,PMODELCONFIG);	        /* Get information of supported scan width */
static void GetGrayLebelName(int,PMODELCONFIG);			/* Get filename for adjustting gray level */
static void GetColorMatchName(int,PMODELCONFIG);		/* Get filename for color mutching */
#else  //M-LNX-20
static void GetSupportScanSrc(int,PMODELINF,PMODELCONFIG);	/* Get information of supported scan source */
static void GetSupportScanAreaHeight(int,PMODELCONFIG);	        /* Get information of supported scan height */
static void GetSupportScanAreaWidth(int,PMODELCONFIG);	        /* Get information of supported scan width */
static void GetGrayLebelName(int,PMODELINF,PMODELCONFIG);	/* Get filename for adjustting gray level */
static void GetColorMatchName(int,PMODELINF,PMODELCONFIG);	/* Get filename for color mutching */
#endif  //M-LNX-20
static int GetFaxResoEnable(PMODELCONFIG);			/* Get resolution information for FAX */
static int GetNoUseColorMatch(PMODELCONFIG);			/* Get flag about color matching isn't used */
static int GetCompressEnbale(PMODELCONFIG);			/* Get flag about compression is enable	*/
static int GetLogFile(PMODELCONFIG);			        /* Get flag of log file */
static int GetInBuffSize(PMODELCONFIG);				/* Get information of input buffer size */

static int SectionNameCheck(LPCTSTR,char *);
static int  KeyNameCheckInt(LPCTSTR,char *,int *);
static int KeyNameCheckString(LPCTSTR, char *);
static void GetKeyValueString(LPTSTR, int,char *,int *);
static int AllSectionName(LPTSTR, int,char *,int *);
static int AllKeyName(LPTSTR,int,char *,int *);
static int GetModelInfoSize(int *,int *,char *);
static int GetModelInfoKeyValueSize(LPCTSTR,int *,char *);
static int AllReadModelInfo(LPTSTR, int, char *,int *);

static int ScanModeIDString(int, char *);
static int ResoIDInt(int, int *);
static int ScanSrcIDString(int, char *);

/*==========================================*/
/* 		parameter		    */
/*==========================================*/

static PMODELINF modelListStart;
static int modelListGetEnable = FALSE;


static const char bwString[]            = "Black & White";
static const char errDiffusionString[]  = "Gray[Error Diffusion]";
static const char tGrayString[]         = "True Gray";
static const char ColorString[]       = "24bit Color";
static const char ColorFastString[]   = "24bit Color[Fast]";

static const char fbString[] = "FlatBed";
static const char adfString[] = "Automatic Document Feeder";
static const char adfdupString[] = "Automatic Document Feeder(Duplex)";  //06/02/27

#define MAX_PATH 260

/*
;------------------------------------------------------------------------------
;	modulename	: init_model_info
;	Summay of capability	: Initialize model information
;	Input			: None
;	Return			: TRUE:Noamrl,FALSE:Not exist init file or [SupportModel]
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	histroy
;	date		fixer	comments
;------------------------------------------------------------------------------
*/

int init_model_info(void)
{
	char		*readModelInfo;
	char		*modelRecord;
	char		modelTypeNo[BUF_SIZE];
	char		*recordPoint;
	char		*readInfoPoint;
	PMODELINF	model;

	int	size;
	int	structSize;
	int	modelTypeSize;
	int	modelNameSize;
	int	recordLength;
	int	record;
	int	dummy;
	int	res;
	int	count;
	char szFileName[MAX_PATH];

	strcpy( szFileName, BROTHER_SANE_DIR );
	strcat( szFileName, INIFILE_NAME );
#ifndef  NET_AND_ADVINI //for network and inifile extension (M-LNX16,17) kado
	modelListGetEnable = ReadModelInfoSize(SUPORT_MODEL_SECTION,NULL, &size, &record, szFileName);/* Get size of model information from init fil */
#else    //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
	modelListGetEnable = ReadModelInfoSize2(
	       &size, &record);/* Get size of model information from init fil */
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado



	if(modelListGetEnable != TRUE)								    /* Fail to get size */
		return modelListGetEnable;							    /* error return */
	if(NULL == (readModelInfo = MALLOC( size + record + 1)))			            /* Allocate memory to strage model information */
	{
		/* fail to memory allocate */
		modelListGetEnable = FALSE;
		return modelListGetEnable;		/* error return */
	}
#ifndef  NET_AND_ADVINI //for network and inifile extension (M-LNX16,17) kado
	modelListGetEnable = ReadModelInfo(SUPORT_MODEL_SECTION, readModelInfo, size, szFileName);	/*Get model information from file  */
#else    //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
        modelListGetEnable = ReadModelInfo2(
		  readModelInfo, 
		  size);	/*Get model information from file  */ 
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
	if(modelListGetEnable != TRUE)			/* Fail to get model information */
	{
		FREE(readModelInfo);			/* Free allocated area */
		return modelListGetEnable;		/* error return */
	}
	if(NULL == (modelListStart = MALLOC( (structSize=sizeof(MODELINF)) * (record+1))))	/* Allocate memory for storing model name  */
	{
		/* Fail to memory allocate */
		modelListGetEnable = FALSE;
		FREE(readModelInfo);			/* Free allocated area*/
		return modelListGetEnable;		/* Error return */
	}
	model = modelListStart;
	readInfoPoint = readModelInfo;
	count = 0;
	while(1)
	{
#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
	        model->index = count;
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
		count++;						 /* Add record number */
		model->next = NULL_C;				         /* Add null to pointer of next model. */
		recordLength = strlen(readInfoPoint);			 /* count length of record */
		if(NULL == ( modelRecord = MALLOC(recordLength+1)))	 /* Allocate memory for 1 record.*/
		{
			/* ERR処理 */
			(model-1)->next = NULL_C;
			exit_model_info();			         /* Free all alocated area */
			modelListGetEnable = FALSE;
			break;
		}
		strcpy(modelRecord,readInfoPoint);			 /* separate to 1 record1 */
		readInfoPoint += recordLength+1;			 /* Move recoed ponter */
		recordPoint = modelRecord;				 /* Set to pointer that indicated to array */
#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
		res = GetDecInfo(recordPoint,&(model->expcaps));	 /* M-LNX-20 get advanced capability flags */
		recordPoint += NextPoint(recordPoint);			 /* M-LNX-20 Move pointer that indicated array */
		res = GetHexInfo(recordPoint,&(model->vendorID));	 /* get product ID */
		recordPoint += NextPoint(recordPoint);			 /* Move pointer that indicated array */
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
		res = GetHexInfo(recordPoint,&(model->productID));	 /* get product ID */
		recordPoint += NextPoint(recordPoint);			 /* Move pointer that indicated array */
		res *= GetDecInfo(recordPoint,&(model->seriesNo));	 /* get siries number */
		recordPoint += NextPoint(recordPoint);			 /* Move pointer that indicated array */
		res *= GetModelNo(recordPoint,modelTypeNo);		 /* get model type number */
		modelTypeSize =0;
		if(res == TRUE)
			/* get size of model type name */
			res *= ReadModelInfoSize(MODEL_TYPE_NAME_SECTION,modelTypeNo, &modelTypeSize, &dummy, szFileName);
		if(NULL == (model->modelTypeName = MALLOC(modelTypeSize+1)) || res == FALSE)	/* allocate memory for model type name */
		{
			/* Error */
			if(res == FALSE && NULL != (model->modelTypeName))
				FREE(model->modelTypeName);
			FREE(modelRecord);
			(model-1)->next = NULL_C;
			exit_model_info();			        /* Free all alocated area */
			modelListGetEnable = FALSE;
			break;
		}
		ReadInitFileString(MODEL_TYPE_NAME_SECTION,modelTypeNo,ERR_STRING,model->modelTypeName,modelTypeSize,szFileName);	/* get model type name */
		if(NULL ==(recordPoint = strchr(recordPoint,',')) || 0 == strcmp(model->modelTypeName,ERR_STRING))	/* Move pointer that indicated array */
		{
			/* Error */
			FREE(modelRecord);
			FREE(model->modelTypeName);
			(model-1)->next = NULL_C;
			exit_model_info();						/* Free all alocated area */
			modelListGetEnable = FALSE;
			break;
		}
		recordPoint++;
		if(NULL !=strchr(recordPoint,','))
		{
			/* Error */
			FREE(modelRecord);
			FREE(model->modelTypeName);
			(model-1)->next = NULL_C;
			exit_model_info();						/* Free all alocated area */
			modelListGetEnable = FALSE;
			break;
		}
		modelNameSize = strlen(recordPoint);				        /* get size of model name */
		if(*recordPoint == '\"' && *(recordPoint+modelNameSize-1) == '\"')	/* delete "" from model name */
		{
			*(recordPoint+modelNameSize-1) = NULL_C;
			recordPoint++;
			modelNameSize --;
		}
		if(NULL == (model->modelName = MALLOC(modelNameSize+1)))			/* allocaate memory for model name */
		{
			/* Error */
			FREE(modelRecord);
			FREE(model->modelTypeName);
			(model-1)->next = NULL_C;
			exit_model_info();						/* Free all alocated area */
			modelListGetEnable = FALSE;
			break;
		}
		strcpy(model->modelName,recordPoint);		/* get model name */
		recordPoint += NextPoint(recordPoint)-1;	/* Move pointer that indicated array */
		FREE(modelRecord);				/* Free redord area */
		if(count >= record)				/* check continuance */
		{
			/* End */
			modelListGetEnable = TRUE;
			break;
		}
		model->next = model+1;			        /* Add pointer to next model information */
		model = model->next;				/* Move to next model structure */
	}

	FREE(readModelInfo);
	return modelListGetEnable;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetHexInfo
;	Summary of Capability	: convert first item that delimited by "," in redcoed to hexdecimal
;	input			: record pointer,storage pointer
;	Return			: TRUE:Normal,FALSE:error
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int GetHexInfo(char *modelRecord,int *receiveInfo)
{
	char	para[BUF_SIZE];
	char	*comma_pt;
	int		res;

	res = FALSE;
	comma_pt = strchr(modelRecord,',');			/* check position of "," */
	if(comma_pt != NULL)
	{
		*comma_pt = NULL_C;
		strcpy(para,modelRecord);
		*receiveInfo = strtol(para,(char **)(para+strlen(para)),16);	/* Convert to hex */
		res = TRUE;
	}

	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetDecInfo
;	Summary of Capability	: convert first item that delimited by "," in redcoed to decimal
;	Iinput			: record pointer,storage pointer
;	Return			: TRUE:normal,FALSE:error
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int GetDecInfo(char *modelRecord,int *receiveInfo)
{
	char	para[BUF_SIZE];
	char	*comma_pt;
	int		res;

	res = FALSE;
	comma_pt = strchr(modelRecord,',');			/* check position of ","  */
	if(comma_pt != NULL)
	{
		strcpy(para,modelRecord);
		*comma_pt = NULL_C;
		*receiveInfo = strtol(para,(char **)(para+strlen(para)),10);	/* convert to decimal*/
		res = TRUE;
	}

	return res;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: NextPoint
;	Summary of Capability	: get next value that delimited by NULL
;	Iinput			: Ponter to move
;	Return			: TRUE:normal,FALSE:Not exist next value
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int NextPoint(char *point)
{
	int length;

	if(1 <= (length = strlen(point)))	     /* count length of string */
		length++;			     /* if string is exist,increase length +1 */
	else
		length = 0;			     /* if string not exist,length is 0 */
	return length;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetModelNo
;	Summary of Capability	: get model type number
;	Iinput			: record pointer,strage pointer
;	Return			: TRUE:normal,FALSE:error
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetModelNo(char *modelRecord,char *modelTypeNo)	      /* get model type number */
{
	int		length;
	int		res;

	res = FALSE;
	length = strcspn(modelRecord,",");				/* check position of "," */
	if(length != 0)
	{
		strncpy(modelTypeNo,modelRecord,length);	/* Copy last element */
		*(modelTypeNo+length) = NULL_C;			/* Add NULL */
		res = TRUE;
	}
	return res;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: get_model_info
;	Summary of Capability	: get corresponding model information
;	Iinput			: pointer of model_inf structure
;	Return			: TRUE:normal,FALSE:not exist setting file or SupportModel
;	Created			: 2003.08.07
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int get_model_info(PMODELINF modelInfList)
{
	int	res;

	res = FALSE;
	if(modelListGetEnable == TRUE )
	{
		modelInfList->modelName = modelListStart->modelName;		/* Pass the information of model list */
		modelInfList->modelTypeName = modelListStart->modelTypeName;	/* Pass model type name  */
		modelInfList->next = modelListStart->next;		        /* Pass pointer of next model information structure */
#ifdef    NET_AND_ADVINI  //for network and inifile extension (M-LNX16,17) kado
		modelInfList->expcaps = modelListStart->expcaps;		/* M-LNX-20 */
		modelInfList->vendorID = modelListStart->vendorID;		/* Pass product ID */
		modelInfList->index = modelListStart->index;	      
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
		modelInfList->productID = modelListStart->productID;		/* Pass product ID */
		modelInfList->seriesNo = modelListStart->seriesNo;		/* Pass serial ID */
		res = TRUE;
	}
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: exit_model_info
;	Summary of Capability	: terminate to conduct model information
;	Iinput			: None
;	Return			: TRUE:normal,FALSE:not exist setting file or SupportModel
;	Created			: 2003.08.07
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int exit_model_info(void)
{
	MODELINF  modelInfList;
	PMODELINF model;
	int res;

	res = get_model_info(&modelInfList);
	if(res == TRUE)
	{
		model = &modelInfList;
		while(1)
		{
			FREE(model->modelName);			/* Free model name area */
			FREE(model->modelTypeName);		/* Free model type name area */
			if(model->next == NULL)			/* Is exist next model structure? */
			{
				FREE(modelListStart);		/* Free all sturuct area */
				modelListGetEnable = FALSE;
				break;
			}
			model = model->next;			/* Move next structure */
		}
	}
	return res;

}



/*
;------------------------------------------------------------------------------
;	Module Name	: get_model_config
;	Summary of Capability	: Initialize model information
;	Iinput			: pointer to model infortmation structure,pointer to model setting structure
;	Return			: TRUE:normal,FALSE:not exist setting file
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int get_model_config(PMODELINF modelInf,PMODELCONFIG modelConfig)
{
	int	res;
	int series;

	res = GetSeriesNo(modelInf,&series);			/* get series number		 */
	GetSupportReso(series,modelConfig);			/* get supported resolution	 */
	GetSupportScanMode(series,modelConfig);			/* get supported scan mode	 */
#if 0  //M-LNX-20
	GetSupportScanSrc(series,modelConfig);			/* get supported scan source	 */
	GetSupportScanAreaHeight(series,modelConfig);	        /* get supported scan length	 */
	GetSupportScanAreaWidth(series,modelConfig);	        /* get supported scan width	 */
	GetGrayLebelName(series,modelConfig);			/* get filename for adjusting gray level */
	GetColorMatchName(series,modelConfig);			/* get filename forr color matching */
#else  //M-LNX-20
	GetSupportScanSrc(series,modelInf,modelConfig);		/* get supported scan source	 */
	GetSupportScanAreaHeight(series,modelConfig);	        /* get supported scan length	 */
	GetSupportScanAreaWidth(series,modelConfig);	        /* get supported scan width	 */
	GetGrayLebelName(series,modelInf,modelConfig);		/* get filename for adjusting gray level */
	GetColorMatchName(series,modelInf,modelConfig);		/* get filename forr color matching */
#endif  //M-LNX-20
	res *= GetFaxResoEnable(modelConfig);			/* get flag of resolution for FAX	 */
	res *= GetNoUseColorMatch(modelConfig);			/* get flag of not use ColorMacth		 */
	res *= GetCompressEnbale(modelConfig);			/* get flag of compression is enable */
	res *= GetLogFile(modelConfig);				/* get flag of log file */

	res *= GetInBuffSize(modelConfig);			/* get input buffer size		 */

	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetSeriesNo
;	Summary of Capability	: get series number
;	Iinput			: pointer of model information structure,pointer of series number
;	Return			: None
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int GetSeriesNo(PMODELINF modelInf,int *series)
{
	int res;

  	*series = modelInf->seriesNo;
	if(*series <= 0 || MAX_SERIES_NO < *series)	      /* check series number */
		res = FALSE;				      /* error */
	else
		res = TRUE;
	return res;
}

#if       BRSANESUFFIX == 2
/*
;------------------------------------------------------------------------------
;	Module Name	: GetSupportReso
;	Summary of Capability	: get reslution information
;	Iinput			: series number,pointer of model setting structure
;	Return			: None
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2004/04/18      Kazzy   
;------------------------------------------------------------------------------
*/
void GetSupportReso(int series,PMODELCONFIG modelConfig)
{
	modelConfig->SupportReso.val   = 0x0000;						/* initialize */
	switch(series)
	{
		case	ALL_SF_TYPE:
		  	modelConfig->SupportReso.bit.bDpi100x100   = TRUE;		/*  100 x  100 dpi */
			modelConfig->SupportReso.bit.bDpi150x150   = TRUE;		/*  150 x  150 dpi */
			modelConfig->SupportReso.bit.bDpi200x200   = TRUE;		/*  200 x  200 dpi */
			modelConfig->SupportReso.bit.bDpi300x300   = TRUE;		/*  300 x  300 dpi */
			modelConfig->SupportReso.bit.bDpi400x400   = TRUE;		/*  400 x  400 dpi */
			modelConfig->SupportReso.bit.bDpi600x600   = TRUE;		/*  600 x  600 dpi */
			modelConfig->SupportReso.bit.bDpi1200x1200 = TRUE;		/* 1200 x 1200 dpi */
			modelConfig->SupportReso.bit.bDpi2400x2400 = FALSE;		/* 2400 x 2400 dpi */
			modelConfig->SupportReso.bit.bDpi4800x4800 = FALSE;		/* 4800 x 4800 dpi */
			modelConfig->SupportReso.bit.bDpi9600x9600 = FALSE;		/* 9600 x 9600 dpi */
			break;

		case	BH3_SF_TYPE:
			modelConfig->SupportReso.bit.bDpi100x100   = TRUE;		/*  100 x  100 dpi */
			modelConfig->SupportReso.bit.bDpi150x150   = TRUE;		/*  150 x  150 dpi */
			modelConfig->SupportReso.bit.bDpi200x200   = TRUE;		/*  200 x  200 dpi */
			modelConfig->SupportReso.bit.bDpi300x300   = TRUE;		/*  300 x  300 dpi */
			modelConfig->SupportReso.bit.bDpi400x400   = TRUE;		/*  400 x  400 dpi */
			modelConfig->SupportReso.bit.bDpi600x600   = TRUE;		/*  600 x  600 dpi */
			modelConfig->SupportReso.bit.bDpi1200x1200 = TRUE;		/* 1200 x 1200 dpi */
			modelConfig->SupportReso.bit.bDpi2400x2400 = TRUE;		/* 2400 x 2400 dpi */
			modelConfig->SupportReso.bit.bDpi4800x4800 = FALSE;		/* 4800 x 4800 dpi */
			modelConfig->SupportReso.bit.bDpi9600x9600 = FALSE;		/* 9600 x 9600 dpi */
			break;

		default:
			modelConfig->SupportReso.bit.bDpi100x100   = TRUE;		/*  100 x  100 dpi */
			modelConfig->SupportReso.bit.bDpi150x150   = TRUE;		/*  150 x  150 dpi */
			modelConfig->SupportReso.bit.bDpi200x200   = TRUE;		/*  200 x  200 dpi */
			modelConfig->SupportReso.bit.bDpi300x300   = TRUE;		/*  300 x  300 dpi */
			modelConfig->SupportReso.bit.bDpi400x400   = TRUE;		/*  400 x  400 dpi */
			modelConfig->SupportReso.bit.bDpi600x600   = TRUE;		/*  600 x  600 dpi */
			modelConfig->SupportReso.bit.bDpi1200x1200 = TRUE;		/* 1200 x 1200 dpi */
			modelConfig->SupportReso.bit.bDpi2400x2400 = TRUE;		/* 2400 x 2400 dpi */
			modelConfig->SupportReso.bit.bDpi4800x4800 = TRUE;		/* 4800 x 4800 dpi */
			modelConfig->SupportReso.bit.bDpi9600x9600 = TRUE;		/* 9600 x 9600 dpi */
	}

	return;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetSupportScanMode
;	Summary of Capability	: get scan mode information
;	Iinput			: series number, pointer of model setting structure
;	Return			: なし
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2004/04/18      Kazzy   
;       2005/11/21      Kazzy   Add AL
;       2006/02/28      Kazzy   Add AL_DUPLEX
;------------------------------------------------------------------------------
*/
void GetSupportScanMode(int series,PMODELCONFIG modelConfig)
{
	modelConfig->SupportScanMode.val = 0x0000;								/* Initialize */
	switch(series)
	{
		case	ALL_SF_TYPE:
			modelConfig->SupportScanMode.bit.bBlackWhite     = TRUE;		/* B/W		*/
			modelConfig->SupportScanMode.bit.bErrorDiffusion = TRUE;		/* error diffusion */
			modelConfig->SupportScanMode.bit.bTrueGray       = TRUE;		/*  gray scale */
			modelConfig->SupportScanMode.bit.b24BitColor     = FALSE;		/* 24bit color		*/
			modelConfig->SupportScanMode.bit.b24BitNoCMatch  = FALSE;		/* 24bitcolor（no ColorMatch）*/
			break;

		case	ALL_FB_DCP:
		case	ALL_FB_ONLY:
	        case    AL_FB_DCP:
	        case    AL_DUPLEX:  
			modelConfig->SupportScanMode.bit.bBlackWhite     = TRUE;		/* B/W		*/
			modelConfig->SupportScanMode.bit.bErrorDiffusion = TRUE;		/* error diffusion	*/
			modelConfig->SupportScanMode.bit.bTrueGray       = TRUE;		/*  gray scale	*/
			modelConfig->SupportScanMode.bit.b24BitColor     = TRUE;		/* 24bitcolor		*/
			modelConfig->SupportScanMode.bit.b24BitNoCMatch  = TRUE;		/* 24bitcolor（ColorMatch）*/
			break;

		default:
			modelConfig->SupportScanMode.bit.bBlackWhite     = TRUE;		/* B/W		*/
			modelConfig->SupportScanMode.bit.bErrorDiffusion = TRUE;		/* error diffusion	*/
			modelConfig->SupportScanMode.bit.bTrueGray       = TRUE;		/* gray scale	*/
			modelConfig->SupportScanMode.bit.b24BitColor     = TRUE;		/* 24bitcolor		*/
			modelConfig->SupportScanMode.bit.b24BitNoCMatch  = FALSE;		/* 24bitcolor（no ColorMatch）*/
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetSupportScanSrc
;	Summary of Capability	: get scan source information
;	Iinput			: series number, pointer of model setting structure
;	Return			: None
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2006/02/28      Kazzy   DUPLEX
;------------------------------------------------------------------------------
*/
#if 0   //M-LNX-20
void GetSupportScanSrc(int series,PMODELCONFIG modelConfig)
{
	modelConfig->SupportScanSrc.val = 0x0000;					/* Initialize */
#else   //M-LNX-20
void GetSupportScanSrc(int series,PMODELINF modelInf ,PMODELCONFIG modelConfig){
	modelConfig->SupportScanSrc.val = 0x0000;					/* Initialize */
        if( (modelInf->expcaps & EXP_CAPDUPLEX) != 0){
	  modelConfig->SupportScanSrc.bit.ADF_DUP    = TRUE;		/* Duplex capability TRUE */
	}
	else{
	  modelConfig->SupportScanSrc.bit.ADF_DUP    = FALSE;		/* Duplex capability FALSE */
	}
#endif  //M-LNX-20
	switch(series)
	{
		case	ALL_SF_TYPE:
		case	BH3_SF_TYPE:
		case	GENERIC_RGB_NOFB:
		case	GENERIC_YCBCR_NOFB:
		case	GENERIC_YCBCR_NOFB_2:
			modelConfig->SupportScanSrc.bit.FB     = FALSE;		/* FlatBed				*/
			modelConfig->SupportScanSrc.bit.ADF    = TRUE;		/* AutoDocumentFeeder	*/
			break;

		case	ALL_FB_DCP:
		case	BH3_FB_DCP:
	        case    AL_FB_DCP:
	        case    L4CFB:
			modelConfig->SupportScanSrc.bit.FB     = TRUE;		/* FlatBed				*/
			modelConfig->SupportScanSrc.bit.ADF    = TRUE;		/* AutoDocumentFeeder	*/
			break;

		case	ALL_FB_ONLY:
		case    BH3_FB_ONLY:
		case	GENERIC_RGB_NOADF:
		case	GENERIC_YCBCR_NOADF:
		case	GENERIC_YCBCR_NOADF_2:
			modelConfig->SupportScanSrc.bit.FB     = TRUE;		/* FlatBed				*/
			modelConfig->SupportScanSrc.bit.ADF    = FALSE;		/* AutoDocumentFeeder	*/
			break;

		default:
		  	modelConfig->SupportScanSrc.bit.FB     = TRUE;		/* FlatBed				*/
			modelConfig->SupportScanSrc.bit.ADF    = TRUE;		/* AutoDocumentFeeder	*/
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetSupportScanAreaHeight
;	Summary of Capability	: get supported scan length
;	Iinput			: series number, pointer of model setting structure
;	Return			: なし
;	Created			: 2003.08.21
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2004/04/18      Kazzy   
;------------------------------------------------------------------------------
*/
void GetSupportScanAreaHeight(int series,PMODELCONFIG modelConfig)
{
	switch(series)
	{

		default:
#if 0  //M-LNX-58
			modelConfig->SupportScanAreaHeight = 297.0;
#else  //M-LNX-58
			modelConfig->SupportScanAreaHeight = 355.6;
#endif //M-LNX-58
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetSupportScanAreaWidth
;	Summary of Capability	: get supported scan width information
;	Iinput			: series number, pointer of model setting structure
;	Return			: None
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2004/04/18      Kazzy   
;------------------------------------------------------------------------------
*/
void GetSupportScanAreaWidth(int series,PMODELCONFIG modelConfig)
{
	switch(series)
	{

		default:
#if 0  //M-LNX-58
			modelConfig->SupportScanAreaWidth = 210.0;
#else  //M-LNX-58
			modelConfig->SupportScanAreaWidth = 215.9;
#endif //M-LNX-58
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetGrayLebelName
;	Summary of Capability	: Get filename for adjusting gray level.
;	Iinput			: series number, pointer of model setting structure
;	Return			: None
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2004/04/18      Kazzy   
;------------------------------------------------------------------------------
*/
#if 0  //M-LNX-20
void GetGrayLebelName(int series,PMODELCONFIG modelConfig)
{
	char	*name;
#else  //M-LNX-20
void GetGrayLebelName(int series,PMODELINF modelInf,PMODELCONFIG modelConfig){
	char	*name;
	name = (get_p_model_info_by_index(modelInf->index))->graylevelTBL;
	if(name){
	  if(*name){
	    strcpy(modelConfig->szGrayLebelName,name);
	    return;
	  }
	}
#endif  //M-LNX-20
	switch(series)						/* get filename from series number */
	{
		default:
			name = NULL_S;				/* if not exist,set NULL */
	}
	strcpy(modelConfig->szGrayLebelName,name);
	return;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: GetColorMatchName
;	Summary of Capability	: get filename of color matching
;	Iinput			: series number, pointer of model setting structure
;	Return			: なし
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2004/04/18      Kazzy   
;------------------------------------------------------------------------------
*/
#if 0  //M-LNX-20
void GetColorMatchName(int series,PMODELCONFIG modelConfig)
{
	char	*name;
#else  //M-LNX-20
void GetColorMatchName(int series,PMODELINF modelInf,PMODELCONFIG modelConfig){
	char	*name;
	name = (get_p_model_info_by_index(modelInf->index))->colmatchTBL;
	if(name){
	  if(*name){
	    strcpy(modelConfig->szColorMatchName,name);
	    return;
	  }
	}
#endif  //M-LNX-20

	/* get filename from series number */
	switch(series)
	{
		case	ALL_FB_DCP:
		case	ALL_FB_ONLY:
			name = ALL_FB_DCP_CM_NAME;
			break;
		case    AL_FB_DCP:
		case    AL_DUPLEX:
		        name = AL_FB_DCP_CM_NAME;
			break;
		default:
			name = NULL_S;	/* if not exist,set NULL */
			break;
	}
	strcpy(modelConfig->szColorMatchName,name);
	return;
}
#elif  BRSANESUFFIX == 1

/*
;------------------------------------------------------------------------------
;	モジュール名	: GetSupportReso
;	機能概要		: サポート解像度取得
;	入力			: シリーズ番号,モデル設定構造体ポインタ
;	戻り値			: なし
;	作成日			: 2003.08.11
;	特記事項		:
;------------------------------------------------------------------------------
;	変更履歴
;	日付		更新者	コメント
;------------------------------------------------------------------------------
*/
void GetSupportReso(int series,PMODELCONFIG modelConfig)
{
	modelConfig->SupportReso.val   = 0x0000;						/* 初期化 */
	switch(series)
	{
		case	YL4_SF_TYPE:
			modelConfig->SupportReso.bit.bDpi100x100   = TRUE;		/*  100 x  100 dpi */
			modelConfig->SupportReso.bit.bDpi150x150   = TRUE;		/*  150 x  150 dpi */
			modelConfig->SupportReso.bit.bDpi200x200   = TRUE;		/*  200 x  200 dpi */
			modelConfig->SupportReso.bit.bDpi300x300   = TRUE;		/*  300 x  300 dpi */
			modelConfig->SupportReso.bit.bDpi400x400   = TRUE;		/*  400 x  400 dpi */
			modelConfig->SupportReso.bit.bDpi600x600   = TRUE;		/*  600 x  600 dpi */
			modelConfig->SupportReso.bit.bDpi1200x1200 = TRUE;		/* 1200 x 1200 dpi */
			modelConfig->SupportReso.bit.bDpi2400x2400 = FALSE;		/* 2400 x 2400 dpi */
			modelConfig->SupportReso.bit.bDpi4800x4800 = FALSE;		/* 4800 x 4800 dpi */
			modelConfig->SupportReso.bit.bDpi9600x9600 = FALSE;		/* 9600 x 9600 dpi */
			break;

		case	BHL_SF_TYPE:
		case	BHL2_SF_TYPE:
			modelConfig->SupportReso.bit.bDpi100x100   = TRUE;		/*  100 x  100 dpi */
			modelConfig->SupportReso.bit.bDpi150x150   = TRUE;		/*  150 x  150 dpi */
			modelConfig->SupportReso.bit.bDpi200x200   = TRUE;		/*  200 x  200 dpi */
			modelConfig->SupportReso.bit.bDpi300x300   = TRUE;		/*  300 x  300 dpi */
			modelConfig->SupportReso.bit.bDpi400x400   = TRUE;		/*  400 x  400 dpi */
			modelConfig->SupportReso.bit.bDpi600x600   = TRUE;		/*  600 x  600 dpi */
			modelConfig->SupportReso.bit.bDpi1200x1200 = TRUE;		/* 1200 x 1200 dpi */
			modelConfig->SupportReso.bit.bDpi2400x2400 = TRUE;		/* 2400 x 2400 dpi */
			modelConfig->SupportReso.bit.bDpi4800x4800 = FALSE;		/* 4800 x 4800 dpi */
			modelConfig->SupportReso.bit.bDpi9600x9600 = FALSE;		/* 9600 x 9600 dpi */
			break;

		default:
			modelConfig->SupportReso.bit.bDpi100x100   = TRUE;		/*  100 x  100 dpi */
			modelConfig->SupportReso.bit.bDpi150x150   = TRUE;		/*  150 x  150 dpi */
			modelConfig->SupportReso.bit.bDpi200x200   = TRUE;		/*  200 x  200 dpi */
			modelConfig->SupportReso.bit.bDpi300x300   = TRUE;		/*  300 x  300 dpi */
			modelConfig->SupportReso.bit.bDpi400x400   = TRUE;		/*  400 x  400 dpi */
			modelConfig->SupportReso.bit.bDpi600x600   = TRUE;		/*  600 x  600 dpi */
			modelConfig->SupportReso.bit.bDpi1200x1200 = TRUE;		/* 1200 x 1200 dpi */
			modelConfig->SupportReso.bit.bDpi2400x2400 = TRUE;		/* 2400 x 2400 dpi */
			modelConfig->SupportReso.bit.bDpi4800x4800 = TRUE;		/* 4800 x 4800 dpi */
			modelConfig->SupportReso.bit.bDpi9600x9600 = TRUE;		/* 9600 x 9600 dpi */
	}

	return;
}

/*
;------------------------------------------------------------------------------
;	モジュール名	: GetSupportScanMode
;	機能概要		: サポートScanMode取得
;	入力			: シリーズ番号,モデル設定構造体ポインタ
;	戻り値			: なし
;	作成日			: 2003.08.11
;	特記事項		:
;------------------------------------------------------------------------------
;	変更履歴
;	日付		更新者	コメント
;------------------------------------------------------------------------------
*/
void GetSupportScanMode(int series,PMODELCONFIG modelConfig)
{
	modelConfig->SupportScanMode.val = 0x0000;								/* 初期化 */
	switch(series)
	{
		case	YL4_SF_TYPE:
		case	ZLE_SF_TYPE:
		case	ZL2_SF_TYPE:
			modelConfig->SupportScanMode.bit.bBlackWhite     = TRUE;		/* ２値(白黒)		*/
			modelConfig->SupportScanMode.bit.bErrorDiffusion = TRUE;		/* 誤差拡散			*/
			modelConfig->SupportScanMode.bit.bTrueGray       = TRUE;		/* グレースケール	*/
			modelConfig->SupportScanMode.bit.b24BitColor     = FALSE;		/* 24bitカラー		*/
			modelConfig->SupportScanMode.bit.b24BitNoCMatch  = FALSE;		/* 24bitカラー高速（ColorMatchなし）*/
			break;

		case	YL4_FB_DCP:
		case	ZLE_FB_DCP:
		case	ZL2_FB_DCP:
			modelConfig->SupportScanMode.bit.bBlackWhite     = TRUE;		/* ２値(白黒)		*/
			modelConfig->SupportScanMode.bit.bErrorDiffusion = TRUE;		/* 誤差拡散			*/
			modelConfig->SupportScanMode.bit.bTrueGray       = TRUE;		/* グレースケール	*/
			modelConfig->SupportScanMode.bit.b24BitColor     = TRUE;		/* 24bitカラー		*/
			modelConfig->SupportScanMode.bit.b24BitNoCMatch  = TRUE;		/* 24bitカラー高速（ColorMatchなし）*/
			break;

		default:
			modelConfig->SupportScanMode.bit.bBlackWhite     = TRUE;		/* ２値(白黒)		*/
			modelConfig->SupportScanMode.bit.bErrorDiffusion = TRUE;		/* 誤差拡散			*/
			modelConfig->SupportScanMode.bit.bTrueGray       = TRUE;		/* グレースケール	*/
			modelConfig->SupportScanMode.bit.b24BitColor     = TRUE;		/* 24bitカラー		*/
			modelConfig->SupportScanMode.bit.b24BitNoCMatch  = FALSE;		/* 24bitカラー高速（ColorMatchなし）*/
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	モジュール名	: GetSupportScanSrc
;	機能概要		: サポートScanSrc取得
;	入力			: シリーズ番号,モデル設定構造体ポインタ
;	戻り値			: なし
;	作成日			: 2003.08.11
;	特記事項		:
;------------------------------------------------------------------------------
;	変更履歴
;	日付		更新者	コメント
;------------------------------------------------------------------------------
*/
#if 0   //M-LNX-20
void GetSupportScanSrc(int series,PMODELCONFIG modelConfig)
{

	modelConfig->SupportScanSrc.val = 0x0000;					/* 初期化 */
#else   //M-LNX-20
void GetSupportScanSrc(int series,PMODELINF modelInf ,PMODELCONFIG modelConfig){
	modelConfig->SupportScanSrc.val = 0x0000;					/* 初期化 */
        if( (modelInf->expcaps & EXP_CAPDUPLEX) != 0){
	  modelConfig->SupportScanSrc.bit.ADF_DUP    = TRUE;		/* Duplex capability TRUE */
	}
	else{
	  modelConfig->SupportScanSrc.bit.ADF_DUP    = FALSE;		/* Duplex capability FALSE */
	}
#endif  //M-LNX-20
	switch(series)
	{
		case	YL4_SF_TYPE:
		case	ZLE_SF_TYPE:
		case	ZL2_SF_TYPE:
		case	BHL_SF_TYPE:
		case	BHL2_SF_TYPE:
			modelConfig->SupportScanSrc.bit.FB     = FALSE;		/* FlatBed				*/
			modelConfig->SupportScanSrc.bit.ADF    = TRUE;		/* AutoDocumentFeeder	*/
			break;

		case	YL4_FB_DCP:
		case	ZLE_FB_DCP:
		case	ZL2_FB_DCP:
		case	BHL_FB_DCP:
		case	BHM_FB_TYPE:
		case	BHL2_FB_DCP:
			modelConfig->SupportScanSrc.bit.FB     = TRUE;		/* FlatBed				*/
			modelConfig->SupportScanSrc.bit.ADF    = TRUE;		/* AutoDocumentFeeder	*/
			break;

		case	BHMINI_FB_ONLY:
			modelConfig->SupportScanSrc.bit.FB     = TRUE;		/* FlatBed				*/
			modelConfig->SupportScanSrc.bit.ADF    = FALSE;		/* AutoDocumentFeeder	*/
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	モジュール名	: GetSupportScanAreaHeight
;	機能概要		: サポート読み込み範囲長取得
;	入力			: シリーズ番号,モデル設定構造体ポインタ
;	戻り値			: なし
;	作成日			: 2003.08.21
;	特記事項		:
;------------------------------------------------------------------------------
;	変更履歴
;	日付		更新者	コメント
;------------------------------------------------------------------------------
*/
void GetSupportScanAreaHeight(int series,PMODELCONFIG modelConfig)
{
	switch(series)
	{
		case	BHMINI_FB_ONLY:
			modelConfig->SupportScanAreaHeight = 297.0;
			break;
		default:
			modelConfig->SupportScanAreaHeight = 355.6;
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	モジュール名	: GetSupportScanAreaWidth
;	機能概要		: サポート読み込み範囲幅取得
;	入力			: シリーズ番号,モデル設定構造体ポインタ
;	戻り値			: なし
;	作成日			: 2003.08.11
;	特記事項		:
;------------------------------------------------------------------------------
;	変更履歴
;	日付		更新者	コメント
;------------------------------------------------------------------------------
*/
void GetSupportScanAreaWidth(int series,PMODELCONFIG modelConfig)
{
	switch(series)
	{
		case	BHL2_SF_TYPE:
		case	BHL2_FB_DCP:
			modelConfig->SupportScanAreaWidth = 210.0;
			break;

		case	ZL2_FB_DCP:
			modelConfig->SupportScanAreaWidth = 212.0;
			break;

		default:
			modelConfig->SupportScanAreaWidth = 208.0;
			break;
	}
	return;
}

/*
;------------------------------------------------------------------------------
;	モジュール名	: GetGrayLebelName
;	機能概要		: グレイレベル用データ名
;	入力			: シリーズ番号,モデル設定構造体ポインタ
;	戻り値			: なし
;	作成日			: 2003.08.11
;	特記事項		:
;------------------------------------------------------------------------------
;	変更履歴
;	日付		更新者	コメント
;------------------------------------------------------------------------------
*/
#if 0  //M-LNX-20
void GetGrayLebelName(int series,PMODELCONFIG modelConfig)
{
	char	*name;
#else  //M-LNX-20
void GetGrayLebelName(int series,PMODELINF modelInf,PMODELCONFIG modelConfig){
	char	*name;
	name = (get_p_model_info_by_index(modelInf->index))->graylevelTBL;
	if(name){
	  if(*name){
	    strcpy(modelConfig->szGrayLebelName,name);
	    return;
	  }
	}
#endif  //M-LNX-20

	switch(series)						/* シリーズNoからNameを取得 */
	{
		case	YL4_SF_TYPE:
			name = YL4_SF_TYPE_NAME;
			break;
		case	YL4_FB_DCP:
			name = YL4_FB_DCP_NAME;
			break;
		case	ZLE_SF_TYPE:
			name = ZLE_SF_TYPE_NAME;
			break;
		case	ZLE_FB_DCP:
			name = ZLE_FB_DCP_NAME;
			break;
		case	ZL2_SF_TYPE:
			name = ZL2_SF_TYPE_NAME;
			break;
		case	ZL2_FB_DCP:
			name = ZL2_FB_DCP_NAME;
			break;
		case	BHL_SF_TYPE:
			name = BHL_SF_TYPE_NAME;
			break;
		case	BHL_FB_DCP:
			name = BHL_FB_DCP_NAME;
			break;
		case	BHM_FB_TYPE:
			name = BHM_FB_TYPE_NAME;
			break;
		case	BHMINI_FB_ONLY:
			name = BHMINI_FB_ONLY_NAME;
			break;
		case	BHL2_SF_TYPE:
			name = BHL2_SF_TYPE_NAME;
			break;
		case	BHL2_FB_DCP:
			name = BHL2_FB_DCP_NAME;
			break;
		default:
			name = NULL_S;				/* 該当するものがなければNULL */
	}
	strcpy(modelConfig->szGrayLebelName,name);
	return;
}


/*
;------------------------------------------------------------------------------
;	モジュール名	: GetColorMatchName
;	機能概要		: カラーマッチング用データ名
;	入力			: シリーズ番号,モデル設定構造体ポインタ
;	戻り値			: なし
;	作成日			: 2003.08.11
;	特記事項		:
;------------------------------------------------------------------------------
;	変更履歴
;	日付		更新者	コメント
;------------------------------------------------------------------------------
*/

#if 0  //M-LNX-20
void GetColorMatchName(int series,PMODELCONFIG modelConfig)
{
	char	*name;
#else  //M-LNX-20
void GetColorMatchName(int series,PMODELINF modelInf,PMODELCONFIG modelConfig){
	char	*name;
	name = (get_p_model_info_by_index(modelInf->index))->colmatchTBL;
	if(name){
	  if(*name){
	    strcpy(modelConfig->szColorMatchName,name);
	    return;
	  }
	}
#endif  //M-LNX-20


	/* シリーズ番号によりNameを取得 */
	switch(series)
	{
		case	YL4_FB_DCP:
			name = YL4_FB_DCP_CM_NAME;
			break;
		case	ZLE_FB_DCP:
			name = ZLE_FB_DCP_CM_NAME;
			break;
		case	ZL2_FB_DCP:
			name = ZL2_FB_DCP_CM_NAME;
			break;
		default:
			name = NULL_S;	/* 該当するものがなければNULL */
			break;
	}
	strcpy(modelConfig->szColorMatchName,name);
	return;
}

#else    //BRSANESUFFIX
  force causing compile error
#endif   //BRSANESUFFIX

/*
;------------------------------------------------------------------------------
;	Module Name	: GetFaxResoEnable
;	Summary of Capability	: get flag of resoluton of FAX
;	Iinput			: pointer of model setting structure
;	Return			: TRUE:success,FALSE:fail
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetFaxResoEnable(PMODELCONFIG modelConfig)
{
	int	res;
	char szFileName[MAX_PATH];

	strcpy( szFileName, BROTHER_SANE_DIR );
	strcat( szFileName, INIFILE_NAME );
	modelConfig->bFaxResoEnable = (BYTE)ReadInitFileInt(DRIVER_SECTION,FAX_RESO_KEY,ERROR_INT,szFileName);
	if(modelConfig->bFaxResoEnable == 0 || modelConfig->bFaxResoEnable == 1)	/* value check */
		res = TRUE;				/* success*/
	else
		res = FALSE;			/* fail*/
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetNoUseColorMatch
;	Summary of Capability	: get flag of not use color matching
;	Iinput			: pointer of model setting structure
;	Return			: TRUE:success,FALSE:fail
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetNoUseColorMatch(PMODELCONFIG modelConfig)
{
	int	res;
	char szFileName[MAX_PATH];

	strcpy( szFileName, BROTHER_SANE_DIR );
	strcat( szFileName, INIFILE_NAME );
	modelConfig->bNoUseColorMatch = (BYTE)ReadInitFileInt(DRIVER_SECTION,NO_USE_CM_KEY,ERROR_INT,szFileName);
	if(modelConfig->bNoUseColorMatch == 0 || modelConfig->bNoUseColorMatch == 1)	/* cehck value */
		res = TRUE;				/* success*/
	else
		res = FALSE;			/* fail */
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetCompressEnbale
;	Summary of Capability	: get flag of compression is enable
;	Iinput			: pointer of model setting structure
;	Return			: TRUE:success,FALSE:fail
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetCompressEnbale(PMODELCONFIG modelConfig)
{
	int	res;
	char szFileName[MAX_PATH];

	strcpy( szFileName, BROTHER_SANE_DIR );
	strcat( szFileName, INIFILE_NAME );
	modelConfig->bCompressEnbale = (BYTE)ReadInitFileInt(DRIVER_SECTION,COMPRESS_KEY,ERROR_INT,szFileName);
	if(modelConfig->bCompressEnbale == 0 || modelConfig->bCompressEnbale ==1)	/* check value */
		res = TRUE;				/* success */
	else
		res = FALSE;			/* fail */
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetLogFile
;	Summary of Capability	: get flag of log file
;	Iinput			: pointer of model setting structure
;	Return			: TRUE:success,FALSE:fail
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetLogFile(PMODELCONFIG modelConfig)
{
	int	res;
	char szFileName[MAX_PATH];

	strcpy( szFileName, BROTHER_SANE_DIR );
	strcat( szFileName, INIFILE_NAME );
	modelConfig->bLogFile = (BYTE)ReadInitFileInt(DRIVER_SECTION,LOGFILE_KEY,ERROR_INT,szFileName);
	if(modelConfig->bLogFile == 0 || modelConfig->bLogFile ==1)	/* check value */
		res = TRUE;				/* success */
	else
		res = FALSE;			/* fail */
	return res;
}
/*
;------------------------------------------------------------------------------
;	Module Name	: GetInBuffSize
;	Summary of Capability	: get input buffer size
;	Iinput			: pointer of model setting structure
;	Return			: TRUE:success,FALSE:fail
;	Created			: 2003.08.11
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetInBuffSize(PMODELCONFIG modelConfig)
{
	int	res;
	int bufsize;
	char szFileName[MAX_PATH];

	strcpy( szFileName, BROTHER_SANE_DIR );
	strcat( szFileName, INIFILE_NAME );
	bufsize = ReadInitFileInt(DRIVER_SECTION,IN_BUF_KEY,-1,szFileName );
	if( 0 <= bufsize || bufsize < WORD_MAX)		/* check buffer size */
	{
		res = TRUE;							/* success */
		modelConfig->wInBuffSize = (WORD)bufsize;
	}
	else
	{
		res = FALSE;							/* fail */
		modelConfig->wInBuffSize = 0;
	}
	return res;
}

/*
/////////////////////////////////////////////////
////////       read_ini_file         ////////
/////////////////////////////////////////////////
*/

/*
;------------------------------------------------------------------------------
;	Module Name	: ReadInitFileInt
;	Summary of Capability	: Convet key in ini file to integer
;	Iinput			: seach SectionName,seach lpKeyName,default value,seach file name
;	Return			: value of key(If dont came up,return default value)
;	Created			: 2003.07.31
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int ReadInitFileInt( LPCTSTR lpAppName,		/*  address of section name */
					 LPCTSTR lpKeyName,		/*  address of key name */
					 int nDefault,			/*  return value if key name is not found */
					 LPCTSTR lpFileName)	/*  address of initialization filename */
{
	int		result;
	int		sectionFind;
	int		keyFind;
	FILE	*rfile;
	char	buf[BUF_SIZE];
	char	state;

	state = 0;
	result = nDefault;						/* default return value */
	if(NULL == (rfile = fopen(lpFileName, "r")))	return result;	/* Can't open file:error:return */
	while(1)
	{
		if(feof(rfile))
		{
			break;							/* end of file */
		}
		else
		{
			if(NULL == fgets(buf,BUF_SIZE,rfile))		break;		/*fail to get line */
			if(state == SECTION_CHECK)
			{
				sectionFind = SectionNameCheck(lpAppName,buf);		/* check SectionName */
				if(sectionFind == FIND )	state++;		/* if Section is exist,move next step */
			}
			else
			{
				keyFind = KeyNameCheckInt(lpKeyName,buf,&result);	/* return key value as integer */
				if(keyFind == FIND )	break;						/* end */
			}
		}
	}
//	fclose(rfile);										
	return result;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: SectionNameCheck
;	Summary of Capability	: search SectionName
;	Iinput			: search SectionName,search string
;	Return			: FIND,NOFIND
;	Created			: 2003.07.31
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int SectionNameCheck(LPCTSTR lpAppName, char *buf)
{
	int		res;
	int		i;
	int		count;
	int		f_char;
	int		lp_char;
	char	*SectionNameEnd;

	res = NOFIND;
	if(*buf == '[' )
	{
		SectionNameEnd = strchr(buf,']');
		if(SectionNameEnd != NULL)                            	/* SectionName? */
		{
			*SectionNameEnd = NULL_C;				/* convert  ']' to NULL */

			for(i=1,count=0;i<BUF_SIZE; i++,count++)
			{
				f_char  = tolower(*(buf+i));		/* convert to lower case */
				lp_char = tolower(*(lpAppName+count));	/* convert to lower case */
				if(f_char != lp_char)		break;	  
				else if(*(buf+i)== NULL_C)	res = FIND; 
			}
		}
	}
	return res;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: KeyNameCheckInt
;	Summary of Capability	:search keyName,and return Key as integer
;	Iinput			:searcing KeyName,searcing string,pinter of storaging interger
;	Return		        : key value(if not find,return default value)
;	Created			: 2003.07.31
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int KeyNameCheckInt(LPCTSTR lpKeyName, char *buf, int *result)
{
	int		res;
	int		i;
	int		f_char;
	int		lp_char;
	char	*keyNameEnd;

	res= NOFIND;
	if(*buf != '[')							       /* check first character */
	{
		keyNameEnd = strchr(buf,'=');
		if(keyNameEnd != NULL)
		{
			*keyNameEnd = NULL_C;					/* convert  '=' to NULL */
			for(i=0;i<BUF_SIZE; i++)
			{
				f_char  = tolower(*(buf+i));		/* convert to lower case */
				lp_char = tolower(*(lpKeyName+i));	/* convert to lower case */
				if(f_char != lp_char)
				{
					break;
				}
				else if(*(buf+i)== NULL_C)
				{
					*result = atoi(keyNameEnd+1);		/* convert Key value to integer */
					res = FIND;
				}
			}
		}
	}
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: ReadInitFileString
;	Summary of Capability	: search keyName,and return Key as string
;	Iinput			: searcing SectionName,searcing lpKeyName,default value,
;				: buffer of storaging retunrns,size of buffer,searching file name
;	Return			: length of storaging string
;	Created			: 2003.07.31
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int ReadInitFileString( LPCTSTR lpAppName,			/*  points to section name */
						LPCTSTR lpKeyName,			/*  points to key name */
						LPCTSTR lpDefault,			/*  points to default string */
						LPTSTR  lpReturnedString,	/*  points to destination buffer */
						int nSize,					/*  size of destination buffer */
						LPCTSTR lpFileName)			/*  points to initialization filename */
{
	int		result;
	int		count;
	int		sectionFind;
	int		checkEnd;
	FILE	*rfile;
	char	buf[BUF_SIZE];
	int		state;

	state = 0;
	count = 0;						/* If lpAppName and lpKeyName are NULL,use this */
	strcpy(lpReturnedString,lpDefault);			/* set default value to buffer */
	result = strlen(lpDefault);			        /* set default length to return value */
	if(NULL != (rfile = fopen(lpFileName, "r")))		/* read file open */
	{
		while(1)
		{
			if(feof(rfile))
			{
				break;
			}
			else
			{
				if(NULL == fgets(buf,BUF_SIZE,rfile))	/* read one line */
				{
					*buf=NULL_C;
				}
				if(lpAppName == NULL_C)			     /* if lpAppName is NULL */
				{
					checkEnd = AllSectionName(lpReturnedString, nSize, buf,&count);	/* return list of SectionName */
					if(feof(rfile) || checkEnd == END)					/* end  */
					{
						*(lpReturnedString + count) = NULL_C;			/* last 2 characeters are NULL */
						if(count == 0)
							*(lpReturnedString + count+1) = NULL_C;
						else
							count--;				     /* don't count last NULL */
						result = count;
						break;
					}
				}
				else if(state == SECTION_CHECK)
				{
					sectionFind = SectionNameCheck(lpAppName,buf);		/* check SectionName */
					if(sectionFind == FIND )	state++;		/* if Section is found, move next step*/
				}
				else
				{
					if(lpKeyName == NULL_C)
					{
						checkEnd = AllKeyName(lpReturnedString, nSize, buf,&count);	/* return KeyName list */
						if(feof(rfile) || checkEnd == END)				/* End */
						{
							*(lpReturnedString + count) = NULL_C;			/* last 2 characeters are NULL */
							if(count == 0)
								*(lpReturnedString + count + 1) = NULL_C;
							else
								count--;										/* not count last NULL */
							result = count;
							break;
						}
					}
					else
					{
						checkEnd = KeyNameCheckString(lpKeyName,buf);		  /* return key value as string */
						if(checkEnd == END )
						{
							GetKeyValueString(lpReturnedString, nSize, buf,&result);
							break;
						}
					}
				}
			}
		}
//		fclose(rfile);										/
	}

	return result;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: KeyNameCheckString
;	Summary of Capability	: Check KeyName
;	Iinput			: searching KeyName,searching string
;				: size of buffer,searching file pointer
;	Return			: length of storaging string
;	Created			: 2003.07.31
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int KeyNameCheckString(LPCTSTR lpKeyName, char *buf)
{
	int		res;
	int		i;
	int		f_char;
	int		lp_char;
	char	*keyNameEnd;

	res= NOEND;
	if(*buf != '[')							   /* check first character */
	{
		keyNameEnd = strchr(buf,'=');
		if(keyNameEnd != NULL)
		{
			*keyNameEnd = NULL_C;					/* convert  '=' to NULL */
			for(i=0;i<BUF_SIZE; i++)
			{
				f_char  = tolower(*(buf+i));		/* convert to lower case */
				lp_char = tolower(*(lpKeyName+i));	/* convert to lower case */
				if(f_char != lp_char)	break;
				else if(*(buf+i)== NULL_C)	res = END;
			}
			*keyNameEnd = '=';						/*  restore */
		}
	}
	return res;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: GetkeyValueString
;	Summary of Capability	: get key value as string
;	Iinput			: buffer of storaging result,result size,strings included KeyName,Pointer of number of storaging characters
;	Return			: None
;	Created			: 2003.08.04
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
void GetKeyValueString(LPTSTR lpReturnedString, int nSize,char *buf,int *result)
{
	char	*keyValueStart;
	int		length;

	keyValueStart = strchr(buf,'=')+1;
	if((length = strlen(keyValueStart)) > (WORD)nSize)	/* Fixed strins size */
		*(keyValueStart+nSize-1) = NULL_C;
	if( *(keyValueStart+length-1)== LF && length != 0)
		*(keyValueStart+length-1) = NULL_C;				/* convert LF to NULL */
	strcpy(lpReturnedString,(keyValueStart));			/* storage data */
	*result = strlen(lpReturnedString);				/* count storaging string */
	return;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: AllSection
;	Summary of Capability	: return list of SectionName
;	Iinput			: buffer of storaging result,size of buffer,pointer of searching file,pointer of number of storaging characters
;	Return			: Normal:0,over size:1)
;	Created			: 2003.07.31
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int AllSectionName(LPTSTR lpReturnedString, int nSize,char *buf,int *count)
{
	int		res;
	char	*SectionNameEnd;
	char	*movePoint;

	res = 0;
	if(*buf == '[' )
	{
		SectionNameEnd = strchr(buf,']');
		if(SectionNameEnd != NULL)
		{
			/* SectionName */
			*SectionNameEnd = NULL_C;			/*  convert ']' to NULL */
			if((*count) + strlen(buf+1) > (WORD)(nSize-2))	/* Last two character are NULL.So,nSize-2 */
			{
				*(buf+1+(nSize-2)-(*count)) = NULL_C;	/*Add NULL */
				res = 1;
			}
			movePoint =(lpReturnedString+*count);		/* Start point of straging */
			strcpy(movePoint,buf+1);			/* straging data */
			*count += strlen(buf+1)+1;		        /* update input characters */
		}
	}
	return res;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: AllKey
;	Summary of Capability	: return list of keyname
;	Iinput			: buffer of storaging result,size of buffer,pointer of searching file,pointer of number of storaging characters
;	Return			: length of storaging characters
;	Created			: 2003.07.31
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int AllKeyName(LPTSTR lpReturnedString, int nSize,char *buf,int *count)
{
	int		res;
	char	*KeyNameEnd;
	char	*movePoint;

	res = NOEND;
	if(*buf == '[' )      /* End */
	{
		res = END;
	}
	else
	{
		KeyNameEnd = strchr(buf,'=');
		if(KeyNameEnd != NULL)
		{
			/* KeyName */
			*KeyNameEnd = NULL_C;				/*  convert '=' to NULL */
			if((*count) + strlen(buf) > (WORD)(nSize-2))	/* Last two character are NULL.So,nSize-2 */
			{
				*(buf+(nSize-2)-(*count)) = NULL_C;	/* Add NULLL */
				res = END;
			}
			movePoint =(lpReturnedString+*count);			/* Start point of straging */
			strcpy(movePoint,buf);					/* Storage data */
			*count += strlen(buf)+1;				/* update number of input characters */
		}
	}
	return res;
}



/*
;------------------------------------------------------------------------------
;	Module Name	        : ReadModelInfoSize
;	Summary of Capability	: get size of model information from file of setting information
;				: If SectionName and KeyName are exist,get size of key value
;	Iinput			: name of searching section,size of buffer,number of record,searching file name
;	Return			: searching result
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int ReadModelInfoSize(LPCTSTR lpAppName, LPCTSTR lpKeyName, int *size, int *record, LPCTSTR lpFileName)
{
	int		result;
	int		sectionFind;
	int		checkEnd;
	FILE	*rfile;
	char	buf[BUF_SIZE];
	int		state;

	result = FALSE;
	*size = 0;
	*record = 0;
	state = SECTION_CHECK;
	if(!(rfile = fopen(lpFileName, "r")))		/* open read file */
		return result;

	while(1)
	{
		if(feof(rfile))
		{
			break;
		}
		else
		{
			if(NULL ==fgets(buf,BUF_SIZE,rfile))
			{
				break;
			}
			if(state == SECTION_CHECK)
			{
				sectionFind = SectionNameCheck(lpAppName,buf);		/* check SectionName*/
				if(sectionFind == FIND )	state++;		/* Section exist,move next step */
			}
			else
			{
				if(lpKeyName == NULL_C)
					checkEnd = GetModelInfoSize(size,record,buf);	/* return size of model information*/
				else
					checkEnd = GetModelInfoKeyValueSize(lpKeyName,size,buf);	/* return size of key value */

				if(checkEnd == END )	break;
			}
		}
	}

//	fclose(rfile);							  

	if(*size != 0)		result = TRUE;				
	return result;
}



/*
;------------------------------------------------------------------------------
;	Module Name	: GetModelInfoSize
;	Summary of Capability	: rerutn size of model information
;	Iinput			: size of buffer,number of record,searching string
;	Return			: searching result
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetModelInfoSize(int *size,int *record,char *buf)
{
	int		res;
	int		length;

	res = NOEND;
	if(*buf == '[' )							/* check first character*/
	{
		res = END;
	}
	else
	{
		if((length = strlen(buf)) != 0)			                 /* length is not 0 */
		{
			if(*buf != LF)						/* not only LF */
			{
				*size += length;				/* Add length */
				(*record)++;					/* increase number of record */
			}
		}
	}
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: GetModelInfoKeyValueSize
;	Summary of Capability		: return size of key value
;	Iinput			: size of buffer,searching string
;	Return			: searching result
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int GetModelInfoKeyValueSize(LPCTSTR lpKeyName,int *size,char *buf)
{
	int		res;
	int		i;
	int		lp_char;
	int		f_char;
	char	*keyNameEnd;

	res = NOFIND;
	if(*buf == '[' )						/* check first character */
	{
		res = FIND;
	}
	else
	{
		keyNameEnd = strchr(buf,'=');
		if(keyNameEnd != NULL)
		{
			*keyNameEnd = NULL_C;					/*  convert '=' to NULL */
			for(i=0;i<BUF_SIZE; i++)
			{
				f_char  = tolower(*(buf+i));		/* convert to lower case */
				lp_char = tolower(*(lpKeyName+i));	/* convert to lower case */
				if(f_char != lp_char)
				{
					break;
				}
				else if(*(buf+i)== NULL_C)
				{
					/* KeyName */
					*size = strlen(keyNameEnd+1);
					res = FIND;
				}
			}
		}
	}
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: ReadModelInfo
;	Summary of Capability	: get model information from setting information file
;	Iinput			: name of searching section,size of buffer,number of record,searching file name
;	Return			: searching result
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int ReadModelInfo(LPCTSTR lpAppName, LPTSTR lpReturnedString,int nSize, LPCTSTR lpFileName)
{
	int		result;
	int		count;
	int		sectionFind;
	int		checkEnd;
	FILE	*rfile;
	char	buf[BUF_SIZE];
	int		state;

	state = 0;
	count = 0;						 /* If lpAppName and lpKeyNamea are NULL,use this */
	result = FALSE;
	state = 0;
	if(NULL != (rfile = fopen(lpFileName, "r")))		/* Open read file */
	{
		while(1)
		{
			if(feof(rfile))
			{
				break;
			}
			else
			{
				if(NULL ==fgets(buf,BUF_SIZE,rfile))			  /* read one line */
				{
					*(lpReturnedString + count) = NULL_C;		  /* last character is NULL */
					if(count != 0)	count--;			  /* not count last NULL */
					break;
				}
				if(state == SECTION_CHECK)
				{
					sectionFind = SectionNameCheck(lpAppName,buf);	  /* check SectionName */
					if(sectionFind == FIND )	state++;	  /* Section exist,move next step */
				}
				else
				{
					checkEnd = AllReadModelInfo(lpReturnedString, nSize, buf,&count);	/* return KeyName list */
					if(feof(rfile) || checkEnd == END)					/* End */
					{
						*(lpReturnedString + count) = NULL_C;			/* last character is NULL */
						if(count != 0)	count--;			       /* not count last NULL */
						break;
					}
				}
			}
		}
//		fclose(rfile);				
	}
	if(count != 0)	result = TRUE;	
	return result;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: AllReadModelInfo
;	Summary of Capability		: get model information
;	Iinput			: size of buffer,number of record,searching string
;	Return			: searching result
;	Created			: 2003.08.06
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int AllReadModelInfo(LPTSTR lpReturnedString, int nSize, char *buf,int *count)
{
	int		res;
	char	*movePoint;
	int		length;

	res = NOEND;
	if(*buf == '[' )					      /* End */
	{
		res = END;
	}
	else
	{
		if(*buf != LF)					       /* not only LF*/
		{
			if((*count) + strlen(buf) > (WORD)(nSize-1))	/* last character is NULL.so,straging number is nSize-1 */
			{
				*(buf+(nSize-1)-(*count)) = NULL_C;			/* Add NULL */
				res = END;
			}
			movePoint =(lpReturnedString+*count);			/* Start point of straging */
			length = strlen(buf);
			*count += strlen(buf);					/* update number of input characters */
			if(*(buf+length-1)== LF)				/* convert from LF to NULLに */
				*(buf+length-1) = NULL_C;
			strcpy(movePoint,buf);				       /* Straging data */
		}
	}
	return res;
}





/*
/////////////////////////////////////////////////
////////       get_Suport_inf関数        ////////
/////////////////////////////////////////////////
*/

/*
;------------------------------------------------------------------------------
;	Module Name	: get_scanmode_string
;	Summary of Capability	: get label of scanmode
;	Iinput			: scanmode structure,poitner of array for storage label.
;	Return			: number of support item.
;	Created			: 2003.08.18
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int get_scanmode_string(SCANMODELIST scanMode, const char **scanModeList)
{
	int i;
	int count=0;

	for(i=0;i<COLOR_TYPE_COUNT+1;i++)
	{
		scanModeList[i] = NULL;					/* Initialize array */
	}

	if(scanMode.bit.bBlackWhite == TRUE)				/*  support Black&White? */
	{
		scanModeList[count] = bwString;
		count++;
	}
	if(scanMode.bit.bErrorDiffusion == TRUE)			/* support Gray(ErrorDiffusion)? */
	{
		scanModeList[count] = errDiffusionString;
		count++;
	}
	if(scanMode.bit.bTrueGray == TRUE)					/* support TrueGray? */
	{
		scanModeList[count] = tGrayString;
		count++;
	}
	if(scanMode.bit.b24BitColor == TRUE)				/* support 24bitColor? */
	{
		scanModeList[count] = ColorString;
		count++;
	}
	if(scanMode.bit.b24BitNoCMatch == TRUE)		    	/* support 24bitColorFast? */
	{
		scanModeList[count] = ColorFastString;
		count++;
	}
	return count;


}

/*
;------------------------------------------------------------------------------
;	Module Name	: get_reso_int
;	Summary of Capability	: get label of resolution
;	Iinput			: structure of resolution,poitner of array for storage label
;	Return			: number of support item
;	Created			: 2003.08.18
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/
int get_reso_int(RESOLIST reso, int *resoList)
{
	int i;
	int count;

	for(i=0;i<RESO_COUNT+1;i++)
	{
		resoList[i] = 0;					/* Initialize array */
	}

	count=1;
	if(reso.bit.bDpi100x100 == TRUE)				/* support Dpi100x100 ? */
	{
		resoList[count] = DPI100x100;
		count++;
	}
	if(reso.bit.bDpi150x150 == TRUE)				/* support Dpi150x150 ? */
	{
		resoList[count] = DPI150x150;
		count++;
	}
	if(reso.bit.bDpi200x200 == TRUE)				/* support Dpi200x200? */
	{
		resoList[count] = DPI200x200;
		count++;
	}
	if(reso.bit.bDpi300x300 == TRUE)				/*support Dpi300x300? */
	{
		resoList[count] = DPI300x300;
		count++;
	}
	if(reso.bit.bDpi400x400 == TRUE)				/*support Dpi400x400? */
	{
		resoList[count] = DPI400x400;
		count++;
	}
	if(reso.bit.bDpi600x600 == TRUE)				/*support Dpi600x600? */
	{
		resoList[count] = DPI600x600;
		count++;
	}
	if(reso.bit.bDpi1200x1200 == TRUE)				/*support Dpi1200x1200? */
	{
		resoList[count] = DPI1200x1200;
		count++;
	}
	if(reso.bit.bDpi2400x2400 == TRUE)				/*support Dpi2400x2400? */
	{
		resoList[count] = DPI2400x2400;
		count++;
	}
	if(reso.bit.bDpi4800x4800 == TRUE)				/*support Dpi4800x4800? */
	{
		resoList[count] = DPI4800x4800;
		count++;
	}
	if(reso.bit.bDpi9600x9600 == TRUE)				/*support Dpi9600x9600? */
	{
		resoList[count] = DPI9600x9600;
		count++;
	}
	resoList[0] = count-1;	/* set item number to top */
	
	return count;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: get_scansrc_string
;	Summary of Capability	: get ScanSrc structure string
;	Iinput			: ScanSrc structure,poitner of array for storage label
;	Return			: number of support item
;	Created			: 2003.08.18
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2006/02/28      kazzy   Add Duplex
;------------------------------------------------------------------------------
*/
int get_scansrc_string(SCANSRCLIST scanSrc, const char **scanSrcList)
{
	int i;
	int count=0;

	for(i=0;i<SCAN_SRC_COUNT+1;i++)
	{
		scanSrcList[i] = NULL;				/* Initialize aray */
	}

	if(scanSrc.bit.FB == TRUE)						/* support FB? */
	{
		scanSrcList[count] = fbString;
		count++;
	}
	if(scanSrc.bit.ADF == TRUE)						/* support ADF? */
	{
		scanSrcList[count] = adfString;
		count++;
	}
	//06/02/28
	if(scanSrc.bit.ADF_DUP == TRUE)						/* support ADF? */
	{
		scanSrcList[count] = adfdupString;
		count++;
	}

	scanSrcList[count] = (void *)0;
	return count;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: get_scanmode_id
;	Summary of Capability	: get scanmode ID number from string
;	input			: ScanMode 文字列
;	Return			: ScanModeID
;	Created			: 2003.08.19
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int get_scanmode_id(const char *scanmode)
{
	int res;

	if( 0 ==strcmp(scanmode,bwString))				/* Black&White? */
		res = COLOR_BW;
	else if( 0 == strcmp(scanmode, errDiffusionString))		/* Gray(ErrorDiffusion)? */
		res = COLOR_ED;
	else if( 0 == strcmp(scanmode, tGrayString))			/* TrueGray? */
		res = COLOR_TG;
	else if( 0 == strcmp(scanmode, ColorString))				/* 24bitColor? */
		res = COLOR_FUL;
	else if( 0 == strcmp(scanmode, ColorFastString))				/* 24bitColorFast? */
		res = COLOR_FUL_NOCM;
	else
		res = -1;		/* Error */

	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: get_reso_id
;	Summary of Capability	: get resolution id number from string
;	Iinput			: string of resolution
;	Return		        : resolution ID
;	Created			: 2003.08.19
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int get_reso_id(const int reso)
{
	int res;

	if( reso == DPI100x100)				/* 100x100? */
		res = RES100X100;
	else if( reso == DPI150x150)		/* 150x150? */
		res = RES150X150;
	else if( reso == DPI200x200)		/* 200x200? */
		res = RES200X200;
	else if( reso == DPI300x300)		/* 300x300? */
		res = RES300X300;
	else if( reso == DPI400x400)		/* 400x400? */
		res = RES400X400;
	else if( reso == DPI600x600)		/* 600x600? */
		res = RES600X600;
	else if( reso == DPI1200x1200)		/* 1200x1200? */
		res = RES1200X1200;
	else if( reso == DPI2400x2400)		/* 2400x2400? */
		res = RES2400X2400;
	else if( reso == DPI4800x4800)		/* 4800x4800? */
		res = RES4800X4800;
	else if( reso == DPI9600x9600)		/* 9600x9600? */
		res = RES9600X9600;
	else
		res = -1;			   /* Error */

	return res;
}


/*
;------------------------------------------------------------------------------
;	Module Name	: get_scanmode_id
;	Summary of Capability	: get scanmode id from string
;	Iinput			: string of ScanSrc
;	Return			: ScanSrcID
;	Created			: 2003.08.19
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2006/02/28      kazzy   Add Duplex
;------------------------------------------------------------------------------
*/

int get_scansrc_id(const char *scanSrc)
{
	int res;

	if( 0 ==strcmp(scanSrc,fbString))				/* FlatBed? */
		res = SCANSRC_FB;
	else if( 0 == strcmp(scanSrc, adfString))		/* Auto Document Feeder? */
		res = SCANSRC_ADF;
	else if( 0 == strcmp(scanSrc, adfdupString))		/* Duplex? */
		res = SCANSRC_ADF_DUP;
	else
		res = -1;	       /* error */

	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: get_scanmode_listcnt
;	Summary of Capability	: get number of ScanMode string list
;	Iinput			: pointer of ScanMode strings list,ScanModeID
;	Return			: number of ScanMode string list
;	Created			: 2003.08.20
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int get_scanmode_listcnt(const char **scanModeList, int scanModeID)
{
	char IDString[MAX_STRING];
	int  count = 0;
	int  res;

	res = ScanModeIDString(scanModeID, IDString);
	if(res == TRUE)
	{
		while(1)
		{
			if(!scanModeList[count])
			{
				count = -1;
				break;
			}
			else if( 0 == strcmp(scanModeList[count],IDString))
			{
				break;
			}
			count++;
		}
	}
	else
	{
		count = -1;
	}
	return count;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: ScanModeIDString
;	Summary of Capability	: get string of ScanModeID
;	Iinput			: ScanModeID,pointer of string
;	Return			: success:TRUE , fail:FALSE
;	Created			: 2003.08.20
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int ScanModeIDString(int scanModeID, char *IDString)
{
	int res;

	res = TRUE;
	switch(scanModeID)
	{
		case	COLOR_BW:
			strcpy(IDString, bwString);
			break;
		case	COLOR_ED:
			strcpy(IDString, errDiffusionString);
			break;
		case	COLOR_TG:
			strcpy(IDString, tGrayString);
			break;
		case	COLOR_FUL:
			strcpy(IDString, ColorString);
			break;
		case	COLOR_FUL_NOCM:
			strcpy(IDString, ColorFastString);
			break;
		default:
			res = FALSE;
	}
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: get_reso_listcnt
;	Summary of Capability	:get number of array of reslution list
;	Iinput			: poitner of resolution list,seaching resoID
;	Return			: number of reslution list
;	Created			: 2003.08.20
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int get_reso_listcnt(int *resoList, int resoID)
{
	int  IDInt;
	int  count = 1;
	int  res;

	res = ResoIDInt(resoID, &IDInt);
	if(res == TRUE)
	{
		while( count <= resoList[0])
		{
			if(!resoList[count])
			{
				count = -1;
				break;
			}
			else if( resoList[count] == IDInt)
			{
				break;
			}
			count++;
		}
	}
	else
	{
		count = -1;
	}
	return count;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: ResoIDInt
;	Summary of Capability	: get number of resoID
;	Iinput			: resoID, storaging point
;	Return			: success:TRUE fail:FALSE
;	Created			: 2003.08.20
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int ResoIDInt(int resoID, int *IDInt)
{
	int res;

	res = TRUE;
	switch(resoID)
	{
		case	RES100X100:
			*IDInt = DPI100x100;
			break;
		case	RES150X150:
			*IDInt = DPI150x150;
			break;
		case	RES200X200:
			*IDInt = DPI200x200;
			break;
		case	RES300X300:
			*IDInt = DPI300x300;
			break;
		case	RES400X400:
			*IDInt = DPI400x400;
			break;
		case	RES600X600:
			*IDInt = DPI600x600;
			break;
		case	RES1200X1200:
			*IDInt = DPI1200x1200;
			break;
		case	RES2400X2400:
			*IDInt = DPI2400x2400;
			break;
		case	RES4800X4800:
			*IDInt = DPI4800x4800;
			break;
		case	RES9600X9600:
			*IDInt = DPI9600x9600;
			break;
		default:
			res = FALSE;
	}
	return res;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: get_scansrc_listcnt
;	Summary of Capability	: get number of ScanSrc string list.
;	Iinput			: pointer of ScanSrc string list,searching scanSrcID
;	Return			: number of ScanSrc list
;	Created			: 2003.08.20
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;------------------------------------------------------------------------------
*/

int get_scansrc_listcnt(const char **scanSrcList, int scanSrcID)
{
	char IDString[MAX_STRING];
	int  count = 0;
	int  res;

	res = ScanSrcIDString(scanSrcID, IDString);
	if(res == TRUE)
	{
		while(1)
		{
			if(!scanSrcList[count])
			{
				count = -1;
				break;
			}
			else if( 0 == strcmp(scanSrcList[count],IDString))
			{
				break;
			}
			count++;
		}
	}
	else
	{
		count = -1;
	}
	return count;
}

/*
;------------------------------------------------------------------------------
;	Module Name	: ScanSrcIDString
;	Summary of Capability	: get string of scanSrcID
;	Iinput			: scanSrcID,pointer of string
;	Return			: success:TRUE , fail:FALSE
;	Created			: 2003.08.20
;	Notes		:
;------------------------------------------------------------------------------
;	History
;	Date		Fixer	Comments
;       2006/02/28      kazzy   Add Duplex
;------------------------------------------------------------------------------
*/

int ScanSrcIDString(int scanSrcID, char *IDString)
{
	int res;

	res = TRUE;
	switch(scanSrcID)
	{
		case	SCANSRC_FB:
			strcpy(IDString, fbString);
			break;
		case	SCANSRC_ADF:
			strcpy(IDString, adfString);
			break;
		case    SCANSRC_ADF_DUP:
			strcpy(IDString, adfdupString);
			break;
		default:
			res = FALSE;
	}
	return res;
}
