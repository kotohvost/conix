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
/*                      ���室 � ��⠫�� �� ᯨ᪠
/****************************************************************************/
{
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  int     use_colors;     /* �ਧ��� �ᯮ�짮����� 梥⮢ */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  chtype  atr[];          /* ���᪠ */
extern  char    *tmp_str;       /* ��ப� ��� ���᪠ */
extern  size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
extern  struct  win     win52;
extern  struct  win     win55;
	FILE    *prot=NULL;     /* १����� find � 䠩��*/
	WINDOW  *win_box;       /* ���� - த�⥫� ��� win */
	WINDOW  *win;           /* ���� ࠡ��� � ᯨ᪮� ��������� 䠩���*/
 struct stat    stbuf;
	short   lines;          /* ������⢮ ��ப � ᯨ᪥ */
	int     stop=0;         /*�ਧ��� ������� ���᪮� */
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     x;
	int     y;
	int     ch;
	short   curline;        /* ����� ⥪�饩 ��ப� */
	short   firstline;      /* ����� ��ࢮ� ��ப� ����*/
	char    *dirname;       /* ��� ��⠫��� */
	char    *filename;      /* ��� 䠩��, �� ����� ���⠢���� 㪠��⥫�*/
static  char    name[]=".conix.goto";
	char    *home;          /* "����譨�" ��⠫�� ���짮��⥫�*/
/*==========================================================================*/
if(access(name, R_OK)==0)
  { /* 䠩� ���� � ⥪�饬 ��⠫��� */
    home=".";
  }else{
    home=getenv("HOME");
    if(!home) home=getenv("home");
  }
if((!home) || chdir(home))
  { /* ��� HOME ��� ������㯥� ����譨� ��⠫�� */
    ask(&win52, 2, 0);
  }else{ /* ����� �᪠�� 䠩� */
    if(!(prot=fopen(name, "r")))
      { /* �� ���뢠���� 䠩� */
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
	/* wrefresh(win_box); �� �� �㤥� �뢥���� �� refresh()*/
	delwin(win_box);

	if(use_colors){
	  /* ��ᮢ���� ⥭� */
	  /* ���⨪��쭠� ����*/
	  x=BEGX_LST+COLS_LST;
	  for(y=BEGY_LST+1; y<=BEGY_LST+LINES_LST; y++){
	      move(y,x);
	      ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	      addch(ch);
	      ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	      addch(ch);
	  }

	  /* ��ਧ��⠫쭠� ����*/
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
	  /* ��諨 �� ��砫� �뢮����� ���*/
	  wmove(win,0,0);
	  while(feof(prot)==0 && y<(firstline+LINES_LST-2)){
	    x=0;
	    if(y==curline)
	      { /* ⥪��� ��ப� */
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
	    /* ⥪���� ��ப� �㦭� �ய���� �� ���� ����*/
	    if(y==curline) while(x++ < COLS_LST-1) waddch(win, ' ');
	    y++;
	  }
	  wrefresh(win);

	  if(use_keycap)
	    { /*�ᯮ�짮���� ᢮� ����ன�� �� ���������� */
	      ch=getkey_();
	    }else{ /* �ᯮ�짮���� �⠭����� �।�⢠ curses */
	      ch=getch();
	    }
	  switch(ch)
	    {
	      case K_SD:  /*��५�� ����*/
	      case KEY_DOWN:  /*��५�� ����*/
		if(curline<lines-1){
		  curline++;
		  if(curline>firstline+LINES_LST-3) firstline++;
		}
	      break;


	      case K_SU:    /*��५�� �����*/
	      case KEY_UP:    /*��५�� �����*/
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


	      case K_ESC:       /*����� ࠡ���*/
	      case ESC:       /*����� ࠡ���*/
	      case K_F0:
	      case KEY_F(0):
		stop++;
	      break;

	      case K_CR:
	      case K_LF:
	      case ENTER: /* �롮� 䠩�� */
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
		    tst_err_malloc(tmp_str,18); /* �믮���� exit, �᫨ NULL*/
		  }
		  tmp_str[x]=getc(prot);
		  if(tmp_str[x]=='\n' || feof(prot) || ferror(prot)){
		    tmp_str[x]='\0';
		    stop++;
		  }
		}
		if(strcmp(tmp_str, " HOME DIRECTORY")==0){ /* ���室 � ����譨� ��⠫�� */
		  if(chdir(home)) beep();
		  outpanel=cpanel;
		  read_to_panel();
		}else if(strcmp(tmp_str, " OTHER PANEL DIRECTORY")==0){ /* ���室 � ��⠫�� ��⨢��������� ������*/
		  outpanel=(cpanel)?0:1;
		  if(chdir(panels[outpanel].full_dir)) beep();
		  outpanel=cpanel;
		  read_to_panel();
		}else{
		  if(stat(tmp_str, &stbuf)==0 && filtype(stbuf.st_mode)==FTYPE_DIR)
		    { /* ������ ��⠫�� */
		      dirname=tmp_str;
		      filename=NULL; /* ������ � ��⠫�� � ���⠢�� 㪠��⥫� �� .. */
		    }else{
		      for(x=0; tmp_str[x]; x++);
		      while(tmp_str[x]!='/' && x ) x--;
		      if(tmp_str[x]=='/')
			{ /* ������ ��᫥���� ࠧ����⥫� */
			  tmp_str[x]='\0';
			  dirname=tmp_str;
			  filename=tmp_str+x+1;
			}else{ /* ��� ࠧ����⥫�, 䠩� � ⥪�饬 ��⠫��� */
			  dirname=NULL;
			  filename=tmp_str;
			}
		    }
		  if(dirname){ /* ���� ��⠫��, � ����� �㦭� ��३�*/
		    if(chdir(dirname)) beep();
		  }
		  outpanel=cpanel;
		  read_to_panel();
		  if(filename){ /* ���� 䠩�, �� ����� �㦭� ���⠢��� 㪠��⥫�*/
		    for(nfile=0;
		       nfile<panels[cpanel].nfiles &&
		       strcmp(filename,panels[cpanel].names+panels[cpanel].itembuf[nfile].nname)
		       ;nfile++);

		    if(nfile<panels[cpanel].nfiles){ /* ������*/
		      panels[cpanel].curfile=nfile;
		      if(nfile>=(panels[cpanel].lines-panels[cpanel].lnused))
			{ /* 䠩� - �� �� ��ࢮ� ��࠭��*/
			  panels[cpanel].firstfile=nfile-(panels[cpanel].lines-panels[cpanel].lnused)/2;
			  if(panels[cpanel].firstfile<0) panels[cpanel].firstfile=0;
			}else{ /* �� ��ࢮ� ��࠭��*/
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
	overwrite(stdscr, save_scr); /* ����� ����� ���孨� �ணࠬ�*/
	if(nosave_ask==0) refresh();
      }
  }
}
