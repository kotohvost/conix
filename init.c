#include <sys/utsname.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "defs.h"

/****************************************************************************/
				init()
/*                      ���樠������ curses ����ன�� 梥⮢,
			�뤥����� ����� � �ନ஢���� �������
*****************************************************************************/
{
extern struct panel panels[];
extern  int     cpanel;         /* ⥪��� ������*/
extern  int     outpanel;       /* �뢮����� ������ */
extern  char    *fnd_str;       /* ��ப� ��� �६����� ������ */
extern  size_t  fnd_len;        /* ����, �뤥������ ��� fnd_str*/
extern  char    *tmp_str;       /* ��ப� ��� �६����� ������ */
extern  size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
extern  char    *inp_str;       /* ��ப� ��� ����� ������ */
extern  size_t  inp_len;        /* ����, �뤥������ ��� inp_str*/
extern  char    *history;       /* ���� �࠭���� ���ਨ */
extern  size_t  his_len;        /* ����, �뤥������ ��� history*/
extern  int     inp_first;      /* ����� ��ࢮ�� �뢥������� ᨬ���� ������� */
extern  int     inp_cur;        /* ����� ⥪�饣� ᨬ���� ������� */
extern  int     his_cur;        /* ����� ����� history ��� ���������� */
extern  char    *helpfile;      /* 䠩� � �ࠢ��� */

extern  WINDOW  *dn_menu;       /* ������ ���� */
extern  WINDOW  *inp_win;       /* ���� ����� ������ */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  WINDOW  *clock_win;     /* ���� �뢮�� �६��� */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  chtype  atr[];          /* ���᪠ */
extern struct fields fields[];  /* �ࠪ���⨪� ��������� ����� �뢮��*/
extern  char    *keycapfile;    /* ��� 䠩�� � ���ᠭ��� ���������� (�ଠ� TERMCAP)*/
extern  int     use_colors;     /* �ਧ��� �ᯮ�짮����� 梥⮢ */
extern  char    mach_name[SYS_NMLN];/* �������� ��設� */
extern  char    user_name[LOGNM_MAX]; /* ��� ���짮��⥫� */
extern  int     prompt;         /* ⨯ �ਣ��襭��:0-���, 1-NodeName */
extern  uid_t   user_eid;       /* ��䥪⨢�� UserID ��� 0, �᫨ getuid()==0*/
extern  int     fromcode;       /* ⨯ ����஢��, �� ���ன ��४���஢��� �� ��ᬮ��*/
extern  int     tocode;         /* ⨯ ����஢��, � ������ ��४���஢��� �� ��ᬮ��*/
static  char    *dmenu="1+      - 2+      - 3+View  - 4+Edit  - 5+Copy  - 6+RenMov- 7+Mkdir - 8+Delete- 9+PullDn- 10+Quit ";
static  char    *defaultkeys = "/usr/local/lib/conix/keycap.conix";
	short   pair;
	char    *ptr;
	int     i;
	int     nfield;
	struct utsname utsname; /* ��� ��।������ �������� ��設� */
extern  char    *getlogin();    /* � SCO UNIX ��� ���ᠭ�� � stdlib.h */
extern  void    alrm_hand();    /* ��ࠡ��稪 ᨣ���� alarm */
static  char    *codes[] = {"ALT","ISO","KOI","MSWIN",NULL};

/*==========================================================================*/

if(!initscr()){
  fprintf(stderr, "Can't init.screen!\n");
  exit(-1);
}

/*
 * ��⠭���� ��ࠡ�⪨ ᨣ����� - ��᫥ initscr() �.�. � ��������
 * ॠ������� �� ���⠢��� ᢮� ��ࠡ��� (����. � FreeBSD)
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

/*---------------------- ������� ����஢�� "�� 㬮�砭��" ------------------*/
if ( ptr = getenv("CONIXTERMCODE") ) {
  for(i=0; ptr[i]; i++) ptr[i] = toupper(ptr[i]);

  for (i=0; codes[i] && strcmp(codes[i],ptr); i++);

  if (codes[i])
    { /* ������� ����஢�� �ନ���� */
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
    { /* ������� ����஢�� �ନ���� */
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
  { /* ����᫮��� �� �맮���� 䥭�樨 梥⭮�� curses */
#else
start_color();
if(has_colors() && COLORS>=8 && !getenv("NOCOLORCONIX"))
  { /* �ନ��� 梥⭮�, �� ����� 8 梥⮢ */
    /* if(can_change_color()) {
     * move(2,1);
     * addstr("And can change colors!");
     * }
     */

    use_colors=1;   /* �ਧ��� �ᯮ�짮����� 梥⮢ */
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

  }else{ /* �୮-���� �ନ��� */
#endif
    use_colors=0;   /* �ਧ��� �ᯮ�짮����� 梥⮢ */
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

/* NO_DRAW_ATTR=~1; �� ᠬ�� ���� �ࠢ�������� ���� */

prompt=2;         /* ⨯ �ਣ��襭��:0-���, 1-NodeName 2-User@Node */
if(uname(&utsname)<0)
  { /* �����-� �訡�� */
    strcpy(mach_name, "unknown?");
  }else{
    strcpy(mach_name, utsname.nodename);
  }
user_eid=geteuid();
if(getuid()==0) user_eid=0; /* user_eid �ᯮ������ ��� ������樨 root */
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
tst_err_malloc(clock_win,24); /* �믮���� exit, �᫨ NULL*/
wattrset(clock_win, MAIN_PANEL_DIR_ATTR);
leaveok(clock_win, FALSE);


fnd_len=128;
fnd_str=malloc(fnd_len);
tst_err_malloc(fnd_str,12); /* �믮���� exit, �᫨ NULL*/
fnd_str[0]='\0';

tmp_len=1024;
tmp_str=malloc(tmp_len);
tst_err_malloc(tmp_str,8); /* �믮���� exit, �᫨ NULL*/
tmp_str[0]='\0';

inp_len=256;   /* his_len ������ ���� ����� inp_len !! */
inp_str=malloc(inp_len);
tst_err_malloc(inp_str,11); /* �믮���� exit, �᫨ NULL*/
for(inp_cur=0;inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';
inp_str[inp_len-1]='\0';
inp_cur=0;
inp_first=0;

his_len=1024;  /* his_len ������ ���� ����� inp_len !! */
history=malloc(his_len);
tst_err_malloc(history,10); /* �믮���� exit, �᫨ NULL*/
for(his_cur=0;his_cur<his_len;his_cur++) history[his_cur]=1;
his_cur=0;
history[0]=history[his_len-1]='\0';


/* ����稥 䠩�� �ࠢ��
   !!! �ᯮ������ tmp_str[] �
       ������ ���� �뤥��� ���� ��� ࠡ��� � TERMCAP
 */
ptr=tmp_str;
if(use_keycap && t_tgetstr("hC",&ptr)!=0) { /*��� 䠩�� �ࠢ�� ���ᠭ� � TERMCAP*/
  if(access(tmp_str, R_OK)==0){ /*䠩� ����㯥� */
    helpfile = malloc(ptr - tmp_str);
    tst_err_malloc(helpfile, 31);
    strcpy(helpfile, tmp_str);
  }
}
tmp_str[0]='\0';

if ( ! helpfile ) { /* � TERMCAP �� ������� ���ᠭ�� hC, ���஡㥬 ���᪠�� � �⠭���⭮� ����*/
  helpfile = "/usr/local/lib/conix/conix.hlp";
  if(access(helpfile, R_OK)!=0) helpfile = NULL;
}

if(helpfile){ /* ��ନ஢��� �㭪� ������� ���� Help */
  for(ptr="Help", i=2; *ptr ; ptr++, i++) dmenu[i] = *ptr;
}

/* ⥯��� ����� �뢮���� ������ ���� */
dn_menu=newwin(1,COLS-1,LINES-1,0); /* newwin �.�. ���, �����, �㤥� �몫������*/
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

/* ��砫쭠� ���樠������ ������� */
for(outpanel=0; outpanel<NPANELS; outpanel++){
  panels[outpanel].lines=LINES-2;
  panels[outpanel].cols =COLS/NPANELS;
  panels[outpanel].win=subwin(stdscr, panels[outpanel].lines, panels[outpanel].cols, 0, outpanel*panels[outpanel].cols);
  tst_err_malloc(panels[outpanel].win,1); /* �믮���� exit, �᫨ NULL*/
  wattrset(panels[outpanel].win, MAIN_PANEL_ATTR);
  mywbkgd(panels[outpanel].win, MAIN_PANEL_ATTR);

  panels[outpanel].save_on=panels[outpanel].on=1;
  /* panels[outpanel].curfile=panels[outpanel].firstfile=0; �㤥� ᤥ���� � read_to_panel() */
  panels[outpanel].lnused=5;

  panels[outpanel].full_dir_size=128;
  panels[outpanel].full_dir=malloc(panels[outpanel].full_dir_size);
  tst_err_malloc(panels[outpanel].full_dir,2); /* �믮���� exit, �᫨ NULL*/
  panels[outpanel].full_dir[0]='.';
  panels[outpanel].full_dir[1]='\0';

  panels[outpanel].nitems=100;
  panels[outpanel].itembuf=malloc(panels[outpanel].nitems*sizeof(struct itembuf));
  tst_err_malloc(panels[outpanel].itembuf,3); /* �믮���� exit, �᫨ NULL*/

  panels[outpanel].nfiles=0;

  panels[outpanel].names_len=2048;
  panels[outpanel].names=malloc(panels[outpanel].names_len);
  tst_err_malloc(panels[outpanel].names,4); /* �믮���� exit, �᫨ NULL*/

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

/* read_setup() ����� �������� panels[outpanel].full_dir */
for(outpanel=0; outpanel<NPANELS; outpanel++){
 if(chdir(panels[outpanel].full_dir)) beep();
  read_to_panel();  /* extern outpanel */
}

refr_inp();
}
