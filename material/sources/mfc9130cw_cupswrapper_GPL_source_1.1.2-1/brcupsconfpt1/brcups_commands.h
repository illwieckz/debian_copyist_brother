/* $Id: brcups_commands.h,v 1.11 2005/08/22 02:20:49 cvs Exp $ */
//* Brother CUPS wrapper tool
//* Copyright (C) 2005-2013 Brother. Industries, Ltd.//*
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
	{  "BRCollate=OFF"			,	"-collate OFF" },
	{  "BRCollate=ON"			,	"-collate ON" },
	
	// Media
	{  "BRMediaType=Plain"		,	"-md Plain" },
	{  "BRMediaType=Thin"		,	"-md Thin" },
	{  "BRMediaType=Thick"		,	"-md Thick" },
	{  "BRMediaType=Thicker"	,	"-md Thicker" },
	{  "BRMediaType=BOND"		,	"-md BOND" },
	{  "BRMediaType=Recycled"	,	"-md Recycled" },
	{  "BRMediaType=Env"		,	"-md Env" },
	{  "BRMediaType=EnvThin"	,	"-md EnvThin" },
	{  "BRMediaType=EnvThick"	,	"-md EnvThick" },
	{  "BRMediaType=PostCard"	,	"-md PostCard" },
	{  "BRMediaType=Label"		,	"-md Label" },
	{  "BRMediaType=Glossy"		,	"-md Glossy" },

 // RResolution
	{  "BRResolution=600dpi"		,	"-reso Normal" },
	{  "BRResolution=600x2400dpi"	,	"-reso Fine" },

	//InputSlot
	{  "BRInputSlot=AutoSelect"	,	"-inslot AutoSelect" },
	{  "BRInputSlot=Tray1"		,	"-inslot Tray1" },
	{  "BRInputSlot=Manual"		,	"-inslot Manual" },

	// BRMonoColor
    {  "BRMonoColor=Auto"		,	"-corm Auto" },
    {  "BRMonoColor=FullColor"	,	"-corm FullColor" },
    {  "BRMonoColor=Mono"		,	"-corm Mono" },

	// Paper (Brother command)
	{ "papersize=A4"		,	"-pt A4" },
	{ "papersize=Letter"	,	"-pt Letter" },
	{ "papersize=Legal"		,	"-pt Legal" },
	{ "papersize=Executive"	,	"-pt Executive" },
	{ "papersize=A5"		,	"-pt A5" },
	{ "papersize=A5Rotated"	,	"-pt PRA5Rotated" },
	{ "papersize=A6"		,	"-pt A6" },
	{ "papersize=B5"		,	"-pt ISOB5" },
	{ "papersize=JISB5"		,	"-pt JISB5" },
	{ "papersize=JISB6"		,	"-pt JISB6" },

	{ "papersize=EnvDL"					,	"-pt EnvDL" },
	{ "papersize=EnvC5"					,	"-pt EnvC5" },
	{ "papersize=Env10"					,	"-pt Env10" },
	{ "papersize=EnvMonarch"			,	"-pt EnvMonarch" },
	{ "papersize=Br3x5"					,	"-pt Br3x5" },
	{ "papersize=FanFoldGermanLegal"	,	"-pt FanFoldGermanLegal" },
	{ "papersize=EnvPRC5Rotated"		,	"-pt EnvPRC5Rotated" },
	{ "papersize=Postcard"				,	"-pt Postcard" },
	{ "papersize=EnvYou4"				,	"-pt EnvYou4" },
	{ "papersize=EnvChou3"				,	"-pt EnvChou3" },
	
	{ "papersize=210x270mm"				,	"-pt 210x270mm" },
	{ "papersize=195x270mm"				,	"-pt 195x270mm" },
	{ "papersize=184x260mm"				,	"-pt 184x260mm" },
	{ "papersize=197x273mm"				,	"-pt 197x273mm" },

	// For OpenOffice
	{ "PageSize=A4"						,	"-pt A4" },
	{ "PageSize=Letter"					,	"-pt Letter" },
	{ "PageSize=Legal"					,	"-pt Legal" },
	{ "PageSize=Executive"				,	"-pt Executive" },
	{ "PageSize=A5"						,	"-pt A5" },
	{ "PageSize=A5Rotated"				,	"-pt PRA5Rotated" },
	{ "PageSize=A6"						,	"-pt A6" },
	{ "PageSize=B5"						,	"-pt ISOB5" },
	{ "PageSize=JISB5"					,	"-pt JISB5" },
	{ "PageSize=JISB6"					,	"-pt JISB6" },

	{ "PageSize=EnvDL"					,	"-pt EnvDL" },
	{ "PageSize=EnvC5"					,	"-pt EnvC5" },
	{ "PageSize=Env10"					,	"-pt Env10" },
	{ "PageSize=EnvMonarch"				,	"-pt EnvMonarch" },
	{ "PageSize=Br3x5"					,	"-pt Br3x5" },
	{ "PageSize=FanFoldGermanLegal"		,	"-pt FanFoldGermanLegal" },
	{ "PageSize=EnvPRC5Rotated"			,	"-pt EnvPRC5Rotated" },
	{ "PageSize=Postcard"				,	"-pt Postcard" },
	{ "PageSize=EnvYou4"				,	"-pt EnvYou4" },
	{ "PageSize=EnvChou3"				,	"-pt EnvChou3" },
	{ "PageSize=210x270mm"				,	"-pt 210x270mm" },
	{ "PageSize=195x270mm"				,	"-pt 195x270mm" },
	{ "PageSize=184x260mm"				,	"-pt 184x260mm" },
	{ "PageSize=197x273mm"				,	"-pt 197x273mm" },

	// Gray
	{ "BRGray=OFF"	, "-gray OFF" },               
	{ "BRGray=ON"	, "-gray ON" },

	// ColorMatch
	{ "BRColorMatching=Normal"	, "-cm Normal" },
	{ "BRColorMatching=Vivid"	, "-cm Vivid" },
	{ "BRColorMatching=None"	, "-cm None" },

	// ToneSave
	{ "BRTonerSaveMode=OFF"	, "-ts OFF" },
	{ "BRTonerSaveMode=ON"	, "-ts ON" },

	// BRImproveOutput
	{ "BRImproveOutput=OFF"				, "-improve OFF" },
	{ "BRImproveOutput=BRLessPaperCurl"	, "-improve BRLessPaperCurl" },
	{ "BRImproveOutput=BRFixIntensity"	, "-improve BRFixIntensity" },

	// BREnhanceBlkPrt
	{ "BREnhanceBlkPrt=OFF"	, "-bp OFF" },
	{ "BREnhanceBlkPrt=ON"	, "-bp ON" },

 	//BRReversePrint
  	{"BRReverse=ON"		, "-rev ON"},
  	{"BRReverse=OFF"	, "-rev OFF"},

    // Duplex
    {  "BRDuplex=None"				,	"-duplex None"  },
    {  "BRDuplex=DuplexTumble"		,	"-duplex DuplexTumble"  },
    {  "BRDuplex=DuplexNoTumble"	,	"-duplex DuplexNoTumble"  },

    // Skip Blank Page
    {  "BRSkipBlank=OFF"	, "-sb OFF" },
    {  "BRSkipBlank=ON"		, "-sb ON" },
    
	{ NULL   ,   NULL    },
};

