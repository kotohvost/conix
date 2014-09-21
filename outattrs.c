#include "defs.h"
#include <sys/stat.h>
/****************************************************************************/
		outattrs(win, c_win, curr, pos, mode_set, mode_clr)
/*      вывод атрибутов с установкой на col,pos
 *                                      Ревтов С.И.
*****************************************************************************/
	struct  win     *win;
	WINDOW  *c_win;
	int     curr;           /* номер текущей строки в окне win */
	int     pos;            /* позиция редактирования битов в режиме доступа*/
	ushort  *mode_set;       /* &режим доступа (set) */
	ushort  *mode_clr;       /* &режим доступа (clear) или NULL */

{
	ushort  modes[2];       /* значения *mode_set и *mode_clr */
	int     nmodes=0;       /* количество заполненных элементов в modes*/
	int     nmode;          /* номер элемента в modes*/
	ushort  mode;
	short   x;
	short   y;
/*==========================================================================*/
if(mode_set) modes[nmodes++]= *mode_set;
if(mode_clr) modes[nmodes++]= *mode_clr;

for(nmode=0; nmode<nmodes;nmode++){
   x=win->txts[3].x+1+nmode*6;
   y=win->txts[3].y;
   mode=modes[nmode];
   wmove(c_win, y, x);
   if(mode&S_IRUSR) waddch(c_win,'r'); else waddch(c_win,'-');
   if(mode&S_IWUSR) waddch(c_win,'w'); else waddch(c_win,'-');
   if(mode&S_IXUSR) waddch(c_win,'x'); else waddch(c_win,'-');
   y++;

   wmove(c_win, y, x);
   if(mode&S_IRGRP) waddch(c_win,'r'); else waddch(c_win,'-');
   if(mode&S_IWGRP) waddch(c_win,'w'); else waddch(c_win,'-');
   if(mode&S_IXGRP) waddch(c_win,'x'); else waddch(c_win,'-');
   y++;

   wmove(c_win, y, x);
   if(mode&S_IROTH) waddch(c_win,'r'); else waddch(c_win,'-');
   if(mode&S_IWOTH) waddch(c_win,'w'); else waddch(c_win,'-');
   if(mode&S_IXOTH) waddch(c_win,'x'); else waddch(c_win,'-');
}

if(pos>2)
  { /* колонка Clear - на 1 символ правее */
    wmove(c_win, win->txts[curr].y, win->txts[curr].x+1+pos+3);
  }else{
    wmove(c_win, win->txts[curr].y, win->txts[curr].x+1+pos);
  }
}
