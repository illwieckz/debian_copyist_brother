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
//	Source filename: brother_scanner.c
//
//		Copyright(c) 1995-2000 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//			the module for scan process
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include <usb.h>

#include "sane/sane.h"

#include "brother_dtype.h"

#include "brother.h"
#include "brother_cmatch.h"
#include "brother_mfccmd.h"
#include "brother_devaccs.h"
#include "brother_log.h"
#include "brother_modelinf.h"
#include "brother_scandec.h"
#include "brother_deccom.h"

#include "brother_scanner.h"


#ifdef NO39_DEBUG
#include <sys/time.h>
#endif

//$$$$$$$$
LPBYTE  lpRxBuff = NULL;
LPBYTE  lpRxTempBuff = NULL;
DWORD   dwRxTempBuffLength = 0;
LPBYTE  lpFwTempBuff = NULL;
DWORD	dwFwTempBuffMaxSize= 0;
int     FwTempBuffLength = 0;
BOOL    bTxScanCmd = FALSE;
DWORD	dwRxBuffMaxSize= 0;

LONG lRealY = 0;

HANDLE	hGray;			//Gray table from brgray.bin

//$$$$$$$$

//
// the valiables used for the expansion of the scaned data and exchange the resolution
//
SCANDEC_OPEN   ImageProcInfo;
SCANDEC_WRITE  ImgLineProcInfo;
DWORD  dwImageBuffSize;
DWORD  dwWriteImageSize;
int    nWriteLineCount;


//
// the name of ScenDec dinamic link library 
//
#if       BRSANESUFFIX == 2
static char  szScanDecDl[] = "libbrscandec2.so";
#elif  BRSANESUFFIX == 1
static char  szScanDecDl[] = "libbrscandec.so";
#else
Not support (force causing compile error)
#endif   //BRSANESUFFIX


BOOL	bReceiveEndFlg;


// Static valiable for debug process 
int nPageScanCnt;
int nReadCnt;


DWORD dwFWImageSize;
DWORD dwFWImageLine;
int nFwLenTotal;

//-----------------------------------------------------------------------------
//
//	Function name:	LoadScanDecDll
//
//
//	Abstract:
//		Load the ScanDec library and get the procedure addresses 
//
//
//	Parameters:
//		None
//
//
//	Return values:
//		TRUE  = SUCCESS
//		FALSE = cannot find the ScenDec library (error occured)
//
//-----------------------------------------------------------------------------
//	LoadColorMatchDll (The parts of DllMain in the Windows version)
BOOL
LoadScanDecDll( Brother_Scanner *this )
{
	BOOL  bResult = TRUE;


	this->scanDec.hScanDec = dlopen ( szScanDecDl, RTLD_LAZY );

	if( this->scanDec.hScanDec != NULL ){
		//
		// get the procedure addresses  of scanDec
		//
		this->scanDec.lpfnScanDecOpen      = dlsym ( this->scanDec.hScanDec, "ScanDecOpen" );
		this->scanDec.lpfnScanDecSetTbl    = dlsym ( this->scanDec.hScanDec, "ScanDecSetTblHandle" );
		this->scanDec.lpfnScanDecPageStart = dlsym ( this->scanDec.hScanDec, "ScanDecPageStart" );
		this->scanDec.lpfnScanDecWrite     = dlsym ( this->scanDec.hScanDec, "ScanDecWrite" );
		this->scanDec.lpfnScanDecPageEnd   = dlsym ( this->scanDec.hScanDec, "ScanDecPageEnd" );
		this->scanDec.lpfnScanDecClose     = dlsym ( this->scanDec.hScanDec, "ScanDecClose" );

		if(  this->scanDec.lpfnScanDecOpen == NULL || 
			 this->scanDec.lpfnScanDecSetTbl  == NULL || 
			 this->scanDec.lpfnScanDecPageStart  == NULL || 
			 this->scanDec.lpfnScanDecWrite  == NULL || 
			 this->scanDec.lpfnScanDecPageEnd  == NULL || 
			 this->scanDec.lpfnScanDecClose  == NULL )
		{
			// ERROR: library exists but cannot get the procedure address 
			dlclose ( this->scanDec.hScanDec );
			this->scanDec.hScanDec = NULL;
			bResult = FALSE;
		}
	}else{
		this->scanDec.lpfnScanDecOpen      = NULL;
		this->scanDec.lpfnScanDecSetTbl    = NULL;
		this->scanDec.lpfnScanDecPageStart = NULL;
		this->scanDec.lpfnScanDecWrite     = NULL;
		this->scanDec.lpfnScanDecPageEnd   = NULL;
		this->scanDec.lpfnScanDecClose     = NULL;
		bResult = FALSE;
	}
	return bResult;
}


//-----------------------------------------------------------------------------
//
//	Function name:	FreeScanDecDll
//
//
//	Abstract:
//		Free the ScanDec DLL
//
//
//	Parameters:
//	        None
//
//
//	Return values:
//	        None
//
//-----------------------------------------------------------------------------
//
void
FreeScanDecDll( Brother_Scanner *this )
{
	if( this->scanDec.hScanDec != NULL ){
		//
		// Free ColorMatch DLL
		//
		dlclose ( this->scanDec.hScanDec );
		this->scanDec.hScanDec = NULL;
	}
}


/********************************************************************************
 *										*
 *	FUNCTION	ScanStart						*
 *										*
 *	PURPOSE		Start process of the scan 				*
 *				start the resource manager and open the BI-DI	*
 *										*
 *	IN		Brother_Scanner *this					*
 *	OUT		None							*
 *										*
 ********************************************************************************/
BOOL
ScanStart( Brother_Scanner *this )
{
	BOOL  bResult;

	int   rc,errornum;

	WriteLog( "" );
	WriteLog( ">>>>> Start Scanning >>>>>" );
	WriteLog( "nPageCnt = %d bEOF = %d  iProcessEnd = %d\n",
		this->scanState.nPageCnt, this->scanState.bEOF, this->scanState.iProcessEnd);

#if 1
	// debug for MASU
	dwFWImageSize = 0;
	dwFWImageLine = 0;
	nFwLenTotal = 0;
#endif
	this->scanState.nPageCnt++;
	this->scanState.bReadbufEnd=FALSE;
	this->scanState.bEOF=FALSE;	

	if( this->scanState.nPageCnt == 1){
		int nResoLine;

		bTxScanCmd = FALSE;
		lRealY = 0;		

		this->devScanInfo.wResoType  = this->uiSetting.wResoType;
		this->devScanInfo.wColorType = this->uiSetting.wColorType;
		

#ifndef DEBUG_No39


#ifndef  NET_AND_ADVINI //for network and inifile extension (M-LNX16,17) kado
		/* open and prepare USB scanner handle */
		this->hScanner=usb_open(g_pdev->pdev);
		if (!this->hScanner)
			return SANE_STATUS_IO_ERROR;

		//05/11/11 Fix to escape I/O error
		//if (usb_set_configuration(this->hScanner, 1))
		//	return SANE_STATUS_IO_ERROR;

		errornum = usb_set_configuration(this->hScanner, 1);

		if (usb_claim_interface(this->hScanner, 1))
			return SANE_STATUS_IO_ERROR;




#else    //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
		{
		  //if(this->hScanner ==NULL){
		  //  this->hScanner = calloc(sizeof(dev_handle),1);
		  //}
		  this ->hScanner->usb_w_ep 
		    = get_p_model_info_by_index(this->modelInf.index)->w_endpoint;
		  this ->hScanner->usb_r_ep 
		    = get_p_model_info_by_index(this->modelInf.index)->r_endpoint;

		  if (IFTYPE_USB == this->hScanner->device){
		    this->hScanner->usb=usb_open(g_pdev->pdev);
		    if (!this->hScanner->usb)
		      return SANE_STATUS_IO_ERROR;

		    //05/11/11 Fix to escape I/O error
		    //if (usb_set_configuration(this->hScanner->usb, 1))
		    //  return SANE_STATUS_IO_ERROR;
		    
		    // degrade :  2006/04/05 M-LNX-23 kado 
		    //(M-LNX-24 2006/04/11 kado for Fedora5 USB2.0) 
		    //errornum = usb_set_configuration(this->hScanner->usb, 1);
		    errornum = usb_set_configuration_or_reset_toggle(this, 1); 
		    if (usb_claim_interface(this->hScanner->usb, 1))
		      return SANE_STATUS_IO_ERROR;
		    
		    //(05/11/11 Fix to escape I/O error)   if (usb_set_configuration(this->hScanner->usb, 1))
		    //(05/11/11 Fix to escape I/O error)     return SANE_STATUS_IO_ERROR;
		  }
		  errornum = 0;
		}

#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado

		// Open Device
		rc= OpenDevice(this->hScanner, this->modelInf.seriesNo);
		if (!rc)
			return SANE_STATUS_IO_ERROR;
#endif

		CnvResoNoToUserResoValue( &this->scanInfo.UserSelect, this->devScanInfo.wResoType );

		bResult = QueryScannerInfo( this );
		if (!bResult)
			return SANE_STATUS_INVAL;

		GetScanAreaParam( this );


		// treat as error when the start position of the scan area is out of the MAX scan area 
		if ( (this->devScanInfo.wScanSource == MFCSCANSRC_FB) && 
			(this->scanInfo.ScanAreaMm.top >= (LONG)(this->devScanInfo.dwMaxScanHeight - 80)) )
			return SANE_STATUS_INVAL;


		bResult = StartDecodeStretchProc( this );
		if (!bResult)
			return SANE_STATUS_INVAL;

		//
		// Initialize the ColorMatch 
		//
		InitColorMatchingFunc( this, this->devScanInfo.wColorType, CMATCH_DATALINE_RGB );

		//
		// start the page process of Decode/Stretch
		//
		if( this->devScanInfo.wColorType == COLOR_DTH || this->devScanInfo.wColorType == COLOR_TG ){
			//
			// Preparation of adjusting the Brightness/Contrast
			//
			if( this->uiSetting.nBrightness == 0 && this->uiSetting.nContrast == 0 ){
				//
				// Use the original gray table when both of Brightness and Contrast are the middle position 
				//
				hGray = this->cmatch.hGrayTbl;
			}else{
				//
				// Create the GrayTable for adjustiong the Brightness/Contrast
				//
				hGray = SetupGrayAdjust( this );
			}
		}else{
			hGray = NULL;
		}
		
		//
		// Allocate the buffer to store the received data 
		//
		
		// Allocate the tranmission-preserve buffer , the buffer has enough area to keep more than 3 lines  
		//
		if (this->devScanInfo.wColorType == COLOR_FUL || this->devScanInfo.wColorType == COLOR_FUL_NOCM)
			dwRxBuffMaxSize = (this->devScanInfo.ScanAreaByte.lWidth + 3) * 3; 
		else
			dwRxBuffMaxSize = (this->devScanInfo.ScanAreaByte.lWidth + 3);

		dwRxBuffMaxSize *= (3 + 1); // Allocate the 4 line size (to keep more than 3 line )
		
		if (dwRxBuffMaxSize < (DWORD)gwInBuffSize)
			dwRxBuffMaxSize = (DWORD)gwInBuffSize;
		
		lpRxBuff = (LPBYTE)AllocReceiveBuffer( dwRxBuffMaxSize  );
		lpRxTempBuff = (LPBYTE)MALLOC( dwRxBuffMaxSize  );
	
		if( lpRxBuff == NULL || lpRxTempBuff == NULL ){
			//
			//  Because the buffer cannot be allocated ,return with error code
			//
			return SANE_STATUS_NO_MEM;
		}

		dwRxTempBuffLength = 0;
		FwTempBuffLength = 0;

		// Allocate the Transmission-preserve buffer 
			
		// Allocate the buffer that contains more than 6 lins 
		dwFwTempBuffMaxSize = this->scanInfo.ScanAreaByte.lWidth * 6;
		dwFwTempBuffMaxSize *=2;

		// If exchangeing the resolution is required , allocate the additional area to store the 
		//      expanded data
		nResoLine = this->scanInfo.UserSelect.wResoY / this->devScanInfo.DeviceScan.wResoY;
		if (nResoLine > 1) // Check whether exchanging the resolution is required or not
			dwFwTempBuffMaxSize *= nResoLine;

		// if the value is less than gwInBuffSize, expand to the same size .
		if (dwFwTempBuffMaxSize < (DWORD)gwInBuffSize)
			dwFwTempBuffMaxSize = (DWORD)gwInBuffSize;

		lpFwTempBuff = (LPBYTE)MALLOC( dwFwTempBuffMaxSize );
		WriteLog( " dwRxBuffMaxSize = %d, dwFwTempBuffMaxSize = %d, ", dwRxBuffMaxSize, dwFwTempBuffMaxSize );
		if( lpFwTempBuff == NULL ){
			//
			// if buffer allocation failed , return with No-Memory-error status
			//
			return SANE_STATUS_NO_MEM;
		}

		// Start page scanning
		if (!PageScanStart( this )) {
			ScanEnd( this);
			return SANE_STATUS_INVAL;
		}

		this->scanState.bScanning=TRUE;
		this->scanState.bCanceled=FALSE;
		this->scanState.iProcessEnd=0;
	}
	else {
		WriteLog( " PageStart scanState.iProcessEnd = %d, ", this->scanState.iProcessEnd );

		lRealY = 0;		
		dwRxTempBuffLength = 0;
		FwTempBuffLength = 0;

		//06/02/28
		if ((this->devScanInfo.wScanSource == MFCSCANSRC_ADF || this->devScanInfo.wScanSource == MFCSCANSRC_ADF_DUP) 
                        && this->scanState.iProcessEnd == SCAN_MPS) {
			// The case  next page exists 
			//    start the scanning
			this->scanState.iProcessEnd=0;
			if (!PageScanStart( this )) {
				return SANE_STATUS_INVAL;
			}
			bResult = SANE_STATUS_GOOD;
		}
		else if (this->scanState.iProcessEnd == SCAN_EOF){
			// The case  next page doesn&t exist
			bResult = SANE_STATUS_NO_DOCS;
			return bResult;
		}
		else {
			// The case  the scan-button is pushed during scanning 
			bResult = SANE_STATUS_DEVICE_BUSY;
			return bResult;
		}
	}

	// Start the process of expanding the compressed data
	if (this->scanDec.lpfnScanDecSetTbl != NULL && this->scanDec.lpfnScanDecPageStart != NULL) {
		this->scanDec.lpfnScanDecSetTbl( hGray, NULL );
		bResult = this->scanDec.lpfnScanDecPageStart();
		if (!bResult)
			return SANE_STATUS_INVAL;
		else
			bResult = SANE_STATUS_GOOD;
	}
	else {
		return SANE_STATUS_INVAL;
	}

	nPageScanCnt = 0;	// clear the debug-counter 

	nReadCnt = 0;	// clear the debug-counter 

	return bResult;
}


