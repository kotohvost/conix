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
/*              ���⠢����� �ਧ���� ���뢠��� �� alarm
*****************************************************************************/
	int     arg;    /* �⮡� �� �㣠��� cc �� sun */
{
was_alarm=1;
}


/****************************************************************************/
				findfile()
/*                              ���� 䠩��
/****************************************************************************/
{
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  chtype  atr[];          /* ���᪠ */
extern  struct  win     win3;   /* ���� ᮮ�饭�� �� �訡��*/
extern  struct  win     win42;  /* ���� ����� 蠡���� ���᪠*/
extern  struct  win     win43;  /* ᮮ�饭�� - �� 㪠��� ��⠫�� ��砫� ���᪠*/
extern  struct  win     win44;  /* ᮮ�饭�� �� �訡�� ���᪠*/
extern  struct  win     win74;  /* ᮮ�饭�� �� �訡�� ���᪠*/
extern  char    *fnd_str;       /* ��ப� ��� ���᪠ */
extern  size_t  fnd_len;        /* ����, �뤥������ ��� fnd_str*/
extern  char    *tmp_str;       /* ��ப� ��� ���᪠ */
extern  size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
extern  char    *last_search;   /* ��� 䠩�� � ��᫥���� १���⮬ ���᪠ */
static  char    schname[]="/tmp/conixXXXXXX";/*��� ��� mktemp()*/
	int     find;           /*�ਧ��� ���᪠*/
	int     stop=0;         /*�ਧ��� ������� ���᪮� */
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
 struct inp_lst inp_lst[3];     /* ᯨ᮪ ����� ��� inp_menu()*/
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	FILE    *prot=NULL;     /* १����� find � 䠩��*/
	WINDOW  *win_box=NULL;  /* ���� - த�⥫� ��� win */
	WINDOW  *win=NULL;      /* ���� ࠡ��� � ᯨ᪮� ��������� 䠩���*/
 struct stat    stbuf;
	int     fd[2];
	int     x;
	int     y;
	int     ch;
	char    simb;
	int     stopfind;       /* �ਧ��� ��⠭���� �ணࠬ�� find */
	unsigned setalarm;      /* ��� ����������� ��ண� ���祭�� alarm */
	void    (*old_alarm)(); /* ���� �㭪�� ��ࠡ�⪨ alarm*/
	pid_t   pid;            /* ����� ����� ��� �������� */
	pid_t   pid1;           /* ���祭��, �����頥��� wait() */
	int     outprot=1;      /* �ਧ��� �뢮�� ��⮪��� � 䠩� */
	short   curline;        /* ����� ⥪�饩 ��ப� */
	short   firstline;      /* ����� ��ࢮ� ��ப� ����*/
	short   lines;          /* ������⢮ ��ப � ᯨ᪥ */
	char    *dirname;       /* ��� ��⠫��� */
	char    *filename;      /* ��� 䠩��, �� ����� ���⠢���� 㪠��⥫�*/
	int     i;
/*==========================================================================*/
/* fnd_str[0]='\0'; - �� ����祭� ��� ������� ���᪮� */

if(chdir(panels[cpanel].full_dir)) beep();

/* ������� ⥪�騩 ��⠫�� � ��ப� ।���஢���� */
if(tmp_len<=strlen(panels[cpanel].full_dir)){
  free(tmp_str);
  tmp_len=(strlen(panels[cpanel].full_dir)/128+1)*128;
  tmp_str=malloc(tmp_len);
  tst_err_malloc(tmp_str,13);
}
strcpy(tmp_str,panels[cpanel].full_dir);

/* ��ନ஢��� ᯨ᮪ ����� ��� inp_menu() */
inp_lst[0].str=fnd_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;

inp_lst[1].str=tmp_str;
/* inp_lst[1].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[1].str_len=tmp_len;
inp_lst[1].npunkt=3;

inp_lst[2].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

while(!stop){
  if(last_search){ /*�� ����, ����� �ᯮ�짮���� ��� १����� */
      win42.txts[6].active=1;
  }/*����-㦥 0*/

  find=inp_menu(inp_lst, &win42, 1, HOR|VERT);
  if(find==1 || find==3 || find==4)
    { /* �᪠�� 䠩� */
      if(stat(tmp_str, &stbuf)!=0 || filtype(stbuf.st_mode)!=FTYPE_DIR)
	{ /* � tmp_str[] - �� ��⠫�� */
	  if(ask(&win43,1,HOR)!=1) stop++;
	}else{ /* ����� ����᪠�� find */
	  if(pipe(fd)){
	    endwin();
	    perror("Can't make pipe");
	    exit(-1);
	  }

	  switch(pid=fork())
	    {
	      case 0:  /* ��⮬�� */
		/* ��७������� �뢮� � ��⮪�� �訡�� */
		dup2(fd[1],1);
		dup2(fd[1],2);
		for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		execlp("find", "find", tmp_str, "-name", fnd_str, "-print", NULL);
		perror("Can't execute 'find'");
		exit(-1);
	      break;

	      case -1: /* �� ᮧ������ ����� */
		close(fd[0]);
		close(fd[1]);
		ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
	      break;

	      default: /* ����� - த�⥫� */
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

		/* ��� ��饭�� ᮧ����� �����쭮�� �।�⢠ �ᨭ�஭���� */
		/* ����� � ���������� ᮧ����� �⤥��� ����� �⥭�� �� */
		/* � ����� � �ணࠬ��� �����, � ����稥 ������ � ���    */
		/* ����� �஢���� �� fstat()                              */
		/* �� �� ������ kbstart(), kbhit(), kbget() � kbstop()            */
		/* ࠡ�� �� kbhit() ᫨誮� ᨫ쭮 ��� �����୮� �६�*/

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
			   /* ������ � �뢥�� ��ப�, �᫨ ��� ����*/
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
		      { /* ����� 䠩�� ��� alarm */
			if(!was_alarm) stopfind=1; /* ����� 䠩�� */
		      }else{ /* ��-� ���⠫��� */
			if(outprot) putc(simb, prot);
			if(ferror(prot) || feof(prot)) outprot=0;
			switch(simb)
			  {
			    case '\n': /* ����� ��ப� �� find */
#ifdef FREEBSD
			      /* � BSD - ��室���� �ய��뢠�� ��ப� �� ���� */
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
      /* �� ᠬ�� ���� �� �������� �� � 横��, � ���� ࠧ */
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
    ask(&win74,1,0); /* ᮮ�饭�� �� �訡��*/
  }
}

if(prot){ /* ��ᬮ��/�롮� */
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
#ifdef FREEBSD
      /* � � BSD - �� ⮫쪮 ⥪���� */
      while(x++ < COLS_LST-1) waddch(win, ' ');
#else
      if(y==curline) while(x++ < COLS_LST-1) waddch(win, ' ');
#endif
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

	case K_CR:        /*�롮� �㭪� ����*/
	case K_LF:        /*�롮� �㭪� ����*/
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
	      tst_err_malloc(tmp_str,18); /* �믮���� exit, �᫨ NULL*/
	    }
	    tmp_str[x]=getc(prot);
	    if(tmp_str[x]=='\n' || feof(prot) || ferror(prot)){
	      tmp_str[x]='\0';
	      stop++;
	    }
	  }
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
  overwrite(stdscr, save_scr); /* ����� ����� ���孨� �ணࠬ�*/
  if(nosave_ask==0) refresh();
}
if(prot) fclose(prot);
if(win) delwin(win);
if(win_box) delwin(win_box);
}

