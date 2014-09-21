#include "defs.h"

/*============================================================================
			     �㭪樨 ���� Commands
	      ******************************************************
	      *   swappanels()    - ����� ������� ���⠬�          *
	      *   on_off_panels() - ����祭��/�몫�祭�� �������   *
	      *   compare_dirs() - �ࠢ����� ��⠫����             *
	      *   histlist()     - ���⠭�� � �롮� ���ਨ        *
	      *   doublewidth()  - �����ୠ�/������� �ਭ� �������*
	      ******************************************************
============================================================================*/

/****************************************************************************/
			swappanels()
/*                      ����� ������� ���⠬�
*****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  WINDOW  *dn_menu;       /* ������ ���� */
extern  WINDOW  *inp_win;       /* ���� ����� ������ */
	struct  panel   tmp_panel;
	WINDOW  *tmp_win;
/*==========================================================================*/
  tmp_panel=panels[0];
  panels[0]=panels[1];
  panels[1]=tmp_panel;

  tmp_win=panels[0].win;
  panels[0].win=panels[1].win;
  panels[1].win=tmp_win;

  outpanel=cpanel;
  cpanel++;
  if(cpanel==NPANELS) cpanel=0;
  if(panels[cpanel].on<=0) cpanel=outpanel;

  erase();
  for(outpanel=0;outpanel<NPANELS;outpanel++){
    refr_panel();  /* extern outpanel */
    if(panels[outpanel].on>0){
      /* wrefresh(panels[outpanel].win);*/
    }
  }
  touchwin(stdscr);
  refresh();
  touchwin(dn_menu);
  wrefresh(dn_menu);
  refr_inp();
  touchwin(inp_win);
  wrefresh(inp_win); /* ����� ������ ����� � ���� ����� �������! */
}


/****************************************************************************/
			on_off_panels()
/*                      ����祭��/�몫�祭�� �������
*****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  WINDOW  *dn_menu;       /* ������ ���� */
extern  WINDOW  *inp_win;       /* ���� ����� ������ */
extern  int     was_on;         /* � ����: 1<<�����_���_���*/
	int     is_on;          /* � ����: 1<<�����_���_���*/
/*==========================================================================*/

  is_on=0;
  for(outpanel=0;outpanel<NPANELS;outpanel++){
    if(panels[outpanel].on>0) is_on|=1<<outpanel;
  }

  if(is_on)
    { /* �����-� ������ ����祭�, �몫���� */
      was_on=is_on;
      erase();
      for(outpanel=0;outpanel<NPANELS;outpanel++){
	if(panels[outpanel].on>0) panels[outpanel].on *= -1;
      }
    }else{ /* ��� ����祭��� �������, ������� */
      erase();
      touchwin(stdscr);
      for(outpanel=0;outpanel<NPANELS;outpanel++){
	if( was_on==0 || (was_on&(1<<outpanel)) ){
	      panels[outpanel].on *= -1;
	      refr_panel();  /* extern outpanel */
	}
      }
    }
  refr_inp();
  refresh();
  touchwin(dn_menu);
  wrefresh(dn_menu);
  touchwin(inp_win);
  wrefresh(inp_win); /* ����� ������ ����� � ���� ����� �������! */
}

/****************************************************************************/
			compare_dirs()
/*                      �ࠢ����� ��⠫����
*****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  win     win37;  /* ᮮ�饭�� �� ���������� ��⠫���� */

extern  char    cmp_exist;      /* �ਧ��� �ࠢ����� �� ������ */
extern  char    cmp_time;       /* �ਧ��� �ࠢ����� �� �६��� */
extern  char    cmp_size;       /* �ਧ��� �ࠢ����� �� ࠧ���� */
extern  char    cmp_mode;       /* �ਧ��� �ࠢ����� �� ������ */
extern  char    cmp_body;       /* �ਧ��� �ࠢ����� �� ᮤ�ন���� */
extern  char    cmp_nlink;      /* �ਧ��� �ࠢ����� �� ���-�� ��뫮� */

 struct itembuf *item1;         /* �����, � ����� �ࠢ�������� */
 struct itembuf *item2;         /* ������ ���筮� ������ */
 struct itembuf *item2_end;     /* ����� ��ᬮ�� ���筮�� ᯨ᪠ */
 struct itembuf *item2_beg;     /* ��砫� ��ᬮ�� ���筮�� ᯨ᪠ */
	char    *name1;         /* ��� 䠩�� �� �᭮���� ������ */
	char    *names2;        /* ��砫� ���ᨢ� ���� ���筮� ������ */
	int     second;         /* ����� ���筮� ������ */
	int     nfile;          /* ����� 䠩�� �᭮���� ������ */
	int     differs=0;      /* ������⢮ ࠧ�������� 䠩��� */
	int     find;           /* �ਧ��� ���᪠ 䠩�� � ��������� ������ */
	int     stop=0;
