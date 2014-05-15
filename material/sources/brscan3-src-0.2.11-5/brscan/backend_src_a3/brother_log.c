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
//	Source filename: brother_log.c
//
//		Copyright(c) 2005-2008 Brother Industries, Ltd.  All Rights Reserved.
//
//
//	Abstract:
//		
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

#include "brother_dtype.h"

#include "brother_devaccs.h"
#include "brother_misc.h"
//#include "brother.h"

#include "brother_log.h"
#include "brother_modelinf.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
//
// 
//
static int  nLogFile = 1;
static int  nNotInit = 1;
static HANDLE  hLogFile = 0;

//M-LNX-104  #define MAX_PATH 256

#define BROTHER_SANE_DIR "/usr/local/Brother/sane/"


//-----------------------------------------------------------------------------
//
//	Function name:	WriteLogFileString
//
//
//	Abstract:
//		
//
//
//	Parameters:
//		lpszLogStr
//		
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//	WriteLogFileString° µÏLogString_file°À
void
WriteLogFileString( LPSTR lpszLogStr )
{
#if 1
  
  if (nNotInit == 1 ){
    char szIniFileName[MAX_PATH];
    strcpy( szIniFileName, BROTHER_SANE_DIR );
    strcat( szIniFileName, INIFILE_NAME );
    nLogFile = (BYTE)ReadInitFileInt(DRIVER_SECTION,
				     LOGFILE_KEY,
				     ERROR_INT,szIniFileName);
    if(nLogFile < BLOGFILE_NONE 
       && nLogFile > BLOGFILE_FILE2){	/* check value */
           nLogFile = 0;
    }

    if (nLogFile == BLOGFILE_FILE1){
      char  szLogFile[ MAX_PATH ];
      strcpy( szLogFile, LOGFILENAME );
      unlink( szLogFile );
    }
    nNotInit = 0;
  }
	if( hLogFile == 0 ){
		//
		// 
		//
		char  szLogFile[ MAX_PATH ];

		//
		// 
		//
		switch(nLogFile){
		case BLOGFILE_NONE:
		  hLogFile = NULL;
		  break;  
		case BLOGFILE_STDOUT:
		  hLogFile = stdout;
		  break;
		case BLOGFILE_STDERR:
		  hLogFile = stderr;
		  break;
	        case BLOGFILE_FILE1:
	        case BLOGFILE_FILE2:
		  strcpy( szLogFile, LOGFILENAME );
		  hLogFile = fopen(szLogFile,"a");
		}
    }


	if( hLogFile != NULL){
		//
		// 
		//
		char   szStrBuff[ LOGSTRMAXLEN ];
		DWORD  dwStrLen;

		time_t ltime;
		struct tm *sysTime;
		void *b_sysTime;
#if 1
		if(NULL != (sysTime = malloc(sizeof(struct tm))))
#else
		if(NULL != (sysTime = MALLOC(sizeof(struct tm))))
#endif
		{
			b_sysTime = sysTime;
			if( lpszLogStr != NULL ){
				//
				//
				//
	
				time(&ltime);
				sysTime = localtime(&ltime);

				//
				//
				//
	
				dwStrLen = sprintf(szStrBuff,
								"%02d:%02d:%02d.%03d  %s\n",
								sysTime->tm_hour,
								sysTime->tm_min,
								sysTime->tm_sec,
								(ltime%1000),
								lpszLogStr
							);
			}else{
	
				strcpy( szStrBuff, "\n" );
				dwStrLen = 2;
			}
#if 1
			free(b_sysTime);
#else
			FREE(b_sysTime);
#endif
		}
		//
		//
		//

		fwrite( szStrBuff, sizeof(char), dwStrLen, hLogFile);
	}
	CloseLogFile();
#endif
}


//-----------------------------------------------------------------------------
//
//	Function name:	WriteLog
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
//	OutputLogString° µÏLogString°À
void WriteLog( LPSTR first, ... )
{
#if 1
	if( nLogFile ){
		//
		// 
		//
		va_list  marker;
		char  szStrBuff[ LOGSTRMAXLEN ];

		va_start( marker, first );		//


		vsprintf( 						//
			(LPSTR)szStrBuff, 			//
			first, 						//
			marker 						//
		);

		va_end( marker );				//


		WriteLogFileString( (LPSTR)szStrBuff );
	}

	return;
#endif
}


//-----------------------------------------------------------------------------
//
//	Function name:	WriteLogScanCmd
//
//
//	Abstract:
//		
//
//
//	Parameters:
//		lpszId
//		
//
//		lpszCmd
//		
//
//
//	Return values:
//		
//
//-----------------------------------------------------------------------------
//	WriteLogScanCmd° µÏWriteLogBidiCmd°À
void
WriteLogScanCmd( LPSTR lpszId, LPSTR lpszCmd )
{
#if 1
	if( nLogFile ){
		//
		//
		//
		int   nCmdLen, i;
		char  szStrBuff[ LOGSTRMAXLEN ];
		LPSTR lpCmdStr;


		nCmdLen = strlen( lpszCmd );

		lpCmdStr = szStrBuff;


		strcpy( szStrBuff, "[" );
		lpCmdStr++;

		if( nCmdLen > 0 && *lpszCmd == 0x1B ){
			//
			//
			//

			strcat( szStrBuff, "ESC+" );
			nCmdLen--;
			lpszCmd++;
			lpCmdStr += 4;
		}
		for( i = 0 ; i < nCmdLen ; i++, lpszCmd++ ){
			if( *lpszCmd == '\n'){
				//
				//
				//
				*lpCmdStr++ = ' ';
			}else if( (BYTE)*lpszCmd == (BYTE)0x80 ){
				//
				// Scanner Command Terminator
				//
				break;
			}else if( ' ' <= *lpszCmd && *lpszCmd < 0x80 ){
				//
				// Printable
				//
				*lpCmdStr++ = *lpszCmd;
			}else{
				//
				//
				//
				*lpszCmd++ = '.';
			}
		}
		*lpCmdStr++ = ']';
		*lpCmdStr   = '\0';

		//
		//
		//
		WriteLog( "%s %s", lpszId, szStrBuff );
	}
#endif
}


//-----------------------------------------------------------------------------
//
//	Function name:	CloseLogFile
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
CloseLogFile( void )
{
#if 1
  switch(nLogFile){
  case BLOGFILE_NONE:
  case BLOGFILE_STDOUT:
  case BLOGFILE_STDERR:
    hLogFile = NULL;
    break;
  case BLOGFILE_FILE1:
  case BLOGFILE_FILE2:
    if( hLogFile != NULL){
      fclose( hLogFile );
      hLogFile = NULL;
    }
    return;
  }
#endif
}


//-----------------------------------------------------------------------------
//
//	Function name:	GetLogSwitch
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
GetLogSwitch( Brother_Scanner *this )
{
	nLogFile = this->modelConfig.bLogFile;

}



//////// end of brother_log.c ////////

