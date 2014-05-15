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


#ifndef _MODEL_9_    /*Add ,2008.4.17*/
#define _MODEL_9_
#endif   /*_MODEL_9_*/

#ifndef _MODEL_M11_LINUX_DRIVER_		/* 2010-08-12*/
#define _MODEL_M11_LINUX_DRIVER_
#endif		// end _MODEL_M11_LINUX_DRIVER_

#ifndef _MODEL_mini11_LINUX_DRIVER_	// Added 2011-03-09
#define _MODEL_mini11_LINUX_DRIVER_
#endif // end _MODEL_mini11_LINUX_DRIVER_

#ifndef _MODEL_11_LINUX_DRIVER_ // Added , 2011-12-18.
#define _MODEL_11_LINUX_DRIVER_
#endif //end _MODEL_11_LINUX_DRIVER

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
#ifndef _MODEL_M11_LINUX_DRIVER_ // Removed , 2010-08-12
	// Collate
	{  "Collate=OFF"			,	"-collate OFF"      },
	{  "Collate=ON"				,	"-collate ON"      },
#endif  // end _MODEL_M11_LINUX_DRIVER_

#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified , 2010-08-12
	{  "BRMediaType=PlainDuplex"		,"-md PlainDuplex"      },
	{  "BRMediaType=Plain"			,"-md Plain"      },
	{  "BRMediaType=Inkjet"			,"-md Inkjet"      },
	{  "BRMediaType=BrotherGlossyR"		,"-md BrotherGlossyR"      },
	{  "BRMediaType=BrotherGlossy"		,"-md BrotherGlossy"      },
	{  "BRMediaType=Glossy"			,"-md Glossy"      },
	{  "BRMediaType=Transparencies"		,"-md Transparencies"      },
	{  "BRMediaType=HagakiAtena"		,"-md HagakiAtena"      },
	{  "BRMediaType=HagakiUra"		,"-md HagakiUra"      },
#ifdef _MODEL_mini11_LINUX_DRIVER_       // Added , 2011-03-09    
	{  "BRMediaType=BrotherBP60Matte"	,"-md BrotherBP60Matte"      },
	{  "BRMediaType=PrintableDisc"		,"-md PrintableDisc"      },
	{  "BRMediaType=GlossyPrintableDisc"	,"-md GlossyPrintableDisc"      },
#endif // end _MODEL_mini11_LINUX_DRIVER_
#else    // _MODEL_M11_LINUX_DRIVER_
  // Media  
#ifdef _MODEL_9_    /*Add ,2008.4.17*/
	{  "BRColorMediaType=Plain"			,"-md Plain"      },
	{  "BRColorMediaType=Inkjet"			,"-md Inkjet"      },
	{  "BRColorMediaType=BrotherGlossyR"		,"-md BrotherGlossyR"      },
	{  "BRColorMediaType=BrotherGlossy"		,"-md BrotherGlossy"      },
	{  "BRColorMediaType=Glossy"			,"-md Glossy"      },
	{  "BRColorMediaType=Transparencies"		,"-md Transparencies"      },
#else      /*_MODEL_9_*/
	{  "BRColorMediaType=Plain"			,"-md Plain"      },
	{  "BRColorMediaType=Inkjet"			,"-md Inkjet"      },
	{  "BRColorMediaType=BrotherGlossy"		,"-md BrotherGlossy"      },
	{  "BRColorMediaType=Glossy"			,"-md Glossy"      },
	{  "BRColorMediaType=Transparencies"		,"-md Transparencies"      },

#endif     /*_MODEL_9_*/
#endif   // end  _MODEL_M11_LINUX_DRIVER_
#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified , 2010-08-12
	{  "BRResolution=PlainFast"	,	"-reso PlainFast"      },
	{  "BRResolution=PlainNormal"	,	"-reso PlainNormal"      },
	{  "BRResolution=Fast"		,	"-reso Fast"      },
	{  "BRResolution=Normal"	,	"-reso Normal"      },
	{  "BRResolution=Photo"		,	"-reso Photo"      },
	{  "BRResolution=High"		,	"-reso High"      },
	{  "BRResolution=Best"		,	"-reso Best"      },
#ifdef _MODEL_mini11_LINUX_DRIVER_       // Added 2011-03-09
	{  "BRResolution=DiscNormal"	,	"-reso DiscNormal"      },
	{  "BRResolution=DiscSlow"	,	"-reso DiscSlow"      },
