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
//	Source filename: brother_mfccmd.c
//
//		Copyright(c) 2005-2009 Brother Industries, Ltd.  
//                    All Rights Reserved.
//
//
//	Abstract:
//			
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#include <usb.h>

#include "brother.h"

#include "brother_devinfo.h"
#include "brcolor.h"
#include "brother_devaccs.h"
#include "brother_misc.h"
#include "brother_log.h"

#include "brother_mfccmd.h"


extern BOOL  bTxScanCmd;


//
// Cancel
//
BOOL  bTxCancelCmd = FALSE;


//-----------------------------------------------------------------------------
//
//	Function name:	SendCancelCommand
//
//
//	Abstract:
//		
//
//
//	Parameters:
//		
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//
void
SendCancelCommand( usb_dev_handle *hScanner, int seriesNo )
{
	if( bTxScanCmd ){
		//
		//
		//
		if( !bTxCancelCmd ){
			WriteDeviceCommand( hScanner, 
					 (LPBYTE)MFCMD_CANCELSCAN, //M-LNX-104 
				         strlen( MFCMD_CANCELSCAN ), 
				         seriesNo );
			bTxCancelCmd = TRUE;

			WriteLogScanCmd( "Send CANCEL command", MFCMD_CANCELSCAN );
		}else{
			WriteLog( "Already sending CANCEL command" );
		}
	}else{
		//
		//
		//
		WriteLog( "Not need to send CANCEL command" );
	}
}


//-----------------------------------------------------------------------------
//
//	Function name:	MakeupScanQueryCmd
//
//
//	Abstract:
//		I-Command
//
//
//	Parameters:
//		lpszCmdStr
//			
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//
int
MakeupScanQueryCmd( Brother_Scanner *this, LPSTR lpszCmdStr )
{
	char  szCmdStrTemp[ 16 ];


	//
	//
	//
	strcpy( lpszCmdStr, MFCMD_QUERYSCANINFO );

	//
	//
	//
	strcat( lpszCmdStr, MFCMD_RESOLUTION );

	//
	WordToStr( this->scanInfo.UserSelect.wResoX, szCmdStrTemp );
	strcat( lpszCmdStr, szCmdStrTemp );
	strcat( lpszCmdStr, MFCMD_SEPARATOR );

	//
	WordToStr( this->scanInfo.UserSelect.wResoY, szCmdStrTemp );
	strcat( lpszCmdStr, szCmdStrTemp );
	strcat( lpszCmdStr, MFCMD_LF );

	//
	//
	//
	MakeupColorTypeCommand( this->devScanInfo.wColorType, lpszCmdStr );

	//
	//
	//
	strcat( lpszCmdStr, (LPSTR)MFCMD_TERMINATOR );
	WriteLog("Resolution Command : %s",lpszCmdStr);
	return strlen( lpszCmdStr );
}


