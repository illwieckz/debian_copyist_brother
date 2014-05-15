/* $Id: brcups_commands.h,v 1.11 2005/08/22 02:20:49 cvs Exp $ */
//* Brother CUPS wrapper tool
//* Copyright (C) 2005 Brother. Industries, Ltd.//*
//*                                    Ver1.00
//*
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

#ifndef _BRCUPS_COMMANDS_H_
#define _BRCUPS_COMMANDS_H_




char brprintconf[30]="brprintconf_";

		
typedef struct {
    char *option;
    char *value;
} CMDLINELIST;
//   +++++++++++++++++++++++++++

//       command line   (Brother command)
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist[] = {
		// for OpenOffice
		{"papersize=A4"			,	"-Paper A4"},
		{"papersize=Letter"			,	"-Paper Letter"},
		{"papersize=Legal"			,	"-Paper Legal"},
		{"papersize=Executive"		,	"-Paper Executive"},
		{"papersize=A5"			,	"-Paper A5"},
		{"papersize=A6"			,	"-Paper A6"},
		{"papersize=ISOB5"			,	"-Paper ISOB5"},
		{"papersize=ISOB6"			,	"-Paper ISOB6"},
		{"papersize=B5"			,	"-Paper B5"},
		{"papersize=EnvDL"			,	"-Paper EnvDL"},
		{"papersize=EnvC5"			,	"-Paper EnvC5"},
		{"papersize=Env10"			,	"-Paper Env10"},
		{"papersize=EnvMonarch"		,	"-Paper EnvMonarch"},
		{"papersize=BrOrgJ"			,	"-Paper BrOrgJ"},
		{"papersize=BrOrgK"			,	"-Paper BrOrgK"},
		{"papersize=Statement"		,	"-Paper Statement"},
		{"papersize=Br3x5"			,	"-Paper Br3x5"},
		{"papersize=BrA4Long"		,	"-Paper BrA4Long"},
		{"papersize=FanFoldGermanLegal"	,	"-Paper FanFoldGermanLegal"},
		{"papersize=EnvPRC5Rotated"		,	"-Paper EnvPRC5Rotated"},
		{"papersize=PostCard"		,	"-Paper PostCard"},

		// paper size
		{"PageSize=A4"			,	"-Paper A4"},
		{"PageSize=Letter"			,	"-Paper Letter"},
		{"PageSize=Legal"			,	"-Paper Legal"},
		{"PageSize=Executive"		,	"-Paper Executive"},
		{"PageSize=A5"			,	"-Paper A5"},
		{"PageSize=A6"			,	"-Paper A6"},
		{"PageSize=ISOB5"			,	"-Paper ISOB5"},
		{"PageSize=ISOB6"			,	"-Paper ISOB6"},
		{"PageSize=B5"			,	"-Paper B5"},
		{"PageSize=EnvDL"			,	"-Paper EnvDL"},
		{"PageSize=EnvC5"			,	"-Paper EnvC5"},
		{"PageSize=Env10"			,	"-Paper Env10"},
		{"PageSize=EnvMonarch"		,	"-Paper EnvMonarch"},
		{"PageSize=BrOrgJ"			,	"-Paper BrOrgJ"},
		{"PageSize=BrOrgK"			,	"-Paper BrOrgK"},
		{"PageSize=Statement"		,	"-Paper Statement"},
		{"PageSize=Br3x5"			,	"-Paper Br3x5"},
		{"PageSize=BrA4Long"		,	"-Paper BrA4Long"},
		{"PageSize=FanFoldGermanLegal"	,	"-Paper FanFoldGermanLegal"},
		{"PageSize=EnvPRC5Rotated"		,	"-Paper EnvPRC5Rotated"},
		{"PageSize=PostCard"		,	"-Paper PostCard"},


	   // Input slot
	   {  "InputSlot=AutoSelect"   ,   "-InputSlot AutoSelect"       },
	   {  "InputSlot=Tray1"   ,   "-InputSlot Tray1"       },
	   {  "InputSlot=Tray2"   ,   "-InputSlot Tray2"       },
	   {  "InputSlot=MPTray"   ,   "-InputSlot MPTray"   },
   	   {  "InputSlot=Manual"   ,   "-InputSlot Manual"   },
          
		// resolution
	   {  "BRResolution=Normal"		,   "-BRResolution Normal"       	},
	   {  "BRResolution=CAPT"		,   "-BRResolution CAPT"       	},
                                          
	   	   
	   // MonoColor
	   {  "BRMonoColor=Auto"   ,       		"-BRMonoColor Auto"      		},
	   {  "BRMonoColor=FullColor"   ,		"-BRMonoColor FullColor"		},
	   {  "BRMonoColor=Mono"   ,  		"-BRMonoColor Mono"		},

	   // Media type
	   {  "MediaType=Plain"		,   "-MediaType Plain"     },
	   {  "MediaType=Thin"		,   "-MediaType Thin"      },
	   {  "MediaType=Thick"		,   "-MediaType Thick"     },
	   {  "MediaType=Thicker"	,   "-MediaType Thicker"   },
	   {  "MediaType=BOND"		,   "-MediaType BOND"      },
   	   {  "MediaType=Recycled"	,   "-MediaType Recycled"  },
	   {  "MediaType=Env"		,   "-MediaType Env"       },
	   {  "MediaType=EnvThin"	,   "-MediaType EnvThin"   },
	   {  "MediaType=EnvThick"	,   "-MediaType EnvThick"  },
	   {  "MediaType=PostCard"	,   "-MediaType PostCard"  },
   	   {  "MediaType=Label"		,   "-MediaType Label"     },

       // Color Matching
	   {  "BRColorMatching=MatchMonitor"	,   "-BRColorMatching MatchMonitor"	},
	   {  "BRColorMatching=VividText"		,   "-BRColorMatching VividText"},
	   {  "BRColorMatching=None"			,   "-BRColorMatching None"       	},

	   // Gray
   	   {  "BRGray=OFF"		,   "-BRGray OFF"	},
	   {  "BRGray=ON"		,   "-BRGray ON"  },

   	   // Enhance Black
   	   {  "BREnhanceBlkPrt=OFF"		,   "-BREnhanceBlkPrt OFF"	},
   	   {  "BREnhanceBlkPrt=ON"		,   "-BREnhanceBlkPrt ON"	},

       // Mirror
   	   {  "BRMirror=OFF"		,   "-BRMirror OFF"	},
   	   {  "BRMirror=ON"			,   "-BRMirror ON"	},

       // TonerSaveMode
   	   {  "TonerSaveMode=OFF"		,   "-TonerSaveMode OFF"	},
   	   {  "TonerSaveMode=ON"		,   "-TonerSaveMode ON"		},

		// BRReverse
   	   {  "BRReverse=OFF"		,   "-BRReverse OFF"	},
   	   {  "BRReverse=ON"		,   "-BRReverse ON"	},

       // Duplex
       {  "BRDuplex=None"				,	"-Duplex None"  },
       {  "BRDuplex=DuplexTumble"		,	"-Duplex DuplexTumble"  },
       {  "BRDuplex=DuplexNoTumble"		,	"-Duplex DuplexNoTumble"  },

       // Collate
       {  "Collate=OFF"		,	"-Collate OFF"  },
       {  "Collate=ON"		,	"-Collate ON"  },
       
	   { NULL   ,   NULL    },
};