#endif // end _MODEL_mini11_LINUX_DRIVER_
#else    // _MODEL_M11_LINUX_DRIVER_		
 // RResolution
	{  "BRResolution=150x150dpi"	,	"-reso 150x150dpi"      },
	{  "BRResolution=300x300dpi"	,	"-reso 300x300dpi"      },
	{  "BRResolution=600x600dpi"	,	"-reso 600x600dpi"      },
	{  "BRResolution=1200x1200dpi"	,	"-reso 1200x1200dpi"      },
	{  "BRResolution=1200x2400dpi"	,	"-reso 1200x2400dpi"      },
	{  "BRResolution=1200x6000dpi"	,	"-reso 1200x6000dpi"      },
#endif   // end  _MODEL_M11_LINUX_DRIVER_


	// BiDirPrint
	{  "BRBiDir=OFF"	,"-bidir OFF"	},
	{  "BRBiDir=ON"		,"-bidir ON"	},
	{  "noBRBiDir"		,"-bidir OFF"	},
	{  "BRBiDir"		,"-bidir ON"	},

#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified , 2010-08-12
	{  "BRMirror=OFF"	,	"-mirro OFF"	},
	{  "BRMirror=ON"	,	"-mirro ON"	},
	{  "noBRMirror"	,	"-mirro OFF"	},	// For Debian
	{  "BRMirror"	,	"-mirro ON"	},		// For Debian
#else    // _MODEL_M11_LINUX_DRIVER_	
	// Mirror
#ifdef _MODEL_9_    /* 20080519*/
	{  "MirrorPrint=OFF"	,	"-mirro False"	},
	{  "MirrorPrint=ON"	,	"-mirro True"	},
	{  "noBRMirror"	,	"-mirro False"	},	// For Debian
	{  "BRMirror"	,	"-mirro True"	},		// For Debian
#else        /*MODEL_9. Modified by Chenyipan. 20080519*/
	{  "MirrorPrint=OFF"	,	"-mirro OFF"	},
	{  "MirrorPrint=ON"	,	"-mirro ON"	},
	{  "NoMirrorPrint"	,	"-mirro OFF"	},
	{  "MirrorPrint"	,	"-mirro ON"	},
#endif      /*MODEL_9. Modified by Chenyipan. 20080519*/

#endif   // end  _MODEL_M11_LINUX_DRIVER_


#ifdef _MODEL_9_    /*MODEL_9. Modified by Chenyipan. 20080701*/
	{  "BRInputSlot=AutoSelect",	"-feed AutoSelect"	},
	{  "BRInputSlot=Tray1"	,	"-feed Tray1"	},
	{  "BRInputSlot=Tray2"	,	"-feed Tray2"	},
#endif
	// BRMonoColor
	{  "BRMonoColor=Color"	,	"-corm Color"	},
	{  "BRMonoColor=Mono"	,	"-corm Mono"	},


// Paper
#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified 2010-08-12
#ifndef _MODEL_mini11_LINUX_DRIVER_ // Removed , 2011-03-09
	{  "PageSize=A3"				,	"-pt A3"			},
	{  "PageSize=BrA3_S_Duplex"			,	"-pt BrA3_S_Duplex"		},
	{  "PageSize=BrA3_L"				,	"-pt BrA3_L"			},
	{  "PageSize=BrA3_Duplex"			,	"-pt BrA3_Duplex"		},
	{  "PageSize=BrA3_B_S"				,	"-pt BrA3_B_S"			},
	{  "PageSize=BrA3_B_B"				,	"-pt BrA3_B_B"			},
	{  "PageSize=B4_S"				,	"-pt B4_S"			},
	{  "PageSize=B4_Duplex"				,	"-pt B4_Duplex"			},
	{  "PageSize=BrB4_S_S"				,	"-pt BrB4_S_S"			},
	{  "PageSize=BrB4_S_Duplex"			,	"-pt BrB4_S_Duplex"		},
	{  "PageSize=BrB4_B_B"				,	"-pt BrB4_B_B"			},
	{  "PageSize=BrB4_B_S"				,	"-pt BrB4_B_S"			},
	{  "PageSize=Tabloid_S"				,	"-pt Tabloid_S"			},
	{  "PageSize=Tabloid_Duplex"			,	"-pt Tabloid_Duplex"		},
	{  "PageSize=BrLedger_S_S"			,	"-pt BrLedger_S_S"		},
	{  "PageSize=BrLedger_S_Duplex"			,	"-pt BrLedger_S_Duplex"		},
	{  "PageSize=BrLedger_B_B"			,	"-pt BrLedger_B_B"		},
	{  "PageSize=BrLedger_B_S"			,	"-pt BrLedger_B_S"		},