//-----------------------------------------------------------------------------
//
//	Function name:	PageScanStart
//
//
//	Abstract:
//		Start the page-scanning
//
//
//	Parameters:
//		hWndDlg
//			Handle for windows
//
//
//	Return values:
//		None
//
//-----------------------------------------------------------------------------
//	PageScanStart（A part of PageScan of Windows version）
BOOL
PageScanStart( Brother_Scanner *this )
{
	BOOL rc=FALSE;

	if( this->scanState.nPageCnt == 1 ){
		//send command only 1st page
		if( !bTxScanCmd ){
			//
			// Start-scan-command hasn't be sended yet
			//
			char  szCmdStr[ MFCMAXCMDLENGTH ];
			int   nCmdStrLen;

			//
			// Initialise the command-sended-flag
			//
			bTxScanCmd = TRUE;		// Start-scan-command has already been sended 
			bTxCancelCmd = FALSE;	// Cancel command hasn't been sended yet 

			//
			// send the start-scan-command
			//
			nCmdStrLen = MakeupScanStartCmd( this, szCmdStr );
			if (WriteDeviceCommand( this->hScanner, szCmdStr, nCmdStrLen, this->modelInf.seriesNo ))
				rc=TRUE;

			sleep(3);
			WriteLogScanCmd( "Write",  szCmdStr );
		}
	}else if( this->scanState.nPageCnt > 1 ){

		//
		// The command for starting to scan the 2nd page or following page 
		//
		if (WriteDeviceCommand( this->hScanner, MFCMD_SCANNEXTPAGE, strlen( MFCMD_SCANNEXTPAGE ), this->modelInf.seriesNo))
			rc = TRUE;

		sleep(3);
		WriteLogScanCmd( "Write",  MFCMD_SCANNEXTPAGE );
	}

	return rc;
}


/********************************************************************************
 *										*
 *	FUNCTION	StatusChk(2006/02/28 FIX)						*
 *										*
 *	PURPOSE		Check whether the status code is available or not	*
 *										*
 *	IN		char *lpBuff	buffer to be checked 			*
 *			int nDataSize	size of buffer				*
 *										*
 *	OUT		None							*
 *                                                                              *
 *      Return value	0 	Status code is not availabel			*
 *			1	Status code is available			*
 *                      2       Status code is DUPLEX_NORMAL                    *
 *                      3       Status code is DUPLEX_REVERSE                   *
 *										*
 ********************************************************************************/							  
BOOL
StatusChk(char *lpBuff, int nDataSize)
{
	BOOL	rc=FALSE;	
	LPBYTE	pt = lpBuff;	

	while( 1 ) {
		BYTE headch;
		WORD length;

		if( nDataSize <= 0 )	break;	// All data can be processed (receive all of cluster)

		headch = (BYTE)*pt;

		if ((char)headch < 0) {
			// code of STATUS,CTRL family 
			// refer the following header information
			//06/02/28		  
			rc=1;

			if(headch == 0x84){
			  rc=2;
			}
			else if(headch == 0x85){
			  rc=3;
			}

			WriteLog( ">>> StatusChk header=%2x <<<", headch);
			break;
		}else{
			if (headch == 0) {
				length = 1;
				pt += length;
			}
			else {
				// Image data

				if( nDataSize < 3 )
					length = 0;		// clear
				else
					// Acquire the information of raster data size
					length = *(WORD *)( pt + 1 );	// format: [HEADER(1B)][LENGTH(intel 2B)][DATA...]
				
				if( nDataSize < ( length + 3) ){	// length+3 = head(1B)+length(2B)+data(length)
					break;
				}
				else{
					// whole of 1 line data exists
					nDataSize -= length + 3;	// Consume the Image data size (length +3)
					pt += length + 3;			// get the next header information
				}
			}
			WriteLog( "Header=%2x  Count=%4d nDataSize=%d", (BYTE)headch, length, nDataSize );
		}
	}
	return rc;
}

#define READ_TIMEOUT 5*60*1000 // 5 minute

#define NO39_DEBUG


#ifdef NO39_DEBUG
static struct timeval save_tv;		// Valiable for time-interval information(sec,msec)
static struct timezone save_tz;		// Valiable for time-interval information(min)

#endif


#if       BRSANESUFFIX == 2
/********************************************************************************
 *										*
 *	FUNCTION	PageScan						*
 *										*
 *	PURPOSE		Scan 1 page     					*
 *										*
 *      ARGUMENT	Brother_Scanner *this	： Brother_Scanner structure	*
 *			char *lpFwBuf		： transmission buffer		*
 *			int nMaxLen		： size of transmission buffer	*
 *			int *lpFwLen		： size of data to sent		*
 *										*
 *										*
 *										*
 ********************************************************************************/