/*==========================================================================*/
second=(cpanel)?0:1;
names2=panels[second].names;
item2_end= &(panels[second].itembuf[panels[second].nfiles]);
item2_beg= &(panels[second].itembuf[2     ]);

for(nfile=2,item1=&(panels[cpanel].itembuf[2]);
    nfile<panels[cpanel].nfiles && stop==0;
    nfile++,item1++){
  name1=panels[cpanel].names+item1->nname;

  find=0;
  for(item2=item2_beg; item2!=item2_end && !find;){
    if(strcmp(name1, names2+item2->nname) == 0)
      {
	find++;
      }else{
	item2++;
      }
  }

  if(find)
    { /* ������ 䠩� � ⠪��-�� ������, ��� � name1 */

      if(item1->mtime > item2->mtime && cmp_time){
	/* item1 �����, 祬 item2 */
	  if(! item1->attrs&MARKED){
	    item1->attrs |= MARKED;
	    panels[cpanel].selected++;
	    panels[cpanel].size_sel+=item1->size;
	  }
	  differs++;
      }else if(item1->mtime < item2->mtime && cmp_time){
	/* item1 ��॥, 祬 item2 */
	  if(! item2->attrs&MARKED){
	    item2->attrs |= MARKED;
	    panels[second].selected++;
	    panels[second].size_sel+=item2->size;
	  }
	  differs++;
      }else if( (item1->size!=item2->size   && cmp_size)  ||
		(item1->nlink!=item2->nlink && cmp_nlink) ||
		(item1->mode!=item2->mode   && cmp_mode)){

	  /* ��㣨� ࠧ���� �� ���������� ��� */
	  if(! item1->attrs&MARKED){
	    item1->attrs |= MARKED;
	    panels[cpanel].selected++;
	    panels[cpanel].size_sel+=item1->size;
	  }
	  if(! item2->attrs&MARKED){
	    item2->attrs |= MARKED;
	    panels[second].selected++;
	    panels[second].size_sel+=item2->size;
	  }
	  differs++;
      }else if(cmp_body && filtype(item1->mode)==FTYPE_REG &&
			   filtype(item2->mode)==FTYPE_REG){
	switch(cmp_files(panels[cpanel].full_dir, name1,
			 panels[second].full_dir, names2+item2->nname))
	  {
	    case 0: /* ��������� 䠩�� */
	    break;
	    case 1: /* ������� 䠩�� */
	    case 2: /* �訡�� �ࠢ����� */
	      if(! item1->attrs&MARKED){
		item1->attrs |= MARKED;
		panels[cpanel].selected++;
		panels[cpanel].size_sel+=item1->size;
	      }
	      if(! item2->attrs&MARKED){
		item2->attrs |= MARKED;
		panels[second].selected++;
		panels[second].size_sel+=item2->size;
	      }
	      differs++;
	    break;
	    case 3: /* �४���� �ࠢ����� */
	      stop++;
	    break;
	    default:
	      endwin();
	      fprintf(stderr, "Programmer's error in compare_dirs()\n");
	    break;
	  }
      }
    }else{ /* ⠪��� 䠩�� �� ������� */

      if(cmp_exist){
	if(cmp_exist && ! item1->attrs&MARKED){
	  item1->attrs |= MARKED;
	  panels[cpanel].selected++;
	  panels[cpanel].size_sel+=item1->size;
	}
	differs++;
      }
    }
}

if(cmp_exist){ /* ���� 䠩��, ����騥�� �� ��ன ������ � ���������騥 � ��ࢮ� */
  names2=panels[cpanel].names;
  item2_end= &(panels[cpanel].itembuf[panels[cpanel].nfiles]);
  item2_beg= &(panels[cpanel].itembuf[2     ]);

  for(nfile=2,item1=&(panels[second].itembuf[2]);
      nfile<panels[second].nfiles;
      nfile++,item1++){
    name1=panels[second].names+item1->nname;

    find=0;
    for(item2=item2_beg; item2!=item2_end && !find;){
      if(strcmp(name1, names2+item2->nname) == 0)
	{
	  find++;
	}else{
	  item2++;
	}
    }

    if(!find){
      if(! item1->attrs&MARKED){
	item1->attrs |= MARKED;
	panels[second].selected++;
	panels[second].size_sel+=item1->size;
      }
      differs++;
    }
  }
}

