#include "defs.h"
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#define FIRST   2       /* номер первой строки */
#define NONAME  (22+13) /* кол-во колонок, кот.не могут быть исп.для имени*/
/****************************************************************************/
				viewkill()
/*              Просмотр списка процессов и посылка сигналов
/****************************************************************************/
{
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
extern  int     use_colors;     /* признак использования цветов */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  chtype  atr[];          /* раскраска */
extern unsigned char *fnd_str;  /* строка для временных данных */
extern  size_t  fnd_len;        /* место, выделенное для fnd_str*/
static  char    *dmenu="1+      - 2+      - 3+Next  - 4+      - 5+      - 6+User  - 7+Search- 8+Kill  - 9+SigNum- 10+Quit ";
static  char    *name="/tmp/ps_text_XXXXXX";
extern  struct  win     win71;  /* окно с запросом */
extern  struct  win     win72;  /* окно с ошибкой */
extern  struct  win     win73;  /* запрос номера посылаемого сигнала */
extern  struct  win     win75;  /* запрос имени пользователя */
extern  char    user_name[LOGNM_MAX]; /* имя пользователя */
extern  uid_t   user_eid;       /* эффективный UserID или 0, если getuid()==0*/
	FILE    *file=NULL;
	int     stop=0;         /*признак окончания просмотра */
	int     ch;
	long    nextseek;       /* смещение на следующий экран*/
	int     x;
	int     y;
	int     x_beg=0;        /* сдвиг выводимой части текста */
	int     lines=LINES-3;    /* количество строк для вывода */
	long    firstline=0;    /* номер первой выводимой строки файла*/
	int     find;           /* признак поиска */
	long    srchlnseek;     /* смещение до начала строки при поиске*/
	long    srchseek;       /* смещение до символа после первого совпадения*/
	long    srchseeklast=(-1);/* смещение предыдущего поиска */
	int     srchln;         /* номер строки при поиске */
	int     old_nosave;     /* станое значение nosave_ask */
	int     i;
	int     fd;             /* дескриптор файла со списком процессов */
	int     len;            /* максимальная длина выводимой части имени*/
	int     need_refr=1;    /* признак необходимости перевывода списка*/
	char    *ptr;           /* имя файла для вывода ошибки и для прохода по шаблону */
 struct inp_lst inp_lst[2];     /* список ввода для inp_menu()*/
 struct inp_lst usr_inp_lst[2]; /* список ввода для inp_menu()*/
extern  long    linesback();    /* поиск смещения в файле на несколько строк назад */
	long    *seeks=NULL;    /* массив смещений в списке процессов*/
	long    seeks_len=0;    /* количество элементов в seeks */
	char    *stats=NULL;    /* признаки отметки */
	int     nlines=1;       /* количество строк в файле списка процессов*/
	int     curline=0;      /* номер текущей строки */
	long    nproc;          /* номер процесса */
static  int     sig=15;         /* номер сигнала */
static  char    usr_str[30]={'\0'};/* строка с именем пользователя для ps(1)*/
	char    *tmp_first;     /* для запоминания первой строки*/
	int     pos;            /* для работы с tmp_first */
	int     next_srch=0;    /* признак продолжения поиска без запроса */
/*==========================================================================*/
tmp_first=malloc(COLS);
tst_err_malloc(tmp_first,27); /* выполняет exit, если NULL*/

/* обычным пользователям по умолчанию показывать только свои */
if(user_eid && !usr_str[0]){
  strncpy(usr_str, user_name, sizeof(usr_str));
  usr_str[sizeof(usr_str)-1]='\0';
}

usr_inp_lst[0].str=usr_str;
/* usr_inp_lst[0].fld_len=...;  выставит inp_menu()*/
usr_inp_lst[0].str_len=sizeof(usr_str);
usr_inp_lst[0].npunkt=1;
usr_inp_lst[1].str=NULL; /* признак конца списка для inp_menu()*/

inp_lst[0].str=(char *)fnd_str;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* признак конца списка для inp_menu()*/

mktemp(name);

old_nosave=nosave_ask;
nosave_ask=1;
if(old_nosave==0){
  delsoob();
  overwrite(curscr, save_scr);
}

attrset(MAIN_PANEL_ATTR);
mywbkgd(stdscr, MAIN_PANEL_ATTR);
move(0,0);
addch(' ');
refresh();
clear();

#ifdef NCURSES
mywbkgd(stdscr, MAIN_PANEL_ATTR);
#endif

move(0,0);
addch(' ');

attrset(MAIN_PANEL_ATTR);
mywbkgd(stdscr, MAIN_PANEL_ATTR);
refresh();

len=strlen(name);
stop=0;
while(!stop){
  if(need_refr){
    need_refr=0;
    if(file) fclose(file);
    switch(fork())
      {
	case -1:
	  endwin();
	  fprintf(stderr, "Can't fork\n");
	  exit(-1);
	break;
	case 0: /* порожденный процесс*/
	  fd=creat(name, 0777);
	  if(fd<=0){
	    endwin();
	    fprintf(stderr, "Can't create file %s\n", name);
	    exit(-1);
	  }
	  dup2(fd, 1);
	  dup2(fd, 2);
#ifdef FREEBSD
	      execlp("ps", "ps", "-awwxo", "user pid ppid stat start tty command", NULL);
#else
	  if(usr_str[0])
	    { /* задано имя пользователя для которого смотрятся процессы */
	      execlp("ps", "ps", "-f", "-u", usr_str, NULL);
	    }else{
	      execlp("ps", "ps", "-ef", NULL);
	    }
#endif
	  fprintf(stderr, "Can't exec to ps\n");
	  exit(-1);
	break;

	default:
	  wait(NULL);
	break;
      }


    if(!(file=fopen(name, "r"))) { /* не открывается файл */
	endwin();
	fprintf(stderr, "Can't open file %s\n", name);
	exit(-1);
    }

    i=1;
    pos=0;
    /* первую строку читаем в tmp_first */
    do{
      if( (tmp_first[pos] = getc(file)) == '\n')
	{
	  i++;
	}else{
	  if(pos<COLS-1 && !feof(file)) pos++;
	}
    }while(i==1 && !feof(file));
    tmp_first[pos] = '\0';

    while(!feof(file)){
      if(getc(file)=='\n') i++;
    }
    nlines=i-2;

    if(i>seeks_len){
      if(seeks_len){
	free(seeks);
	free(stats);
      }
      seeks_len=i-1;
      seeks=malloc((i-1)*sizeof(long));
      tst_err_malloc(seeks,25); /* выполняет exit, если NULL*/
      stats=malloc((i-1)*sizeof(char));
      tst_err_malloc(stats,26); /* выполняет exit, если NULL*/
    }
    rewind(file);
    i=0;
    while(!feof(file)){
      if(getc(file)=='\n'){
	seeks[i]=ftell(file);
	stats[i]='\0';
	i++;
      }
    }
    /* проверить корректность текущей строки и первой строки */
    if(curline>nlines-1){
      curline=nlines-1;
      if(curline<firstline) firstline=curline;
    }

  }

  mywbkgd(stdscr, MAIN_PANEL_ATTR);
  /* Заголовок просмотра */
  attrset(MAIN_PANEL_TOP_ATTR);
  move(0,0);
  if(x_beg){
    printw("  Shift: %d", x_beg);
  }
  if(firstline>=0)
    { /* известен номер первой строки*/
      printw(" Line: %d", curline+1);
    }else{
      printw(" Line: ?");
    }
  if(usr_str[0])
    { /* показываются процессы выбранного пользователя*/
      addstr("  User: ");
      addstr(usr_str);
    }else{
      addstr("  All users");
    }
  getyx(stdscr, y, x);
  while(x++<COLS) addch(' ');

  attrset(MAIN_PANEL_ATTR);


  /* вывод первой строки */
  move(FIRST-1, 0);
  addstr(tmp_first);

  fseek(file, seeks[firstline], 0);

  y=FIRST;

  while(feof(file)==0 && ferror(file)==0 && y<FIRST+lines){
    x=0;
    if(y-FIRST==curline-firstline) attrset(MAIN_PANEL_PTR_ATTR);
    do{
      ch=getc(file);
      if(feof(file)==0 && ferror(file)==0){
	if((x-x_beg)==COLS)
	  {
	    attrset(MAIN_PANEL_PTR_ATTR);
	    move(y, COLS-1);
	    addch('>');
	    attrset(MAIN_PANEL_ATTR);
	  }else{
	    if(ch=='\t'){
	      x=((x+8)/8)*8-1;
	      ch=' ';
	    }
	    if((x-x_beg)<COLS && ch!='\n'){
	      if(x>=x_beg){
		move(y, (x-x_beg));
		if(ch<' ' || ch==0177)
		  {
		    attrset(MAIN_PANEL_PTR_ATTR);
		    ch=CTRL(ch) +'@';
		    addch(ch);
		    attrset(MAIN_PANEL_ATTR);
		  }else{
		    addch(ch);
		  }
	      }
	    }
	  }
	x++;
      }
    }while(ch!='\n' && feof(file)==0 && ferror(file)==0);
    if(y-FIRST==curline-firstline){ /*дописать конец текущей строки*/
      if(x) x--; /* при выводе в конце цикла x++ */
      if(x<x_beg) { /* эта строка при таком смещении совсем не выводилась*/
	x=x_beg;
      }
      x-=x_beg; /* теперь это координата в окне */
      move(y, x);
      while(x<COLS){
	addch(' ');
	x++;
      }
      attrset(MAIN_PANEL_ATTR);
    }
    y++;
  }

  /* нарисовать строку с клавишами */
  move(LINES-1, 0);
  attrset(BOTT_LINE_KEYS_ATTR);
  for(ptr=dmenu;*ptr;ptr++){
    switch(*ptr)
      {
	case '+':
	  attrset(BOTT_LINE_TEXT_ATTR);
	break;
	case '-':
	  attrset(BOTT_LINE_KEYS_ATTR);
	break;
	default:
	  addch(*ptr);
	break;
      }
  }

  refresh();

  if(ferror(file))
    { /* такое бывает с Symlink на каталог */
      ch=ESC;
    }else{
      if(use_keycap)
	{ /*использовать свою настройку на клавиатуру */
	  ch=getkey_();
	}else{ /* использовать стандартные средства curses */
	  ch=getch();
	}
    }
  switch(ch)
    {
      case K_SD:  /*стрелка вниз*/
      case KEY_DOWN:  /*стрелка вниз*/
	srchseeklast=(-1);
	if(curline<nlines-1){
	  curline++;
	  if(curline>firstline+lines-1) firstline++;
	}
      break;


      case K_SU:    /*стрелка вверх*/
      case KEY_UP:    /*стрелка вверх*/
	srchseeklast=(-1);
	if(curline>0) curline--;
	if(curline<firstline) firstline--;
      break;

      case K_PAGE_UP:
      case KEY_PPAGE:
	srchseeklast=(-1);
	if(curline>0){
	  curline-=lines-2;
	  if(curline<0) curline=0;

	  firstline-=lines-2;
	  if(firstline<0) firstline=0;
	}
      break;

      case K_PAGE_DN:
      case KEY_NPAGE:
	srchseeklast=(-1);
	if(curline<nlines-1){
	  curline+=lines-2;
	  if(curline>nlines-1) curline=nlines-1;

	  firstline+=lines-2;
	  if(firstline>nlines-(lines-2)) firstline=nlines-(lines-2);
	  if(firstline<0) firstline=0;
	}
      break;

      case K_HOME:
      case KEY_HOME:
	srchseeklast=(-1);
	curline=0;
	firstline=0;
      break;

      case K_END:
      case KEY_END:
	  srchseeklast=(-1);
	  curline=nlines-1;
	  firstline=nlines-(lines-2);
	  if(firstline<0) firstline=0;
      break;

      case K_SR:      /* Стрелка вправо */
      case KEY_RIGHT: /* Стрелка вправо */
	srchseeklast=(-1);
	x_beg++;
      break;

      case K_SL:      /* Стрелка влево */
      case KEY_LEFT:  /* Стрелка влево */
	srchseeklast=(-1);
	if(x_beg) x_beg--;
      break;

      case K_F6:      /* User */
      case KEY_F(6):
	if(inp_menu(usr_inp_lst, &win75, 1, 0)==1 && usr_str[0])
	  {
	    /* нужно только usr_str[0] */
	  }else{
	    usr_str[0] = '\0';
	  }
	srchseeklast=(-1);
	curline=0;
	firstline=0;
	need_refr++;
      break;


      case K_F3:      /* продолжение поиска */
      case KEY_F(3):
	next_srch=1;
      case K_F7:      /* поиск */
      case KEY_F(7):
	if( (next_srch && fnd_str[0]) ||
	    (inp_menu(inp_lst, &win71, 1, 0)==1 && fnd_str[0])){

	  srchlnseek=seeks[curline];
	  srchln=curline+1;
	  find=0;
	  fseek(file, srchlnseek, 0);
	  while(feof(file)==0 && ferror(file)==0 && find==0){
	    ch=getc(file);
	    if(!feof(file)){

	      if(ch=='\n'){
		srchln++;
		srchlnseek=ftell(file);
	      }
	      if( cmp_chrs(ch, fnd_str[0], 0)==0 ){
		srchseek=ftell(file);
		ptr=(char*)fnd_str;
		if(srchseeklast<srchseek){  /* -1 или предыдущий */
		  while(feof(file)==0 && *ptr && cmp_chrs(ch, *ptr, 0)==0 ){
		    ch=getc(file);
		    ptr++;
		  }
		}
		if(*ptr=='\0')
		  { /* полное соответствие */
		    find=1;
		    if(srchseek-srchlnseek>=COLS-2)
		      { /* образец не виден на экране */
			x_beg=srchseek-srchlnseek-(COLS/2);
		      }else{
			x_beg=0;
		      }
		    srchseeklast=srchseek;
		    curline=srchln-1;
		    if(curline>firstline+lines-1 || curline<firstline) firstline=curline;

		  }else{ /* несоответствие в каком-то (не первом) символе*/
		    fseek(file, srchseek, 0); /* продолжить поиск с места, где найден совпад.символ*/
		  }
	      }
	    }
	  }
	  if(!find){
	    ask(&win72, 2, 0);
	  }
	}
	next_srch=0;
      break;

      case K_F8:      /* посылка сигнала */
      case KEY_F(8):
	need_refr++;

	fseek(file, seeks[curline], 0);

	do{ /* пропустить ведущие пробелы*/
	  ch=getc(file);
	}while(isspace(ch) && feof(file)==0 && ch!='\n');

	/* пропустить имя пользователя */
	while(isspace(ch)==0 && feof(file)==0 && ch!='\n'){
	  ch=getc(file);
	}

	/* пропустить пробелы после имени */
	while(isspace(ch) && feof(file)==0 && ch!='\n'){
	  ch=getc(file);
	}

	if(isdigit(ch) && feof(file)==0 && ch!='\n')
	  { /* действительно какой-то номер */
	    fseek(file, -1L, 1); /* вернуться на символ назад */
	    nproc=(-1);
	    fscanf(file,"%ld", &nproc);
	    if(nproc>=0 && nproc!=getpid())
	      {
		if(kill((pid_t)nproc, sig)) beep();
	      }else{
		beep();
		beep();
		beep();
	      }
	  }else{
	    beep();
	    beep();
	    beep();
	  }
      break;

      case K_F9:      /* посылка сигнала */
      case KEY_F(9):
	if((i=ask(&win73, sig, VERT))>0) sig=i;
      break;

      case K_ESC:       /*конец работы*/
      case ESC:         /*конец работы*/
      case K_F0:
      case KEY_F(0):
	stop++;
      break;

      case K_REFRESH: /* клавиша перевывода экрана */
      case REFRESH:   /* клавиша перевывода экрана */
	clearok(stdscr, TRUE);
      case REREAD:    /* клавиша пересчитывания каталога */
      case K_REREAD:
	need_refr++;
      break;

      default:
	beep();
      break;
    }

} /* while !stop */

fclose(file);
unlink(name);

mywbkgd(stdscr, A_NORMAL);
wattrset(stdscr, A_NORMAL);
/* #ifdef sun */
/* из-зи ошибок в curses или в описании терминала очистка вып-ся не тем цветом*/
move(0,0);
addch(' ');
refresh();
/* #endif */
clear();

#ifdef NCURSES
mywbkgd(stdscr, MAIN_PANEL_ATTR);
#endif

refresh();
overwrite(save_scr, stdscr);
#ifdef FREEBSD
touchwin(stdscr);
#endif
clearok(stdscr, TRUE);
refresh();
if(old_nosave==0) nosave_ask=0;

if(seeks_len){
  free(seeks);
  free(stats);
}
free(tmp_first);
}
