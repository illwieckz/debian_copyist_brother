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
//	Source filename: brother_mfccmd.h
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
#ifndef _BROTHER_MFCCMD_H_
#define _BROTHER_MFCCMD_H_


//
//
//
#define MFCMAXCMDLENGTH      256

#define MFCMD_QUERYDEVINFO   "\x1BQ\n\x80"
#define MFCMD_GETCOLORTABLE  "\x1BP\n\x80"
#define MFCMD_QUERYSCANINFO  "\x1BI\n"

#define MFCMD_STARTSCANNING  "\x1BX\n"
#define MFCMD_SCANNEXTPAGE   "\x1BX\n\x80"
#define MFCMD_CANCELSCAN     "\x1BR"

#define MFCMD_RESOLUTION     "R="

#define MFCMD_COLORTYPE      "M="
#define MFCMD_CTYPE_TEXT     "TEXT\n"
#define MFCMD_CTYPE_ERRDIF   "ERRDIF\n"
#define MFCMD_CTYPE_GRAY64   "GRAY64\n"
#define MFCMD_CTYPE_GRAY256  "GRAY256\n"
#define MFCMD_CTYPE_4BITC    "C16\n"
#define MFCMD_CTYPE_8BITC    "C256\n"
#define MFCMD_CTYPE_24BITC   "CGRAY\n"

#define MFCMD_COMPRESSION    "C="
#define MFCMD_COMP_NONE      "NONE\n"
#define MFCMD_COMP_MH        "MH\n"
#define MFCMD_COMP_PACKBITS  "RLENGTH\n"

#define MFCMD_BRIGHTNESS     "B="
#define MFCMD_CONTRAST       "N="

#define MFCMD_BUSINESS_OFF   "U=OFF\n"
#define MFCMD_BUSINESS_ON    "U=ON\n"

#define MFCMD_PHOTOMODE_OFF  "P=OFF\n"
#define MFCMD_PHOTOMODE_ON   "P=ON\n"

#define MFCMD_SCANNIGAREA    "A="

//06/02/27 Duplex Commands
#define MFCMD_SCANDUPLEX     "D=DUP\n"
#define MFCMD_SCANSINGLE     "D=SIN\n"

#define MFCMD_LF             "\n"
#define MFCMD_LFCHR          '\n'
#define MFCMD_SEPARATOR      ","
#define MFCMD_SEPARATORCHR   ','
#define MFCMD_TERMINATOR     "\x80"

//
//
//
extern BOOL  bTxCancelCmd;

//
//
//
void  SendCancelCommand( usb_dev_handle *hScanner,int seriesNo );
void  MakeupColorTypeCommand( WORD nColorType, LPSTR lpszColorCmd );
int   MakeupScanQueryCmd( Brother_Scanner *this, LPSTR lpszCmdStr );
int   MakeupScanStartCmd( Brother_Scanner *this, LPSTR lpszCmdStr );
void  MakePercentStr( int nPercent, LPSTR lpszStr );
void  MakeDotStr(int nPosition, LPSTR lpszStr, BOOL bSeparator );


#endif //_BROTHER_MFCCMD_H_


//////// end of brother_mfccmd.h ////////
