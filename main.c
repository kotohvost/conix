#include "defs.h"
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define unset_alarms()  if(alarm_set) { alarm_set=0; alarm(0);}
	chtype  atr[NUM_ATTRS];   /* ���᪠ */
struct  panel   panels[NPANELS];
	int     cpanel=1; /* ⥪��� ������*/
	int     outpanel;
	WINDOW  *dn_menu;       /* ������ ���� */
	WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
	WINDOW  *soob_win;      /* ���� �뤠� ᮮ�饭�� */
	WINDOW  *inp_win;       /* ���� ����� ������ */
	WINDOW  *clock_win;     /* ���� �뢮�� �६��� */
	WINDOW  *dbl_win;       /* ���� �뢮�� ������� ������� �ਭ� */
	WINDOW  *saved_win[NPANELS]; /* ��� ��࠭���� 㪠��⥫�� �� ���� ������� */
	unsigned inmenu;        /*��� ����樨 ������ ������ */
	char    *fnd_str;       /* ��ப� ��� �६����� ������ */
	size_t  fnd_len;        /* ����, �뤥������ ��� fnd_str*/
	char    *tmp_str;       /* ��ப� ��� �६����� ������ */
	size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
	char    *inp_str;       /* ��ப� ��� ����� ������ */
	size_t  inp_len;        /* ����, �뤥������ ��� inp_str*/
	int     inp_first;      /* ����� ��ࢮ�� �뢥������� ᨬ���� ������� */
	int     inp_cur;        /* ����� ⥪�饣� ᨬ���� ������� */
	char    *history;       /* ���� �࠭���� ���ਨ */
	size_t  his_len;        /* ����, �뤥������ ��� history*/
	int     his_cur;        /* ����� ����� history ��� ���������� */
	int     his_go=0;       /* ����� ����� history */
	int     was_on=0;       /* � ����: 1<<�����_���_���*/
	int     use_keycap=0;   /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
	char    *last_search;   /* ��� 䠩�� � ��᫥���� १���⮬ ���᪠ */
	short   pred_file=(-1); /* ����� ��।��饣� 䠩�� ��� refr_panel()*/
struct  fields  fields[MAXFIELDS]={
	{ FIELD_TYPE, 2          ,""    },        /* ᨬ��� ⨯� 䠩�� */
	{ FIELD_MARK, 1          ,""    },        /* ᨬ��� �⬥⪨ 䠩�� */
	{ FIELD_NAME, OUTNAMELEN ,"Name"},
	{ FIELD_SIZE, 9          ,"Size"},
	{ FIELD_DATE, 9          ,"Date"},
	{ FIELD_TIME, 6          ,"Time"},
	{ FIELD_INUM, 7          ,"Inum"},        /* ����� inode */
	{ FIELD_MODE, 10         ,"Mode"},
	{ FIELD_OWNR, 6          ,"Owner"},
	{ FIELD_GRP , 6          ,"Group"},
	{ FIELD_NLNK, 5          ,"Link"}   };    /* ���� �뢮�� */
	int     nosave_ask=0;   /* �ਧ��� "����࠭����" �࠭� ��� ask */
	char    *keycapfile;    /* ��� 䠩�� � ���ᠭ��� ���������� (�ଠ� TERMCAP)*/
	int     use_colors;     /* �ਧ��� �ᯮ�짮����� 梥⮢ */
	char    cmp_exist=1;    /* �ਧ��� �ࠢ����� �� ������ */
	char    cmp_time =1;    /* �ਧ��� �ࠢ����� �� �६��� */
	char    cmp_size =1;    /* �ਧ��� �ࠢ����� �� ࠧ���� */
	char    cmp_mode =1;    /* �ਧ��� �ࠢ����� �� ������ */
	char    cmp_body =0;    /* �ਧ��� �ࠢ����� �� ᮤ�ন���� */
	char    cmp_nlink=1;    /* �ਧ��� �ࠢ����� �� ���-�� ��뫮� */
	int     silent;         /* �ਧ��� �ࠢ����� ��� ᮮ�饭�� �� �訡���*/
	char    mach_name[SYS_NMLN];/* �������� ��設� */
	char    user_name[LOGNM_MAX]; /* ��� ���짮��⥫� */
	uid_t   user_eid;       /* ��䥪⨢�� UserID ��� 0, �᫨ getuid()==0*/
	int     prompt;         /* ⨯ �ਣ��襭��:0-���, 1-NodeName */
	int     clock_need=0;   /* �ਧ��� �뢮�� �६��� 0-��� 1-�� */
	int     alarmed=0;      /* �ਧ��� ⮣�, �� �뫮 ���뢠��� �� alarm()*/
	int     alarm_set=0;    /* �ਧ��� ⮣�, �� ��⠭����� �㤨�쭨� */
	int     fromcode;       /* ⨯ ����஢��, �� ���ன ��४���஢��� �� ��ᬮ��*/
	int     tocode;         /* ⨯ ����஢��, � ������ ��४���஢��� �� ��ᬮ��*/