//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_cups_commandlinelist[] = {
	// for Fedora OpenOffice
		{"media=A4"			,	"-Paper A4"},
		{"media=Letter"			,	"-Paper Letter"},
		{"media=Legal"			,	"-Paper Legal"},
		{"media=Executive"		,	"-Paper Executive"},
		{"media=A5"			,	"-Paper A5"},
		{"media=A6"			,	"-Paper A6"},
		{"media=ISOB5"			,	"-Paper ISOB5"},
		{"media=ISOB6"			,	"-Paper ISOB6"},
		{"media=B5"			,	"-Paper B5"},
		{"media=EnvDL"			,	"-Paper EnvDL"},
		{"media=EnvC5"			,	"-Paper EnvC5"},
		{"media=Env10"			,	"-Paper Env10"},
		{"media=EnvMonarch"		,	"-Paper EnvMonarch"},
		{"media=BrOrgJ"			,	"-Paper BrOrgJ"},
		{"media=BrOrgK"			,	"-Paper BrOrgK"},
		{"media=Statement"		,	"-Paper Statement"},
		{"media=Br3x5"			,	"-Paper Br3x5"},
		{"media=BrA4Long"		,	"-Paper BrA4Long"},
		{"media=FanFoldGermanLegal"	,	"-Paper FanFoldGermanLegal"},
		{"media=EnvPRC5Rotated"		,	"-Paper EnvPRC5Rotated"},
		{"media=PostCard"		,	"-Paper PostCard"},

		{ NULL   ,   NULL    },
};