//-----------------------------------------------------------------------------
//
//	Function name:	MakeupScanStartCmd
//
//
//	Abstract:
//		X-Command
//
//
//	Parameters:
//		lpszCmdStr
//			
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//	MakeupScanStartCmd
int
MakeupScanStartCmd( Brother_Scanner *this, LPSTR lpszCmdStr )
{
	char  szCmdStrTemp[ 16 ];


	//
	//
	//
	strcpy( lpszCmdStr, MFCMD_STARTSCANNING );

	//
	//
	//
	strcat( lpszCmdStr, MFCMD_RESOLUTION );

	//
	WordToStr( this->devScanInfo.DeviceScan.wResoX, szCmdStrTemp );
	strcat( lpszCmdStr, szCmdStrTemp );
	strcat( lpszCmdStr, MFCMD_SEPARATOR );

	//
	WordToStr( this->devScanInfo.DeviceScan.wResoY, szCmdStrTemp );
	strcat( lpszCmdStr, szCmdStrTemp );
	strcat( lpszCmdStr, MFCMD_LF );

	//
	//
	//
	MakeupColorTypeCommand( this->devScanInfo.wColorType, lpszCmdStr );

	//
	//
	//
	strcat( lpszCmdStr, MFCMD_COMPRESSION );
	if( this->modelConfig.bCompressEnbale ){
		// PackBits
		strcat( lpszCmdStr, MFCMD_COMP_PACKBITS );
	}else{
		//
		strcat( lpszCmdStr, MFCMD_COMP_NONE );
	}

	//
	// Brightness
	//
	strcat( lpszCmdStr, MFCMD_BRIGHTNESS );
	MakePercentStr( this->uiSetting.nBrightness, szCmdStrTemp );
	strcat( lpszCmdStr, szCmdStrTemp );

	//
	// Contrast
	//
	strcat( lpszCmdStr, MFCMD_CONTRAST );
	MakePercentStr( this->uiSetting.nContrast, szCmdStrTemp );
	strcat( lpszCmdStr, szCmdStrTemp );

	//
	//
	//
	strcat( lpszCmdStr, MFCMD_BUSINESS_OFF );

	//
	// Photo
	//
	strcat( lpszCmdStr, MFCMD_PHOTOMODE_OFF );

	//
	//
	//


#if 0 // M-LNX-103	
	strcat( lpszCmdStr, MFCMD_SCANNIGAREA );
	MakeDotStr( this->devScanInfo.ScanAreaDot.left, szCmdStrTemp, TRUE );
	strcat( lpszCmdStr, szCmdStrTemp );
	MakeDotStr( this->devScanInfo.ScanAreaDot.top,  szCmdStrTemp, TRUE);
	strcat( lpszCmdStr, szCmdStrTemp );

	MakeDotStr( this->devScanInfo.ScanAreaDot.right,  szCmdStrTemp, TRUE );
	strcat( lpszCmdStr, szCmdStrTemp );
	MakeDotStr( this->devScanInfo.ScanAreaDot.bottom, szCmdStrTemp, FALSE );
	strcat( lpszCmdStr, szCmdStrTemp );
#else  //M-LNX-103
	if (this->uiSetting.nSrcAlign == SCANSRC_ADF_LFTALN){
	  strcat( lpszCmdStr, MFCMD_SCANNIGAREA );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.left, szCmdStrTemp, TRUE );
	  strcat( lpszCmdStr, szCmdStrTemp );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.top,  szCmdStrTemp, TRUE);
	  strcat( lpszCmdStr, szCmdStrTemp );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.right,  szCmdStrTemp, TRUE );
	  strcat( lpszCmdStr, szCmdStrTemp );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.bottom, szCmdStrTemp, FALSE );
	  strcat( lpszCmdStr, szCmdStrTemp );

	}
	else if (this->uiSetting.nSrcAlign == SCANSRC_ADF_CTRALN){
	  int x_err_rec,shift_x, max_width , src_width;

   #if 0
	  src_width = this->devScanInfo.ScanAreaDot.right 
	    - this->devScanInfo.ScanAreaDot.left;
	  max_width = (this->modelConfig.SupportScanAreaWidth) * 10 
	    * this->devScanInfo.DeviceScan.wResoX / 254;
	  shift_x = (max_width - src_width) /2;
   #else
	  // The left origin assume to be 0
	  src_width = this->devScanInfo.ScanAreaDot.right; 
	  max_width = (this->modelConfig.SupportScanAreaWidth) * 10 
	    * this->devScanInfo.DeviceScan.wResoX / 254;
	  shift_x = (max_width - src_width) /2;
   #endif

	  //M-LNX-108
	  x_err_rec = this->modelConfig.xshift_c *
	    this->devScanInfo.DeviceScan.wResoX / 72;

#if 0
	  if ( (x_err_rec >= 0) && (x_err_rec > shift_x) ){
	    //x_err_rec = shift_x ;
	    this->devScanInfo.ScanAreaDot.right -= (x_err_rec - shift_x);
	    src_width -= (x_err_rec - shift_x);
	  }
	  if ( (x_err_rec <  0) && (x_err_rec < (-shift_x)) ){
	    x_err_rec = (-shift_x) ;
	  }
#else
	  if ( x_err_rec > shift_x )x_err_rec = shift_x; 
	  if ( x_err_rec < -shift_x )x_err_rec = -shift_x;
#endif
	  shift_x += x_err_rec;
	  //M-LNX-108

	  strcat( lpszCmdStr, MFCMD_SCANNIGAREA );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.left + shift_x, 
		      szCmdStrTemp, TRUE );
	  strcat( lpszCmdStr, szCmdStrTemp );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.top,  
		      szCmdStrTemp, TRUE);
	  strcat( lpszCmdStr, szCmdStrTemp );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.right + shift_x,
		      szCmdStrTemp, TRUE );
	  strcat( lpszCmdStr, szCmdStrTemp );
	  MakeDotStr( this->devScanInfo.ScanAreaDot.bottom, 
		      szCmdStrTemp, FALSE );
	  strcat( lpszCmdStr, szCmdStrTemp );
	}
	else{
	  // not support
	}

