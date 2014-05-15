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
//	Source filename: brother_devinfo.c
//
//		Copyright(c) 2005-2009 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:Module of getting device information
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <pthread.h>

#include "brother.h"

#include "brother_cmatch.h"
#include "brother_mfccmd.h"
#include "brother_devaccs.h"
#include "brother_misc.h"
#include "brother_log.h"

#include "brother_devinfo.h"

#define QUERYTIMEOUY 5000	// 5000ms
//-----------------------------------------------------------------------------
//
//	Function name:	ExecQueryThread
//
//
//	Abstract:Execution of thread that get device information
//
//	Parameters:
//		lpQueryProc: Pointer to thread function
//
//		lpQueryPara:Parameter for thread function
//
//
//	Return values:
//		TRUE  = Normal End
//		FALSE = Fail to get device information
//
//-----------------------------------------------------------------------------
//
BOOL
ExecQueryThread( Brother_Scanner *this, void *lpQueryProc)
{
	pthread_t   tid;
        
        /*  Create receving thread */
	if (pthread_create(&tid,NULL, lpQueryProc,(void*)this))
		return TRUE;

        /*  Wait end of receving thread.  */	
	if (pthread_join(tid,NULL)) 
		return TRUE;

	return TRUE;
}
// #define FOR_THREAD
//-----------------------------------------------------------------------------
//
//	Function name:	QueryDeviceInfo
//
//
//	Abstract:
//		Get informations of device's scanner and video capability.
//
//
//	Parameters:
//		None
//
//
//	Return values:
//		TRUE  = Success to get device information（Storage information from device）
//		FALSE = Fail to get device information（Storage default information
//
//-----------------------------------------------------------------------------
//	QueryDeviceInfo
BOOL
QueryDeviceInfo( Brother_Scanner *this )
{
	BOOL  bResult = FALSE;


	WriteLog( "Query device info" );

	if( this->mfcModelInfo.bQcmdEnable ){
		//
		// Execute Q-command
		//
#ifdef FOR_THREAD
		bResult = ExecQueryThread( this, QCommandProc );
#else
		bResult = QCommandProc( this );
#endif
	}

	if( bResult == FALSE )
		return bResult;

	if( this->mfcDeviceInfo.nColorType.val == 0 ){
		if( this->mfcModelInfo.bColorModel ){
			//
			//
			//
			this->mfcDeviceInfo.nColorType.val = MFCDEVINFCOLOR;
		}else{
			//
			//
			//
			//
			//
			//
			//
			this->mfcDeviceInfo.nColorType.val = MFCDEVINFMONO;
		}
	}

	return bResult;
}

