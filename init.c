#include <sys/utsname.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"

/****************************************************************************/
				init()
/*                      Инициализация curses настройка цветов,
			выделение памяти и формирование панелей
*****************************************************************************/
{
extern struct panel panels[];
extern  int     cpanel;         /* текущая панель*/
extern  int     outpanel;       /* выводимая панель */
extern  char    *fnd_str;       /* строка для временных данных */
extern  size_t  fnd_len;        /* место, выделенное для fnd_str*/
extern  char    *tmp_str;       /* строка для временных данных */
extern  size_t  tmp_len;        /* место, выделенное для tmp_str*/
extern  char    *inp_str;       /* строка для ввода команд */
extern  size_t  inp_len;        /* место, выделенное для inp_str*/
extern  char    *history;       /* буфер хранения истории */
extern  size_t  his_len;        /* место, выделенное для history*/
extern  int     inp_first;      /* номер первого выведенного символа команды */
extern  int     inp_cur;        /* номер текущего символа команды */
extern  int     his_cur;        /* номер элемента history для заполнения */
extern  char    *helpfile;      /* файл со справкой */

extern  WINDOW  *dn_menu;       /* нижнее меню */
extern  WINDOW  *inp_win;       /* окно ввода команд */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  WINDOW  *clock_win;     /* окно вывода времени */
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
extern  chtype  atr[];          /* раскраска */
extern struct fields fields[];  /* характеристики возможных полей вывода*/
extern  char    *keycapfile;    /* имя файла с описанием клавиатуры (формат TERMCAP)*/
extern  int     use_colors;     /* признак использования цветов */
extern  char    mach_name[SYS_NMLN];/* название машины */
extern  char    user_name[LOGNM_MAX]; /* имя пользователя */
extern  int     prompt;         /* тип приглашения:0-нет, 1-NodeName */
extern  uid_t   user_eid;       /* эффективный UserID или 0, если getuid()==0*/
extern  int     fromcode;       /* тип кодировки, из которой перекодировать при просмотре*/
extern  int     tocode;         /* тип кодировки, в которую перекодировать при просмотре*/
static  char    *dmenu="1+      - 2+      - 3+View  - 4+Edit  - 5+Copy  - 6+RenMov- 7+Mkdir - 8+Delete- 9+PullDn- 10+Quit ";
static  char    *defaultkeys = "/usr/local/lib/conix/keycap.conix";
	short   pair;
	char    *ptr;
	int     i;
	int     nfield;
	struct utsname utsname; /* для определения названия машины */
extern  char    *getlogin();    /* в SCO UNIX нет описания в stdlib.h */
extern  void    alrm_hand();    /* обработчик сигнала alarm */
static  char    *codes[] = {"ALT","ISO","KOI","MSWIN",NULL};

/*==========================================================================*/

if(!initscr()){
  fprintf(stderr, "Can't init.screen!\n");
  exit(-1);
}

/*
 * Установка обработки сигналов - после initscr() т.к. в некоторых
 * реализациях он выставляет свою обработку (напр. в FreeBSD)
 */

/* signal(SIGHUP, SIG_IGN); */
signal(SIGINT, SIG_IGN);
signal(SIGQUIT, SIG_IGN);
signal(SIGTRAP, SIG_IGN);
signal(SIGTERM, SIG_IGN);
signal(SIGTSTP, _exit);
signal(SIGSTOP, SIG_IGN);
signal(SIGALRM, alrm_hand);

keycapfile=getenv(KEYCAP);
if(!keycapfile){
  if(access(defaultkeys, R_OK)==0) keycapfile = defaultkeys;
}
if(!keycapfile) keycapfile=getenv("TERMCAP");

if(keycapfile)
  {
    use_keycap=1;
    setkey_(1);
  }else{
    keypad(stdscr, TRUE);
  }

/*---------------------- Таблицы кодировки "по умолчанию" ------------------*/
if ( ptr = getenv("CONIXTERMCODE") ) {
  for(i=0; ptr[i]; i++) ptr[i] = toupper(ptr[i]);

  for (i=0; codes[i] && strcmp(codes[i],ptr); i++);

  if (codes[i])
    { /* найдена кодировка терминала */
      tocode = i;
    }else{
      fprintf(stderr, "Bad CONIXTERMCODE (%s) Press Enter...", ptr);
      getchar();
      endwin();
      exit(-1);
    }
}

if ( ptr = getenv("CONIXFILESCODE") ) {
  for(i=0; ptr[i]; i++) ptr[i] = toupper(ptr[i]);

  for (i=0; codes[i] && strcmp(codes[i],ptr); i++);
  if (codes[i])
    { /* найдена кодировка терминала */
      fromcode = i;
    }else{
      fprintf(stderr, "Bad CONIXFILESCODE (%s) Press Enter...", ptr);
      getchar();
      endwin();
      exit(-1);
    }
}


if(!(save_scr=newwin(0,0,0,0))){
  fprintf(stderr, "Can't create save window!\n");
  exit(-1);
}

my_raw();
/*
raw();
cbreak();
noecho();
#ifdef NCURSES
system("stty raw");
#endif
*/

#ifdef NOCOLORCURSES
  { /* безусловно не вызовать фенкции цветного curses */
#else
start_color();
if(has_colors() && COLORS>=8 && !getenv("NOCOLORCONIX"))
  { /* терминал цветной, не меньше 8 цветов */
    /* if(can_change_color()) {
     * move(2,1);
     * addstr("And can change colors!");
     * }
     */

    use_colors=1;   /* признак использования цветов */
    pair=1;

    init_pair(pair, CYAN   ,BLUE   );
    MAIN_PANEL_ATTR          =
    MAIN_PANEL_BOX_ATTR      = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, WHITE  ,BLUE   );
    MAIN_PANEL_TOP_ATTR      =
    MAIN_PANEL_BOT_OUT_ATTR  =
    MAIN_PANEL_OUTL_ATTR     = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, BLACK  ,CYAN   );
    MAIN_PANEL_PTR_ATTR      =
    MAIN_PANEL_DIR_ATTR      =
    BOTT_LINE_TEXT_ATTR      =
    TOP_MENU_ATTR            =
    POPUP_WIN_OUTL_ATTR      =
    POPDN_WIN_BOX_ATTR       = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, BROWN  ,CYAN   );
    MAIN_PANEL_OUTL_PTR_ATTR =
    POPDN_WIN_ATTR           = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, WHITE  ,BLACK  );
    BOTT_LINE_KEYS_ATTR      =
    TOP_MENU_OUTL_ATTR       =
    SHADOW_ATTR              =
    POPDN_WIN_OUTL_ATTR      = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, BLACK  ,WHITE  );
    ERROR_WIN_OUTL_ATTR      =
    POPUP_WIN_ATTR           =
    POPUP_WIN_BOX_ATTR       = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, MAGENTA,CYAN   );
    POPDN_WIN_HDRS_ATTR      = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, WHITE  ,RED    );
    ERROR_WIN_ATTR           = COLOR_PAIR(pair);
    pair++;

    init_pair(pair, RED    ,BLACK  );
    SUPER_PROMPT_ATTR           = COLOR_PAIR(pair);
    pair++;

  }else{ /* черно-белый терминал */
#endif
    use_colors=0;   /* признак использования цветов */
    MAIN_PANEL_ATTR          = A_NORMAL  ;
    MAIN_PANEL_TOP_ATTR      = A_STANDOUT;
    MAIN_PANEL_BOT_OUT_ATTR  = A_NORMAL  ;
    MAIN_PANEL_PTR_ATTR      = A_STANDOUT;
    MAIN_PANEL_OUTL_ATTR     = A_NORMAL  ;
    MAIN_PANEL_OUTL_PTR_ATTR = A_STANDOUT;
    MAIN_PANEL_BOX_ATTR      = A_NORMAL  ;
    MAIN_PANEL_DIR_ATTR      = A_STANDOUT;
    BOTT_LINE_TEXT_ATTR      = A_STANDOUT;
    BOTT_LINE_KEYS_ATTR      = A_NORMAL  ;
    TOP_MENU_ATTR            = A_NORMAL  ;
    TOP_MENU_OUTL_ATTR       = A_STANDOUT;
    POPDN_WIN_ATTR           = A_NORMAL  ;
    POPDN_WIN_BOX_ATTR       = A_NORMAL  ;
    POPDN_WIN_OUTL_ATTR      = A_STANDOUT;
    POPDN_WIN_HDRS_ATTR      = A_STANDOUT;
    POPUP_WIN_ATTR           = A_NORMAL  ;
    POPUP_WIN_BOX_ATTR       = A_NORMAL  ;
    POPUP_WIN_OUTL_ATTR      = A_STANDOUT;
    SHADOW_ATTR              = A_NORMAL  ;
    ERROR_WIN_ATTR           = A_STANDOUT;
    ERROR_WIN_OUTL_ATTR      = A_NORMAL  ;
    SUPER_PROMPT_ATTR        = A_STANDOUT;
}