//   +++++++++++++++++++++++++++
//       command line numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	   {	"Copies="   	,   "-Copies "      },
	   {	"BRBrightness="	,	"-BRBrightness "	},
	   {	"BRContrast="	,	"-BRContrast "		},
	   {	"BRRed="		,	"-BRRed "		},
	   {	"BRGreen="		,	"-BRGreen "	},
	   {	"BRBlue="		,	"-BRBlue "	},
	   {	"BRSaturation="	,	"-BRSaturation "},
	   { NULL   ,   NULL    },
};
/*      PPD Default   */
//   +++++++++++++++++++++++++++
//       PPD setting list

//   +++++++++++++++++++++++++++



typedef struct {
    char *value;
    char *brcommand;
} PPDCOMMANDLIST;


typedef   struct   {
   char             *label;
   PPDCOMMANDLIST   ppdcommandlist[];
}  PPDCOMMAND;

//       ***** DefaultResolution *****
PPDCOMMAND  DefaultResolution={
  "DefaultBRResolution",
  {
  		{ "Normal"  , "-BRResolution Normal"  },
    	{ "CAPT"    , "-BRResolution CAPT"  },
 	    { NULL , NULL  }
  }
};

//       ***** DefaultPageSize *****
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    {
	   {"A4"				,	"-Paper A4"},
       {"Letter"			,	"-Paper Letter"},
       {"Legal"				,	"-Paper Legal"},
       {"Executive"			,	"-Paper Executive"},
       {"A5"				,	"-Paper A5"},
       {"A6"				,	"-Paper A6"},
       {"ISOB5"				,	"-Paper ISOB5"},
       {"ISOB6"				,	"-Paper ISOB6"},
       {"B5"				,	"-Paper B5"},
       {"EnvDL"				,	"-Paper EnvDL"},
       {"EnvC5"				,	"-Paper EnvC5"},
       {"Env10"				,	"-Paper Env10"},
       {"EnvMonarch"		,	"-Paper EnvMonarch"},
       {"BrOrgJ"			,	"-Paper BrOrgJ"},
       {"BrOrgK"			,	"-Paper BrOrgK"},
       {"Statement"			,	"-Paper Statement"},
       {"Br3x5"				,	"-Paper Br3x5"},
       {"BrA4Long"			,	"-Paper BrA4Long"},
	   {"FanFoldGermanLegal",	"-Paper FanFoldGermanLegal"},
	   {"EnvPRC5Rotated"	,	"-Paper EnvPRC5Rotated"},
       {"PostCard"			,	"-Paper PostCard"},
       { NULL , NULL  }     
     }
};
//       ***** DefaultInputSlot *****
PPDCOMMAND  DefaultInputSlot={
    "DefaultInputSlot",
    {
 	    { "Tray1" 		, "-InputSlot Tray1"  },
        { "Tray2" 		, "-InputSlot Tray2"  },
 	    { "MPTray" 		, "-InputSlot MPTray"  },
 	    { "Manual" 		, "-InputSlot Manual"  },
 	    { "AutoSelect" 	, "-InputSlot AutoSelect"  },
 	    { NULL , NULL  }
     }
};

