/*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along with
* this program; if not, write to the Free Software Foundation, Inc., 59 Temple
* Place, Suite 330, Boston, MA  02111-1307  USA
*
*/
 

#ifndef _BRCUPS_COMMANDS_H_
#define _BRCUPS_COMMANDS_H_
#define _SL4C_





char brprintconf[30]="brprintconfcl1";
char *default_setting[] = {
		 
	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -pt A4"   ,
	     
	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -feed AUTO",
	     
	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -reso 600"   ,
	     
	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -capt OFF",
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -corm FULLCOLOR"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -md PLAIN"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -cm MATCHMONITOR"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -impg OFF"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -ebp OFF",
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -mirro OFF"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -brit 0"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -cont 0"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -red 0"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -green 0"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -blue 0"   ,
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -saturation 0"   ,
	     
	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -tsm OFF"   ,
	     
	     
   	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -hrc PRINTERDEFAULT",
         
	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -copy 1"   ,
	     
	     "brprintconfcl1 -P BROTHERPRINTER_XXX  -adj NORMAL",
	     
	     NULL
};
typedef struct {
    char *option;
    char *value;
} CMDLINELIST;

CMDLINELIST commandlinelist[] = {
		
	   {  "PaperType=A4"		,   "-pt A4"       },
	   {  "PaperType=Letter"	,   "-pt LT"     },
	   {  "PaperType=Legal"		,   "-pt LGL"     },
	   {  "PaperType=Executive"	,   "-pt EXE"    },
	   {  "PaperType=A5"		,   "-pt A5"       },
	   {  "PaperType=B5"		,   "-pt B5"       },
   	   {  "PaperType=JISB5"   	,   "-pt B5"       },
	   {  "PaperType=com10"   	,   "-pt COM-10"       },
	   {  "PaperType=Com10"   	,   "-pt COM-10"       },
	   {  "PaperType=COM10"   	,   "-pt COM-10"       },
	   {  "PaperType=COM-10"   	,   "-pt COM-10"       },
	   {  "PaperType=Com-10"   	,   "-pt COM-10"       },
	   {  "PaperType=com-10"   	,   "-pt COM-10"       },
	   {  "PaperType=DL"   		,   "-pt DL"       },

	   
	   {  "PaperSource=Tray1"   ,   "-feed TRAY1"       },
	   {  "PaperSource=Tray2"   ,   "-feed TRAY2"       },
	   {  "PaperSource=AutoSelect", "-feed AUTO"   },
   	   {  "PaperSource=Manul"   ,   "-feed MANUL"   },

		
	   {  "Resolution=600"		,   "-reso 600"       	},
	   {  "Resolution=2400"		,   "-reso 2400"       	},

	   
	   {  "CAPT=OFF"			,	"-capt OFF"      },
	   {  "CAPT=ON"				,	"-capt ON"       },
	   {  "noCAPT"			    ,	"-capt OFF"      }, 
	   {  "CAPT"				,	"-capt ON"       }, 
	   	   
	   
	   {  "ColorOrMono=Mono"   ,       		"-corm MONO"      		},
	   {  "ColorOrMono=FullColor"   ,		"-corm FULLCOLOR"		},
	   {  "ColorOrMono=BlackCyan"   ,  		"-corm BLACKCYAN"		},
	   {  "ColorOrMono=BlackMagenta",		"-corm BLACKMAGENTA"	},
	   {  "ColorOrMono=BlackYellow"   ,   	"-corm BLACKYELLOW"	},

	   
	   {  "Media=PlainPaper"		,   "-md PLAIN"       },
	   {  "Media=ThinPaper"			,   "-md THIN"       },
	   {  "Media=ThickPaper"		,   "-md THICK"       },
	   {  "Media=ThickerPaper"		,   "-md THICKER"       },
	   {  "Media=Transparencies"	,   "-md TRANS"       },
   	   {  "Media=Recycled"			,   "-md RECY"       },

       
	   {  "ColorMatching=MatchMonitor"	,   "-cm MATCHMONITOR"	},
	   {  "ColorMatching=Vivid"			,   "-cm VIVID"       	},
	   {  "ColorMatching=None"			,   "-cm NONE"       	},

	   
   	   {  "ImproveGray=OFF"		,   "-impg OFF"	},
	   {  "ImproveGray=ON"		,   "-impg ON"  },
   	   {  "noImproveGray"		,   "-impg OFF"	},  
	   {  "ImproveGray"		    ,   "-impg ON"  },  

   	   
   	   {  "EnBlkPrinting=OFF"		,   "-ebp OFF"	},
   	   {  "EnBlkPrinting=ON"		,   "-ebp ON"	},
   	   {  "noEnBlkPrinting"		,   "-ebp OFF"	},  
   	   {  "EnBlkPrinting"		,   "-ebp ON"	},  

       
   	   {  "MirrorPrint=OFF"		,   "-mirro OFF"	},
   	   {  "MirrorPrint=ON"		,   "-mirro ON"	},
   	   {  "noMirrorPrint"	    ,   "-mirro OFF"	},  
   	   {  "MirrorPrint"		    ,   "-mirro ON"	},      

       
   	   {  "TonerSaveMode=OFF"		,   "-tsm OFF"	},
   	   {  "TonerSaveMode=ON"		,   "-tsm ON"	},
   	   {  "noTonerSaveMode"		,   "-tsm OFF"	},      
   	   {  "TonerSaveMode"		,   "-tsm ON"	},      

       
   	   {  "HRC=PrinterDefault"		,   "-hrc PRINTERDEFAULT"	},
   	   {  "HRC=Light"				,   "-hrc LIGHT"	},
  	   {  "HRC=Medium"				,   "-hrc MEDIUM"	},
   	   {  "HRC=Dark"				,   "-hrc DARK"	},
  	   {  "HRC=Off"					,   "-hrc OFF"	},

       	
   	   {  "LandscapeAdjustment=Normal"		,   "-adj NORMAL"	},
   	   {  "LandscapeAdjustment=Adjust"		,   "-adj ADJUST"	},
       
	   { NULL   ,   NULL    },
};