// +++++++++++++++++++++++++++
// command line   (Standard command)
// +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	// for Fedora OpenOffice
/*	{ "media=A4",					"-pt A4" },
	{ "media=Letter",				"-pt Letter" },
	{ "media=Legal",				"-pt Legal" },
	{ "media=Executive",			"-pt Executive" },
	{ "media=A5",					"-pt A5" },
	{ "media=PRA5Rotated",			"-pt PRA5Rotated" },
	{ "media=A6",					"-pt A6" },
	{ "media=B5",					"-pt B5" },
	{ "media=B6",					"-pt B6" },
	{ "media=JISB5",				"-pt JISB5" },
	{ "media=JISB6",				"-pt JISB6" },
	{ "media=EnvDL",				"-pt EnvDL" },
	{ "media=EnvC5",				"-pt EnvC5" },
	{ "media=Env10",				"-pt Env10" },
	{ "media=EnvMonarch",			"-pt EnvMonarch" },
	{ "media=Br3x5",				"-pt Br3x5" },
	{ "media=BrA4Long",				"-pt BrA4Long" },
	{ "media=FanFoldGermanLegal",	"-pt FanFoldGermanLegal" },
	{ "media=EnvPRC5Rotated",		"-pt EnvPRC5Rotated" },
	{ "media=PostCard",				"-pt PostCard" },
	{ "media=EnvYou4",				"-pt EnvYou4" },
	{ "media=EnvChou3",				"-pt EnvChou3" },
*/
	{ NULL	,	NULL },
};

