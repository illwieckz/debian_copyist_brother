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
// +++++++++++++++++++++++++++
// set default
// +++++++++++++++++++++++++++
char *default_setting[] = {
    // Collate
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -collate OFF",

    // cut at end
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -cutend OFF",

    // cutlabel
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -cutlabel 0",

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

    // Rotate.
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -rotate OFF",

    // Peeler.
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -peeler OFF",

    // Quality
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -quality SPEED",

    // Resolution
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -reso 203",

    // Margin(Feed)
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -feed 0",

    // Media
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -media 50x30",

    // copies
    "brprintconfpt1 -P BROTHERPRINTER_XXX  -copy 1",

    NULL
};

typedef struct {
    char *option;
    char *value;
} CMDLINELIST;
// +++++++++++++++++++++++++++
// command line   (Brother command)
// +++++++++++++++++++++++++++
CMDLINELIST commandlinelist[] = {
    // resolution
    { "Resolution=203"  ,   "-reso 203" },

    // Collate
    { "Collate=OFF"     ,   "-collate OFF" },
    { "Collate=ON"      ,   "-collate ON" },

    // CutAtEnd
    { "CutAtEnd=OFF"    ,   "-cutend OFF" },
    { "CutAtEnd=ON"     ,   "-cutend ON" },
    { "noCutAtEnd"      ,   "-cutend OFF" },
    { "CutAtEnd"        ,   "-cutend ON" },

    // Trimtapeafterdata
    { "Trimtape=OFF"    ,   "-trimtape OFF" },
    { "Trimtape=ON"     ,   "-trimtape ON" },
    { "noTrimtape"      ,   "-trimtape OFF" },
    { "Trimtape"        ,   "-trimtape ON" },

    // Compress
    { "Compress=OFF"    ,   "-compress OFF" },
    { "Compress=ON"     ,   "-compress ON" },
    { "noCompress"      ,   "-compress OFF" },
    { "Compress"        ,   "-compress ON" },

    // Mirror
    { "MirrorPrint=OFF" ,   "-mirro OFF" },
    { "MirrorPrint=ON"  ,   "-mirro ON" },
    { "noMirrorPrint"   ,   "-mirro OFF" },
    { "MirrorPrint"     ,   "-mirro ON" },

    // Rotate
    {  "RotatePrint=OFF",   "-rotate OFF"    },
    {  "RotatePrint=ON" ,   "-rotate ON"    },
    {  "noRotatePrint"  ,   "-rotate OFF"    },
    {  "RotatePrint"    ,   "-rotate ON"    },

    // For peeler.
    {  "Peeler=OFF"     ,   "-peeler OFF"    },
    {  "Peeler=ON"      ,   "-peeler ON"    },
    {  "noPeeler"       ,   "-peeler OFF"    },
    {  "Peeler"         ,   "-peeler ON"    },

    // HalftonePattern
    { "Halftone=ERROR"  ,   "-half ERROR" },
    { "Halftone=BINARY" ,   "-half BINARY" },
    { "Halftone=DITHER" ,   "-half DITHER" },

    // Quality
    { "Priority=SPEED"  ,   "-quality SPEED" },
    { "Priority=QUALITY",   "-quality QUALITY" },


    { NULL    ,    NULL },
};


// +++++++++++++++++++++++++++
// command line   (Standard command)
// +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
    // Paper Size
    { "media=30x30"    ,    "-media 30x30" },
    { "media=40x40"    ,    "-media 40x40" },
    { "media=40x50"    ,    "-media 40x50" },
    { "media=40x60"    ,    "-media 40x60" },
    { "media=50x30"    ,    "-media 50x30" },
    { "media=51x26"    ,    "-media 51x26" },
    { "media=60x60"    ,    "-media 60x60" },
    { "media=57X1"     ,    "-media 57X1" },
    { "media=58X1"     ,    "-media 58X1" },
    { NULL    ,    NULL },
};

// +++++++++++++++++++++++++++
// command line numerical value
// +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
    { "Copies="        ,    "-copy " },
    { "Brightness="    ,    "-brit " },
    { "Contrast="    ,    "-cont " },
    { "Feed="        ,    "-feed " },

    { "CutLabel="    ,    "-cutlabel " },
    { NULL    ,    NULL },
};
/* PPD Default */
// +++++++++++++++++++++++++++
// PPD setting list
// +++++++++++++++++++++++++++



typedef struct {
    char *value;
    char *brcommand;
} PPDCOMMANDLIST;


