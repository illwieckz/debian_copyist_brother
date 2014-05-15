 
//* Brother CUPS wrapper tool
//* Copyright (C) 2005-2013 Brother. Industries, Ltd.//*
//*
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




char brprintconf[64]="brprintconflsr3";
//   +++++++++++++++++++++++++++
//       set default
//   +++++++++++++++++++++++++++
char *default_setting[] = {
	     "-res 600"   ,
	     "-ps AUTO"   ,
	     "-dx OFF"   ,
	     "-dxt LONG"   ,
	     "-pt A4"   ,
	     "-md PLAIN"   ,
	     "-cp 1"   ,
	     "-sp PRINTER"   ,
	     "-ts OFF"   ,
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
	   {  "Resolution=600x300"   ,   "-res 600x300"       },
	   {  "Resolution=1200"   ,   "-res 1200"       },
	   {  "Resolution=HQ1200"   ,   "-res HQ1200"       },
	   {  "Resolution=HQ1200A"   ,   "-res HQ1200A"       },
	   {  "Resolution=HQ1200B"   ,   "-res HQ1200B"       },
	   {  "PaperSource=Manual"   ,   "-ps MANUAL"       },
	   {  "PaperSource=MPTray"   ,   "-ps MP"       },
	   {  "PaperSource=Tray1"   ,   "-ps T1"       },
	   {  "PaperSource=Tray2"   ,   "-ps T2"       },
	   {  "PaperSource=Tray3"   ,   "-ps T3"       },
	   {  "PaperSource=Tray4"   ,   "-ps T4"       },
	   {  "PaperSource=Tray5"   ,   "-ps T5"       },
	   {  "PaperSource=AutoSelect"   ,   "-ps AUTO"       },
	   {  "Duplex=Simplex"   ,   "-dx OFF"       },
	   {  "Duplex=Duplex"   ,   "-dx ON"       },
	   {  "DuplexType=Long"   ,   "-dx ON -dxt LONG"       },
	   {  "DuplexType=Short"   ,   "-dx ON -dxt SHORT"       },
	   {  "PaperType=A4"   ,   "-pt A4"       },
	   {  "PaperType=FanFoldGermanLegal"   ,   "-pt FOLIO"       },
	   {  "PaperType=210x270mm"   ,   "-pt A4L"       },
	   {  "PaperType=Letter"   ,   "-pt LT"       },
	   {  "PaperType=Legal"   ,   "-pt LGL"       },
	   {  "PaperType=Exective"   ,   "-pt EXE"       },
	   {  "PaperType=Executive"   ,   "-pt EXE"       },
	   {  "PaperType=A5"   ,   "-pt A5"       },
	   {  "PaperType=A6"   ,   "-pt A6"       },
	   {  "PaperType=B5"   ,   "-pt JISB5"       },
	   {  "PaperType=ISOB5"   ,   "-pt B5"       },
	   {  "PaperType=JISB5"   ,   "-pt JISB5"       },
	   {  "PaperType=B6"   ,   "-pt B6"       },
	   {  "PaperType=ISOB6"   ,   "-pt B6"       },
           {  "PaperType=JISB6"   ,   "-pt JISB6"       },
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
	   {  "PaperType=4x6"   ,   "-pt PC4X6"       },
	   {  "PaperType=Postcard"   ,   "-pt POSTCARD"       },
	   {  "PaperType=DoublePostcardRotated"   ,   "-pt DHAGAKI"       },
	   {  "PaperType=DHagaki"   ,   "-pt DHAGAKI"       },
	   {  "PaperType=EnvYou4"   ,   "-pt ENVYOU4"       },
	   {  "PaperType=EnvChou3"   ,   "-pt ENVCHOU3"       },
	   {  "PaperType=EnvChou4"   ,   "-pt ENVCHOU4"       },
           {  "PaperType=3x5"   ,   "-pt 3X5"       },
	   {  "PaperType=CUSTOM0"   ,   "-pt CUSTOM0"       },
	   {  "PaperType=CUSTOM1"   ,   "-pt CUSTOM1"       },
	   {  "PaperType=CUSTOM2"   ,   "-pt CUSTOM2"       },
	   {  "PaperType=CUSTOM3"   ,   "-pt CUSTOM3"       },
	   {  "PaperType=CUSTOM4"   ,   "-pt CUSTOM4"       },
	   {  "PaperType=CUSTOM5"   ,   "-pt CUSTOM5"       },
	   {  "PaperType=CUSTOM6"   ,   "-pt CUSTOM6"       },
	   {  "PaperType=CUSTOM7"   ,   "-pt CUSTOM7"       },
	   {  "PaperType=CUSTOM8"   ,   "-pt CUSTOM8"       },
	   {  "PaperType=CUSTOM9"   ,   "-pt CUSTOM9"       },
	   {  "Media=PlainPaper"   ,   "-md PLAIN"       },
	   {  "Media=ThinPaper"   ,   "-md THIN"       },
	   {  "Media=ThickPaper"   ,   "-md THICK"       },
	   {  "Media=ThickerPaper"   ,   "-md THICKER"       },
	   {  "Media=BondPaper"   ,   "-md BOND"       },
	   {  "Media=Transparencies"   ,   "-md TRANS"       },
	   {  "Media=Envelopes"   ,   "-md ENV"       },
	   {  "Media=Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "Media=Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "Media=Recycled"   ,   "-md RECYCLED"       },
	   {  "Sleep=PrinterDefault"   ,   "-sp PRINTER"       },
	   { NULL   ,   NULL    },
};
//   +++++++++++++++++++++++++++
//       command line   (Standard command)
//   +++++++++++++++++++++++++++
CMDLINELIST standard_commandlinelist[] = {
	   {  "media=A4"   ,   "-pt A4"       },
	   {  "media=FanFoldGermanLegal"   ,   "-pt FOLIO"       },
	   {  "media=210x270mm"   ,   "-pt A4L"       },
	   {  "media=Letter"   ,   "-pt LT"       },
	   {  "media=Legal"   ,   "-pt LGL"       },
	   {  "media=Exective"   ,   "-pt EXE"       },
	   {  "media=Executive"   ,   "-pt EXE"       },
	   {  "media=A5"   ,   "-pt A5"       },
	   {  "media=A6"   ,   "-pt A6"       },
	   {  "media=B5"   ,   "-pt JISB5"       },
	   {  "media=ISOB5"   ,   "-pt B5"       },
	   {  "media=JISB5"   ,   "-pt JISB5"       },
	   {  "media=B6"   ,   "-pt B6"       },
	   {  "media=ISOB6"   ,   "-pt B6"       },
           {  "media=JISB6"   ,   "-pt JISB6"       },
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
	   {  "media=4x6"   ,   "-pt PC4X6"       },
	   {  "media=Postcard"   ,   "-pt POSTCARD"       },
	   {  "media=DoublePostcardRotated"   ,   "-pt DHAGAKI"       },
	   {  "media=DHagaki"   ,   "-pt DHAGAKI"       },
	   {  "media=EnvYou4"   ,   "-pt ENVYOU4"       },
	   {  "media=EnvChou3"   ,   "-pt ENVCHOU3"       },
	   {  "media=EnvChou4"   ,   "-pt ENVCHOU4"       },
	   {  "media=3x5"   ,   "-pt 3X5"       },
	   {  "media=CUSTOM0"   ,   "-pt CUSTOM0"       },
	   {  "media=CUSTOM1"   ,   "-pt CUSTOM1"       },
	   {  "media=CUSTOM2"   ,   "-pt CUSTOM2"       },
	   {  "media=CUSTOM3"   ,   "-pt CUSTOM3"       },
	   {  "media=CUSTOM4"   ,   "-pt CUSTOM4"       },
	   {  "media=CUSTOM5"   ,   "-pt CUSTOM5"       },
	   {  "media=CUSTOM6"   ,   "-pt CUSTOM6"       },
	   {  "media=CUSTOM7"   ,   "-pt CUSTOM7"       },
	   {  "media=CUSTOM8"   ,   "-pt CUSTOM8"       },
	   {  "media=CUSTOM9"   ,   "-pt CUSTOM9"       },
	   {  "media=PlainPaper"   ,   "-md PLAIN"       },
	   {  "media=ThinPaper"   ,   "-md THIN"       },
	   {  "media=ThickPaper"   ,   "-md THICK"       },
	   {  "media=ThickerPaper"   ,   "-md THICKER"       },
	   {  "media=BondPaper"   ,   "-md BOND"       },
	   {  "media=Transparencies"   ,   "-md TRANS"       },
	   {  "media=Envelopes"   ,   "-md ENV"       },
	   {  "media=Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "media=Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "media=Recycled"   ,   "-md RECYCLED"       },
	   {  "media=Manual"   ,   "-ps MANUAL"       },
	   {  "media=MPTray"   ,   "-ps MP"       },
	   {  "media=Tray1"   ,   "-ps T1"       },
	   {  "media=Tray2"   ,   "-ps T2"       },
	   {  "media=Tray3"   ,   "-ps T3"       },
	   {  "media=Tray4"   ,   "-ps T4"       },
	   {  "media=Tray5"   ,   "-ps T5"       },
	   {  "media=AutoSelect"   ,   "-ps AUTO"       },
	   {  "media=.*,A4"   ,   "-pt A4"       },
	   {  "media=.*,FanFoldGermanLegal"   ,   "-pt FOLIO"       },
	   {  "media=.*,210x270mm"   ,   "-pt A4L"       },
	   {  "media=.*,Letter"   ,   "-pt LT"       },
	   {  "media=.*,Legal"   ,   "-pt LGL"       },
	   {  "media=.*,Exective"   ,   "-pt EXE"       },
	   {  "media=.*,Executive"   ,   "-pt EXE"       },
	   {  "media=.*,A5"   ,   "-pt A5"       },
	   {  "media=.*,A6"   ,   "-pt A6"       },
	   {  "media=.*,B5"   ,   "-pt JISB5"       },
	   {  "media=.*,ISOB5"   ,   "-pt B5"       },
	   {  "media=.*,JISB5"   ,   "-pt JISB5"       },
	   {  "media=.*,B6"   ,   "-pt B6"       },
	   {  "media=.*,ISOB6"   ,   "-pt B6"       },
           {  "media=.*,JISB6"   ,   "-pt JISB6"       },
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
	   {  "media=.*,4x6"   ,   "-pt PC4X6"       },
	   {  "media=.*,Postcard"   ,   "-pt POSTCARD"       },
	   {  "media=.*,DoublePostcardRotated"   ,   "-pt DHAGAKI"       },
	   {  "media=.*,DHagaki"   ,   "-pt DHAGAKI"       },
	   {  "media=.*,EnvYou4"   ,   "-pt ENVYOU4"       },
	   {  "media=.*,EnvChou3"   ,   "-pt ENVCHOU3"       },
	   {  "media=.*,EnvChou4"   ,   "-pt ENVCHOU4"       },
           {  "media=.*,3x5"   ,   "-pt 3X5"       },
	   {  "media=.*,CUSTOM0"   ,   "-pt CUSTOM0"       },
	   {  "media=.*,CUSTOM1"   ,   "-pt CUSTOM1"       },
	   {  "media=.*,CUSTOM2"   ,   "-pt CUSTOM2"       },
	   {  "media=.*,CUSTOM3"   ,   "-pt CUSTOM3"       },
	   {  "media=.*,CUSTOM4"   ,   "-pt CUSTOM4"       },
	   {  "media=.*,CUSTOM5"   ,   "-pt CUSTOM5"       },
	   {  "media=.*,CUSTOM6"   ,   "-pt CUSTOM6"       },
	   {  "media=.*,CUSTOM7"   ,   "-pt CUSTOM7"       },
	   {  "media=.*,CUSTOM8"   ,   "-pt CUSTOM8"       },
	   {  "media=.*,CUSTOM9"   ,   "-pt CUSTOM9"       },
	   {  "media=.*,PlainPaper"   ,   "-md PLAIN"       },
	   {  "media=.*,ThinPaper"   ,   "-md THIN"       },
	   {  "media=.*,ThickPaper"   ,   "-md THICK"       },
	   {  "media=.*,ThickerPaper"   ,   "-md THICKER"       },
	   {  "media=.*,BondPaper"   ,   "-md BOND"       },
	   {  "media=.*,Transparencies"   ,   "-md TRANS"       },
	   {  "media=.*,Envelopes"   ,   "-md ENV"       },
	   {  "media=.*,Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "media=.*,Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "media=.*,Recycled"   ,   "-md RECYCLED"       },
	   {  "media=.*,Manual"   ,   "-ps MANUAL"       },
	   {  "media=.*,MPTray"   ,   "-ps MP"       },
	   {  "media=.*,Tray1"   ,   "-ps T1"       },
	   {  "media=.*,Tray2"   ,   "-ps T2"       },
	   {  "media=.*,Tray3"   ,   "-ps T3"       },
	   {  "media=.*,Tray4"   ,   "-ps T4"       },
	   {  "media=.*,Tray5"   ,   "-ps T5"       },
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
            { "600x300dpi"  , "-res 600x300"  },
            { "1200dpi" , "-res 1200" },
            { "HQ1200dpi" , "-res HQ1200" },
            { "1200x600dpi" , "-res HQ1200A" },
            { "2400x600dpi" , "-res HQ1200B" },
 	    { NULL , NULL  }    
  }
};

