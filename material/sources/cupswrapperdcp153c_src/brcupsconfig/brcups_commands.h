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
#define _SL4C_





char brprintconf[30]="brprintconf";
/*
//   +++++++++++++++++++++++++++
//       set default
//   +++++++++++++++++++++++++++

char *default_setting[] = {
		// Collate
		"brprintconf -P BROTHERPRINTER_XXX  -collate OFF"   ,
		// copies
		"brprintconf -P BROTHERPRINTER_XXX  -copy 1",
		// Chain
		"brprintconf -P BROTHERPRINTER_XXX  -md Plain",
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
		"brprintconf -P BROTHERPRINTER_XXX  -reso Normal"   ,
		// Margin(Feed)
		"brprintconf -P BROTHERPRINTER_XXX  -margin 1",
		// Media
		"brprintconf -P BROTHERPRINTER_XXX  -media brS12mm",
	     
	     NULL
};






*/
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

	// Media
	{  "BRColorMediaType=Plain"			,	"-md Plain"      },
	{  "BRColorMediaType=Inkjet"			,	"-md Inkjet"      },
	{  "BRColorMediaType=BrotherGlossy"	,	"-md BrotherGlossy"      },
	{  "BRColorMediaType=Glossy"			,	"-md Glossy"      },
	{  "BRColorMediaType=Transparencies"	,	"-md Transparencies"      },


	// RResolution
	{  "Resolution=Draft"	,	"-reso Draft"      },
	{  "Resolution=Fast"	,	"-reso Fast"      },
	{  "Resolution=Normal"	,	"-reso Normal"      },
	{  "Resolution=Fine"	,	"-reso Fine"      },
	{  "Resolution=Photo"	,	"-reso Photo"      },
	{  "Resolution=Highest"	,	"-reso Highest"      },

	// BiDirPrint
	{  "BRBiDir=OFF"	,	"-bidir OFF"	},
	{  "BRBiDir=ON"	,	"-bidir ON"	},
	{  "noBRBiDir"	,	"-bidir OFF"	},
	{  "BRBiDir"	,	"-bidir ON"	},

	// Mirror
	{  "MirrorPrint=OFF"	,	"-mirro OFF"	},
	{  "MirrorPrint=ON"	,	"-mirro ON"	},
	{  "NoMirrorPrint"	,	"-mirro OFF"	},
	{  "MirrorPrint"	,	"-mirro ON"	},

	// BRMonoColor
	{  "BRMonoColor=BrColor"	,	"-corm Color"	},
	{  "BRMonoColor=BrMono"	,	"-corm Mono"	},

	// Paper
	{ "PageSize=A4",				"-pt A4" },
       { "PageSize=BrA4_B",			"-pt BrA4_B" },
	{ "PageSize=Letter",			"-pt Letter" },
	{ "PageSize=BrLetter_B",		"-pt BrLetter_B" },
	{ "PageSize=Legal",			"-pt Legal" },
	{ "PageSize=Executive",		"-pt Executive" },
	{ "PageSize=B5",				"-pt B5" },
	{ "PageSize=A5",				"-pt A5" },
	{ "PageSize=A6",				"-pt A6" },
	{ "PageSize=BrA6_B",			"-pt BrA6_B" },
	{ "PageSize=PostC4x6",		"-pt PostC4x6" },
	{ "PageSize=BrPostC4x6_B",	"-pt BrPostC4x6_B" },
	{ "PageSize=IndexC5x8",		"-pt IndexC5x8" },
	{ "PageSize=BrIndexC5x8_B",	"-pt BrIndexC5x8_B" },
	{ "PageSize=PhotoL",		"-pt PhotoL" },
       { "PageSize=BrPhotoL_B",		"-pt BrPhotoL_B" },
       { "PageSize=Photo2L",		"-pt Photo2L" },
       { "PageSize=BrPhoto2L_B",		"-pt BrPhoto2L_B" },
       { "PageSize=PostCard",		"-pt PostCard" },
       { "PageSize=BrHagaki_B",		"-pt BrHagaki_B" },
       { "PageSize=DoublePostCardRotated",	"-pt DoublePostCardRotated" },
       { "PageSize=EnvC5",				"-pt EnvC5" },
       { "PageSize=EnvDL",				"-pt EnvDL" },
       { "PageSize=Env10",				"-pt Env10" },
       { "PageSize=EnvMonarch",			"-pt EnvMonarch" },
       { "PageSize=EnvYou4",				"-pt EnvYou4" },
