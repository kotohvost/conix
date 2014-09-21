#include "defs.h"
#define NMENUS  4
/****************************************************************************/
				columns()
/*                      ����ன�� ᮤ�ন���� ������� �������
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  chtype  atr[];          /* ���᪠ */
extern struct fields fields[];  /* �ࠪ���⨪� ��������� ����� �뢮��*/
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  struct  win     win50;  /* ���� �롮� ⨯� ���� �� ᯨ�� */
extern  struct  win     win68;  /* ���� �롮� ��⮢�� ����ன�� */
	short   outfields[MAXFIELDS];/* ����� �뢮����� ����⮢ fields */
	int     curr=0;         /*����� ⥪�饣� �㭪� ����*/
	int     nfield;
	short   size;           /* ��� ���� ������⢠ ����� � ������� */
	WINDOW  *win;
	short   w_lines;
	short   w_cols;
	short   w_begx;
	short   w_begy;
	short   x;
	short   y;
	short   width;
	int     stop=0;
	chtype  ch;                     /*��� ����⮩ ������ */
	int     nosave_ask_old; /* ���� �ਧ��� "����࠭����" �࠭� ��� ask */
static  char    *txts[]={"[ OK ]", "[ Cancel ]", "[ Reset ]", "[ Template ]"};
static  short   txt_x[]={ 2, 9, 20, 30 };
/*==========================================================================*/
x=2;
y=1;
for(nfield=0; nfield<MAXFIELDS; nfield++,y++){
 outfields[nfield]=panels[cpanel].outfields[nfield];
 switch(fields[nfield].fieldtype)
   {
    case FIELD_TYPE:
      win50.txts[nfield].txt="Type";
    break;
    case FIELD_MARK:
      win50.txts[nfield].txt="Mark";
    break;
    default:
      win50.txts[nfield].txt=fields[nfield].fieldname;
    break;
   }
  win50.txts[nfield].x=x;
  win50.txts[nfield].y=y;
  win50.txts[nfield].active=1;
  win50.txts[nfield].attrs=&POPUP_WIN_ATTR;
}
win50.txts[MAXFIELDS].txt="None";
win50.txts[MAXFIELDS].x=x;
win50.txts[MAXFIELDS].y=y;
win50.txts[MAXFIELDS].active=1;
win50.txts[MAXFIELDS].attrs=&POPUP_WIN_ATTR;

if(nosave_ask==0){
  delsoob();
  overwrite(curscr, save_scr);
  overwrite(curscr, stdscr);
  clearok(stdscr,FALSE);
}

w_lines=MAXFIELDS+FROM_TOP+FROM_BOTT;
w_cols=44;
w_begy=3;
w_begx=(COLS-w_cols)/2;

win=subwin(stdscr, w_lines, w_cols, w_begy, w_begx);
tst_err_malloc(win,19); /* �믮���� exit, �᫨ NULL*/

/* ��ᮢ���� ⥭� */
/* ���⨪��쭠� ����*/
x=w_begx+w_cols;
for(y=w_begy+1; y<=w_begy+w_lines; y++){
    move(y,x);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
}

/* ��ਧ��⠫쭠� ����*/
y=w_begy+w_lines;
for(x=w_begx+2; x<=w_begx+w_cols+1; x++){
    move(y,x);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
}

y=FROM_TOP+MAXFIELDS+1;


