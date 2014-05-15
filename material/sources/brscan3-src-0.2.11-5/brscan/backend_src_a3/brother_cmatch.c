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
//	Source filename: brother_cmatch.c
//
//		Copyright(c) 2005-2008 Brother Industries, Ltd.  
//                All Rights Reserved.
//
//
//	Abstract:
//			
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


#include <dlfcn.h>

#include "brother.h"
#include "brother_misc.h"
#include "brother_log.h"

#include "brother_cmatch.h"







//-----------------------------------------------------------------------------
//
//	Function name:	LoadGrayTable
//
//
//	Abstract:
//		GrayTable
//
//
//	Parameters:
//		
//
//
//	Return values:
//		TRUE  = 
//		FALSE = 
//
//-----------------------------------------------------------------------------
//	LoadGrayTable
BOOL
LoadGrayTable( Brother_Scanner *this, BYTE GrayTableNo )
{
#if  BRSANESUFFIX == 3             //M-LNX-111
  this->cmatch.hGrayTbl=NULL;
  return FALSE;
#else //BRSANESUFFIX == 3          //M-LNX-111
	char      szGrayBinPathName[ MAX_PATH ];
	FILE      *hGrayBinFile;
	LONG      lFilePtr;
	size_t       nReadSize = 0;
	LPBYTE    lpGrayTable;
	BOOL      bResult = FALSE;


	strcpy( szGrayBinPathName, BROTHER_SANE_DIR );
	strcat( szGrayBinPathName, BROTHER_GRAYCMDATA_DIR); 
	strcat( szGrayBinPathName, this->modelConfig.szGrayLebelName);

	if( GrayTableNo > 0 ){
		hGrayBinFile = fopen( szGrayBinPathName, "rb" );
		if( hGrayBinFile != NULL ){

			this->cmatch.hGrayTbl = MALLOC( 256 );
			if( this->cmatch.hGrayTbl != NULL ){

				lpGrayTable = (LPBYTE)this->cmatch.hGrayTbl;
				lFilePtr = ( GrayTableNo - 1 ) * 512;

				nReadSize = fread( lpGrayTable, 1, 256, hGrayBinFile );
				if( nReadSize == 256 ){
					bResult = TRUE;
				}else{
					nReadSize = 0;
				}
			}
			fclose( hGrayBinFile );

			if( nReadSize <= 0 ){
				if ( this->cmatch.hGrayTbl) {
					FREE( this->cmatch.hGrayTbl );
					this->cmatch.hGrayTbl=NULL;
				}
			}
		}
	}
	return bResult;
#endif //BRSANESUFFIX == 3          //M-LNX-111
}


//-----------------------------------------------------------------------------
//
//	Function name:	FreeGrayTable
//
//
//	Abstract:
//		GrayTable
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
FreeGrayTable( Brother_Scanner *this )
{
	if( this->cmatch.hGrayTbl != NULL ){
		//
		// GrayTable
		//
		FREE( this->cmatch.hGrayTbl );
		this->cmatch.hGrayTbl = NULL;
	}
}


//-----------------------------------------------------------------------------
//
//	Function name:	SetupGrayAdjust
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
//	SetupGrayAdjust
HANDLE
SetupGrayAdjust( Brother_Scanner *this )
{
	HANDLE   hGrayAdj;
	LPBYTE   lpGray;
	LPBYTE   lpGrayTbl;
	int      nBrightness;
	int      nContrast;
	LONG     lGrayVal;
	LONG     lIndex;

	hGrayAdj = MALLOC(256); 
	if( hGrayAdj ){
		lpGray = (LPBYTE)hGrayAdj;

		WriteLog( "malloc hGrayAdj, size = 256" );

		nBrightness = this->uiSetting.nBrightness;
		nContrast   = this->uiSetting.nContrast;

		if( this->cmatch.hGrayTbl == NULL ){
			//
			//
			//
			for( lIndex = 0; lIndex < 256; lIndex++ ){
				lGrayVal = (LONG)lIndex;
				if( nContrast ){
					lGrayVal = AdjustContrast( lGrayVal, nContrast );
				}
				if( nBrightness ){
					lGrayVal = AdjustBright( lGrayVal, nBrightness );
				}
				if( lGrayVal > 255 ){
					lGrayVal = 255;
				}else if( lGrayVal < 0 ){
					lGrayVal = 0;
				}
				*lpGray++ = (BYTE)lGrayVal;
			}
		}else{
			lpGrayTbl = (LPBYTE)this->cmatch.hGrayTbl;

			for( lIndex = 0; lIndex < 256; lIndex++ ){
				lGrayVal = (LONG)*lpGrayTbl++;
				if( nContrast ){
					lGrayVal = AdjustContrast( lGrayVal, nContrast );
				}
				if( nBrightness ){
					lGrayVal = AdjustBright( lGrayVal, nBrightness );
				}
				if( lGrayVal > 255 ){
					lGrayVal = 255;
				}else if( lGrayVal < 0 ){
					lGrayVal = 0;
				}
				*lpGray++ = (BYTE)lGrayVal;
			}
		}
	}else{
		//
		//
		//
		hGrayAdj = this->cmatch.hGrayTbl;
	}
	return hGrayAdj;
}


LONG
AdjustContrast( LONG lGrayVal, int sdc )
{
	LONG  lGamma;

	lGamma = (LONG)sdc;
	lGamma <<= 2;

	if( lGamma >= 0 ){
		lGrayVal = ( ( 255 * ( lGrayVal - 128 ) ) / ( 255 - lGamma ) ) + 128;
	}else{
		lGrayVal = ( ( ( 255 + lGamma ) * ( lGrayVal - 128 ) ) / 255 ) + 128;
	}
	return lGrayVal;
}


LONG
AdjustBright( LONG lGrayVal, int sdc )
{
	LONG  lGamma;

	lGamma = (LONG)sdc;
	lGamma <<= 1;
	lGrayVal += lGamma;

	return lGrayVal;
}


//////// end of DsCMatch.c ////////
