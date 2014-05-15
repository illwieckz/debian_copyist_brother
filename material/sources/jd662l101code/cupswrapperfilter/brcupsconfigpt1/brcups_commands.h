//* Brother CUPS wrapper tool
//* Copyright (C) 2005-2006 Brother. Industries, Ltd.//*
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
#define _SL4C_





char brprintconf[30]="brprintconfpt1";
//   +++++++++++++++++++++++++++
//       set default
//   +++++++++++++++++++++++++++
char *default_setting[] = {
		// Collate
		"brprintconfpt1 -P BROTHERPRINTER_XXX  -collate OFF"   ,
		// Auto cut
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -cut ON"   ,
		// Bringhtness
//	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -brit 0"   ,
		// Contrast
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -cont 0"   ,
		// Halftone
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -half ERROR"   ,
		// Mirror
   	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -mirro OFF"   ,
   	     	//Added for Density
	     "brprintconfpt1 -P BROTHERPRINTER_XXX -density 5",
   	     	// Added for 2PlyMode
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -2plymode OFF"   ,  
	     // Added for NegativePrint
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -negativeprint OFF"   ,
	     	// Added for Dash Line Print
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -dashline OFF"   ,
		// Quality
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -quality SPEED"   ,
		// Resolution
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -reso 200"   ,
	     // Margin(Feed)
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -feed 0",
             // (Form Feed for PJ)
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -formfeed FIXED",
              // Extra Feed
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -extrafeed ONE",
	     // Media
	     "brprintconfpt1 -P BROTHERPRINTER_XXX  -media Letter",
            // copies
		"brprintconfpt1 -P BROTHERPRINTER_XXX  -copy 1",
	     
	     NULL
};
typedef struct {
    char *option;
    char *value;
} CMDLINELIST;
//   +++++++++++++++++++++++++++

//       command line   (Brother command)
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist[] = {
		// resolution
	   {  "Resolution=200"		,   "-reso 400"       	},

	   // CutAtEnd
	   {  "CutAtEnd=OFF"			,	"-cutend OFF"      },
	   {  "CutAtEnd=ON"			,	"-cutend ON"      },
	   {  "noCutAtEnd"			,	"-cutend OFF"      },
	   {  "CutAtEnd"			,	"-cutend ON"      },

				// Trimtapeafterdata 
	   /*{  "Trimtape=OFF"			 ,	 "-trimtape OFF"		},
	   {  "Trimtape=ON"		 ,	 "-trimtape ON"	   },
	   {  "noTrimtape" 		 ,	 "-trimtape OFF"		},
	   {  "Trimtape"			 ,	 "-trimtape ON"	   },*/

	   // Compress
	   {  "Compress=OFF"			,	"-compress OFF"      },
	   {  "Compress=ON"			,	"-compress ON"      },
	   {  "noCompress"			,	"-compress OFF"      },
	   {  "Compress"			,	"-compress ON"      },


	   // Collate
	   {  "Collate=OFF"			,	"-collate OFF"      },
	   {  "Collate=ON"			,	"-collate ON"      },

	   // AutoCut
	   {  "AutoCut=OFF"			,	"-cut OFF"      },
	   {  "AutoCut=ON"				,	"-cut ON"       },
	   {  "noAutoCut"			    ,	"-cut OFF"      }, // for debian
	   {  "AutoCut"				,	"-cut ON"       }, // for debian

	   // Mirror
   	   {  "MirrorPrint=OFF"		,   "-mirro OFF"	},
   	   {  "MirrorPrint=ON"		,   "-mirro ON"	},
   	   {  "noMirrorPrint"		,   "-mirro OFF"	},  // for debian
   	   {  "MirrorPrint"		,   "-mirro ON"	},      // for debian
	   
	   // Added for 2PlyMode //
   	   {  "2PlyMode=OFF"	,   "-2plymode OFF"	},
   	   {  "2PlyMode=ON"	,   "-2plymode ON" },
   	   {  "no2PlyMode"	,   "-2plymode OFF"	},  // for debian
   	   {  "2PlyMode"	,   "-2plymode ON" },      // for debian
   	   
   	   // Added for Dash Line Print //
   	   {  "DashLine=OFF"	,   "-dashline OFF"	},
   	   {  "DashLine=ON"	,   "-dashline ON" },
   	   {  "noDashLine"	,   "-dashline OFF"	},  // for debian
   	   {  "DashLine"	,   "-dashline ON" },      // for debian
   	   
   	   // Added for NegativePrint //
   	   {  "NegativePrint=OFF"	,   "-negativeprint OFF"	},
   	   {  "NegativePrint=ON"	,   "-negativeprint ON" },
   	   {  "noNegativePrint"		,   "-negativeprint OFF"	},  // for debian
   	   {  "NegativePrint"		,   "-negativeprint ON" },      // for debian
   	   	   
	   // HalftonePattern
	   {  "Halftone=ERROR"   ,       	"-half ERROR"},
	   {  "Halftone=BINARY"   ,		"-half BINARY"},
	   {  "Halftone=DITHER"   ,  		"-half DITHER"},

        // Formfeedmode
	   {  "Formfeedmode=FIXED"   ,       	"-formfeed FIXED"},
	   {  "Formfeedmode=ENDOFPAGE"   ,		"-formfeed ENDOFPAGE"},
	   {  "Formfeedmode=NOFEED"   ,  		"-formfeed NOFEED"},
           {  "Formfeedmode=ENDOFPAGERETRACT"   ,  		"-formfeed ENDOFPAGERETRACT"},

	// Extrafeedmode
	   {  "Extrafeedmode=NONE"   ,       	"-extrafeed NONE"},
	   {  "Extrafeedmode=HALF"   ,		"-extrafeed HALF"},
	   {  "Extrafeedmode=ONE"   ,  		"-extrafeed ONE"},
	   {  "Extrafeedmode=ONEANDHALF"   ,  		"-extrafeed ONEANDHALF"},
	   {  "Extrafeedmode=TWO"   ,  		"-extrafeed TWO"},


	   // Quality
	   {  "Priority=SPEED"   ,       	"-quality SPEED"},
	   {  "Priority=QUALITY"   ,		"-quality QUALITY"},

      
	   { NULL   ,   NULL    },
};