int
PageScan( Brother_Scanner *this, char *lpFwBuf, int nMaxLen, int *lpFwLen )
{
	WORD	wData=0;	// received data size (byte)
	WORD	wDataLineCnt=0;	// number of received lines
	int	nAnswer=0;
	int	rc;
	LPBYTE  lpRxTop;
	WORD	wProcessSize;
		
	int	nReadSize;
	LPBYTE  lpReadBuf;
	int	nMinReadSize; // minimum read size

	//05/07/31 For select scan are
	int     nHeadOnly;
	WORD    nLength;

#ifdef NO39_DEBUG
	struct timeval start_tv, tv;
	struct timezone tz;
	long   nSec, nUsec;
#endif
	if (!this->scanState.bScanning) {
		rc = SANE_STATUS_IO_ERROR;
		return rc;
	}
	if (this->scanState.bCanceled) { //cancel-process 
		WriteLog( "Page Canceled" );

		rc = SANE_STATUS_CANCELLED;
		this->scanState.bScanning=FALSE;
		this->scanState.bCanceled=FALSE;
		this->scanState.nPageCnt = 0;

		return rc;
	}

	nPageScanCnt++;
	WriteLog( ">>> PageScan Start <<< cnt=%d nMaxLen=%d\n", nPageScanCnt, nMaxLen);

#ifdef NO39_DEBUG
	if (gettimeofday(&tv, &tz) == 0) {
  					
		if (tv.tv_usec < save_tv.tv_usec) {
			tv.tv_usec += 1000 * 1000 ;
			tv.tv_sec-- ;
		}
		nUsec = tv.tv_usec - save_tv.tv_usec;
		nSec = tv.tv_sec - save_tv.tv_sec;

		WriteLog( " No39 nSec = %d Usec = %d\n", nSec, nUsec ) ;
	}
#endif


	WriteLog( "scanInfo.ScanAreaSize.lWidth = [%d]", this->scanInfo.ScanAreaSize.lWidth );
	WriteLog( "scanInfo.ScanAreaSize.lHeight = [%d]", this->scanInfo.ScanAreaSize.lHeight );
	WriteLog( "scanInfo.ScanAreaByte.lWidth = [%d]", this->scanInfo.ScanAreaByte.lWidth );
	WriteLog( "scanInfo.ScanAreaByte.lHeight = [%d]", this->scanInfo.ScanAreaByte.lHeight );

	WriteLog( "devScanInfo.ScanAreaSize.lWidth = [%d]", this->devScanInfo.ScanAreaSize.lWidth );
	WriteLog( "devScanInfo.ScanAreaSize.lHeight = [%d]", this->devScanInfo.ScanAreaSize.lHeight );
	WriteLog( "devScanInfo.ScanAreaByte.lWidth = [%d]", this->devScanInfo.ScanAreaByte.lWidth );
	WriteLog( "devScanInfo.ScanAreaByte.lHeight = [%d]", this->devScanInfo.ScanAreaByte.lHeight );
	WriteLog( "ReadbufEnd is %d",this->scanState.bReadbufEnd); //050428

	
	memset(lpFwBuf, 0x00, nMaxLen);	//  clear the transmission buffer to zero
	*lpFwLen = 0;

	if ( (!this->scanState.iProcessEnd) && ( FwTempBuffLength < nMaxLen) ) { 
	// The case that the status code hasn't be received yet and 
	//      the size of transmission-buffer is less than the size of data in a transmission-stored data  

	// if the data exists in a data storing buffer , copy the data to the receive-buffer
	memmove( lpRxBuff, lpRxTempBuff, dwRxTempBuffLength );	// restore the keeped data
	wData += dwRxTempBuffLength;	// revise the lenght of the stored data

	lpRxTop = lpRxBuff;
	
	// get the data from the device to expand the more than 3 line data
	if (this->devScanInfo.wColorType == COLOR_FUL || this->devScanInfo.wColorType == COLOR_FUL_NOCM)
		nMinReadSize = (this->devScanInfo.ScanAreaByte.lWidth + 3) * 3; 
	else
		nMinReadSize = (this->devScanInfo.ScanAreaByte.lWidth + 3);

	nMinReadSize *= 3; // Get more than 3 line 
	if ( !this->scanState.bReadbufEnd ) {
		for (rc=0 ; wData < nMinReadSize;)
		{

			nReadSize = dwRxBuffMaxSize - (dwRxTempBuffLength + wData);
			lpReadBuf = lpRxTop+wData;

			nReadCnt++;
			WriteLog( "Read request size is %d, (dwRxTempBuffLength = %d)", gwInBuffSize - dwRxTempBuffLength, dwRxTempBuffLength );
			WriteLog( "PageScan ReadNonFixedData Cnt = %d", nReadCnt );
			
			rc = ReadNonFixedData( this->hScanner, lpReadBuf, nReadSize, READ_TIMEOUT, this->modelInf.seriesNo );
			if (rc < 0) {
				this->scanState.bReadbufEnd = TRUE;
				WriteLog( "  bReadbufEnd =TRUE" );
				break;
			}
			else if (rc > 0){
				//06/02/28
				int sc;
				wData += rc;

				sc = StatusChk(lpRxBuff, wData);
				if (sc == 1) { // check whether the status code has been received or not
					this->scanState.bReadbufEnd = TRUE;
					WriteLog( "bReadbufEnd =TRUE" );
					break;
				}
				else if(sc == 2){
				  break;
				}
			}
		 }
	}

	WriteLog( "Read data size is %d, (dwRxTempBuffLength = %d)", wData, dwRxTempBuffLength );

	WriteLog( "Adjusted wData = %d, (dwRxTempBuffLength = %d)", wData, dwRxTempBuffLength );

	if (wData != 0)
	// spilit the data to each lines
	{
	LPBYTE  pt = lpRxBuff;
	int nFwTempBuffMaxLine;
	int nResoLine;

	// the width (dot) of image data to sent
	if (this->devScanInfo.wColorType == COLOR_FUL || this->devScanInfo.wColorType == COLOR_FUL_NOCM ) {
 		nFwTempBuffMaxLine = (dwFwTempBuffMaxSize / 2 - FwTempBuffLength) / this->scanInfo.ScanAreaByte.lWidth;
		nFwTempBuffMaxLine *= 3; 
	}
	else {
	 	nFwTempBuffMaxLine = (dwFwTempBuffMaxSize / 2 - FwTempBuffLength) / this->scanInfo.ScanAreaByte.lWidth;
	}
	nResoLine= this->scanInfo.UserSelect.wResoY / this->devScanInfo.DeviceScan.wResoY;
	if (nResoLine > 1)
		nFwTempBuffMaxLine /= nResoLine;

	//05/07/31
	nHeadOnly=0;
	nLength=0;

	dwRxTempBuffLength = wData;

	for (wDataLineCnt=0; wDataLineCnt < nFwTempBuffMaxLine;){
		BYTE headch;

		if( dwRxTempBuffLength <= 0 )	break;	// All data can be processed (receive all of cluster)

		headch = (BYTE)*pt;
		if ((char)headch < 0) {
			// STATUS,CTRL code family 
			dwRxTempBuffLength --;			// spend 1 byte for CTRL code family
			pt++;					// refer the next header information
			wDataLineCnt+=3;
		}else{
			if (headch == 0) {
				dwRxTempBuffLength -= 1;
				pt += 1;
				wDataLineCnt++;
				nHeadOnly++;    //05/07/31	
			}
			else {
				// Image data
				WORD length;

				if( dwRxTempBuffLength < 3 )
					length = 0;		// clear
				else{
					// get the length information of raster data 
					length = *(WORD *)( pt + 1 );	// format: [HEADER(1B)][LENGTH(intel 2B)][DATA...]
					nLength = length+3;   //05/07/31
				}
				if( dwRxTempBuffLength < (DWORD)( length + 3) ){	// length+3 = head(1B)+length(2B)+data(length)
					break;
				}
				else{
					// 1 line data exists
					dwRxTempBuffLength -= length + 3;	// spend length+3 byte for image data
					pt += length + 3;			// refer the next header information
					wDataLineCnt++;
				}
			}
		}
	} // end of for(;;)
	wData -= dwRxTempBuffLength;	// take off the odd data (less than 1 line data)

	//transfer the data to the RGB data for the specified models
	if( (this->modelInf.seriesNo >=  MUST_CONVERT_MODEL && this->devScanInfo.wColorType == COLOR_FUL) || 
	        (this->modelInf.seriesNo >=  MUST_CONVERT_MODEL && this->devScanInfo.wColorType == COLOR_FUL_NOCM) ) {
	  	//Analize the buffer data (though the second process)
		LPBYTE lpOrg;
		BYTE headch;
		WORD wDataLineCntTemp;

		//05/07/30 Adjust number of lines to multiple number of three 
		while(( (wDataLineCnt - nHeadOnly) % 3) != 0){
		  WriteLog( "wDataLineCnt - nHeadOnly = %d", wDataLineCnt - nHeadOnly );
		  wDataLineCnt--;
		  wData -= nLength;
		  dwRxTempBuffLength += nLength;
		}
	
		wDataLineCntTemp = wDataLineCnt;
		
		lpOrg = lpRxBuff;

		for (wDataLineCntTemp;  wDataLineCntTemp > 0;){
	
			headch = (BYTE)*lpOrg;

			//06/02/28 for Duplex
			if(headch == 0x84 || headch == 0x85 || (char)headch == 0 ){
			  lpOrg++;
			  wDataLineCntTemp--;
			}else if((char)headch < 0){
			  break;
			}else{
				// Image data
			        WORD length,i;
				unsigned char Y, Cb, Cr;
				int R, G, B;
				unsigned char* pData;

				// get the lenght-information of the raster data
				length = *(WORD *)( lpOrg + 1 );	// format: [HEADER(1B)][LENGTH(intel 2B)][DATA...]
				length += 3;
				pData = (unsigned char *)lpOrg;
			
				//rerew the header to RGB header type
				*pData = (*pData & 0xE3) | 0x04;
				*(pData + length) = (*(pData + length) & 0xE3) | 0x08;
				*(pData + length*2) = (*(pData + length*2) & 0xE3) | 0x0C;

				pData +=3;

				//calculate the RGB data from YCbCr data 
				for(i=0;i < length-3; i++){
				  Cr = *(pData + i);
				  Y  = *(pData + length + i);
				  Cb = *(pData + length*2 + i);
				  
				  R =  Y + 1.402 * (Cr - 128);
				  G =  Y - 0.34414 * (Cb - 128) - 0.71414 * (Cr - 128);
				  B =  Y + 1.772 * (Cb -128);

				  if(R > 255) R=255;
				  if(R < 0) R=0;
				  if(G > 255) G=255;
				  if(G < 0) G=0;
				  if(G > 255) B=255;
				  if(G < 0) B=0;
				  
				  *(pData + i) = (unsigned char)R;
				  *(pData + length + i) = (unsigned char)G;
				  *(pData + length*2 + i) = (unsigned char)B;
				  
				}
				
				wDataLineCntTemp -= 3;
				//renew the pointer 
				if(wDataLineCntTemp > 2){
				  lpOrg += length*3;
				}
				
			}
		}
	}//the end of RGB-exchanging process
	// the end of raster data expanding operation 
#ifdef NO39_DEBUG
	if (gettimeofday(&start_tv, &tz) == -1)
		return FALSE;
#endif

	nAnswer = ProcessMain( this, wData, wDataLineCnt, lpFwTempBuff+FwTempBuffLength, &FwTempBuffLength, &wProcessSize );

#ifdef NO39_DEBUG
	if (gettimeofday(&tv, &tz) == 0) {
		if (tv.tv_usec < start_tv.tv_usec) {
			tv.tv_usec += 1000 * 1000 ;
			tv.tv_sec-- ;
		}
		nSec = tv.tv_sec - start_tv.tv_sec;
		nUsec = tv.tv_usec - start_tv.tv_usec;

		WriteLog( " PageScan ProcessMain Time %d sec %d Us", nSec, nUsec );
		
	}

#endif

	
	if ((dwRxTempBuffLength > 0) || (wProcessSize < wData)) {
		dwRxTempBuffLength += (wData - wProcessSize); 
		memmove( lpRxTempBuff, lpRxBuff+wProcessSize, dwRxTempBuffLength );	// Keep the rest data
	}

	if ( nAnswer == SCAN_EOF || nAnswer == SCAN_MPS )  {
		// The case of last page 
		if( lRealY > 0 ){

			ImgLineProcInfo.pWriteBuff = lpFwTempBuff+FwTempBuffLength;
			ImgLineProcInfo.dwWriteBuffSize = dwImageBuffSize;

			dwWriteImageSize = this->scanDec.lpfnScanDecPageEnd( &ImgLineProcInfo, &nWriteLineCount );
			if( nWriteLineCount > 0 ){
				FwTempBuffLength += dwWriteImageSize;
				lRealY += nWriteLineCount;
			}

#if 1	// DEBUG for MASU
			dwFWImageSize += dwWriteImageSize;
			dwFWImageLine += nWriteLineCount;
			WriteLog( "DEBUG for MASU (PageScan) dwFWImageSize  = %d dwFWImageLine = %d", dwFWImageSize, dwFWImageLine );
			WriteLog( "  PageScan End1 nWriteLineCount = %d", nWriteLineCount );
#endif
		}
		// keep the sitiation  because  the status code is stored to the transmission-keep buffer
		this->scanState.iProcessEnd = nAnswer;
		WriteLog( " PageScan scanState.iProcessEnd = %d, ", this->scanState.iProcessEnd );
	}

	} 
	else { // wData == 0
		if (FwTempBuffLength == 0 && dwRxTempBuffLength == 0) {
			nAnswer = SCAN_EOF;
			WriteLog( "<<<<< PageScan [Read Error End]  <<<<<" );
		}
	}
	WriteLog( "ProcessMain End dwRxTempBuffLength = %d", dwRxTempBuffLength );

	} // if ( (!this->scanState.iProcessEnd) || ( FwTempBuffLength > nMaxLen) ) 

	if (this->scanState.iProcessEnd) { // The case the status code is stored in the transmission-keep buffer
		WriteLog( "<<<<< PageScan Status Code Read!!!" );
		nAnswer = this->scanState.iProcessEnd;
	}

	/* Copy the image data stored in the transmission-keep buffer */
	WriteLog( "<<<<< PageScan FwTempBuffLength = %d", FwTempBuffLength );

	if ( FwTempBuffLength > nMaxLen )
		*lpFwLen = nMaxLen;
	else
		*lpFwLen = FwTempBuffLength;

	FwTempBuffLength -= *lpFwLen ;

	memmove( lpFwBuf, lpFwTempBuff, *lpFwLen);	// copy the data from the transmission-keep buffer to transmission buffer
	memmove( lpFwTempBuff, lpFwTempBuff+*lpFwLen, FwTempBuffLength ); // move the rest data to the buffer top

	rc = SANE_STATUS_GOOD;
	
#ifdef NO39_DEBUG
	gettimeofday(&save_tv, &save_tz);
#endif
	if ( nAnswer == SCAN_EOF || nAnswer == SCAN_MPS )  {

		if (FwTempBuffLength != 0 ) { 
			return rc;
		}
		else {	
			// clear the rest area to white ,if the size of received data is less than the specified size 
			if( lRealY < this->scanInfo.ScanAreaSize.lHeight ){
				// the case that it becomes page-end status before the received data fills  the area of specified size.
				int nHeightLen = this->scanInfo.ScanAreaSize.lHeight - lRealY;
				int nSize = this->scanInfo.ScanAreaByte.lWidth * nHeightLen; 
				int nMaxSize = nMaxLen - *lpFwLen;
				int nMaxLine;
				int nVal;
	
				if (this->devScanInfo.wColorType < COLOR_TG)
					nVal = 0x00;
				else
					nVal = 0xFF;
					
				if ( nSize < nMaxSize ) {
					memset(lpFwBuf+*lpFwLen, nVal, nSize);
					*lpFwLen += nSize;
					lRealY += nHeightLen;
					WriteLog( "PageScan AddSpace End lRealY = %d, nHeightLen = %d nSize = %d nMaxSize = %d *lpFwLen = %d",
					lRealY, nHeightLen, nSize, nMaxSize, *lpFwLen );
				}
				else {
					memset(lpFwBuf+*lpFwLen, nVal, nMaxSize);
					nMaxLine = nMaxSize / this->scanInfo.ScanAreaByte.lWidth;
					*lpFwLen += this->scanInfo.ScanAreaByte.lWidth * nMaxLine;
					lRealY += nMaxLine;
					WriteLog( "PageScan AddSpace lRealY = %d, nHeightLen = %d nSize = %d nMaxSize = %d *lpFwLen = %d",
					lRealY, nHeightLen, nSize, nMaxSize, *lpFwLen );
				}
			}
		}
	}

	switch( nAnswer ){
		case SCAN_CANCEL:
			WriteLog( "Page Canceled" );

			this->scanState.nPageCnt = 0;
			rc = SANE_STATUS_CANCELLED;
			this->scanState.bScanning=FALSE;
			this->scanState.bCanceled=FALSE;
			break; 

		case SCAN_EOF:
			WriteLog( "Page End" );
			WriteLog( "  nAnswer = %d lRealY = %d", nAnswer, lRealY );

			if( lRealY != 0 ) {
				// return the SANE_STATUS_GOOD if  the data exists in transmission-keep buffer
				if (*lpFwLen == 0) {
					this->scanState.bEOF=TRUE;
					this->scanState.bScanning=FALSE;
					rc = SANE_STATUS_EOF
;
				}
			}
			else {
				// return the error status code if EOF is received without receiving any data
				rc = SANE_STATUS_IO_ERROR;
			}
			break; 
		case SCAN_MPS:
			// return the SANE_STATUS_GOOD if  the data exists in transmission-keep buffer
			if (*lpFwLen == 0) {
				this->scanState.bEOF=TRUE;
				rc = SANE_STATUS_EOF;
			}
			break; 
		case SCAN_NODOC:
			rc = SANE_STATUS_NO_DOCS;
			break; 
		case SCAN_DOCJAM:
			rc = SANE_STATUS_JAMMED;
			break; 
		case SCAN_COVER_OPEN:
			rc = SANE_STATUS_COVER_OPEN;
			break; 
		case SCAN_SERVICE_ERR:
			rc = SANE_STATUS_IO_ERROR;
			break; 
	}

	//2006/03/03 for sane_read
	if(nAnswer == SCAN_DUPLEX_NORMAL && *lpFwLen == 0){
	  *lpFwLen = 1;
	  rc = SANE_STATUS_DUPLEX_ADVERSE;
	}

	if(nAnswer != SCAN_DUPLEX_NORMAL)
	  nFwLenTotal += *lpFwLen;	

	WriteLog( "<<<<< PageScan End <<<<< nFwLenTotal = %d lpFwLen = %d ",nFwLenTotal, *lpFwLen);

	return rc;
}
#elif  BRSANESUFFIX == 1

