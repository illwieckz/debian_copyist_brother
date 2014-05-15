/*

 This file is part of the Brother MFC/DCP backend for SANE.

 This program is free software; you can redistribute it and/or modify it
 under the terms of the GNU General Public License as published by the Free
 Software Foundation; either version 2 of the License, or (at your option)
 any later version.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 more details.

 You should have received a copy of the GNU General Public License along with
 this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 Place, Suite 330, Boston, MA  02111-1307  USA

*/



/* ======================================================================

brother.c

SANE backend master module

(C) Marian Matthias Eichholz 2001

Start: 2.4.2001

====================================================================== */

//		Copyright(c) 2005-2009 Brother Industries, Ltd.  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <usb.h>

#define BUILD	1

#ifndef BACKEND_NAME
#if       BRSANESUFFIX == 2
#define BACKEND_NAME brother2
#elif  BRSANESUFFIX == 1
#define BACKEND_NAME brother
#elif  BRSANESUFFIX == 3          //M-LNX-109
#define BACKEND_NAME brother3     //M-LNX-109
#else
Not support (force causing compile error)
#endif   //BRSANESUFFIX
#endif

#include "sane/sane.h"
#include "sane/config.h"
#include "sane/sanei.h"
#include "sane/sanei_backend.h"
#include "sane/sanei_config.h"
#include "sane/saneopts.h"


#include "brother_netdev.h"
#include "brother_advini.h"

#include "brother.h"
#include "brother_mfcinfo.h"
#include "brother_devaccs.h"
#include "brother_devinfo.h"
#include "brother_cmatch.h"
#include "brother_scanner.h"
#include "brother_bugchk.h"
#include "brother_log.h"


TDevice *g_pdev;

static int                num_devices;	// 
static TDevice           *pdevFirst;	// 
static Brother_Scanner   *pinstFirst;	// 

/* =====================================================================*/

#include "brother_cmatch.c"
#include "brother_devaccs.c"
#include "brother_devinfo.c"
#include "brother_mfccmd.c"
#include "brother_misc.c"
#include "brother_modelinf.c"
#include "brother_scanner.c"
#include "brother_bugchk.c"
#include "brother_log.c"
#include "brother_netdev.c"
#include "brother_advini.c"

/* ======================================================================

Initialise SANE options

====================================================================== */

#define DEBUG_MODELINF

static const SANE_Range rangeLumi = {
  SANE_FIX(-50.0),
  SANE_FIX(50.0),
  SANE_FIX(1.0) };

#define NUM_SCANMODE (5+1)
#define NUM_RESO (12+1)

//#define NUM_SCANSRC (3+1)   //06/03/02 FB,ADF,ADF_DUPLEX

// M-LNX-103 measure for the center aligned document
#define NUM_SCANSRC (SCAN_SRC_COUNT+1)  //FB,ADF,ADF_DUPLEX,ADFC,ADF_DUPLEXC


static SANE_String_Const * scanModeList = 0;

static SANE_Int *scanResoList = 0;

static SANE_String_Const * scanSrcList = 0;

static SANE_Range rangeXmm;
static SANE_Range rangeYmm;

static SANE_Status
SetupScanMode (Brother_Scanner *this, int scanMode)
{
   // 
   if (scanMode == COLOR_FUL || scanMode == COLOR_FUL_NOCM)
   {
     this->aoptDesc[optContrast].cap |= SANE_CAP_INACTIVE;
     this->aoptDesc[optBrightness].cap |= SANE_CAP_INACTIVE;
   }
   else
   {
     if (scanMode == COLOR_BW)
     {
       this->aoptDesc[optContrast].cap |= SANE_CAP_INACTIVE;
     }
     else
     {
      this->aoptDesc[optContrast].cap &= ~SANE_CAP_INACTIVE;
     }
     this->aoptDesc[optBrightness].cap &=~SANE_CAP_INACTIVE;
   }
   return SANE_STATUS_GOOD;
}

