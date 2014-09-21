#include "defs.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#define LINES_LST 10
#define COLS_LST  60
#define BEGY_LST  ((LINES-LINES_LST)/2)
#define BEGX_LST  ((COLS-COLS_LST)/2)

/****************************************************************************/
				go_to()
/*                      Переход в каталог из списка
/****************************************************************************/
{
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
extern  int     use_colors;     /* признак использования цветов */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  chtype  atr[];          /* раскраска */
extern  char    *tmp_str;       /* строка для поиска */
extern  size_t  tmp_len;        /* место, выделенное для tmp_str*/
extern  struct  win     win52;
extern  struct  win     win55;
	FILE    *prot=NULL;     /* результаты find в файле*/
	WINDOW  *win_box;       /* окно - родитель для win */
	WINDOW  *win;           /* окно работы со списком найденных файлов*/
 struct stat    stbuf;
	short   lines;          /* количество строк в списке */
	int     stop=0;         /*признак повторных поисков */
	int     nfile;          /* номер текущего файла*/
	int     x;
	int     y;
	int     ch;
	short   curline;        /* номер текущей строки */
	short   firstline;      /* номер первой строки окна*/
	char    *dirname;       /* имя каталога */
	char    *filename;      /* имя файла, на который поставитть указатель*/
static  char    name[]=".conix.goto";
	char    *home;          /* "домашний" каталог пользователя*/
/*==========================================================================*/
if(access(name, R_OK)==0)
  { /* файл есть в текущем каталоге */
    home=".";
  }else{
    home=getenv("HOME");
    if(!home) home=getenv("home");
  }
if((!home) || chdir(home))
  { /* нет HOME или недоступен домашний каталог */
    ask(&win52, 2, 0);
  }else{ /* можно искать файл */
    if(!(prot=fopen(name, "r")))
      { /* не открывается файл */
	ask(&win55, 2, 0);
      }else{
	if(nosave_ask==0){
	  delsoob();
	  overwrite(curscr, save_scr);
	  overwrite(curscr, stdscr);
	  clearok(stdscr,FALSE);
	}

	win_box=subwin(stdscr,LINES_LST,COLS_LST,BEGY_LST,BEGX_LST);
	tst_err_malloc(win_box, 20);
	mywbkgd(win_box, POPUP_WIN_ATTR);
	box(win_box, 0, 0);
	/* wrefresh(win_box); все это будет выведено при refresh()*/
	delwin(win_box);

	if(use_colors){
	  /* Рисование тени */
	  /* вертикальная часть*/
	  x=BEGX_LST+COLS_LST;
	  for(y=BEGY_LST+1; y<=BEGY_LST+LINES_LST; y++){
	      move(y,x);
	      ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	      addch(ch);
	      ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	      addch(ch);
	  }

	  /* горизонтальная часть*/
	  y=BEGY_LST+LINES_LST;
	  for(x=BEGX_LST+2; x<=BEGX_LST+COLS_LST+1; x++){
	      move(y,x);
	      ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	      addch(ch);
	  }
	}

	touchwin(stdscr);
	refresh();

	win=subwin(stdscr,LINES_LST-2,COLS_LST-2,BEGY_LST+1,BEGX_LST+1);
	tst_err_malloc(win_box, 21);
	mywbkgd(win, POPUP_WIN_ATTR);

	lines=0;
	while(!feof(prot)){
	  ch=getc(prot);
	  if(ch=='\n') lines++;
	}

	firstline=0;
	curline=0;

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
	    if(y==curline) while(x++ < COLS_LST-1) waddch(win, ' ');
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

	      case K_CR:
	      case K_LF:
	      case ENTER: /* выбор файла */
		if(panels[cpanel].on<0) panels[cpanel].on *= -1;
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
		if(strcmp(tmp_str, " HOME DIRECTORY")==0){ /* переход в домашний каталог */
		  if(chdir(home)) beep();
		  outpanel=cpanel;
		  read_to_panel();
		}else if(strcmp(tmp_str, " OTHER PANEL DIRECTORY")==0){ /* переход в каталог противоположной панели*/
		  outpanel=(cpanel)?0:1;
		  if(chdir(panels[outpanel].full_dir)) beep();
		  outpanel=cpanel;
		  read_to_panel();
		}else{
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
		}
		stop++;
	      break;
	    }

	} /* while !stop */

	delwin(win);

	fclose(prot);

	overwrite(save_scr, stdscr);
#ifdef FREEBSD
	touchwin(stdscr);
#endif
	outpanel=cpanel;
	refr_panel();
	overwrite(stdscr, save_scr); /* обман более верхних программ*/
	if(nosave_ask==0) refresh();
      }
  }
}
