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
// MediaType Start
	{  "BRMediaType=PlainDuplex"		, "-md PlainDuplex" },
	{  "BRMediaType=Plain"				, "-md Plain" },
	{  "BRMediaType=Inkjet"				, "-md Inkjet" },
	{  "BRMediaType=BrotherGlossyR"		, "-md BrotherGlossyR" },
	{  "BRMediaType=BrotherBP60Matte"	, "-md BrotherBP60Matte" },
	{  "BRMediaType=Glossy"				, "-md Glossy" },
	{  "BRMediaType=Transparencies"		, "-md Transparencies" },
	{  "BRMediaType=HagakiAtena"		, "-md HagakiAtena"},
	{  "BRMediaType=HagakiUra"			, "-md HagakiUra" },
    {  "BRMediaType=PrintableDisc"		, "-md PrintableDisc" },
                    
// Resolution Start
	{  "BRResolution=PlainFast"  , "-reso PlainFast" },
	{  "BRResolution=PlainNormal", "-reso PlainNormal" },
	{  "BRResolution=Fast"		 , "-reso Fast" },
	{  "BRResolution=Normal"	 , "-reso Normal" },
	{  "BRResolution=Photo"		 , "-reso Photo" },
	{  "BRResolution=Best"		 , "-reso Best" },
    {  "BRResolution=DiscSlow"	 , "-reso DiscSlow" },
    {  "BRResolution=DiscNormal" , "-reso DiscNormal" },

	// BiDirPrint Start
	{  "BRBiDir=OFF"	, "-bidir OFF"	},
	{  "BRBiDir=ON"		, "-bidir ON"	},
	{  "noBRBiDir"		, "-bidir OFF"	},
	{  "BRBiDir"		, "-bidir ON"	},

// BRMonoColor Start
	{  "BRMonoColor=Color"	, "-corm Color"	},
	{  "BRMonoColor=Mono"	, "-corm Mono"	},

// Paper Start
	{  "PageSize=A4"					,	"-pt A4"				},
	{  "PageSize=BrA4_B"				,	"-pt BrA4_B"			},
	{  "PageSize=Letter"				,	"-pt Letter"			},
	{  "PageSize=BrLetter_B"			,	"-pt BrLetter_B"		},
	{  "PageSize=Legal"					,	"-pt Legal"				},
	{  "PageSize=Executive"				,	"-pt Executive"			},
	{  "PageSize=B5"					,	"-pt B5"				},
	{  "PageSize=A5"					,	"-pt A5"				},
	{  "PageSize=A6"					,	"-pt A6"				},
	{  "PageSize=BrA6_B"				,	"-pt BrA6_B"			},
	{  "PageSize=BrPostC4x6_S"			,	"-pt BrPostC4x6_S"		},
	{  "PageSize=BrPostC4x6_B"			,	"-pt BrPostC4x6_B"		},
	{  "PageSize=BrIndexC5x8_S"			,	"-pt BrIndexC5x8_S"		},
	{  "PageSize=BrIndexC5x8_B"			,	"-pt BrIndexC5x8_B"		},
	{  "PageSize=BrPhotoL_S"			,	"-pt BrPhotoL_S"		},
	{  "PageSize=BrPhotoL_B"			,	"-pt BrPhotoL_B"		},
	{  "PageSize=BrPhoto2L_S"			,	"-pt BrPhoto2L_S"		},
	{  "PageSize=BrPhoto2L_B"			,	"-pt BrPhoto2L_B"		},
	{  "PageSize=Postcard"				,	"-pt Postcard"			},
	{  "PageSize=BrHagaki_B"			,	"-pt BrHagaki_B"		},
	{  "PageSize=DoublePostcardRotated"	,	"-pt DoublePostcardRotated"	},
	{  "PageSize=Br12cmDisc_B"			,	"-pt Br12cmDisc_B"			},
	{  "PageSize=EnvC5"					,	"-pt EnvC5"				},
	{  "PageSize=EnvDL"					,	"-pt EnvDL"				},
	{  "PageSize=Env10"					,	"-pt Env10"				},
	{  "PageSize=EnvMonarch"			,	"-pt EnvMonarch"		},
	{  "PageSize=EnvYou4"				,	"-pt EnvYou4"			},
	{  "PageSize=EnvChou3"				,	"-pt EnvChou3"			},
	{  "PageSize=EnvChou4"				,	"-pt EnvChou4"			},
	{  "PageSize=EnvC6"					,	"-pt EnvC6"				},

// Thick Start
	{ "BRColorPaperThick=Regular"	, "-thick Regular" },
	{ "BRColorPaperThick=Thick"		, "-thick Thick" },
	{ "BRColorPaperThick=Env"		, "-thick Env" },