static
SANE_Status
InitOptions (Brother_Scanner *this)
{
  SANE_Status rc;
  TOptionIndex iOpt;
  SANE_Option_Descriptor *pdesc;
  TOptionValue           *pval;
  int listcnt;
  int nSize;

  static char *achNamesXY[]= {
	SANE_NAME_SCAN_TL_X,	SANE_NAME_SCAN_TL_Y,
	SANE_NAME_SCAN_BR_X,	SANE_NAME_SCAN_BR_Y };
  static char *achTitlesXY[]= {
	SANE_TITLE_SCAN_TL_X,	SANE_TITLE_SCAN_TL_Y,
	SANE_TITLE_SCAN_BR_X,	SANE_TITLE_SCAN_BR_Y };
  static char *achDescXY[]= {
	SANE_DESC_SCAN_TL_X,	SANE_DESC_SCAN_TL_Y,
	SANE_DESC_SCAN_BR_X,	SANE_DESC_SCAN_BR_Y };

#if 0  //suport A3   M-LNX-98
  static SANE_Word rangXY_DEF[4]= { SANE_FIX(0.0),
                                    SANE_FIX(0.0),
                                    SANE_FIX(215.9),
                                    SANE_FIX(355.6) };
#else  //suport A3    M-LNX-98
  static SANE_Word rangXY_DEF[4]= { SANE_FIX(0.0),
                                    SANE_FIX(0.0),
                                    SANE_FIX(297.0),
                                    SANE_FIX(420.0) };
#endif  //suport A3   M-LNX-98

  static const SANE_Range *aRangesXY[] = { &rangeXmm,&rangeYmm,&rangeXmm,&rangeYmm };

  memset(this->aoptDesc,0,sizeof(this->aoptDesc));
  memset(this->aoptVal,0,sizeof(this->aoptVal));

  // 
  nSize = NUM_SCANMODE * sizeof (scanModeList[0]); 
  scanModeList = MALLOC (nSize);
  if (!scanModeList)
    return SANE_STATUS_NO_MEM;
  rc = get_scanmode_string(this->uiSetting.ScanModeList, scanModeList);
  if (!rc)
      return SANE_STATUS_INVAL;
  // 
  nSize = NUM_RESO * sizeof (scanResoList);
  scanResoList = MALLOC (nSize);
  if (!scanResoList)
    return SANE_STATUS_NO_MEM;
  rc = get_reso_int(this->uiSetting.ResoList, scanResoList);
  if (!rc)
      return SANE_STATUS_INVAL;
  // 
  nSize = NUM_SCANSRC * sizeof (scanSrcList[0]); 
  scanSrcList = MALLOC (nSize);
  if (!scanSrcList)
    return SANE_STATUS_NO_MEM;
  rc = get_scansrc_string(this->uiSetting.ScanSrcList, scanSrcList);
  if (!rc)
      return SANE_STATUS_INVAL;

  // 
  rangeXmm.min = SANE_FIX(0.0);
  rangeXmm.max = SANE_FIX(this->modelConfig.SupportScanAreaWidth);
  rangeXmm.quant = SANE_FIX(0.1);

  //
  if (rangXY_DEF[optBRX-optTLX] > rangeXmm.max)
    rangXY_DEF[optBRX-optTLX] = rangeXmm.max;

  //
  rangeYmm.min = SANE_FIX(0.0);
  rangeYmm.max = SANE_FIX(this->modelConfig.SupportScanAreaHeight);
  rangeYmm.quant = SANE_FIX(0.1);

  // 
  if (rangXY_DEF[optBRY-optTLX] > rangeYmm.max)
    rangXY_DEF[optBRY-optTLX] = rangeYmm.max;

  for (iOpt=optCount; iOpt!=optLast; iOpt++)
    {
      /* shorthands */
      pdesc=this->aoptDesc+iOpt;
      pval=this->aoptVal+iOpt;
      /* default */
      pdesc->size=sizeof(SANE_Word);
      pdesc->cap=SANE_CAP_SOFT_SELECT | SANE_CAP_SOFT_DETECT;

      /*
	Some hints:
	*every* field needs a constraint, elseway there will be a warning.
	*/
    
      switch (iOpt)
	{
	case optCount:
	  pdesc->title  =SANE_TITLE_NUM_OPTIONS;
	  pdesc->desc   =SANE_DESC_NUM_OPTIONS;
	  pdesc->cap    =SANE_CAP_SOFT_DETECT;
	  pval->w       =(SANE_Word)optLast;
	  break;
	case optGroupMode:
	  pdesc->title="Mode";
	  pdesc->desc ="";
	  pdesc->type = SANE_TYPE_GROUP;
	  pdesc->cap  = SANE_CAP_ADVANCED;
	  break;
	case optMode:
	  pdesc->name   =SANE_NAME_SCAN_MODE;
	  pdesc->title  =SANE_TITLE_SCAN_MODE;
	  pdesc->desc   ="Select the scan mode";
	  pdesc->type   =SANE_TYPE_STRING;
	  pdesc->size   =30;
	  pdesc->constraint_type = SANE_CONSTRAINT_STRING_LIST;
	  pdesc->constraint.string_list = scanModeList;
	  if (this->modelConfig.SupportScanMode.bit.b24BitColor)
            listcnt = get_scanmode_listcnt(scanModeList, COLOR_FUL);
          else
            listcnt = get_scanmode_listcnt(scanModeList, COLOR_BW);
	  pval->s       = strdup(scanModeList[listcnt]);
	  break;
	case optScanSrc:
	  pdesc->name   =SANE_NAME_SCAN_SOURCE;
	  pdesc->title  =SANE_TITLE_SCAN_SOURCE;
	  pdesc->desc   =SANE_DESC_SCAN_SOURCE;
	  pdesc->type   =SANE_TYPE_STRING;
	  pdesc->size   =MAX_STRING;
	  pdesc->constraint_type = SANE_CONSTRAINT_STRING_LIST;
	  pdesc->constraint.string_list = scanSrcList;
	  if (this->modelConfig.SupportScanSrc.bit.ADF)
	    listcnt = get_scansrc_listcnt(scanSrcList, SCANSRC_ADF);
	  else
	    listcnt = get_scansrc_listcnt(scanSrcList, SCANSRC_FB);
	  pval->s       = strdup(scanSrcList[listcnt]);
	  break;
	case optResolution:
	  pdesc->name   =SANE_NAME_SCAN_RESOLUTION;
	  pdesc->title  =SANE_TITLE_SCAN_RESOLUTION;
	  pdesc->desc   =SANE_DESC_SCAN_RESOLUTION;
	  pdesc->type   =SANE_TYPE_INT;
	  pdesc->unit   =SANE_UNIT_DPI;
	  pdesc->constraint_type = SANE_CONSTRAINT_WORD_LIST;
	  pdesc->constraint.word_list = scanResoList;
	  listcnt = get_reso_listcnt(scanResoList, DEF_RESOTYPE);
	  pval->w       = scanResoList[listcnt];
	  break;
	case optBrightness:
	  pdesc->name   =SANE_NAME_BRIGHTNESS;
	  pdesc->title  =SANE_TITLE_BRIGHTNESS;
	  pdesc->desc   =SANE_DESC_BRIGHTNESS;
	  pdesc->type   =SANE_TYPE_FIXED;
	  pdesc->unit   =SANE_UNIT_PERCENT;
	  pdesc->constraint_type =SANE_CONSTRAINT_RANGE;
	  pdesc->constraint.range=&rangeLumi;
	  pval->w       =SANE_FIX(0);
	  break;
	case optContrast:
	  pdesc->name   =SANE_NAME_CONTRAST;
	  pdesc->title  =SANE_TITLE_CONTRAST;
	  pdesc->desc   =SANE_DESC_CONTRAST;
	  pdesc->type   =SANE_TYPE_FIXED;
	  pdesc->unit   =SANE_UNIT_PERCENT;
	  pdesc->constraint_type =SANE_CONSTRAINT_RANGE;
	  pdesc->constraint.range=&rangeLumi;
	  pval->w       =SANE_FIX(0);
	  break;
	case optGroupGeometry:
	  pdesc->title="Geometry";
	  pdesc->desc ="";
	  pdesc->type = SANE_TYPE_GROUP;
	  pdesc->constraint_type=SANE_CONSTRAINT_NONE;
	  pdesc->cap  = SANE_CAP_ADVANCED;
	  break;
	case optTLX: case optTLY: case optBRX: case optBRY:
	  pdesc->name   =achNamesXY[iOpt-optTLX];
	  pdesc->title  =achTitlesXY[iOpt-optTLX];
	  pdesc->desc   =achDescXY[iOpt-optTLX];
	  pdesc->type   =SANE_TYPE_FIXED;
	  pdesc->unit   =SANE_UNIT_MM; /* arghh */
	  pdesc->constraint_type =SANE_CONSTRAINT_RANGE;
	  pdesc->constraint.range=aRangesXY[iOpt-optTLX];
	  pval->w       =rangXY_DEF[iOpt-optTLX];
	  break;
	case optLast: /* not reached */
	  break;
	}
    }

 if (this->modelConfig.SupportScanMode.bit.b24BitColor)
   SetupScanMode(this, COLOR_FUL);
 else
   SetupScanMode(this, COLOR_BW);

  return SANE_STATUS_GOOD;
}



