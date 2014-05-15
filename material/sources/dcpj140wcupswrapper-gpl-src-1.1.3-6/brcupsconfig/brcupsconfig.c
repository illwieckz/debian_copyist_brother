//* Brother CUPS wrapper tool
//* Copyright (C) 2005-2012 Brother. Industries, Ltd.//*
//*                                    Ver1.00

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


#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "brcups_commands.h"

//#include "brcups_setting.h"


CMDLINELIST standard_side_commandlinelist[];

int divide_media_token(char *input,char output[5][30]);


#if 0
#define DEBUGPRINT(a)                    fprintf(stderr,a);fflush(stdout)
#define DEBUGPRINT1(a1,a2)               fprintf(stderr,a1,a2);fflush(stdout)
#define DEBUGPRINT2(a1,a2,a3)            fprintf(stderr,a1,a2,a3);fflush(stdout)
#define DEBUGPRINT3(a1,a2,a3,a4)         fprintf(stderr,a1,a2,a3,a4);fflush(stdout)
#define DEBUGPRINT4(a1,a2,a3,a4,a5)      fprintf(stderr,a1,a2,a3,a4,a5);fflush(stdout)
#define DEBUGPRINT5(a1,a2,a3,a4,a5,a6)   fprintf(stderr,a1,a2,a3,a4,a5,a6);fflush(stdout)
#else
#define DEBUGPRINT(a)              
#define DEBUGPRINT1(a1,a2)         
#define DEBUGPRINT2(a1,a2,a3)      
#define DEBUGPRINT3(a1,a2,a3,a4)   
#define DEBUGPRINT4(a1,a2,a3,a4,a5)
#define DEBUGPRINT5(a1,a2,a3,a4,a5,a6)
#endif



int     log_level = 0;


typedef struct {
  char value[50];
  char option[50];
}  SETCOMMAND;


SETCOMMAND command_array[100];


//
//   local functions 
//
int initialize_command_list();
int add_command_list(char *option,char  *command);
int add_command_list_brcommand(char  *command);
int exec_brprintconf(char *brprintconf,char *printer);
char *strstr_ex(char *data , char *serch_data);
char *delete_ppd_comment(char *line);
char *chk_ppd_default_setting_line(char *ppd_line);
char *get_token(char *input,char *output);