// ColorMatch Start
	{ "BRColorMatching=Natural"	, "-cm Natural" },
	{ "BRColorMatching=Vivid"	, "-cm Vivid" },
	{ "BRColorMatching=None"	, "-cm None" },

// BRHalfTonePattern Start
	{ "BRHalfTonePattern=Diffusion"	, "-ht Diffusion" },
	{ "BRHalfTonePattern=Dither"	, "-ht Dither" },

// BRColorEnhancement Start
	{ "BRColorEnhancement=OFF"	, "-ce OFF" },
	{ "BRColorEnhancement=ON"	, "-ce ON" },

// BRSlowDrying Start
	{ "BRSlowDrying=OFF", "-sd OFF" },
	{ "BRSlowDrying=ON"	, "-sd ON" },
	
// BRJpegPrint Start
	{"BRJpeg=Recommended"	, "-jpeg Recommended" },
	{"BRJpeg=QualityPrior"	, "-jpeg QualityPrior" },
	{"BRJpeg=SpeedPrior"	, "-jpeg SpeedPrior" },

// BRReversePrint Start
	{"BRReverse=ON"	, "-rev ON"},
	{"BRReverse=OFF", "-rev OFF"},
	

// Duplex Start  
	{  "BRDuplex=None"			, "-duplex None"		},
	{  "BRDuplex=DuplexTumble"	, "-duplex DuplexTumble"  },
	{  "BRDuplex=DuplexNoTumble", "-duplex DuplexNoTumble"},

// ManualDuplex Start

// Advanced Duplex Settings Start
	{  "BRDuplexMode=Normal"	, "-duplexmode Normal"	},
	{  "BRDuplexMode=Duplex1"	, "-duplexmode Duplex1"	},
	{  "BRDuplexMode=Duplex2"	, "-duplexmode Duplex2"	},
	
	{ NULL, NULL },
};

CMDLINELIST commandlinelist1[] = {

	{  "media=A4"					,	"-pt A4"			},
	{  "media=BrA4_B"				,	"-pt BrA4_B"			},
	{  "media=Letter"				,	"-pt Letter"			},
	{  "media=BrLetter_B"			,	"-pt BrLetter_B"		},
	{  "media=Legal"				,	"-pt Legal"			},
	{  "media=Executive"			,	"-pt Executive"			},
	{  "media=B5"					,	"-pt B5"			},
	{  "media=A5"					,	"-pt A5"			},
	{  "media=A6"					,	"-pt A6"			},
	{  "media=BrA6_B"				,	"-pt BrA6_B"			},
	{  "media=BrPostC4x6_S"			,	"-pt BrPostC4x6_S"		},
	{  "media=BrPostC4x6_B"			,	"-pt BrPostC4x6_B"		},
	{  "media=BrIndexC5x8_S"		,	"-pt BrIndexC5x8_S"		},
	{  "media=BrIndexC5x8_B"		,	"-pt BrIndexC5x8_B"		},
	{  "media=BrPhotoL_S"			,	"-pt BrPhotoL_S"		},
	{  "media=BrPhotoL_B"			,	"-pt BrPhotoL_B"		},
	{  "media=BrPhoto2L_S"			,	"-pt BrPhoto2L_S"		},
	{  "media=BrPhoto2L_B"			,	"-pt BrPhoto2L_B"		},
	{  "media=Postcard"				,	"-pt Postcard"			},
	{  "media=BrHagaki_B"			,	"-pt BrHagaki_B"		},
	{  "media=DoublePostcardRotated",	"-pt DoublePostcardRotated"	},
	{  "media=Br12cmDisc_B"			,   "-pt Br12cmDisc_B"			},
	{  "media=EnvC5"				,	"-pt EnvC5"			},
	{  "media=EnvDL"				,	"-pt EnvDL"			},
	{  "media=Env10"				,	"-pt Env10"			},
	{  "media=EnvMonarch"			,	"-pt EnvMonarch"		},
	{  "media=EnvYou4"				,	"-pt EnvYou4"			},
	{  "media=EnvChou3"				,	"-pt EnvChou3"			},
	{  "media=EnvChou4"				,	"-pt EnvChou4"			},
	{  "media=EnvC6"				,	"-pt EnvC6"				},
	{ NULL   ,   NULL    },
} ;


// +++++++++++++++++++++++++++
// command line   (Standard command)
// +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	// for Fedora OpenOffice

	{ NULL , NULL },
};