// 		CAPT
PPDCOMMAND  DefaultBRCAPT={
	"DefaultBRCAPT",
	{
		{	NULL, NULL	}
	}	
};

// Media Type
PPDCOMMAND  DefaultMediaType={
    "DefaultMediaType",
    {
	   {  "Plain"		,   "-MediaType Plain"       },
	   {  "Thin"		,   "-MediaType Thin"       },
	   {  "Thick"		,   "-MediaType Thick"       },
	   {  "Thicker"		,   "-MediaType Thicker"       },
	   {  "BOND"		,   "-MediaType BOND"       },
   	   {  "Recycled"	,   "-MediaType Recycled"       },
   	   {  "Env"			,   "-MediaType Env"       },
   	   {  "EnvThin"		,   "-MediaType EnvThin"       },
   	   {  "EnvThick"	,   "-MediaType EnvThick"       },
   	   {  "PostCard"	,   "-MediaType PostCard"       },
   	   {  "Label"		,   "-MediaType Label"       },		
       { NULL , NULL  }
     }
};

/*
//       ***** DefaultBRColorMatching*****
PPDCOMMAND  DefaultBRColorMatching={
    "DefaultBRColorMatching",
    {
 	    { "MatchMonitor",	"-cm MATCHMONITOR"  },
 	    { "VividText",		"-cm VIVID"  },
 	    { "None",			"-cm NONE"  },
 	    { NULL , NULL  }
     }
};
*/

//       ***** DefaultBRColorMatching*****
PPDCOMMAND  DefaultBRColorMatching={
    "DefaultBRColorMatching",
    {
 	    { "MatchMonitor",	"-BRColorMatching MatchMonitor"  },
 	    { "VividText",		"-BRColorMatching VividText"  },  
 	    { "None",			"-BRColorMatching None"  },            
 	    { NULL , NULL  }
     }
};


