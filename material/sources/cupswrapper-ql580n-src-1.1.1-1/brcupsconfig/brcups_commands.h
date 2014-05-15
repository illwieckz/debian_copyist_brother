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

char brprintconf[30]="brprintconfpt1";
//   +++++++++++++++++++++++++++
//       set default
//   +++++++++++++++++++++++++++
char *default_setting[] = {
    // Collate
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -collate OFF",

    // Auto cut
    //"brprintconfpt1 -P BROTHERPRINTER_XXX  -cut ON",

    // added cut at end
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -cutend ON",

    // added cutlabel
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -cutlabel 1",

    // added compress
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -compress OFF",

    // Bringhtness
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -brit 0",

    // Contrast
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -cont 0",

    // Halftone
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -half ERROR",

    // Mirror
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -mirro OFF",

    // Quality
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -quality SPEED",

    // Resolution
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -reso 300",

    // Margin(Feed)
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -feed 0",

    // Media
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -media 29x90",

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
    { "Resolution=300"  , "-reso 600"       },

    // Collate
    { "Collate=OFF"     , "-collate OFF"    },
    { "Collate=ON"      , "-collate ON"     },

    // AutoCut
    // { "AutoCut=OFF"  , "-cut OFF"        },
    // { "AutoCut=ON"   , "-cut ON"         },
    // { "noAutoCut"    , "-cut OFF"        }, // for debian
    // { "AutoCut"      , "-cut ON"         }, // for debian

    // CutAtEnd
    { "CutAtEnd=OFF"    , "-cutend OFF"     },
    { "CutAtEnd=ON"     , "-cutend ON"      },
    { "noCutAtEnd"      , "-cutend OFF"     },
    { "CutAtEnd"        , "-cutend ON"      },

    { "Compress=OFF"    , "-compress OFF"   },
    { "Compress=ON"     , "-compress ON"    },
    { "noCompress"      , "-compress OFF"   },
    { "Compress"        , "-compress ON"    },

    // Mirror
    { "MirrorPrint=OFF" , "-mirro OFF"      },
    { "MirrorPrint=ON"  , "-mirro ON"       },
    { "noMirrorPrint"   , "-mirro OFF"      }, // for debian
    { "MirrorPrint"     , "-mirro ON"       }, // for debian

    // HalftonePattern
    { "Halftone=ERROR"  , "-half ERROR"     },
    { "Halftone=BINARY" , "-half BINARY"    },
    { "Halftone=DITHER" , "-half DITHER"    },

    // Quality
    { "Priority=SPEED"  , "-quality SPEED"  },
    { "Priority=QUALITY", "-quality QUALITY"},

    { NULL  , NULL  },
};


//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
    // Paper Size
    { "media=17x54" , "-media 17x54" },
    { "media=17x87" , "-media 17x87" },
    { "media=23x23" , "-media 23x23" },
    { "media=29x90" , "-media 29x90" },
    { "media=38x90" , "-media 38x90" },
    { "media=39x48" , "-media 39x48" },
    { "media=52x29" , "-media 52x29" },
    { "media=62x29" , "-media 62x29" },
    { "media=62x100", "-media 62x100"},
    { "media=12Dia" , "-media 12Dia" },
    { "media=24Dia" , "-media 24Dia" },
    { "media=58Dia" , "-media 58Dia" },
    { "media=12X1"  , "-media 12X1"  },
    { "media=29X1"  , "-media 29X1"  },
    { "media=38X1"  , "-media 38X1"  },
    { "media=50X1"  , "-media 50X1"  },
    { "media=54X1"  , "-media 54X1"  },
    { "media=62X1"  , "-media 62X1"  },
    { "media=12X2"  , "-media 12X2"  },
    { "media=29X2"  , "-media 29X2"  },
    { "media=38X2"  , "-media 38X2"  },
    { "media=50X2"  , "-media 50X2"  },
    { "media=54X2"  , "-media 54X2"  },
    { "media=62X2"  , "-media 62X2"  },
    { "media=12X3"  , "-media 12X3"  },
    { "media=29X3"  , "-media 29X3"  },
    { "media=38X3"  , "-media 38X3"  },
    { "media=50X3"  , "-media 50X3"  },
    { "media=54X3"  , "-media 54X3"  },
    { "media=62X3"  , "-media 62X3"  },
    { "media=12X4"  , "-media 12X4"  },
    { "media=29X4"  , "-media 29X4"  },
    { "media=38X4"  , "-media 38X4"  },
    { "media=50X4"  , "-media 50X4"  },
    { "media=54X4"  , "-media 54X4"  },
    { "media=62X4"  , "-media 62X4"  },

    { NULL   ,   NULL    },
};

//   +++++++++++++++++++++++++++
//       command line numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
    { "Copies="     , "-copy "      },
    { "Brightness=" , "-brit "      },
    { "Contrast="   , "-cont "      },
    { "Feed="       , "-feed "      },
    { "CutLabel="   , "-cutlabel "  },
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
   PPDCOMMANDLIST   *ppdcommandlist;
}  PPDCOMMAND;

//       ***** DefaultResolution *****
PPDCOMMANDLIST   ppdCmdListRes[] =
{
    { "Normal"  , "-reso 300"  },
    { NULL , NULL  }
};

PPDCOMMAND  DefaultResolution={
    "DefaultResolution",
    ppdCmdListRes
};