/* NO_DRAW_ATTR=~1; на самом деле сравниваются адреса */

prompt=2;         /* тип приглашения:0-нет, 1-NodeName 2-User@Node */
if(uname(&utsname)<0)
  { /* какие-то ошибки */
    strcpy(mach_name, "unknown?");
  }else{
    strcpy(mach_name, utsname.nodename);
  }
user_eid=geteuid();
if(getuid()==0) user_eid=0; /* user_eid используется для индикации root */
if(ptr=getlogin())
  {
    strncpy(user_name, ptr, sizeof(user_name));
    user_name[sizeof(user_name)-1]='\0';
  }else if(ptr=getenv("LOGNAME")) {
    strncpy(user_name, ptr, sizeof(user_name));
    user_name[sizeof(user_name)-1]='\0';
  }else if(ptr=getenv("USER")) {
    strncpy(user_name, ptr, sizeof(user_name));
    user_name[sizeof(user_name)-1]='\0';
  }else{
    sprintf(user_name, "?%d?", (int)getuid());
  }

inp_win=subwin(stdscr,1,COLS-1,LINES-2,0);
wattrset(inp_win, A_NORMAL);
leaveok(inp_win, FALSE);

clock_win=subwin(stdscr,1,5,0,COLS-5);
tst_err_malloc(clock_win,24); /* выполняет exit, если NULL*/
wattrset(clock_win, MAIN_PANEL_DIR_ATTR);
leaveok(clock_win, FALSE);


