#include "defs.h"
/****************************************************************************/
		void    outwin(win,curr,pred, c_win)
/*              �뢮� ᮤ�ন���� (��� ���������) ���� �������
*               �᫨ curr >=0, � ��ப� curr �뢮����� � �뤥������
*               �᫨ pred==-1, � �뢮����� �� ��ப�
*                    pred==-2, � �।��騥 �뤥����� �� ᭨������
*                    � ��⠫��� ����� pred �뢮����� � ��� ᮡ��. 梥⮬
*                                       ���⮢ �.�.
*****************************************************************************/
struct  win     *win;                  /*����� �� ������*/
	int     curr;           /*���� �뤥����� �㭪� ����*/
	int     pred;           /*�।��騩 �뤥����� �㭪� ����*/
	WINDOW  *c_win;         /* ���� curses */
{
	struct  win_txt *txts;  /*⥪���*/
	int     ntext;          /*����� ��ப�*/
/*==========================================================================*/
txts=win->txts;

/* �᫨ pred==-1, � �뢮����� ���� ⥪��*/
if(pred!=(-2)){
  if(pred==(-1))
    {/* �뢥�� ���� ⥪��*/
      for(ntext=0;ntext<win->npunkt;ntext++){
	wattrset(c_win, *(txts[ntext].attrs));
	wmove(c_win, txts[ntext].y,txts[ntext].x);
	waddstr(c_win, txts[ntext].txt);
      }
    }else{ /*�뢥�� ⮫쪮 ��������� ��ப�*/
      wattrset(c_win, *(txts[pred].attrs));
      wmove(c_win, txts[pred].y,txts[pred].x);
      waddstr(c_win, txts[pred].txt);
    }
}

if(curr>=0){ /*���� �뤥����� ⥪��*/
  wattrset(c_win, *(win->attr_astr));
  wmove(c_win, txts[curr].y,txts[curr].x);
  waddstr(c_win, txts[curr].txt);
}
wrefresh(c_win);
}