if(differs)
  { /* ���� ࠧ���� */
    if(panels[cpanel].on<0) panels[cpanel].on *= -1;
    if(panels[second].on<0) panels[second].on *= -1;
    /* �뢮� ��������� - � popdn() */
  }else{ /* ࠧ��稩 �� ������� */
    ask(&win37,2,0);
  }
delsoob();
}

/****************************************************************************/
			histlist()
/*                      ���⠭�� � �롮� ���ਨ
*****************************************************************************/
{
extern  char    *history;       /* ���� �࠭���� ���ਨ */
extern  size_t  his_len;        /* ����, �뤥������ ��� history*/
extern  int     his_cur;        /* ����� ����� history ��� ���������� */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  chtype  atr[];          /* ���᪠ */
extern struct   win win38;      /* ���� ���ਨ */
extern struct   win win39;      /* ���� ᮮ�饭�� �� �訡�� */
extern  char    *inp_str;       /* ��ப� ��� ����� ������ */
extern  size_t  inp_len;        /* ����, �뤥������ ��� inp_str*/
extern  int     inp_first;      /* ����� ��ࢮ�� �뢥������� ᨬ���� ������� */
extern  int     inp_cur;        /* ����� ⥪�饣� ᨬ���� ������� */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
	int     his_go;         /* ����� ����� history */
	int     cnstr;          /* ������⢮ ��ப history */
	int     curnstr=0;      /* ����� ⥪�饩 ��ப� history �� ���� */
	int     lastcnstr=0;    /* ����� ��᫥���� ��ப� history �� ���� */
	WINDOW  *w;             /* ࠡ�祥 ���� */
	int     len;            /* ����� �뢮����� ��ப� ���ਨ */
	char    *ptr;
	short   lines;          /* ���� ������⢮ ��ப � ���� ���ਨ (��)*/
	short   npunkt;         /* ���� ������⢮ ��ப � ���� ���ਨ (��⨢���)*/
	int     ch;                     /*��� ����⮩ ������ */
	int     stop=0;
	short   y;
	short   x;
	int     nstr;
/*==========================================================================*/
if(win38.x==(-1)){ /* ��஢���� ���� �� �।���*/
  win38.x=(COLS-win38.cols)/2;
}
if(win38.y==(-1)){ /* ��஢���� ���� �� �।���*/
  win38.y=(LINES-win38.lines)/2;
}

len=strlen(win38.txts[0].txt);
his_go=his_cur;

/* ������� ��饥 ������⢮ ��ப ���ਨ */
for(cnstr=0; (his_go=hispred(his_go))!=his_cur;cnstr++);

if(cnstr==0)
  { /* ��� ���ਨ */
    ask(&win39, 1, 0);
  }else{
    if(nosave_ask==0){
      delsoob();
      overwrite(curscr, save_scr);
      overwrite(curscr, stdscr);
      clearok(stdscr,FALSE);
    }

    lines=win38.lines;
    npunkt=win38.npunkt;
    if(npunkt>cnstr)
      {
	win38.npunkt=cnstr;
	win38.lines-=npunkt-cnstr;
      }
    win38.y=(LINES-win38.lines)/2;
    win38.x=(COLS-win38.cols)/2;


    w=subwin(stdscr,win38.lines,win38.cols,win38.y,win38.x);
    mywbkgd(w, *(win38.attr));

    if(win38.attr_box != &NO_DRAW_ATTR){
      /* ��ᮢ���� ⥭� */
      /* ���⨪��쭠� ����*/
      x=win38.x+win38.cols;
      for(y=win38.y+1; y<=win38.y+win38.lines; y++){
	  move(y,x);
	  ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	  addch(ch);
	  ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	  addch(ch);
      }

      /* ��ਧ��⠫쭠� ����*/
      y=win38.y+win38.lines;
      for(x=win38.x+2; x<=win38.x+win38.cols+1; x++){
	  move(y,x);
	  ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	  addch(ch);
      }

      wattrset(w, *(win38.attr_box));
      box(w, 0,0);
    }

    lastcnstr=0;

    while(!stop){

      his_go=his_cur;
      /* �ய����� ��ப�, �� �室�騥 � ���� (᭨��) */
      for(nstr=0;nstr!=lastcnstr;nstr++){
	his_go=hispred(his_go);
      }
      /* �ନ஢���� ��ப � ���� win38 */
      for(nstr=0; nstr<win38.npunkt; nstr++){
	his_go=hispred(his_go);
	strncpy(win38.txts[win38.npunkt-nstr-1].txt, &history[his_go], len);
	win38.txts[win38.npunkt-nstr-1].txt[len]='\0';
	for(x=strlen(win38.txts[win38.npunkt-nstr-1].txt);x<len;x++) win38.txts[win38.npunkt-nstr-1].txt[x]=' ';
      }
      outwin(&win38, win38.npunkt-(curnstr-lastcnstr)-1, -1, w);
      refresh();

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
	    if(curnstr) {
	      curnstr--;
	      if(curnstr<lastcnstr) lastcnstr--;
	    }
	  break;

	  case K_SU:    /*��५�� �����*/
	  case KEY_UP:    /*��५�� �����*/
	    if(curnstr<cnstr-1) {
	      curnstr++;
	      if(curnstr>=lastcnstr+win38.npunkt) lastcnstr++;
	    }
	  break;

	  case K_ESC:       /*����� ࠡ���*/
	  case ESC:       /*����� ࠡ���*/
	  case K_F0:
	  case KEY_F(0):
	    stop++;
	  break;

	  case K_CR:        /*�롮� �㭪� ����*/
	  case K_LF:        /*�롮� �㭪� ����*/
	  case ENTER:     /*�롮� �㭪� ����*/
	     stop++;

	     his_go=his_cur;
	     for(nstr=0;nstr!=curnstr+1;nstr++){
	       his_go=hispred(his_go);
	     }

	     strcpy(inp_str, &history[his_go]);
	     for(inp_cur=strlen(inp_str);inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';

	     inp_str[inp_len-1]='\0';
	     inp_cur=0;
	     inp_first=0;
	     /* refr_inp(); - �㤥� ��᫥ ����⠭������� �࠭� */
	  break;

	  default:
	    beep();
	  break;
	}
    } /*while(!stop)*/

    /*����⠭����� �࠭ ��� ����� */
    delwin(w);
    if(nosave_ask==0){
      overwrite(save_scr, stdscr);
      refresh();
    }
    refr_inp();
    /* ����⠭����� ��ࠬ���� win38 ��� ��᫥����� �맮���*/
    win38.npunkt=npunkt;
    win38.lines=lines;
  }
}

