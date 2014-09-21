#include "defs.h"
/****************************************************************************/
		void    outwin(win,curr,pred, c_win)
/*              Вывод содержимого (или изменений) окна диалога
*               Если curr >=0, то строка curr выводится с выделением
*               Если pred==-1, то выводятся все строки
*                    pred==-2, то предыдущие выделения не снимаются
*                    в остальных случаях pred выводится с его собств. цветом
*                                       Ревтов С.И.
*****************************************************************************/
struct  win     *win;                  /*данные по запросу*/
	int     curr;           /*новый выделенный пункт меню*/
	int     pred;           /*предыдущий выделенный пункт меню*/
	WINDOW  *c_win;         /* окно curses */
{
	struct  win_txt *txts;  /*тексты*/
	int     ntext;          /*номер строки*/
/*==========================================================================*/
txts=win->txts;

/* если pred==-1, то выводится весь текст*/
if(pred!=(-2)){
  if(pred==(-1))
    {/* вывести весь текст*/
      for(ntext=0;ntext<win->npunkt;ntext++){
	wattrset(c_win, *(txts[ntext].attrs));
	wmove(c_win, txts[ntext].y,txts[ntext].x);
	waddstr(c_win, txts[ntext].txt);
      }
    }else{ /*вывести только измененные строки*/
      wattrset(c_win, *(txts[pred].attrs));
      wmove(c_win, txts[pred].y,txts[pred].x);
      waddstr(c_win, txts[pred].txt);
    }
}

if(curr>=0){ /*есть выделенный текст*/
  wattrset(c_win, *(win->attr_astr));
  wmove(c_win, txts[curr].y,txts[curr].x);
  waddstr(c_win, txts[curr].txt);
}
wrefresh(c_win);
}