#if 1		// Added , 2006.10.17
	// for Command Line
	{ "media=A4",				"-pt A4" },
       { "media=BrA4_B",			"-pt BrA4_B" },
	{ "media=Letter",			"-pt Letter" },
	{ "media=BrLetter_B",		"-pt BrLetter_B" },
	{ "media=Legal",			"-pt Legal" },
	{ "media=Executive",		"-pt Executive" },
	{ "media=B5",				"-pt B5" },
	{ "media=A5",				"-pt A5" },
	{ "media=A6",				"-pt A6" },
	{ "media=BrA6_B",			"-pt BrA6_B" },
	{ "media=PostC4x6",		"-pt PostC4x6" },
	{ "media=BrPostC4x6_B",	"-pt BrPostC4x6_B" },
	{ "media=IndexC5x8",		"-pt IndexC5x8" },
	{ "media=BrIndexC5x8_B",	"-pt BrIndexC5x8_B" },
	{ "media=PhotoL",		"-pt PhotoL" },
       { "media=BrPhotoL_B",		"-pt BrPhotoL_B" },
       { "media=Photo2L",		"-pt Photo2L" },
       { "media=BrPhoto2L_B",		"-pt BrPhoto2L_B" },
       { "media=PostCard",		"-pt PostCard" },
       { "media=BrHagaki_B",		"-pt BrHagaki_B" },
       { "media=DoublePostCardRotated",	"-pt DoublePostCardRotated" },
       { "media=EnvC5",				"-pt EnvC5" },
       { "media=EnvDL",				"-pt EnvDL" },
       { "media=Env10",				"-pt Env10" },
       { "media=EnvMonarch",			"-pt EnvMonarch" },
       { "media=EnvYou4",				"-pt EnvYou4" },
#endif	// 1
	// Thick
	{ "BRColorPaperThick=Regular", "-thick Regular" },
	{ "BRColorPaperThick=Thick", "-thick Thick" },

	// Thick
	{ "BRDocument=Photo", "-doc Photo" },
	{ "BRDocument=Graphics", "-doc Graphics" },
	{ "BRDocument=custom", "-doc Custom" },

	// ColorMatch
	{ "BRColorMatching=Natural", "-cm Natural" },
	{ "BRColorMatching=Vivid", "-cm Vivid" },
	{ "BRColorMatching=None", "-cm None" },

	// BRHalfTonePattern
	{ "BRHalfTonePattern=Diffusion", "-ht Diffusion" },
	{ "BRHalfTonePattern=Dither", "-ht Dither" },

	// BRColorEnhancement
	{ "BRColorEnhancement=OFF", "-ce OFF" },
	{ "BRColorEnhancement=ON", "-ce ON" },

	// BRSlowDrying
	{ "BRSlowDrying=OFF", "-sd OFF" },
	{ "BRSlowDrying=ON", "-sd ON" },

	// 	BRInputSlot
	{  "BRInputSlot=AUTO"	,	"-feed AUTO"	},
	{  "BRInputSlot=Tray1"	,	"-feed Tray1"	},
	{  "BRInputSlot=Tray2"	,	"-feed Tray2"	},

	{ NULL   ,   NULL    },
};