//-----------------------------------------------------------------------------
//
//	Function name:	QueryScannerInfo
//
//
//	Abstract:Get device scanner information about resolution that selected by user.
//
//	Parameters:
//		nDevPort:Identifier of device
//
//		lpTwDev:Pointer to setting information of DataSource
//
//	Return values:
//		TRUE  = Success to get device information（Storage information from device）
//		FALSE = Fail to get device information（Storage default information
//
//-----------------------------------------------------------------------------
//
BOOL
QueryScannerInfo( Brother_Scanner *this )
{
	BOOL   bResult = FALSE;


	WriteLog( "Query scanner info" );

	// If device's PC-scan protcol is later than 2000 ver. and
        // has capability to inquire resolution, execute I-command
	//
#ifdef FOR_THREAD
	bResult = ExecQueryThread( this, QueryScanInfoProc);
#else
	bResult = QueryScanInfoProc( this );
#endif

	if( bResult == FALSE ){
	  WriteLog("I command Fault");
		//If I-commnad is filed or no-support,
                //use defult value of scanner information.

		CnvResoNoToDeviceResoValue( this, this->devScanInfo.wResoType, this->devScanInfo.wColorType );
		SetDefaultScannerInfo( this );
	}
	return bResult;
}
//-----------------------------------------------------------------------------
//
//	Function name:	SetDefaultScannerInfo
//
//
//	Abstract:Set fixed value of scanner parameter
//
//	Parameters:None
//
//	Return values:None
//
//	Note:
//         Some don't support I-command.
//         So,these use fixed value(default value).
//         When I-command fail,use default value.
//
//-----------------------------------------------------------------------------
//
void
SetDefaultScannerInfo( Brother_Scanner *this )
{
	WriteLog( "Set default scanner info" );
	//
	// Set scan sourece to ADF
	this->devScanInfo.wScanSource = MFCSCANSRC_ADF;

	//
	// Max width of scan. (in unit of 0.1 mm) 
	this->devScanInfo.dwMaxScanWidth = MFCSCANMAXWIDTH;

	//
	// Max dot numbers of scan.
	switch( this->devScanInfo.DeviceScan.wResoX ){
		case 100:
			this->devScanInfo.dwMaxScanPixels = MFCSCAN200MAXPIXEL / 2;
			break;

		case 150:
			this->devScanInfo.dwMaxScanPixels = MFCSCAN300MAXPIXEL / 2;
			break;

		case 200:
			this->devScanInfo.dwMaxScanPixels = MFCSCAN200MAXPIXEL;
			break;

		case 300:
			this->devScanInfo.dwMaxScanPixels = MFCSCAN300MAXPIXEL;
			break;
	}

	//
	// Max length of scan by FlatBed.(unit of 0.1mm)
	this->devScanInfo.dwMaxScanHeight = MFCSCANFBHEIGHT;

	//
	// Max raster number of scan by FlatBed.
	switch( this->devScanInfo.DeviceScan.wResoY ){
		case 100:
			this->devScanInfo.dwMaxScanRaster = MFCSCAN400FBRASTER / 4;
			break;

		case 200:
			this->devScanInfo.dwMaxScanRaster = MFCSCAN400FBRASTER / 2;
			break;

		case 400:
			this->devScanInfo.dwMaxScanRaster = MFCSCAN400FBRASTER;
			break;

		case 150:
			this->devScanInfo.dwMaxScanRaster = MFCSCAN600FBRASTER / 4;
			break;

		case 300:
			this->devScanInfo.dwMaxScanRaster = MFCSCAN600FBRASTER / 2;
			break;

		case 600:
			this->devScanInfo.dwMaxScanRaster = MFCSCAN600FBRASTER;
			break;
	}
}
//-----------------------------------------------------------------------------
//
//	Function name:	QCommandProc
//
//
//	Abstract:Usage of Q-command(function for thread)
//
//	Parameters:
//		lpParameter:Not use
//
//	Return values:
//		TRUE  = Normal end
//		FALSE = Fail to read device information
//
//-----------------------------------------------------------------------------
//	QCommandProc
DWORD
QCommandProc( void *lpParameter )
{
	Brother_Scanner *this;
	BOOL bResult = FALSE;
	DWORD  dwQcmdTimeOut;
	
	int nReadSize;
	char *pReadBuf;

	this=(Brother_Scanner *)lpParameter;

	WriteLog( "Start Q-command proc" );

	//
	// Make Q-command
	//
	//WriteDeviceCommand( this->hScanner, MFCMD_QUERYDEVINFO, strlen( MFCMD_QUERYDEVINFO ) );
	//2005/11/11 Add SeriesNnumber information for L4CFB
	WriteDeviceCommand( this->hScanner, 
			    (LPBYTE)MFCMD_QUERYDEVINFO, // M-LNX-103
			    strlen( MFCMD_QUERYDEVINFO ),
			    this->modelInf.seriesNo  );
	
	// Set value of timeout
	//
	dwQcmdTimeOut = QUERYTIMEOUY ;

	nReadSize = sizeof( MFCDEVICEHEAD ) + sizeof( MFCDEVICEINFO );
	pReadBuf = MALLOC( nReadSize + 0x100);
	if (!pReadBuf)
		return FALSE;

	//if (ReadNonFixedData( this->hScanner, pReadBuf, nReadSize + 0x100, dwQcmdTimeOut )){
	//2005/11/11 Add SeriesNnumber information for L4CFB
	if (ReadNonFixedData( this->hScanner, 
			      (LPBYTE)pReadBuf,  //M-LNX-104 
			      nReadSize + 0x100, 
			      dwQcmdTimeOut,
			      this->modelInf.seriesNo )){
		this->mfcDevInfoHeader.wDeviceInfoID = (*(WORD *)pReadBuf);
		this->mfcDevInfoHeader.nInfoSize = *(BYTE *)(pReadBuf+2);
		this->mfcDevInfoHeader.nProtcolType = *(BYTE *)(pReadBuf+3);

		memset( &this->mfcDeviceInfo, 0, sizeof( MFCDEVICEINFO ) );
		this->mfcDeviceInfo.nColorType.val = *(BYTE *)(pReadBuf+5); 
		this->mfcDeviceInfo.nHardwareVersion = *(BYTE *)(pReadBuf+13); 
		this->mfcDeviceInfo.nMainScanDpi = *(BYTE *)(pReadBuf+14); 
		this->mfcDeviceInfo.nPaperSizeMax = *(BYTE *)(pReadBuf+15); 

#if 1 //support A3  M-LNX-98
		this->modelConfig.nPaperSizeMax 
		  = this->mfcDeviceInfo.nPaperSizeMax;
		if(this->mfcDevInfoHeader.nProtcolType >= 5){
		  this->mfcDeviceInfo.nXposFB  = *(BYTE *)(pReadBuf+16); 
		  this->mfcDeviceInfo.nYposFB  = *(BYTE *)(pReadBuf+17); 
		  this->mfcDeviceInfo.nXposADF = *(BYTE *)(pReadBuf+18); 
		  this->mfcDeviceInfo.nYposADF = *(BYTE *)(pReadBuf+19); 

		  this->modelConfig.nXposFB  = *(BYTE *)(pReadBuf+16); 
		  this->modelConfig.nYposFB  = *(BYTE *)(pReadBuf+17); 
		  this->modelConfig.nXposADF = *(BYTE *)(pReadBuf+18); 
		  this->modelConfig.nYposADF = *(BYTE *)(pReadBuf+19); 

		}
		else{
		  this->mfcDeviceInfo.nXposFB  = 0; 
		  this->mfcDeviceInfo.nYposFB  = 0; 
		  this->mfcDeviceInfo.nXposADF = 0; 
		  this->mfcDeviceInfo.nYposADF = 0;

		  this->modelConfig.nXposFB  = 0;
		  this->modelConfig.nYposFB  = 0;
		  this->modelConfig.nXposADF = 0;
		  this->modelConfig.nYposADF = 0; 
		}

		WriteLog("MaxPaperSize = %d\n",
			 this->mfcDeviceInfo.nPaperSizeMax);
		WriteLog("Align Information %d %d %d %d\n",
		  this->modelConfig.nXposFB,
		  this->modelConfig.nYposFB,
		  this->modelConfig.nXposADF,
			 this->modelConfig.nYposADF);
#endif //support A3  M-LNX-98


		bResult=TRUE;
	}	
	else {
		//Fail to read to return value of Q-command.
		//
		WriteLog( "SENDQ : read err@timeout " );
	}

	FREE( pReadBuf );

	return bResult;
}