#endif // end _MODEL_mini11_LINUX_DRIVER_
	{  "PageSize=A4"				,	"-pt A4"			},
	{  "PageSize=BrA4_B"				,	"-pt BrA4_B"			},
	{  "PageSize=Letter"				,	"-pt Letter"			},
	{  "PageSize=BrLetter_B"			,	"-pt BrLetter_B"		},
	{  "PageSize=Legal"				,	"-pt Legal"			},
	{  "PageSize=Executive"				,	"-pt Executive"			},
	{  "PageSize=B5"				,	"-pt B5"			},
	{  "PageSize=A5"				,	"-pt A5"			},
	{  "PageSize=A6"				,	"-pt A6"			},
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
	{  "PageSize=DoublePostcardRotated"		,	"-pt DoublePostcardRotated"	},
	{  "PageSize=EnvC5"				,	"-pt EnvC5"			},
	{  "PageSize=EnvDL"				,	"-pt EnvDL"			},
	{  "PageSize=Env10"				,	"-pt Env10"			},
	{  "PageSize=EnvMonarch"			,	"-pt EnvMonarch"		},
	{  "PageSize=EnvYou4"				,	"-pt EnvYou4"			},
#ifdef _MODEL_mini11_LINUX_DRIVER_ // Added , 2011-03-09
	{  "PageSize=Br12cmDisc_B"			,	"-pt Br12cmDisc_B"		},
#endif

//	{  "media=A3"					,	"-pt A3"			},
//	{  "media=BrA3_S_Duplex"			,	"-pt BrA3_S_Duplex"		},
//	{  "media=BrA3_L"				,	"-pt BrA3_L"			},
//	{  "media=BrA3_Duplex"				,	"-pt BrA3_Duplex"		},
//	{  "media=BrA3_B_S"				,	"-pt BrA3_B_S"			},
//	{  "media=BrA3_B_B"				,	"-pt BrA3_B_B"			},
//	{  "media=B4_S"					,	"-pt B4_S"			},
//	{  "media=B4_Duplex"				,	"-pt B4_Duplex"			},
//	{  "media=BrB4_S_S"				,	"-pt BrB4_S_S"			},
//	{  "media=BrB4_S_Duplex"			,	"-pt BrB4_S_Duplex"		},
//	{  "media=BrB4_B_B"				,	"-pt BrB4_B_B"			},
//	{  "media=BrB4_B_S"				,	"-pt BrB4_B_S"			},
//	{  "media=Tabloid_S"				,	"-pt Tabloid_S"			},
//	{  "media=Tabloid_Duplex"			,	"-pt Tabloid_Duplex"		},
//	{  "media=BrLedger_S_S"				,	"-pt BrLedger_S_S"		},
//	{  "media=BrLedger_S_Duplex"			,	"-pt BrLedger_S_Duplex"		},
//	{  "media=BrLedger_B_B"				,	"-pt BrLedger_B_B"		},
//	{  "media=BrLedger_B_S"				,	"-pt BrLedger_B_S"		},
//	{  "media=A4"					,	"-pt A4"			},
//	{  "media=BrA4_B"				,	"-pt BrA4_B"			},
//	{  "media=Letter"				,	"-pt Letter"			},
//	{  "media=BrLetter_B"				,	"-pt BrLetter_B"		},
//	{  "media=Legal"				,	"-pt Legal"			},
//	{  "media=Executive"				,	"-pt Executive"			},
//	{  "media=B5"					,	"-pt B5"			},
//	{  "media=A5"					,	"-pt A5"			},
//	{  "media=A6"					,	"-pt A6"			},
//	{  "media=BrA6_B"				,	"-pt BrA6_B"			},
//	{  "media=BrPostC4x6_S"				,	"-pt BrPostC4x6_S"		},
//	{  "media=BrPostC4x6_B"				,	"-pt BrPostC4x6_B"		},
//	{  "media=BrIndexC5x8_S"			,	"-pt BrIndexC5x8_S"		},
//	{  "media=BrIndexC5x8_B"			,	"-pt BrIndexC5x8_B"		},
//	{  "media=BrPhotoL_S"				,	"-pt BrPhotoL_S"		},
//	{  "media=BrPhotoL_B"				,	"-pt BrPhotoL_B"		},
//	{  "media=BrPhoto2L_S"				,	"-pt BrPhoto2L_S"		},
//	{  "media=BrPhoto2L_B"				,	"-pt BrPhoto2L_B"		},
//	{  "media=Postcard"				,	"-pt Postcard"			},
//	{  "media=BrHagaki_B"				,	"-pt BrHagaki_B"		},
//	{  "media=DoublePostcardRotated"		,	"-pt DoublePostcardRotated"	},
//	{  "media=EnvC5"				,	"-pt EnvC5"			},
//	{  "media=EnvDL"				,	"-pt EnvDL"			},
//	{  "media=Env10"				,	"-pt Env10"			},
//	{  "media=EnvMonarch"				,	"-pt EnvMonarch"		},
//	{  "media=EnvYou4"				,	"-pt EnvYou4"			},

#else    // _MODEL_M11_LINUX_DRIVER_