/********************************************************************************
 *										*
 *	FUNCTION	PageScan						*
 *										*
 *	PURPOSE		１ページ分をスキャンする。					*
 *										*
 *	引数		Brother_Scanner *this	： Brother_Scanner構造体		*
 *			char *lpFwBuf		： 送信バッファ			*
 *			int nMaxLen		： 送信バッファ長			*
 *			int *lpFwLen		： 送信データ長			*
 *										*
 *										*
 *										*
 ********************************************************************************/
int
PageScan( Brother_Scanner *this, char *lpFwBuf, int nMaxLen, int *lpFwLen )
{
	WORD	wData=0;	// 受信データサイズ（バイト数）
	WORD	wDataLineCnt=0;	// 受信データのライン数
	int	nAnswer=0;
	int	rc;
	LPBYTE  lpRxTop;
	WORD	wProcessSize;
		
	int	nReadSize;
	LPBYTE  lpReadBuf;
	int	nMinReadSize; // 最少リードサイズ

#ifdef NO39_DEBUG
	struct timeval start_tv, tv;
	struct timezone tz;
	long   nSec, nUsec;
#endif
	if (!this->scanState.bScanning) {
		rc = SANE_STATUS_IO_ERROR;
		return rc;
	}
	if (this->scanState.bCanceled) { //キャンセル処理
		WriteLog( "Page Canceled" );

		rc = SANE_STATUS_CANCELLED;
		this->scanState.bScanning=FALSE;
		this->scanState.bCanceled=FALSE;
		this->scanState.nPageCnt = 0;

		return rc;
	}

	nPageScanCnt++;
	WriteLog( ">>> PageScan Start <<< cnt=%d nMaxLen=%d\n", nPageScanCnt, nMaxLen);

#ifdef NO39_DEBUG
	if (gettimeofday(&tv, &tz) == 0) {
  					
		if (tv.tv_usec < save_tv.tv_usec) {
			tv.tv_usec += 1000 * 1000 ;
			tv.tv_sec-- ;
		}
		nUsec = tv.tv_usec - save_tv.tv_usec;
		nSec = tv.tv_sec - save_tv.tv_sec;

		WriteLog( " No39 nSec = %d Usec = %d\n", nSec, nUsec ) ;
	}
#endif


	WriteLog( "scanInfo.ScanAreaSize.lWidth = [%d]", this->scanInfo.ScanAreaSize.lWidth );
	WriteLog( "scanInfo.ScanAreaSize.lHeight = [%d]", this->scanInfo.ScanAreaSize.lHeight );
	WriteLog( "scanInfo.ScanAreaByte.lWidth = [%d]", this->scanInfo.ScanAreaByte.lWidth );
	WriteLog( "scanInfo.ScanAreaByte.lHeight = [%d]", this->scanInfo.ScanAreaByte.lHeight );

	WriteLog( "devScanInfo.ScanAreaSize.lWidth = [%d]", this->devScanInfo.ScanAreaSize.lWidth );
	WriteLog( "devScanInfo.ScanAreaSize.lHeight = [%d]", this->devScanInfo.ScanAreaSize.lHeight );
	WriteLog( "devScanInfo.ScanAreaByte.lWidth = [%d]", this->devScanInfo.ScanAreaByte.lWidth );
	WriteLog( "devScanInfo.ScanAreaByte.lHeight = [%d]", this->devScanInfo.ScanAreaByte.lHeight );

	
	memset(lpFwBuf, 0x00, nMaxLen);	//  送信バッファをゼロクリアしておく。
	*lpFwLen = 0;

	if ( (!this->scanState.iProcessEnd) && ( FwTempBuffLength < nMaxLen) ) { 
	// ステータスコードを受信していない場合でかつ送信バッファサイズより送信保存バッファのデータ長が小さい場合

	// 保存データバッファにデータが存在する場合は、受信データバッファにコピーする。
	memmove( lpRxBuff, lpRxTempBuff, dwRxTempBuffLength );	// 先頭に待避データ復元
	wData += dwRxTempBuffLength;	// 格納データlengthを補正

	lpRxTop = lpRxBuff;
	
	// 送信保存バッファに最低3ライン分は展開できるようにスキャナからデータ読み込む。
	if (this->devScanInfo.wColorType == COLOR_FUL || this->devScanInfo.wColorType == COLOR_FUL_NOCM )
		nMinReadSize = (this->devScanInfo.ScanAreaByte.lWidth + 3) * 3; 
	else
		nMinReadSize = (this->devScanInfo.ScanAreaByte.lWidth + 3);

	nMinReadSize *= 3; // 最低3ライン分はリードする。
	if ( !this->scanState.bReadbufEnd ) {
		for (rc=0 ; wData < nMinReadSize;)
		{
			nReadSize = dwRxBuffMaxSize - (dwRxTempBuffLength + wData);
			lpReadBuf = lpRxTop+wData;

			nReadCnt++;
			WriteLog( "Read request size is %d, (dwRxTempBuffLength = %d)", gwInBuffSize - dwRxTempBuffLength, dwRxTempBuffLength );
			WriteLog( "PageScan ReadNonFixedData Cnt = %d", nReadCnt );
			
			rc = ReadNonFixedData( this->hScanner, lpReadBuf, nReadSize, READ_TIMEOUT , this->modelInf.seriesNo );
			if (rc < 0) {
				this->scanState.bReadbufEnd = TRUE;
				WriteLog( "  bReadbufEnd =TRUE" );
				break;
			}
			else if (rc > 0){
				wData += rc;

				if (StatusChk(lpRxBuff, wData)) { // ステータスコードを受信したかチェックする。
					this->scanState.bReadbufEnd = TRUE;
					WriteLog( "bReadbufEnd =TRUE" );
					break;
				}
			}
		}
	}

	WriteLog( "Read data size is %d, (dwRxTempBuffLength = %d)", wData, dwRxTempBuffLength );

	WriteLog( "Adjusted wData = %d, (dwRxTempBuffLength = %d)", wData, dwRxTempBuffLength );

	if (wData != 0)
	// データをライン単位までに区切る
	{
	LPBYTE  pt = lpRxBuff;
	int nFwTempBuffMaxLine;
	int nResoLine;

	// 送信するイメージデータの幅(ドット)
	if (this->devScanInfo.wColorType == COLOR_FUL || this->devScanInfo.wColorType == COLOR_FUL_NOCM ) {
 		nFwTempBuffMaxLine = (dwFwTempBuffMaxSize / 2 - FwTempBuffLength) / this->scanInfo.ScanAreaByte.lWidth;
		nFwTempBuffMaxLine *= 3; 
	}
	else {
	 	nFwTempBuffMaxLine = (dwFwTempBuffMaxSize / 2 - FwTempBuffLength) / this->scanInfo.ScanAreaByte.lWidth;
	}
	nResoLine= this->scanInfo.UserSelect.wResoY / this->devScanInfo.DeviceScan.wResoY;
	if (nResoLine > 1)
		nFwTempBuffMaxLine /= nResoLine;

	dwRxTempBuffLength = wData;
	for (wDataLineCnt=0; wDataLineCnt < nFwTempBuffMaxLine;){
		BYTE headch;

		if( dwRxTempBuffLength <= 0 )	break;	// 全てのデータは処理可能(区切り良く受信された)

		headch = (BYTE)*pt;
		if ((char)headch < 0) {
			// STATUS,CTRL系コード
			dwRxTempBuffLength --;			// CTRL系コードは1byte消費
			pt++;					// 次のheader情報を参照
			
			wDataLineCnt+=3;
		}else{
			if (headch == 0) {
				dwRxTempBuffLength -= 1;
				pt += 1;
				wDataLineCnt++;
			}
			else {
				// 画像データ
				WORD length;

				if( dwRxTempBuffLength < 3 )
					length = 0;		// 初期化
				else
					// ラスタデータ長の取得
					length = *(WORD *)( pt + 1 );	// format: [HEADER(1B)][LENGTH(intel 2B)][DATA...]
				
				if( dwRxTempBuffLength < (DWORD)( length + 3) ){	// length+3 = head(1B)+length(2B)+data(length)
					break;
				}
				else{
					// 1line分のデータあり
					dwRxTempBuffLength -= length + 3;	// 画像データは length+3 byte消費
					pt += length + 3;			// 次のheader情報を参照
					wDataLineCnt++;
				}
			}
		}
	} // end of for(;;)
	wData -= dwRxTempBuffLength;	// 展開処理に回すデータから１ライン未満のデータを除く

	// ラスタデータの展開処理
#ifdef NO39_DEBUG
	if (gettimeofday(&start_tv, &tz) == -1)
		return FALSE;
#endif

	nAnswer = ProcessMain( this, wData, wDataLineCnt, lpFwTempBuff+FwTempBuffLength, &FwTempBuffLength, &wProcessSize );

#ifdef NO39_DEBUG
	if (gettimeofday(&tv, &tz) == 0) {
		if (tv.tv_usec < start_tv.tv_usec) {
			tv.tv_usec += 1000 * 1000 ;
			tv.tv_sec-- ;
		}
		nSec = tv.tv_sec - start_tv.tv_sec;
		nUsec = tv.tv_usec - start_tv.tv_usec;

		WriteLog( " PageScan ProcessMain Time %d sec %d Us", nSec, nUsec );
		
	}

#endif

	
	if ((dwRxTempBuffLength > 0) || (wProcessSize < wData)) {
		dwRxTempBuffLength += (wData - wProcessSize); 
		memmove( lpRxTempBuff, lpRxBuff+wProcessSize, dwRxTempBuffLength );	// 残りデータを保存
	}

	if ( nAnswer == SCAN_EOF || nAnswer == SCAN_MPS )  {
		// 最後のページデータの場合
		if( lRealY > 0 ){

			ImgLineProcInfo.pWriteBuff = lpFwTempBuff+FwTempBuffLength;
			ImgLineProcInfo.dwWriteBuffSize = dwImageBuffSize;

			dwWriteImageSize = this->scanDec.lpfnScanDecPageEnd( &ImgLineProcInfo, &nWriteLineCount );
			if( nWriteLineCount > 0 ){
				FwTempBuffLength += dwWriteImageSize;
				lRealY += nWriteLineCount;
			}

#if 1	// DEBUG for MASU
			dwFWImageSize += dwWriteImageSize;
			dwFWImageLine += nWriteLineCount;
			WriteLog( "DEBUG for MASU (PageScan) dwFWImageSize  = %d dwFWImageLine = %d", dwFWImageSize, dwFWImageLine );
			WriteLog( "  PageScan End1 nWriteLineCount = %d", nWriteLineCount );
#endif
		}
		// ステータスコードが送信保存バッファに入ったため、状態を覚えておく
		this->scanState.iProcessEnd = nAnswer;
		WriteLog( " PageScan scanState.iProcessEnd = %d, ", this->scanState.iProcessEnd );
	}

	} 
	else { // wData == 0
		if (FwTempBuffLength == 0 && dwRxTempBuffLength == 0) {
			nAnswer = SCAN_EOF;
			WriteLog( "<<<<< PageScan [Read Error End]  <<<<<" );
		}
	}
	WriteLog( "ProcessMain End dwRxTempBuffLength = %d", dwRxTempBuffLength );

	} // if ( (!this->scanState.iProcessEnd) || ( FwTempBuffLength > nMaxLen) ) 

	if (this->scanState.iProcessEnd) { // 送信保存バッファにステータスコードを受信している場合
		WriteLog( "<<<<< PageScan Status Code Read!!!" );
		nAnswer = this->scanState.iProcessEnd;
	}

	/* 送信バッファに送信保存バッファにあるイメージデータをコピーする。*/
	WriteLog( "<<<<< PageScan FwTempBuffLength = %d", FwTempBuffLength );

	if ( FwTempBuffLength > nMaxLen )
		*lpFwLen = nMaxLen;
	else
		*lpFwLen = FwTempBuffLength;

	FwTempBuffLength -= *lpFwLen ;

	memmove( lpFwBuf, lpFwTempBuff, *lpFwLen);	// 送信保存バッファから送信バッファへコピーする。
	memmove( lpFwTempBuff, lpFwTempBuff+*lpFwLen, FwTempBuffLength ); // 残りの保存データを先頭に移動する。	

	rc = SANE_STATUS_GOOD;
	
#ifdef NO39_DEBUG
	gettimeofday(&save_tv, &save_tz);
#endif
	if ( nAnswer == SCAN_EOF || nAnswer == SCAN_MPS )  {

		if (FwTempBuffLength != 0 ) { 
			return rc;
		}
		else {	
			// 指定したデータ長より受信したデータ長が少ない場合、残りのデータ長を空白としてセットする。		
			if( lRealY < this->scanInfo.ScanAreaSize.lHeight ){
				// 指定した長さより少ない値の状態で、ページエンドステータスとなった場合
				int nHeightLen = this->scanInfo.ScanAreaSize.lHeight - lRealY;
				int nSize = this->scanInfo.ScanAreaByte.lWidth * nHeightLen; 
				int nMaxSize = nMaxLen - *lpFwLen;
				int nMaxLine;
				int nVal;
	
				if (this->devScanInfo.wColorType < COLOR_TG)
					nVal = 0x00;
				else
					nVal = 0xFF;
					
				if ( nSize < nMaxSize ) {
					memset(lpFwBuf+*lpFwLen, nVal, nSize);
					*lpFwLen += nSize;
					lRealY += nHeightLen;
					WriteLog( "PageScan AddSpace End lRealY = %d, nHeightLen = %d nSize = %d nMaxSize = %d *lpFwLen = %d",
					lRealY, nHeightLen, nSize, nMaxSize, *lpFwLen );
				}
				else {
					memset(lpFwBuf+*lpFwLen, nVal, nMaxSize);
					nMaxLine = nMaxSize / this->scanInfo.ScanAreaByte.lWidth;
					*lpFwLen += this->scanInfo.ScanAreaByte.lWidth * nMaxLine;
					lRealY += nMaxLine;
					WriteLog( "PageScan AddSpace lRealY = %d, nHeightLen = %d nSize = %d nMaxSize = %d *lpFwLen = %d",
					lRealY, nHeightLen, nSize, nMaxSize, *lpFwLen );
				}
			}
		}
	}

	switch( nAnswer ){
		case SCAN_CANCEL:
			WriteLog( "Page Canceled" );

			this->scanState.nPageCnt = 0;
			rc = SANE_STATUS_CANCELLED;
			this->scanState.bScanning=FALSE;
			this->scanState.bCanceled=FALSE;
			break; 

		case SCAN_EOF:
			WriteLog( "Page End" );
			WriteLog( "  nAnswer = %d lRealY = %d", nAnswer, lRealY );

			if( lRealY != 0 ) {
				// 送信保存バッファにデータがある間は、SANE_STATUS_GOODを返す。
				if (*lpFwLen == 0) {
					this->scanState.bEOF=TRUE;
					this->scanState.bScanning=FALSE;
					rc = SANE_STATUS_EOF;
				}
			}
			else {
				// データ受信無しでEOFの場合、エラーとする。
				rc = SANE_STATUS_IO_ERROR;
			}
			break; 
		case SCAN_MPS:
			// 送信保存バッファにデータがある間は、SANE_STATUS_GOODを返す。
			if (*lpFwLen == 0) {
				this->scanState.bEOF=TRUE;
				rc = SANE_STATUS_EOF;
			}
			break; 
		case SCAN_NODOC:
			rc = SANE_STATUS_NO_DOCS;
			break; 
		case SCAN_DOCJAM:
			rc = SANE_STATUS_JAMMED;
			break; 
		case SCAN_COVER_OPEN:
			rc = SANE_STATUS_COVER_OPEN;
			break; 
		case SCAN_SERVICE_ERR:
			rc = SANE_STATUS_IO_ERROR;
			break; 
	}

	nFwLenTotal += *lpFwLen;
	WriteLog( "<<<<< PageScan End <<<<< nFwLenTotal = %d lpFwLen = %d ",nFwLenTotal, *lpFwLen);

	return rc;
}