//#S2
//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	// Paper Size
{  "media=A4"	,   "-media A4"     },
{  "media=RollA4"	,   "-media RollA4"     },
{  "media=PerfA4"	,   "-media PerfA4"     },
{  "media=RetractA4"	,   "-media RetractA4"     },
{  "media=Letter"	,   "-media Letter"     },
{  "media=RollLetter"	,   "-media RollLetter"     },
{  "media=PerfLetter"	,   "-media PerfLetter"     },
{  "media=RetractLetter"	,   "-media RetractLetter"     },
{  "media=Legal"	,   "-media Legal"     },
{  "media=RollLegal"	,   "-media RollLegal"     },
{  "media=PerfLegal"	,   "-media PerfLegal"     },
{  "media=RetractLegal"	,   "-media RetractLegal"     },
{  "media=ContinuousLetter"	,   "-media ContinuousLetter"     },
	{  "media=29x90"	,   "-media 29x90"     },
	{  "media=12X1"   	,   "-media 12X1"      },
	{  "media=29X1"   	,   "-media 29X1"      },
	{  "media=38X1"   	,   "-media 38X1"      },
	{  "media=50X1"   	,   "-media 50X1"      },
	{  "media=54X1"   	,   "-media 54X1"      },
	{  "media=62X1"   	,   "-media 62X1"      },
	{  "media=102X1"	,   "-media 102X1"     },

	{ NULL   ,   NULL    },
};

//   +++++++++++++++++++++++++++
//       command line numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	{	"Copies="   	,  	 "-copy "    	},
//	{	"Brightness="	,	"-brit "	},
	{	"Density="	,	"-density"	},		//Added 
	{	"Contrast="	,	"-cont "	},
	{	"Feed="		,	"-feed "	},
	//
	{	"CutLabel="	,	"-cutlabel "	},
	{ 	NULL   		,   	NULL  		},
};
/*      PPD Default   */
//   +++++++++++++++++++++++++++
//       PPD setting list
//   +++++++++++++++++++++++++++



typedef struct {
    char 		*value;
    char		*brcommand;
} PPDCOMMANDLIST;


typedef   struct   {
   char             *label;
   PPDCOMMANDLIST   *ppdcommandlist;
}  PPDCOMMAND;