#ifdef _MODEL_9_        /*Add ,2008.4.17*/

  { "PageSize=A3",				"-pt BrA3_S" },
  { "PageSize=BrA3_A3",				"-pt BrA3_A3" },
  { "PageSize=BrA3_B_S",				"-pt BrA3_B_S" },
  { "PageSize=BrA3_B_A3",				"-pt BrA3_B_A3" }, 
  { "PageSize=B4",				"-pt B4" },
  { "PageSize=BrB4_S",				"-pt BrB4_S" },
  { "PageSize=BrB4_B_B4",				"-pt BrB4_B_B4" },
  { "PageSize=BrB4_B_S",				"-pt BrB4_B_S" },

  { "PageSize=Tabloid",				"-pt Tabloid" },
  { "PageSize=BrLedger_S",				"-pt BrLedger_S" },
  { "PageSize=BrLedger_B_Ledger",				"-pt BrLedger_B_Ledger" },
  { "PageSize=BrLedger_B_S",				"-pt BrLedger_B_S" },
#endif     /*_MODEL_9_*/
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

#if 1		// 

	// for Command Line
#ifdef _MODEL_9_    /*_MODEL_9_*/    /**/
  { "media=A3",				"-pt BrA3_S" },
  { "media=BrA3_A3",				"-pt BrA3_A3" },
  { "media=BrA3_B_S",				"-pt BrA3_B_S" },
  { "media=BrA3_B_A3",				"-pt BrA3_B_A3" }, /**/
  { "media=B4",				"-pt B4" },
  { "media=BrB4_S",				"-pt BrB4_S" },/**/
  { "media=BrB4_B_B4",				"-pt BrB4_B_B4" },
  { "media=BrB4_B_S",				"-pt BrB4_B_S" },
  { "media=Tabloid",				"-pt Tabloid" },
  { "media=BrLedger_S",				"-pt BrLedger_S" },
  { "media=BrLedger_B_Ledger",				"-pt BrLedger_B_Ledger" },
  { "media=BrLedger_B_S",				"-pt BrLedger_B_S" },
#endif    /*_MODEL_9_*/
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
#endif   // end  _MODEL_M11_LINUX_DRIVER_
// Thick
	{ "BRColorPaperThick=Regular", "-thick Regular" },
	{ "BRColorPaperThick=Thick", "-thick Thick" },
#ifdef _MODEL_M11_LINUX_DRIVER_ // Added , 2010-08-12
	{ "BRColorPaperThick=Env", "-thick Env" },
#else // _MODEL_M11_LINUX_DRIVER_

	// Thick
	{ "BRDocument=Photo", "-doc Photo" },
	{ "BRDocument=Graphics", "-doc Graphics" },
	{ "BRDocument=Custom", "-doc Custom" },
#endif// end _MODEL_M11_LINUX_DRIVER_

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
#ifdef _MODEL_9_   /*MODEL_9,Add ,2008.4.17*/
	//BRJpegPrint
	{"BRJpeg=Recommended", "-jpeg Recommended" },
	{"BRJpeg=QualityPrior", "-jpeg QualityPrior" },
	{"BRJpeg=SpeedPrior", "-jpeg SpeedPrior" },
	//BRReversePrint
	{"BRReverse=ON","-rev ON"},
	{"BRReverse=OFF","-rev OFF"},
#ifdef _MODEL_M11_LINUX_DRIVER_ // Added , 2010-08-12
    // Duplex      
	{  "BRDuplex=None"		,	"-duplex None"		},
	{  "BRDuplex=DuplexTumble"	,	"-duplex DuplexTumble"  },
	{  "BRDuplex=DuplexNoTumble"	,	"-duplex DuplexNoTumble"},
    // ManualDuplex
	{  "BRPageSet=All"		,	"-ps All"	},
	{  "BRPageSet=Even"		,	"-ps Even"	},
	{  "BRPageSet=Odd"		,	"-ps Odd"	},
    // Advanced Duplex Settings
	{  "BRDuplexMode=Normal"	,	"-duplexmode Normal"	},
	{  "BRDuplexMode=Duplex1"	,	"-duplexmode Duplex1"	},
	{  "BRDuplexMode=Duplex2"	,	"-duplexmode Duplex2"	},	
#endif // end  _MODEL_M11_LINUX_DRIVER_
    // Duplex
//    {  "Duplex=None"				,	"-duplex None"  },
//    {  "Duplex=DuplexTumble"		,	"-duplex DuplexTumble"  },
//    {  "Duplex=DuplexNoTumble"	,	"-duplex DuplexNoTumble"  },
    // ManualDuplex
//    {  "ManualDuplex=off"				,	"-mandup off"  },
//    {  "ManualDuplex=even"		,	"-mandup even"  },
//    {  "ManualDuplex=odd"	,	"-mandup odd"  },