#else    //BRSANESUFFIX
  force causing compile error
#endif   //BRSANESUFFIX



void
ReadTrash( Brother_Scanner *this )
{
	// discard the received data 
	//    regard NO-DATA if ZERO datas are recieved continuously for  more than 1 sec

	BYTE *lpBrBuff;
	int nResultSize;
	int nEndPoint,i;

	struct timeval start_tv, tv;
	struct timezone tz;
	long   nSec, nUsec;
	long   nTimeOutSec, nTimeOutUsec;

	//2005/11/11 Add SeriesNnumber information for L4CFB and later
	nEndPoint = 0x84;

	for(i=0; i < ANOTHERENDPOINT; i++){
          if( this->modelInf.seriesNo == ChangeEndpoint[i]){
	    nEndPoint = 0x85;
	    break;
	  }
	}

	//if(this->modelInf.seriesNo == L4CFB)
	//  nEndPoint=0x85;
	//else
	//  nEndPoint=0x84;

	if (gettimeofday(&start_tv, &tz) == -1)
		return ;

	lpBrBuff = (LPBYTE)MALLOC( 32000 );
	if (!lpBrBuff)
		return;

	if (gettimeofday(&start_tv, &tz) == -1) {
		FREE(lpBrBuff);
		return ;
	}
	// calculate the second-order of the timeout value
	nTimeOutSec = 1;
	// calculate the micro-second-order of the timeout value
	nTimeOutUsec = 0;

	nResultSize = 1;
	while (1) {
		if (gettimeofday(&tv, &tz) == 0) {
			if (tv.tv_usec < start_tv.tv_usec) {
				tv.tv_usec += 1000 * 1000 ;
				tv.tv_sec-- ;
			}
			nSec = tv.tv_sec - start_tv.tv_sec;
			nUsec = tv.tv_usec - start_tv.tv_usec;

			WriteLog( "AbortPageScan Read nSec = %d Usec = %d\n", nSec, nUsec ) ;

			if (nSec > nTimeOutSec) { // break if timeout occures
				break;
			} 
			else if( nSec == nTimeOutSec) { // the case the read time  is the same as the timeout value in second-order
				if (nUsec >= nTimeOutUsec) { // break if read time is larger than timeout value 
					break;
				}
			}
		}
		else {
			break;
		}
		
		usleep(30 * 1000); // wait for 30ms

		// discard the data
#ifndef  NET_AND_ADVINI //for network and inifile extension (M-LNX16,17) kado
		nResultSize = usb_bulk_read(this->hScanner,
	        nEndPoint,
	        lpBrBuff, 
	        32000,
	        2000
		);
#else    //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
		if (IFTYPE_NET != this->hScanner->device){
		  nResultSize = usb_bulk_read(this->hScanner->usb,
					      nEndPoint,
					      lpBrBuff, 
					      32000,
					      2000
					      );
		}
		else{
		  struct timeval net_timeout = NETTIMEOUTST;
		  nResultSize = 0;
		  read_device_net(this->hScanner->net, 
				  lpBrBuff, 
				  32000,
				  &nResultSize,
				  &net_timeout);

		}
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado

		WriteLog( "AbortPageScan Read end nResultSize = %d %d", nResultSize,nEndPoint) ;

		// reset the timer until the data can be read.
		if (nResultSize > 0) { // the case data exists.

			if (gettimeofday(&start_tv, &tz) == -1)
				break;
		}
	}
	FREE(lpBrBuff);

}
//-----------------------------------------------------------------------------
//
//	Function name:	AbortPageScan
//
//
//	Abstract:
//		Abort the Page-scan process 
//
//
//	Parameters:
//
//	Return values:
//
//-----------------------------------------------------------------------------
//	AbortPageScan（The part of PageScan i the windows version）
void
AbortPageScan( Brother_Scanner *this )
{
	WriteLog( ">>>>> AbortPageScan Start >>>>>" );

	//
	// Send the cancel command 
	//
	SendCancelCommand(this->hScanner, this->modelInf.seriesNo);
	this->scanState.bCanceled=TRUE;
	ReadTrash( this );

	WriteLog( "<<<<< AbortPageScan End <<<<<" );

	return;
}