/****************************************************************************/
			doublewidth()
/*                      �����ୠ�/������� �ਭ� �������
*****************************************************************************/
{
extern  chtype  atr[NUM_ATTRS];   /* ���᪠ */
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  WINDOW  *dn_menu;       /* ������ ���� */
extern  WINDOW  *inp_win;       /* ���� ����� ������ */

extern  WINDOW  *dbl_win;       /* ���� �뢮�� ������� ������� �ਭ� */
extern  int     dwidth;         /* �ਧ��� ������� ������� �ਭ� */
extern  WINDOW  *saved_win[NPANELS]; /* ��� ��࠭���� 㪠��⥫�� �� ���� ������� */

/*==========================================================================*/
dwidth = 1 - dwidth;
outpanel=cpanel;

if ( dwidth )
  { /* �㤥� �뢮� � ������� �ਭ�� ������� */

    if ( !dbl_win ) { /* ᮧ���� ���� ��� ������� �ਭ� ������� */
      dbl_win=subwin(stdscr, panels[outpanel].lines, COLS, 0, 0);
      tst_err_malloc(dbl_win,32); /* �믮���� exit, �᫨ NULL*/
      wattrset(dbl_win, MAIN_PANEL_ATTR);
      mywbkgd(dbl_win, MAIN_PANEL_ATTR);
    }

    for ( outpanel=0; outpanel<NPANELS; outpanel++){
      saved_win[outpanel] = panels[outpanel].win;
      panels[outpanel].win = dbl_win;
      panels[outpanel].cols =COLS;
      if ( panels[outpanel].on == 2 ) panels[outpanel].on=1;
      /* panels[outpanel].firstfile=0; */
      /* panels[outpanel].lastfile=0; */
    }
    outpanel=cpanel;
    refr_panel();  /* extern outpanel */
    wrefresh(panels[cpanel].win);

  }else{ /* ��४��祭�� � ०��� �ப�� ������� �� ��ଠ��� */

    erase();
    for ( outpanel=0; outpanel<NPANELS; outpanel++){
      panels[outpanel].cols = COLS/NPANELS;
      panels[outpanel].win = saved_win[outpanel];
      /* panels[outpanel].firstfile=0; */
      panels[outpanel].lastfile=0;

      refr_panel();  /* extern outpanel */
    }

    outpanel=cpanel;
    touchwin(stdscr);

  }

refr_inp();
refresh();
touchwin(dn_menu);
wrefresh(dn_menu);
touchwin(inp_win);
wrefresh(inp_win); /* ����� ������ ����� � ���� ����� �������! */

}