// +++++++++++++++++++++++++++
// command line numerical value
// +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	{	"BRBrightness="	,	"-brit "	},
	{	"BRContrast="	,	"-cont "	},
	{   "BRRed="		,	"-red "		},
	{   "BRGreen="		,	"-green "	},
	{   "BRBlue="		,	"-blue "	},
	{   "BRSaturation="	,	"-satu "	},
	{   NULL			,	NULL		},
};

/* PPD Default   */
// +++++++++++++++++++++++++++
// PPD setting list
// +++++++++++++++++++++++++++
typedef struct {
    char *value;
    char *brcommand;
} PPDCOMMANDLIST;

typedef   struct   {
   char				*label;
   PPDCOMMANDLIST	ppdcommandlist[];
}  PPDCOMMAND;

// ***** DefaultResolution *****
PPDCOMMAND  DefaultBRResolution={
  "DefaultBRResolution",
	{
		{ "600dpi"		, "-reso Normal" },
		{ "600x2400dpi"	, "-reso Fine" },
		{ NULL , NULL  }
	}
};

// ***** DefaultPageSize *****
PPDCOMMAND  DefaultPageSize={                                  
	"DefaultPageSize",
	{
		{ "A4"					,	"-pt A4" },
		{ "Letter"				,	"-pt Letter" },
		{ "Legal"				,	"-pt Legal" },
		{ "Executive"			,	"-pt Executive" },
		{ "A5"					,	"-pt A5" },
		{ "A5Rotated"			,	"-pt PRA5Rotated" },
		{ "A6"					,	"-pt A6" },
		{ "B5"					,	"-pt ISOB5" },
		//{ "B6"				,	"-pt ISOB6" },
		{ "JISB5"				,	"-pt JISB5" },
		{ "JISB6"				,	"-pt JISB6" },
		{ "EnvDL"				,	"-pt EnvDL" },
		{ "EnvC5"				,	"-pt EnvC5" },
		{ "Env10"				,	"-pt Env10" },
		{ "EnvMonarch"			,	"-pt EnvMonarch" },
		{ "Br3x5"				,	"-pt Br3x5" },
		//	{ "BrA4Long"		,	"-pt BrA4Long" },
		{ "FanFoldGermanLegal"	,	"-pt FanFoldGermanLegal" },
		{ "EnvPRC5Rotated"		,	"-pt EnvPRC5Rotated" },
		{ "Postcard"			,	"-pt Postcard" },
		{ "EnvYou4"				,	"-pt EnvYou4" },
		{ "EnvChou3"			,	"-pt EnvChou3" },
		{ "210x270mm"			,	"-pt 210x270mm" },
		{ "195x270mm"			,	"-pt 195x270mm" },
		{ "184x260mm"			,	"-pt 184x260mm" },
		{ "197x273mm"			,	"-pt 197x273mm" },
		{ NULL					,	NULL }
	}
};

// ***** DefaultBRMonoColor*****
PPDCOMMAND  DefaultBRMonoColor={
	"DefaultBRMonoColor",
	{
		{  "Auto"		,	"-corm Auto"	},
		{  "FullColor"	,	"-corm FullColor"	},
		{  "Mono"		,	"-corm Mono"	},
		{ NULL			,	NULL	}
	}
};

// ***** DefaultBRGray*****

PPDCOMMAND  DefaultBRGray={
	"DefaultBRGray",
	{
		{  "OFF"	,	"-gray OFF"	},
		{  "ON"		,	"-gray ON"	},
		{ NULL 		, NULL  }
	}
};