static SANE_Status
RegisterSaneDev (struct usb_device *pdevUSB, 
		 char *szName, PMODELINF pModelInf, 
		 int index){
  TDevice * q;
  errno = 0;
  q = MALLOC (sizeof (*q));
  if (!q)
    return SANE_STATUS_NO_MEM;
  memset (q, 0, sizeof (*q)); /* clear every field */
  q->sane.name   = strdup (szName);
  if(pModelInf->vendorID == SCANNER_VENDOR){
    q->sane.vendor = "Brother";
  }
  else{
    q->sane.vendor = "";
  }
  if(index == -1){
    //q->sane.type   = q->modelInf.modelTypeName = strdup (pModelInf->modelTypeName);
    q->sane.type   = q->modelInf.modelTypeName = strdup ("USB scanner");
  }
  else{
    char name[260];
    get_net_ini_value(index ,KEY_DEVICE, name, sizeof(name));
    if(*name){
      q->sane.type   = q->modelInf.modelTypeName = strdup (name);
    }
    else{
      q->sane.type   = q->modelInf.modelTypeName = strdup (pModelInf->modelTypeName);
    }
  } 
  q->sane.model  = q->modelInf.modelName = strdup (pModelInf->modelName); 
  q->modelInf.expcaps  = pModelInf->expcaps;     //M-LNX-20
  q->modelInf.vendorID = pModelInf->vendorID;
  q->modelInf.index = pModelInf->index;
  q->modelInf.productID = pModelInf->productID;
  q->modelInf.seriesNo = pModelInf->seriesNo;
  q->pdev=pdevUSB;
  q->index = index;
  ++num_devices;
  q->pNext = pdevFirst; /* link backwards */
  pdevFirst = q;

  return SANE_STATUS_GOOD;
}