fnd_len=128;
fnd_str=malloc(fnd_len);
tst_err_malloc(fnd_str,12); /* выполняет exit, если NULL*/
fnd_str[0]='\0';

tmp_len=1024;
tmp_str=malloc(tmp_len);
tst_err_malloc(tmp_str,8); /* выполняет exit, если NULL*/
tmp_str[0]='\0';

inp_len=256;   /* his_len должен быть больше inp_len !! */
inp_str=malloc(inp_len);
tst_err_malloc(inp_str,11); /* выполняет exit, если NULL*/
for(inp_cur=0;inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';
inp_str[inp_len-1]='\0';
inp_cur=0;
inp_first=0;

his_len=1024;  /* his_len должен быть больше inp_len !! */
history=malloc(his_len);
tst_err_malloc(history,10); /* выполняет exit, если NULL*/
for(his_cur=0;his_cur<his_len;his_cur++) history[his_cur]=1;
his_cur=0;
history[0]=history[his_len-1]='\0';


/* Наличие файла справки
   !!! используется tmp_str[] и
       должен быть выделен буфер для работы с TERMCAP
 */
ptr=tmp_str;
if(use_keycap && t_tgetstr("hC",&ptr)!=0) { /*имя файла справки описано в TERMCAP*/
  if(access(tmp_str, R_OK)==0){ /*файл доступен */
    helpfile = malloc(ptr - tmp_str);
    tst_err_malloc(helpfile, 31);
    strcpy(helpfile, tmp_str);
  }
}
tmp_str[0]='\0';

if ( ! helpfile ) { /* в TERMCAP не найдено описание hC, попробуем поискать в стандартном месте*/
  helpfile = "/usr/local/lib/conix/conix.hlp";
  if(access(helpfile, R_OK)!=0) helpfile = NULL;
}

if(helpfile){ /* сформировать пункт нижнего меню Help */
  for(ptr="Help", i=2; *ptr ; ptr++, i++) dmenu[i] = *ptr;
}

/* теперь можно выводить нижнее меню */
dn_menu=newwin(1,COLS-1,LINES-1,0); /* newwin т.к. оно, может, будет выключаться*/
wattrset(dn_menu, BOTT_LINE_KEYS_ATTR);
for(ptr=dmenu;*ptr;ptr++){
  switch(*ptr)
    {
      case '+':
	wattrset(dn_menu, BOTT_LINE_TEXT_ATTR);
      break;
      case '-':
	wattrset(dn_menu, BOTT_LINE_KEYS_ATTR);
      break;
      default:
	waddch(dn_menu, *ptr);
      break;
    }
}

/* начальная инициализация структур */
for(outpanel=0; outpanel<NPANELS; outpanel++){
  panels[outpanel].lines=LINES-2;
  panels[outpanel].cols =COLS/NPANELS;
  panels[outpanel].win=subwin(stdscr, panels[outpanel].lines, panels[outpanel].cols, 0, outpanel*panels[outpanel].cols);
  tst_err_malloc(panels[outpanel].win,1); /* выполняет exit, если NULL*/
  wattrset(panels[outpanel].win, MAIN_PANEL_ATTR);
  mywbkgd(panels[outpanel].win, MAIN_PANEL_ATTR);

  panels[outpanel].save_on=panels[outpanel].on=1;
  /* panels[outpanel].curfile=panels[outpanel].firstfile=0; будет сделано в read_to_panel() */
  panels[outpanel].lnused=5;

  panels[outpanel].full_dir_size=128;
  panels[outpanel].full_dir=malloc(panels[outpanel].full_dir_size);
  tst_err_malloc(panels[outpanel].full_dir,2); /* выполняет exit, если NULL*/
  panels[outpanel].full_dir[0]='.';
  panels[outpanel].full_dir[1]='\0';

  panels[outpanel].nitems=100;
  panels[outpanel].itembuf=malloc(panels[outpanel].nitems*sizeof(struct itembuf));
  tst_err_malloc(panels[outpanel].itembuf,3); /* выполняет exit, если NULL*/

  panels[outpanel].nfiles=0;

  panels[outpanel].names_len=2048;
  panels[outpanel].names=malloc(panels[outpanel].names_len);
  tst_err_malloc(panels[outpanel].names,4); /* выполняет exit, если NULL*/

  panels[outpanel].firstfile=0;
  panels[outpanel].selected=0;
  panels[outpanel].size_sel=0L;
  panels[outpanel].sort=1;
  panels[outpanel].nfiles=0;
  panels[outpanel].lastfile=0;
  panels[outpanel].filter_read[0]='\0';
  panels[outpanel].filter_noread[0]='\0';

  clearok(panels[outpanel].win, FALSE);

  for(i=0; i<MAXFIELDS; i++) panels[outpanel].outfields[i]=i;
  read_to_panel();  /* extern outpanel */

}
read_setup();

/* read_setup() может изменить panels[outpanel].full_dir */
for(outpanel=0; outpanel<NPANELS; outpanel++){
 if(chdir(panels[outpanel].full_dir)) beep();
  read_to_panel();  /* extern outpanel */
}

refr_inp();
}
