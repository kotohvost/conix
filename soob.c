#include "defs.h"
/****************************************************************************/
			soob(win)
/*              Выдача сообщения
*               создает окно и помещает указатель в soob_win
*                                       Ревтов С.И.
*****************************************************************************/
       struct  win     *win;           /*данные по запросу*/
{
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  chtype  atr[];          /* раскраска */
extern  WINDOW  *soob_win;      /* окно выдачи сообщений */
	short   y;
	short   x;
	chtype  ch;
	struct  win_txt *txts;  /*тексты*/
	int     ntext;          /*номер строки*/
/*==========================================================================*/
/* Вычислить координаты выравнивания */
if(win->x==(-1)){ /* выровнять окно на середину*/
  win->x=(COLS-win->cols)/2;
}
if(win->y==(-1)){ /* выровнять окно на середину*/
  win->y=(LINES-win->lines)/2-3;
  if(win->y<0) win->y=0;
}
txts=win->txts;
for(ntext=0;ntext<win->npunkt;ntext++){
  if(txts[ntext].x==(-1)){ /* выровнять на середину окна */
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
  ){ /* окна нет или оно другого размера*/
     delsoob(soob_win);
     soob_win=subwin(stdscr,win->lines,win->cols,win->y,win->x);
}

/* Рисование тени */
/* вертикальная часть*/
x=win->x+win->cols;
for(y=win->y+1; y<=win->y+win->lines; y++){
    move(y,x);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
}

/* горизонтальная часть*/
y=win->y+win->lines;
for(x=win->x+2; x<=win->x+win->cols+1; x++){
    move(y,x);
    ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
    addch(ch);
}

mywbkgd(soob_win, *(win->attr));

wattrset(soob_win, *(win->attr_box));
box(soob_win, 0,0);

outwin(win,-1,-1,soob_win); /*вывод текста в окне, раньше не выделенного*/
refresh();

}


/****************************************************************************/
		int     delsoob()
/*              Стирание ранее выданного сообщения
*
*                                       Ревтов С.И.
*****************************************************************************/
{
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  WINDOW  *soob_win;      /* окно выдачи сообщений */
/*==========================================================================*/
if(soob_win){
  /*восстановить экран под окном */
  delwin(soob_win);
  soob_win=NULL;

  overwrite(save_scr, stdscr);
#ifdef FREEBSD
  touchwin(stdscr);
#endif
  refresh();
}
}