//-----------------------------------------------------------------------------
//
//	Function name:	QueryScanInfoProc
//
//
//	Abstract:Usage of I-command(functiuon for thread)
//               Get device scanner information about resolution that selected by user.
//
//	Parameters:
//		lpParameter:Identifier of device
//
//	Return values:
//		TRUE  = Normal end
//		FALSE = Fail to read to scan information
//
//-----------------------------------------------------------------------------
//
DWORD
QueryScanInfoProc(
	void *lpParameter )
{
	Brother_Scanner *this;
	char  szCmdStr[ MFCMAXCMDLENGTH ];
	int   CmdLength;
	BOOL  bResult = FALSE;
	DWORD  dwQcmdTimeOut;
	WORD   wReadSize;
	int nReadSize, nRealReadSize;
	char *pReadBuf;
	char *pReadBuf_top;   //M-LNX-124

	this=(Brother_Scanner *)lpParameter;

	WriteLog( "Start I-command proc" );

	this = (Brother_Scanner *)lpParameter;

	// Make I-command
	//
	CmdLength = MakeupScanQueryCmd( this, szCmdStr );
	WriteLogScanCmd( "Write", szCmdStr );
	//WriteDeviceCommand( this->hScanner, szCmdStr, CmdLength );
	//2005/11/11 Add SeriesNnumber information for L4CFB
	WriteDeviceCommand( this->hScanner, 
			    (LPBYTE)szCmdStr,  //M-LNX-104 
			    CmdLength, 
			    this->modelInf.seriesNo );

	//Set value of timeout
	//
	dwQcmdTimeOut = QUERYTIMEOUY;

	nReadSize = 100;
	pReadBuf = MALLOC( nReadSize + 0x100 );
	pReadBuf_top = pReadBuf;   //M-LNX-124

	if (!pReadBuf)
		return FALSE;

	//2005/11/11 Add SeriesNnumber information for L4CFB
	//nRealReadSize = ReadNonFixedData( this->hScanner, pReadBuf, nReadSize + 0x100, dwQcmdTimeOut );
	nRealReadSize = ReadNonFixedData( this->hScanner, 
					  (LPBYTE)pReadBuf, //M-LNX-104 
					  nReadSize + 0x100, 
					  dwQcmdTimeOut, 
					  this->modelInf.seriesNo );
	if ( nRealReadSize < 2) {
		//Fail to read to return of I-command
		//
		WriteLog( "SENDI : read err@timeout [%d]", nRealReadSize );
	}
	else {
#if       BRSANESUFFIX == 2 || BRSANESUFFIX == 3
		LPSTR  lpDataBuff;
		int    timesX,timesY;
 
		// M-LNX-135
		// Garbage charactor follows the valid data. 
		if(nRealReadSize < (nReadSize + 0x100))pReadBuf[nRealReadSize] = 0;

		timesX=1;
		timesY=1;

		//M-LNX-124   measure against the protocol 06 (08 models)
		WriteLog("Q-command :protcol = %d\n"
			 ,this->mfcDevInfoHeader.nProtcolType);
		if(this->mfcDevInfoHeader.nProtcolType >= 6){
		  pReadBuf++;
		}
		//M-LNX-124   measure against the protocol 06 (08 models)

		lpDataBuff = pReadBuf+2; // Move pointer for reginon of size.
		wReadSize = nRealReadSize-2;

		//Read scanner information
		//
		bResult = TRUE;
		//Add Null to end of data to treat as strings.
		*( lpDataBuff + wReadSize ) = '\0';
		WriteLog( "  Response is [%s]", lpDataBuff );
		// Get resolution information
		this->devScanInfo.DeviceScan.wResoX = StrToWord( GetToken( &lpDataBuff ) );
		this->devScanInfo.DeviceScan.wResoY = StrToWord( GetToken( &lpDataBuff ) );

	        if(this->devScanInfo.DeviceScan.wResoX > 600){
		  timesX = this->devScanInfo.DeviceScan.wResoX / 600;
		  this->devScanInfo.DeviceScan.wResoX = 600;
		}		
		if(this->devScanInfo.DeviceScan.wResoY > 600){
		  timesY = this->devScanInfo.DeviceScan.wResoY / 600;
		  this->devScanInfo.DeviceScan.wResoY = 600;
		}
		
	        if(this->devScanInfo.DeviceScan.wResoX > 400 && this->devScanInfo.wColorType == COLOR_TG){
		  timesX = this->devScanInfo.DeviceScan.wResoX / 300;
		  this->devScanInfo.DeviceScan.wResoX = 300;
		  timesY = this->devScanInfo.DeviceScan.wResoY / 300;
		  this->devScanInfo.DeviceScan.wResoY = 300;
		}

                //If RGB converting is executed and color setting is 24bit,don't use 400dpi setting
		//if(this->modelInf.seriesNo >= 10 && this->devScanInfo.DeviceScan.wResoX == 400 && ( this->devScanInfo.wColorType == COLOR_FUL || this->devScanInfo.wColorType == COLOR_FUL_NOCM  )){
		//  timesX = 2;
		//  this->devScanInfo.DeviceScan.wResoX = 200;
		//}
		
		
		if( this->devScanInfo.DeviceScan.wResoX == 0 || this->devScanInfo.DeviceScan.wResoY == 0 ){
			// Resolution setting is wrong
			bResult = FALSE;
		}
		// Get scan source information.
		this->devScanInfo.wScanSource = StrToWord( GetToken( &lpDataBuff ) );
		// Get Max width inforamtion (unit of 0.1mm)
		this->devScanInfo.dwMaxScanWidth  = StrToWord( GetToken( &lpDataBuff ) ) * 10;
		this->devScanInfo.dwMaxScanPixels = ( StrToWord( GetToken( &lpDataBuff ) ) ) / timesX;
		if( this->devScanInfo.dwMaxScanWidth == 0 || this->devScanInfo.dwMaxScanPixels == 0 ){
			// Max width information is wrong
			bResult = FALSE;
		}
		// Get information of Max width with FlatBed(unit of 0.1mm)
		this->devScanInfo.dwMaxScanHeight = StrToWord( GetToken( &lpDataBuff ) ) * 10;
		this->devScanInfo.dwMaxScanRaster = ( StrToWord( GetToken( &lpDataBuff ) ) ) / timesY;
		
		bResult = TRUE;
#elif  BRSANESUFFIX == 1
		LPSTR  lpDataBuff;
 
		lpDataBuff = pReadBuf+2; //
		wReadSize = nRealReadSize-2;
		//
		//
		//
		bResult = TRUE;
		//
		*( lpDataBuff + wReadSize ) = '\0';
		WriteLog( "  Response is [%s]", lpDataBuff );
		//
		//
		this->devScanInfo.DeviceScan.wResoX = StrToWord( GetToken( &lpDataBuff ) );
		this->devScanInfo.DeviceScan.wResoY = StrToWord( GetToken( &lpDataBuff ) );
		if( this->devScanInfo.DeviceScan.wResoX == 0 || this->devScanInfo.DeviceScan.wResoY == 0 ){
			//
			//
			bResult = FALSE;
		}
		//
		//
		this->devScanInfo.wScanSource = StrToWord( GetToken( &lpDataBuff ) );
		//
		//
		this->devScanInfo.dwMaxScanWidth  = StrToWord( GetToken( &lpDataBuff ) ) * 10;
		this->devScanInfo.dwMaxScanPixels = StrToWord( GetToken( &lpDataBuff ) );
		if( this->devScanInfo.dwMaxScanWidth == 0 || this->devScanInfo.dwMaxScanPixels == 0 ){
			//
			//
			bResult = FALSE;
		}
		//
		//
		this->devScanInfo.dwMaxScanHeight = StrToWord( GetToken( &lpDataBuff ) ) * 10;
		this->devScanInfo.dwMaxScanRaster = StrToWord( GetToken( &lpDataBuff ) );
		
		bResult = TRUE;
#else    //BRSANESUFFIX
  force causing compile error
#endif   //BRSANESUFFIX
	}

	//M-LNX-124   FREE( pReadBuf );
	//M-LNX-124   measure against the protocol 06 (08 models)
	FREE( pReadBuf_top );
	//M-LNX-124   measure against the protocol 06 (08 models)
	return bResult;
}


