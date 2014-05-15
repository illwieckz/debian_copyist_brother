//* This program is free software; you can redistribute it and/or modify it
//* under the terms of the GNU General Public License as published by the Free
//* Software Foundation; either version 2 of the License, or (at your option)
//* any later version.
//*
//* This program is distributed in the hope that it will be useful, but WITHOUT
//* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
//* more details.
//*
//* You should have received a copy of the GNU General Public License along with
//* this program; if not, write to the Free Software Foundation, Inc., 59 Temple
//* Place, Suite 330, Boston, MA  02111-1307  USA
//*




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
//	Source filename: brother_devaccs.h
//
//		Copyright(c) 2006-2006 Brother Industries, Ltd.  
//               All Rights Reserved.
//
//
//	Abstract:
//		
//
//
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



#define  INVALIDPARAMETER   "Invalid parameter"
#define  INVALIDPARAMETERN  "Invalid parameter %d\n"
#define  INVALIDMODELNAME   "Invalid model name"
#define  INVALIDFNAME       "Invalid name"
#define  INVALIDFNAME2      "\"%s\" is already registered.\n"
#define  INVALIDADDRESS     "Invalid IP address or NODENAME"
#define  INVALIDADDRESS2    "Invalid IP address (%s)\n"




char *HELP[]={
  "USAGE: brsaneconfig%s [-OPTION]",
  "   OPTION:",
#ifdef SUPPORT_INTERACTIVE
  "       -a          : Add network scanner (Interactive)",
#endif //SUPPORT_INTERACTIVE
  "       -a name=FRIENDLY-NAME model=MODEL-NAME ip=xx.xx.xx.xx    ",
  "       -a name=FRIENDLY-NAME model=MODEL-NAME nodename=BRN_xxxxx ",
  "                   : Add network scanner",
  "       -r FRIENDLY-NAME [FRIENDLY-NAME ...]",
  "                   : Remove network scanner",
  "       -q          : Query supported models and available network scanners",
  "       -d          : Diagnosis",
  "       -p          : Ping (for network scanners)  ",
  "       -s:[LABEL]  : Save current configuration",
  "       -l:[LABEL]  : Load saved configuration",
  NULL
};

