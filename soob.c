#include "defs.h"
/****************************************************************************/
			soob(win)
/*              �뤠� ᮮ�饭��
*               ᮧ���� ���� � ����頥� 㪠��⥫� � soob_win
*                                       ���⮢ �.�.
*****************************************************************************/
       struct  win     *win;           /*����� �� ������*/
{
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  chtype  atr[];          /* ���᪠ */
extern  WINDOW  *soob_win;      /* ���� �뤠� ᮮ�饭�� */
	short   y;
	short   x;
	chtype  ch;
	struct  win_txt *txts;  /*⥪���*/
	int     ntext;          /*����� ��ப�*/
/*==========================================================================*/
/* ���᫨�� ���न���� ��ࠢ������� */
if(win->x==(-1)){ /* ��஢���� ���� �� �।���*/
  win->x=(COLS-win->cols)/2;
}
if(win->y==(-1)){ /* ��஢���� ���� �� �।���*/
  win->y=(LINES-win->lines)/2-3;
  if(win->y<0) win->y=0;
}
txts=win->txts;
for(ntext=0;ntext<win->npunkt;ntext++){
  if(txts[ntext].x==(-1)){ /* ��஢���� �� �।��� ���� */
    txts[ntext].x=(win->cols-strlen(txts[ntext].txt))/2;
  }
}

if(soob_win == NULL && nosave_ask==0){
  overwrite(curscr, save_scr);
  overwrite(curscr, stdscr);
  clearok(stdscr,FALSE);
}
if(soob_win == NULL ||
     soob_win->_maxy != win->lines ||
     soob_win->_maxx != win->cols  ||
     soob_win->_begy != win->y     ||
     soob_win->_begx != win->x
  ){ /* ���� ��� ��� ��� ��㣮�� ࠧ���*/
     delsoob(soob_win);
     soob_win=subwin(stdscr,win->lines,win->cols,win->y,win->x);
}

/* ��ᮢ���� ⥭� */
/* ���⨪��쭠� ����*/
x=win->x+win->cols;
for(y=win->y+1; y<=win->y+win->lines; y++){
    move(y,x);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
}

/* ��ਧ��⠫쭠� ����*/
y=win->y+win->lines;
for(x=win->x+2; x<=win->x+win->cols+1; x++){
    move(y,x);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
}

mywbkgd(soob_win, *(win->attr));

wattrset(soob_win, *(win->attr_box));
box(soob_win, 0,0);

outwin(win,-1,-1,soob_win); /*�뢮� ⥪�� � ����, ࠭�� �� �뤥�������*/
refresh();

}


/****************************************************************************/
		int     delsoob()
/*              ��࠭�� ࠭�� �뤠����� ᮮ�饭��
*
*                                       ���⮢ �.�.
*****************************************************************************/
{
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  WINDOW  *soob_win;      /* ���� �뤠� ᮮ�饭�� */
/*==========================================================================*/
if(soob_win){
  /*����⠭����� �࠭ ��� ����� */
  delwin(soob_win);
  soob_win=NULL;

  overwrite(save_scr, stdscr);
#ifdef FREEBSD
  touchwin(stdscr);
#endif
  refresh();
}
}
