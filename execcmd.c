#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include "defs.h"
/****************************************************************************/
			execcmd(cmd)
/*                      Выполнение команды
/****************************************************************************/
	char    *cmd;           /* команда */
{
extern  char    *history;       /* буфер хранения истории */
extern  size_t  his_len;        /* место, выделенное для history*/
extern  int     his_cur;        /* номер элемента history для заполнения */
extern  int     his_go;         /* номер элемента history */
extern  struct  win     win1;   /* описание окна подтверждения выхода*/
extern  struct  win     win76;  /* сообщение о работе внешнего просмотрщика */
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     in_viewer;      /* признак вызова execcmd() в режиме просмотра */
static  char    *shell=NULL;    /* текущий интерпретатор команд */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  WINDOW  *soob_win;      /* окно выдачи сообщений */
	pid_t   pid;            /* номер процесса для ожидания */
	pid_t   pid1;           /* значение, возвращаемое wait() */
	int     i;
	chtype  ch;
	char    *ptr;
	int     stop=0;
	int     nfile;          /* номер текущего файла*/
	int     nfirst;         /* номер первого выводимого файла*/
	int     save;           /* признак сохранения команды в истории*/
	int     old_nosave_ask;
	int     cantchdir=1;
	DIR     *dirp;          /* для проверки читаемости каталога */
	struct dirent *dp;      /* аналогично */
/*==========================================================================*/

if(cmd[0]=='c' && cmd[1]=='d' &&
  (cmd[2]==' ' || cmd[2]=='\0'))
  { /* команду cd обрабатываем самостоятельно */
    for(i=2;cmd[i]==' ';i++);
    if(cmd[i])
      { /* задан каталог */

	cantchdir = 1;  /* теперь это признак неперехода в каталог*/

	if(access(cmd+i, X_OK|R_OK)==0) /* каталог, доступный для перехода и чтения*/
	  cantchdir = 0;

	/* проверка читаемости каталога нужна для того, чтобы не ловить root в нечитаемые (NFS) каталоги */
	if(cantchdir==0){
	  if(dirp = opendir(cmd+i))
	    {
	      if ( (dp=readdir(dirp))==NULL ) cantchdir++;
	      if ( cantchdir || (dp=readdir(dirp))==NULL ) cantchdir++;
	      closedir(dirp);
	      if(cantchdir) { beep(); beep(); }
	    }else{
	      cantchdir++;
	    }
	}
	if ( cantchdir || chdir( cmd+i )!=0) cantchdir++;

	if ( cantchdir ) beep();
      }else{ /* Пустой аргумент - переход в $HOME */
	if(!(ptr=getenv("HOME")))
	  { /* нет переменной HOME */
	    beep();
	  }else{
	    if(chdir(ptr)) beep();
	  }
      }
    outpanel=cpanel;
    read_to_panel();  /* extern outpanel */

  }else if(strcmp(cmd, "exit")==0){
    /* команду exit обрабатываем самостоятельно */
    if(ask(&win1,0,HOR)==0) stop++;
  }else{  /* обычная команда */
    overwrite(curscr, save_scr);
    if(in_viewer)
      { /* повесить сообщение об обработке */
	old_nosave_ask=nosave_ask;
	nosave_ask=1;
	soob(&win76);
	delwin(soob_win);
	soob_win=NULL;
	nosave_ask=old_nosave_ask;
      }else{ /* в режиме просмотра все должно выглядеть как встроенный просмотрщие*/
	clear();
	refresh();
      }
    overwrite(save_scr, stdscr);
#ifdef FREEBSD
    touchwin(stdscr);
#endif
    endwin();

    if(!in_viewer){ /* в режиме просмотра все должно выглядеть как встроенный просмотрщие*/
      /* for(i=COLS;i--;) putc('\n',stderr);*/
      fprintf(stderr,"%s\n", cmd);
    }
    if(!shell) shell=getenv("SHELL");
    if(!shell) shell="sh";

    switch(pid=fork())
      {
	case 0:  /* потомок */
	  for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
	  execlp(shell, shell, "-c", cmd, NULL);
	  perror("Can't execute 'shell'");
	  exit(-1);
	break;

	case -1:  /* не создается процесс */
	  perror("Can't fork");
	break;

	default:
	  do{
	   pid1=wait(NULL);
	  }while(pid1!=pid && pid1!=(-1));
	  /* вся обработка - после switch()*/
	break;
      }

    if(!in_viewer){ /* в режиме просмотра все должно выглядеть как встроенный просмотрщик*/
      fprintf(stderr,"\r\nPress ENTER to continue...");
    }
    for(outpanel=0;outpanel<NPANELS;outpanel++){
      nfile=panels[outpanel].curfile;
      nfirst=panels[outpanel].firstfile;
      chdir(panels[outpanel].full_dir);
      read_to_panel();  /* extern outpanel */
      if(nfile<panels[outpanel].nfiles) {
	panels[outpanel].firstfile=nfirst;
	panels[outpanel].curfile=nfile;
	refr_panel();
      }
    }
    if(!in_viewer){ /* в режиме просмотра все должно выглядеть как встроенный просмотрщие*/
      do{
	ch=getchar();
      }while(ch!='\r' && ch!='\n');
    }

    doupdate();
    refresh();
    if(use_keycap) setkey_(2); /* инициализация (через ESC)*/
  } /* if ( == "cd ...") else */

if(!in_viewer){ /* в режиме просмотра все должно выглядеть как встроенный просмотрщие*/
  /* Запись команды в историю */
  save=1;
  his_go=hispred(his_cur);
  if(his_go!=his_cur && strcmp(cmd, &history[his_go])==0) {
    /* текущая команда не отличается от предыдущей */
    save=0; /* не записывать */
  }

  if(save){ /* Записать команду в историю */
    his_go=strlen(cmd);
    if(his_go>his_len-his_cur-1){ /* команда не помещается в конец буфера*/
      while(his_cur<his_len-1) history[his_cur++]=1;
      his_cur=0;
    }
    strcpy(history+his_cur, cmd);
    his_cur+=his_go+1;
  }
  his_go=his_cur;
}

for(outpanel=0;outpanel<NPANELS;outpanel++){
  if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
}
return(stop);
}