SANE_Status
sane_init (SANE_Int *version_code, SANE_Auth_Callback authCB)
{
  struct usb_bus    *pbus;
  struct usb_device *pdev;
  int                iBus,rc;
  MODELINF          modelInfList;

  int i,nnetdev;

  WriteLog( "<<< sane_init start >>> " );
#if       BRSANESUFFIX == 2 || BRSANESUFFIX == 3      //M-LNX-109
  DBG_INIT();
#elif  BRSANESUFFIX == 1
#else
Not support (force causing compile error)
#endif    //BRSANESUFFIX

  authCB++; /* compiler */

  DBG(DEBUG_VERBOSE,"brother init\n");
  if (version_code)
   {
    *version_code = SANE_VERSION_CODE (V_MAJOR, V_MINOR, BUILD);
    DBG(DEBUG_VERBOSE,"brother version: %lx\n",
    	SANE_VERSION_CODE(V_MAJOR, V_MINOR, BUILD));
   }

  pdevFirst=NULL;

  usb_init();
  usb_find_busses();
  usb_find_devices();

  rc=init_model_info();
  if (!rc)
    return SANE_STATUS_IO_ERROR;
    
  rc=get_model_info(&modelInfList);
  if (!rc)
    return SANE_STATUS_IO_ERROR;

    nnetdev=get_net_device_num();
    if (!usb_busses && nnetdev==0){
      return SANE_STATUS_IO_ERROR;
    }

  iBus=0;
  DBG(DEBUG_INFO,"starting bus scan\n");
  for (pbus = usb_busses; pbus; pbus = pbus->next)
  {
      int iDev=0;
      iBus++;
      /* 0.1.3b no longer has a "busnum" member */
      DBG(DEBUG_JUNK,"scanning bus %s\n", pbus->dirname);
      for (pdev=pbus->devices; pdev; pdev  = pdev->next)
	{
	  PMODELINF       pModelInf;

	  iDev++;
	  DBG(DEBUG_JUNK,"found dev %04X/%04X\n",
		  pdev->descriptor.idVendor,
		  pdev->descriptor.idProduct);
	  /* the loop is not SO effective, but straight! */
	
	  for (pModelInf=&modelInfList; pModelInf; pModelInf = pModelInf->next)
	  {
	      if (pdev->descriptor.idVendor  ==  pModelInf->vendorID &&
		  pdev->descriptor.idProduct == pModelInf->productID)
		{
		  char ach[100];
		  sprintf(ach,"bus%d;dev%d",iBus,iDev);
		  RegisterSaneDev(pdev,ach,pModelInf,-1);
		  break;
		}
	  }
	}
  }

    WriteLog( "<<< sane_init Check Interface >>> " );
    for(i = 0 ; i < nnetdev ; i ++){
	  PMODELINF       pModelInf;
	  char ach[100];
          int inf_id_vendor,inf_id_Product;
	  pModelInf = &modelInfList;
	  get_device_id(i,&inf_id_vendor,&inf_id_Product);
	  for (pModelInf=&modelInfList; pModelInf; pModelInf = pModelInf->next){
	      if (inf_id_vendor  ==  pModelInf->vendorID &&
		  inf_id_Product == pModelInf->productID){
		sprintf(ach,"net1;dev%d",i);
		WriteLog( "<<< sane_init RegisterSaneDev (%s) >>> ",ach );
		RegisterSaneDev(NULL,ach,pModelInf,i);
                break;
	      }
	  }
    }

  rc=exit_model_info();
  if (!rc)
    return SANE_STATUS_IO_ERROR;

  WriteLog( "<<< sane_init end >>> " );
  return SANE_STATUS_GOOD;
}

static const SANE_Device ** devlist = 0; /* only pseudo-statical */

void
sane_exit (void)
{
  TDevice   *dev, *pNext;

  WriteLog( "<<< sane_exit start >>> " );

  /* free all bound resources and instances */
  while (pinstFirst)
    sane_close((SANE_Handle)pinstFirst); /* free all resources */

  /* free all device descriptors */
  for (dev = pdevFirst; dev; dev = pNext)
    {
      pNext = dev->pNext;
      // strdup
      free ((void *) dev->sane.name);
      free ((void *) dev->sane.model);
      free ((void *) dev->sane.type);
      FREE (dev);
    }
  if (devlist) FREE(devlist);
  devlist=NULL;
  free_inifile_tree();
  free_net_inifile_tree();

  WriteLog( "<<< sane_exit end >>> " );
}