// ***** DefaultBRColorMediaType*****
PPDCOMMAND  DefaultBRMediaType={
	"DefaultBRMediaType",
	{
		{  "Plain"		,	"-md Plain" },
		{  "Thin"		,	"-md Thin" },
		{  "Thick"		,	"-md Thick" },
		{  "Thicker"	,	"-md Thicker" },
		{  "BOND"		,	"-md BOND" },
		{  "Recycled"	,	"-md Recycled" },
		{  "Env"		,	"-md Env" },
		{  "EnvThin"	,	"-md EnvThin" },
		{  "EnvThick"	,	"-md EnvThick" },
		{  "PostCard"	,	"-md PostCard" },
		{  "Label"		,	"-md Label" },
		{  "Glossy"		,	"-md Glossy" },
		{ NULL			, NULL  }
	}
};

// ******DefaultBRColorMatching**********
PPDCOMMAND  DefaultBRColorMatching={
	"DefaultBRColorMatching",
	{
		{ "Normal"	, "-cm Normal" },
		{ "Vivid"	, "-cm Vivid" },
		{ "None"	, "-cm None" },
		{ NULL 		, NULL  }
	}
};

// ******DefaultBRHalfTonePattern**********
/* PPDCOMMAND  DefaultBRHalfTonePattern={
	"DefaultBRHalfTonePattern",
	{

		{ "Diffusion"	, "-ht Diffusion" },
		{ "Dither"		, "-ht Dither" },
		{ NULL 			, NULL  }
	}
}; */

// ******DefaultTonerSaveMode**********
PPDCOMMAND  DefaultBRTonerSaveMode={
	"DefaultBRTonerSaveMode",
	{
		{ "OFF"	, "-ts OFF" },
		{ "ON"	, "-ts ON" },
		{ NULL , NULL  }
	}
};

// ******DefaultBRImproveOutput**********
PPDCOMMAND  DefaultBRImproveOutput={
	"DefaultBRImproveOutput",
	{
		{ "OFF"				, "-improve OFF" },
		{ "BRLessPaperCurl"	, "-improve BRLessPaperCurl" },
		{ "BRFixIntensity"	, "-improve BRFixIntensity" },
		{ NULL , NULL  }
	}
};  

// ******DefaultBRColorEnhancement**********
PPDCOMMAND  DefaultBRColorEnhancement={
	"DefaultBREnhanceBlkPrt",
	{
		{  "OFF"	,	"-bp OFF"	},
		{  "ON"		,	"-bp ON"	},
		{ NULL , NULL  }
	}
};

// ******DefaultBRInputSlot**********
PPDCOMMAND  DefaultBRInputSlot={
	"DefaultBRInputSlot",
	{
		{  "AutoSelect"	,	"-inslot AutoSelect" },
		{  "Tray1"		,	"-inslot Tray1" },
		{  "Manual"		,	"-inslot Manual" },
		{ NULL , NULL  }
	}
};

// ******DefaultBRReversePrint**********
PPDCOMMAND  DefaultBRReverse={
	"DefaultBRReverse",
	{
		{  "OFF"	,	"-rev OFF"	},
		{  "ON"		,	"-rev ON"	},
		{ NULL , NULL  }
	}
};
// ******DefaultBRJpegPrint**********
/* PPDCOMMAND  DefaultBRJpeg={
	"DefaultBRJpeg",
	{
		{  "Recommended"	,	"-jpeg Recommended"	},
		{  "QualityPrior"	,	"-jpeg QualityPrior"	},
		{  "SpeedPrior"		,	"-jpeg SpeedPrior"	},
		{ NULL , NULL  }
	}
}; */

// ******DefaultBRDuplex**********
PPDCOMMAND DefaultBRDuplex={
	"DefaultBRDuplex",
	{
		{ "None"			,	"-duplex None" },
		{ "DuplexTumble"	,	"-duplex DuplexTumble" },
		{ "DuplexNoTumble"	,	"-duplex DuplexNoTumble" },
		{ NULL	,	NULL }
	}
}; 
// ******DefaultBRManualDuplex**********
/* PPDCOMMAND DefaultManualDuplex={
	"DefaultDuplex",
	{
		{ "off"		,	"-Duplex off" },
		{ "even"	,	"-Duplex even" },
		{ "odd"		,	"-Duplex odd" },
		{ NULL		,	NULL }
	}
}; */

