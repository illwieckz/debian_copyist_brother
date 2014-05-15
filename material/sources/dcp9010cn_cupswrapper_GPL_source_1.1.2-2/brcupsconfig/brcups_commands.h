/* $Id: brcups_commands.h,v 1.11 2005/08/22 02:20:49 cvs Exp $ */
//* Brother CUPS wrapper tool
//* Copyright (C) 2005-2011 Brother. Industries, Ltd.//*
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
	{  "Collate=ON"			,	"-collate OFF"      },    
  // Media                                               
  {  "MediaType=Plain"			,	"-md Plain"      },
	{  "MediaType=Thin"			,	"-md Thin"      },
  {  "MediaType=Thick"	,	"-md Thick"      },
  {  "MediaType=Thicker"	,	"-md Thicker"      },
  {  "MediaType=BOND"			,	"-md BOND"      },
  {  "MediaType=Recycled"	,	"-md Recycled"      },
  {  "MediaType=Env"	,	"-md Env"      },
  {  "MediaType=EnvThin"	,	"-md EnvThin"      },
  {  "MediaType=EnvThick"	,	"-md EnvThick"      },
  {  "MediaType=PostCard"	,	"-md PostCard"      },
  {  "MediaType=Label"	,	"-md Label"      },

 // RResolution
	{  "BRResolution=600dpi"	,	"-reso Normal"      },
	{  "BRResolution=600x2400dpi"	,	"-reso Fine"      },

	// Mirror
/*	{  "MirrorPrint=OFF"	,	"-mirro OFF"	},
	{  "MirrorPrint=ON"	,	"-mirro ON"	},
	{  "NoMirrorPrint"	,	"-mirro OFF"	},
	{  "MirrorPrint"	,	"-mirro ON"	},      */

	//InputSlot
	{  "InputSlot=AutoSelect",	"-inslot AutoSelect"	},
	{  "InputSlot=Tray1"	,	"-inslot Tray1"	},
	{  "InputSlot=Manual"	,	"-inslot Manual"	},

	// BRMonoColor
  {  "BRMonoColor=Auto"	,	"-corm Auto"	},
	{  "BRMonoColor=FullColor"	,	"-corm Color"	},
	{  "BRMonoColor=Mono"	,	"-corm Mono"	},

	// Paper (Brother command)
	{ "papersize=A4",				"-pt A4" },
	{ "papersize=Letter",				"-pt Letter" },
	{ "papersize=Legal",				"-pt Legal" },
	{ "papersize=Executive",				"-pt Executive" },
	{ "papersize=A5",				"-pt A5" },
	{ "papersize=PRA5Rotated",				"-pt PRA5Rotated" },
	{ "papersize=A6",				"-pt A6" },
	{ "papersize=B5",				"-pt B5" },
	{ "papersize=B6",				"-pt B6" },
	{ "papersize=JISB5",				"-pt JISB5" },
	{ "papersize=JISB6",				"-pt JISB6" },

	{ "papersize=EnvDL",				"-pt EnvDL" },
	{ "papersize=EnvC5",				"-pt EnvC5" },
	{ "papersize=Env10",				"-pt Env10" },
	{ "papersize=EnvMonarch",				"-pt EnvMonarch" },
	{ "papersize=Br3x5",			"-pt Br3x5" },
	{ "papersize=BrA4Long",		"-pt BrA4Long" },
	{ "papersize=FanFoldGermanLegal",				"-pt FanFoldGermanLegal" },
	{ "papersize=EnvPRC5Rotated",				"-pt EnvPRC5Rotated" },
	{ "papersize=PostCard",				"-pt PostCard" },
	{ "papersize=EnvYou4",			"-pt EnvYou4" },
	{ "papersize=EnvChou3",		"-pt EnvChou3" },

	// For OpenOffice
	{ "PageSize=A4",				"-pt A4" },
	{ "PageSize=Letter",				"-pt Letter" },
	{ "PageSize=Legal",				"-pt Legal" },
	{ "PageSize=Executive",				"-pt Executive" },
	{ "PageSize=A5",				"-pt A5" },
	{ "PageSize=PRA5Rotated",				"-pt PRA5Rotated" },
	{ "PageSize=A6",				"-pt A6" },
	{ "PageSize=B5",				"-pt B5" },
	{ "PageSize=B6",				"-pt B6" },
	{ "PageSize=JISB5",				"-pt JISB5" },
	{ "PageSize=JISB6",				"-pt JISB6" },

	{ "PageSize=EnvDL",				"-pt EnvDL" },
	{ "PageSize=EnvC5",				"-pt EnvC5" },
	{ "PageSize=Env10",				"-pt Env10" },
	{ "PageSize=EnvMonarch",				"-pt EnvMonarch" },
	{ "PageSize=Br3x5",			"-pt Br3x5" },
	{ "PageSize=BrA4Long",		"-pt BrA4Long" },
	{ "PageSize=FanFoldGermanLegal",				"-pt FanFoldGermanLegal" },
	{ "PageSize=EnvPRC5Rotated",				"-pt EnvPRC5Rotated" },
	{ "PageSize=PostCard",				"-pt PostCard" },
	{ "PageSize=EnvYou4",			"-pt EnvYou4" },
	{ "PageSize=EnvChou3",		"-pt EnvChou3" },