SANE_Status
sane_get_devices (const SANE_Device *** device_list,
		  SANE_Bool local_only)
{
  TDevice *dev;
  int i;

  WriteLog( "<<< sane_get_devices start >>> " );

  local_only = TRUE; /* Avoid compile warning */

  if (devlist) FREE (devlist);

  devlist = MALLOC ((num_devices + 1) * sizeof (devlist[0]));
  if (!devlist)
    return SANE_STATUS_NO_MEM;

  i = 0;
  for (dev = pdevFirst; i < num_devices; dev = dev->pNext)
    devlist[i++] = &dev->sane;
  devlist[i++] = 0;

  *device_list = devlist;

  WriteLog( "<<< sane_get_devices end >>> " );
  return SANE_STATUS_GOOD;
}

SANE_Status
sane_open (SANE_String_Const devicename, SANE_Handle *handle)
{
  TDevice    *pdev;
  Brother_Scanner  *this;
  SANE_Status rc;
  int errornum;

  WriteLog( "<<< sane_open start dev_name=%s>>> ", devicename );
  if (devicename[0]) /* selected */
    {
      for (pdev=pdevFirst; pdev; pdev=pdev->pNext)
	if (!strcmp(devicename,pdev->sane.name))
	  break;
      /* no dynamic post-registration */
    }
  else
    pdev=pdevFirst;

  if (!pdev)
      return SANE_STATUS_INVAL;
  this = (Brother_Scanner*) MALLOC(sizeof(Brother_Scanner));
  if (!this) return SANE_STATUS_NO_MEM;
  memset(this, 0, sizeof(Brother_Scanner));

  *handle = (SANE_Handle)this;

  this->pNext=pinstFirst; /* register open handle */
  pinstFirst=this;
  /* open and prepare USB scanner handle */

 {
                  this->hScanner = calloc(sizeof(dev_handle),1);
		  if(strncmp(devicename,"net1;dev",strlen("net1;dev")) == 0){
		    this->hScanner->device = IFTYPE_NET;
		  }
		  else{
		    this->hScanner->device = IFTYPE_USB;
		  }
		  this ->hScanner->usb_w_ep 
		    = get_p_model_info_by_index(pdev->modelInf.index)->w_endpoint;
		  this ->hScanner->usb_r_ep
		    = get_p_model_info_by_index(pdev->modelInf.index)->r_endpoint;

		  if (IFTYPE_USB == this->hScanner->device){
		    this->hScanner->net_device_index = -1;
		    this->hScanner->usb = usb_open(pdev->pdev);
#ifndef DEBUG_No39
		    g_pdev = pdev;
#endif
		    if (!this->hScanner->usb)
		      return SANE_STATUS_IO_ERROR;

		    //2005/11/10 not check returned value from usb_set_configuration()
		    //if (usb_set_configuration(this->hScanner, 1))
		    //   return SANE_STATUS_IO_ERROR;		    
		    //(M-LNX-24 2006/04/11 kado for Fedora5 USB2.0) 
		    //errornum = usb_set_configuration(this->hScanner->usb, 1);
		    errornum = usb_set_configuration_or_reset_toggle(this, 1);

		    if (usb_claim_interface(this->hScanner->usb, 1))
		      return SANE_STATUS_IO_ERROR;
		  }
		  else{
		    sscanf(devicename,"net1;dev%d",&this->hScanner->net_device_index);
		  }
 }



  // 
  rc= OpenDevice(this->hScanner, pdev->modelInf.seriesNo);
  if (!rc)
      return SANE_STATUS_INVAL;

  // 
  this->scanState.bEOF = FALSE;
  this->scanState.bCanceled = FALSE;
  this->scanState.bScanning = FALSE;
  this->scanState.nPageCnt = 0;

  // 
  this->modelInf.productID = pdev->modelInf.productID;
  this->modelInf.expcaps = pdev->modelInf.expcaps;     //M-LNX-20
  this->modelInf.vendorID = pdev->modelInf.vendorID;
  this->modelInf.index = pdev->modelInf.index;

  this->modelInf.seriesNo = pdev->modelInf.seriesNo;
  this->modelInf.modelName = pdev->modelInf.modelName;
  this->modelInf.modelTypeName = pdev->modelInf.modelTypeName;

#if 1  // suport A3      M-LNX-98
  this->mfcModelInfo.bQcmdEnable = TRUE;
  if (!QueryDeviceInfo(this)) //
      return SANE_STATUS_INVAL;
#endif

  get_model_config(&this->modelInf, &this->modelConfig);

  GetLogSwitch( this );

  // 
  this->uiSetting.ResoList.val = this->modelConfig.SupportReso.val;
  this->uiSetting.ScanModeList.val = this->modelConfig.SupportScanMode.val;
  this->uiSetting.ScanSrcList.val = this->modelConfig.SupportScanSrc.val;

  this->mfcModelInfo.wModelType  = 16;
  this->mfcModelInfo.wDialogType = TWDSUI_NOVC;
  this->mfcModelInfo.bColorModel = TRUE;
  this->mfcModelInfo.bDither     = FALSE;
  this->mfcModelInfo.b3in1Type   = FALSE;
  this->mfcModelInfo.bVideoCap   = FALSE;
  this->mfcModelInfo.bQcmdEnable = TRUE;

  GetDeviceAccessParam( this );

  if (!QueryDeviceInfo(this)) // 
      return SANE_STATUS_INVAL;

#ifndef DEBUG_No39
   if (IFTYPE_USB == this->hScanner->device){       //check i/f
     if(this->hScanner->usb){
       CloseDevice(this->hScanner);
       usb_release_interface(this->hScanner->usb, 1); //   USB
       usb_close(this->hScanner->usb);                //   USB
       this->hScanner->usb = NULL;
     }
   }
   else{
     if(this->hScanner->net){
       CloseDevice(this->hScanner);
       this->hScanner->net = NULL;
     }
   }
#endif
   ///
   /// ColorMatch DLL
   ///
   this->modelInf.index = pdev->modelInf.index;     // cp index
   //M-LNX-111  delete color matching function
  
  //
  // Scan Decode DLL
  //
  rc = LoadScanDecDll( this );
  if ( !rc )  // Scan Decode DLL
      return SANE_STATUS_INVAL;

  // GrayTable
  LoadGrayTable( this, GRAY_TABLE_NO );

  rc = InitOptions(this);
  WriteLog( "<<< sane_open end >>> " );
  return rc;
}