//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	// for Fedora OpenOffice
/*	{ "PageSize=A4",				"-pt A4" },
       { "PageSize=BrA4_B",			"-pt BrA4_B" },
	{ "PageSize=Letter",			"-pt Letter" },
	{ "PageSize=BrLetter_B",		"-pt BrLetter_B" },
	{ "PageSize=Legal",			"-pt Legal" },
	{ "PageSize=Executive",		"-pt Executive" },
	{ "PageSize=B5",				"-pt B5" },
	{ "PageSize=A5",				"-pt A5" },

	{ "PageSize=A6",				"-pt A6" },
	{ "PageSize=BrA6_B",			"-pt BrA6_B" },
	{ "PageSize=PostC4x6",		"-pt PostC4x6" },
	{ "PageSize=BrPostC4x6_B",	"-pt BrPostC4x6_B" },
	{ "PageSize=IndexC5x8",		"-pt IndexC5x8" },
	{ "PageSize=BrIndexC5x8_B",	"-pt BrIndexC5x8_B" },
	{ "PageSize=PhotoL",		"-pt PhotoL" },
       { "PageSize=BrPhotoL_B",		"-pt BrPhotoL_B" },
       { "PageSize=Photo2L",		"-pt Photo2L" },
       { "PageSize=BrPhoto2L_B",		"-pt BrPhoto2L_B" },
       { "PageSize=PostCard",		"-pt PostCard" },
       { "PageSize=BrHagaki_B",		"-pt BrHagaki_B" },
       { "PageSize=DoublePostCardRotated",	"-pt DoublePostCardRotated" },
       { "PageSize=EnvC5",				"-pt EnvC5" },
       { "PageSize=EnvDL",				"-pt EnvDL" },
       { "PageSize=Env10",				"-pt Env10" },
       { "PageSize=EnvMonarch",			"-pt EnvMonarch" },
       { "PageSize=EnvYou4",				"-pt EnvYou4" },

*/	{ NULL   ,   NULL    },
};

//   +++++++++++++++++++++++++++
//       command line numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	{	"Copies="   	,   "-copy "      },
	{	"BRBrightness="	,	"-brit "	},
	{	"BRContrast="		,	"-cont "		},
	{	"Margin="		,	"-margin "		},
	{   "BRRedkey="		,	"-red "		},
	{   "BRGreenkey="		,	"-green "		},
	{   "BRBluekey="		,	"-blue "		},
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
		{ "Draft"  , "-reso Draft"  },
		{ "Fast"  , "-reso Fast"  },
		{ "Normal"  , "-reso Normal"  },
		{ "Fine"  , "-reso Fine"  },
		{ "Photo"  , "-reso Photo"  },
		{ "Highest"  , "-reso Highest"  },
		{ NULL , NULL  }
	}
};

//       ***** DefaultPageSize *****
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    {
	{ "A4",				"-pt A4" },
       { "BrA4_B",			"-pt BrA4_B" },
	{ "Letter",				"-pt Letter" },
	{ "BrLetter_B",		"-pt BrLetter_B" },
	{ "Legal",				"-pt Legal" },
	{ "Executive",			"-pt Executive" },
	{ "B5",				"-pt B5" },
	{ "A5",				"-pt A5" },
	{ "A6",				"-pt A6" },
	{ "BrA6_B",			"-pt BrA6_B" },
	{ "PostC4x6",		"-pt PostC4x6" },
	{ "BrPostC4x6_B",		"-pt BrPostC4x6_B" },
	{ "IndexC5x8",		"-pt IndexC5x8" },
	{ "BrIndexC5x8_B",	"-pt BrIndexC5x8_B" },
	{ "PhotoL",			"-pt PhotoL" },
       { "BrPhotoL_B",		"-pt BrPhotoL_B" },
       { "Photo2L",			"-pt Photo2L" },
       { "BrPhoto2L_B",		"-pt BrPhoto2L_B" },
       { "PostCard",			"-pt PostCard" },
       { "BrHagaki_B",		"-pt BrHagaki_B" },
       { "DoublePostCardRotated",	"-pt DoublePostCardRotated" },
       { "EnvC5",				"-pt EnvC5" },
       { "EnvDL",				"-pt EnvDL" },
       { "Env10",				"-pt Env10" },
       { "EnvMonarch",			"-pt EnvMonarch" },
       { "EnvYou4",				"-pt EnvYou4" },
       { NULL , NULL  }
     }
};


//       ***** DefaultBRMonoColor*****
PPDCOMMAND  DefaultBRMonoColor={
    "DefaultBRMonoColor",
    {
		{  "BrColor"	,	"-corm Color"	},
		{  "BrMono"	,	"-corm Mono"	},
 	    	{ NULL , NULL  }
     }
};


//       ***** DefaultBRSlowDrying*****

PPDCOMMAND  DefaultBRSlowDrying={
    "DefaultBRSlowDrying",
    {
 	    { "OFF", "-sd OFF"  },
 	    { "ON" , "-sd ON"  },
 	    { NULL , NULL  }
     }
};

