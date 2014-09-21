#include "defs.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
/****************************************************************************/
			renmove()
/*              Переименование/перемещение файла/файлов
/****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  win     win3;
extern  struct  win     win44;  /* запрос на подтверждение переим.группы ф.*/
extern  struct  win     win45;  /* сообщение об ошибке*/
extern  struct  win     win46;  /* сообщение об ошибке доступа к каталогу */
extern  struct  win     win47;  /* сообщение о копировании файла(в)*/
extern  struct  win     win48;  /* сообщение о копировании*/
extern  struct  win     win49;  /* сообщение об ошибке */
extern  struct  win     win77;  /* запрос на Overwrite */
extern  char    *tmp_str;       /* строка для временных данных */
extern  size_t  tmp_len;        /* место, выделенное для tmp_str*/
	char    str[80];
	char    *name;          /* имя файла */
	pid_t   pid;
	pid_t   pid1;
	pid_t   pid_ready;
	int     wait_stat;      /* статус завершения процесса */
 struct itembuf *item;          /* для обработки выбранного файла*/
 struct itembuf *item_mark;     /* для обработки единственного выбранного файла*/
	int     tested;         /* количество проверенных файлов*/
	int     incl_dir;       /* признак наличия каталогов в списке*/
	int     fd[2];
	int     fd_err[2];      /* для переназначения протокола ошибок*/
	char    **spis;         /* список аргументов для программы копирования*/
	char    **spis_p;       /* список аргументов для программы копирования*/
	int     i;
	int     status;         /* состояние завершения процесса*/
	int     copy=0;         /* признак и тип */
	int     panel;          /* "не текущая" панель */
	int     err=0;          /* признак ошибки */
	int     say_error=1;    /* сообщать об ошибках */
	int     nfile;          /* номер текущего файла*/
	int     nfirst;         /* номер первого выводимого файла*/
	int     exists=0;       /* признак необх-сти запроса Overwrite */
	char    *ptr;           /* для образования имени в каталоге */
 struct stat    stbuf;
 struct inp_lst inp_lst[2];      /* список ввода для inp_menu()*/
 static char    copyprog[]="mv"; /*программа копирования*/
/*==========================================================================*/
inp_lst[0].str=tmp_str;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=tmp_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* признак конца списка для inp_menu()*/

if(cpanel)
  {
    panel=0;
  }else{
    panel=1;
  }

if(panels[panel].on==1 || panels[panel].on==3 || panels[cpanel].selected)
  { /* противоположная панель включена или есть отмеченные файлы */
    name=panels[panel].full_dir;
  }else{
    name=panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname;
  }
if(tmp_len<=strlen(name)){
  free(tmp_str);
  tmp_len=(strlen(name)/128+1)*128;
  tmp_str=malloc(tmp_len);
  tst_err_malloc(tmp_str,9);
}

strcpy(tmp_str,name);

if(panels[cpanel].selected)
  { /* Выделено несколько файлов/каталогов */
    /* проверить, есть ли среди отм. файлов каталоги*/
    for(nfile=0, item=panels[cpanel].itembuf, tested=0, incl_dir=0;
	tested<panels[cpanel].selected && nfile<panels[cpanel].nfiles && incl_dir==0;
	item++,nfile++){
	if(item->attrs & MARKED){
	  item_mark=item;
	  tested++;
	  if(filtype(item->mode)==FTYPE_DIR) incl_dir=1;
	}
    }
    if(incl_dir)
      { /* есть каталоги */
	sprintf(str, "Move %d files and dirs to", panels[cpanel].selected);
      }else{ /* только файлы */
	sprintf(str, "Move %d files to", panels[cpanel].selected);
      }
    win44.txts[0].txt = str;
    /*win44.txts[0].x=(win44.cols-strlen(str))/2;*/
    switch(inp_menu(inp_lst, &win44, 1, HOR|VERT))
      {
	case 1: /* нажали Enter на имени каталога */
	case 2: /* нажали Enter на <Move> */
	  if(panels[cpanel].selected==1)
	    { /*безусловно попробовать */
	      copy=1;
	    }else{ /* несколько файлов можно копировать в существ. каталог*/
	      if(stat(tmp_str, &stbuf)==0 &&
		 filtype(stbuf.st_mode)==FTYPE_DIR &&
		 access(tmp_str, W_OK)==0)
		{ /* есть доступный на запись каталог */
		  copy=1;
		}else{ /* что-то не в порядке с каталогом, в который копируем*/
		  copy=0;
		  ask(&win46, 2, HOR);
		}
	    }
	break;
	default: /* ESC и ошибки */
	case 3: /* нажали Enter на <Cancel> */
	  copy=0;
	break;
      }
    /* if(ask(&win44, 0, HOR) == 0) copy=1; */
  }else{ /* копирование одного файла/каталога без отметки */
    item_mark=item= &panels[cpanel].itembuf[panels[cpanel].curfile];
    if(filtype(item->mode)==FTYPE_DIR)
      { /* каталог */
	incl_dir=1;
	strcpy(str, "Move DIRECTORY \"");
      }else{ /* файл */
	incl_dir=0;
	strcpy(str, "Move file \"");
      }
    i=win44.cols-2*win44.txts[0].x-5; /* макс. место для строки */
    strncat(str, panels[cpanel].names+item->nname, i-strlen(str));
    str[i]='\0';
    if(strlen(str)==i){ /* имя поместилось не все*/
      str[i-1]=str[i-2]=str[i-3]='.';
    }
    strcat(str, "\" to");

    win44.txts[0].txt = str;
    /*win44.txts[0].x=(win44.cols-strlen(str))/2;*/
    switch(inp_menu(inp_lst, &win44, 1, HOR|VERT))
      {
	case 1: /* нажали Enter на имени каталога */
	case 2: /* нажали Enter на <Move> */
	  copy=1;
	break;
	default: /* ESC и ошибки */
	case 3: /* нажали Enter на <Cancel> */
	  copy=0;
	break;
      }

    if(copy){
      /* имитируем отметку */
      /* item= &panels[cpanel].itembuf[panels[cpanel].curfile]; - было выше*/
      item->attrs ^= MARKED;
      panels[cpanel].selected++;
      panels[cpanel].size_sel+=item->size;
    }
  }

