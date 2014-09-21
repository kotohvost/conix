#include "defs.h"
#include <sys/stat.h>
/****************************************************************************/
		outattrs(win, c_win, curr, pos, mode_set, mode_clr)
/*      �뢮� ��ਡ�⮢ � ��⠭����� �� col,pos
 *                                      ���⮢ �.�.
*****************************************************************************/
	struct  win     *win;
	WINDOW  *c_win;
	int     curr;           /* ����� ⥪�饩 ��ப� � ���� win */
	int     pos;            /* ������ ।���஢���� ��⮢ � ०��� ����㯠*/
	ushort  *mode_set;       /* &०�� ����㯠 (set) */
	ushort  *mode_clr;       /* &०�� ����㯠 (clear) ��� NULL */

{
	ushort  modes[2];       /* ���祭�� *mode_set � *mode_clr */
	int     nmodes=0;       /* ������⢮ ����������� ����⮢ � modes*/
	int     nmode;          /* ����� ����� � modes*/
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
  { /* ������� Clear - �� 1 ᨬ��� �ࠢ�� */
    wmove(c_win, win->txts[curr].y, win->txts[curr].x+1+pos+3);
  }else{
    wmove(c_win, win->txts[curr].y, win->txts[curr].x+1+pos);
  }
}
