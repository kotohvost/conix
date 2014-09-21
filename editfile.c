#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "defs.h"
/****************************************************************************/
				editfile(name)
/*                      ������஢���� 䠩��
/****************************************************************************/
	char    *name;          /* ��� ।����㥬��� 䠩�� */
{
extern  struct  win     win2;   /* ���ᠭ�� ���� �訡�� �맮�� ।����*/
extern  struct  win     win3;   /* ���ᠭ�� ���� �訡�� ᮧ����� �����*/
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
	char    *editor;
	pid_t   pid;
	int     i;
/*==========================================================================*/
nfile=panels[cpanel].curfile;
nfirst=panels[cpanel].firstfile;

if( !(editor=getenv("EDITOR")) ){
    editor="redin";
}
endwin();
switch(pid=fork())
  {
    case 0: /* ��஦����� ����� */
      for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
      execlp(editor, editor, name, NULL);
      doupdate();
      win2.txts[1].txt=editor;
      ask(&win2,5,0); /* ᮮ�饭�� �� �訡��*/
      exit(-1);
    break;

    case -1: /* �� ᮧ������ �����*/
      doupdate();
      ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
    break;

    default: /* �����-த�⥫� */
      while(wait(NULL)!=pid);
      doupdate();
      if(use_keycap) setkey_(2); /* ���樠������ (�१ ESC)*/
    break;
  }

outpanel=cpanel;
read_to_panel();  /* extern outpanel */
if(nfile<panels[cpanel].nfiles) {
  panels[cpanel].firstfile=nfirst;
  panels[cpanel].curfile=nfile;
  refr_panel();
}
wrefresh(panels[cpanel].win);
}
