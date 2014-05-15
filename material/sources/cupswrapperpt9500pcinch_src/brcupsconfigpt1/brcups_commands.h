/* $Id: brcups_commands.h,v 1.11 2005-2006/08/22 02:20:49 cvs Exp $ */
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





char brprintconf[30]="brprintconf";
//   +++++++++++++++++++++++++++
//       set default
//   +++++++++++++++++++++++++++
char *default_setting[] = {
		// Collate
		"brprintconf -P BROTHERPRINTER_XXX  -collate OFF"   ,
		// copies
		"brprintconf -P BROTHERPRINTER_XXX  -copy 1",
		// Chain
		"brprintconf -P BROTHERPRINTER_XXX  -chain OFF",
		// Bringhtness
		"brprintconf -P BROTHERPRINTER_XXX  -brit 0"   ,
		// Contrast
		"brprintconf -P BROTHERPRINTER_XXX  -cont 0"   ,
		// Halftone
		"brprintconf -P BROTHERPRINTER_XXX  -half ErrorDiffusion"   ,
		// Auto cut
		"brprintconf -P BROTHERPRINTER_XXX  -hcut ON"   ,
		// Auto cut
		"brprintconf -P BROTHERPRINTER_XXX  -cut OFF"   ,
		// Mirror
		"brprintconf -P BROTHERPRINTER_XXX  -mirro OFF"   ,
		// Resolution
		"brprintconf -P BROTHERPRINTER_XXX  -reso 360"   ,
		// Margin(Feed)
		"brprintconf -P BROTHERPRINTER_XXX  -margin 1",
		// Media
		"brprintconf -P BROTHERPRINTER_XXX  -media brS12mm",
	     
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
	   // Collate
	   {  "Collate=OFF"			,	"-collate OFF"      },
	   {  "Collate=ON"			,	"-collate ON"      },

	   // Chain
	   {  "Chain=OFF"			,	"-chain OFF"      },
	   {  "Chain=ON"				,	"-chain ON"       },
	   {  "noChain"			    	,	"-chain OFF"      }, // for debian
	   {  "Chain"				,	"-chain ON"       }, // for debian

	   // HalftonePattern
	   {  "Halftone=ERROR"   ,       	"-half ErrorDiffusion"},
	   {  "Halftone=BINARY"   ,		"-half Binary"},
	   {  "Halftone=DITHER"   ,  		"-half Dither"},

	   // HalfCut
	   {  "HalfCut=OFF"			,	"-hcut OFF"      },
	   {  "HalfCut=ON"				,	"-hcut ON"       },
	   {  "noHalfCut"			    ,	"-hcut OFF"      }, // for debian
	   {  "HalfCut"				,	"-hcut ON"       }, // for debiDefaultBrChainan

	   // AutoCut
	   {  "AutoCut=OFF"			,	"-cut OFF"      },

	   {  "AutoCut=ON"				,	"-cut ON"       },
	   {  "noAutoCut"			    ,	"-cut OFF"      }, // for debian
	   {  "AutoCut"				,	"-cut ON"       }, // for debian

       // Mirror
   	   {  "Mirror=OFF"		,   "-mirro OFF"	},
   	   {  "Mirror=ON"		,   "-mirro ON"	},
   	   {  "noMirror"			,   "-mirro OFF"	},  // for debian
   	   {  "Mirror"			,   "-mirro ON"	},      // for debian
	   	   


	   // Quality
//	   {  "Priority=SPEED"   ,       	"-quality SPEED"},
//	   {  "Priority=QUALITY"   ,		"-quality QUALITY"},

	   // Resolution
	   {  "Resolution=360"		,   "-reso 360"       	},
//	   {  "Resolution=720"		,   "-reso 720"       	},
      
	   { NULL   ,   NULL    },
};


//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	// Paper Size
	{  "media=brS6mm"   	,   "-media brS6mm"       },
	{  "media=brS9mm"   	,   "-media brS9mm"       },
	{  "media=brS12mm"   	,   "-media brS12mm"       },
	{  "media=brS18mm"   	,   "-media brS18mm"       },
	{  "media=brS24mm"   	,   "-media brS24mm"       },
	{  "media=brS36mm"   	,   "-media brS36mm"       },