/* Do not remove copyright, add your own if needed! */
static  char    long_title[]="Commander for Unix by S.Revtov V1.10 1997 (sir@cb.tver.ru)\n";
	char    shrt_title[]="conix V1.10";

	char    *helpfile;      /* 䠩� � �ࠢ��� */
	int     in_viewer;      /* �ਧ��� �맮�� execcmd() � ०��� ��ᬮ�� */
	int     dwidth = 0;     /* �ਧ��� ������� ������� �ਭ� */


/****************************************************************************/
				main(argc, argv)
/****************************************************************************/
	int     argc;
	char    *argv[];
{
extern  struct  win     win1;   /* ���ᠭ�� ���� ���⢥ত���� ��室�*/
	short   stop=0;         /* �ਧ��� ���� ࠡ��� */
	int     ch;             /* ᨬ��� � ����������*/
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
	char    *ptr;
	int     find;           /* �ਧ��� ��� ���᪠ */
	int     i;
	struct stat stbuf;
	int     wasbs=0;        /* �ਧ���, �� 㦥 �� ������ �� BACKSPACE */
	DIR     *dirp;          /* ��� �஢�ન �⠥���� ��⠫��� */
	struct dirent *dp;      /* �������筮 */
static  char    name[]={ 'c'-'a','o'-'b','n'-'c','i'-'d','x'-'e'};
/*==========================================================================*/
printf(long_title);
for(ptr=argv[0]; *ptr;ptr++);
while(ptr!=argv[0] && *ptr!='/') ptr--;
if(*ptr=='/') ptr++;
for(ch='a',i=0; i<5; i++, ch++){
  if(name[i]+ch != ptr[i]){
    fprintf(stderr,"Illegal commander name, must begin with \"conix\"\n");
    exit(-1);
  }
}

init(); /* ���樠������ curses, ����ன�� 梥⮢, �ନ஢���� ������� */

#ifdef NCURSES
touchwin(stdscr);
#endif

refresh();
wrefresh(dn_menu);
wrefresh(inp_win); /* ����� ������ ����� � ���� ����� �������! */

while(!stop){
  if(alarm_set==0 && clock_need){ /* ����� �� alarm() �� ��襭, � ⥯��� �㦥�*/
    alarm_set=1;
    alarm(1);
  }

  if(use_keycap)
    { /*�ᯮ�짮���� ᢮� ����ன�� �� ���������� */
      ch=getkey_();
    }else{ /* �ᯮ�짮���� �⠭����� �।�⢠ curses */
      ch=getch();
      if(ch==ERR && alarmed) ch=ALARMED;
    }

  unset_alarms();

  if(wasbs &&
     ch!=K_BACKSPACE && ch!=BACKSP &&
     ch!=K_SL        && ch!=KEY_LEFT
    ) wasbs=0; /* ⥯��� �� BACKSPACE � ��५�� ����� � ��砫� ��ப� - ������*/

  switch(ch)
    {
     case ALARMED: /* �� �६� ࠡ��� getkey_() ��襫 ᨣ���*/
       /* ��ࠡ�⪠ १���⮢ - � ���� 横�� */
       /* ���� �� ��ࠡ��뢠���� �.�. ᨣ��� �.�. �� �६� ࠧ��ન */
     break;

     case K_SU:
     case KEY_UP:
       if(panels[cpanel].on>0)
	 { /* ������ ����祭�, ����� ��६������ �� 䠩���*/
	   if(panels[cpanel].curfile){
	     panels[cpanel].curfile--;
	     if(panels[cpanel].curfile<panels[cpanel].firstfile)
	       {
		 panels[cpanel].firstfile--;
	       }else{ /* ��������� ⮫쪮 ���� 㪠��⥫� */
		 pred_file=panels[cpanel].curfile+1;
	       }
	     outpanel=cpanel;
	     refr_panel();
	     wrefresh(panels[cpanel].win);
	   }
	 }else{ /* ������ �몫�祭�, ��६�饭�� �� ���ਨ */
	   his_go=hispred(his_go);

	   if(his_go==his_cur)
	     { /* ��諨 �� ������ �� ⥪�饩 �������*/
	       inp_str[0]='\0';
	     }else{ /* ������� �।����/᫥����� ������� */
	       strcpy(inp_str, &history[his_go]);
	     }
	   inp_cur=strlen(inp_str);
	   for(inp_first=inp_cur;inp_first<inp_len;inp_first++) inp_str[inp_first]=' ';
	   inp_first=0;  /* � refr_inp() �� ���祭�� ����� ����������*/
	   inp_str[inp_len-1]='\0';
	   refr_inp();
	 }
     break;

     case K_SD:
     case KEY_DOWN:
       if(panels[cpanel].on>0)
	 { /* ������ ����祭�, ����� ��६������ �� 䠩���*/
	   if(panels[cpanel].nfiles-1>panels[cpanel].curfile){
	     panels[cpanel].curfile++;
	     if(panels[cpanel].curfile>panels[cpanel].lastfile)
	       {
		 panels[cpanel].firstfile++;
	       }else{ /* ��������� ⮫쪮 ���� 㪠��⥫� */
		 pred_file=panels[cpanel].curfile-1;
	       }
	     outpanel=cpanel;
	     refr_panel();
	     wrefresh(panels[cpanel].win);
	   }
	 }else{ /* ������ �몫�祭�, ��६�饭�� �� ���ਨ */
	   his_go=hisnext(his_go);

	   if(his_go==his_cur)
	     { /* ��諨 �� ������ �� ⥪�饩 �������*/
	       inp_str[0]='\0';
	     }else{ /* ������� �।����/᫥����� ������� */
	       strcpy(inp_str, &history[his_go]);
	     }
	   inp_cur=strlen(inp_str);
	   for(inp_first=inp_cur;inp_first<inp_len;inp_first++) inp_str[inp_first]=' ';
	   inp_first=0;  /* � refr_inp() �� ���祭�� ����� ����������*/
	   inp_str[inp_len-1]='\0';
	   refr_inp();
	 }
     break;

     case K_PAGE_UP:
     case KEY_PPAGE:
       if(panels[cpanel].on>0){
	if(panels[cpanel].curfile){

	  i = files_on_page(cpanel);

	  panels[cpanel].curfile -= i;
	  panels[cpanel].firstfile -= i;
	  if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;
	  if(panels[cpanel].curfile <= 0) panels[cpanel].curfile=1;
	  outpanel=cpanel;
	  refr_panel();
	  wrefresh(panels[cpanel].win);
	}
       }
     break;

     case K_PAGE_DN:
     case KEY_NPAGE:
       if(panels[cpanel].on>0){
	if(panels[cpanel].nfiles-1>panels[cpanel].curfile){

	  i = files_on_page(cpanel);

	  panels[cpanel].curfile += i;
	  if(panels[cpanel].curfile>panels[cpanel].nfiles-1) /* ��᪮稫� */
		panels[cpanel].curfile=panels[cpanel].nfiles-1;

	  panels[cpanel].firstfile += i;
	  if(panels[cpanel].firstfile + i > panels[cpanel].nfiles-1)
	    panels[cpanel].firstfile = panels[cpanel].nfiles - i;

	  if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;

	  outpanel=cpanel;
	  refr_panel();
	  wrefresh(panels[cpanel].win);
	}
       }
     break;

     case K_HOME:
     case KEY_HOME:
       if(panels[cpanel].on>0)
	 { /* ������ ����祭�, ����� ��६������ �� 䠩���*/
	   if(panels[cpanel].curfile){
	     panels[cpanel].curfile=1;
	     panels[cpanel].firstfile=0;
	     outpanel=cpanel;
	     refr_panel();
	     wrefresh(panels[cpanel].win);
	   }
	 }else{ /* ������ �몫�祭�, ��६�饭�� �� ������� */
	   inp_cur=0;
	   refr_inp();
	 }
     break;

     case K_END:
     case KEY_END:
       if(panels[cpanel].on>0)
	 { /* ������ ����祭�, ����� ��६������ �� 䠩���*/
	   panels[cpanel].firstfile = panels[cpanel].nfiles-files_on_page(cpanel);

	   if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;

	   panels[cpanel].curfile=panels[cpanel].nfiles-1;

	   outpanel=cpanel;
	   refr_panel();
	   wrefresh(panels[cpanel].win);
	 }else{ /* ������ �몫�祭�, ��६�饭�� �� ������� */
	   inp_cur=inp_len-2;
	   while(inp_cur && inp_str[inp_cur]==' ') inp_cur--;
	   if(inp_cur && inp_cur<inp_len-2) inp_cur++;
	   refr_inp();
	 }
     break;

     case K_TAB:
     case TAB:
       if( (panels[cpanel].on==1 || panels[cpanel].on==3) &&
	   (panels[1-cpanel].on==1 || panels[1-cpanel].on==3)){
	outpanel = cpanel;
	cpanel = 1 - cpanel;
	refr_panel(); /* �� �뢮�� �ࠢ��� cpanel � outpanel*/
	wrefresh(panels[outpanel].win);

	outpanel=cpanel;
	refr_panel();
	wrefresh(panels[cpanel].win);
       }
     break;

     case K_LF: /* ^J */
       if(panels[cpanel].on>0){
	 ptr=panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname;
	 if(strcmp(ptr, ".")==0){ /* ��� 䠩�� - ⥪�騩 ��⠫��, �������� ������ ��� ���*/
	   ptr=panels[cpanel].full_dir;
	 }
	 while(inp_cur<inp_len-1 && *ptr){
	     /* ��� ��᮪ �㦭� ��।����� ����� ��䥪⨢�� */
	     for(i=inp_len-2; i>inp_cur; i--) inp_str[i]=inp_str[i-1];
	     inp_str[inp_cur]=*ptr++;
	     inp_cur++;
	 }
	 if(inp_cur<inp_len-1){ /* �������� �஡�� */
	     for(i=inp_len-2; i>inp_cur; i--) inp_str[i]=inp_str[i-1];
	     inp_str[inp_cur]=' ';
	     inp_cur++;
	 }
	 refr_inp();
       }
     break;

     case K_CR:
     case ENTER:
	if(inp_str[0]!=' ')
	  { /* ������� ������� */
	    if(chdir(panels[cpanel].full_dir) &&
		    !( inp_str[0]=='c' && inp_str[1]=='d' &&
		      (inp_str[2]==' ' || inp_str[2]=='\0')
		     ))
	      { /* �� ����稫��� ��३� � ⥪�騩 ��⠫�� ������ */
		beep();
	      }else{
		for(inp_cur=inp_len-2; inp_str[inp_cur]==' ' && inp_cur; inp_cur--);
		inp_cur++;
		inp_str[inp_cur]='\0';

		stop=execcmd(inp_str);

		for(inp_cur=0;inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';
		inp_str[inp_len-1]='\0';
		inp_cur=inp_first=0;
		refr_inp();
		/* wrefresh(inp_win); �믮������ �� ������ 蠣� */
	      }
	  }else{ /* ࠡ�� � ������� */
	    if(panels[cpanel].on>0){
	      if(chdir(panels[cpanel].full_dir)) beep();
	      item= &panels[cpanel].itembuf[panels[cpanel].curfile];

	      i = 1;  /* ⥯��� �� �ਧ��� �����室� � ��⠫��*/

	      stbuf.st_mode=0;
	      stat(panels[cpanel].names + item->nname, &stbuf);
	      if((stbuf.st_mode & S_IFMT) == S_IFDIR)
		{ /* ��⠫�� */

		 if(access(panels[cpanel].names + item->nname, X_OK|R_OK)==0) /* ��⠫��, ����㯭� ��� ���室� � �⥭��*/
		   i = 0;

		 /* �஢�ઠ �⠥���� ��⠫��� �㦭� ��� ⮣�, �⮡� �� ������ root � ���⠥�� (NFS) ��⠫��� */
		 if(i==0){
		   if(dirp = opendir(panels[cpanel].names + item->nname))
		     {
		       if ( (dp=readdir(dirp))==NULL ) i++;
		       if ( i || (dp=readdir(dirp))==NULL ) i++;
		       closedir(dirp);
		       if(i) { beep(); beep(); }
		     }else{
		       i++;
		     }
		 }
		 if ( i || chdir( panels[cpanel].names + item->nname )!=0) i++;

		 if ( i ) beep();

		}else{ /* 䠩� */

		  if(access(panels[cpanel].names + item->nname, X_OK)==0)
		    {
		      stop=execcmd(panels[cpanel].names + item->nname);
		      refr_inp();
		      /* wrefresh(inp_win); �믮������ �� ������ 蠣� */
		    }else{
		      if(ext_enter(panels[cpanel].names+item->nname) != 0) beep();  /* ��� 䠩�� ��ࠡ�⪨ ���७�� ��� ��� ��ப� �� ������� ���७��*/

		    }
		}
		if(i==0){ /* ���諨 � ��⠫�� */

		  outpanel=cpanel;

		  if(strcmp(panels[cpanel].names + item->nname, "..")==0 &&
		     strcmp(panels[cpanel].full_dir, "/"))
		    { /* ���室 �� ��⠫�� ���*/

		      for(ptr=panels[cpanel].full_dir; *ptr; ptr++);
		      while(*ptr!='/' && ptr!=panels[cpanel].full_dir) ptr--;
		      if(*ptr=='/') ptr++;
		      if(strlen(ptr)<tmp_len)
			{
			  strcpy(tmp_str, ptr);
			}else{
			  tmp_str[0]='\0';
			}

		      read_to_panel();  /* extern outpanel */

		      if(tmp_str[0]){
			/* ���� ��⠫��� � ���������묨 ������ */
			for(nfile=0,item= panels[cpanel].itembuf;
			   nfile<panels[cpanel].nfiles &&
			   strcmp(tmp_str,panels[cpanel].names+item->nname)
			   ;nfile++,item++);

			if(nfile<panels[cpanel].nfiles){ /* ������*/
			  panels[cpanel].curfile=nfile;
			  if(nfile>=(panels[cpanel].lines-panels[cpanel].lnused))
			    { /* 䠩� - �� �� ��ࢮ� ��࠭��*/
			      panels[cpanel].firstfile=nfile-(panels[cpanel].lines-panels[cpanel].lnused)/2;
			      if(panels[cpanel].firstfile<0) panels[cpanel].firstfile=0;
			    }else{ /* �� ��ࢮ� ��࠭��*/
			      panels[cpanel].firstfile=0;
			    }
			  refr_panel();
			}
		      }
		    }else{
		      read_to_panel();  /* extern outpanel */
		    }
		  wrefresh(panels[cpanel].win);
		  if(panels[1-cpanel].on == 2) wrefresh(panels[1-cpanel].win);
		} /* ��ࠡ�⪠ ���室� � ��⠫�� */
	    } /* if panel is on */
	  }
     break;

     case K_INSRT: /* Insert */
     case K_INSERT:/* Insert */
     case INSERT:  /* Insert */
     case KEY_IC:  /* Insert */
       if(panels[cpanel].on>0){
	item= &panels[cpanel].itembuf[panels[cpanel].curfile];
	if( strcmp(panels[cpanel].names+item->nname, ".") &&
	    strcmp(panels[cpanel].names+item->nname, "..") ){
	      item->attrs ^= MARKED;
	      if(item->attrs & MARKED)
		{ /*�⬥⪠ ������ 䠩��*/
		  panels[cpanel].selected++;
		  panels[cpanel].size_sel+=item->size;
		}else{
		  panels[cpanel].selected--;
		  panels[cpanel].size_sel-=item->size;
		}
	}
	if(panels[cpanel].nfiles-1>panels[cpanel].curfile){
	  panels[cpanel].curfile++;
	  if(panels[cpanel].curfile>panels[cpanel].lastfile){
	    panels[cpanel].firstfile++;
	  }
	}
	outpanel=cpanel;
	refr_panel();
	wrefresh(panels[cpanel].win);
       }
     break;

     case K_SL:     /* ��५�� ����� */
     case KEY_LEFT: /* ��५�� ����� */
	if(panels[cpanel].on==3)
	  { /* Brief */
	    i = panels[cpanel].lines-panels[cpanel].lnused;

	    panels[cpanel].curfile -= i;
	    if(panels[cpanel].curfile <= panels[cpanel].firstfile) panels[cpanel].firstfile -= i;
	    if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;
	    if(panels[cpanel].curfile <= 0) panels[cpanel].curfile=1;
	    outpanel=cpanel;
	    refr_panel();
	    wrefresh(panels[cpanel].win);

	  }else{
	    if(inp_cur)
	      {
		inp_cur--;
		refr_inp();
	      }else{
		if(!wasbs){
		  beep();
		  wasbs++;
		}
	      }
	  }
     break;

     case K_SR: /* ��५�� ��ࠢ� */
     case KEY_RIGHT: /* ��५�� ��ࠢ� */
	if(panels[cpanel].on==3)
	  { /* Brief */
	    i = panels[cpanel].lines-panels[cpanel].lnused;

	    panels[cpanel].curfile += i;
	    if(panels[cpanel].curfile>panels[cpanel].nfiles-1) /* ��᪮稫� */
		  panels[cpanel].curfile=panels[cpanel].nfiles-1;

	    if(panels[cpanel].curfile >= panels[cpanel].lastfile) panels[cpanel].firstfile += i;
	    if(panels[cpanel].firstfile + files_on_page(cpanel) > panels[cpanel].nfiles-1)
	      panels[cpanel].firstfile = panels[cpanel].nfiles - files_on_page(cpanel);

	    if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;

	    outpanel=cpanel;
	    refr_panel();
	    wrefresh(panels[cpanel].win);

	  }else{ /* ࠡ�� � ��������� ��ப� */

	   if(inp_cur<inp_len-1)
	     {
	       inp_cur++;
	       refr_inp();
	     }else{
	       beep();
	     }
	  }
     break;

     case PRED:   /* ������� ������� (�� ���ਨ)*/
     case K_PRED: /* ������� ������� (�� ���ਨ)*/
     case K_NEXT: /* ᫥����� ������� (�� ���ਨ)*/
     case NEXT:   /* ᫥����� ������� (�� ���ਨ)*/
       switch(ch)
	 {
	   case PRED: /* ������� ������� (�� ���ਨ)*/
	   case K_PRED: /* ������� ������� (�� ���ਨ)*/
	     his_go=hispred(his_go);
	   break;
	   case K_NEXT: /* ᫥����� ������� (�� ���ਨ)*/
	   case NEXT: /* ᫥����� ������� (�� ���ਨ)*/
	     his_go=hisnext(his_go);
	   break;
	 }

       if(his_go==his_cur)
	 { /* ��諨 �� ������ �� ⥪�饩 �������*/
	   inp_str[0]='\0';
	 }else{ /* ������� �।����/᫥����� ������� */
	   strcpy(inp_str, &history[his_go]);
	 }
       inp_cur=strlen(inp_str);
       for(inp_first=inp_cur;inp_first<inp_len;inp_first++) inp_str[inp_first]=' ';
       inp_first=0;  /* � refr_inp() �� ���祭�� ����� ����������*/
       inp_str[inp_len-1]='\0';
       refr_inp();
     break;

     case K_ONOFF: /* ������ ���/�몫 ������� CTRL-O */
     case ONOFF: /* ������ ���/�몫 ������� CTRL-O */
       on_off_panels();
     break;

     case K_SWAPP: /* ������ ᬥ�� ���⠬� ������� CTRL-U */
     case SWAPP: /* ������ ᬥ�� ���⠬� ������� CTRL-U */
       if ( dwidth )
	 { /* �� ������� �ਭ� ��ࠡ��뢠�� ��� TAB */
	   /* ��᮪ ᪮��஢�� �� ��ࠡ�⪨ ⠡��樨 */
	   if( (panels[cpanel].on==1 || panels[cpanel].on==3) &&
	       (panels[1-cpanel].on==1 || panels[1-cpanel].on==3)){
	    outpanel = cpanel;
	    cpanel = 1 - cpanel;
	    refr_panel(); /* �� �뢮�� �ࠢ��� cpanel � outpanel*/
	    wrefresh(panels[outpanel].win);

	    outpanel=cpanel;
	    refr_panel();
	    wrefresh(panels[cpanel].win);
	   }
	 }else{
	   swappanels();   /* �� �㭪樨 ��室���� � 䠩�� commands.c */
	 }
     break;

     case K_CDROOT: /* ������ ���室� � ��୥��� ��⠫��*/
     case CDROOT:   /* ������ ���室� � ��୥��� ��⠫��*/
       if(panels[cpanel].on>0){
	 chdir("/");
	 outpanel=cpanel;
	 read_to_panel();
	 wrefresh(panels[cpanel].win);
	 if(panels[1-cpanel].on == 2) wrefresh(panels[1-cpanel].win);
       }
     break;

     case K_INFO:  /* �뢮�� ���� Info*/
     case INFO:
       pred_file = -1;      /* ����� ��।��饣� 䠩�� ��� refr_panel()*/

       if(panels[cpanel].on>0 && !dwidth){
	 if(panels[1-cpanel].on==2)
	   { /* �뢥�� ����⨢��� ������ � 䠩���� */
	     outpanel=1-cpanel;
	     panels[outpanel].on = panels[outpanel].save_on;
	   }else{ /* �뢥�� ����⨢��� ������ � Info */
	     /* ��� �⮣� �㦭� �맢��� �뢮� ��⨢��� */
	     outpanel=cpanel;
	     panels[1-cpanel].save_on = panels[1-cpanel].on;
	     panels[1-cpanel].on=2;
	   }
	 refr_panel();
	 wrefresh(panels[cpanel].win);
	 wrefresh(panels[1-cpanel].win);
       }
     break;

     case K_REFRESH: /* ������ ��ॢ뢮�� �࠭� */
     case REFRESH:   /* ������ ��ॢ뢮�� �࠭� */
       touchwin(stdscr);
       touchwin(dn_menu);
       touchwin(inp_win);
       clearok(stdscr, TRUE);
       refresh();
       wrefresh(dn_menu);
       /* wrefresh(inp_win); - � ���� 横�� */
     break;

     case K_REREAD:  /* ������ ������뢠��� ��⠫��� */
     case REREAD:    /* ������ ������뢠��� ��⠫��� */
       if(panels[cpanel].on>0){
	 outpanel=cpanel;
	 if(chdir(panels[outpanel].full_dir)) beep();
	 nfile=panels[outpanel].curfile;
	 nfirst=panels[outpanel].firstfile;
	 read_to_panel();
	 if(nfile<panels[outpanel].nfiles){ /* ������*/
	   panels[outpanel].curfile=nfile;
	   panels[outpanel].firstfile=nfirst;
	   refr_panel();
	 }
	 wrefresh(panels[outpanel].win);
	 if(panels[1-cpanel].on == 2) wrefresh(panels[1-cpanel].win);
       }
     break;

     case K_FIND:   /* ������ ���᪠ � ⥪�饬 ��⠫��� */
     case FIND:
       if(panels[cpanel].on>0){
	 findcdir();
       }
     break;

     case K_GOOTH:   /* ������ ���室� ����⨢��� ������ � ��⠫�� ��⨢��� */
     case GOOTH:
       if(panels[cpanel].on>0){
	 if(chdir(panels[cpanel].full_dir)) beep();
	 outpanel=(cpanel) ? 0 : 1;
	 read_to_panel();
	 wrefresh(panels[outpanel].win);
       }
     break;

     case K_PROC:    /* ������ ���室� � ०�� pRocess control */
     case PROC:
       viewkill();
     break;

     case K_LONGNM:  /* ������ �뢮�� ������� ���� */
     case LONGNM:
       panels[cpanel].outfields[0]=0;
       panels[cpanel].outfields[1]=1;
       panels[cpanel].outfields[2]=2;
       panels[cpanel].outfields[3]=3;
       for(i=4; i<MAXFIELDS; i++) panels[cpanel].outfields[i]=(-1);

       outpanel=cpanel;
       refr_panel();  /* extern outpanel */
       wrefresh(panels[cpanel].win);
     break;

     case K_DFLT: /* ������ �뢮�� ������� ������ �� 㬮�砭�� */
     case DFLT:
       for(i=0; i<MAXFIELDS; i++) panels[cpanel].outfields[i]=i;
       outpanel=cpanel;
       refr_panel();  /* extern outpanel */
       wrefresh(panels[cpanel].win);
     break;

     case K_DWIDTH: /* ������ �뢮�� ������� �� ���� �࠭ */
     case DWIDTH:
       if(panels[0].on>0 || panels[1].on>0 ) doublewidth();
     break;

     case K_F1:         /* �뤠� �ࠢ��, �᫨ ��� ���� */
     case KEY_F(1):
       if(helpfile)
	 {
	    viewfile(helpfile);
	  }else{
	    beep();
	  }
     break;

     case K_F3:
     case KEY_F(3):
       if(panels[cpanel].on>0){
	item= &panels[cpanel].itembuf[panels[cpanel].curfile];
	if(chdir(panels[cpanel].full_dir) || filtype(item->mode)==FTYPE_DIR)
	  {
	    beep();
	  }else{
	    if(ext_view(panels[cpanel].names+item->nname) != 0){
	      /* ��� 䠩�� ��ࠡ�⪨ ���७�� ��� ��� ��ப� �� ������� ���७��*/
	      viewfile(panels[cpanel].names+item->nname);
	    }
	  }
       }
     break;

     case K_F4:
     case KEY_F(4):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir) ||
	   filtype(panels[cpanel].itembuf[panels[cpanel].curfile].mode)==FTYPE_DIR)
	  {
	    beep();
	  }else{
	    editfile(panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname);
	  }
       }
     break;

     case K_F5:
     case KEY_F(5):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* �����-� �஡���� � ���室�� � ⥪�騩 ��⠫�� */
	    beep();
	  }else{
	    copyfile();     /* ����஢���� [�⬥祭���] 䠩���*/
	  }
       }
     break;

     case K_F6:
     case KEY_F(6):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* �����-� �஡���� � ���室�� � ⥪�騩 ��⠫�� */
	    beep();
	  }else{
	    renmove();     /* ��६�饭�� [�⬥祭���] 䠩���*/
	  }
       }
     break;

     case K_F7:
     case KEY_F(7):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* �����-� �஡���� � ���室�� � ⥪�騩 ��⠫�� */
	    beep();
	  }else{
	    makedir();
	  }
       }
     break;

     case K_F8:
     case KEY_F(8):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* �����-� �஡���� � ���室�� � ⥪�騩 ��⠫�� */
	    beep();
	  }else{
	    deletefile();     /* 㤠����� [�⬥祭���] 䠩���*/
	  }
       }
     break;

     case K_F9:
     case KEY_F(9):
       popdn();
     break;

     case K_F0:
     case KEY_F(10):
     case QUIT:
     case K_QUIT:
       if(ask(&win1,0,HOR)==0) stop++;
     break;

     case K_ESC:
     case ESC:
       for(inp_cur=0;inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';
       inp_str[inp_len-1]='\0';
       inp_cur=inp_first=0;
       refr_inp();
     break;

     case K_BACKSPACE:
     case BACKSP: /* ������ BACKSPACE */
	if(inp_cur)
	  {
	    for(i=inp_cur; i<inp_len-1; i++) inp_str[i-1]=inp_str[i];
	    inp_str[inp_len-2]=' ';
	    inp_str[inp_len-1]='\0';
	    inp_cur--;
	    refr_inp();
	  }else{
	    if(!wasbs){
	      beep();
	      wasbs++;
	    }
	  }

     break;

     case K_DELETE: /* ������ DELETE */
     case DELETE: /* ������ DELETE */
	for(i=inp_cur; i<inp_len-2; i++) inp_str[i]=inp_str[i+1];
	inp_str[inp_len-2]=' ';
	inp_str[inp_len-1]='\0';
	refr_inp();
     break;

     default:
	if(use_keycap && (ch&SIMBTYPE)==PECH_SIMB) ch&=SIMBOL;
	if(my_isprint(ch))
	  { /* � ��ப� ����� ������� */
	    if((ch=='+'||ch=='-'||ch=='*') && inp_cur==0)
	      { /* ᯥ�. ᨬ��� �⬥⪨ */
		switch(ch)
		  {
		   case '+':
		     selectgroup();
		   break;
		   case '-':
		     deselectgroup();
		   break;
		   case '*':
		     invertselect();
		   break;
		  }
	      }else{
		if(inp_cur<inp_len-1)
		  {
		    /* if(ch=='\\') ch='/'; */
		    for(i=inp_len-2; i>inp_cur; i--) inp_str[i]=inp_str[i-1];
		    inp_str[inp_cur]=ch;
		    inp_cur++;
		    refr_inp();
		  }else{
		    beep();
		  }
	      }
	  }else{
/*          fprintf(stderr, "\r %o %d  ", ch,ch); */
	    beep();
	  }
     break;
    }

  if(alarmed){ /* �� ᨣ��� �� alarm() */
    alarmed=0;
    do_alarms();
  }

  wrefresh(inp_win); /* ����� ������ ����� � ���� ����� �������! */
}
clear();
refresh();
nocbreak();
echo();
endwin();
if(use_keycap){
  setkey_(0); /*����� ������������ ��᫥����⥫쭮��� �몫�祭�� ��஢�� ��.*/
}
if(last_search) unlink(last_search);
exit(0);
}