//
// Reasolution table of each device.
//
static RESOLUTION  tblDecScanReso100[] = 
{
	{ 100, 100 },	//  100 x  100 dpi
	{ 150, 150 },	//  150 x  150 dpi
	{ 200, 100 },	//  200 x  100 dpi
	{ 200, 200 },	//  200 x  200 dpi
	{ 200, 400 },	//  200 x  400 dpi
	{ 300, 300 },	//  300 x  300 dpi
	{ 200, 400 },	//  400 x  400 dpi
	{ 300, 600 },	//  600 x  600 dpi
	{ 200, 400 },	//  800 x  800 dpi
	{ 300, 600 },	// 1200 x 1200 dpi
	{ 300, 600 },	// 2400 x 2400 dpi
	{ 300, 600 },	// 4800 x 4800 dpi
	{ 300, 600 }	// 9600 x 9600 dpi
};

static RESOLUTION  tblDecScanReso300[] = 
{
	{ 200, 100 },	//  100 x  100 dpi
	{ 150, 150 },	//  150 x  150 dpi
	{ 200, 100 },	//  200 x  100 dpi
	{ 200, 200 },	//  200 x  200 dpi
	{ 200, 400 },	//  200 x  400 dpi
	{ 300, 300 },	//  300 x  300 dpi
	{ 200, 400 },	//  400 x  400 dpi
	{ 300, 600 },	//  600 x  600 dpi
	{ 200, 400 },	//  800 x  800 dpi
	{ 300, 600 },	// 1200 x 1200 dpi
	{ 300, 600 },	// 2400 x 2400 dpi
	{ 300, 600 },	// 4800 x 4800 dpi
	{ 300, 600 }	// 9600 x 9600 dpi
};

