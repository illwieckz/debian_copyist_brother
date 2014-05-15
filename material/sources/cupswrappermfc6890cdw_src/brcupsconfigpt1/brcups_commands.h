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


#ifndef _INK4_    /*INK4 2008.4.17*/
#define _INK4_
#endif   /*_INK4_*/




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
#ifdef _INK4_    /*INK4 2008.4.17*/
    {  "BRColorMediaType=Plain"			,	"-md Plain"      },
	{  "BRColorMediaType=Inkjet"			,	"-md Inkjet"      },
  {  "BRColorMediaType=BrotherGlossyR"	,	"-md BrotherGlossyR"      },
  {  "BRColorMediaType=BrotherGlossy"	,	"-md BrotherGlossy"      },
  {  "BRColorMediaType=Glossy"			,	"-md Glossy"      },
  {  "BRColorMediaType=Transparencies"	,	"-md Transparencies"      },
#else      /*_INK4_*/
  {  "BRColorMediaType=Plain"			,	"-md Plain"      },
	{  "BRColorMediaType=Inkjet"			,	"-md Inkjet"      },
	{  "BRColorMediaType=BrotherGlossy"	,	"-md BrotherGlossy"      },
	{  "BRColorMediaType=Glossy"			,	"-md Glossy"      },
	{  "BRColorMediaType=Transparencies"	,	"-md Transparencies"      },

#endif     /*_INK4_*/
	
 // RResolution
	{  "Resolution=150x150dpi"	,	"-reso 150x150dpi"      },
	{  "Resolution=300x300dpi"	,	"-reso 300x300dpi"      },
	{  "Resolution=600x600dpi"	,	"-reso 600x600dpi"      },
	{  "Resolution=1200x1200dpi"	,	"-reso 1200x1200dpi"      },
	{  "Resolution=1200x2400dpi"	,	"-reso 1200x2400dpi"      },
	{  "Resolution=1200x6000dpi"	,	"-reso 1200x6000dpi"      },

	// BiDirPrint
	{  "BRBiDir=OFF"	,	"-bidir OFF"	},
	{  "BRBiDir=ON"	,	"-bidir ON"	},
	{  "noBRBiDir"	,	"-bidir OFF"	},
	{  "BRBiDir"	,	"-bidir ON"	},

	// Mirror
#ifdef _INK4_    /*INK4. Modified by Chenyipan. 20080519*/
	{  "MirrorPrint=OFF"	,	"-mirro False"	},
	{  "MirrorPrint=ON"	,	"-mirro True"	},
	{  "noBrMirror"	,	"-mirro False"	},	// For Debian
	{  "BrMirror"	,	"-mirro True"	},		// For Debian
#else
	{  "MirrorPrint=OFF"	,	"-mirro OFF"	},
	{  "MirrorPrint=ON"	,	"-mirro ON"	},
	{  "NoMirrorPrint"	,	"-mirro OFF"	},
	{  "MirrorPrint"	,	"-mirro ON"	},
#endif

#ifdef _INK4_    /*INK4. Modified by Chenyipan. 20080701*/
	{  "BRInputSlot=AutoSelect",	"-feed AutoSelect"	},
	{  "BRInputSlot=Tray1"	,	"-feed Tray1"	},
	{  "BRInputSlot=Tray2"	,	"-feed Tray2"	},
#endif

	// BRMonoColor
  {  "BRMonoColor=Color"	,	"-corm Color"	},
	{  "BRMonoColor=Mono"	,	"-corm Mono"	},


// Paper


#ifdef _INK4_        /*INK4 2008.4.17*/

  { "PageSize=BrA3_S",				"-pt BrA3_S" },
  { "PageSize=BrA3_A3",				"-pt BrA3_A3" },
  { "PageSize=BrA3_B_S",				"-pt BrA3_B_S" },
  { "PageSize=BrA3_B_A3",				"-pt BrA3_B_A3" }, /*INK4 2008.4.17*/
  { "PageSize=B4",				"-pt B4" },
  { "PageSize=BrB4_S",				"-pt BrB4_S" },/*INK4 2008.4.17*/
  { "PageSize=BrB4_B_B4",				"-pt BrB4_B_B4" },
  { "PageSize=BrB4_B_S",				"-pt BrB4_B_S" },
  { "PageSize=Tabloid",				"-pt Tabloid" },
  { "PageSize=BrLedger_S",				"-pt BrLedger_S" },
  { "PageSize=BrLedger_B_Ledger",				"-pt BrLedger_B_Ledger" },
  { "PageSize=BrLedger_B_S",				"-pt BrLedger_B_S" },
#endif     /*_INK4_*/
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
       { "PageSize=Postcard",		"-pt Postcard" },
       { "PageSize=BrHagaki_B",		"-pt BrHagaki_B" },
       { "PageSize=DoublePostcardRotated",	"-pt DoublePostcardRotated" },
       { "PageSize=EnvC5",				"-pt EnvC5" },
       { "PageSize=EnvDL",				"-pt EnvDL" },
       { "PageSize=Env10",				"-pt Env10" },
       { "PageSize=EnvMonarch",			"-pt EnvMonarch" },
       { "PageSize=EnvYou4",				"-pt EnvYou4" },