void
sane_close (SANE_Handle handle)
{
  Brother_Scanner *this,*pParent,*p;
  this=(Brother_Scanner*)handle;

  WriteLog( "<<< sane_close start >>> " );
  if (this->hScanner)
    {
      if (this->scanState.bScanning)
        ScanEnd( this );

      FreeGrayTable( this );
      //M-LNX-111  delete color matching function
      FreeScanDecDll( this );

#ifdef DEBUG_No39
      if (IFTYPE_USB == this->hScanner->device){
	if(this->hScanner->usb){
	  CloseDevice(this->hScanner);
	  usb_release_interface(this->hScanner->usb, 1);
	  usb_close(this->hScanner->usb);
	}
      }
      else{
	if(this->hScanner->net){
	  CloseDevice(this->hScanner);
	  this->hScanner->net = NULL;
	}
      }
      free(this->hScanner);
      this->hScanner = NULL;

#endif
    }

  if (scanModeList)
    FREE(scanModeList);

  if (scanResoList)
    FREE(scanResoList);

  if (scanSrcList)
    FREE(scanSrcList);

  /* unlink active device entry */
  pParent=NULL;
  for (p=pinstFirst; p; p=p->pNext)
    {
      if (p==this) break;
      pParent=p;
    }
  
  if (!p)
    {
      DBG(1,"invalid handle in close()\n");
      return;
    }
  /* delete instance from instance list */
  if (pParent)
    pParent->pNext=this->pNext;
  else
    pinstFirst=this->pNext; /* NULL with last entry */
  FREE(this);

  WriteLog( "<<< sane_close end >>> " );
}

const SANE_Option_Descriptor *
sane_get_option_descriptor (SANE_Handle handle, SANE_Int iOpt)
{
  Brother_Scanner *this=(Brother_Scanner*)handle;
  WriteLog( "<<< sane_get_option_descriptor start >>> " );

  if (iOpt<NUM_OPTIONS)
    return this->aoptDesc+iOpt;

  WriteLog( "<<< sane_get_option_descriptor end >>> " );
  return NULL;
}

