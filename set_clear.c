#include <ctype.h>
#include "defs.h"
#define MENU_LIN 6      /* "��ப�" ��砫� ���� */
/****************************************************************************/
		set_clear(win, mode_set, mode_clr)
/*              ������� ��⠭����������/��頥��� ��⮢ ०��� ����㯠
*                                       ���⮢ �.�.
*****************************************************************************/
	struct  win     *win;
	ushort  *mode_set;       /* &०�� ����㯠 (set) */
	ushort  *mode_clr;       /* &०�� ����㯠 (clear) ��� NULL */

{
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  chtype  atr[];          /* ���᪠ */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
	int     ch;             /*��� ����⮩ ������ */
	int     stop=0;
	WINDOW  *c_win;
	short   y;
	short   x;
	int     i;
	char    *txt;
	int     curr=3;         /* ����� ⥪�饩 ��ப� (��� ��ப� ����)*/
	int     pred;           /* ����� �।��饣� �㭪� ���� */
	int     pos=0;          /* ����� ����樨 �� ��������� ०����*/
	ushort  mode;
	struct  win_txt *txts;  /*⥪���*/
	int     ntext;          /*����� ��ப�*/
/*==========================================================================*/
/* ���᫨�� ���न���� ��ࠢ������� */
if(win->x==(-1)){ /* ��஢���� ���� �� �।���*/
  win->x=(COLS-win->cols)/2;
}
if(win->y==(-1)){ /* ��஢���� ���� �� �।���*/
  win->y=(LINES-win->lines)/2;
}
txts=win->txts;
for(ntext=0;ntext<win->npunkt;ntext++){
  if(txts[ntext].x==(-1)){ /* ��஢���� �� �।��� ���� */
    txts[ntext].x=(win->cols-strlen(txts[ntext].txt))/2;
  }
}

if(nosave_ask==0){
  delsoob();
  overwrite(curscr, save_scr);
  overwrite(curscr, stdscr);
  clearok(stdscr,FALSE);
}

c_win=subwin(stdscr,win->lines,win->cols,win->y,win->x);
mywbkgd(c_win, *(win->attr));
leaveok(c_win, FALSE);

if(win->attr_box != &NO_DRAW_ATTR){
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

  wattrset(c_win, *(win->attr_box));
  box(c_win, 0,0);
}

outwin(win,-1,-1,c_win); /*�뢮� ⥪�� � ���� ��� �뤥�������*/

outattrs(win, c_win, curr, pos, mode_set, mode_clr); /* �뢮� ��ਡ�⮢ � ��⠭����� �� col,pos*/

refresh();
wrefresh(c_win);

while(!stop){

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
	  if(curr>=MENU_LIN)
	    { /* ��६�饭�� �� �㭪⠬ ���� set/cancel */
	      pred=curr;
	      curr=calcnext(win,curr);
	      outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	    }else{ /* ���室 �� ���� ��⮢ */
	      if( pos<2 || (pos<5&&mode_clr)) pos++;
	      outattrs(win, c_win, curr, pos, NULL, NULL); /* ��⠭���� �� col,pos*/
	      wrefresh(c_win);
	    }
	break;

	case K_SD:  /*��५�� ����*/
	case KEY_DOWN:  /*��५�� ����*/
	  if(curr>=MENU_LIN)
	    { /* ��६�饭�� �� �㭪⠬ ���� set/cancel */
	      pred=curr;
	      curr=calcnext(win,curr);
	      outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	    }else{ /* ��६�饭�� �� ���� ��ਡ�⮢ */
	      curr++;
	      if(curr==MENU_LIN)
		{ /* ��室 � ���� */
		  outwin(win,curr,-2,c_win);  /*��ᢥ��� ���� �㭪�*/
		}else{
		  outattrs(win, c_win, curr, pos, NULL, NULL); /* ��⠭���� �� col,pos*/
		  wrefresh(c_win);
		}
	    }
	break;


	case K_SL:  /*��५�� �����*/
	case KEY_LEFT:  /*��५�� �����*/
	  if(curr>=MENU_LIN)
	    { /* ��६�饭�� �� �㭪⠬ ���� set/cancel */
	      pred=curr;
	      curr=calcpred(win,curr);
	      outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	    }else{ /* ���室 �� ���� ��⮢ */
	      if(pos) pos--;
	      outattrs(win, c_win, curr, pos, NULL, NULL); /* ��⠭���� �� col,pos*/
	      wrefresh(c_win);
	    }
	break;

	case K_SU:    /*��५�� �����*/
	case KEY_UP:    /*��५�� �����*/
	  if(curr>=MENU_LIN)
	    { /* ��६�饭�� �� �㭪⠬ ���� set/cancel */
	      if(curr==MENU_LIN)
		{ /* ��室 �� ���� */
		  outwin(win, -2, curr, c_win);  /*������� ����*/
		  curr--;
		  outattrs(win, c_win, curr, pos, NULL, NULL); /* ��⠭���� �� col,pos*/
		  wrefresh(c_win);
		}else{ /* ࠡ�� � ����*/
		  pred=curr;
		  curr=calcpred(win,curr);
		  outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
		}
	    }else{ /* ���室 �� ���� ��⮢ */
	      if(curr>3) curr--;
	      outattrs(win, c_win, curr, pos, NULL, NULL); /* ��⠭���� �� col,pos*/
	      wrefresh(c_win);
	    }
	break;

	case ' '|PECH_SIMB:
	case ' ':
	  if(curr<MENU_LIN)
	    { /* ��⠭����/��� ��� */
	      if(pos<3)
		{ /* Set */
		  mode = (1<<(2-pos)) << ((MENU_LIN-curr-1)*3);
/*                  printf("pos=%d curr=%d MENU_LIN-curr-1=%d mode=%o\n",pos,curr,MENU_LIN-curr-1,mode); getchar();*/
		  if(*mode_set & mode) /* �᪫.��� �� 1 ��� - ⠪ ����⭥�*/
		     *mode_set&=~mode; /* ��� ��� */
		    else
		     *mode_set|=mode;  /*��⠭���� ��� */
		}else{ /* Clear */
		  mode = (1<<(5-pos)) << ((MENU_LIN-curr-1)*3);
		  if(*mode_clr & mode) /* �᪫.��� �� 1 ��� - ⠪ ����⭥�*/
		     *mode_clr&=~mode;
		    else
		     *mode_clr|=mode;
		}
	      outattrs(win, c_win, curr, pos, mode_set, mode_clr); /* �뢮� ��ਡ�⮢ � ��⠭����� �� col,pos*/
	      wrefresh(c_win);
	    }else{ /* � ���� - �������⨬� */
	      beep();
	    }
	break;

	case K_PAGE_DN:
	case KEY_NPAGE:
	case K_END:
	case KEY_END:
	  if(curr<MENU_LIN)
	    { /* ��室 � ���� */
	      curr=MENU_LIN;
	      outwin(win,curr,-2,c_win);  /*��ᢥ��� ���� �㭪�*/
	    }
	break;

	case K_ESC:       /*����� ࠡ���*/
	case ESC:       /*����� ࠡ���*/
	case K_F0:
	case KEY_F(0):
	  curr=(-1);
	  stop++;
	break;

	case K_CR:        /*�롮� �㭪� ����*/
	case K_LF:        /*�롮� �㭪� ����*/
	case ENTER:     /*�롮� �㭪� ����*/
	   if(curr>=MENU_LIN)
	     {
	       stop++;
	     }else{
	       beep();
	     }
	break;

	default:
	   beep();
	break;
      }

} /*while(!stop)*/

/*����⠭����� �࠭ ��� ����� */
delwin(c_win);

if(nosave_ask==0){
  overwrite(save_scr, stdscr);
#ifdef FREEBSD
  touchwin(stdscr);
#endif
  refresh();
}

return(curr);
}