// +++++++++++++++++++++++++++
// command line numerical value
// +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	{	"BRBrightness="	, "-brit "	},
	{	"BRContrast="	, "-cont "	},

	{   "BRRed="		, "-red "	},
	{   "BRGreen="		, "-green "	},
	{   "BRBlue="		, "-blue "	},
	{ NULL				, NULL 		},
};

/*      PPD Default   */
// +++++++++++++++++++++++++++
// PPD setting list
// +++++++++++++++++++++++++++
typedef struct {
    char *value;
    char *brcommand;
} PPDCOMMANDLIST;


typedef   struct   {
   char             *label;
   PPDCOMMANDLIST   ppdcommandlist[];
}  PPDCOMMAND;

// ***** DefaultResolution *****
PPDCOMMAND  DefaultBRResolution={
	"DefaultBRResolution",
	{
		{ "PlainFast"	, "-reso PlainFast"  },
		{ "PlainNormal"	, "-reso PlainNormal"  },
		{ "Fast"  		, "-reso Fast"  },
		{ "Normal"		, "-reso Normal"  },
		{ "Photo"		, "-reso Photo"  },
		{ "Best"		, "-reso Best"  },
        { "DiscSlow"	, "-reso DiscSlow"  },
        { "DiscNormal"	, "-reso DiscNormal"  },
		{ NULL			, NULL  }
	}
};


//       ***** DefaultPageSize *****

PPDCOMMAND  DefaultPageSize={
	"DefaultPageSize",
  	{
		{  "A4"						, "-pt A4"				},
		{  "BrA4_B"					, "-pt BrA4_B"			},
		{  "Letter"					, "-pt Letter"			},
		{  "BrLetter_B"				, "-pt BrLetter_B"		},
		{  "Legal"					, "-pt Legal"			},
		{  "Executive"				, "-pt Executive"		},
		{  "B5"						, "-pt B5"				},
		{  "A5"						, "-pt A5"				},
		{  "A6"						, "-pt A6"				},
		{  "BrA6_B"					, "-pt BrA6_B"			},
		{  "BrPostC4x6_S"			, "-pt BrPostC4x6_S"	},
		{  "BrPostC4x6_B"			, "-pt BrPostC4x6_B"	},
		{  "BrIndexC5x8_S"			, "-pt BrIndexC5x8_S"	},
		{  "BrIndexC5x8_B"			, "-pt BrIndexC5x8_B"	},
		{  "BrPhotoL_S"				, "-pt BrPhotoL_S"		},
		{  "BrPhotoL_B"				, "-pt BrPhotoL_B"		},
		{  "BrPhoto2L_S"			, "-pt BrPhoto2L_S"		},
		{  "BrPhoto2L_B"			, "-pt BrPhoto2L_B"		},
		{  "Postcard"				, "-pt Postcard"		},
		{  "BrHagaki_B"				, "-pt BrHagaki_B"		},
		{  "DoublePostcardRotated"	, "-pt DoublePostcardRotated" },
		{  "Br12cmDisc_B"			, "-pt Br12cmDisc_B"	},
		{  "EnvC5"					, "-pt EnvC5"			},
		{  "EnvDL"					, "-pt EnvDL"			},
		{  "Env10"					, "-pt Env10"			},
		{  "EnvMonarch"				, "-pt EnvMonarch"		},
		{  "EnvYou4"				, "-pt EnvYou4"			},
		{  "EnvChou3"				, "-pt EnvChou3"		},
		{  "EnvChou4"				, "-pt EnvChou4"		},
		{  "EnvC6"					, "-pt EnvC6"		},
		{ NULL 						, NULL }
     }
};


// ***** DefaultBRMonoColor*****
PPDCOMMAND  DefaultBRMonoColor={
    "DefaultBRMonoColor",
    {
		{  "Color"	, "-corm Color"	},
		{  "Mono"	, "-corm Mono"	},
 	    	{ NULL	, NULL  }
     }
};


// ***** DefaultBRSlowDrying*****

PPDCOMMAND  DefaultBRSlowDrying={
    "DefaultBRSlowDrying",
    {
 	    { "OFF"	, "-sd OFF"  },
 	    { "ON" 	, "-sd ON"  },
 	    { NULL	, NULL  }
     }
};

//       ***** DefaultBRColorMediaType*****
PPDCOMMAND  DefaultBRColorMediaType={
    "DefaultBRMediaType",
		{  
		{  "PlainDuplex"		,	"-md PlainDuplex"	},
		{  "Plain"				,	"-md Plain"		},
		{  "Inkjet"				,	"-md Inkjet"		},
		{  "BrotherGlossyR"		,	"-md BrotherGlossyR"	},
		{  "BrotherBP60Matte"	,	"-md BrotherBP60Matte"	},
		{  "Glossy"				,	"-md Glossy"		},
		{  "Transparencies"		,	"-md Transparencies"	},
		{  "HagakiAtena"		,	"-md HagakiAtena"	},
		{  "HagakiUra"			,	"-md HagakiUra"		},
        {  "PrintableDisc"		,	"-md PrintableDisc"	},
		{ NULL					,	NULL  }
	}
};