SANE_Status
sane_control_option (SANE_Handle handle, SANE_Int iOpt,
		     SANE_Action action, void *pVal, 
		     SANE_Int *pnInfo)
{
  SANE_Word   cap;
  SANE_Status rc;
  Brother_Scanner *this;
  int id;

  WriteLog( "<<< sane_control_option start >>> " );

  this=(Brother_Scanner*)handle;
  rc=SANE_STATUS_GOOD;
  if (pnInfo)
    *pnInfo=0;

  if (iOpt>=NUM_OPTIONS)
    return SANE_STATUS_INVAL;

  cap=this->aoptDesc[iOpt].cap;
  
  switch (action)
    {

      /* ------------------------------------------------------------ */

    case SANE_ACTION_GET_VALUE:
      switch ((TOptionIndex)iOpt)
	{
	case optCount:
	case optResolution:
	case optBrightness:
	case optContrast:
	case optTLX: case optTLY: case optBRX: case optBRY:
	  *(SANE_Word*)pVal = this->aoptVal[iOpt].w;
	  break;
	case optMode:
	case optScanSrc:
	  strcpy(pVal,this->aoptVal[iOpt].s);
	  break;
	default:
	  return SANE_STATUS_INVAL;
	}
      break;

      /* ------------------------------------------------------------ */

    case SANE_ACTION_SET_VALUE:
      if (!SANE_OPTION_IS_SETTABLE(cap))
	  return SANE_STATUS_INVAL;
      rc=sanei_constrain_value(this->aoptDesc+iOpt,pVal,pnInfo);
      if (rc!=SANE_STATUS_GOOD)
	  return rc;
      switch ((TOptionIndex)iOpt)
	{
	case optResolution:
	case optTLX: case optTLY: case optBRX: case optBRY:
	  if (pnInfo) (*pnInfo) |= SANE_INFO_RELOAD_PARAMS;
	case optBrightness:
	case optContrast:
	  this->aoptVal[iOpt].w = *(SANE_Word*)pVal;
	  break;
	case optMode:
	  if (pnInfo)
	    (*pnInfo) |= SANE_INFO_RELOAD_PARAMS | SANE_INFO_RELOAD_OPTIONS;

          // 
	  id = get_scanmode_id(pVal);
	  if (id == -1)
	    return SANE_STATUS_INVAL;

          if ((rc = SetupScanMode( this, id )) != SANE_STATUS_GOOD)
            return rc;

	case optScanSrc:

        if (this->aoptVal[iOpt].s)
          free (this->aoptVal[iOpt].s); // strdup
        this->aoptVal[iOpt].s = strdup (pVal);

	  break;
	default:
	  return SANE_STATUS_INVAL;
	}
      break;
    case SANE_ACTION_SET_AUTO:
      return SANE_STATUS_UNSUPPORTED;
    } /* switch action */

  WriteLog( "<<< sane_control_option end >>> " );
  return rc; /* normally GOOD */
}


static SANE_Status
SetupInternalParameters(Brother_Scanner *this)
{
  int nWidthMm, nHeightMm;
 
  this->uiSetting.wColorType = get_scanmode_id(this->aoptVal[optMode].s);
  this->uiSetting.wResoType = get_reso_id(this->aoptVal[optResolution].w);
  this->uiSetting.nBrightness=(int)(this->aoptVal[optBrightness].w>>SANE_FIXED_SCALE_SHIFT);
  this->uiSetting.nContrast=(int)(this->aoptVal[optContrast].w>>SANE_FIXED_SCALE_SHIFT);
  this->uiSetting.nSrcType = get_scansrc_id(this->aoptVal[optScanSrc].s);  //06/02/27 Duplex Scan


  // M-LNX-103
  this->uiSetting.nSrcAlign = get_scansrc_align(this->aoptVal[optScanSrc].s);


  //
  if (this->aoptVal[optTLX].w == this->aoptVal[optBRX].w )
    return SANE_STATUS_INVAL;
    
  if (this->aoptVal[optTLX].w < this->aoptVal[optBRX].w )
  {
    this->uiSetting.ScanAreaMm.left = (int)(SANE_UNFIX(this->aoptVal[optTLX].w) * 10);
    this->uiSetting.ScanAreaMm.right = (int)(SANE_UNFIX(this->aoptVal[optBRX].w) * 10);
  }
  else
  {
    this->uiSetting.ScanAreaMm.left = (int)(SANE_UNFIX(this->aoptVal[optBRX].w) * 10);
    this->uiSetting.ScanAreaMm.right = (int)(SANE_UNFIX(this->aoptVal[optTLX].w) * 10);
  }
  
  // 
  if (this->aoptVal[optTLY].w == this->aoptVal[optBRY].w )
    return SANE_STATUS_INVAL;
    
  if (this->aoptVal[optTLY].w < this->aoptVal[optBRY].w )
  {
    this->uiSetting.ScanAreaMm.top = (int)(SANE_UNFIX(this->aoptVal[optTLY].w) * 10);
    this->uiSetting.ScanAreaMm.bottom = (int)(SANE_UNFIX(this->aoptVal[optBRY].w) * 10);
  }
  else
  {
    this->uiSetting.ScanAreaMm.top = (int)(SANE_UNFIX(this->aoptVal[optBRY].w) * 10);
    this->uiSetting.ScanAreaMm.bottom = (int)(SANE_UNFIX(this->aoptVal[optTLY].w) * 10);
  }

  nWidthMm = this->uiSetting.ScanAreaMm.right - this->uiSetting.ScanAreaMm.left + 1;
  nHeightMm = this->uiSetting.ScanAreaMm.bottom - this->uiSetting.ScanAreaMm.top + 1;

  // 
  if (nWidthMm <= 80 || nHeightMm <= 80)
    return SANE_STATUS_INVAL;

  return SANE_STATUS_GOOD;
}