static RESOLUTION  tblDecScanReso200BW[] = 
{
	{ 200, 100 },	//  100 x  100 dpi
	{ 150, 150 },	//  150 x  150 dpi
	{ 200, 100 },	//  200 x  100 dpi
	{ 200, 200 },	//  200 x  200 dpi
	{ 200, 400 },	//  200 x  400 dpi
	{ 300, 300 },	//  300 x  300 dpi
	{ 200, 400 },	//  400 x  400 dpi
	{ 200, 200 },	//  600 x  600 dpi
	{ 200, 400 },	//  800 x  800 dpi
	{ 200, 400 },	// 1200 x 1200 dpi
	{ 200, 400 },	// 2400 x 2400 dpi
	{ 200, 400 },	// 4800 x 4800 dpi
	{ 200, 400 }	// 9600 x 9600 dpi
};

static RESOLUTION  tblDecScanReso200Gray[] = 
{
	{ 200, 100 },	//  100 x  100 dpi
	{ 150, 150 },	//  150 x  150 dpi
	{ 200, 100 },	//  200 x  100 dpi
	{ 200, 200 },	//  200 x  200 dpi
	{ 200, 400 },	//  200 x  400 dpi
	{ 300, 300 },	//  300 x  300 dpi
	{ 200, 400 },	//  400 x  400 dpi
	{ 200, 400 },	//  600 x  600 dpi
	{ 200, 400 },	//  800 x  800 dpi
	{ 200, 400 },	// 1200 x 1200 dpi
	{ 200, 400 },	// 2400 x 2400 dpi
	{ 200, 400 },	// 4800 x 4800 dpi
	{ 200, 400 }	// 9600 x 9600 dpi
};