void write_log_file(int level,char *format,...);
int main(int argc,char * argv[]) {

  char *printer;
  char ppd_line[500],tmp[500],*p_tmp,tmp_n[10],tmp_op[500];
  FILE *fp_ppd;
  char *p;
  char *commandline,*ppdfile, *printer_model;
  int i,ii;
  DEBUGPRINT("main:start\n");

  //debug code
//  fprintf (stderr, "ERROR: brcupsconfpt1 pid=%d\n", getpid());
//  sleep(100);

  if(argc < 1){
    return 0;
  }
  printer = argv[1];
  if(argc > 2){
    ppdfile= argv[2];
  }
  else{
    ppdfile="";
  }
	if(argc > 3){
    if(argv[3][0] >= '0' && argv[3][0] <= '9'){

      log_level = argv[3][0] -'0';
    }
    else{
      log_level = 0;
    }
  }
  else{
    log_level = 0;
  }

  if(argc > 4){
    commandline = argv[4];
  }
  else{
    commandline = "NULL COMMAND LINE";
  }

	printer_model = argv[5];

  fp_ppd = fopen(ppdfile , "r");
  if( fp_ppd == NULL) return 0;


  initialize_command_list();




  //************************************
  //  set default setting
  //************************************

#if	0
  DEBUGPRINT("main:set default setting\n");
  write_log_file(5,"DEFAULT SETTING\n");  
  for ( i = 0; default_setting[i] != NULL; i ++){
    p = strstr_ex(default_setting[i],"BROTHERPRINTER_XXX");
    if(p){
      p = strchr(p,'-');
      if(p){
	add_command_list_brcommand(p);
      }
    }
  }
#endif	// 0

  //************************************
  //  set PPD option 
  //************************************


  DEBUGPRINT("main:set PPD option (string)\n");
  write_log_file(5,"PPD SETTING\n");

  while(fgets(ppd_line,sizeof(ppd_line),fp_ppd)){
    if(NULL == delete_ppd_comment(ppd_line))continue;
    if(NULL == chk_ppd_default_setting_line(ppd_line))continue;

    //************************************
    //  set PPD option (string)
    //************************************

	for ( i = 0; ppdcommand_all_list[i]!= NULL; i ++)
	{
      //DEBUGPRINT2(    "main: set PPD option (string)    [%s]  [%s]\n",
      //	      ppd_line,ppdcommand_all_list[i]->label);
		p = strstr_ex(ppd_line,ppdcommand_all_list[i]->label);
		if(p)
		{
	//DEBUGPRINT2(    "main: set PPD option (string)    [%s]  [%s]\n",
	//       ppdcommand_all_list[i]->label,ppd_line);
	
		#if 1		
			char * tmp;
			tmp = p + strlen( ppdcommand_all_list[i]->label);
        #endif
			for (ii = 0; ppdcommand_all_list[i]->ppdcommandlist[ii].value != NULL; ii++)
			{
	  //DEBUGPRINT3(    "main: set PPD option (string)    [%s]  [%s]  [%s]\n",
	  //       ppdcommand_all_list[i]->label,ppdcommand_all_list[i]->ppdcommandlist[ii].value ,  ppd_line);
	  		#if 0        // 
				p = strstr_ex(ppd_line,ppdcommand_all_list[i]->ppdcommandlist[ii].value);
			#else
				p = strstr_ex(tmp,ppdcommand_all_list[i]->ppdcommandlist[ii].value);
			#endif



				if(p)
				{
					add_command_list_brcommand(ppdcommand_all_list[i]->ppdcommandlist[ii].brcommand);
					break;
				}
			}




		}
	}



    //************************************
    //  set PPD option (numerical)
    //************************************
    for ( i = 0; PPDdefaultN[i].option!= NULL; i ++){
      strcpy(tmp,PPDdefaultN[i].option);
      p_tmp = tmp;
      if(tmp[0] == '^')p_tmp ++;
      p = strstr_ex(ppd_line,p_tmp);
      if(p){
	//DEBUGPRINT2(    "main: set PPD option (numerical)    [%s]  [%s]\n",
	//       ppd_line,p_tmp);

	sprintf(tmp,"%s  %s",PPDdefaultN[i].value,
	       p + strlen(PPDdefaultN[i].option));
        //DEBUGPRINT1("set commandline option(n)   **********    : [%s]\n",tmp);

	get_token(PPDdefaultN[i].value ,tmp_op);
	get_token(p + strlen(PPDdefaultN[i].option) ,tmp_n);

	add_command_list(tmp_op,tmp_n);

      }
    }

  }



  //************************************
  //  set brother command line option (string)
  //************************************
  DEBUGPRINT("main:set brother command line option (string)\n");
  write_log_file(5,"BROTHER COMMAND LINE SETTING(S)\n");  
  for ( i = 0; commandlinelist[i].value != NULL; i ++){
    //DEBUGPRINT2("main:set command line option (string)    Loop [%s] [%s]\n",
    //       commandline,commandlinelist[i].value );
   p = strstr_ex(commandline,commandlinelist[i].option);
   if(p){
      add_command_list_brcommand(commandlinelist[i].value);
    }
  }







  //************************************
  //  set cups standard command line option (duplex)
  //************************************
  DEBUGPRINT("main:set standard command line option (duplex)\n");
  write_log_file(5,"STANDARD COMMAND LINE SETTING(DUPLEX)\n");  
  for ( i = 0; standard_side_commandlinelist[i].value != NULL; i ++){
    //DEBUGPRINT2("main:set command line option (duplex)    Loop [%s] [%s]\n",
    //       commandline,standard_side_commandlinelist[i].option );
    p = strstr_ex(commandline,standard_side_commandlinelist[i].option);
    if(p){

      add_command_list_brcommand(standard_side_commandlinelist[i].value);
    }


  }



#if 0	/*Ink_2009_models*/
  //************************************
  //  set cups standard command line option (media )
  //************************************
  DEBUGPRINT("main:set standard command line option (media)\n");
  write_log_file(5,"STANDARD COMMAND LINE SETTING(MEDIA)\n");  
  {
    char output[5][30];
    int max;

    max = divide_media_token(commandline,output);
    //DEBUGPRINT2("main:set command line option (media)    Loop count %d   %s \n",
    //         max,commandline);
    for ( ii=0; ii < max; ii++){
//      for ( i = 0; standard_media_commandlinelist[i].value != NULL; i ++){
		for ( i = 0; standard_commandlinelist[i].value != NULL; i ++){
		
	//DEBUGPRINT2("main:set command line option (media)    Loop [%s] [%s]\n",
	//	    output[ii],standard_media_commandlinelist[i].option);
			p = strstr(standard_commandlinelist[i].option, output[ii]);
			if(p){
				add_command_list_brcommand(standard_commandlinelist[i].value);
			}
		}
    	}
  }
#endif
  
  //************************************

  //  set command line option (numerical)
  //************************************
  DEBUGPRINT("main:set command line option (numerical)\n");
  write_log_file(5,"COMMAND LINE SETTING(N)\n");  

  for(i = 0; commandlinelist2[i].option != NULL; i ++){
    p = strstr_ex(commandline,commandlinelist2[i].option);
    //DEBUGPRINT3("set commandline option(n) : [%s] [%s] [%s]\n"
    //	,commandline,commandlinelist2[i].option,p);
    if(p){
      //DEBUGPRINT3("set commandline option(n) 1  **********  : [%s] [%s] [%s]\n"
      //	  ,tmp_n,p + strlen(commandlinelist2[i].option),p);
      get_token(commandlinelist2[i].value   ,tmp_op);
      get_token(p + strlen(commandlinelist2[i].option) ,tmp_n);
      sprintf(tmp,"%s  %s",tmp_op,tmp_n );
      //DEBUGPRINT2("set commandline option(n) 2  **********    : [%s] [%s]\n"
      //	  ,tmp_op,tmp_n);

      add_command_list(tmp_op,tmp_n);

    }
  }

  //************************************
  //  call brprintconf
  //************************************

	exec_brprintconf(brprintconf,printer_model);

	return 0;
}