SANE_Status
sane_get_parameters (SANE_Handle handle, SANE_Parameters *p)
{
  /* extremly important for xscanimage */
  Brother_Scanner *this;
  LPAREARECT  lpScanAreaDot;
  LONG    lUserResoX,   lUserResoY;
 
  WriteLog( "<<< sane_get_parameters start >>> " );

  this=(Brother_Scanner *)handle;
  SetupInternalParameters(this);

  if (this->scanState.bScanning) {
    p->pixels_per_line = this->scanInfo.ScanAreaSize.lWidth;
    p->lines = this->scanInfo.ScanAreaSize.lHeight;
  }
  else {
    CnvResoNoToUserResoValue( &this->uiSetting.UserSelect, this->uiSetting.wResoType );

    lUserResoX   = this->uiSetting.UserSelect.wResoX;
    lUserResoY   = this->uiSetting.UserSelect.wResoY;

    lpScanAreaDot = &this->uiSetting.ScanAreaDot;
    lpScanAreaDot->left   = this->uiSetting.ScanAreaMm.left   * lUserResoX / 254L;
    lpScanAreaDot->right  = this->uiSetting.ScanAreaMm.right  * lUserResoX / 254L;
    lpScanAreaDot->top    = this->uiSetting.ScanAreaMm.top    * lUserResoY / 254L;
    lpScanAreaDot->bottom = this->uiSetting.ScanAreaMm.bottom * lUserResoY / 254L;

    p->pixels_per_line = lpScanAreaDot->right  - lpScanAreaDot->left;
    p->lines = lpScanAreaDot->bottom - lpScanAreaDot->top;
  }
  p->last_frame=SANE_TRUE;

  switch (this->uiSetting.wColorType)
    {
    case COLOR_FUL:
    case COLOR_FUL_NOCM:
      p->format=SANE_FRAME_RGB;
      p->depth=8;
      p->bytes_per_line=p->pixels_per_line*3;
      break;
    case COLOR_TG:
      p->format=SANE_FRAME_GRAY;
      p->depth=8;
      p->bytes_per_line=p->pixels_per_line;
      break;
    case COLOR_BW:
    case COLOR_ED:
      p->format=SANE_FRAME_GRAY;
      p->depth=1;
      p->bytes_per_line=(p->pixels_per_line+7)/8;
      break;
    }      
  WriteLog( "<<< sane_get_parameters end (bytes_per_line=%d, lines=%d) >>> ",p->bytes_per_line, p->lines);

  return SANE_STATUS_GOOD;
}

SANE_Status
sane_start (SANE_Handle handle)
{
  SANE_Status   rc;
  Brother_Scanner    *this;
  this=(Brother_Scanner*)handle;
  WriteLog( "<<< sane_start start >>> " );

  rc=SetupInternalParameters(this);
  if (rc) // 
	return rc;

  rc = ScanStart(this);
  if (rc) return rc;

  WriteLog( "<<< sane_start End >>> " );
  return rc;
}

SANE_Status
sane_read (SANE_Handle handle, SANE_Byte *buf,
	   SANE_Int maxlen,
	   SANE_Int *len)
{
  SANE_Status    rc;
  Brother_Scanner     *this;
  this=(Brother_Scanner*)handle;

  WriteLog( "<<< sane_read start >>> maxlen = %d", maxlen );

  *len=0;

  if (!this->scanState.bEOF) {
    rc=PageScan(this,
		(char *)buf,  //M-LNX-104
		maxlen,len);
    if(rc == SANE_STATUS_DUPLEX_ADVERSE  && *len == 1) //06/02/27 if 0x84 is only returned, retry PageScan.
      //while(rc == SANE_STATUS_DUPLEX_ADVERSE  && *len == 1)
      rc =PageScan(this,
		   (char *)buf,  //M-LNX-104
		   maxlen,len);
  }
  else {
    rc = SANE_STATUS_EOF;
  }

  WriteLog( "<<< sane_read End >>> rc = %d, len = %d", rc, *len);
  return rc;
}

void
sane_cancel (SANE_Handle handle)
{
  Brother_Scanner     *this;
  this=(Brother_Scanner*)handle;


  WriteLog( "sane_cancel start");
  DBG(DEBUG_VERBOSE,"cancel called...\n");

  if (this->scanState.bScanning) {
    AbortPageScan( this );
    this->scanState.bScanning=FALSE;

  }
  ScanEnd( this );

  WriteLog( "sane_cancel end");

}

SANE_Status
sane_set_io_mode(SANE_Handle h, SANE_Bool m)
{
  h++;
  if (m==SANE_TRUE) /* no non-blocking-mode */
    return SANE_STATUS_UNSUPPORTED;
  return SANE_STATUS_GOOD;
}

SANE_Status
sane_get_select_fd(SANE_Handle handle, SANE_Int *fd)
{
  handle++; fd++;
  return SANE_STATUS_UNSUPPORTED; /* we have no file IO */
}