CMDLINELIST standard_commandlinelist[] = {
		
	   {  "media=A4"		,   "-pt A4"       },
	   {  "media=Letter"	,   "-pt LT"     },
	   {  "media=Legal"		,   "-pt LGL"     },
	   {  "media=Executive"	,   "-pt EXE"    },
	   {  "media=A5"		,   "-pt A5"       },
	   {  "media=B5"		,   "-pt B5"       },
   	   {  "media=JISB5"   	,   "-pt B5"       },
	   {  "media=Com10"   	,   "-pt COM-10"       },
	   {  "media=com10"   	,   "-pt COM-10"       },
	   {  "media=COM10"   	,   "-pt COM-10"       },
	   {  "media=COM-10"   	,   "-pt COM-10"       },
	   {  "media=com-10"   	,   "-pt COM-10"       },
	   {  "media=Com-10"   	,   "-pt COM-10"       },
	   {  "media=DL"   		,   "-pt DL"       },

	   
	   {  "media=PlainPaper"		,   "-md PLAIN"       },
	   {  "media=ThinPaper"			,   "-md THIN"       },
	   {  "media=ThickPaper"		,   "-md THICK"       },
	   {  "media=ThickerPaper"		,   "-md THICKER"       },
	   {  "media=Transparencies"	,   "-md TRANS"       },
   	   {  "media=Recycled"			,   "-md RECY"       },

   	   
	   {  "media=Tray1"   ,   "-feed TRAY1"       },
	   {  "media=Tray2"   ,   "-feed TRAY2"       },
	   {  "media=AutoSelect", "-feed AUTO"   },
   	   {  "media=Manul"   ,   "-feed MANUL"   },

       
	   {  "media=.*,A4"		,   "-pt A4"       },
	   {  "media=.*,Letter"	,   "-pt LT"     },
	   {  "media=.*,Legal"		,   "-pt LGL"     },
	   {  "media=.*,Executive"	,   "-pt EXE"    },
	   {  "media=.*,A5"		,   "-pt A5"       },
	   {  "media=.*,B5"		,   "-pt B5"       },
   	   {  "media=.*,JISB5"   	,   "-pt B5"       },
	   {  "media=.*,COM10"   	,   "-pt COM-10"       },
	   {  "media=.*,Com10"   	,   "-pt COM-10"       },
	   {  "media=.*,com10"   	,   "-pt COM-10"       },
	   {  "media=.*,COM-10"   	,   "-pt COM-10"       },
	   {  "media=.*,Com-10"   	,   "-pt COM-10"       },
	   {  "media=.*,com-10"   	,   "-pt COM-10"       },
	   {  "media=.*,DL"   		,   "-pt DL"       },

	   
	   {  "media=.*,PlainPaper"		,   "-md PLAIN"       },
	   {  "media=.*,ThinPaper"			,   "-md THIN"       },
	   {  "media=.*,ThickPaper"		,   "-md THICK"       },
	   {  "media=.*,ThickerPaper"		,   "-md THICKER"       },
	   {  "media=.*,Transparencies"	,   "-md TRANS"       },
   	   {  "media=.*,Recycled"			,   "-md RECY"       },

   	   
	   {  "media=.*,Tray1"   ,   "-feed TRAY1"       },
	   {  "media=.*,Tray2"   ,   "-feed TRAY2"       },
	   {  "media=.*,AutoSelect", "-feed AUTO"   },
   	   {  "media=.*,Manul"   ,   "-feed MANUL"   },
	   { NULL   ,   NULL    },
};