int initialize_command_list(){
  int i;
  char *p;

  //DEBUGPRINT("initialize_command_list:start\n");

	p = (char *)command_array;
	for ( i = 0; i < (int)sizeof(command_array) ; i ++){
		*p = 0;

		p ++;
	}
	return i;
}


int exec_brprintconf(char *brprintconf,char *printer){
  int i;
  char exec[300];

  DEBUGPRINT("exec_brprintconf:start\n");

  for ( i = 0; command_array[i].value[0] != 0; i ++ ){
    sprintf(exec,"%s_%s  %s  \"%s\"",brprintconf, printer,
	    command_array[i].option,
	    command_array[i].value);

    write_log_file(1,"%s\n",exec);

//	fprintf (stderr, "ERROR:%s\n", exec);
//    printf ("Hello\n");
    system(exec);

    //DEBUGPRINT1("exec: %s\n",exec);
  }

	return 0;
}



int add_command_list(char *option,char  *value){






   char *p;
  int i;


  //DEBUGPRINT2("   add_command_list:start [%s] [%s]\n",option,value);
  if(!option || !value || !option[0] || !value[0]){


    //DEBUGPRINT2("   add_command_list:start ERROR [%d] [%s]\n",option,value);
    return 0;
  }

  for ( i = 0; command_array[i].option[0] != 0; i ++ ){
    //DEBUGPRINT4("   add_command_list:[%s] [%s] [%s] %d\n"
    //	,command_array[i].value,option,value,i);
    p = strstr_ex(command_array[i].option , option);
    if(p){
      //DEBUGPRINT2("     add_command_list   chg element %s %s\n",option,value);  
      write_log_file(3,"        C  %s  %s\n",option,value);  
      strcpy(command_array[i].value, value);
      break;
    }
  }

  if(command_array[i].option[0] == 0){
    //DEBUGPRINT2("     add_command_list   add element %s %s\n",option,value);  
    strcpy( command_array[i].option ,option);
    strcpy( command_array[i].value  ,value);
    write_log_file(3,"        A  %s  %s\n",option,value);  
  }
  return 1;
}




int add_command_list_brcommand_sub(char  *command){
  char  option[100],*p1,*p2;
  char  value[100];
  //DEBUGPRINT1("   add_command_list_brcommand:start %s\n",command);

  strncpy(option,command,sizeof(option)-1);

  option[strlen(option)] = 0;	/*Ink_2009_models. */




  p1 = strchr(option, ' ');
  p2 = strchr(option, '\t');

  if(p1 > p2 && p2 != NULL)p1 = p2;

  if(p1 == NULL){
    return 0;
  }

  *p1 = 0;
  p1 ++;

  while(*p1 == ' ' || *p1 == '\t')    p1 ++;

  strcpy(value , p1);


  //DEBUGPRINT2("call add_command_list   %s %s",option,value);
  return add_command_list(option,value);

}




int add_command_list_brcommand(char  *command){
  char multi_brcommands[500];

  char *p,*start;
  strcpy(multi_brcommands,command);


  start = multi_brcommands;
  
  while((p = strchr(start+1,'-'))){
    if(p > multi_brcommands+1){
      if(*(p-1) == ' ' || *(p-1) == '\t'){
	*(p-1) = 0;
      }
    }
    add_command_list_brcommand_sub(start);
    start = p;
  }

  add_command_list_brcommand_sub(start);

  return 0;
}