/*
	{  "media=brS6X2"   	,   "-media brS6X2"       },
	{  "media=brS9X2"   	,   "-media brS9X2"       },
	{  "media=brS12X2"   	,   "-media brS12X2"       },
	{  "media=brS18X2"   	,   "-media brS18X2"       },
	{  "media=brS24X2"   	,   "-media brS24X2"       },
	{  "media=brS36X2"   	,   "-media brS36X2"       },

	{  "media=brS6X3"   	,   "-media brS6X3"       },
	{  "media=brS9X3"   	,   "-media brS9X3"       },
	{  "media=brS12X3"   	,   "-media brS12X3"       },
	{  "media=brS18X3"   	,   "-media brS18X3"       },
	{  "media=brS24X3"   	,   "-media brS24X3"       },
	{  "media=brS36X3"   	,   "-media brS36X3"       },

	{  "media=brS6X4"   	,   "-media brS6X4"       },
	{  "media=brS9X4"   	,   "-media brS9X4"       },
	{  "media=brS12X4"   	,   "-media brS12X4"       },
	{  "media=brS18X4"   	,   "-media brS18X4"       },
	{  "media=brS24X4"   	,   "-media brS24X4"       },
	{  "media=brS36X4"   	,   "-media brS36X4"       },
*/
	{  "media=brSAV1789"   	,   "-media brSAV1789"       },
	{  "media=brSAV1957"   	,   "-media brSAV1957"       },
	{  "media=brSAV2067"   	,   "-media brSAV2067"       },

	// for Fedora OpenOffice
/*	{  "PageSize=17x54"	,   "-media 17x54"     },
	{  "PageSize=17x87"	,   "-media 17x87"     },
	{  "PageSize=29x90"	,   "-media 29x90"     },
	{  "PageSize=38x90"	,   "-media 38x90"     },

	{  "PageSize=62x29"	,   "-media 62x29"     },
	{  "PageSize=62x100"	,   "-media 62x100"   },
	{  "PageSize=12Dia"   	,   "-media 12Dia"     },
	{  "PageSize=24Dia"   	,   "-media 24Dia"       },
	{  "PageSize=58Dia"   	,   "-media 58Dia"       },
	{  "PageSize=12X1"   	,   "-media 12X1"       },
	{  "PageSize=29X1"   	,   "-media 29X1"       },
	{  "PageSize=62X1"   	,   "-media 62X1"       },
	{  "PageSize=12X2"   	,   "-media 12X2"       },
	{  "PageSize=29X2"   	,   "-media 29X2"       },
	{  "PageSize=62X2"   	,   "-media 62X2"       },
	{  "PageSize=12X3"   	,   "-media 12X3"       },
	{  "PageSize=29X3"   	,   "-media 29X3"       },
	{  "PageSize=62X3"   	,   "-media 62X3"       },
	{  "PageSize=12X4"   	,   "-media 12X4"       },
	{  "PageSize=29X4"   	,   "-media 29X4"       },
	{  "PageSize=62X4"   	,   "-media 62X4"       },
*/
	{ NULL   ,   NULL    },
};

//   +++++++++++++++++++++++++++
//       command line numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	{	"Copies="   	,   "-copy "      },
	{	"Brightness="	,	"-brit "	},
	{	"Contrast="		,	"-cont "		},
	{	"Margin="		,	"-margin "		},
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
  "DefaultResolution",
  {
  		{ "Standard"  , "-reso 360"  },
 	    { NULL , NULL  }
  }
};

//       ***** DefaultPageSize *****
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    {
	{  "brS6mm"   	,   "-media brS6mm"       },
	{  "brS9mm"   	,   "-media brS9mm"       },
	{  "brS12mm"   	,   "-media brS12mm"       },
	{  "brS18mm"   	,   "-media brS18mm"       },
	{  "brS24mm"   	,   "-media brS24mm"       },
	{  "brS36mm"   	,   "-media brS36mm"       },
  /*
	{  "brS6X2"   	,   "-media brS6X2"       },
	{  "brS9X2"   	,   "-media brS9X2"       },
	{  "brS12X2"   	,   "-media brS12X2"       },
	{  "brS18X2"   	,   "-media brS18X2"       },
	{  "brS24X2"   	,   "-media brS24X2"       },
	{  "brS36X2"   	,   "-media brS36X2"       },

	{  "brS6X3"   	,   "-media brS6X3"       },
	{  "brS9X3"   	,   "-media brS9X3"       },
	{  "brS12X3"   	,   "-media brS12X3"       },
	{  "brS18X3"   	,   "-media brS18X3"       },
	{  "brS24X3"   	,   "-media brS24X3"       },
	{  "brS36X3"   	,   "-media brS36X3"       },

	{  "brS6X4"   	,   "-media brS6X4"       },
	{  "brS9X4"   	,   "-media brS9X4"       },
	{  "brS12X4"   	,   "-media brS12X4"       },
	{  "brS18X4"   	,   "-media brS18X4"       },
	{  "brS24X4"   	,   "-media brS24X4"       },
	{  "brS36X4"   	,   "-media brS36X4"       },
*/
	{  "brSAV1789"   	,   "-media brSAV1789"       },
	{  "brSAV1957"   	,   "-media brSAV1957"       },
	{  "brSAV2067"   	,   "-media brSAV2067"       },
       { NULL , NULL  }
     }
};