/********************************************************************************
 *										*
 *	FUNCTION	ScanEnd							*
 *										*
 *	PURPOSE		Process of termination of the scan session		*
 *				request the source-closeing process		*
 *										*
 *	IN		HWND	hdlg	handle to the dialog 			*
 *										*
 *	OUT		None							*
 *										*
 ********************************************************************************/
void
ScanEnd( Brother_Scanner *this )
{
	BOOL  bResult;

	this->scanState.nPageCnt = 0;
	bTxScanCmd = FALSE;		// Clear the begining-of-scan flag

#ifndef DEBUG_No39
	if ( this->hScanner != NULL ) {

#ifndef  NET_AND_ADVINI //for network and inifile extension (M-LNX16,17) kado
		CloseDevice(this->hScanner);
		usb_release_interface(this->hScanner, 1);
		usb_close(this->hScanner);
		this->hScanner=NULL;
#else    //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado
		if (IFTYPE_USB == this->hScanner->device){
		  if(this->hScanner->usb){
		    CloseDevice(this->hScanner);
		    usb_release_interface(this->hScanner->usb, 1);
		    usb_close(this->hScanner->usb);
		    this->hScanner->usb = NULL;
		  }
		}
		else {
		  if(this->hScanner->net){
		    CloseDevice(this->hScanner);
		    this->hScanner->net = NULL;
		  }
		}

		// this->hScanner must be available until sane is closed 
#endif   //NET_AND_ADVINI//for network and inifile extension (M-LNX16,17) kado

	}
#endif
	if( hGray && ( hGray != this->cmatch.hGrayTbl ) ){
		FREE( hGray );
		hGray = NULL;
		WriteLog( "free hGray" );
	}

	FreeReceiveBuffer();
	if (lpRxTempBuff) {
		FREE(lpRxTempBuff);
		lpRxTempBuff = NULL;
	}
	if (lpFwTempBuff) {
		FREE(lpFwTempBuff);
		lpFwTempBuff = NULL;
	}
	//
	// end of the decode/stretch process
	//
	bResult = this->scanDec.lpfnScanDecClose();

	WriteLog( "<<<<< Terminate Scanning <<<<<" );

}


//-----------------------------------------------------------------------------
//
//	Function name:	GetScanAreaParam
//
//
//	Abstract:
//		Acquire the scan-area informations for the scan parameters
//		     from the scan-area information (0.1mm-order)   ??
//
//
//	Parameters:
//		None
//
//
//	Return values:
//		None
//
//-----------------------------------------------------------------------------
//	GetScanAreaParam(GetScanDot of the windows version）
void 
GetScanAreaParam( Brother_Scanner *this )
{
	LPAREARECT  lpScanAreaDot;
	LONG    lUserResoX,   lUserResoY;
	LONG    lDeviceResoX, lDeviceResoY;


	//
	// read the information from UI setting and write the information to the scanInfo structure  
	//
	this->scanInfo.ScanAreaMm = this->uiSetting.ScanAreaMm;

	//
	// exchange the scan-area-coordinates form  0.1mm unit to pixel unit.
	//
	lUserResoX   = this->scanInfo.UserSelect.wResoX;
	lUserResoY   = this->scanInfo.UserSelect.wResoY;
	lDeviceResoX = this->devScanInfo.DeviceScan.wResoX;
	lDeviceResoY = this->devScanInfo.DeviceScan.wResoY;

	// user-specified scan-area-coordinates (dot unit)
	lpScanAreaDot = &this->scanInfo.ScanAreaDot;
	lpScanAreaDot->left   = this->scanInfo.ScanAreaMm.left   * lUserResoX / 254L;
	lpScanAreaDot->right  = this->scanInfo.ScanAreaMm.right  * lUserResoX / 254L;
	lpScanAreaDot->top    = this->scanInfo.ScanAreaMm.top    * lUserResoY / 254L;
	lpScanAreaDot->bottom = this->scanInfo.ScanAreaMm.bottom * lUserResoY / 254L;

	// The scan-area-coordinates for scan-parameters (dot unit)
	lpScanAreaDot = &this->devScanInfo.ScanAreaDot;
	lpScanAreaDot->left   = this->scanInfo.ScanAreaMm.left   * lDeviceResoX / 254L;
	lpScanAreaDot->right  = this->scanInfo.ScanAreaMm.right  * lDeviceResoX / 254L;
	lpScanAreaDot->top    = this->scanInfo.ScanAreaMm.top    * lDeviceResoY / 254L;
	lpScanAreaDot->bottom = this->scanInfo.ScanAreaMm.bottom * lDeviceResoY / 254L;

	//
	// Calculate the scan-area-coordinates for scan-parameters
	//
	GetDeviceScanArea( this, lpScanAreaDot );

	//
	// Calculate the scan size (dot unit)
	//
	this->devScanInfo.ScanAreaSize.lWidth  = lpScanAreaDot->right  - lpScanAreaDot->left;
	this->devScanInfo.ScanAreaSize.lHeight = lpScanAreaDot->bottom - lpScanAreaDot->top;

	//
	// Calculate the size of scan-area (byte unit)
	//
	if( this->devScanInfo.wColorType == COLOR_BW || this->devScanInfo.wColorType == COLOR_ED ){
		this->devScanInfo.ScanAreaByte.lWidth = ( this->devScanInfo.ScanAreaSize.lWidth + 7 ) / 8;
	}else{
		this->devScanInfo.ScanAreaByte.lWidth = this->devScanInfo.ScanAreaSize.lWidth;
	}
	this->devScanInfo.ScanAreaByte.lHeight = this->devScanInfo.ScanAreaSize.lHeight;

	WriteLog(" brother_scanner.c GetScanAreaParam LOG START !!");

	WriteLog( "scanInfo.ScanAreaMm.left = [%d]", this->scanInfo.ScanAreaMm.left );
	WriteLog( "scanInfo.ScanAreaMm.right = [%d]", this->scanInfo.ScanAreaMm.right );
	WriteLog( "scanInfo.ScanAreaMm.top = [%d]", this->scanInfo.ScanAreaMm.top );
	WriteLog( "scanInfo.ScanAreaMm.bottom = [%d]", this->scanInfo.ScanAreaMm.bottom );

	WriteLog( "lUserResoX = [%d]", lUserResoX );
	WriteLog( "lUserResoY = [%d]", lUserResoY );

	WriteLog( "scanInfo.ScanAreaSize.lWidth = [%d]", this->scanInfo.ScanAreaSize.lWidth );
	WriteLog( "scanInfo.ScanAreaSize.lHeight = [%d]", this->scanInfo.ScanAreaSize.lHeight );
	WriteLog( "scanInfo.ScanAreaByte.lWidth = [%d]", this->scanInfo.ScanAreaByte.lWidth );
	WriteLog( "scanInfo.ScanAreaByte.lHeight = [%d]", this->scanInfo.ScanAreaByte.lHeight );

	WriteLog( "lDeviceResoX = [%d]", lDeviceResoX );
	WriteLog( "lDeviceResoY = [%d]", lDeviceResoY );

	WriteLog( "devScanInfo.ScanAreaSize.lWidth = [%d]", this->devScanInfo.ScanAreaSize.lWidth );
	WriteLog( "devScanInfo.ScanAreaSize.lHeight = [%d]", this->devScanInfo.ScanAreaSize.lHeight );
	WriteLog( "devScanInfo.ScanAreaByte.lWidth = [%d]", this->devScanInfo.ScanAreaByte.lWidth );
	WriteLog( "devScanInfo.ScanAreaByte.lHeight = [%d]", this->devScanInfo.ScanAreaByte.lHeight );

	WriteLog(" brother_scanner.c GetScanAreaParam LOG END !!");
}


