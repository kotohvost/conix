#include "defs.h"
#include <sys/stat.h>
/****************************************************************************/
				makedir()
/*                      �������� ��⠫���
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  win     win22;  /* ����� �� ������� ����� ��⠫��� */
extern  struct  win     win23;  /* ᮮ�饭�� �� �訡��*/
extern  char    *tmp_str;       /* ��ப� ��� �६����� ������ */
extern  size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
	int     retcode;        /* ��� ��室� 0-OK, !0-�訡�� */
	char    str[25];        /* ��ப� ��� ᮮ�饭��*/
	int     nfile;          /* ��� ���᪠ ⥪�饣� 䠩�� */
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
 struct inp_lst inp_lst[2];       /* ᯨ᮪ ����� ��� inp_menu()*/
/*==========================================================================*/
inp_lst[0].str=tmp_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=tmp_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

switch(inp_menu(inp_lst, &win22, 1, HOR|VERT))
  {
    case 1:  /* ��� */
    case 2:  /* Create */
      if( (retcode=mkdir(tmp_str,0777))!=0)
	{ /* ��⠫�� �� ᮧ��� */
	  if(strlen(tmp_str) >= sizeof(str))
	    { /* ������ ��� �� ����頥��� */
	      strcpy(str,"...");
	      strncpy(str+3, tmp_str, sizeof(str)-3);
	      str[sizeof(str)-1]='\0';
	    }else{ /* ��� 䠩�� ����頥��� 楫����*/
	      strcpy(str,tmp_str);
	    }
	  win23.txts[1].x=(win23.cols-strlen(str))/2;
	  win23.txts[1].txt=str;
	  ask(&win23,2,0); /* ᮮ�饭�� �� �訡��*/
	}
    break;

    default:
      retcode=(-1);
    break;
  }

if(retcode==0){
  /* ����⠭������� ࠡ�祣� ���ﭨ� �࠭� � ��⠭���� ⥪�饣� ��������� */
  outpanel=cpanel;
  read_to_panel();  /* extern outpanel */
  if(tmp_str[0]){ /* makedir() ����ᠫ� � tmp_str ��� ᮧ������� ��⠫���*/
    /* ���� ��⠫��� � ���������묨 ������ */
    for(nfile=0,item= panels[cpanel].itembuf;
       nfile<panels[cpanel].nfiles &&
       strcmp(tmp_str,panels[cpanel].names+item->nname)
       ;nfile++,item++);

    if(nfile<panels[cpanel].nfiles){ /* ������*/
      panels[cpanel].curfile=nfile;
      if(nfile>=(panels[cpanel].lines-panels[cpanel].lnused))
	{ /* 䠩� - �� �� ��ࢮ� ��࠭��*/
	  panels[cpanel].firstfile=nfile-(panels[cpanel].lines-panels[cpanel].lnused)/2;
	  if(panels[cpanel].firstfile<0) panels[cpanel].firstfile=0;
	}else{ /* �� ��ࢮ� ��࠭��*/
	  panels[cpanel].firstfile=0;
	}
      refr_panel();
    }
  }

  for(outpanel=0; outpanel<NPANELS; outpanel++){
    if(outpanel!=cpanel && strcmp(panels[outpanel].full_dir,panels[cpanel].full_dir)==0){
      if(chdir(panels[outpanel].full_dir)) beep();
      nfile=panels[outpanel].curfile;
      nfirst=panels[outpanel].firstfile;
      read_to_panel();  /* extern outpanel */
      if(nfile<panels[outpanel].nfiles) {
	panels[outpanel].firstfile=nfirst;
	panels[outpanel].curfile=nfile;
	refr_panel();
      }
    }
    if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
  }
}
return(retcode);
}