#if 1		// 2006.10.17
	// for Command Line
#ifdef _INK4_    /*_INK4_*/    /*2008.4.28*/
  { "media=BrA3_S",				"-pt BrA3_S" },
  { "media=BrA3_A3",				"-pt BrA3_A3" },
  { "media=BrA3_B_S",				"-pt BrA3_B_S" },
  { "media=BrA3_B_A3",				"-pt BrA3_B_A3" }, /*INK4 2008.4.17*/
  { "media=B4",				"-pt B4" },
  { "media=BrB4_S",				"-pt BrB4_S" },/*INK4 2008.4.17*/
  { "media=BrB4_B_B4",				"-pt BrB4_B_B4" },
  { "media=BrB4_B_S",				"-pt BrB4_B_S" },
  { "media=Tabloid",				"-pt Tabloid" },
  { "media=BrLedger_S",				"-pt BrLedger_S" },
  { "media=BrLedger_B_Ledger",				"-pt BrLedger_B_Ledger" },
  { "media=BrLedger_B_S",				"-pt BrLedger_B_S" },
#endif    /*_INK4_*/
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
       { "media=Postcard",		"-pt Postcard" },
       { "media=BrHagaki_B",		"-pt BrHagaki_B" },
       { "media=DoublePostcardRotated",	"-pt DoublePostcardRotated" },
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
	{ "BRDocument=Custom", "-doc Custom" },

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
#ifdef _INK4_   /*INK4 2008.4.17*/
  	//BRJpegPrint
  	{"BRJpeg=Recommended", "-jpeg Recommended" },
  	{"BRJpeg=QualityPrior", "-jpeg QualityPrior" },
  	{"BRJpeg=SpeedPrior", "-jpeg SpeedPrior" },
 	//BRReversePrint
  	{"BRReverse=ON","-rev ON"},
  	{"BRReverse=OFF","-rev OFF"},
   
    // Duplex
    {  "BRDuplex=None"				,	"-duplex None"  },
    {  "BRDuplex=DuplexTumble"		,	"-duplex DuplexTumble"  },
    {  "BRDuplex=DuplexNoTumble"	,	"-duplex DuplexNoTumble"  },

    // Manul Duplex
    {  "BRManualDuplex=Even"	,	"-manualduplex Even"  },
    {  "BRManualDuplex=Odd"		,	"-manualduplex Odd"  },
    {  "BRManualDuplex=OFF"		,	"-manualduplex OFF"  },

    // Advanced Duplex Mode
    {  "BRDuplexMode=Normal"	,	"-advduplexmode Normal"  },
    {  "BRDuplexMode=Duplex1"	,	"-advduplexmode Duplex1"  },
    {  "BRDuplexMode=Duplex2"	,	"-advduplexmode Duplex2"  },

    // ManualDuplex
//    {  "ManualDuplex=off"				,	"-mandup off"  },
//    {  "ManualDuplex=even"		,	"-mandup even"  },
//    {  "ManualDuplex=odd"	,	"-mandup odd"  },


#endif /*_INK4_*/     /*INK4 2008.4.17*/

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
       { "PageSize=Postcard",		"-pt Postcard" },
       { "PageSize=BrHagaki_B",		"-pt BrHagaki_B" },

       { "PageSize=DoublePostcardRotated",	"-pt DoublePostcardRotated" },
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
	{   "BRRed="		,	"-red "		},
	{   "BRGreen="		,	"-green "		},
	{   "BRBlue="		,	"-blue "		},
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

		{ "150x150dpi"  , "-reso 150x150dpi"  },
		{ "300x300dpi"  , "-reso 300x300dpi"  },
		{ "600x600dpi"  , "-reso 600x600dpi"  },
		{ "1200x1200dpi"  , "-reso 1200x1200dpi"  },
		{ "1200x2400dpi"  , "-reso 1200x2400dpi"  },
		{ "1200x6000dpi"  , "-reso 1200x6000dpi"  },
		{ NULL , NULL  }
	}
};