//       ***** DefaultPageSize *****         
PPDCOMMAND  DefaultPageSize={
    "DefaultPageSize",
    {
 	    { "A4" , "-pt A4"  },
 	    { "FanFoldGermanLegal" , "-pt FOLIO"  },
	    { "210x270mm"   ,   "-pt A4L"       },
 	    { "Letter" , "-pt LT"  },
 	    { "Legal" , "-pt LGL"  },
 	    { "Executive" , "-pt EXE"  },
 	    { "A5" , "-pt A5"  },
 	    { "A6" , "-pt A6"  },
 	    { "B5" , "-pt JISB5"  },
 	    { "ISOB5" , "-pt B5"  },
 	    { "JISB5" , "-pt JISB5"  },
 	    { "B6" , "-pt B6"  },
 	    { "ISOB6" , "-pt B6"  },
            { "JISB6" , "-pt JISB6"  },
 	    { "C5" , "-pt C5"  },
 	    { "EnvC5" , "-pt C5"  },
 	    { "DL" , "-pt DL"  },
 	    { "EnvDL" , "-pt DL"  },
 	    { "Env10" , "-pt COM-10"  },
 	    { "EnvMonarch" , "-pt MONARCH"  },
	    { "Monarch" , "-pt MONARCH"  },
 	    { "4x6"   ,   "-pt PC4X6"       },
 	    { "Postcard" , "-pt POSTCARD"  },
 	    { "DoublePostcardRotated" , "-pt DHAGAKI"  },
	    { "DHagaki" , "-pt DHAGAKI"  },
	    { "EnvYou4"   ,   "-pt ENVYOU4"       },
	    { "EnvChou3"   ,   "-pt ENVCHOU3"       },
	    { "EnvChou4"   ,   "-pt ENVCHOU4"       },
            { "3x5"   ,   "-pt 3X5"       },
	    { "CUSTOM0"   ,   "-pt CUSTOM0"       },
	    { "CUSTOM1"   ,   "-pt CUSTOM1"       },
	    { "CUSTOM2"   ,   "-pt CUSTOM2"       },
	    { "CUSTOM3"   ,   "-pt CUSTOM3"       },
	    { "CUSTOM4"   ,   "-pt CUSTOM4"       },
	    { "CUSTOM5"   ,   "-pt CUSTOM5"       },
	    { "CUSTOM6"   ,   "-pt CUSTOM6"       },
	    { "CUSTOM7"   ,   "-pt CUSTOM7"       },
	    { "CUSTOM8"   ,   "-pt CUSTOM8"       },
	    { "CUSTOM9"   ,   "-pt CUSTOM9"       },
 	    { "Envelopes"   ,   "-md ENV"       },
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
 	    { "RECYCLED" , "-md RECYCLED"  },
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
 	    { "TRAY4" , "-ps T4"  },
 	    { "TRAY5" , "-ps T5"  },
 	    { "AUTO" , "-ps AUTO"  },
 	    { NULL , NULL  }
     }
};
//       ***** DefaultDuplex *****         
PPDCOMMAND  DefaultDuplex={
    "DefaultDuplex",
    {
 	    { "DuplexTumble" , "-dx ON -dxt SHORT"  },
 	    { "DuplexNoTumble" , "-dx ON -dxt LONG"  },
 	    { "None" , "-dx OFF"  },
 	    { NULL , NULL  }
     }
};
//       ***** DefaultTonerSaveMode *****         
PPDCOMMAND  DefaultTonerSaveMode={
    "DefaultTonerSaveMode",
    {
 	    { "OFF" , "-ts OFF"  },
 	    { "ON" , "-ts ON"  },
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
	   {  "FanFoldGermanLegal"   ,   "-pt FOLIO"       },
	   {  "210x270mm"   ,   "-pt A4L"       },
	   {  "Letter"   ,   "-pt LT"       },
	   {  "Legal"   ,   "-pt LGL"       },
	   {  "Exective"   ,   "-pt EXE"       },
	   {  "Executive"   ,   "-pt EXE"       },
	   {  "A5"   ,   "-pt A5"       },
	   {  "A6"   ,   "-pt A6"       },
	   {  "B5"   ,   "-pt JISB5"       },
	   {  "ISOB5"   ,   "-pt B5"       },
	   {  "JISB5"   ,   "-pt JISB5"       },
	   {  "B6"   ,   "-pt B6"       },
	   {  "ISOB6"   ,   "-pt ISOB6"       },
           {  "JISB6"   ,   "-pt JISB6"       },
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
	   {  "4x6"   ,   "-pt PC4X6"       },
	   {  "Postcard"   ,   "-pt POSTCARD"       },
	   {  "DoublePostcardRotated"   ,   "-pt DHAGAKI"       },
	   {  "DHagaki"   ,   "-pt DHAGAKI"       },
	   {  "EnvYou4"   ,   "-pt ENVYOU4"       },
	   {  "EnvChou3"   ,   "-pt ENVCHOU3"       },
	   {  "EnvChou4"   ,   "-pt ENVCHOU4"       },
           {  "3x5"   ,   "-pt 3X5"       },
	   {  "CUSTOM0"   ,   "-pt CUSTOM0"       },
	   {  "CUSTOM1"   ,   "-pt CUSTOM1"       },
	   {  "CUSTOM2"   ,   "-pt CUSTOM2"       },
	   {  "CUSTOM3"   ,   "-pt CUSTOM3"       },
	   {  "CUSTOM4"   ,   "-pt CUSTOM4"       },
	   {  "CUSTOM5"   ,   "-pt CUSTOM5"       },
	   {  "CUSTOM6"   ,   "-pt CUSTOM6"       },
	   {  "CUSTOM7"   ,   "-pt CUSTOM7"       },
	   {  "CUSTOM8"   ,   "-pt CUSTOM8"       },
	   {  "CUSTOM9"   ,   "-pt CUSTOM9"       },
	   {  "PlainPaper"   ,   "-md PLAIN"       },
	   {  "ThinPaper"   ,   "-md THIN"       },
	   {  "ThickPaper"   ,   "-md THICK"       },
	   {  "ThickerPaper"   ,   "-md THICKER"       },
	   {  "BondPaper"   ,   "-md BOND"       },
	   {  "Transparencies"   ,   "-md TRANS"       },
	   {  "Envelopes"   ,   "-md ENV"       },
	   {  "Env.Thick"   ,   "-md ENV-THICK"       },
	   {  "Env.Thin"   ,   "-md ENV-THIN"       },
	   {  "RECYCLED"   ,   "-md RECYCLED"       },
	   {  "Manual"   ,   "-ps MANUAL"       },
	   {  "MPTray"   ,   "-ps MP"       },
	   {  "Tray1"   ,   "-ps T1"       },
	   {  "Tray2"   ,   "-ps T2"       },
	   {  "Tray3"   ,   "-ps T3"       },
	   {  "Tray4"   ,   "-ps T4"       },
	   {  "Tray5"   ,   "-ps T5"       },
	   {  "AutoSelect"   ,   "-ps AUTO"       },
	   { NULL   ,   NULL    }
};