char *get_next_element(char *data,char *search_word){
  char *p;
  p = strstr(data,search_word);
  if(!p)return p;
  p += strlen(search_word);

  while(*p == ' '|| *p == '\t' || *p == ';' || *p == ':')p++;

  return p;

}




char *strstr_ex(char *data , char *search_data){
  char *p , *pp;
  p = strstr(data,search_data);
  if(!p)return p;

  pp = p;
  p += strlen(search_data);

  if(p > search_data){
      if ( ! ((*(p-1) == ' '|| *(p-1) == '\t' || *(p-1) == ';' || *(p-1) == ':' 
	   || *(p-1) == 0 || *(p-1) == '\n' || *(p-1) == '*') ||
	   *(p-1) == '=' )){



	  if( ! (*p == ' '|| *p == '\t' || *p == ';' || *p == ':' 

		 || *p == 0 || *p == '\n' || *p == '*'))  return NULL;
      }


  }
  if(pp > data){
    p = pp-1;
    if( ! (*p == ' '|| *p == '\t' || *p == ';' || *p == ':'
	   || *p == 0 || *p == '\n' || *p == '*'))  return NULL;
  }

  return pp;
}

char *delete_ppd_comment(char *line){
  char *p;
  p = strchr(line , '%');
  if(p)*p = 0;

  p = strchr(line , 0x0a);
  if(p)*p = 0;

  p = strchr(line , 0x0d);
  if(p)*p = 0;
  p = strchr(line , 0x0c);
  if(p)*p = 0;

  if(line[0] == 0)return NULL;

  return line;

}



char *chk_ppd_default_setting_line(char *ppd_line){
  char *p;
  p=strstr(ppd_line,"*Default");
  if(p != ppd_line)return NULL;
  return p;
}




char *get_token(char *input,char *output){
  char c,*pi,*po;

  po = output;
  pi = input;

  while((c = *pi)){
    switch(c){
    case ' ':

    case '\t':
    case '=':
    case ':':
    case 0x0d:
    case 0x0a:
    case 0x0c:
      pi++;
      continue;
    case 0x00:
      break;
    }    
    break;
  }


  while((c = *pi)){
    switch(c){
    case ' ':
    case '\t':
    case '=':
    case 0x0d:
    case 0x0a:
    case 0x0c:
    case 0x00:
      *po = 0;
      //DEBUGPRINT2("get_token2A:   [%s]  [%s] \n", input,output);
      return output; 


      break;
    }
    *po = c;
    po ++;
    pi ++;
  }
  *po = 0;
  //  DEBUGPRINT2("get_token2B :   [%s]  [%s] \n", input,output);
 
  return output;
}








//--------------------for debug log-------------------
void write_log_file(int level,char *format,...){
//  FILE  *fp_logfile;

  char  logdata[300];
  va_list argp;
  va_start(argp,format);


  if(log_level == 0)return;
  if(level > log_level)return;

  vsprintf(logdata,format,argp);

  fputs(logdata , stdout);
  fflush(stdout);
  return ;
}




#define  MEDIAEQ1   "media="
#define  MEDIAEQ2   "PageSize="

int divide_media_token(char *input,char output[5][30]){
  char media_command[100];
  char *p,*pp;
  int i;

	if((p = strstr(input,MEDIAEQ1)) != NULL)
	{
		strcpy(media_command,p+strlen(MEDIAEQ1));
	}
	else if((p = strstr(input,MEDIAEQ2)) != NULL)
	{
		strcpy(media_command,p+strlen(MEDIAEQ2));
	}
	else
	{
		return 0;
	}
	


  media_command[sizeof(media_command)-1] = 0;
  media_command[sizeof(media_command)-2] = 0;
  media_command[sizeof(media_command)-3] = 0;


  p = strchr(media_command,' ');
  if(p)*p =0;

  p = strchr(media_command,'\t');

  if(p)*p =0;

  p = strchr(media_command,'\n');
  if(p)*p =0;

  p = media_command;

  for (i = 0; i < 5;  ){
    if(*p == 0)break;
    pp = strchr(p , ',');
    if(pp) *pp = 0;
    strcpy(output[i],p);
    i ++;
    if(pp == NULL)break;
    p = pp+1;
  }
  return i;
}








CMDLINELIST standard_side_commandlinelist[] = {

	   {  "sides=two-sided-long-edge"   ,   "-dx ON -dxt LONG"       },
	   {  "sides=two-sided-short-edge"   ,   "-dx ON -dxt SHORT"       },
	   {  "sides=one-side"   ,   "-dx OFF"       },
	   { NULL   ,   NULL    }
};
