#include "defs.h"
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>
static  int     was_alarm;
#define LINES_LST 10
#define COLS_LST  60
#define BEGY_LST  ((LINES-LINES_LST)/2)
#define BEGX_LST  ((COLS-COLS_LST)/2)
/****************************************************************************/
			void    chkalarm(arg)
/*              Выставление признака прерывания по alarm
*****************************************************************************/
	int     arg;    /* чтобы не ругался cc на sun */
{
was_alarm=1;
}


/****************************************************************************/
				findfile()
/*                              Поиск файла
/****************************************************************************/
{
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  chtype  atr[];          /* раскраска */
extern  struct  win     win3;   /* окно сообщения об ошибке*/
extern  struct  win     win42;  /* окно запроса шаблона поиска*/
extern  struct  win     win43;  /* сообщение - не указан каталог начала поиска*/
extern  struct  win     win44;  /* сообщение об ошибке поиска*/
extern  struct  win     win74;  /* сообщение об ошибке поиска*/
extern  char    *fnd_str;       /* строка для поиска */
extern  size_t  fnd_len;        /* место, выделенное для fnd_str*/
extern  char    *tmp_str;       /* строка для поиска */
extern  size_t  tmp_len;        /* место, выделенное для tmp_str*/
extern  char    *last_search;   /* имя файла с последним результатом поиска */
static  char    schname[]="/tmp/conixXXXXXX";/*имя для mktemp()*/
	int     find;           /*признак поиска*/
	int     stop=0;         /*признак повторных поисков */
 struct itembuf *item;          /* для обработки выбранного файла*/
 struct inp_lst inp_lst[3];     /* список ввода для inp_menu()*/
	int     nfile;          /* номер текущего файла*/
	FILE    *prot=NULL;     /* результаты find в файле*/
	WINDOW  *win_box=NULL;  /* окно - родитель для win */
	WINDOW  *win=NULL;      /* окно работы со списком найденных файлов*/
 struct stat    stbuf;
	int     fd[2];
	int     x;
	int     y;
	int     ch;
	char    simb;
	int     stopfind;       /* признак остановки программы find */
	unsigned setalarm;      /* для запоминания старого значения alarm */
	void    (*old_alarm)(); /* старая функция обработки alarm*/
	pid_t   pid;            /* номер процесса для ожидания */
	pid_t   pid1;           /* значение, возвращаемое wait() */
	int     outprot=1;      /* признак вывода протокола в файл */
	short   curline;        /* номер текущей строки */
	short   firstline;      /* номер первой строки окна*/
	short   lines;          /* количество строк в списке */
	char    *dirname;       /* имя каталога */
	char    *filename;      /* имя файла, на который поставитть указатель*/
	int     i;
/*==========================================================================*/
/* fnd_str[0]='\0'; - не включено для повторных поисков */

if(chdir(panels[cpanel].full_dir)) beep();

/* Записать текущий каталог в строку редактирования */
if(tmp_len<=strlen(panels[cpanel].full_dir)){
  free(tmp_str);
  tmp_len=(strlen(panels[cpanel].full_dir)/128+1)*128;
  tmp_str=malloc(tmp_len);
  tst_err_malloc(tmp_str,13);
}
strcpy(tmp_str,panels[cpanel].full_dir);

/* Сформировать список ввода для inp_menu() */
inp_lst[0].str=fnd_str;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;

inp_lst[1].str=tmp_str;
/* inp_lst[1].fld_len=...;  выставит inp_menu()*/
inp_lst[1].str_len=tmp_len;
inp_lst[1].npunkt=3;

inp_lst[2].str=NULL; /* признак конца списка для inp_menu()*/

while(!stop){
  if(last_search){ /*был поиск, можно использовать его результаты */
      win42.txts[6].active=1;
  }/*иначе-уже 0*/

  find=inp_menu(inp_lst, &win42, 1, HOR|VERT);
  if(find==1 || find==3 || find==4)
    { /* искать файл */
      if(stat(tmp_str, &stbuf)!=0 || filtype(stbuf.st_mode)!=FTYPE_DIR)
	{ /* в tmp_str[] - не каталог */
	  if(ask(&win43,1,HOR)!=1) stop++;
	}else{ /* можно запускать find */
	  if(pipe(fd)){
	    endwin();
	    perror("Can't make pipe");
	    exit(-1);
	  }

	  switch(pid=fork())
	    {
	      case 0:  /* потомок */
		/* переназначить вывод и протокол ошибок */
		dup2(fd[1],1);
		dup2(fd[1],2);
		for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		execlp("find", "find", tmp_str, "-name", fnd_str, "-print", NULL);
		perror("Can't execute 'find'");
		exit(-1);
	      break;

	      case -1: /* не создается процесс */
		close(fd[0]);
		close(fd[1]);
		ask(&win3,1,0); /* сообщение об ошибке*/
	      break;

	      default: /* процесс - родитель */
		close(fd[1]);

		if(nosave_ask==0){
		  delsoob();
		  overwrite(curscr, save_scr);
		  overwrite(curscr, stdscr);
		  clearok(stdscr,FALSE);
		}

		win_box=subwin(stdscr,LINES_LST,COLS_LST,BEGY_LST,BEGX_LST);
		tst_err_malloc(win_box, 16);
		mywbkgd(win_box, POPUP_WIN_ATTR);
		box(win_box, 0, 0);

		win=subwin(stdscr,LINES_LST-2,COLS_LST-2,BEGY_LST+1,BEGX_LST+1);
		tst_err_malloc(win_box, 17);
		scrollok(win, TRUE);
		mywbkgd(win, POPUP_WIN_ATTR);
		wattrset(win, POPUP_WIN_ATTR);
		wrefresh(win_box);
		wmove(win, 4, 24);
		waddstr(win, "Searching...");
		wrefresh(win);
		mywbkgd(win, POPUP_WIN_ATTR);
		x=0;
		y=0;

		/* Для упрощения создания мобильного средства асинхронного */
		/* ввода с клавиатуры создадим отдельный процесс чтения ее */
		/* и записи в программный канал, а наличие данных в нем    */
		/* можно проверить по fstat()                              */
		/* все это делают kbstart(), kbhit(), kbget() и kbstop()            */
		/* работа по kbhit() слишком сильно ест процессорное время*/

		kbstart();

		if(!last_search){
		  mktemp(schname);
		  last_search=schname;
		}
		prot=fopen(schname, "w+");


		old_alarm=signal(SIGALRM, chkalarm);
		setalarm=alarm(0);
		stopfind=0;
		while(stopfind==0){
		    if(kbhit()){
		      switch(kbget())
			{
			 case K_ESC:
			   /* дочитать и вывести строку, если она есть*/
			   stbuf.st_size=0;
			   fstat(fd[0], &stbuf);
			   while(stopfind==0 && stbuf.st_size){
			     if(read(fd[0], &simb, 1)<=0) stopfind=1;
			     fstat(fd[0], &stbuf);
			     if(!stopfind){
			       if(simb=='\n') stopfind=1;
			       if(outprot) putc(simb, prot);
			       if(ferror(prot) || feof(prot)) outprot=0;
			       if(simb && (x<60 || simb=='\n')){
				 x++;
				 waddch(win, simb);
			       }
			     }
			   }
			   kill(pid, SIGINT);
			   stopfind=1;
			 break;
			 default:
			   beep();
			 break;
			}
		    }

		    was_alarm=0;
		    signal(SIGALRM, chkalarm);
		    alarm(1);

		    if(stopfind || read(fd[0], &simb, 1)<=0)
		      { /* конец файла или alarm */
			if(!was_alarm) stopfind=1; /* конец файла */
		      }else{ /* что-то прочиталось */
			if(outprot) putc(simb, prot);
			if(ferror(prot) || feof(prot)) outprot=0;
			switch(simb)
			  {
			    case '\n': /* конец строки от find */
#ifdef FREEBSD
			      /* в BSD - прходится прописывать строку до конца */
			      while(x++ < COLS_LST-2) waddch(win, ' ');
#else
			      waddch(win,simb);
#endif
			      y++;
			      x=0;
			      wrefresh(win);
			    break;

			    default:
			      if(x++<COLS_LST-3) waddch(win, simb);
			    break;
			  }
		      }
		    alarm(0);
		}
		signal(SIGALRM, old_alarm);
		alarm(setalarm);
		kbstop();
		close(fd[0]);
		wrefresh(win);

		do{
		 pid1=wait(NULL);
		}while(pid1!=pid && pid1!=(-1));

		stop++;

	      break;
	    }  /* switch */
	}
    }else if(find==6){
      /* на самом деле это делается не в цикле, а один раз */
      stop++;
      prot=fopen(schname, "r");

      if(nosave_ask==0){
	delsoob();
	overwrite(curscr, save_scr);
	overwrite(curscr, stdscr);
#ifdef FREEBSD
	touchwin(stdscr);
#endif

	clearok(stdscr,FALSE);
      }

      win_box=subwin(stdscr,LINES_LST,COLS_LST,BEGY_LST,BEGX_LST);
      tst_err_malloc(win_box, 14);
      mywbkgd(win_box, POPUP_WIN_ATTR);

      win=subwin(stdscr,LINES_LST-2,COLS_LST-2,BEGY_LST+1,BEGX_LST+1);
      tst_err_malloc(win_box, 15);
      scrollok(win, TRUE);
      mywbkgd(win, POPUP_WIN_ATTR);
    }else{
      stop++;
    }
}

if(prot){
  rewind(prot);
  fseek(prot, 0L, 2);
  if(ftell(prot) == 0){
    fclose(prot);
    prot=NULL;
    ask(&win74,1,0); /* сообщение об ошибке*/
  }
}

if(prot){ /* Просмотр/выбор */
  rewind(prot);
  lines=0;
  while(!feof(prot)){
    ch=getc(prot);
    if(ch=='\n') lines++;
  }
  scrollok(win, FALSE);

  mywbkgd(win_box, POPUP_WIN_ATTR);
  box(win_box, 0, 0);
  wrefresh(win_box);

  firstline=lines-(LINES_LST-2);
  if(firstline<0) firstline=0;
  curline=lines-1;

  stop=0;
  while(!stop){
    rewind(prot);
    mywbkgd(win, POPUP_WIN_ATTR);
    for(y=0;y<firstline;y++){
      while(feof(prot)==0 && getc(prot)!='\n');
    }
    /* дошли до начала выводимой части*/
    wmove(win,0,0);
    while(feof(prot)==0 && y<(firstline+LINES_LST-2)){
      x=0;
      if(y==curline)
	{ /* текущая строка */
	  wattrset(win, POPUP_WIN_OUTL_ATTR);
	  x=0;
	}else{
	  wattrset(win, POPUP_WIN_ATTR);
	}
      wmove(win, y-firstline, 0);
      do{
	ch=getc(prot);
	if(!feof(prot)){
	  if(x<COLS_LST-2 && ch!='\n') waddch(win, ch);
	  x++;
	}
      }while(ch!='\n' && !feof(prot));
      /* текущую строку нужно прописать до конца окна*/
#ifdef FREEBSD
      /* а в BSD - не только текущую */
      while(x++ < COLS_LST-1) waddch(win, ' ');
#else
      if(y==curline) while(x++ < COLS_LST-1) waddch(win, ' ');
#endif
      y++;
    }
    wrefresh(win);

    if(use_keycap)
      { /*использовать свою настройку на клавиатуру */
	ch=getkey_();
      }else{ /* использовать стандартные средства curses */
	ch=getch();
      }
    switch(ch)
      {
	case K_SD:  /*стрелка вниз*/
	case KEY_DOWN:  /*стрелка вниз*/
	  if(curline<lines-1){
	    curline++;
	    if(curline>firstline+LINES_LST-3) firstline++;
	  }
	break;


	case K_SU:    /*стрелка вверх*/
	case KEY_UP:    /*стрелка вверх*/
	  if(curline){
	    curline--;
	    if(curline<firstline) firstline--;
	  }
	break;

	case K_PAGE_UP:
	case KEY_PPAGE:
	  if(curline){
	    curline-=LINES_LST-2;
	    if(curline<0) curline=0;

	    firstline-=LINES_LST-2;
	    if(firstline<0) firstline=0;
	  }
	break;

	case K_PAGE_DN:
	case KEY_NPAGE:
	  if(curline<lines-1){
	    curline+=LINES_LST-2;
	    if(curline>lines-1) curline=lines-1;

	    firstline+=LINES_LST-2;
	    if(firstline>lines-(LINES_LST-2)) firstline=lines-(LINES_LST-2);
	    if(firstline<0) firstline=0;
	  }
	break;

	case K_HOME:
	case KEY_HOME:
	  curline=0;
	  firstline=0;
	break;

	case K_END:
	case KEY_END:
	    curline=lines-1;
	    firstline=lines-(LINES_LST-2);
	    if(firstline<0) firstline=0;
	break;


	case K_ESC:       /*конец работы*/
	case ESC:       /*конец работы*/
	case K_F0:
	case KEY_F(0):
	  stop++;
	break;

	case K_CR:        /*выбор пункта меню*/
	case K_LF:        /*выбор пункта меню*/
	case ENTER:
	  rewind(prot);
	  for(y=0;y<curline;y++){
	    while(feof(prot)==0 && getc(prot)!='\n');
	  }
	  stop=0;
	  for(x=0; !stop; x++){
	    if(x>=tmp_len){
	      tmp_len+=128;
	      tmp_str=realloc(tmp_str, tmp_len);
	      tst_err_malloc(tmp_str,18); /* выполняет exit, если NULL*/
	    }
	    tmp_str[x]=getc(prot);
	    if(tmp_str[x]=='\n' || feof(prot) || ferror(prot)){
	      tmp_str[x]='\0';
	      stop++;
	    }
	  }
	  if(stat(tmp_str, &stbuf)==0 && filtype(stbuf.st_mode)==FTYPE_DIR)
	    { /* найден каталог */
	      dirname=tmp_str;
	      filename=NULL; /* войдем в каталог и поставим указатель на .. */
	    }else{
	      for(x=0; tmp_str[x]; x++);
	      while(tmp_str[x]!='/' && x ) x--;
	      if(tmp_str[x]=='/')
		{ /* найден последний разделитель */
		  tmp_str[x]='\0';
		  dirname=tmp_str;
		  filename=tmp_str+x+1;
		}else{ /* нет разделителя, файл в текущем каталоге */
		  dirname=NULL;
		  filename=tmp_str;
		}
	    }
	  if(dirname){ /* есть каталог, в который нужно перейти*/
	    if(chdir(dirname)) beep();
	  }
	  outpanel=cpanel;
	  read_to_panel();
	  if(filename){ /* есть файл, на который нужно поставить указатель*/
	    for(nfile=0;
	       nfile<panels[cpanel].nfiles &&
	       strcmp(filename,panels[cpanel].names+panels[cpanel].itembuf[nfile].nname)
	       ;nfile++);

	    if(nfile<panels[cpanel].nfiles){ /* найден*/
	      panels[cpanel].curfile=nfile;
	      if(nfile>=(panels[cpanel].lines-panels[cpanel].lnused))
		{ /* файл - не на первой странице*/
		  panels[cpanel].firstfile=nfile-(panels[cpanel].lines-panels[cpanel].lnused)/2;
		  if(panels[cpanel].firstfile<0) panels[cpanel].firstfile=0;
		}else{ /* на первой странице*/
		  panels[cpanel].firstfile=0;
		}
	    }
	  }
	  stop++;
	break;
      }

  } /* while !stop */

  overwrite(save_scr, stdscr);
#ifdef FREEBSD
  touchwin(stdscr);
#endif
  outpanel=cpanel;
  refr_panel();
  overwrite(stdscr, save_scr); /* обман более верхних программ*/
  if(nosave_ask==0) refresh();
}
if(prot) fclose(prot);
if(win) delwin(win);
if(win_box) delwin(win_box);
}