//       ***** DefaultPageSize *****
PPDCOMMANDLIST   ppdCmdListPSize[] =
{
    { "17x54" , "-media 17x54" },
    { "17x87" , "-media 17x87" },
    { "23x23" , "-media 23x23" },
    { "29x90" , "-media 29x90" },
    { "38x90" , "-media 38x90" },
    { "39x48" , "-media 39x48" },
    { "52x29" , "-media 52x29" },
    { "62x29" , "-media 62x29" },
    { "62x100", "-media 62x100"},
    { "12Dia" , "-media 12Dia" },
    { "24Dia" , "-media 24Dia" },
    { "58Dia" , "-media 58Dia" },
    { "12X1"  , "-media 12X1"  },
    { "29X1"  , "-media 29X1"  },
    { "38X1"  , "-media 38X1"  },
    { "50X1"  , "-media 50X1"  },
    { "54X1"  , "-media 54X1"  },
    { "62X1"  , "-media 62X1"  },
    { "12X2"  , "-media 12X2"  },
    { "29X2"  , "-media 29X2"  },
    { "38X2"  , "-media 38X2"  },
    { "50X2"  , "-media 50X2"  },
    { "54X2"  , "-media 54X2"  },
    { "62X2"  , "-media 62X2"  },
    { "12X3"  , "-media 12X3"  },
    { "29X3"  , "-media 29X3"  },
    { "38X3"  , "-media 38X3"  },
    { "50X3"  , "-media 50X3"  },
    { "54X3"  , "-media 54X3"  },
    { "62X3"  , "-media 62X3"  },
    { "12X4"  , "-media 12X4"  },
    { "29X4"  , "-media 29X4"  },
    { "38X4"  , "-media 38X4"  },
    { "50X4"  , "-media 50X4"  },
    { "54X4"  , "-media 54X4"  },
    { "62X4"  , "-media 62X4"  },

    { NULL , NULL  }
};

PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    ppdCmdListPSize
};

/*
//       ***** DefaultBrAutoTapeCut*****
PPDCOMMANDLIST   ppdCmdListAutoCut[] =
{
    { "OFF", "-cut OFF" },
    { "ON" , "-cut ON"  },
    { NULL , NULL  }
};

PPDCOMMAND  DefaultBrAutoTapeCut={
    "DefaultBrAutoTapeCut",
    ppdCmdListAutoCut
};
*/
//       ***** DefaultBrCutAtEnd*****
PPDCOMMANDLIST ppdCmdListCutAtEnd[ ] =
{
    { "OFF", "-cutend OFF" },
    { "ON" , "-cutend ON"  },
    { NULL , NULL  }
};

PPDCOMMAND  DefaultBrCutAtEnd={
    "DefaultBrCutAtEnd",
    ppdCmdListCutAtEnd
};

//       ***** DefaultBrCompress*****
PPDCOMMANDLIST ppdCmdListCompress[ ] =
{
    { "OFF", "-compress OFF" },
    { "ON" , "-compress ON"  },
    { NULL , NULL  }
};

PPDCOMMAND  DefaultBrCompress={
    "DefaultBrCompress",
    ppdCmdListCompress
};

//       ***** DefaultBRMirror*****
PPDCOMMANDLIST   ppdCmdListMirr[] =
{
    { "OFF", "-mirro OFF" },
    { "ON" , "-mirro ON"  },
    { NULL , NULL  }
};

PPDCOMMAND  DefaultBrMirror={
    "DefaultBrMirror",
    ppdCmdListMirr
};

//       ***** DefaultBrPriority *****
PPDCOMMANDLIST   ppdCmdListPrio[] =
{
    { "BrSpeed"     , "-quality SPEED"  },
    { "BrQuality"   , "-quality QUALITY"},
    { NULL , NULL  }
};

PPDCOMMAND  DefaultBrPriority={
    "DefaultBrPriority",
    ppdCmdListPrio
};

//    ******DefaultBrHalftonePattern**********
PPDCOMMANDLIST   ppdCmdListHalftone[] =
{
    { "BrErrorDiffusion"    , "-half ERROR" },
    { "BrBinary"            , "-half BINARY"},
    { "BrDither"            , "-half DITHER"},
    { NULL , NULL  }
};

PPDCOMMAND  DefaultBrHalftonePattern={
    "DefaultBrHalftonePattern",
    ppdCmdListHalftone
};

PPDCOMMAND   *ppdcommand_all_list[] = {
    &DefaultBrCompress,
    &DefaultResolution,
    &DefaultPageSize,
    &DefaultBrCutAtEnd,
//    &DefaultBrAutoTapeCut,
    &DefaultBrMirror,
    &DefaultBrPriority,
    &DefaultBrHalftonePattern,
    NULL
};

/*      PPD Default   end*/
//   +++++++++++++++++++++++++++
//       PPD numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST PPDdefaultN[] = {
    { "DefaultBrBrightness"  , "-brit "   },
    { "DefaultBrContrast"    , "-cont "   },
    { "DefaultBrMargin"      , "-feed "   },
    { "DefaultBrCutLabel"    , "-cutlabel"},
    { NULL   ,   NULL    },
};

#define _LABEL_BUF_MAX_     33
#define _LABEL_MAX_     (_LABEL_BUF_MAX_ - 1)
#define _LABLE_ID_HEAD_                "BrL"

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

#endif