/*	// for Command Line
  { "media=A4",				"-pt A4" },
  { "media=Letter",				"-pt Letter" },
  { "media=Legal",				"-pt Legal" },
  { "media=Executive",				"-pt Executive" },
  { "media=A5",				"-pt A5" },
  { "media=A5Rotated",				"-pt PRA5Rotated" },
  { "media=A6",				"-pt A6" },
  { "media=B5",				"-pt B5" },
  { "media=B6",				"-pt B6" },
  { "media=B5",				"-pt B5" },
  { "media=B6",				"-pt B6" },
  { "media=EnvDL",				"-pt EnvDL" },
  { "media=EnvC5",				"-pt EnvC5" },
  { "media=Env10",				"-pt Env10" },
  { "media=EnvMonarch",				"-pt EnvMonarch" },
	{ "media=Br3x5",			"-pt Br3x5" },
	{ "media=BrA4Long",		"-pt BrA4Long" },
	{ "media=FanFoldGermanLegal",				"-pt FanFoldGermanLegal" },
	{ "media=EnvPRC5Rotated",				"-pt EnvPRC5Rotated" },
	{ "media=PostCard",				"-pt PostCard" },
	{ "media=EnvYou4",			"-pt EnvYou4" },
	{ "media=EnvChou3",		"-pt EnvChou3" },
*/

// Gray
	{ "BRGray=OFF", "-gray OFF" },               
	{ "BRGray=ON", "-gray ON" },


	// ColorMatch
	{ "BRColorMatching=Normal", "-cm Normal" },
	{ "BRColorMatching=Vivid", "-cm Vivid" },
	{ "BRColorMatching=None", "-cm None" },

	// ToneSave
	{ "TonerSaveMode=OFF", "-ts OFF" },
	{ "TonerSaveMode=ON", "-ts ON" },

	// BRImproveOutput
	{ "BRImproveOutput=OFF", "-output OFF" },
	{ "BRImproveOutput=BRLessPaperCurl", "-output BRLessPaperCurl" },
	{ "BRImproveOutput=BRFixIntensity", "-output BRFixIntensity" },


	// BREnhanceBlkPrt
	{ "BREnhanceBlkPrt=OFF", "-ce OFF" },

	{ "BREnhanceBlkPrt=ON", "-ce ON" },

 	//BRReversePrint
  	{"BRReverse=ON","-reve ON"},
  	{"BRReverse=OFF","-reve OFF"},
    // Duplex
/*    {  "Duplex=None"				,	"-duplex None"  },
    {  "Duplex=DuplexTumble"		,	"-duplex DuplexTumble"  },
    {  "Duplex=DuplexNoTumble"	,	"-duplex DuplexNoTumble"  },     */
    // ManualDuplex
//    {  "ManualDuplex=off"				,	"-mandup off"  },
//    {  "ManualDuplex=even"		,	"-mandup even"  },
//    {  "ManualDuplex=odd"	,	"-mandup odd"  },

	{ NULL   ,   NULL    },
};



//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	// for Fedora OpenOffice
	{ "media=A4",				"-pt A4" },
  { "media=Letter",				"-pt Letter" },
  { "media=Legal",				"-pt Legal" },
  { "media=Executive",				"-pt Executive" },
  { "media=A5",				"-pt A5" },
  { "media=PRA5Rotated",				"-pt PRA5Rotated" },
  { "media=A6",				"-pt A6" },
  { "media=B5",				"-pt B5" },
  { "media=B6",				"-pt B6" },
  { "media=JISB5",				"-pt JISB5" },
  { "media=JISB6",				"-pt JISB6" },
  { "media=EnvDL",				"-pt EnvDL" },
  { "media=EnvC5",				"-pt EnvC5" },
  { "media=Env10",				"-pt Env10" },
  { "media=EnvMonarch",				"-pt EnvMonarch" },
	{ "media=Br3x5",			"-pt Br3x5" },
	{ "media=BrA4Long",		"-pt BrA4Long" },
	{ "media=FanFoldGermanLegal",				"-pt FanFoldGermanLegal" },
	{ "media=EnvPRC5Rotated",				"-pt EnvPRC5Rotated" },
	{ "media=PostCard",				"-pt PostCard" },
	{ "media=EnvYou4",			"-pt EnvYou4" },
	{ "media=EnvChou3",		"-pt EnvChou3" },

	{ NULL   ,   NULL    },
};

