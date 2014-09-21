#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "defs.h"
/****************************************************************************/
				deletefile()
/*                      Удаление файла/файлов
/****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  win     win4;   /* запрос на удаление группы файлов */
extern  struct  win     win5;   /* запрос на удаление файла из группы*/
extern  struct  win     win6;   /* запрос на удаление read only - файла*/
extern  struct  win     win3;   /* описание окна ошибки создания процесса*/
extern  struct  win     win7;   /* сообщение о начале удаления каталога*/
extern  struct  win     win8;   /* сообщение об ошибке удаления каталога*/
extern  struct  win     win9;
extern  struct  win     win10;
extern  struct  win     win11;
extern  struct  win     win12;
extern  struct  win     win13;  /* сообщение об ошибке удаления одиночного ф.*/
extern  struct  win     win14;  /* сообщение об ошибке удаления каталога*/
	char    str[40];
	int     del_all=0;      /* признак ответа delete all*/
	int     stop=0;         /* признак прекращения удаления */
	int     del_file;       /* признак удаления файла */
	int     ask_ronly=1;    /* запрашивать подтверждение для read only*/
	int     say_error=1;    /* сообщать об ошибках */
	char    *name;          /* имя файла */
	pid_t   pid;
	int     wait_stat;      /* статус завершения процесса */
 struct itembuf *item;          /* для обработки выбранного файла*/
	int     nfile;          /* номер текущего файла*/
	int     nfirst;         /* номер первого выводимого файла*/
	int     i;
	int     deleted=0;      /* признак того, что что-то удалялось */
	int     namelen;        /* длина имени файла */
	int     outlen;         /* минимальный размер максимальной длины имени файла, помещающегося в окна запросов*/
unsigned char   *ptr;           /* для проверки имени на CTRL-символы */
/*==========================================================================*/
/* минимальный размер максимальной длины имени файла, помещающегося в окна запросов*/
outlen=sizeof(str)-1;
if(outlen>win5.cols) outlen=win5.cols;
if(outlen>win6.cols) outlen=win6.cols;
if(outlen>win7.cols) outlen=win7.cols;
if(outlen>win8.cols) outlen=win8.cols;
if(outlen>win9.cols) outlen=win9.cols;
if(outlen>win10.cols) outlen=win10.cols;
if(outlen>win11.cols) outlen=win11.cols;
if(outlen>win12.cols) outlen=win12.cols;
if(outlen>win13.cols) outlen=win13.cols;
if(outlen>win14.cols) outlen=win14.cols;
outlen-=4; /* на рамку и пробелы около нее */
/*-------------------------------------*/