//       ***** DefaultResolution *****
PPDCOMMANDLIST ppdComListRes[ ] ={
      	{ "Normal"  , "-reso 200"  },
 	{ NULL , NULL  }
}  ;
 PPDCOMMAND  DefaultResolution={
  "DefaultResolution",
   ppdComListRes
};
//#S2
//       ***** DefaultPageSize *****
PPDCOMMANDLIST ppdComListPSiz[ ] =
 {         
           //{"210x297"   ,     "-media 210x297"},
           {  "A4"	,   "-media A4"    },
{  "RollA4"	,   "-media RollA4"    },
{  "PerfA4"	,   "-media PerfA4"    },
{  "RetractA4"	,   "-media RetractA4"    },
{  "Letter"	,   "-media Letter"    },
{  "RollLetter"	,   "-media RollLetter"    },
{  "PerfLetter"	,   "-media PerfLetter"    },
{  "RetractLetter"	,   "-media RetractLetter"    },
{  "Legal"	,   "-media Legal"    },
{  "RollLegal"	,   "-media RollLegal"    },
{  "PerfLegal"	,   "-media PerfLegal"    },
{  "RetractLegal"	,   "-media RetractLegal"    },
{  "ContinuousLetter"	,   "-media ContinuousLetter"    },
	   {  "29x90"	,   "-media 29x90"     },
	   {  "12X1"   	,   "-media 12X1"      },
	   {  "29X1"   	,   "-media 29X1"      },
	   {  "38X1"   	,   "-media 38X1"      },	   
	   {  "50X1"   	,   "-media 50X1"      },
          {  "54X1"   	,   "-media 54X1"      },
          {  "62X1"   	,   "-media 62X1"      },
	   {  "102X1"	,   "-media 102X1"     },
       { NULL , NULL  }
  };
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
     ppdComListPSiz
};


//       ***** DefaultBrAutoTapeCut*****
PPDCOMMANDLIST ppdComListAutC[ ] =
{
 	    { "OFF", "-cut OFF"  },
 	    { "ON" , "-cut ON"  },
 	    { NULL , NULL  }
} ;
PPDCOMMAND  DefaultBrAutoTapeCut={
    "DefaultBrAutoTapeCut",
    ppdComListAutC
};