//-----------------------------------------------------------------------------
//
//	Function name:	StartDecodeStretchProc
//
//
//	Abstract:
//		Initialize the scanned-data-expanding-modules/resolution-exchange-module
//
//
//	Parameters:
//		None
//
//
//	Return values:
//		TRUE  = terminate successfully
//		FALSE = initializing was failed
//
//-----------------------------------------------------------------------------
//
BOOL
StartDecodeStretchProc( Brother_Scanner *this )
{
	BOOL  bResult = TRUE;

	//
	// Set the raster information to the ImageProcInfo structure
	//
	ImageProcInfo.nInResoX  = this->devScanInfo.DeviceScan.wResoX;
	ImageProcInfo.nInResoY  = this->devScanInfo.DeviceScan.wResoY;
	ImageProcInfo.nOutResoX = this->scanInfo.UserSelect.wResoX;
	ImageProcInfo.nOutResoY = this->scanInfo.UserSelect.wResoY;
	WriteLog("nInResoX=%d nInResoY=%d nOutResoX=%d nOutResoY=%d",ImageProcInfo.nInResoX,ImageProcInfo.nInResoY,ImageProcInfo.nOutResoX,ImageProcInfo.nOutResoY);
	ImageProcInfo.dwInLinePixCnt = this->devScanInfo.ScanAreaSize.lWidth;

	ImageProcInfo.nOutDataKind = SCODK_PIXEL_RGB;

#if 1 // modify for the bug that scanning is failed  at Black^White mode 
	ImageProcInfo.bLongBoundary = FALSE;	// never do 4 byte alignment 
#else
	ImageProcInfo.bLongBoundary = TRUE;
#endif
	//
	// set color type 
	//
	switch( this->devScanInfo.wColorType ){
		case COLOR_BW:			// Black & White
			ImageProcInfo.nColorType = SCCLR_TYPE_BW;
			break;

		case COLOR_ED:			// Error Diffusion Gray
			ImageProcInfo.nColorType = SCCLR_TYPE_ED;
			break;

		case COLOR_DTH:			// Dithered Gray
			ImageProcInfo.nColorType = SCCLR_TYPE_DTH;
			break;

		case COLOR_TG:			// True Gray
			ImageProcInfo.nColorType = SCCLR_TYPE_TG;
			break;

		case COLOR_256:			// 256 Color
			ImageProcInfo.nColorType = SCCLR_TYPE_256;
			break;

		case COLOR_FUL:			// 24bit Full Color
			ImageProcInfo.nColorType = SCCLR_TYPE_FUL;
			break;

		case COLOR_FUL_NOCM:	// 24bit Full Color(do not colormatch)
			ImageProcInfo.nColorType = SCCLR_TYPE_FULNOCM;
			break;
	}

	//
	// Initialize the scanned-data-expanding-modules/resolution-exchange-module
	//
	if (this->scanDec.lpfnScanDecOpen) {
		bResult = this->scanDec.lpfnScanDecOpen( &ImageProcInfo );
		WriteLog( "Result from ScanDecOpen is %d", bResult );
	}
	
	dwImageBuffSize = ImageProcInfo.dwOutWriteMaxSize;
	WriteLog( "ScanDec Func needs maximum size is %d", dwImageBuffSize );

	//
	//Set the expaned data width/ resolution-exchanged data width
	//
	this->scanInfo.ScanAreaSize.lWidth = ImageProcInfo.dwOutLinePixCnt;
	this->scanInfo.ScanAreaByte.lWidth = ImageProcInfo.dwOutLineByte;

	this->scanInfo.ScanAreaSize.lHeight = this->devScanInfo.ScanAreaSize.lHeight;

	if( this->devScanInfo.DeviceScan.wResoY != this->scanInfo.UserSelect.wResoY ){
		//
		// revise the scaled/magnified scan-length 
		//
		this->scanInfo.ScanAreaSize.lHeight *= this->scanInfo.UserSelect.wResoY;
		this->scanInfo.ScanAreaSize.lHeight /= this->devScanInfo.DeviceScan.wResoY;
	}
	this->scanInfo.ScanAreaByte.lHeight = this->scanInfo.ScanAreaSize.lHeight;

	return bResult;
}


//-----------------------------------------------------------------------------
//
//	Function name:	GetDeviceScanArea
//
//
//	Abstract:
//		Acquire the scan-area informations for the scan parameters
//
//
//	Parameters:
//		lpScanAreaDot
//			pointer to the scan-area coordinates
//
//
//	Return values:
//		None
//
//-----------------------------------------------------------------------------
//	GetDeviceScanArea（Parts of GetScanDot in  the Windows version）
void
GetDeviceScanArea( Brother_Scanner *this, LPAREARECT lpScanAreaDot )
{
	LONG    lMaxScanPixels;
	LONG    lMaxScanRaster;
	LONG    lTempWidth;


	lMaxScanPixels = this->devScanInfo.dwMaxScanPixels;

	if( this->devScanInfo.wScanSource == MFCSCANSRC_FB ){
		//
		// When the scan-source is FB
		//    restrict to the max claster number (it is acquired from the device) 
		//
		lMaxScanRaster = this->devScanInfo.dwMaxScanRaster;
	}else{
		//
		// When the scan-source is ADF restrict to 14 inch.
		//
		lMaxScanRaster = 14 * this->devScanInfo.DeviceScan.wResoY;
	}
	//
	// revise the right position of the scan-area
	//
	if( lpScanAreaDot->right > lMaxScanPixels ){
		lpScanAreaDot->right = lMaxScanPixels;
	}
	lTempWidth = lpScanAreaDot->right - lpScanAreaDot->left;
	if( lTempWidth < 16 ){
	    //
	    // The case the scaned width is less than 16 dots
	    //
	    lpScanAreaDot->right = lpScanAreaDot->left + 16;
	    if( lpScanAreaDot->right > lMaxScanPixels ){
			//
			// if the specified right position of the scan area is over the scannable limit 
			//       calculate the left position of the scan-area based on the right position
			//
			lpScanAreaDot->right = lMaxScanPixels;
			lpScanAreaDot->left  = lMaxScanPixels - 16;
		}
	}
	//
	// revise the bottom position of the scan-area
	//
	if( lpScanAreaDot->bottom > lMaxScanRaster ){
		lpScanAreaDot->bottom = lMaxScanRaster;
	}

	//
	// if the normal-scan is selected round on the 16dot unit 
	//   0-7 -> 0, 8-15 -> 16
	//
	lpScanAreaDot->left  = ( lpScanAreaDot->left  + 0x8 ) & 0xFFF0;
	lpScanAreaDot->right = ( lpScanAreaDot->right + 0x8 ) & 0xFFF0;

}


/********************************************************************************
 *										*
 *	FUNCTION	ProcessMain						*
 *										*
 *	IN		pointer to the Brother_Scanner	structure       	*
 *			WORD		number of data				*
 *			char *		output buffer 				*
 *			int *		pointer to the output data number	*
 *										*
 *	OUT		None							*
 *										* 
 *	COMMENT		process the data acquired from the scanner		*
 *			Regard the stored data as the line unit data		*
 *										*
 ********************************************************************************/