#endif /*_MODEL_9_*/     /*MODEL_9,Add ,2008.4.17*/

	{ NULL   ,   NULL    },
};

#ifdef _MODEL_M11_LINUX_DRIVER_ // Added , 2010-12-15
CMDLINELIST commandlinelist1[] = {
#ifndef _MODEL_mini11_LINUX_DRIVER_ // Removed , 2011-03-09
	{  "media=A3"					,	"-pt A3"			},
	{  "media=BrA3_S_Duplex"			,	"-pt BrA3_S_Duplex"		},
	{  "media=BrA3_L"				,	"-pt BrA3_L"			},
	{  "media=BrA3_Duplex"				,	"-pt BrA3_Duplex"		},
	{  "media=BrA3_B_S"				,	"-pt BrA3_B_S"			},
	{  "media=BrA3_B_B"				,	"-pt BrA3_B_B"			},
	{  "media=B4_S"					,	"-pt B4_S"			},
	{  "media=B4_Duplex"				,	"-pt B4_Duplex"			},
	{  "media=BrB4_S_S"				,	"-pt BrB4_S_S"			},
	{  "media=BrB4_S_Duplex"			,	"-pt BrB4_S_Duplex"		},
	{  "media=BrB4_B_B"				,	"-pt BrB4_B_B"			},
	{  "media=BrB4_B_S"				,	"-pt BrB4_B_S"			},
	{  "media=Tabloid_S"				,	"-pt Tabloid_S"			},
	{  "media=Tabloid_Duplex"			,	"-pt Tabloid_Duplex"		},
	{  "media=BrLedger_S_S"				,	"-pt BrLedger_S_S"		},
	{  "media=BrLedger_S_Duplex"			,	"-pt BrLedger_S_Duplex"		},
	{  "media=BrLedger_B_B"				,	"-pt BrLedger_B_B"		},
	{  "media=BrLedger_B_S"				,	"-pt BrLedger_B_S"		},
	{  "media=A4"					,	"-pt A4"			},
	{  "media=BrA4_B"				,	"-pt BrA4_B"			},
	{  "media=Letter"				,	"-pt Letter"			},
#endif // end _MODEL_mini11_LINUX_DRIVER_ 
	{  "media=BrLetter_B"				,	"-pt BrLetter_B"		},
	{  "media=Legal"				,	"-pt Legal"			},
	{  "media=Executive"				,	"-pt Executive"			},
	{  "media=B5"					,	"-pt B5"			},
	{  "media=A5"					,	"-pt A5"			},
	{  "media=A6"					,	"-pt A6"			},
	{  "media=BrA6_B"				,	"-pt BrA6_B"			},
	{  "media=BrPostC4x6_S"				,	"-pt BrPostC4x6_S"		},
	{  "media=BrPostC4x6_B"				,	"-pt BrPostC4x6_B"		},
	{  "media=BrIndexC5x8_S"			,	"-pt BrIndexC5x8_S"		},
	{  "media=BrIndexC5x8_B"			,	"-pt BrIndexC5x8_B"		},
	{  "media=BrPhotoL_S"				,	"-pt BrPhotoL_S"		},
	{  "media=BrPhotoL_B"				,	"-pt BrPhotoL_B"		},
	{  "media=BrPhoto2L_S"				,	"-pt BrPhoto2L_S"		},
	{  "media=BrPhoto2L_B"				,	"-pt BrPhoto2L_B"		},
	{  "media=Postcard"				,	"-pt Postcard"			},
	{  "media=BrHagaki_B"				,	"-pt BrHagaki_B"		},
	{  "media=DoublePostcardRotated"		,	"-pt DoublePostcardRotated"	},
	{  "media=EnvC5"				,	"-pt EnvC5"			},
	{  "media=EnvDL"				,	"-pt EnvDL"			},
	{  "media=Env10"				,	"-pt Env10"			},
	{  "media=EnvMonarch"				,	"-pt EnvMonarch"		},
	{  "media=EnvYou4"				,	"-pt EnvYou4"			},
#ifdef _MODEL_mini11_LINUX_DRIVER_ // Added , 2011-03-09
	{  "media=Br12cmDisc_B"				,	"-pt Br12cmDisc_B"		},
#endif //end _MODEL_mini11_LINUX_DRIVER_
	{ NULL   ,   NULL    },
} ;
#endif // end _MODEL_M11_LINUX_DRIVER_
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
#ifdef _MODEL_M11_LINUX_DRIVER_ // Removed , 2010-08-12
	{	"Copies="   	,   "-copy "      },
#endif // end  _MODEL_M11_LINUX_DRIVER_
	{	"BRBrightness="	,	"-brit "	},
	{	"BRContrast="		,	"-cont "		},