//       ***** DefaultBRMirror*****
PPDCOMMANDLIST ppdComListMirr[ ] =
 {
 	    { "OFF", "-mirro OFF"  },
 	    { "ON" , "-mirro ON"  },
 	    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrMirror={
    "DefaultBrMirror",
    ppdComListMirr
};

/* Added for 2Ply Mode */
//       ***** DefaultBR2PlyMode*****
PPDCOMMANDLIST ppdComList2PlyMode[ ] =
 {
 	    { "OFF", "-2plymode OFF"  },
 	    { "ON" , "-2plymode ON"  },
 	    { NULL , NULL  }
};
PPDCOMMAND  DefaultBr2PlyMode={
    "DefaultBr2PlyMode",
    ppdComList2PlyMode
};

/* Added for Dash Line Print */
//       ***** DefaultBRDashLine*****
PPDCOMMANDLIST ppdComListDashLine[ ] =
 {
 	    { "OFF", "-dashline OFF"  },
 	    { "ON" , "-dashline ON"  },
 	    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrDashLine={
    "DefaultBrDashLine",
    ppdComListDashLine
};

/* Added for Negative Print Mode */
//       ***** DefaultBRNegativePrint*****
PPDCOMMANDLIST ppdComListNegativePrint[ ] =
 {
 	    { "OFF", "-negativeprint OFF"  },
 	    { "ON" , "-negativeprint ON"  },
 	    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrNegativePrint={
    "DefaultBrNegativePrint",
    ppdComListNegativePrint
};


//       ***** DefaultBrCompress*****
PPDCOMMANDLIST ppdComListComp[ ] =
{
 	    { "OFF", "-compress OFF"  },
 	    { "ON" , "-compress ON"  },
 	    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrCompress={
    "DefaultBrCompress",
    ppdComListComp
};


//       ***** DefaultBrCutAtEnd*****
PPDCOMMANDLIST ppdComListCutE[ ] =
{
 	    { "OFF", "-cutend OFF"  },
 	    { "ON" , "-cutend ON"  },
 	    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrCutAtEnd={
    "DefaultBrCutAtEnd",
    ppdComListCutE
};

//       ***** DefaultBrTrimtapeafterdata*****
/*PPDCOMMANDLIST ppdComListTrimtape[ ] =
{
 	    { "OFF", "-trimtape OFF"  },
 	    { "ON" , "-trimtape ON"  },
 	    { NULL , NULL  }
};*/
/*PPDCOMMAND  DefaultBrTrimtape={
    "DefaultBrTrimtape",
    ppdComListTrimtape
};*/


//       ***** DefaultBrPriority *****
PPDCOMMANDLIST ppdComListPrio [ ] =
{
		{  "BrSpeed"  ,	"-quality SPEED"      		},
		{  "BrQuality"   	,	"-quality QUALITY"		},
 	    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrPriority={

    "DefaultBrPriority",
    ppdComListPrio
};

//    ******DefaultBrHalftonePattern**********
PPDCOMMANDLIST ppdComListHalftone[ ] =
{
	   {  "BrErrorDiffusion"   ,	"-half ERROR"},
	   {  "BrBinary"   ,			"-half BINARY"},
	   {  "BrDither"   ,  			"-half DITHER"},
	   { NULL , NULL  }
};
PPDCOMMAND  DefaultBrHalftonePattern={

	"DefaultBrHalftonePattern",
	ppdComListHalftone
};

//    ******DefaultBrFormfeedmode**********
PPDCOMMANDLIST ppdComListFormfeedmode[ ] =
{
	   {  "BrFixed"   ,	"-formfeed FIXED"},
	   {  "BrEndofpage"   ,			"-formfeed ENDOFPAGE"},
	   {  "BrNofeed"   ,  			"-formfeed NOFEED"},
           {  "BrEndofpageretract"   ,  			"-formfeed ENDOFPAGERETRACT"},
	   { NULL , NULL  }
};
PPDCOMMAND  DefaultBrFormFeedMode={

	"DefaultBrFormFeedMode",
	ppdComListFormfeedmode
};

//    ******DefaultBrExtrafeedmode**********
PPDCOMMANDLIST ppdComListExtrafeedmode[ ] =
{
	   {  "BrNone"   ,	"-extrafeed NONE"},
	   {  "BrHalfinch"   ,			"-extrafeed HALF"},
	   {  "BrOneinch"   ,  			"-extrafeed ONE"},
	   {  "BrOneandhalfinch"   ,			"-extrafeed ONEANDHALF"},
	   {  "BrTwoinch"   ,  			"-extrafeed TWO"},
	   { NULL , NULL  }
};
PPDCOMMAND  DefaultBrExtraFeedMode={

	"DefaultBrExtraFeedMode",
	ppdComListExtrafeedmode
};



static PPDCOMMAND   *ppdcommand_all_list[ ] = {
	&DefaultBrCompress,	//
	&DefaultBrCutAtEnd,	//
	//&DefaultBrTrimtape,	//
	&DefaultResolution,
	&DefaultPageSize,
	&DefaultBrAutoTapeCut,
	&DefaultBrMirror,
	&DefaultBr2PlyMode,		//Added for 2PlyMode 
	&DefaultBrDashLine,		//Added for Dash Line Print
	&DefaultBrNegativePrint,	//Added for Negative Print Mode
	&DefaultBrPriority,
	&DefaultBrHalftonePattern,
        &DefaultBrFormFeedMode,
	&DefaultBrExtraFeedMode,
      NULL
};
/*      PPD Default   end*/
//   +++++++++++++++++++++++++++
//       PPD numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST PPDdefaultN[] = {
//	{"DefaultBrBrightness", "-brit "},
	{"DefaultBrDensity"	, "-density"},		//Added
	{"DefaultBrContrast"	, "-cont "},
	{"DefaultBrMargin"	, "-feed "},
	{"DefaultBrCutLabel"	, "-cutlabel"},	//
	{ NULL   ,   NULL    },
};

#define _LABEL_BUF_MAX_ 	33
#define _LABEL_MAX_ 	(_LABEL_BUF_MAX_ - 1)
#define _LABLE_ID_HEAD_				"BrL"

typedef struct
{
	char tape_name[_LABEL_BUF_MAX_];
	char tape_id[_LABEL_BUF_MAX_];
}TapeItem, *TTapeItem;

typedef struct
{
	TTapeItem tapelist;
	int count;
	int iSelIndex;
}TapeList, *TTapeList;


int add_tape(TTapeList ptapelist, const char* plabelname, const char* plabel_id);
int GetLabel_name_id(const char* lpString, char* lpName, char* lpID);

#endif