//   +++++++++++++++++++++++++++
//       command line numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	{	"Copies="   	,   "-copy "      },
	{	"BRBrightness="	,	"-brit "	},
	{	"BRContrast="		,	"-cont "		},
	{   "BRRed="		,	"-red "		},
	{   "BRGreen="		,	"-green "		},
	{   "BRBlue="		,	"-blue "		},
	{   "BRSaturation="		,	"-satu "		},   
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
		{ "600dpi"  , "-reso Normal"  },
		{ "600x2400dpi"  , "-reso Fine"  },
		{ NULL , NULL  }
	}
};

//       ***** DefaultPageSize *****
PPDCOMMAND  DefaultPageSize={                                  
    "DefaultPageSize",
  {
	{ "A4",				"-pt A4" },
  { "Letter",				"-pt Letter" },
  { "Legal",				"-pt Legal" },
  { "Executive",				"-pt Executive" },
  { "A5",				"-pt A5" },
  { "PRA5Rotated",				"-pt PRA5Rotated" },
  { "A6",				"-pt A6" },
  { "B5",				"-pt B5" },
  { "B6",				"-pt B6" },
  { "JISB5",				"-pt JISB5" },
  { "JISB6",				"-pt JISB6" },
  { "EnvDL",				"-pt EnvDL" },
  { "EnvC5",				"-pt EnvC5" },
  { "Env10",				"-pt Env10" },
  { "EnvMonarch",				"-pt EnvMonarch" },
	{ "Br3x5",			"-pt Br3x5" },
	{ "BrA4Long",		"-pt BrA4Long" },
	{ "FanFoldGermanLegal",				"-pt FanFoldGermanLegal" },
	{ "EnvPRC5Rotated",				"-pt EnvPRC5Rotated" },
	{ "PostCard",				"-pt PostCard" },
	{ "EnvYou4",			"-pt EnvYou4" },
	{ "EnvChou3",		"-pt EnvChou3" },

       { NULL , NULL  }
     }
};


//       ***** DefaultBRMonoColor*****

PPDCOMMAND  DefaultBRMonoColor={
    "DefaultBRMonoColor",
    {
		{  "Auto"	,	"-corm Auto"	},
		{  "FullColor"	,	"-corm Color"	},
		{  "Mono"	,	"-corm Mono"	},
 	    	{ NULL , NULL  }
     }
};

//       ***** DefaultBRGray*****

PPDCOMMAND  DefaultBRGray={
	"DefaultBRGray",
	{
		{  "OFF"	,	"-gray OFF"	},
		{  "ON"	,	"-gray ON"	},
		{ NULL , NULL  }
	}
};


//       ***** DefaultBRColorMediaType*****
PPDCOMMAND  DefaultBRColorMediaType={
    "DefaultMediaType",
    {
	{  "Plain"			,	"-md Plain"      },
	{  "Thin"			,	"-md Thin"      },
  {  "Thick"	,	"-md Thick"      },
  {  "Thicker"	,	"-md Thicker"      },                    
  {  "BOND"			,	"-md BOND"      },
  {  "Recycled"	,	"-md Recycled"      },
  {  "Env"	,	"-md Env"      },
  {  "EnvThin"	,	"-md EnvThin"      },
  {  "EnvThick"	,	"-md EnvThick"      },
  {  "PostCard"	,	"-md PostCard"      },
  {  "Label"	,	"-md Label"      },

 	{ NULL , NULL  }
     }
};


//       ***** DefaultBRColorPaperThick *****
/*PPDCOMMAND  DefaultBRColorPaperThick={
    "DefaultBRColorPaperThick",
    {
	{ "Regular", "-thick Regular" },
	{ "Thick", "-thick Thick" },
 	{ NULL , NULL  },
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
};   */

//    ******DefaultBrMirror**********
/* PPDCOMMAND  DefaultBrMirrorPrint={
	"DefaultMirrorPrint",
	{
		{  "OFF"	,	"-mirro OFF"	},
		{  "ON"	,	"-mirro ON"	},
		{ NULL , NULL  }
	}
};  */

