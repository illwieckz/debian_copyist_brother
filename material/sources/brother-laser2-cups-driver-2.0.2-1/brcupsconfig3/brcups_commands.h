 
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


char brprintconf[30]="brprintconflsr2";
//   +++++++++++++++++++++++++++
//       set default
//   +++++++++++++++++++++++++++
char *default_setting[] = {
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -res 600"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -ps AUTO"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -dx OFF"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -dxt LONG"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -pt A4"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -md PLAIN"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -cp 1"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -sp PRINTER"   ,
	     "brprintconflsr2 -P BROTHERPRINTER_XXX  -ts OFF"   ,
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
	   {  "Resolution=300"   ,   "-res 300"       },
	   {  "Resolution=600"   ,   "-res 600"       },
	   {  "Resolution=1200"   ,   "-res 1200"       },
	   {  "Resolution=HQ1200"   ,   "-res HQ1200"       },
	   {  "PaperSource=Manual"   ,   "-ps MANUAL"       },
	   {  "PaperSource=MPTray"   ,   "-ps MP"       },
	   {  "PaperSource=Tray1"   ,   "-ps T1"       },
	   {  "PaperSource=Tray2"   ,   "-ps T2"       },
	   {  "PaperSource=Tray3"   ,   "-ps T3"       },
	   {  "PaperSource=AutoSelect"   ,   "-ps AUTO"       },
	   {  "Duplex=Simplex"   ,   "-dx OFF"       },
	   {  "Duplex=Duplex"   ,   "-dx ON"       },
	   {  "DuplexType=Long"   ,   "-dx ON -dxt LONG"       },
	   {  "DuplexType=Short"   ,   "-dx ON -dxt SHORT"       },
	   {  "PaperType=A4"   ,   "-pt A4"       },
	   {  "PaperType=Letter"   ,   "-pt LT"       },
	   {  "PaperType=Legal"   ,   "-pt LGL"       },
	   {  "PaperType=Exective"   ,   "-pt EXE"       },
	   {  "PaperType=Executive"   ,   "-pt EXE"       },
	   {  "PaperType=A5"   ,   "-pt A5"       },
	   {  "PaperType=A6"   ,   "-pt A6"       },
	   {  "PaperType=B5"   ,   "-pt B5"       },
	   {  "PaperType=B6"   ,   "-pt B6"       },
	   {  "PaperType=C5"   ,   "-pt C5"       },
	   {  "PaperType=EnvC5"   ,   "-pt C5"       },
	   {  "PaperType=DL"   ,   "-pt DL"       },
	   {  "PaperType=EnvDL"   ,   "-pt DL"       },
	   {  "PaperType=Com10"   ,   "-pt COM-10"       },
	   {  "PaperType=COM10"   ,   "-pt COM-10"       },
	   {  "PaperType=Com-10"   ,   "-pt COM-10"       },
	   {  "PaperType=COM-10"   ,   "-pt COM-10"       },
	   {  "PaperType=EnvCOM10"   ,   "-pt COM-10"       },
	   {  "PaperType=Env10"   ,   "-pt COM-10"       },
	   {  "PaperType=Monarch"   ,   "-pt MONARCH"       },
	   {  "PaperType=EnvMonarch"   ,   "-pt MONARCH"       },
	   {  "Media=PlainPaper"   ,   "-md PLAIN"       },
	   {  "Media=ThinPaper"   ,   "-md THIN"       },
	   {  "Media=ThickPaper"   ,   "-md THICK"       },
	   {  "Media=ThickerPaper"   ,   "-md THICKER"       },
	   {  "Media=BondPaper"   ,   "-md BOND"       },
	   {  "Media=Transparencies"   ,   "-md TRANS"       },
	   {  "Media=Envelopes"   ,   "-md ENV"       },
	   {  "Media=Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "Media=Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "Sleep=PrinterDefault"   ,   "-sp PRINTER"       },
	   { NULL   ,   NULL    },
};
//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	   {  "media=A4"   ,   "-pt A4"       },
	   {  "media=Letter"   ,   "-pt LT"       },
	   {  "media=Legal"   ,   "-pt LGL"       },
	   {  "media=Exective"   ,   "-pt EXE"       },
	   {  "media=Executive"   ,   "-pt EXE"       },
	   {  "media=A5"   ,   "-pt A5"       },
	   {  "media=A6"   ,   "-pt A6"       },
	   {  "media=B5"   ,   "-pt B5"       },
	   {  "media=B6"   ,   "-pt B6"       },
	   {  "media=C5"   ,   "-pt C5"       },
	   {  "media=EnvC5"   ,   "-pt C5"       },
	   {  "media=DL"   ,   "-pt DL"       },
	   {  "media=EnvDL"   ,   "-pt DL"       },
	   {  "media=Com10"   ,   "-pt COM-10"       },
	   {  "media=COM10"   ,   "-pt COM-10"       },
	   {  "media=Com-10"   ,   "-pt COM-10"       },
	   {  "media=COM-10"   ,   "-pt COM-10"       },
	   {  "media=EnvCOM10"   ,   "-pt COM-10"       },
	   {  "media=Env10"   ,   "-pt COM-10"       },
	   {  "media=Monarch"   ,   "-pt MONARCH"       },
	   {  "media=EnvMonarch"   ,   "-pt MONARCH"       },
	   {  "media=PlainPaper"   ,   "-md PLAIN"       },
	   {  "media=ThinPaper"   ,   "-md THIN"       },
	   {  "media=ThickPaper"   ,   "-md THICK"       },
	   {  "media=ThickerPaper"   ,   "-md THICKER"       },
	   {  "media=BondPaper"   ,   "-md BOND"       },
	   {  "media=Transparencies"   ,   "-md TRANS"       },
	   {  "media=Envelopes"   ,   "-md ENV"       },
	   {  "media=Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "media=Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "media=Manual"   ,   "-ps MANUAL"       },
	   {  "media=MPTray"   ,   "-ps MP"       },
	   {  "media=Tray1"   ,   "-ps T1"       },
	   {  "media=Tray2"   ,   "-ps T2"       },
	   {  "media=Tray3"   ,   "-ps T3"       },
	   {  "media=AutoSelect"   ,   "-ps AUTO"       },
	   {  "media=.*,A4"   ,   "-pt A4"       },
	   {  "media=.*,Letter"   ,   "-pt LT"       },
	   {  "media=.*,Legal"   ,   "-pt LGL"       },
	   {  "media=.*,Exective"   ,   "-pt EXE"       },
	   {  "media=.*,Executive"   ,   "-pt EXE"       },
	   {  "media=.*,A5"   ,   "-pt A5"       },
	   {  "media=.*,A6"   ,   "-pt A6"       },
	   {  "media=.*,B5"   ,   "-pt B5"       },
	   {  "media=.*,B6"   ,   "-pt B6"       },
	   {  "media=.*,C5"   ,   "-pt C5"       },
	   {  "media=.*,EnvC5"   ,   "-pt C5"       },
	   {  "media=.*,DL"   ,   "-pt DL"       },
	   {  "media=.*,EnvDL"   ,   "-pt DL"       },
	   {  "media=.*,Com10"   ,   "-pt COM-10"       },
	   {  "media=.*,COM10"   ,   "-pt COM-10"       },
	   {  "media=.*,Com-10"   ,   "-pt COM-10"       },
	   {  "media=.*,COM-10"   ,   "-pt COM-10"       },
	   {  "media=.*,EnvCOM10"   ,   "-pt COM-10"       },
	   {  "media=.*,Env10"   ,   "-pt COM-10"       },
	   {  "media=.*,Monarch"   ,   "-pt MONARCH"       },
	   {  "media=.*,EnvMonarch"   ,   "-pt MONARCH"       },
	   {  "media=.*,PlainPaper"   ,   "-md PLAIN"       },
	   {  "media=.*,ThinPaper"   ,   "-md THIN"       },
	   {  "media=.*,ThickPaper"   ,   "-md THICK"       },
	   {  "media=.*,ThickerPaper"   ,   "-md THICKER"       },
	   {  "media=.*,BondPaper"   ,   "-md BOND"       },
	   {  "media=.*,Transparencies"   ,   "-md TRANS"       },
	   {  "media=.*,Envelopes"   ,   "-md ENV"       },
	   {  "media=.*,Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "media=.*,Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "media=.*,Manual"   ,   "-ps MANUAL"       },
	   {  "media=.*,MPTray"   ,   "-ps MP"       },
	   {  "media=.*,Tray1"   ,   "-ps T1"       },
	   {  "media=.*,Tray2"   ,   "-ps T2"       },
	   {  "media=.*,Tray3"   ,   "-ps T3"       },
	   {  "media=.*,AutoSelect"   ,   "-ps AUTO"       },
	   {  "sides=sides=two-sided-long-edge"   ,   "-dx ON -dxt LONG"       },
	   {  "sides=sides=two-sided-short-edge"   ,   "-dx ON -dxt SHORT"       },
	   {  "sides=sides=one-side"   ,   "-dx OFF"       },
	   { NULL   ,   NULL    },
};
//   +++++++++++++++++++++++++++
//       command line numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST commandlinelist2[] = {
	   {  "Copies="   ,   "-cp "       },
	   {  "Sleep="   ,   "-sp "       },
	   {  "TonerSaveMode="   ,   "-ts "       },
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
            { "300dpi"  , "-res 300"  },
            { "600dpi"  , "-res 600"  },
            { "1200dpi" , "-res 1200" },
            { "HQ1200dpi" , "-res HQ1200" },
            { "1200x600dpi" , "-res HQ1200" },
            { "2400x600dpi" , "-res HQ1200" },
 	    { NULL , NULL  }    
  }
};