//       ***** DefaultPageSize *****
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
  {
#ifdef _INK4_   /*INK4 2008.4.17*/
  { "BrA3_S",				"-pt BrA3_S" },
  { "BrA3_A3",				"-pt BrA3_A3" },
  { "BrA3_B_S",				"-pt BrA3_B_S" },
  { "BrA3_B_A3",				"-pt BrA3_B_A3" }, /*INK4 2008.4.17*/
  { "B4",				"-pt B4" },
  { "BrB4_S",				"-pt BrB4_S" },/*INK4 2008.4.17*/
  { "BrB4_B_B4",				"-pt BrB4_B_B4" },
  { "BrB4_B_S",				"-pt BrB4_B_S" },
  { "Tabloid",				"-pt Tabloid" },
  { "BrLedger_S",				"-pt BrLedger_S" },
  { "BrLedger_B_Ledger",				"-pt BrLedger_B_Ledger" },
  { "BrLedger_B_S",				"-pt BrLedger_B_S" },
#endif    /*_INK4_*/
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
       { "Postcard",			"-pt Postcard" },
       { "BrHagaki_B",		"-pt BrHagaki_B" },
       { "DoublePostcardRotated",	"-pt DoublePostcardRotated" },
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
		{  "Color"	,	"-corm Color"	},
		{  "Mono"	,	"-corm Mono"	},
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

#ifdef _INK4_     /*INK4 2008.4.17*/
	  {  "Plain"			,	"-md Plain"      },
		{  "Inkjet"			,	"-md Inkjet"      },
	  {  "BrotherGlossyR"	,	"-md BrotherGlossyR"      },
	  {  "BrotherGlossy"	,	"-md BrotherGlossy"      },
	  {  "Glossy"			,	"-md Glossy"      },
	  {  "Transparencies"	,	"-md Transparencies"      },
#else       /*_INK4_*/
  {  "Plain"			,	"-md Plain"      },
	{  "Inkjet"			,	"-md Inkjet"      },
	{  "BrotherGlossy"	,	"-md BrotherGlossy"      },
	{  "Glossy"		,	"-md Glossy"      },
	{  "Transparencies"	,	"-md Transparencies"      },
#endif     /*_INK4_*/
	
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

//    ******DefaultBrMirror**********
PPDCOMMAND  DefaultBrMirrorPrint={
	"DefaultBrMirror",
	{
#ifndef _INK4_	/*INK4. Modified by Chenyipan. 20080519*/
		{  "OFF"	,	"-mirro OFF"	},
		{  "ON"	,	"-mirro ON"	},
#else
		{  "OFF"	,	"-mirro False"	},
		{  "ON"	,	"-mirro True"	},
#endif
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
#ifndef _INK4_	/*INK4. Modified by Chenyipan. 20080519*/
		{  "AUTO"	,	"-feed AUTO"	},
#else
		{  "AutoSelect"	,	"-feed AutoSelect"	},
#endif
		{  "Tray1"	,	"-feed Tray1"	},
		{  "Tray2"	,	"-feed Tray2"	},
		{ NULL , NULL  }
	}
};

#ifdef _INK4_    /*INK4 2008.4.17*/
//    ******DefaultBRReversePrint**********
PPDCOMMAND  DefaultBRReverse={
	"DefaultBRReverse",
	{
		{  "OFF"	,	"-rev OFF"	},
		{  "ON"	,	"-rev ON"	},

		{ NULL , NULL  }
	}
};
//    ******DefaultBRJpegPrint**********
PPDCOMMAND  DefaultBRJpeg={
	"DefaultBRJpeg",
	{
		{  "Recommended"	,	"-jpeg Recommended"	},
		{  "QualityPrior"	,	"-jpeg QualityPrior"	},
		{  "SpeedPrior"	,	"-jpeg SpeedPrior"	},
		{ NULL , NULL  }
    
	}
};
#endif     /*INK4 2008.4.17*/

//    ******DefaultBRDuplex**********
PPDCOMMAND DefaultBRDuplex={
	"DefaultBRDuplex",
	{
		{ "None"			,	"-duplex None" },
		{ "DuplexTumble"	,	"-duplex DuplexTumble" },
		{ "DuplexNoTumble"	,	"-duplex DuplexNoTumble" },
		{ NULL	,	NULL }
	}
};
//    ******DefaultBRDuplexMode**********
PPDCOMMAND DefaultBRDuplexMode={
	"DefaultBRDuplexMode",
	{
		{ "Normal"	,	"-advduplexmode Normal" },
		{ "Duplex1",	"-advduplexmode Duplex1" },
		{ "Duplex2"	,	"-advduplexmode Duplex2" },
		{ NULL	,	NULL }
	}
};

//    ******DefaultBRManualDuplex**********
PPDCOMMAND DefaultBRManualDuplex={
	"DefaultBRManualDuplex",
	{
		{ "OFF"	,	"-manualduplex OFF" },
		{ "Even",	"-manualduplex Even" },
		{ "Odd"	,	"-manualduplex Odd" },
		{ NULL	,	NULL }
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
	&DefaultBrMirrorPrint,

	&DefaultBRDocument,
	&DefaultBRColorMatching,
	&DefaultBRHalfTonePattern,
	&DefaultBRColorEnhancement,
#ifdef _INK4_       /*INK4 2008.4.17*/
 	&DefaultBRJpeg,
 	&DefaultBRReverse,
 	&DefaultBRDuplex,
 	&DefaultBRManualDuplex,
	&DefaultBRDuplexMode,
#endif    /*_INK4_*/        /*INK4 2008.4.17*/
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