//    ******DefaultBRDocument**********
/* PPDCOMMAND  DefaultBRDocument={


	"DefaultBRDocument",
	{
		{ "Photo", "-doc Photo" },
		{ "Graphics", "-doc Graphics" },
		{ "custom", "-doc Custom" },
		{ NULL , NULL  }
	}
};    */

//    ******DefaultBRColorMatching**********
PPDCOMMAND  DefaultBRColorMatching={
	"DefaultBRColorMatching",
	{
		{ "Normal", "-cm Normal" },

		{ "Vivid", "-cm Vivid" },
		{ "None", "-cm None" },
		{ NULL , NULL  }
	}
};

//    ******DefaultBRHalfTonePattern**********
/* PPDCOMMAND  DefaultBRHalfTonePattern={
	"DefaultBRHalfTonePattern",
	{

		{ "Diffusion", "-ht Diffusion" },
		{ "Dither", "-ht Dither" },
		{ NULL , NULL  }
	}
};     */

//    ******DefaultTonerSaveMode**********
PPDCOMMAND  DefaultTonerSaveMode={
	"DefaultTonerSaveMode",
	{

		{ "OFF", "-ts OFF" },
		{ "ON", "-ts ON" },
		{ NULL , NULL  }
	}
};

//    ******DefaultBRImproveOutput**********
PPDCOMMAND  DefaultBRImproveOutput={
	"DefaultBRImproveOutput",
	{
    { "OFF", "-output OFF" },
		{ "BRLessPaperCurl", "-output BRLessPaperCurl" },
		{ "BRFixIntensity", "-output BRFixIntensity" },
		{ NULL , NULL  }
	}
};  

//    ******DefaultBRColorEnhancement**********
PPDCOMMAND  DefaultBRColorEnhancement={
	"DefaultBREnhanceBlkPrt",
	{
		{  "OFF"	,	"-ce OFF"	},
		{  "ON"	,	"-ce ON"	},
		{ NULL , NULL  }
	}
};

//    ******DefaultBRInputSlot**********
PPDCOMMAND  DefaultBRInputSlot={
	"DefaultInputSlot",
	{
		{  "AutoSelect"	,	"-inslot AutoSelect"	},

		{  "Tray1"	,	"-inslot Tray1"	},
		{  "Manual"	,	"-inslot Manual"	},
		{ NULL , NULL  }
	}
};

//    ******DefaultBRReversePrint**********
PPDCOMMAND  DefaultBRReverse={
	"DefaultBRReverse",
	{
		{  "OFF"	,	"-reve OFF"	},
		{  "ON"	,	"-reve ON"	},

		{ NULL , NULL  }
	}
};
//    ******DefaultBRJpegPrint**********
/* PPDCOMMAND  DefaultBRJpeg={
	"DefaultBRJpeg",
	{
		{  "Recommended"	,	"-jpeg Recommended"	},
		{  "QualityPrior"	,	"-jpeg QualityPrior"	},
		{  "SpeedPrior"	,	"-jpeg SpeedPrior"	},
		{ NULL , NULL  }
    
	}
};    */

//    ******DefaultBRDuplex**********
/* PPDCOMMAND DefaultDuplex={
	"DefaultDuplex",
	{
		{ "None"	,	"-duplex None" },
		{ "DuplexTumble"	,	"-duplex DuplexTumble" },
		{ "DuplexNoTumble"	,	"-duplex DuplexNoTumble" },
		{ NULL	,	NULL }
	}
};     */
//    ******DefaultBRManualDuplex**********
/* PPDCOMMAND DefaultManualDuplex={
	"DefaultDuplex",
	{
		{ "off"	,	"-Duplex off" },
		{ "even"	,	"-Duplex even" },
		{ "odd"	,	"-Duplex odd" },
		{ NULL	,	NULL }
	}
};      */


PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultBRInputSlot,
	&DefaultResolution,
	&DefaultPageSize,
	&DefaultBRMonoColor,
	&DefaultBRGray,
	&DefaultBRColorMediaType,
//	&DefaultBRColorPaperThick,
//	&DefaultBRBiDir,
//	&DefaultBrMirrorPrint,
//	&DefaultBRDocument,
	&DefaultBRColorMatching,
	&DefaultTonerSaveMode,
	&DefaultBRImproveOutput,
	&DefaultBRColorEnhancement,
 // &DefaultBRJpeg,
  &DefaultBRReverse,
//  &DefaultDuplex,
//  &DefaultBRManualDuplex,
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