#endif  //M-LNX-103
	//06/02/28
	//Duplex command
	//
	if(this->modelConfig.SupportScanSrc.bit.ADF_DUP){
	  if(this->uiSetting.nSrcType ==  SCANSRC_ADF_DUP ||  //M-LNX-133
	     this->uiSetting.nSrcType ==  SCANSRC_ADF_DUPC)   //M-LNX-133
	    strcat(lpszCmdStr, MFCMD_SCANDUPLEX);
	  else
	    strcat(lpszCmdStr, MFCMD_SCANSINGLE);
	}

	//
	//
	//
	strcat( lpszCmdStr, (LPSTR)MFCMD_TERMINATOR );

	return strlen( lpszCmdStr );
}


//-----------------------------------------------------------------------------
//
//	Function name:	MakeupColorTypeCommand
//
//
//	Abstract:
//		
//
//
//	Parameters:
//		nColorType
//		
//
//		lpszColorCmd
//		
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//
void
MakeupColorTypeCommand( WORD nColorType, LPSTR lpszColorCmd )
{
	strcat( lpszColorCmd, MFCMD_COLORTYPE );

	switch( nColorType ){
		case COLOR_BW:
			strcat( lpszColorCmd, MFCMD_CTYPE_TEXT );
			break;

		case COLOR_ED:
			strcat( lpszColorCmd, MFCMD_CTYPE_ERRDIF );
			break;

		case COLOR_TG:
			strcat( lpszColorCmd, MFCMD_CTYPE_GRAY64 );
			break;

		case COLOR_256:
			strcat( lpszColorCmd, MFCMD_CTYPE_8BITC );
			break;

		case COLOR_FUL:
			strcat( lpszColorCmd, MFCMD_CTYPE_24BITC );
			break;

		case COLOR_FUL_NOCM:
			strcat( lpszColorCmd, MFCMD_CTYPE_24BITC );
			break;
	}
}


//-----------------------------------------------------------------------------
//
//	Function name:	MakePercentStr
//
//
//	Abstract:
//		Brightness/Contrastのパラメータ文字列を生成する
//
//
//	Parameters:
//		nPercent
//			Brightness/Contrastパラメータ
//
//		lpszStr
//		
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//	MakePercentStr（旧MakeContrastStr）
void
MakePercentStr( int nPercent, LPSTR lpszStr )
{
	int  nLength;


	//
	// 
	//
	nPercent += 50;
	if( nPercent < 0 ){
		nPercent = 0;
	}else if( nPercent > 100 ){
		nPercent = 100;
	}
	//
	//
	//
	WordToStr( (WORD)nPercent, lpszStr );
	nLength = strlen( lpszStr );
	lpszStr += nLength;
	*lpszStr++ = MFCMD_LFCHR;
	*lpszStr   = '\0';
}


//-----------------------------------------------------------------------------
//
//	Function name:	MakeDotStr
//
//
//	Abstract:
//		
//
//
//	Parameters:
//		nPosition
//		
//
//		lpszStr
//		
//
//		bSeparator
//		
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//
void
MakeDotStr( int nPosition, LPSTR lpszStr, BOOL bSeparator )
{
	int  nLength;


	WordToStr( (WORD)nPosition, lpszStr );
	nLength = strlen( lpszStr );
	lpszStr += nLength;
	if( bSeparator ){
		*lpszStr++ = MFCMD_SEPARATORCHR;
	}else{
		*lpszStr++ = MFCMD_LFCHR;
	}
	*lpszStr   = '\0';
}


//////// end of brother_mfccmd.c ////////
