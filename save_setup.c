#include <stdlib.h>
#include "defs.h"
/****************************************************************************/
				save_setup()
/*                      ������ ����஥� � 䠩� $home/.conix.ini
 *                      ��� ./.conix.ini
/****************************************************************************/
{
extern  struct  win     win51;  /* ᮮ�饭�� � ����� */
extern  struct  win     win52;  /* ᮮ�饭�� �� �訡�� ���室� � $home*/
extern  struct  win     win53;  /* ᮮ�饭�� �� �訡�� ����� � 䠩� */
extern  struct  win     win58;  /* ����� � ���� ��࠭���� setup */
extern struct panel panels[];
extern  int     cpanel;         /* ⥪��� ������*/
extern  int     outpanel;       /* �뢮����� ������ */

extern  char    cmp_exist;      /* �ਧ��� �ࠢ����� �� ������ */
extern  char    cmp_time;       /* �ਧ��� �ࠢ����� �� �६��� */
extern  char    cmp_size;       /* �ਧ��� �ࠢ����� �� ࠧ���� */
extern  char    cmp_mode;       /* �ਧ��� �ࠢ����� �� ������ */
extern  char    cmp_body;       /* �ਧ��� �ࠢ����� �� ᮤ�ন���� */
extern  char    cmp_nlink;      /* �ਧ��� �ࠢ����� �� ���-�� ��뫮� */

extern  int     prompt;         /* ⨯ �ਣ��襭��:0-���, 1-NodeName */
extern  int     clock_need;     /* �ਧ��� �뢮�� �६��� 0-��� 1-�� */

	char    *home;          /* "����譨�" ��⠫�� ���짮��⥫�*/
	char    str[80];        /* ��ப� ��� ����� */
	short   len;
static  char    name[]=".conix.ini";
	FILE    *setup;         /* 䠩�, � ���஬ ��࠭����� ����ன�� */
	int     error=0;        /* �ਧ��� �訡��*/
	short   swaps=1;        /* ��� �஢�ન � ��⠭���� �ਧ���� ��ॢ��� ���⮢ */
/*==========================================================================*/
switch(ask(&win58, 2, HOR|VERT))
  {
    case 2: /* � ����譥� ��⠫��� */
      home=getenv("HOME");
      if(!home) home=getenv("home");
    break;
    case 3: /* � ⥪�饬 ��⠫��� */
      home=".";
      if(chdir(panels[cpanel].full_dir)) home=NULL;
    break;
    default:
      error=1;
    break;
  }
if(!error){ /* � ������ ���� error-�ਧ��� ��࠭���� (�-�� ��� �⪠������)*/
  if((!home) || chdir(home))
    { /* ��� HOME ��� ������㯥� ����譨� ��⠫�� */
      ask(&win52, 2, 0);
    }else{ /* ����� �஡����� ������� */
      /* ��ନ஢��� ��� */
      len=strlen(home);
      if(len+sizeof(name)+1>=win51.cols)
	{ /* ������ ��� �� ����頥��� */
	  strcpy(str, "...");
	  strcat(str, home+(len+sizeof(name)+1-win51.cols)+3);
	  strcat(str, "/");
	  strcat(str, name);
	}else{
	  strcpy(str, home);
	  strcat(str, "/");
	  strcat(str, name);
	}
      win51.txts[1].txt=str;
      win51.txts[1].x=(win51.cols-strlen(str))/2;
      win53.txts[1].txt=str;
      win53.txts[1].x=(win53.cols-strlen(str))/2;
      soob(&win51);
      if(!(setup=fopen(name, "w")))
	{ /* �� ���뢠���� 䠩� */
	  ask(&win53, 2, 0);
	}else{
	  if(!error) fwrite(&swaps, sizeof(swaps), 1, setup);
	  len=cpanel;
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fwrite(&len, sizeof(len), 1, setup);
	  for(outpanel=0; outpanel<NPANELS; outpanel++){
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&panels[outpanel].on, sizeof(panels[outpanel].on), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    len=strlen(panels[outpanel].full_dir)+1;
	    if(!error) fwrite(&len, sizeof(len), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].full_dir, len, 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&panels[outpanel].sort, sizeof(panels[outpanel].sort), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].outfields, sizeof(panels[outpanel].outfields), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].filter_read, sizeof(panels[outpanel].filter_read), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].filter_noread, sizeof(panels[outpanel].filter_noread), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;

	    if(!error) fwrite(&cmp_exist, sizeof(cmp_exist), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_time , sizeof(cmp_time ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_size , sizeof(cmp_size ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_mode , sizeof(cmp_mode ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_body , sizeof(cmp_body ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_nlink, sizeof(cmp_nlink), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;

	    if(!error) fwrite(&prompt, sizeof(prompt), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;

	    if(!error) fwrite(&clock_need, sizeof(clock_need), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	  }
	  if(error){
	    unlink(name); /* 䠩� ����ᠭ �����४⭮ */
	    ask(&win53, 2, 0);
	  }
	  fclose(setup);
	}
      delsoob();
    }
}
}