int
ProcessMain(Brother_Scanner *this, WORD wByte, WORD wDataLineCnt, char * lpFwBuf, int *lpFwBufcnt, WORD *lpProcessSize)
{
	LPBYTE	lpScn = lpRxBuff;
	LPBYTE	lpScnEnd;
	int	answer = SCAN_GOOD;
	char	Header;
	DWORD	Dcount;
	LONG	count;
	LPBYTE	lpSrc;
	LPBYTE	lpHosei;
	WORD	wLineCnt;
#ifdef NO39_DEBUG
	struct timeval start_tv, tv;
	struct timezone tz;
	long   nSec, nUsec;
#endif

	WriteLog( ">>>>> ProcessMain Start >>>>> wDataLineCnt=%d", wDataLineCnt);

	lpScn = lpRxBuff;

	lpScnEnd = lpScn + wByte;
	WriteLog( "lpFwBuf = %X, lpScn = %X, wByte = %d, lpScnEnd = %X", lpFwBuf, lpScn, wByte, lpScnEnd );

	if (this->devScanInfo.wColorType == COLOR_FUL || this->devScanInfo.wColorType == COLOR_FUL_NOCM) {
		if (wDataLineCnt)
		wDataLineCnt = (wDataLineCnt / 3 * 3);
	}
	for( wLineCnt=0; wLineCnt < wDataLineCnt; wLineCnt++ ){
		//
		//	Header <- line header (byte)
		//
		Header = *lpScn++;
		//06/02/28
		if((BYTE)Header == 0x84){
		  WriteLog( "Header = 84" );
		  wLineCnt += 2;
		  answer = SCAN_DUPLEX_NORMAL;
		}
		else if( (BYTE)Header == 0x85){
		  WriteLog( "Header = 85" );
		  wLineCnt += 2;
		  answer = SCAN_DUPLEX_REVERSE;
		} 
		else if( Header < 0 ){
			//
			//	Status code
			//
			WriteLog( "Header=%2x  ", (BYTE)Header );

			answer = GetStatusCode( Header );
			break;	//break for
		}else{
			//
			//	Scanned Data
			//
			if( Header == 0 ){
				//
				//	White line
				//
				WriteLog( "Header=%2x  while line", (BYTE)Header );
				WriteLog( "\tlpFwBufp = %X, lpScn = %X", lpFwBuf, lpScn );

				if( lpFwBuf ){
					lRealY++;
					lpFwBuf += this->scanInfo.ScanAreaByte.lWidth;
				}
			}else{
				//
				//	Scanner data
				//
				Dcount = (DWORD)*( (WORD *)lpScn );	// data length
				count  = (WORD)Dcount;		// data length
				lpScn += 2;
				lpSrc = lpScn;
				lpHosei = 0;	// What is "Hosei" ???

				WriteLog( "Header=%2x  Count=%4d", (BYTE)Header, count );
				WriteLog( "\tlpFwBuf = %X, lpScn = %X", lpFwBuf, lpScn );

				if( lpFwBuf ){
					//
					// set the valiables for the raster data expansion/ resolution exchanging
					//
					SetupImgLineProc( Header );
					ImgLineProcInfo.pLineData      = lpSrc;
					ImgLineProcInfo.dwLineDataSize = count;
					ImgLineProcInfo.pWriteBuff     = lpFwBuf;
					//
					// raster data expansion/ resolution exchanging
					//
#ifdef NO39_DEBUG
	if (gettimeofday(&start_tv, &tz) == -1)
		return FALSE;
#endif

					dwWriteImageSize = this->scanDec.lpfnScanDecWrite( &ImgLineProcInfo, &nWriteLineCount );
					WriteLog( "\tlpFwBuf = %X, WriteSize = %d, LineCount = %d, RealY = %d", lpFwBuf, dwWriteImageSize, nWriteLineCount, lRealY );

#ifdef NO39_DEBUG
	if (gettimeofday(&tv, &tz) == 0) {
		if (tv.tv_usec < start_tv.tv_usec) {
			tv.tv_usec += 1000 * 1000 ;
			tv.tv_sec-- ;
		}
		nSec = tv.tv_sec - start_tv.tv_sec;
		nUsec = tv.tv_usec - start_tv.tv_usec;

		WriteLog( " ProcessMain ScanDecWrite Time %d sec %d Us", nSec, nUsec );
		
	}
#endif

					if( nWriteLineCount > 0 ){
						*lpFwBufcnt += dwWriteImageSize;
#if 1	// DEBUG for MASU
						dwFWImageSize += dwWriteImageSize;
						dwFWImageLine += nWriteLineCount;
						WriteLog( "DEBUG for MASU (ProcessMain) dwFWImageSize  = %d dwFWImageLine = %d", dwFWImageSize, dwFWImageLine );
#endif

						if( this->mfcModelInfo.bColorModel && ! this->modelConfig.bNoUseColorMatch && this->devScanInfo.wColorType == COLOR_FUL ){
							int  i;
							for( i = 0; i < nWriteLineCount; i++ ){
								ExecColorMatchingFunc( this, lpFwBuf, this->scanInfo.ScanAreaByte.lWidth, 1 );
								lpFwBuf += dwWriteImageSize / nWriteLineCount;
							}
						}else{
							lpFwBuf += dwWriteImageSize;
						}
						lRealY += nWriteLineCount;
					}
				}
				lpScn += Dcount;
			}

		}//end of if *lpScn<0
	}//end of for
	*lpProcessSize = (WORD)(lpScn - lpRxBuff);

	WriteLog( "<<<<< ProcessMain End <<<<<" );

	return answer;
}


//-----------------------------------------------------------------------------
//
//	Function name:	SetupImgLineProc
//
//
//	Abstract:
//		set the valiables for raster-data-expansion/ resolution-exchanging
//
//
//	Parameters:
//		chLineHeader
//			line header of the acquired raster data
//
//
//	Return values:
//		None
//
//-----------------------------------------------------------------------------
//
void
SetupImgLineProc( BYTE chLineHeader )
{
	BYTE  chColorMode;
	BYTE  chCompression;


	//
	// Set the color type of the line data
	//
	chColorMode = chLineHeader & 0x1C;
	switch( chColorMode ){
		case 0x00:	// Mono-data
			ImgLineProcInfo.nInDataKind = SCIDK_MONO;
			break;

		case 0x04:	// Red data
			ImgLineProcInfo.nInDataKind = SCIDK_R;
			break;

		case 0x08:	// Green data
			ImgLineProcInfo.nInDataKind = SCIDK_G;
			break;

		case 0x0C:	// Blue data
			ImgLineProcInfo.nInDataKind = SCIDK_B;
			break;

		case 0x10:	// Dot-orderd RGB data (gaso-jyunji-RGB)
			ImgLineProcInfo.nInDataKind = SCIDK_RGB;
			break;

		case 0x14:	// Dot-orderd BGR data (gaso-jyunji-BGR)
			ImgLineProcInfo.nInDataKind = SCIDK_BGR;
			break;

		case 0x1C:	// 256 colors data
			ImgLineProcInfo.nInDataKind = SCIDK_256;
			break;
	}

	//
	// Set the compression mode for the line data
	//
	chCompression = chLineHeader & 0x03;
	if( chCompression == 2 ){
		// Packbits compression
		ImgLineProcInfo.nInDataComp = SCIDC_PACK;
	}else{
		ImgLineProcInfo.nInDataComp = SCIDC_NONCOMP;
	}

	//
	// set other parameters 
	//
#if 1
	ImgLineProcInfo.bReverWrite     = FALSE;
#else
	ImgLineProcInfo.bReverWrite     = TRUE;
#endif
	ImgLineProcInfo.dwWriteBuffSize = dwImageBuffSize;
}


//-----------------------------------------------------------------------------
//
//	Function name:	GetStatusCode
//
//
//	Abstract:
//		proccess for the line status/ error code
//
//
//	Parameters:
//		nLineHeader
//			line header
//
//
//
//	Return values:
//		status information of scan
//
//-----------------------------------------------------------------------------
//
int
GetStatusCode( BYTE nLineHeader )
{
	int   answer;

	switch( nLineHeader ){
		case 0x80:	//terminate
			WriteLog( "\tPage end Detect" );
			answer = SCAN_EOF;
			break;

		case 0x81:	//Page end
			WriteLog( "\tNextPage Detect" );
			answer = SCAN_MPS;
			break;

		case 0xE3:	//MF14	MFC cancel scan because of timeout
		case 0x83:	//cancel acknowledge
			WriteLog( "\tCancel acknowledge" );
			answer = SCAN_CANCEL;
			break;
		//06/02/28
	        case 0x84:
		        WriteLog( "\tDuplex Normal" );
		        answer = SCAN_MPS;
		        break;
	        case 0x85:
		        WriteLog( "\tDuplex Reverse" );
		        answer = SCAN_MPS;
	                break;
		case 0xC2:	//no document
			WriteLog( "\tNo document" );
			answer = SCAN_NODOC;	// if no document, don't send picture
			break;

		case 0xC3:	//document jam
			WriteLog( "\tDocument JAM" );
			answer = SCAN_DOCJAM;
			break;

		case 0xC4:	//Cover Open
			WriteLog( "\tCover open" );
			answer = SCAN_COVER_OPEN;
			break;

		case 0xE5:	//
		case 0xE6:	//
		case 0xE7:	//
		default:	//service error
			WriteLog( "\tService Error\n" );
			answer = SCAN_SERVICE_ERR;
			break;
	}

	return answer;
}


//-----------------------------------------------------------------------------
//
//	Function name:	CnvResoNoToUserResoValue
//
//
//	Abstract:
//		Obtain the resolution value from the resolution type
//
//
//	Parameters:
//		nResoNo
//			the number of the resolution-type
//		pScanInfo
//			information of  scanning
//
//	Return values:
//		None
//
//-----------------------------------------------------------------------------
//
void
CnvResoNoToUserResoValue( LPRESOLUTION pUserSelect, WORD nResoNo )
{
	switch( nResoNo ){
		case RES100X100:	// 100 x 100 dpi
			pUserSelect->wResoX = 100;
			pUserSelect->wResoY = 100;
			break;

		case RES150X150:	// 150 x 150 dpi
			pUserSelect->wResoX = 150;
			pUserSelect->wResoY = 150;
			break;

		case RES200X100:	// 200 x 100 dpi
			pUserSelect->wResoX = 200;
			pUserSelect->wResoY = 100;
			break;

		case RES200X200:	// 200 x 200 dpi
			pUserSelect->wResoX = 200;
			pUserSelect->wResoY = 200;
			break;

		case RES200X400:	// 200 x 400 dpi
			pUserSelect->wResoX = 200;
			pUserSelect->wResoY = 400;
			break;

		case RES300X300:	// 300 x 300 dpi
			pUserSelect->wResoX = 300;
			pUserSelect->wResoY = 300;
			break;

		case RES400X400:	// 400 x 400 dpi
			pUserSelect->wResoX = 400;
			pUserSelect->wResoY = 400;
			break;

		case RES600X600:	// 600 x 600 dpi
			pUserSelect->wResoX = 600;
			pUserSelect->wResoY = 600;
			break;

		case RES800X800:	// 800 x 800 dpi
			pUserSelect->wResoX = 800;
			pUserSelect->wResoY = 800;
			break;

		case RES1200X1200:	// 1200 x 1200 dpi
			pUserSelect->wResoX = 1200;
			pUserSelect->wResoY = 1200;
			break;

		case RES2400X2400:	// 2400 x 2400 dpi
			pUserSelect->wResoX = 2400;
			pUserSelect->wResoY = 2400;
			break;

		case RES4800X4800:	// 4800 x 4800 dpi
			pUserSelect->wResoX = 4800;
			pUserSelect->wResoY = 4800;
			break;

		case RES9600X9600:	// 9600 x 9600 dpi
			pUserSelect->wResoX = 9600;
			pUserSelect->wResoY = 9600;
			break;
	}
}


//////// end of brother_scanner.c ////////