if(copy && stat(tmp_str, &stbuf)==0){ /* проверить на overwrite */

  if(filtype(stbuf.st_mode)==FTYPE_DIR)
    { /* копируем в каталог */
      for(ptr=tmp_str; *ptr; ptr++);
      *ptr='/';

      for(nfile=0, item=panels[cpanel].itembuf, tested=0;
	tested<panels[cpanel].selected && nfile<panels[cpanel].nfiles && exists==0;
	item++,nfile++){
	if(item->attrs & MARKED){
	  strcpy(ptr+1, panels[cpanel].names+item->nname);
	  if(access(tmp_str, 0)==0) exists++;
	}
      }
      *ptr='\0';
    }else{ /* копируем в существующий файл*/
      exists++;
    }
}

if(exists){ /* подтверждение на Overwrite */
  if(ask(&win77, 0, HOR) != 0) copy=0;
}

if(copy){

      for(nfile=0, item=panels[cpanel].itembuf;
	  panels[cpanel].selected && nfile<panels[cpanel].nfiles && copy;
	  item++,nfile++){
	  if(item->attrs & MARKED){

	    name=panels[cpanel].names+item->nname;
	    if(strlen(name) >= sizeof(str))
	      { /* полное имя не помещается */
		strcpy(str,"...");
		strncpy(str+3, name, sizeof(str)-3);
		str[sizeof(str)-1]='\0';
	      }else{ /* имя файла помещается целиком*/
		strcpy(str,name);
	      }
	    win48.txts[1].x=(win48.cols-strlen(str))/2;
	    win48.txts[1].txt=str;
	    soob(&win48);

	    switch(pid=fork())
	      {
		case 0:  /* потомок - будет вызывать *copyprog */
		  /* текущий каталог установлен в main() */
		  if((i=open("/dev/null", O_WRONLY))>=0){
		    dup2(i,2); /* переназначение протокола ошибок */
		    close(i);
		  }

		  for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		  execlp(copyprog, copyprog, "-f",
			 panels[cpanel].names+item->nname,
			 tmp_str,
			 NULL);

		  exit(-1); /* ошибка вызова - будет проверен код*/
		break;

		case -1: /* не создается процесс */
		  ask(&win3,1,0); /* сообщение об ошибке*/
		break;

		default: /* процесс - родитель */
		     while(wait(&wait_stat)!=pid);
		     if(wait_stat)
		       { /* была ошибка */
			 if(say_error){
			   win49.txts[1].x=(win49.cols-strlen(str))/2;
			   win49.txts[1].txt=str;
			   switch(ask(&win49,2,HOR))
			     {
			       default:
			       case 2: /* OK */
			       break;

			       case 3: /* Silently - не спрашивать (в nc этого нет)*/
				 say_error=0;
			       break;

			       case 4: /* Cancel (в nc этого нет)*/
				 copy=0;
			       break;
			     }
			 }
		       }
		     item->attrs ^= MARKED;
		     panels[cpanel].selected--;
		     panels[cpanel].size_sel-=item->size;
		     outpanel=cpanel;
		     refr_panel();

		break;
	      } /* switch */
	  } /* if(отмечен) */
      }   /* for по файлам */
}
delsoob();

/* Восстановление нормального экрана */
for(outpanel=0;outpanel<NPANELS;outpanel++){
    if(copy)
      { /* что-то копировалось */
	nfile=panels[outpanel].curfile;
	nfirst=panels[outpanel].firstfile;
	if(chdir(panels[outpanel].full_dir)) beep();
	read_to_panel();
	if(nfile<panels[outpanel].nfiles) {
	  panels[outpanel].firstfile=nfirst;
	  panels[outpanel].curfile=nfile;
	  refr_panel();
	}
      }else{
#ifdef FREEBSD
	if(panels[outpanel].on>0) refr_panel();
#else
	if(panels[outpanel].on>0) touchwin(panels[outpanel].win);
#endif
      }
    if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
}

}