//       ***** DefaultBrAutoTapeCut*****
PPDCOMMAND  DefaultBrHalfCut={
    "DefaultBrHalfCut",
    {
 	    { "OFF", "-hcut OFF"  },
 	    { "ON" , "-hcut ON"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBrAutoTapeCut*****
PPDCOMMAND  DefaultBrAutoTapeCut={
    "DefaultBrAutoTapeCut",
    {
 	    { "OFF", "-cut OFF"  },
 	    { "ON" , "-cut ON"  },
 	    { NULL , NULL  }
     }

};

//       ***** DefaultBRMirror*****
PPDCOMMAND  DefaultBrMirror={
    "DefaultBrMirror",
    {
 	    { "OFF", "-mirro OFF"  },
 	    { "ON" , "-mirro ON"  },
 	    { NULL , NULL  }
     }
};


//       ***** DefaultBrChain *****
PPDCOMMAND  DefaultBrChain={

    "DefaultBrChain",
    {
 	    { "OFF", "-chain OFF"  },
 	    { "ON" , "-chain ON"  },
 	    { NULL , NULL  }
     }
};

//    ******DefaultBrHalftonePattern**********
PPDCOMMAND  DefaultBrHalftonePattern={

	"DefaultBrHalftonePattern",
	{
	   {  "ErrorDiffusion"   ,	"-half ErrorDiffusion"},
	   {  "Binary"   ,			"-half Binary"},
	   {  "Dither"   ,  			"-half Dither"},
	   { NULL , NULL  }


     }
};

PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultResolution,
	&DefaultPageSize,
	&DefaultBrAutoTapeCut,
	&DefaultBrHalfCut,
	&DefaultBrMirror,
	&DefaultBrChain,
	&DefaultBrHalftonePattern,
      NULL
};
/*      PPD Default   end*/
//   +++++++++++++++++++++++++++
//       PPD numerical value
//   +++++++++++++++++++++++++++

CMDLINELIST PPDdefaultN[] = {
	{"DefaultBrBrightness", "-brit "},
	{"DefaultBrContrast", "-cont "},
	{"DefaultBrMargin", "-margin "},
	{ NULL   ,   NULL    },
};

//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_media_commandlinelist[] = {
/*	   {  "A4"   ,   "-pt A4"       },
	   {  "Letter"   ,   "-pt LT"       },
	   {  "Legal"   ,   "-pt LGL"       },
	   {  "Executive"   ,   "-pt EXE"       },


	   {  "A5"   ,   "-pt A5"       },
//	   {  "A6"   ,   "-pt A6"       },
	   {  "B5"   ,   "-pt B5"       },
//	   {  "B6"   ,   "-pt B6"       },
//	   {  "C5"   ,   "-pt C5"       },
//	   {  "EnvC5"   ,   "-pt C5"       },
	   {  "DL"   ,   "-pt DL"       },

	   {  "EnvDL"   ,   "-pt DL"       },
	   {  "Com10"   ,   "-pt COM-10"       },
	   {  "COM10"   ,   "-pt COM-10"       },
	   {  "Com-10"   ,   "-pt COM-10"       },
	   {  "COM-10"   ,   "-pt COM-10"       },
	   {  "EnvCOM10"   ,   "-pt COM-10"       },
	   {  "Env10"   ,   "-pt COM-10"       },
//	   {  "Monarch"   ,   "-pt MONARCH"       },
//	   {  "EnvMonarch"   ,   "-pt MONARCH"       },
	   
	   {  "PlainPaper"   ,   "-md PLAIN"       },
	   {  "ThinPaper"   ,   "-md THIN"       },
	   {  "ThickPaper"   ,   "-md THICK"       },
	   {  "ThickerPaper"   ,   "-md THICKER"       },
//	   {  "BondPaper"   ,   "-md BOND"       },
	   {  "Transparencies"   ,   "-md TRANS"       },
	   {  "Recycled"   ,   "-md RECY"       },
	   
//	   {  "Envelopes"   ,   "-md ENV"       },
//	   {  "Env.Thick"   ,   "-md ENV-THICK"       },
//	   {  "Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "Manual"   ,   "-feed MANUAL"       },
//	   {  "MPTray"   ,   "-feed MP"       },
	   {  "Tray1"   ,   "-feed TRAY1"       },
	   {  "Tray2"   ,   "-feed TRAY2"       },

	   {  "AutoSelect"   ,   "-feed AUTO"       },
*/	   { NULL   ,   NULL    }
};

#define _LABEL_BUF_MAX_ 	33
#define _LABEL_MAX_ 	(_LABEL_BUF_MAX_ - 1)
#define _LABLE_ID_HEAD_				"BrL"

typedef struct
{
	char tape_name[_LABEL_BUF_MAX_];
	char tape_id[_LABEL_BUF_MAX_];
} TapeItem, *TTapeItem;
typedef struct
{
	TTapeItem tapelist;
	int count;
	int iSelIndex;
}TapeList, *TTapeList;


int add_tape(TTapeList ptapelist, const char* plabelname, const char* plabel_id);
int GetLabel_name_id(const char* lpString, char* lpName, char* lpID);

#endif  //_BRCUPS_COMMANDS_H_