CMDLINELIST commandlinelist2[] = {
	   {	"Copies="   	,   "-copy "      },
	   {	"Brightness="	,	"-brit "	},
	   {	"Contrast="		,	"-cont "		},
	   {	"RedKey="		,	"-red "		},
	   {	"GreenKey="		,	"-green "	},
	   {	"BlueKey="		,	"-blue "	},
	   {	"Saturation="	,	"-saturation "},
	   { NULL   ,   NULL    },
};
 



typedef struct {
    char *value;
    char *brcommand;
} PPDCOMMANDLIST;


typedef   struct   {
   char             *label;
   PPDCOMMANDLIST   ppdcommandlist[];
}  PPDCOMMAND;

PPDCOMMAND  DefaultResolution={
  "DefaultResolution",
  {
  		{ "Normal"  , "-reso 600"  },
    	{ "CAPT"    , "-reso 2400"  },
 	    { NULL , NULL  }
  }
};

PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    {
	   {  "BrA4"		,   "-pt A4"       },
	   {  "BrLetter"	,   "-pt LT"     },
	   {  "BrLegal"		,   "-pt LGL"     },
	   {  "BrExecutive"	,   "-pt EXE"    },
	   {  "BrA5"		,   "-pt A5"       },
	   {  "BrB5"		,   "-pt B5"       },
   	   {  "BrJISB5"   	,   "-pt B5"       },
	   {  "BrCom10"   	,   "-pt COM-10"       },
	   {  "BrDL"   		,   "-pt DL"       },
       { NULL , NULL  }
     }
};
PPDCOMMAND  DefaultInputSlot={
    "DefaultInputSlot",
    {
 	    { "Tray1" 		, "-feed TRAY1"  },
 	    { "Tray2" 		, "-feed TRAY2"  },
 	    { "AutoSelect" 	, "-feed AUTO"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRCAPT={
	"DefaultBRCAPT",
	{
		{	NULL, NULL	}
	}	
};

PPDCOMMAND  DefaultMediaType={
    "DefaultMediaType",
    {
	   {  "Plain"		,   "-md PLAIN"       },
	   {  "Thin"			,   "-md THIN"       },
	   {  "Thick"		,   "-md THICK"       },
	   {  "Thicker"		,   "-md THICKER"       },
	   {  "Transparencies"	,   "-md TRANS"       },
   	   {  "Recycled"			,   "-md RECY"       },		
       { NULL , NULL  }
     }
};

 

PPDCOMMAND  DefaultBRColorMatching={
    "DefaultBRColorMatching",
    {
 	    { "MatchMonitor",	"-cm MATCHMONITOR"  },
 	    { "VividText",		"-cm VIVID"  },

 	    { "None",			"-cm NONE"  },            
 	    { NULL , NULL  }
     }
};


PPDCOMMAND  DefaultBRGray={
    "DefaultBRGray",
    {
 	    { "OFF", "-impg OFF"  },
 	    { "ON" , "-impg ON"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBREnhanceBlkPrt={
    "DefaultBREnhanceBlkPrt",
    {
 	    { "OFF", "-ebp OFF"  },
 	    { "ON" , "-ebp ON"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRMirror={
    "DefaultBRMirror",
    {
 	    { "OFF", "-mirro OFF"  },
 	    { "ON" , "-mirro ON"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultTonerSaveMode={
    "DefaultTonerSaveMode",
    {
 	    { "OFF" , "-tsm OFF"  },
 	    { "ON" , "-tsm ON"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultMonoColor={
    "DefaultMonoColor",
    {
		{  "BrFullColor"  ,	"-corm FULLCOLOR"      		},
		{  "BrMono"   	,	"-corm MONO"		},
		{  "BrB&C"   		,	"-corm BLACKCYAN"		},
		{  "BrB&M"		,	"-corm BLACKMAGENTA"	},
		{  "BrB&Y"		,  	"-corm BLACKYELLOW"	},		
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRSaturation={
    "DefaultBRSaturation",
    {
 	    { "-1" , "-saturation \\-1"  },
 	    { "-2" , "-saturation \\-2"  },
 	    { "-3" , "-saturation \\-3"  },
 	    { "-4" , "-saturation \\-4"  },
 	    { "-5" , "-saturation \\-5"  },
 	    { "-6" , "-saturation \\-6"  },
 	    { "-7" , "-saturation \\-7"  },
 	    { "-8" , "-saturation \\-8"  },
 	    { "-9" , "-saturation \\-9"  },
 	    { "-10" , "-saturation \\-10"  },
 	    { "-11" , "-saturation \\-11"  },
 	    { "-12" , "-saturation \\-12"  },
 	    { "-13" , "-saturation \\-13"  },
 	    { "-14" , "-saturation \\-14"  },
 	    { "-15" , "-saturation \\-15"  },
 	    { "-16" , "-saturation \\-16"  },
 	    { "-17" , "-saturation \\-17"  },
 	    { "-18" , "-saturation \\-18"  },
 	    { "-19" , "-saturation \\-19"  },
 	    { "-20" , "-saturation \\-20"  },
 	    { "0" , "-saturation 0"  },
 	    { "1" , "-saturation 1"  },
 	    { "2" , "-saturation 2"  },
 	    { "3" , "-saturation 3"  },
 	    { "4" , "-saturation 4"  },
 	    { "5" , "-saturation 5"  },
 	    { "6" , "-saturation 6"  },
 	    { "7" , "-saturation 7"  },
 	    { "8" , "-saturation 8"  },
 	    { "9" , "-saturation 9"  },

 	    { "10" , "-saturation 10"  },
 	    { "11" , "-saturation 11"  },
 	    { "12" , "-saturation 12"  },
 	    { "13" , "-saturation 13"  },
 	    { "14" , "-saturation 14"  },
 	    { "15" , "-saturation 15"  },
 	    { "16" , "-saturation 16"  },
 	    { "17" , "-saturation 17"  },
 	    { "18" , "-saturation 18"  },
 	    { "19" , "-saturation 19"  },
 	    { "20" , "-saturation 20"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRBlue={
    "DefaultBRBlue",
    {
 	    { "-1" , "-blue \\-1"  },
 	    { "-2" , "-blue \\-2"  },
 	    { "-3" , "-blue \\-3"  },
 	    { "-4" , "-blue \\-4"  },
 	    { "-5" , "-blue \\-5"  },
 	    { "-6" , "-blue \\-6"  },
 	    { "-7" , "-blue \\-7"  },
 	    { "-8" , "-blue \\-8"  },
 	    { "-9" , "-blue \\-9"  },
 	    { "-10" , "-blue \\-10"  },
 	    { "-11" , "-blue \\-11"  },
 	    { "-12" , "-blue \\-12"  },
 	    { "-13" , "-blue \\-13"  },

 	    { "-14" , "-blue \\-14"  },
 	    { "-15" , "-blue \\-15"  },
 	    { "-16" , "-blue \\-16"  },
 	    { "-17" , "-blue \\-17"  },
 	    { "-18" , "-blue \\-18"  },
 	    { "-19" , "-blue \\-19"  },
 	    { "-20" , "-blue \\-20"  },
 	    { "0" , "-blue 0"  },
 	    { "1" , "-blue 1"  },
 	    { "2" , "-blue 2"  },
 	    { "3" , "-blue 3"  },
 	    { "4" , "-blue 4"  },
 	    { "5" , "-blue 5"  },
 	    { "6" , "-blue 6"  },
 	    { "7" , "-blue 7"  },
 	    { "8" , "-blue 8"  },
 	    { "9" , "-blue 9"  },
 	    { "10" , "-blue 10"  },
 	    { "11" , "-blue 11"  },
 	    { "12" , "-blue 12"  },
 	    { "13" , "-blue 13"  },
 	    { "14" , "-blue 14"  },
 	    { "15" , "-blue 15"  },
 	    { "16" , "-blue 16"  },
 	    { "17" , "-blue 17"  },
 	    { "18" , "-blue 18"  },
 	    { "19" , "-blue 19"  },
 	    { "20" , "-blue 20"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRGreen={
    "DefaultBRGreen",
    {
 	    { "-1" , "-green \\-1"  },
 	    { "-2" , "-green \\-2"  },
 	    { "-3" , "-green \\-3"  },
 	    { "-4" , "-green \\-4"  },
 	    { "-5" , "-green \\-5"  },
 	    { "-6" , "-green \\-6"  },
 	    { "-7" , "-green \\-7"  },
 	    { "-8" , "-green \\-8"  },
 	    { "-9" , "-green \\-9"  },
 	    { "-10" , "-green \\-10"  },
 	    { "-11" , "-green \\-11"  },
 	    { "-12" , "-green \\-12"  },
 	    { "-13" , "-green \\-13"  },
 	    { "-14" , "-green \\-14"  },
 	    { "-15" , "-green \\-15"  },
 	    { "-16" , "-green \\-16"  },
 	    { "-17" , "-green \\-17"  },
 	    { "-18" , "-green \\-18"  },
 	    { "-19" , "-green \\-19"  },
 	    { "-20" , "-green \\-20"  },
 	    { "0" , "-green 0"  },
 	    { "1" , "-green 1"  },
 	    { "2" , "-green 2"  },
 	    { "3" , "-green 3"  },
 	    { "4" , "-green 4"  },
 	    { "5" , "-green 5"  },
 	    { "6" , "-green 6"  },

 	    { "7" , "-green 7"  },
 	    { "8" , "-green 8"  },
 	    { "9" , "-green 9"  },
 	    { "10" , "-green 10"  },
 	    { "11" , "-green 11"  },
 	    { "12" , "-green 12"  },
 	    { "13" , "-green 13"  },
 	    { "14" , "-green 14"  },
 	    { "15" , "-green 15"  },
 	    { "16" , "-green 16"  },
 	    { "17" , "-green 17"  },
 	    { "18" , "-green 18"  },
 	    { "19" , "-green 19"  },
 	    { "20" , "-green 20"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRRed={
    "DefaultBRRed",
    {
 	    { "-1" , "-red \\-1"  },
 	    { "-2" , "-red \\-2"  },
 	    { "-3" , "-red \\-3"  },
 	    { "-4" , "-red \\-4"  },
 	    { "-5" , "-red \\-5"  },
 	    { "-6" , "-red \\-6"  },
 	    { "-7" , "-red \\-7"  },
 	    { "-8" , "-red \\-8"  },
 	    { "-9" , "-red \\-9"  },
 	    { "-10" , "-red \\-10"  },
 	    { "-11" , "-red \\-11"  },
 	    { "-12" , "-red \\-12"  },
 	    { "-13" , "-red \\-13"  },
 	    { "-14" , "-red \\-14"  },
 	    { "-15" , "-red \\-15"  },

 	    { "-16" , "-red \\-16"  },
 	    { "-17" , "-red \\-17"  },
 	    { "-18" , "-red \\-18"  },
 	    { "-19" , "-red \\-19"  },
 	    { "-20" , "-red \\-20"  },
 	    { "0" , "-red 0"  },
 	    { "1" , "-red 1"  },
 	    { "2" , "-red 2"  },
 	    { "3" , "-red 3"  },
 	    { "4" , "-red 4"  },
 	    { "5" , "-red 5"  },
 	    { "6" , "-red 6"  },
 	    { "7" , "-red 7"  },
 	    { "8" , "-red 8"  },
 	    { "9" , "-red 9"  },
 	    { "10" , "-red 10"  },
 	    { "11" , "-red 11"  },
 	    { "12" , "-red 12"  },
 	    { "13" , "-red 13"  },
 	    { "14" , "-red 14"  },
 	    { "15" , "-red 15"  },
 	    { "16" , "-red 16"  },
 	    { "17" , "-red 17"  },
 	    { "18" , "-red 18"  },
 	    { "19" , "-red 19"  },
 	    { "20" , "-red 20"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRContrast={
    "DefaultBRContrast",
    {
 	    { "-1" , "-cont \\-1"  },
 	    { "-2" , "-cont \\-2"  },
 	    { "-3" , "-cont \\-3"  },
 	    { "-4" , "-cont \\-4"  },
 	    { "-5" , "-cont \\-5"  },
 	    { "-6" , "-cont \\-6"  },
 	    { "-7" , "-cont \\-7"  },
 	    { "-8" , "-cont \\-8"  },
 	    { "-9" , "-cont \\-9"  },
 	    { "-10" , "-cont \\-10"  },
 	    { "-11" , "-cont \\-11"  },
 	    { "-12" , "-cont \\-12"  },
 	    { "-13" , "-cont \\-13"  },
 	    { "-14" , "-cont \\-14"  },

 	    { "-15" , "-cont \\-15"  },
 	    { "-16" , "-cont /\\-16"  },
 	    { "-17" , "-cont \\-17"  },
 	    { "-18" , "-cont \\-18"  },
 	    { "-19" , "-cont \\-19"  },
 	    { "-20" , "-cont \\-20"  },
 	    { "0" , "-cont 0"  },
 	    { "1" , "-cont 1"  },
 	    { "2" , "-cont 2"  },
 	    { "3" , "-cont 3"  },
 	    { "4" , "-cont 4"  },
 	    { "5" , "-cont 5"  },
 	    { "6" , "-cont 6"  },
 	    { "7" , "-cont 7"  },
 	    { "8" , "-cont 8"  },
 	    { "9" , "-cont 9"  },
 	    { "10" , "-cont 10"  },
 	    { "11" , "-cont 11"  },
 	    { "12" , "-cont 12"  },
 	    { "13" , "-cont 13"  },
 	    { "14" , "-cont 14"  },
 	    { "15" , "-cont 15"  },
 	    { "16" , "-cont 16"  },
 	    { "17" , "-cont 17"  },
 	    { "18" , "-cont 18"  },

 	    { "19" , "-cont 19"  },
 	    { "20" , "-cont 20"  },
 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultBRBrightness={
    "DefaultBRBrightness",
    {
 	    { "-1" , "-brit \\-1"  },
 	    { "-2" , "-brit \\-2"  },
 	    { "-3" , "-brit \\-3"  },
 	    { "-4" , "-brit \\-4"  },
 	    { "-5" , "-brit \\-5"  },

 	    { "-6" , "-brit \\-6"  },
 	    { "-7" , "-brit \\-7"  },
 	    { "-8" , "-brit \\-8"  },
 	    { "-9" , "-brit \\-9"  },
 	    { "-10" , "-brit \\-10"  },
 	    { "-11" , "-brit \\-11"  },
 	    { "-12" , "-brit \\-12"  },
 	    { "-13" , "-brit \\-13"  },
 	    { "-14" , "-brit \\-14"  },
 	    { "-15" , "-brit \\-15"  },
 	    { "-16" , "-brit \\-16"  },
 	    { "-17" , "-brit \\-17"  },
 	    { "-18" , "-brit \\-18"  },
 	    { "-19" , "-brit \\-19"  },
 	    { "-20" , "-brit \\-20"  },
 	    { "0" , "-brit 0"  },
 	    { "1" , "-brit 1"  },
 	    { "2" , "-brit 2"  },
 	    { "3" , "-brit 3"  },
 	    { "4" , "-brit 4"  },
 	    { "5" , "-brit 5"  },


 	    { "6" , "-brit 6"  },
 	    { "7" , "-brit 7"  },
 	    { "8" , "-brit 8"  },
 	    { "9" , "-brit 9"  },
 	    { "10" , "-brit 10"  },
 	    { "11" , "-brit 11"  },
 	    { "12" , "-brit 12"  },
 	    { "13" , "-brit 13"  },
 	    { "14" , "-brit 14"  },
 	    { "15" , "-brit 15"  },
 	    { "16" , "-brit 16"  },
 	    { "17" , "-brit 17"  },
 	    { "18" , "-brit 18"  },
 	    { "19" , "-brit 19"  },
 	    { "20" , "-brit 20"  },

 	    { NULL , NULL  }
     }
};

PPDCOMMAND  DefaultLandscapeAdjustment={
  "DefaultLandscapeAdjustment",
  {
  		{ "BrNormal"  	, "-adj NORMAL"  },
    	{ "BrAdjust"   , "-adj ADJUST"  },
 	    { NULL , NULL  }
  }
};

PPDCOMMAND  DefaultBRSleepTime={
    "DefaultBRSleepTime",
    {
 	    { NULL , NULL  }
     }
};

PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultResolution,
	&DefaultPageSize,
	&DefaultInputSlot,
	&DefaultBRCAPT,
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
	&DefaultLandscapeAdjustment,
      NULL
};
 
CMDLINELIST PPDdefaultN[] = {
 		
	   { NULL   ,   NULL    },
};

CMDLINELIST PPDdefaultNValue[] = {
		{"DefaultBRBrightness", "-brit "},
		{"DefaultBRContrast", "-cont "},


		{"DefaultBRRed", "-red "},
		{"DefaultBRGreen", "-green "},
		{"DefaultBRBlue", "-blue "},
		{"DefaultBRSaturation", "-saturation "},
	   { NULL   ,   NULL    },
};






CMDLINELIST standard_media_commandlinelist[] = {
	   {  "A4"   ,   "-pt A4"       },
	   {  "Letter"   ,   "-pt LT"       },
	   {  "Legal"   ,   "-pt LGL"       },
	   {  "Executive"   ,   "-pt EXE"       },


	   {  "A5"   ,   "-pt A5"       },
	   {  "B5"   ,   "-pt B5"       },
	   {  "DL"   ,   "-pt DL"       },
	   {  "EnvDL"   ,   "-pt DL"       },
	   {  "Com10"   ,   "-pt COM-10"       },
	   {  "COM10"   ,   "-pt COM-10"       },
	   {  "Com-10"   ,   "-pt COM-10"       },
	   {  "COM-10"   ,   "-pt COM-10"       },
	   {  "EnvCOM10"   ,   "-pt COM-10"       },
	   {  "Env10"   ,   "-pt COM-10"       },
	   
	   {  "PlainPaper"   ,   "-md PLAIN"       },
	   {  "ThinPaper"   ,   "-md THIN"       },
	   {  "ThickPaper"   ,   "-md THICK"       },
	   {  "ThickerPaper"   ,   "-md THICKER"       },
	   {  "Transparencies"   ,   "-md TRANS"       },
	   {  "Recycled"   ,   "-md RECY"       },
	   
	   {  "Manual"   ,   "-feed MANUAL"       },
	   {  "Tray1"   ,   "-feed TRAY1"       },
	   {  "Tray2"   ,   "-feed TRAY2"       },
	   {  "AutoSelect"   ,   "-feed AUTO"       },
	   { NULL   ,   NULL    }
};

#endif  