PPDCOMMAND DefaultBRSkipBlank={
	"DefaultBRSkipBlank",
	{
		{ "ON"	,	"-sb ON" },
		{ "OFF"	,	"-sb OFF" },
		{ NULL	,	NULL }
	}
};

PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultBRInputSlot,

	&DefaultBRResolution,
	&DefaultPageSize,
	&DefaultBRMonoColor,
	&DefaultBRGray,
	&DefaultBRMediaType,
//	&DefaultBRColorPaperThick,
//	&DefaultBRBiDir,
//	&DefaultBrMirrorPrint,
//	&DefaultBRDocument,
	&DefaultBRColorMatching,
	&DefaultBRTonerSaveMode,
	&DefaultBRImproveOutput,
	&DefaultBRColorEnhancement,
 // &DefaultBRJpeg,
	&DefaultBRReverse,
	&DefaultBRDuplex,
	&DefaultBRSkipBlank,
//  &DefaultBRManualDuplex,
	NULL
};
/* PPD Default   end*/
// +++++++++++++++++++++++++++
// PPD numerical value
// +++++++++++++++++++++++++++

CMDLINELIST PPDdefaultN[] = {
	{"DefaultBRBrightness", "-brit "},
	{"DefaultBRContrast", "-cont "},
	{"DefaultBRRed", "-red "},
	{"DefaultBRGreen", "-green "},
	{"DefaultBRBlue", "-blue "},
	{"DefaultBRSaturation", "-satu "},
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

// +++++++++++++++++++++++++++
// command line   (Standard command)
// +++++++++++++++++++++++++++
CMDLINELIST standard_media_commandlinelist[] = {
/*	{  "A4"				,	"-pt A4" },
	{  "Letter"			,	"-pt LT" },
	{  "Legal"			,	"-pt LGL" },
	{  "Executive"		,	"-pt EXE" },
	{  "A5"				,	"-pt A5" },
//	{  "A6"				,	"-pt A6" },
	{  "B5"				,	"-pt B5" },
//	{  "B6"				,	"-pt B6" },
//	{  "C5"				,	"-pt C5" },
//	{  "EnvC5"			,	"-pt C5" },
	{  "DL"				,	"-pt DL" },
	{  "EnvDL"			,	"-pt DL" },
	{  "Com10"			,	"-pt COM-10" },
	{  "COM10"			,	"-pt COM-10" },
	{  "Com-10"			,	"-pt COM-10" },
	{  "COM-10"			,	"-pt COM-10" },
	{  "EnvCOM10"		,	"-pt COM-10" },
	{  "Env10"			,	"-pt COM-10" },
//	{  "Monarch"		,	"-pt MONARCH" },
//	{  "EnvMonarch"		,	"-pt MONARCH" },
	{  "PlainPaper"		,	"-md PLAIN" },
	{  "ThinPaper"		,	"-md THIN" },
	{  "ThickPaper"		,	"-md THICK" },
	{  "ThickerPaper"	,	"-md THICKER" },
//	{  "BondPaper"		,	"-md BOND" },
	{  "Transparencies"	,	"-md TRANS" },
	{  "Recycled"		,	"-md RECY" },
//	{  "Envelopes"		,	"-md ENV" },
//	{  "Env.Thick"		,	"-md ENV-THICK" },
//	{  "Env.Thin"		,	"-md ENV-THIN" },
	{  "Manual"			,	"-feed MANUAL" },
//	{  "MPTray"			,	"-feed MP" },
	{  "Tray1"			,	"-feed TRAY1" },
	{  "Tray2"			,	"-feed TRAY2" },
	{  "AutoSelect"		,	"-feed AUTO" },
*/	{ NULL				,	NULL }
};

#endif  //_BRCUPS_COMMANDS_H_