//-----------------------------------------------------------------------------
//
//	Function name:	CnvResoNoToDeviceResoValue
//
//
//	Abstract:Get device's resolution information.
//
//	Parameters:
//		nResoNo:Number of resolution type
//
//		nColorType:Number of color type
//
//	Return values:None
//
//-----------------------------------------------------------------------------
//
void
CnvResoNoToDeviceResoValue( Brother_Scanner *this, WORD nResoNo, WORD nColorType )
{
	if( nResoNo > RES9600X9600 ){
		nResoNo = RES9600X9600;
	}
	if( !this->mfcModelInfo.bColorModel && ( this->mfcDeviceInfo.nMainScanDpi == 1 ) ){
		if( nColorType == COLOR_BW || nColorType == COLOR_ED ){
			this->devScanInfo.DeviceScan.wResoX = tblDecScanReso200BW[ nResoNo ].wResoX;
			this->devScanInfo.DeviceScan.wResoY = tblDecScanReso200BW[ nResoNo ].wResoY;
		}else{
			this->devScanInfo.DeviceScan.wResoX = tblDecScanReso200Gray[ nResoNo ].wResoX;
			this->devScanInfo.DeviceScan.wResoY = tblDecScanReso200Gray[ nResoNo ].wResoY;
		}
	}else if( this->mfcDeviceInfo.nMainScanDpi == 2 ){
		this->devScanInfo.DeviceScan.wResoX = tblDecScanReso300[ nResoNo ].wResoX;
		this->devScanInfo.DeviceScan.wResoY = tblDecScanReso300[ nResoNo ].wResoY;

	}else{
		this->devScanInfo.DeviceScan.wResoX = tblDecScanReso100[ nResoNo ].wResoX;
		this->devScanInfo.DeviceScan.wResoY = tblDecScanReso100[ nResoNo ].wResoY;
	}
}


//////// end of brother_devinfo.c ////////