#ifdef _MODEL_M11_LINUX_DRIVER_ // Removed , 2010-08-12
	{	"Margin="		,	"-margin "		},
#endif // end  _MODEL_M11_LINUX_DRIVER_
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
#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified, 2010-08-12
PPDCOMMAND  DefaultBRResolution={
  "DefaultBRResolution",
	{

		{ "PlainFast"  , "-reso PlainFast"  },
		{ "PlainNormal"  , "-reso PlainNormal"  },
#ifdef _MODEL_mini11_LINUX_DRIVER_ // Added , 2011-03-09
		{ "DiscNormal" , "-reso DiscNormal" },
		{ "DiscSlow" , "-reso DiscSlow" },
#endif // end _MODEL_mini11_LINUX_DRIVER_
		{ "Fast"  , "-reso Fast"  },
		{ "Normal"  , "-reso Normal"  },
		{ "Photo"  , "-reso Photo"  },
		{ "High"  , "-reso High"  },
		{ "Best"  , "-reso Best"  },
		{ NULL , NULL  }
	}
};
#else        // _MODEL_M11_LINUX_DRIVER_
PPDCOMMAND  DefaultBRResolution={
  "DefaultBRResolution",
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
#endif  //  end _MODEL_M11_LINUX_DRIVER_

//       ***** DefaultPageSize *****
#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified , 2010-08-12
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
  {
#ifndef _MODEL_mini11_LINUX_DRIVER_ // Removed ,
       	{  "A3"					,	"-pt A3"			},
	{  "BrA3_S_Duplex"			,	"-pt BrA3_S_Duplex"		},
	{  "BrA3_L"				,	"-pt BrA3_L"			},
	{  "BrA3_Duplex"			,	"-pt BrA3_Duplex"		},
	{  "BrA3_B_S"				,	"-pt BrA3_B_S"			},
	{  "BrA3_B_B"				,	"-pt BrA3_B_B"			},
	{  "B4_S"				,	"-pt B4_S"			},
	{  "B4_Duplex"				,	"-pt B4_Duplex"			},
	{  "BrB4_S_S"				,	"-pt BrB4_S_S"			},
	{  "BrB4_S_Duplex"			,	"-pt BrB4_S_Duplex"		},
	{  "BrB4_B_B"				,	"-pt BrB4_B_B"			},
	{  "BrB4_B_S"				,	"-pt BrB4_B_S"			},
	{  "Tabloid_S"				,	"-pt Tabloid_S"			},
	{  "Tabloid_Duplex"			,	"-pt Tabloid_Duplex"		},
	{  "BrLedger_S_S"			,	"-pt BrLedger_S_S"		},
	{  "BrLedger_S_Duplex"			,	"-pt BrLedger_S_Duplex"		},
	{  "BrLedger_B_B"			,	"-pt BrLedger_B_B"		},
	{  "BrLedger_B_S"			,	"-pt BrLedger_B_S"		},
#endif // end _MODEL_mini11_LINUX_DRIVER_
	{  "A4"					,	"-pt A4"			},
	{  "BrA4_B"				,	"-pt BrA4_B"			},
	{  "Letter"				,	"-pt Letter"			},
	{  "BrLetter_B"				,	"-pt BrLetter_B"		},
	{  "Legal"				,	"-pt Legal"			},
	{  "Executive"				,	"-pt Executive"			},
	{  "B5"					,	"-pt B5"			},
	{  "A5"					,	"-pt A5"			},
	{  "A6"					,	"-pt A6"			},
	{  "BrA6_B"				,	"-pt BrA6_B"			},
	{  "BrPostC4x6_S"			,	"-pt BrPostC4x6_S"		},
	{  "BrPostC4x6_B"			,	"-pt BrPostC4x6_B"		},
	{  "BrIndexC5x8_S"			,	"-pt BrIndexC5x8_S"		},
	{  "BrIndexC5x8_B"			,	"-pt BrIndexC5x8_B"		},
	{  "BrPhotoL_S"				,	"-pt BrPhotoL_S"		},
	{  "BrPhotoL_B"				,	"-pt BrPhotoL_B"		},
	{  "BrPhoto2L_S"			,	"-pt BrPhoto2L_S"		},
	{  "BrPhoto2L_B"			,	"-pt BrPhoto2L_B"		},
	{  "Postcard"				,	"-pt Postcard"			},
	{  "BrHagaki_B"				,	"-pt BrHagaki_B"		},
	{  "DoublePostcardRotated"		,	"-pt DoublePostcardRotated"	},
	{  "EnvC5"				,	"-pt EnvC5"			},
	{  "EnvDL"				,	"-pt EnvDL"			},
	{  "Env10"				,	"-pt Env10"			},
	{  "EnvMonarch"				,	"-pt EnvMonarch"		},
	{  "EnvYou4"				,	"-pt EnvYou4"			},
#ifdef _MODEL_mini11_LINUX_DRIVER_ // Added
	{  "Br12cmDisc_B"				,	"-pt Br12cmDisc_B"		},
#endif // end _MODEL_mini11_LINUX_DRIVER_

	{ NULL , NULL  }
     }
};
#else        // _MODEL_M11_LINUX_DRIVER_
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
  {
#ifdef _MODEL_9_   /*Add ,2008.4.17*/
  { "BrA3_S",				"-pt BrA3_S" },
  { "BrA3_A3",				"-pt BrA3_A3" },
  { "BrA3_B_S",				"-pt BrA3_B_S" },
  { "BrA3_B_A3",				"-pt BrA3_B_A3" }, /*add ,2008.4.17*/
  { "B4",				"-pt B4" },
  { "BrB4_S",				"-pt BrB4_S" },/*Add ,2008.4.17*/
  { "BrB4_B_B4",				"-pt BrB4_B_B4" },
  { "BrB4_B_S",				"-pt BrB4_B_S" },
  { "Tabloid",				"-pt Tabloid" },
  { "BrLedger_S",				"-pt BrLedger_S" },
  { "BrLedger_B_Ledger",				"-pt BrLedger_B_Ledger" },
  { "BrLedger_B_S",				"-pt BrLedger_B_S" },
#endif    /*_MODEL_9_*/
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
#endif  //  end _MODEL_M11_LINUX_DRIVER_

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
#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified 2010-08-12
PPDCOMMAND  DefaultBRColorMediaType={
    "DefaultBRMediaType",
    {  
	{  "PlainDuplex"	,	"-md PlainDuplex"	},
	{  "Plain"		,	"-md Plain"		},
	{  "Inkjet"		,	"-md Inkjet"		},
	{  "BrotherGlossyR"	,	"-md BrotherGlossyR"	},
	{  "BrotherGlossy"	,	"-md BrotherGlossy"	},
#ifdef _MODEL_mini11_LINUX_DRIVER_ // Added , 2011-03-09
	{  "BrotherBP60Matte"	,	"-md BrotherBP60Matte"	},
#endif // end _MODEL_mini11_LINUX_DRIVER_ 
	{  "Glossy"		,	"-md Glossy"		},
	{  "Transparencies"	,	"-md Transparencies"	},
#ifdef _MODEL_mini11_LINUX_DRIVER_ // Added , 2011-03-09
	{  "PrintableDisc"	,	"-md PrintableDisc"	},
	{  "GlossyPrintableDisc",	"-md GlossyPrintableDisc"},
#endif // end _MODEL_mini11_LINUX_DRIVER_ 
	{  "HagakiAtena"	,	"-md HagakiAtena"	},
	{  "HagakiUra"		,	"-md HagakiUra"		},
 	{ NULL , NULL  }
     }
};

#else  // _MODEL_M11_LINUX_DRIVER_
PPDCOMMAND  DefaultBRColorMediaType={
    "DefaultBRColorMediaType",
    {

#ifdef _MODEL_9_     /*Add ,2008.4.17*/
  {  "Plain"			,	"-md Plain"      },
	{  "Inkjet"			,	"-md Inkjet"      },
  {  "BrotherGlossyR"	,	"-md BrotherGlossyR"      },
  {  "BrotherGlossy"	,	"-md BrotherGlossy"      },
  {  "Glossy"			,	"-md Glossy"      },
  {  "Transparencies"	,	"-md Transparencies"      },
#else       /*_MODEL_9_*/
  {  "Plain"			,	"-md Plain"      },
	{  "Inkjet"			,	"-md Inkjet"      },


	{  "BrotherGlossy"	,	"-md BrotherGlossy"      },
	{  "Glossy"		,	"-md Glossy"      },
	{  "Transparencies"	,	"-md Transparencies"      },
#endif     /*_MODEL_9_*/
	
 	{ NULL , NULL  }
     }
};

#endif // end  _MODEL_M11_LINUX_DRIVER_

//       ***** DefaultBRColorPaperThick *****
PPDCOMMAND  DefaultBRColorPaperThick={
    "DefaultBRColorPaperThick",
    {
	{ "Regular", "-thick Regular" },
	{ "Thick", "-thick Thick" },
#ifdef _MODEL_M11_LINUX_DRIVER_ // Added , 2010-08-12
        { "Env", "-thick Env" },
#endif // end  _MODEL_M11_LINUX_DRIVER_
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
#ifdef _MODEL_M11_LINUX_DRIVER_ // Modified , 2010-08-12
PPDCOMMAND  DefaultBRMirrorPrint={
	"DefaultBRMirror",
	{
		{  "OFF"	,	"-mirro OFF"	},
		{  "ON"	,	"-mirro ON"	},    
		{ NULL , NULL  }
	}
};
#else      // _MODEL_M11_LINUX_DRIVER_
PPDCOMMAND  DefaultBRMirrorPrint={
	"DefaultBRMirror",
	{
#ifndef _MODEL_9_	/*MODEL_9. Modified by Chenyipan. 20080519*/
		{  "OFF"	,	"-mirro OFF"	},

		{  "ON"	,	"-mirro ON"	},
#else
		{  "OFF"	,	"-mirro False"	},
		{  "ON"	,	"-mirro True"	},
#endif
		{ NULL , NULL  }
	}
};
#endif   // end _MODEL_M11_LINUX_DRIVER_

#ifndef _MODEL_M11_LINUX_DRIVER_ // Removed , 2010-08-12
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
#endif   // end _MODEL_M11_LINUX_DRIVER_

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
#ifndef _MODEL_9_	/*MODEL_9. Modified by Chenyipan. 20080519*/
		{  "AUTO"	,	"-feed AUTO"	},
#else
		{  "AutoSelect"	,	"-feed AutoSelect"	},
#endif
		{  "Tray1"	,	"-feed Tray1"	},



		{  "Tray2"	,	"-feed Tray2"	},
		{ NULL , NULL  }
	}
};

#ifdef _MODEL_9_    /*MODEL_9,Add ,2008.4.17*/
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
#endif     /*Add ,2008.4.17*/

#ifndef	_MODEL_9_	/*Add,2008.4.17*/
//    ******DefaultBRDuplex**********
PPDCOMMAND DefaultDuplex={
	"DefaultDuplex",
	{
		{ "None"	,	"-Duplex None" },
		{ "DuplexTumble"	,	"-Duplex DuplexTumble" },
		{ "DuplexNoTumble"	,	"-Duplex DuplexNoTumble" },
		{ NULL	,	NULL }
	}
};
//    ******DefaultBRManualDuplex**********
PPDCOMMAND DefaultManualDuplex={
	"DefaultDuplex",
	{
		{ "off"	,	"-Duplex off" },
		{ "even"	,	"-Duplex even" },
		{ "odd"	,	"-Duplex odd" },
		{ NULL	,	NULL }
	}
};
#endif


#ifdef _MODEL_M11_LINUX_DRIVER_ // Added , 2010-08-12
//    ******DefaultBRDuplex**********
PPDCOMMAND DefaultBRDuplex={
	"DefaultBRDuplex",
	{
		{ "None"	,	"-duplex None" },
		{ "DuplexTumble"	,	"-duplex DuplexTumble" },
		{ "DuplexNoTumble"	,	"-duplex DuplexNoTumble" },
		{ NULL	,	NULL }
	}
};

//    ******DefaultBRManualDuplex**********
PPDCOMMAND DefaultDuplexMode={
	"DefaultBRDuplexMode",
	{
		{ "Normal"	,"-duplexmode Normal" },
		{ "Duplex1"	,"-duplexmode Duplex1" },
		{ "Duplex2"	,"-duplexmode Duplex2" },
		{ NULL	,	NULL }
	}
};

//    ******DefaultBRManualDuplex**********
PPDCOMMAND DefaultBRManualDuplex={
	"DefaultBRPageSet",
	{
		{ "All"	,	"-ps All" },
		{ "Even",	"-ps Even" },
		{ "Odd"	,	"-ps Odd" },
		{ NULL	,	NULL }
	}
};

#endif   // end _MODEL_M11_LINUX_DRIVER_

PPDCOMMAND   *ppdcommand_all_list[] = {
	&DefaultBRInputSlot,
	&DefaultBRResolution,
	&DefaultPageSize,
	&DefaultBRMonoColor,
	&DefaultBRSlowDrying,
	&DefaultBRColorMediaType,
	&DefaultBRColorPaperThick,
	&DefaultBRBiDir,
	&DefaultBRMirrorPrint,
#ifndef _MODEL_M11_LINUX_DRIVER_ // Removed , 2010-08-12
	&DefaultBRDocument,
#endif   // end _MODEL_M11_LINUX_DRIVER_
	&DefaultBRColorMatching,
	&DefaultBRHalfTonePattern,
	&DefaultBRColorEnhancement,
#ifdef _MODEL_9_       /*add ,2008.4.17*/
  &DefaultBRJpeg,
  &DefaultBRReverse,
#ifdef _MODEL_M11_LINUX_DRIVER_ // Add , 2010-08-12
  &DefaultBRDuplex,
  &DefaultDuplexMode,
  &DefaultBRManualDuplex,
#endif   // end _MODEL_M11_LINUX_DRIVER_
//  &DefaultBRDuplex,
//  &DefaultBRManualDuplex,
#endif    /*_MODEL_9_*/        /*Add ,2008.4.17*/
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
