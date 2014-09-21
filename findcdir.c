#include "defs.h"
/****************************************************************************/
				findcdir()
/*                      ���� 䠩�� � ⥪�饬 ��⠫���
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  win     win40;  /* ���� ����� 蠡���� ���᪠*/
extern  struct  win     win41;  /* ᮮ�饭��-����� (䠩� �� ������)*/
extern  char    *fnd_str;       /* ��ப� ��� ���᪠ */
extern  size_t  fnd_len;        /* ����, �뤥������ ��� fnd_str*/
	int     find;           /*�ਧ��� ���᪠*/
	int     stop=0;         /*�ਧ��� ������� ���᪮� */
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
 struct inp_lst inp_lst[2];       /* ᯨ᮪ ����� ��� inp_menu()*/
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
/*==========================================================================*/
/* fnd_str[0]='\0'; - �� ����祭� ��� ������� ���᪮� */

inp_lst[0].str=fnd_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

while(!stop){
  find=inp_menu(inp_lst, &win40, 1, HOR|VERT);
  if(find==1 || find==2)
    { /* �᪠�� 䠩� */
      find=strlen(fnd_str);
      if(find==0 || fnd_str[find-1]!='*') strcpy(fnd_str+find, "*");
      find=0;
      /* ᭠砫� �饬 ���� */
      for(nfile=panels[cpanel].curfile+1, item=panels[cpanel].itembuf+nfile;
	  nfile<panels[cpanel].nfiles && !find;
	  item++,nfile++){
	if(inregular(panels[cpanel].names+item->nname, fnd_str)) find=1;
      }
      if(!find){ /* ���� �� ������, �饬 � ��砫� ������*/
	for(nfile=2, item=panels[cpanel].itembuf+nfile;
	    nfile<panels[cpanel].nfiles && !find;
	    item++,nfile++){
	  if(inregular(panels[cpanel].names+item->nname, fnd_str)) find=1;
	}
      }
      if(find)
	{ /* ������ 䠩�, ᮮ⢥�����騩 蠡���� */
	  panels[cpanel].curfile=nfile-1;

	  if(nfile<=panels[cpanel].firstfile ||
	     nfile>panels[cpanel].firstfile+
	       (panels[cpanel].lines-panels[cpanel].lnused-1)){ /*䠩� - �� �� ⥪.��࠭��*/
	      /* ���஡����� �������� � �।��� ��࠭���*/
	      panels[cpanel].firstfile=nfile-(panels[cpanel].lines-panels[cpanel].lnused)/2;

	      /* �᫨ ��᫥���� ��࠭�� �� �뢥����� ���, ᬥ���� ��*/
	      if(panels[cpanel].firstfile+panels[cpanel].lines-
		 panels[cpanel].lnused > panels[cpanel].nfiles-1)
		   panels[cpanel].firstfile=panels[cpanel].nfiles-
			  (panels[cpanel].lines-panels[cpanel].lnused);

	      /* �᫨ 䠩��� �� ������ ����, �뢥�� ᭠砫� (�.�.�� �஢�७�?)*/
	      if(panels[cpanel].firstfile<0) panels[cpanel].firstfile=0;
	  }

	  if(panels[cpanel].on<0) panels[cpanel].on *= -1; /* ������� ������, �᫨ ��� �뫠 �몫�祭�*/
	  outpanel=cpanel;
	  refr_panel();
	  wrefresh(panels[cpanel].win);
	  stop++;
	}else{ /* 䠩� �� ������ */
	  if(ask(&win41,1,HOR)!=2) stop++;
	}
    }else{ /* �⪠� �� ���᪠ */
      stop++;
    }
}
}