typedef struct {
    char            *label;
    PPDCOMMANDLIST    *ppdcommandlist;
} PPDCOMMAND;

// ***** DefaultResolution *****
PPDCOMMANDLIST   ppdCmdListRes[] =
    {
        { "Normal"  ,   "-reso 203" },
        { NULL      ,   NULL }
    };

PPDCOMMAND  DefaultResolution={
    "DefaultResolution",
    ppdCmdListRes
};

// ***** DefaultPageSize *****
PPDCOMMANDLIST   ppdCmdListPSize[] =
{
    { "30x30"    ,    "-media 30x30" },
    { "40x40"    ,    "-media 40x40" },
    { "40x50"    ,    "-media 40x50" },
    { "40x60"    ,    "-media 40x60" },
    { "50x30"    ,    "-media 50x30" },
    { "51x26"    ,    "-media 51x26" },
    { "60x60"    ,    "-media 60x60" },
    { "57X1"     ,    "-media 57X1" },
    { "58X1"     ,    "-media 58X1" },
    { NULL    ,    NULL }
};
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    ppdCmdListPSize
};

//Cut At End
// ***** DefaultBrCutAtEnd*****
PPDCOMMANDLIST ppdCmdListCutAtEnd[ ] =
{
    { "OFF", "-cutend OFF" },
    { "ON" , "-cutend ON" },
    { NULL , NULL }
};
PPDCOMMAND  DefaultBrCutAtEnd={
    "DefaultBrCutAtEnd",
    ppdCmdListCutAtEnd
};

// Trim Tape
PPDCOMMANDLIST ppdCmdListTrimtape[ ] =
{
    { "OFF", "-trimtape OFF" },
    { "ON" , "-trimtape ON" },
    { NULL , NULL }
};
PPDCOMMAND  DefaultBrTrimtape={
    "DefaultBrTrimtape",
    ppdCmdListTrimtape
};


// compress
PPDCOMMANDLIST ppdCmdListCompress[ ] =
{
    { "OFF", "-compress OFF" },
    { "ON" , "-compress ON" },
    { NULL , NULL }
};
PPDCOMMAND  DefaultBrCompress={
    "DefaultBrCompress",
    ppdCmdListCompress
};



// ***** DefaultBRMirror*****
PPDCOMMANDLIST   ppdCmdListMirr[] =
{
    { "OFF", "-mirro OFF" },
    { "ON" , "-mirro ON" },
    { NULL , NULL }
};
PPDCOMMAND  DefaultBrMirror={
    "DefaultBrMirror",
    ppdCmdListMirr
};

// ***** DefaultBRRotate*****
PPDCOMMANDLIST   ppdCmdListRotate[] =
{
    { "OFF", "-rotate OFF"  },
    { "ON" , "-rotate ON"  },
    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrRotate={
    "DefaultBrRotate",
    ppdCmdListRotate
};

// ***** DefaultBRPeeler***** For peeler.
PPDCOMMANDLIST   ppdCmdListPeeler[] =
{
    { "OFF", "-peeler OFF"  },
    { "ON" , "-peeler ON"  },
    { NULL , NULL  }
};
PPDCOMMAND  DefaultBrPeeler={
    "DefaultBrPeeler",
    ppdCmdListPeeler
};

// ***** DefaultBrPriority *****
PPDCOMMANDLIST   ppdCmdListPrio[] =
{
    { "BrSpeed"      ,    "-quality SPEED" },
    { "BrQuality"    ,    "-quality QUALITY" },
    { NULL    ,    NULL }
};
PPDCOMMAND  DefaultBrPriority={
    "DefaultBrPriority",
    ppdCmdListPrio
};

// ******DefaultBrHalftonePattern**********
PPDCOMMANDLIST   ppdCmdListHalftone[] =
{
    { "BrErrorDiffusion",    "-half ERROR" },
    { "BrBinary"        ,    "-half BINARY" },
    { "BrDither"        ,    "-half DITHER" },
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
    &DefaultBrTrimtape,
    &DefaultBrMirror,
    &DefaultBrRotate,
    &DefaultBrPeeler,
    &DefaultBrPriority,
    &DefaultBrHalftonePattern,
    NULL
};

/*      PPD Default   end*/
//   +++++++++++++++++++++++++++
//       PPD numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST PPDdefaultN[] = {
    {"DefaultBrBrightness"  , "-brit "},
    {"DefaultBrContrast"    , "-cont "},
    {"DefaultBrMargin"      , "-feed "},
    {"DefaultBrCutLabel"    , "-cutlabel"},
    { NULL    ,    NULL },
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