//       ***** DefaultBRGray*****
PPDCOMMAND  DefaultBRGray={
    "DefaultBRGray",
    {
 	    { "OFF", "-BRGray OFF"  },
 	    { "ON" , "-BRGray ON"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBREnhanceBlkPrt*****
PPDCOMMAND  DefaultBREnhanceBlkPrt={
    "DefaultBREnhanceBlkPrt",
    {
 	    { "OFF", "-BREnhanceBlkPrt OFF"  },
 	    { "ON" , "-BREnhanceBlkPrt ON"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBRMirror*****
PPDCOMMAND  DefaultBRMirror={
    "DefaultBRMirror",
    {
 	    { "OFF", "-BRMirror OFF"  },
 	    { "ON" , "-BRMirror ON"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultTonerSaveMode *****
PPDCOMMAND  DefaultTonerSaveMode={
    "DefaultTonerSaveMode",
    {
 	    { "OFF" , "-TonerSaveMode OFF"  },
 	    { "ON" ,  "-TonerSaveMode ON"  },
 	    { NULL , NULL  }

     }
};

//       ***** DefaultMonoColor *****
PPDCOMMAND  DefaultMonoColor={
    "DefaultBRMonoColor",
    {
		{  "Auto"  ,	"-BRMonoColor Auto"      		},
		{  "FullColor"   	,	"-BRMonoColor FullColor"		},
		{  "Mono"   		,	"-BRMonoColor Mono"	},
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBRSaturation *****
PPDCOMMAND  DefaultBRSaturation={
    "DefaultBRSaturation",
    {
 	    { "-1" , "-BRSaturation \\-1"  },
 	    { "-2" , "-BRSaturation \\-2"  },
 	    { "-3" , "-BRSaturation \\-3"  },
 	    { "-4" , "-BRSaturation \\-4"  },
 	    { "-5" , "-BRSaturation \\-5"  },
 	    { "-6" , "-BRSaturation \\-6"  },
 	    { "-7" , "-BRSaturation \\-7"  },
 	    { "-8" , "-BRSaturation \\-8"  },
 	    { "-9" , "-BRSaturation \\-9"  },
 	    { "-10" , "-BRSaturation \\-10"  },
 	    { "-11" , "-BRSaturation \\-11"  },
 	    { "-12" , "-BRSaturation \\-12"  },
 	    { "-13" , "-BRSaturation \\-13"  },
 	    { "-14" , "-BRSaturation \\-14"  },
 	    { "-15" , "-BRSaturation \\-15"  },
 	    { "-16" , "-BRSaturation \\-16"  },
 	    { "-17" , "-BRSaturation \\-17"  },
 	    { "-18" , "-BRSaturation \\-18"  },
 	    { "-19" , "-BRSaturation \\-19"  },
 	    { "-20" , "-BRSaturation \\-20"  },
 	    { "0" , "-BRSaturation 0"  },
 	    { "1" , "-BRSaturation 1"  },
 	    { "2" , "-BRSaturation 2"  },
 	    { "3" , "-BRSaturation 3"  },
 	    { "4" , "-BRSaturation 4"  },
 	    { "5" , "-BRSaturation 5"  },
 	    { "6" , "-BRSaturation 6"  },
 	    { "7" , "-BRSaturation 7"  },
 	    { "8" , "-BRSaturation 8"  },
 	    { "9" , "-BRSaturation 9"  },

 	    { "10" , "-BRSaturation 10"  },
 	    { "11" , "-BRSaturation 11"  },
 	    { "12" , "-BRSaturation 12"  },
 	    { "13" , "-BRSaturation 13"  },
 	    { "14" , "-BRSaturation 14"  },
 	    { "15" , "-BRSaturation 15"  },
 	    { "16" , "-BRSaturation 16"  },
 	    { "17" , "-BRSaturation 17"  },
 	    { "18" , "-BRSaturation 18"  },
 	    { "19" , "-BRSaturation 19"  },
 	    { "20" , "-BRSaturation 20"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBRBlue *****
PPDCOMMAND  DefaultBRBlue={
    "DefaultBRBlue",
    {
 	    { "-1" , "-BRBlue \\-1"  },
 	    { "-2" , "-BRBlue \\-2"  },
 	    { "-3" , "-BRBlue \\-3"  },
 	    { "-4" , "-BRBlue \\-4"  },
 	    { "-5" , "-BRBlue \\-5"  },
 	    { "-6" , "-BRBlue \\-6"  },
 	    { "-7" , "-BRBlue \\-7"  },
 	    { "-8" , "-BRBlue \\-8"  },
 	    { "-9" , "-BRBlue \\-9"  },
 	    { "-10" , "-BRBlue \\-10"  },
 	    { "-11" , "-BRBlue \\-11"  },
 	    { "-12" , "-BRBlue \\-12"  },
 	    { "-13" , "-BRBlue \\-13"  },

 	    { "-14" , "-BRBlue \\-14"  },
 	    { "-15" , "-BRBlue \\-15"  },
 	    { "-16" , "-BRBlue \\-16"  },
 	    { "-17" , "-BRBlue \\-17"  },
 	    { "-18" , "-BRBlue \\-18"  },
 	    { "-19" , "-BRBlue \\-19"  },
 	    { "-20" , "-BRBlue \\-20"  },
 	    { "0" , "-BRBlue 0"  },
 	    { "1" , "-BRBlue 1"  },
 	    { "2" , "-BRBlue 2"  },
 	    { "3" , "-BRBlue 3"  },
 	    { "4" , "-BRBlue 4"  },
 	    { "5" , "-BRBlue 5"  },
 	    { "6" , "-BRBlue 6"  },
 	    { "7" , "-BRBlue 7"  },
 	    { "8" , "-BRBlue 8"  },
 	    { "9" , "-BRBlue 9"  },
 	    { "10" , "-BRBlue 10"  },
 	    { "11" , "-BRBlue 11"  },
 	    { "12" , "-BRBlue 12"  },
 	    { "13" , "-BRBlue 13"  },
 	    { "14" , "-BRBlue 14"  },
 	    { "15" , "-BRBlue 15"  },
 	    { "16" , "-BRBlue 16"  },
 	    { "17" , "-BRBlue 17"  },
 	    { "18" , "-BRBlue 18"  },
 	    { "19" , "-BRBlue 19"  },
 	    { "20" , "-BRBlue 20"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBRGreen *****
PPDCOMMAND  DefaultBRGreen={
    "DefaultBRGreen",
    {
 	    { "-1" , "-BRGreen \\-1"  },

 	    { "-2" , "-BRGreen \\-2"  },
 	    { "-3" , "-BRGreen \\-3"  },
 	    { "-4" , "-BRGreen \\-4"  },
 	    { "-5" , "-BRGreen \\-5"  },
 	    { "-6" , "-BRGreen \\-6"  },
 	    { "-7" , "-BRGreen \\-7"  },
 	    { "-8" , "-BRGreen \\-8"  },
 	    { "-9" , "-BRGreen \\-9"  },
 	    { "-10" , "-BRGreen \\-10"  },
 	    { "-11" , "-BRGreen \\-11"  },
 	    { "-12" , "-BRGreen \\-12"  },
 	    { "-13" , "-BRGreen \\-13"  },
 	    { "-14" , "-BRGreen \\-14"  },
 	    { "-15" , "-BRGreen \\-15"  },
 	    { "-16" , "-BRGreen \\-16"  },
 	    { "-17" , "-BRGreen \\-17"  },
 	    { "-18" , "-BRGreen \\-18"  },
 	    { "-19" , "-BRGreen \\-19"  },
 	    { "-20" , "-BRGreen \\-20"  },
 	    { "0" , "-BRGreen 0"  },
 	    { "1" , "-BRGreen 1"  },

 	    { "2" , "-BRGreen 2"  },
 	    { "3" , "-BRGreen 3"  },
 	    { "4" , "-BRGreen 4"  },
 	    { "5" , "-BRGreen 5"  },
 	    { "6" , "-BRGreen 6"  },

 	    { "7" , "-BRGreen 7"  },
 	    { "8" , "-BRGreen 8"  },
 	    { "9" , "-BRGreen 9"  },
 	    { "10" , "-BRGreen 10"  },
 	    { "11" , "-BRGreen 11"  },
 	    { "12" , "-BRGreen 12"  },
 	    { "13" , "-BRGreen 13"  },
 	    { "14" , "-BRGreen 14"  },
 	    { "15" , "-BRGreen 15"  },
 	    { "16" , "-BRGreen 16"  },
 	    { "17" , "-BRGreen 17"  },
 	    { "18" , "-BRGreen 18"  },
 	    { "19" , "-BRGreen 19"  },
 	    { "20" , "-BRGreen 20"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBRRed *****
PPDCOMMAND  DefaultBRRed={
    "DefaultBRRed",
    {
 	    { "-1" , "-BRRed \\-1"  },
 	    { "-2" , "-BRRed \\-2"  },
 	    { "-3" , "-BRRed \\-3"  },
 	    { "-4" , "-BRRed \\-4"  },
 	    { "-5" , "-BRRed \\-5"  },
 	    { "-6" , "-BRRed \\-6"  },
 	    { "-7" , "-BRRed \\-7"  },
 	    { "-8" , "-BRRed \\-8"  },
 	    { "-9" , "-BRRed \\-9"  },
 	    { "-10" , "-BRRed \\-10"  },
 	    { "-11" , "-BRRed \\-11"  },
 	    { "-12" , "-BRRed \\-12"  },
 	    { "-13" , "-BRRed \\-13"  },
 	    { "-14" , "-BRRed \\-14"  },
 	    { "-15" , "-BRRed \\-15"  },

 	    { "-16" , "-BRRed \\-16"  },
 	    { "-17" , "-BRRed \\-17"  },
 	    { "-18" , "-BRRed \\-18"  },
 	    { "-19" , "-BRRed \\-19"  },
 	    { "-20" , "-BRRed \\-20"  },
 	    { "0" , "-BRRed 0"  },
 	    { "1" , "-BRRed 1"  },
 	    { "2" , "-BRRed 2"  },
 	    { "3" , "-BRRed 3"  },
 	    { "4" , "-BRRed 4"  },
 	    { "5" , "-BRRed 5"  },
 	    { "6" , "-BRRed 6"  },
 	    { "7" , "-BRRed 7"  },
 	    { "8" , "-BRRed 8"  },
 	    { "9" , "-BRRed 9"  },
 	    { "10" , "-BRRed 10"  },
 	    { "11" , "-BRRed 11"  },
 	    { "12" , "-BRRed 12"  },
 	    { "13" , "-BRRed 13"  },
 	    { "14" , "-BRRed 14"  },
 	    { "15" , "-BRRed 15"  },
 	    { "16" , "-BRRed 16"  },
 	    { "17" , "-BRRed 17"  },
 	    { "18" , "-BRRed 18"  },
 	    { "19" , "-BRRed 19"  },
 	    { "20" , "-BRRed 20"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBRContrast *****
PPDCOMMAND  DefaultBRContrast={
    "DefaultBRContrast",
    {
 	    { "-1" , "-BRContrast \\-1"  },
 	    { "-2" , "-BRContrast \\-2"  },
 	    { "-3" , "-BRContrast \\-3"  },
 	    { "-4" , "-BRContrast \\-4"  },
 	    { "-5" , "-BRContrast \\-5"  },
 	    { "-6" , "-BRContrast \\-6"  },
 	    { "-7" , "-BRContrast \\-7"  },
 	    { "-8" , "-BRContrast \\-8"  },
 	    { "-9" , "-BRContrast \\-9"  },
 	    { "-10" , "-BRContrast \\-10"  },
 	    { "-11" , "-BRContrast \\-11"  },
 	    { "-12" , "-BRContrast \\-12"  },
 	    { "-13" , "-BRContrast \\-13"  },
 	    { "-14" , "-BRContrast \\-14"  },

 	    { "-15" , "-BRContrast \\-15"  },
 	    { "-16" , "-BRContrast \\-16"  },
 	    { "-17" , "-BRContrast \\-17"  },
 	    { "-18" , "-BRContrast \\-18"  },
 	    { "-19" , "-BRContrast \\-19"  },
 	    { "-20" , "-BRContrast \\-20"  },
 	    { "0" , "-BRContrast 0"  },
 	    { "1" , "-BRContrast 1"  },
 	    { "2" , "-BRContrast 2"  },
 	    { "3" , "-BRContrast 3"  },
 	    { "4" , "-BRContrast 4"  },
 	    { "5" , "-BRContrast 5"  },
 	    { "6" , "-BRContrast 6"  },
 	    { "7" , "-BRContrast 7"  },
 	    { "8" , "-BRContrast 8"  },
 	    { "9" , "-BRContrast 9"  },
 	    { "10" , "-BRContrast 10"  },
 	    { "11" , "-BRContrast 11"  },
 	    { "12" , "-BRContrast 12"  },
 	    { "13" , "-BRContrast 13"  },
 	    { "14" , "-BRContrast 14"  },
 	    { "15" , "-BRContrast 15"  },
 	    { "16" , "-BRContrast 16"  },
 	    { "17" , "-BRContrast 17"  },
 	    { "18" , "-BRContrast 18"  },

 	    { "19" , "-BRContrast 19"  },

 	    { "20" , "-BRContrast 20"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRBrightness={
    "DefaultBRBrightness",
    {
 	    { "-1" , "-BRBrightness \\-1"  },
 	    { "-2" , "-BRBrightness \\-2"  },
 	    { "-3" , "-BRBrightness \\-3"  },
 	    { "-4" , "-BRBrightness \\-4"  },
 	    { "-5" , "-BRBrightness \\-5"  },

 	    { "-6" , "-BRBrightness \\-6"  },
 	    { "-7" , "-BRBrightness \\-7"  },
 	    { "-8" , "-BRBrightness \\-8"  },
 	    { "-9" , "-BRBrightness \\-9"  },
 	    { "-10" , "-BRBrightness \\-10"  },
 	    { "-11" , "-BRBrightness \\-11"  },
 	    { "-12" , "-BRBrightness \\-12"  },
 	    { "-13" , "-BRBrightness \\-13"  },
 	    { "-14" , "-BRBrightness \\-14"  },
 	    { "-15" , "-BRBrightness \\-15"  },
 	    { "-16" , "-BRBrightness \\-16"  },
 	    { "-17" , "-BRBrightness \\-17"  },
 	    { "-18" , "-BRBrightness \\-18"  },
 	    { "-19" , "-BRBrightness \\-19"  },
 	    { "-20" , "-BRBrightness \\-20"  },
 	    { "0" , "-BRBrightness 0"  },
 	    { "1" , "-BRBrightness 1"  },
 	    { "2" , "-BRBrightness 2"  },
 	    { "3" , "-BRBrightness 3"  },
 	    { "4" , "-BRBrightness 4"  },
 	    { "5" , "-BRBrightness 5"  },


 	    { "6" , "-BRBrightness 6"  },
 	    { "7" , "-BRBrightness 7"  },
 	    { "8" , "-BRBrightness 8"  },
 	    { "9" , "-BRBrightness 9"  },
 	    { "10" , "-BRBrightness 10"  },
 	    { "11" , "-BRBrightness 11"  },
 	    { "12" , "-BRBrightness 12"  },
 	    { "13" , "-BRBrightness 13"  },
 	    { "14" , "-BRBrightness 14"  },
 	    { "15" , "-BRBrightness 15"  },
 	    { "16" , "-BRBrightness 16"  },
 	    { "17" , "-BRBrightness 17"  },
 	    { "18" , "-BRBrightness 18"  },
 	    { "19" , "-BRBrightness 19"  },
 	    { "20" , "-BRBrightness 20"  },

 	    { NULL , NULL  }
     }
};

/////    DefaultBRReverse
PPDCOMMAND DefaultBRReverse={
	"DefaultBRReverse",
	{
		{ "OFF"	,	"-BRReverse OFF" },
		{ "ON"	,	"-BRReverse ON" },
		{ NULL	,	NULL }
	}
};
//////// DefaultDuplex
PPDCOMMAND DefaultBRDuplex={
	"DefaultBRDuplex",
	{
		{ "None"	,	"-Duplex None" },
		{ "DuplexTumble"	,	"-Duplex DuplexTumble" },
		{ "DuplexNoTumble"	,	"-Duplex DuplexNoTumble" },
		{ NULL	,	NULL }
	}
};
//////////////////////////////
PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultResolution,
	&DefaultPageSize,
	&DefaultInputSlot,
	&DefaultMediaType,
	&DefaultBRColorMatching,
	&DefaultMonoColor,
	&DefaultBRGray,
	&DefaultBREnhanceBlkPrt,
	&DefaultBRMirror,
	&DefaultTonerSaveMode,
	&DefaultBRSaturation,
	&DefaultBRBlue,
	&DefaultBRGreen,
	&DefaultBRRed,
	&DefaultBRContrast,
	&DefaultBRBrightness,

	&DefaultBRReverse,
	&DefaultBRDuplex,
 	NULL
};




#endif  //_BRCUPS_COMMANDS_H_
