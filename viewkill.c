#include "defs.h"
#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#define FIRST   2       /* ����� ��ࢮ� ��ப� */
#define NONAME  (22+13) /* ���-�� �������, ���.�� ����� ���� ��.��� �����*/
/****************************************************************************/
				viewkill()
/*              ��ᬮ�� ᯨ᪠ ����ᮢ � ���뫪� ᨣ�����
/****************************************************************************/
{
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  int     use_colors;     /* �ਧ��� �ᯮ�짮����� 梥⮢ */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  chtype  atr[];          /* ���᪠ */
extern unsigned char *fnd_str;  /* ��ப� ��� �६����� ������ */
extern  size_t  fnd_len;        /* ����, �뤥������ ��� fnd_str*/
static  char    *dmenu="1+      - 2+      - 3+Next  - 4+      - 5+      - 6+User  - 7+Search- 8+Kill  - 9+SigNum- 10+Quit ";
static  char    *name="/tmp/ps_text_XXXXXX";
extern  struct  win     win71;  /* ���� � ����ᮬ */
extern  struct  win     win72;  /* ���� � �訡��� */
extern  struct  win     win73;  /* ����� ����� ���뫠����� ᨣ���� */
extern  struct  win     win75;  /* ����� ����� ���짮��⥫� */
extern  char    user_name[LOGNM_MAX]; /* ��� ���짮��⥫� */
extern  uid_t   user_eid;       /* ��䥪⨢�� UserID ��� 0, �᫨ getuid()==0*/
	FILE    *file=NULL;
	int     stop=0;         /*�ਧ��� ����砭�� ��ᬮ�� */
	int     ch;
	long    nextseek;       /* ᬥ饭�� �� ᫥���騩 �࠭*/
	int     x;
	int     y;
	int     x_beg=0;        /* ᤢ�� �뢮����� ��� ⥪�� */
	int     lines=LINES-3;    /* ������⢮ ��ப ��� �뢮�� */
	long    firstline=0;    /* ����� ��ࢮ� �뢮����� ��ப� 䠩��*/
	int     find;           /* �ਧ��� ���᪠ */
	long    srchlnseek;     /* ᬥ饭�� �� ��砫� ��ப� �� ���᪥*/
	long    srchseek;       /* ᬥ饭�� �� ᨬ���� ��᫥ ��ࢮ�� ᮢ�������*/
	long    srchseeklast=(-1);/* ᬥ饭�� �।��饣� ���᪠ */
	int     srchln;         /* ����� ��ப� �� ���᪥ */
	int     old_nosave;     /* �⠭�� ���祭�� nosave_ask */
	int     i;
	int     fd;             /* ���ਯ�� 䠩�� � ᯨ᪮� ����ᮢ */
	int     len;            /* ���ᨬ��쭠� ����� �뢮����� ��� �����*/
	int     need_refr=1;    /* �ਧ��� ����室����� ��ॢ뢮�� ᯨ᪠*/
	char    *ptr;           /* ��� 䠩�� ��� �뢮�� �訡�� � ��� ��室� �� 蠡���� */
 struct inp_lst inp_lst[2];     /* ᯨ᮪ ����� ��� inp_menu()*/
 struct inp_lst usr_inp_lst[2]; /* ᯨ᮪ ����� ��� inp_menu()*/
extern  long    linesback();    /* ���� ᬥ饭�� � 䠩�� �� ��᪮�쪮 ��ப ����� */
	long    *seeks=NULL;    /* ���ᨢ ᬥ饭�� � ᯨ᪥ ����ᮢ*/
	long    seeks_len=0;    /* ������⢮ ����⮢ � seeks */
	char    *stats=NULL;    /* �ਧ���� �⬥⪨ */
	int     nlines=1;       /* ������⢮ ��ப � 䠩�� ᯨ᪠ ����ᮢ*/
	int     curline=0;      /* ����� ⥪�饩 ��ப� */
	long    nproc;          /* ����� ����� */
static  int     sig=15;         /* ����� ᨣ���� */
static  char    usr_str[30]={'\0'};/* ��ப� � ������ ���짮��⥫� ��� ps(1)*/
	char    *tmp_first;     /* ��� ����������� ��ࢮ� ��ப�*/
	int     pos;            /* ��� ࠡ��� � tmp_first */
	int     next_srch=0;    /* �ਧ��� �த������� ���᪠ ��� ����� */
/*==========================================================================*/
tmp_first=malloc(COLS);
tst_err_malloc(tmp_first,27); /* �믮���� exit, �᫨ NULL*/

/* ����� ���짮��⥫� �� 㬮�砭�� �����뢠�� ⮫쪮 ᢮� */
if(user_eid && !usr_str[0]){
  strncpy(usr_str, user_name, sizeof(usr_str));
  usr_str[sizeof(usr_str)-1]='\0';
}

usr_inp_lst[0].str=usr_str;
/* usr_inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
usr_inp_lst[0].str_len=sizeof(usr_str);
usr_inp_lst[0].npunkt=1;
usr_inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

inp_lst[0].str=(char *)fnd_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

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
	case 0: /* ��஦����� �����*/
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
	    { /* ������ ��� ���짮��⥫� ��� ���ண� ᬮ������ ������ */
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


    if(!(file=fopen(name, "r"))) { /* �� ���뢠���� 䠩� */
	endwin();
	fprintf(stderr, "Can't open file %s\n", name);
	exit(-1);
    }

    i=1;
    pos=0;
    /* ����� ��ப� �⠥� � tmp_first */
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
      tst_err_malloc(seeks,25); /* �믮���� exit, �᫨ NULL*/
      stats=malloc((i-1)*sizeof(char));
      tst_err_malloc(stats,26); /* �믮���� exit, �᫨ NULL*/
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
    /* �஢���� ���४⭮��� ⥪�饩 ��ப� � ��ࢮ� ��ப� */
    if(curline>nlines-1){
      curline=nlines-1;
      if(curline<firstline) firstline=curline;
    }

  }

  mywbkgd(stdscr, MAIN_PANEL_ATTR);
  /* ��������� ��ᬮ�� */
  attrset(MAIN_PANEL_TOP_ATTR);
  move(0,0);
  if(x_beg){
    printw("  Shift: %d", x_beg);
  }
  if(firstline>=0)
    { /* �����⥭ ����� ��ࢮ� ��ப�*/
      printw(" Line: %d", curline+1);
    }else{
      printw(" Line: ?");
    }
  if(usr_str[0])
    { /* �����뢠���� ������ ��࠭���� ���짮��⥫�*/
      addstr("  User: ");
      addstr(usr_str);
    }else{
      addstr("  All users");
    }
  getyx(stdscr, y, x);
  while(x++<COLS) addch(' ');

  attrset(MAIN_PANEL_ATTR);


  /* �뢮� ��ࢮ� ��ப� */
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
    if(y-FIRST==curline-firstline){ /*������� ����� ⥪�饩 ��ப�*/
      if(x) x--; /* �� �뢮�� � ���� 横�� x++ */
      if(x<x_beg) { /* �� ��ப� �� ⠪�� ᬥ饭�� ᮢᥬ �� �뢮������*/
	x=x_beg;
      }
      x-=x_beg; /* ⥯��� �� ���न��� � ���� */
      move(y, x);
      while(x<COLS){
	addch(' ');
	x++;
      }
      attrset(MAIN_PANEL_ATTR);
    }
    y++;
  }

  /* ���ᮢ��� ��ப� � �����蠬� */
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
    { /* ⠪�� �뢠�� � Symlink �� ��⠫�� */
      ch=ESC;
    }else{
      if(use_keycap)
	{ /*�ᯮ�짮���� ᢮� ����ன�� �� ���������� */
	  ch=getkey_();
	}else{ /* �ᯮ�짮���� �⠭����� �।�⢠ curses */
	  ch=getch();
	}
    }
  switch(ch)
    {
      case K_SD:  /*��५�� ����*/
      case KEY_DOWN:  /*��५�� ����*/
	srchseeklast=(-1);
	if(curline<nlines-1){
	  curline++;
	  if(curline>firstline+lines-1) firstline++;
	}
      break;


      case K_SU:    /*��५�� �����*/
      case KEY_UP:    /*��५�� �����*/
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

      case K_SR:      /* ��५�� ��ࠢ� */
      case KEY_RIGHT: /* ��५�� ��ࠢ� */
	srchseeklast=(-1);
	x_beg++;
      break;

      case K_SL:      /* ��५�� ����� */
      case KEY_LEFT:  /* ��५�� ����� */
	srchseeklast=(-1);
	if(x_beg) x_beg--;
      break;

      case K_F6:      /* User */
      case KEY_F(6):
	if(inp_menu(usr_inp_lst, &win75, 1, 0)==1 && usr_str[0])
	  {
	    /* �㦭� ⮫쪮 usr_str[0] */
	  }else{
	    usr_str[0] = '\0';
	  }
	srchseeklast=(-1);
	curline=0;
	firstline=0;
	need_refr++;
      break;


      case K_F3:      /* �த������� ���᪠ */
      case KEY_F(3):
	next_srch=1;
      case K_F7:      /* ���� */
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
		if(srchseeklast<srchseek){  /* -1 ��� �।��騩 */
		  while(feof(file)==0 && *ptr && cmp_chrs(ch, *ptr, 0)==0 ){
		    ch=getc(file);
		    ptr++;
		  }
		}
		if(*ptr=='\0')
		  { /* ������ ᮮ⢥��⢨� */
		    find=1;
		    if(srchseek-srchlnseek>=COLS-2)
		      { /* ��ࠧ�� �� ����� �� �࠭� */
			x_beg=srchseek-srchlnseek-(COLS/2);
		      }else{
			x_beg=0;
		      }
		    srchseeklast=srchseek;
		    curline=srchln-1;
		    if(curline>firstline+lines-1 || curline<firstline) firstline=curline;

		  }else{ /* ��ᮮ⢥��⢨� � �����-� (�� ��ࢮ�) ᨬ����*/
		    fseek(file, srchseek, 0); /* �த������ ���� � ����, ��� ������ ᮢ���.ᨬ���*/
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

      case K_F8:      /* ���뫪� ᨣ���� */
      case KEY_F(8):
	need_refr++;

	fseek(file, seeks[curline], 0);

	do{ /* �ய����� ����騥 �஡���*/
	  ch=getc(file);
	}while(isspace(ch) && feof(file)==0 && ch!='\n');

	/* �ய����� ��� ���짮��⥫� */
	while(isspace(ch)==0 && feof(file)==0 && ch!='\n'){
	  ch=getc(file);
	}

	/* �ய����� �஡��� ��᫥ ����� */
	while(isspace(ch) && feof(file)==0 && ch!='\n'){
	  ch=getc(file);
	}

	if(isdigit(ch) && feof(file)==0 && ch!='\n')
	  { /* ����⢨⥫쭮 �����-� ����� */
	    fseek(file, -1L, 1); /* �������� �� ᨬ��� ����� */
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

      case K_F9:      /* ���뫪� ᨣ���� */
      case KEY_F(9):
	if((i=ask(&win73, sig, VERT))>0) sig=i;
      break;

      case K_ESC:       /*����� ࠡ���*/
      case ESC:         /*����� ࠡ���*/
      case K_F0:
      case KEY_F(0):
	stop++;
      break;

      case K_REFRESH: /* ������ ��ॢ뢮�� �࠭� */
      case REFRESH:   /* ������ ��ॢ뢮�� �࠭� */
	clearok(stdscr, TRUE);
      case REREAD:    /* ������ ������뢠��� ��⠫��� */
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
/* ��-�� �訡�� � curses ��� � ���ᠭ�� �ନ���� ���⪠ ��-�� �� ⥬ 梥⮬*/
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