//       ***** DefaultBRColorMediaType*****
PPDCOMMAND  DefaultBRColorMediaType={
    "DefaultBRColorMediaType",
    {
	{  "Plain"			,	"-md Plain"      },
	{  "Inkjet"			,	"-md Inkjet"      },
	{  "BrotherGlossy"	,	"-md BrotherGlossy"      },
	{  "Glossy"		,	"-md Glossy"      },
	{  "Transparencies"	,	"-md Transparencies"      },
 	{ NULL , NULL  }
     }
};


//       ***** DefaultBRColorPaperThick *****
PPDCOMMAND  DefaultBRColorPaperThick={
    "DefaultBRColorPaperThick",
    {
	{ "Regular", "-thick Regular" },
	{ "Thick", "-thick Thick" },
 	{ NULL , NULL  }
     }
};

//    ******DefaultBRBiDir**********
PPDCOMMAND  DefaultBRBiDir={
	"DefaultBRBiDir",
	{
		{  "OFF"	,	"-bidir OFF"	},
		{  "ON"	,	"-bidir ON"	},
		{ NULL , NULL  }
	}
};

//    ******DefaultMirrorPrint**********
PPDCOMMAND  DefaultMirrorPrint={
	"DefaultMirrorPrint",
	{
		{  "OFF"	,	"-mirro OFF"	},
		{  "ON"	,	"-mirro ON"	},
		{ NULL , NULL  }
	}
};

//    ******DefaultBRDocument**********
PPDCOMMAND  DefaultBRDocument={
	"DefaultBRDocument",
	{
		{ "Photo", "-doc Photo" },
		{ "Graphics", "-doc Graphics" },
		{ "custom", "-doc Custom" },
		{ NULL , NULL  }
	}
};

//    ******DefaultBRColorMatching**********
PPDCOMMAND  DefaultBRColorMatching={
	"DefaultBRColorMatching",
	{
		{ "Natural", "-cm Natural" },
		{ "Vivid", "-cm Vivid" },
		{ "None", "-cm None" },
		{ NULL , NULL  }
	}
};

//    ******DefaultBRHalfTonePattern**********
PPDCOMMAND  DefaultBRHalfTonePattern={
	"DefaultBRHalfTonePattern",
	{
		{ "Diffusion", "-ht Diffusion" },
		{ "Dither", "-ht Dither" },
		{ NULL , NULL  }
	}
};

//    ******DefaultBRColorEnhancement**********
PPDCOMMAND  DefaultBRColorEnhancement={
	"DefaultBRColorEnhancement",
	{
		{  "OFF"	,	"-ce OFF"	},
		{  "ON"	,	"-ce ON"	},
		{ NULL , NULL  }
	}
};

//    ******DefaultBRInputSlot**********
PPDCOMMAND  DefaultBRInputSlot={
	"DefaultBRInputSlot",
	{
		{  "AUTO"	,	"-feed AUTO"	},
		{  "Tray1"	,	"-feed Tray1"	},
		{  "Tray2"	,	"-feed Tray2"	},
		{ NULL , NULL  }
	}
};



PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultBRInputSlot,
	&DefaultResolution,
	&DefaultPageSize,
	&DefaultBRMonoColor,
	&DefaultBRSlowDrying,
	&DefaultBRColorMediaType,
	&DefaultBRColorPaperThick,
	&DefaultBRBiDir,
	&DefaultMirrorPrint,
	&DefaultBRDocument,
	&DefaultBRColorMatching,
	&DefaultBRHalfTonePattern,
	&DefaultBRColorEnhancement,
      NULL
};
/*      PPD Default   end*/
//   +++++++++++++++++++++++++++
//       PPD numerical value
//   +++++++++++++++++++++++++++

CMDLINELIST PPDdefaultN[] = {
	{"DefaultBRBrightness", "-brit "},
	{"DefaultBRContrast", "-cont "},
	{"DefaultBRRed", "-red "},
	{"DefaultBRGreen", "-green "},
	{"DefaultBRBlue", "-blue "},
	{ NULL   ,   NULL    },
};

/*
CMDLINELIST PPDdefaultNValue[] = {
		{"DefaultBRBrightness", "-brit "},
		{"DefaultBRContrast", "-cont "},


		{"DefaultBRRed", "-red "},
		{"DefaultBRGreen", "-green "},
		{"DefaultBRBlue", "-blue "},
		{"DefaultBRSaturation", "-saturation "},
	   { NULL   ,   NULL    },
};

*/





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

#endif  //_BRCUPS_COMMANDS_H_