if(panels[cpanel].selected)
  {
   sprintf(str, "You have selected %d files.", panels[cpanel].selected);
   win4.txts[2].txt = str;
   if(ask(&win4, 1, HOR) == 0){ /* получено подтверждение*/
     for(nfile=0; stop==0 && panels[cpanel].selected && nfile<=panels[cpanel].nfiles; nfile++){
       item= &panels[cpanel].itembuf[nfile];
       if(item->attrs & MARKED){ /* выбранный файл */
	 /* Сформировать имя файла для запросов*/
	 name=panels[cpanel].names+item->nname;
	 name_to_out(name, str, outlen);

	 if(del_all)
	  { /* Удаление без запросов*/
	    del_file=1;
	  }else{ /* удаление с запросами */
	     win5.txts[1].x=(win5.cols-strlen(str))/2;
	     win5.txts[1].txt=str;
	     switch(ask(&win5, 2, HOR))
	       {
		 case 2: /* Delete */
		   del_file=1;
		 break;

		 case 3: /* All */
		   del_file=1;
		   del_all=1;
		 break;

		 case 4: /* Skip */
		   del_file=0;
		 break;

		 default:
		 case 5: /* Cancel */
		   del_file=0;
		   stop=1;
		 break;
	       }
	 } /* if(!del_all) */

	 /* подтверждение для read only файлов */
	 if(del_file && ask_ronly){ /* выбрано удаление или режим del_all*/
	     if( access(name,W_OK)){ /* В файл запрещено писать*/
	       win6.txts[1].x=(-1);
	       win6.txts[1].txt=str;
	       switch(ask(&win6,3,HOR))
		 {
		   case 3: /* Delete */
		     /* del_file=1;*/
		   break;
		   default:
		   case 4: /* Skip (в nc-Cancel, но это неправда)*/
		     del_file=0;
		   break;
		   case 5: /* Silently - не спрашивать (в nc этого нет)*/
		     ask_ronly=0;
		   break;
		   case 6: /* Cancel (в nc этого нет)*/
		     del_file=0;
		     stop=1;
		   break;
		 }
	     }
	 }

	 /* Удаление */
	 if(del_file){
	   deleted++;
	   if(filtype(item->mode)==FTYPE_DIR)
	     { /* каталог */
	       win7.txts[1].x=(-1);
	       win7.txts[1].txt=str;
	       soob(&win7);
	       switch(pid=fork())
		 {
		   case 0: /* порожденный процесс*/
		     close(2);
		     open("/dev/null",O_WRONLY);
		     for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		     execlp("rm", "rm", "-rf", name, NULL);
		     exit(-1); /* ошибка вызова - будет проверен код*/
		   break;
		   case -1: /* не создается процесс */
		     ask(&win3,1,0); /* сообщение об ошибке*/
		   break;
		   default: /* процесс - родитель */
		     while(wait(&wait_stat)!=pid);
		     if(wait_stat)
		       { /* была ошибка стирания*/
			 if(say_error){
			   win8.txts[1].x=(-1);
			   win8.txts[1].txt=str;
			   switch(ask(&win8,2,HOR))
			     {
			       default:
			       case 2: /* OK */
			       break;

			       case 3: /* Silently - не спрашивать (в nc этого нет)*/
				 say_error=0;
			       break;

			       case 4: /* Cancel (в nc этого нет)*/
				 del_file=0;
				 stop=1;
			       break;
			     }
			 }
		       }else{ /* каталог нормально стерт */
			 item->attrs ^= MARKED;
			 panels[cpanel].selected--;
			 panels[cpanel].size_sel-=item->size;
			 /* refr_panel(); wrefresh(panels[cpanel].win) */
		       }
		   break;
		 }
	     }else{ /* файл или что-то специальное */
	       win9.txts[1].x=(-1);
	       win9.txts[1].txt=str;
	       soob(&win9);
	       if(unlink(name))
		 { /* ошибка удаления файла */
		   if(say_error){
		     win10.txts[1].x=(-1);
		     win10.txts[1].txt=str;
		     switch(ask(&win10,2,HOR))
		       {
			 default:
			 case 2: /* OK */
			 break;

			 case 3: /* Silently - не спрашивать (в nc этого нет)*/
			   say_error=0;
			 break;

			 case 4: /* Cancel (в nc этого нет)*/
			   del_file=0;
			   stop=1;
			 break;
		       }
		   }
		 }else{ /* файл нормально стерт */
		   item->attrs ^= MARKED;
		   panels[cpanel].selected--;
		   panels[cpanel].size_sel-=item->size;
		   outpanel=cpanel;
		   refr_panel();
		 }
	     }
	 } /* if(del_file) */
       } /* if MARKED */
     } /* for по файлам */
   } /* подтверждение стирания */

  }else{/*---------------------- Нет отмеченных файлов ---------------------*/

    item= &panels[cpanel].itembuf[panels[cpanel].curfile];
    /* Сформировать имя файла для запросов*/
    name=panels[cpanel].names+item->nname;
    name_to_out(name, str, outlen);

    win11.txts[1].x=(-1);
    win11.txts[1].txt=str;
    switch(ask(&win11, 3, HOR))
      {
	case 2: /* Delete */
	  del_file=1;
	break;

	default:
	case 3: /* Cancel */
	  del_file=0;
	break;
      }

    /* подтверждение для read only файлов */
    if(del_file){
	if( access(name,W_OK)){ /* В файл запрещено писать*/
	  win12.txts[1].x=(-1);
	  win12.txts[1].txt=str;
	  switch(ask(&win12,3,HOR))
	    {
	      case 3: /* Delete */
		/* del_file=1;*/
	      break;
	      default:
	      case 4: /* Cancel */
		del_file=0;
		stop=1;
	      break;
	    }
	}
    }

    /* Удаление */
    if(del_file){
      deleted++;
      if(filtype(item->mode)==FTYPE_DIR)
	{ /* каталог */
	  win7.txts[1].x=(-1);
	  win7.txts[1].txt=str;
	  soob(&win7);
	  switch(pid=fork())
	    {
	      case 0: /* порожденный процесс*/
		close(2);
		open("/dev/null",O_WRONLY);
		for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		execlp("rm", "rm", "-rf", name, NULL);
		exit(-1); /* ошибка вызова - будет проверен код*/
	      break;
	      case -1: /* не создается процесс */
		ask(&win3,1,0); /* сообщение об ошибке*/
	      break;
	      default: /* процесс - родитель */
		while(wait(&wait_stat)!=pid);
		if(wait_stat)
		  { /* была ошибка стирания*/
		    if(say_error){
		      win14.txts[1].x=(-1);
		      win14.txts[1].txt=str;
		      switch(ask(&win14,2,HOR))
			{
			  default:
			  case 2: /* OK */
			  break;

			  case 3: /* Silently - не спрашивать (в nc этого нет)*/
			    say_error=0;
			  break;

			  case 4: /* Cancel (в nc этого нет)*/
			    del_file=0;
			    stop=1;
			  break;
			}
		    }
		  }else{ /* каталог нормально стерт */
		    item->attrs ^= MARKED;
		    panels[cpanel].selected--;
		    panels[cpanel].size_sel-=item->size;
		    outpanel=cpanel;
		    refr_panel();
		  }
	      break;
	    }
	}else{ /* файл или что-то специальное */
	  soob(&win9);
	  if(unlink(name))
	    { /* ошибка удаления файла */
	      if(say_error){
		win13.txts[1].x=(-1);
		win13.txts[1].txt=str;
		switch(ask(&win13,2,HOR))
		  {
		    default:
		    case 2: /* OK */
		    break;

		    case 3: /* Silently - не спрашивать (в nc этого нет)*/
		      say_error=0;
		    break;

		    case 4: /* Cancel (в nc этого нет)*/
		      del_file=0;
		      stop=1;
		    break;
		  }
	      }
	    }else{ /* файл нормально стерт */
	      item->attrs ^= MARKED;
	      panels[cpanel].selected--;
	      panels[cpanel].size_sel-=item->size;
	      outpanel=cpanel;
	      refr_panel();
	    }
	}
    } /* if(del_file) */
  } /* if ... selected ... else */
delsoob();

for(outpanel=0;outpanel<NPANELS;outpanel++){
    if(deleted)
      { /* что-то удалялось */
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

