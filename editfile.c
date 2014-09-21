#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "defs.h"
/****************************************************************************/
				editfile(name)
/*                      Редактирование файла
/****************************************************************************/
	char    *name;          /* имя редактируемого файла */
{
extern  struct  win     win2;   /* описание окна ошибки вызова редактора*/
extern  struct  win     win3;   /* описание окна ошибки создания процесса*/
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
	int     nfile;          /* номер текущего файла*/
	int     nfirst;         /* номер первого выводимого файла*/
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
    case 0: /* порожденный процесс */
      for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
      execlp(editor, editor, name, NULL);
      doupdate();
      win2.txts[1].txt=editor;
      ask(&win2,5,0); /* сообщение об ошибке*/
      exit(-1);
    break;

    case -1: /* не создается процесс*/
      doupdate();
      ask(&win3,1,0); /* сообщение об ошибке*/
    break;

    default: /* процесс-родитель */
      while(wait(NULL)!=pid);
      doupdate();
      if(use_keycap) setkey_(2); /* инициализация (через ESC)*/
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