while(!stop){

  size=panels[cpanel].cols-2;
  mywbkgd(win, POPUP_WIN_ATTR);
  wattrset(win, POPUP_WIN_BOX_ATTR);
  box(win, 0,0);

  wattrset(win, POPUP_WIN_ATTR);
  wmove(win, 1, 13);
  wprintw(win, "Choose column types");
  wmove(win, 3, 7);
  wprintw(win, "No.  Type   Size  Availability");

  for(nfield=0; nfield<MAXFIELDS; nfield++){
    width=0;
    if(outfields[nfield]>=0) width=fields[outfields[nfield]].fieldsize;
    wattrset(win, POPUP_WIN_ATTR);
    wmove(win, FROM_TOP+nfield, 8);
    wprintw(win, "%2d          %2d   %s", nfield+1, width, (width<=size+1 && width>0)? "Available":"Unavailable");
    if(width<=size+1) size-=width;

    wmove(win, FROM_TOP+nfield, 12);
    if(curr==nfield)
      { /* ⥪�騩 �㭪� */
	wattrset(win, POPUP_WIN_OUTL_ATTR);
      }
    if(outfields[nfield]<0){
      waddstr(win, "None");
    }else{
      switch(fields[outfields[nfield]].fieldtype)
	{
	  case FIELD_TYPE:
	    waddstr(win, "Type");
	  break;
	  case FIELD_MARK:
	    waddstr(win, "Mark");
	  break;
	  default:
	    waddstr(win, fields[outfields[nfield]].fieldname);
	  break;
	}
    }
  }
  /* �뢮� ��ப ���� */
  /* �� ��ப� */
  wattrset(win, POPUP_WIN_ATTR);
  for(x=0;x<NMENUS; x++){
    wmove(win, y, txt_x[x]);
    waddstr(win, txts[x]);
  }
  if(curr>=MAXFIELDS){ /* �뤥������ ��ப� */
    wattrset(win, POPUP_WIN_OUTL_ATTR);
    wmove(win, y, txt_x[curr-MAXFIELDS]);
    waddstr(win, txts[curr-MAXFIELDS]);
  }
  wrefresh(win);
  refresh();

  if(use_keycap)
    { /*�ᯮ�짮���� ᢮� ����ன�� �� ���������� */
      ch=getkey_();
    }else{ /* �ᯮ�짮���� �⠭����� �।�⢠ curses */
      ch=getch();
    }
  switch(ch)
    {
      case K_SR:    /*��५�� ��ࠢ�*/
      case KEY_RIGHT:    /*��५�� ��ࠢ�*/
	if(curr<MAXFIELDS+NMENUS-1) curr++;
      break;

      case K_SD:  /*��५�� ����*/
      case KEY_DOWN:  /*��५�� ����*/
	if(curr<MAXFIELDS+NMENUS-1) curr++;
      break;


      case K_SL:  /*��५�� �����*/
      case KEY_LEFT:  /*��५�� �����*/
	if(curr) curr--;
      break;

      case ' '|PECH_SIMB: /* �஡�� - ��������� ���祭�� ���� */
      case ' ':           /* �஡�� - ��������� ���祭�� ���� */
	if(curr<MAXFIELDS)
	  {
	    outfields[curr]++;
	    if(outfields[curr]==MAXFIELDS) outfields[curr]=(-1);
	  }else{ /* � �㭪� ���� */
	    beep();
	  }
      break;

      case K_SU:    /*��५�� �����*/
      case KEY_UP:    /*��५�� �����*/
	if(curr) curr--;
      break;

      case K_PAGE_DN:
      case KEY_NPAGE:
      case K_END:
      case KEY_END:
	if(curr<MAXFIELDS) curr=MAXFIELDS;
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
	switch(curr-MAXFIELDS)
	  {
	   case 0:  /* OK */
	     for(nfield=0; nfield<MAXFIELDS; nfield++) panels[cpanel].outfields[nfield]=outfields[nfield];
	     stop++;
	   break;
	   case 1: /* Cancel */
	     stop++;
	   break;
	   case 2: /* Reset */
	     for(nfield=0; nfield<MAXFIELDS; nfield++) outfields[nfield]=panels[cpanel].outfields[nfield];
	   break;
	   case 3: /* Template*/
	     nosave_ask_old=nosave_ask;
	     nosave_ask=1;
	     switch(ask(&win68, 1, VERT))
	       {
		 case 1: /* Defaults */
		   for(nfield=0; nfield<MAXFIELDS; nfield++) outfields[nfield]=nfield;
		 break;
		 case 2: /* ��ᬮ�� ������� ���� */
		   outfields[0]=0;
		   outfields[1]=1;
		   outfields[2]=2;
		   outfields[3]=3;

		   for(nfield=4; nfield<MAXFIELDS; nfield++) outfields[nfield]=(-1);
		 break;
		 case -1: /* Esc */
		 break;
	       }
	     nosave_ask=nosave_ask_old;
	   break;
	   default:
	     nosave_ask_old=nosave_ask;
	     nosave_ask=1;
	     if((x=ask(&win50, (outfields[curr]>=0) ? outfields[curr] : MAXFIELDS, VERT)) >=0){
	       if(x==MAXFIELDS) x=(-1);/* None */
	       outfields[curr]=x;
	     }
	     nosave_ask=nosave_ask_old;
	   break;
	  }
      break;

      default:
	beep();
      break;
    }

} /*while(!stop)*/

/*����⠭����� �࠭ ��� ����� */
delwin(win);

if(nosave_ask==0){
  overwrite(save_scr, stdscr);
#ifdef FREEBSD
  touchwin(stdscr);
#endif
  refresh();
}

}