//       ***** DefaultPageSize *****         
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    {
 	    { "A4" , "-pt A4"  },
 	    { "Letter" , "-pt LT"  },
 	    { "Legal" , "-pt LGL"  },
 	    { "Executive" , "-pt EXE"  },
 	    { "A5" , "-pt A5"  },
 	    { "A6" , "-pt A6"  },
 	    { "B5" , "-pt B5"  },
 	    { "EnvISOB5" , "-pt B5"  },
 	    { "B6" , "-pt B6"  },
 	    { "EnvISOB6" , "-pt B6"  },
 	    { "C5" , "-pt C5"  },
 	    { "EnvC5" , "-pt C5"  },
 	    { "DL" , "-pt DL"  },
 	    { "EnvDL" , "-pt DL"  },
 	    { "Env10" , "-pt COM-10"  },
 	    { "EnvMonarch" , "-pt MONARCH"  },
 	    { NULL , NULL  }
     }
};
//       ***** DefaultBrMediaType *****         
PPDCOMMAND  DefaultBrMediaType={
    "DefaultBrMediaType",
    {
 	    { "PLAIN" , "-md PLAIN"  },
 	    { "THIN" , "-md THIN"  },
 	    { "THICK" , "-md THICK"  },
 	    { "THICKERPAPER2" , "-md THICKER"  },
 	    { "BOND" , "-md BOND"  },
 	    { "TRANSPARENCIES" , "-md TRANS"  },
 	    { "ENV" , "-md ENV"  },
 	    { "ENVTHICK" , "-md ENV-THICK"  },
 	    { "ENVTHIN" , "-md ENV-THIN"  },
 	    { NULL , NULL  }
     }
};
//       ***** DefaultInputSlot *****         
PPDCOMMAND  DefaultInputSlot={
    "DefaultInputSlot",
    {
 	    { "MANUAL" , "-ps MANUAL"  },
 	    { "MPTRAY" , "-ps MP"  },
 	    { "TRAY1" , "-ps T1"  },
 	    { "TRAY2" , "-ps T2"  },
 	    { "TRAY3" , "-ps T3"  },
 	    { "AUTO" , "-ps AUTO"  },
 	    { NULL , NULL  }
     }
};
//       ***** DefaultDuplex *****         
PPDCOMMAND  DefaultDuplex={
    "DefaultDuplex",
    {
 	    { "DuplexTumble" , "-dx ON -dxt LONG"  },
 	    { "DuplexNoTumble" , "-dx ON -dxt SHORT"  },
 	    { "None" , "-dx OFF"  },
 	    { NULL , NULL  }
     }
};
//       ***** DefaultTonerSaveMode *****         
PPDCOMMAND  DefaultTonerSaveMode={
    "DefaultTonerSaveMode",
    {
 	    { "Off" , "-ts OFF"  },
 	    { "On" , "-ts ON"  },
 	    { NULL , NULL  }
     }
};
//       ***** DefaultSleep *****         
PPDCOMMAND  DefaultSleep={
    "DefaultSleep",
    {
 	    { "PrinterDefault" , "-sp PRINTER"  },
 	    { "2minutes" , "-sp 2"  },
 	    { "10minutes" , "-sp 10"  },
 	    { "30minutes" , "-sp 30"  },
 	    { NULL , NULL  }
     }
};
PPDCOMMAND   *ppdcommand_all_list[] = {
      &DefaultResolution,
      &DefaultPageSize,
      &DefaultBrMediaType,
      &DefaultInputSlot,
      &DefaultDuplex,
      &DefaultTonerSaveMode,
      &DefaultSleep,
      NULL
};
/*      PPD Default   end*/
//   +++++++++++++++++++++++++++
//       PPD numerical value
//   +++++++++++++++++++++++++++
CMDLINELIST PPDdefaultN[] = {
	   { NULL   ,   NULL    },
};