// ***** DefaultBRColorPaperThick *****
PPDCOMMAND  DefaultBRColorPaperThick={
    "DefaultBRColorPaperThick",
	{
		{ "Regular", "-thick Regular" },
		{ "Thick", "-thick Thick" },
		{ "Env", "-thick Env" },

 		{ NULL , NULL  }
	}
};

// ******DefaultBRBiDir**********
PPDCOMMAND  DefaultBRBiDir={
	"DefaultBRBiDir",
	{
		{  "OFF"	,	"-bidir OFF"	},
		{  "ON"		,	"-bidir ON"	},
		{ NULL 		, NULL  }
	}
};

// ******DefaultBrMirror**********

PPDCOMMAND  DefaultBRMirrorPrint={
	"DefaultBRMirror",
	{
		{  "OFF"	,	"-mirro OFF"	},
		{  "ON"		,	"-mirro ON"	},    
		{ NULL 		, NULL  }
	}
};


// ******DefaultBRColorMatching**********
PPDCOMMAND  DefaultBRColorMatching={
	"DefaultBRColorMatching",
	{
		{ "Natural", "-cm Natural" },
		{ "Vivid", "-cm Vivid" },
		{ "None", "-cm None" },
		{ NULL , NULL  }
	}
};

// ******DefaultBRHalfTonePattern**********
PPDCOMMAND  DefaultBRHalfTonePattern={
	"DefaultBRHalfTonePattern",
	{
		{ "Diffusion", "-ht Diffusion" },
		{ "Dither", "-ht Dither" },
		{ NULL , NULL  }
	}
};

// ******DefaultBRColorEnhancement**********
PPDCOMMAND  DefaultBRColorEnhancement={
	"DefaultBRColorEnhancement",
	{
		{  "OFF"	,	"-ce OFF"	},
		{  "ON"	,	"-ce ON"	},
		{ NULL , NULL  }
	}
};

// ******DefaultBRReversePrint**********

// ******DefaultBRJpegPrint**********
PPDCOMMAND  DefaultBRJpeg={
	"DefaultBRJpeg",
	{
		{  "Recommended"	,	"-jpeg Recommended"	},
		{  "QualityPrior"	,	"-jpeg QualityPrior"	},
		{  "SpeedPrior"	,	"-jpeg SpeedPrior"	},
		{ NULL , NULL  }    
	}
};


// ******DefaultBRDuplex**********
PPDCOMMAND DefaultBRDuplex={
	"DefaultBRDuplex",
	{
		{ "None"	,	"-duplex None" },
		{ "DuplexTumble"	,	"-duplex DuplexTumble" },
		{ "DuplexNoTumble"	,	"-duplex DuplexNoTumble" },
		{ NULL	,	NULL }
	}
};

// ******DefaultBRManualDuplex**********
PPDCOMMAND DefaultDuplexMode={
	"DefaultBRDuplexMode",
	{
		{ "Normal"	,"-duplexmode Normal" },
		{ "Duplex1"	,"-duplexmode Duplex1" },
		{ "Duplex2"	,"-duplexmode Duplex2" },
		{ NULL	,	NULL }
	}
};

// ******DefaultBRManualDuplex**********

PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultBRResolution,
	&DefaultPageSize,
	&DefaultBRMonoColor,
	&DefaultBRSlowDrying,
	&DefaultBRColorMediaType,
	&DefaultBRColorPaperThick,
	&DefaultBRBiDir,
	&DefaultBRColorMatching,
	&DefaultBRHalfTonePattern,
	&DefaultBRColorEnhancement,
	&DefaultBRJpeg,
	&DefaultBRDuplex,
	&DefaultDuplexMode,
	NULL
};
/* PPD Default end */

// +++++++++++++++++++++++++++
// PPD numerical value
// +++++++++++++++++++++++++++

CMDLINELIST PPDdefaultN[] = {
	{"DefaultBRBrightness", "-brit "},
	{"DefaultBRContrast", "-cont "},
	{"DefaultBRRed", "-red "},
	{"DefaultBRGreen", "-green "},
	{"DefaultBRBlue", "-blue "},
	{ NULL   ,   NULL    },
};


//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_media_commandlinelist[] = {
	   { NULL   ,   NULL    }
};

#endif  //_BRCUPS_COMMANDS_H_