//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_media_commandlinelist[] = {
	   {  "A4"   ,   "-pt A4"       },
	   {  "Letter"   ,   "-pt LT"       },
	   {  "Legal"   ,   "-pt LGL"       },
	   {  "Exective"   ,   "-pt EXE"       },
	   {  "Executive"   ,   "-pt EXE"       },
	   {  "A5"   ,   "-pt A5"       },
	   {  "A6"   ,   "-pt A6"       },
	   {  "B5"   ,   "-pt B5"       },
	   {  "B6"   ,   "-pt B6"       },
	   {  "C5"   ,   "-pt C5"       },
	   {  "EnvC5"   ,   "-pt C5"       },
	   {  "DL"   ,   "-pt DL"       },
	   {  "EnvDL"   ,   "-pt DL"       },
	   {  "Com10"   ,   "-pt COM-10"       },
	   {  "COM10"   ,   "-pt COM-10"       },
	   {  "Com-10"   ,   "-pt COM-10"       },
	   {  "COM-10"   ,   "-pt COM-10"       },
	   {  "EnvCOM10"   ,   "-pt COM-10"       },
	   {  "Env10"   ,   "-pt COM-10"       },
	   {  "Monarch"   ,   "-pt MONARCH"       },
	   {  "EnvMonarch"   ,   "-pt MONARCH"       },
	   {  "PlainPaper"   ,   "-md PLAIN"       },
	   {  "ThinPaper"   ,   "-md THIN"       },
	   {  "ThickPaper"   ,   "-md THICK"       },
	   {  "ThickerPaper"   ,   "-md THICKER"       },
	   {  "BondPaper"   ,   "-md BOND"       },
	   {  "Transparencies"   ,   "-md TRANS"       },
	   {  "Envelopes"   ,   "-md ENV"       },
	   {  "Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "Manual"   ,   "-ps MANUAL"       },
	   {  "MPTray"   ,   "-ps MP"       },
	   {  "Tray1"   ,   "-ps T1"       },
	   {  "Tray2"   ,   "-ps T2"       },
	   {  "Tray3"   ,   "-ps T3"       },
	   {  "AutoSelect"   ,   "-ps AUTO"       },
	   { NULL   ,   NULL    }
};


