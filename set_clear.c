#include <ctype.h>
#include "defs.h"
#define MENU_LIN 6      /* "строка" начала меню */
/****************************************************************************/
		set_clear(win, mode_set, mode_clr)
/*              Задание устанавливаемых/очищаемых битов режима доступа
*                                       Ревтов С.И.
*****************************************************************************/
	struct  win     *win;
	ushort  *mode_set;       /* &режим доступа (set) */
	ushort  *mode_clr;       /* &режим доступа (clear) или NULL */

{
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  chtype  atr[];          /* раскраска */
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
	int     ch;             /*код нажатой клавиши */
	int     stop=0;
	WINDOW  *c_win;
	short   y;
	short   x;
	int     i;
	char    *txt;
	int     curr=3;         /* номер текущей строки (или строки меню)*/
	int     pred;           /* номер предыдущего пункта меню */
	int     pos=0;          /* номер позиции при изменении режимов*/
	ushort  mode;
	struct  win_txt *txts;  /*тексты*/
	int     ntext;          /*номер строки*/
/*==========================================================================*/
/* Вычислить координаты выравнивания */
if(win->x==(-1)){ /* выровнять окно на середину*/
  win->x=(COLS-win->cols)/2;
}
if(win->y==(-1)){ /* выровнять окно на середину*/
  win->y=(LINES-win->lines)/2;
}
txts=win->txts;
for(ntext=0;ntext<win->npunkt;ntext++){
  if(txts[ntext].x==(-1)){ /* выровнять на середину окна */
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

  wattrset(c_win, *(win->attr_box));
  box(c_win, 0,0);
}

outwin(win,-1,-1,c_win); /*вывод текста в окне без выделенного*/

outattrs(win, c_win, curr, pos, mode_set, mode_clr); /* вывод атрибутов с установкой на col,pos*/

refresh();
wrefresh(c_win);

while(!stop){

    if(use_keycap)
      { /*использовать свою настройку на клавиатуру */
	ch=getkey_();
      }else{ /* использовать стандартные средства curses */
	ch=getch();
      }
    switch(ch)
      {
	case K_SR:    /*стрелка вправо*/
	case KEY_RIGHT:    /*стрелка вправо*/
	  if(curr>=MENU_LIN)
	    { /* перемещение по пунктам меню set/cancel */
	      pred=curr;
	      curr=calcnext(win,curr);
	      outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    }else{ /* переход по полям битов */
	      if( pos<2 || (pos<5&&mode_clr)) pos++;
	      outattrs(win, c_win, curr, pos, NULL, NULL); /* установка на col,pos*/
	      wrefresh(c_win);
	    }
	break;

	case K_SD:  /*стрелка вниз*/
	case KEY_DOWN:  /*стрелка вниз*/
	  if(curr>=MENU_LIN)
	    { /* перемещение по пунктам меню set/cancel */
	      pred=curr;
	      curr=calcnext(win,curr);
	      outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    }else{ /* перемещение по полям атрибутов */
	      curr++;
	      if(curr==MENU_LIN)
		{ /* выход в меню */
		  outwin(win,curr,-2,c_win);  /*высветить новый пункт*/
		}else{
		  outattrs(win, c_win, curr, pos, NULL, NULL); /* установка на col,pos*/
		  wrefresh(c_win);
		}
	    }
	break;


	case K_SL:  /*стрелка влево*/
	case KEY_LEFT:  /*стрелка влево*/
	  if(curr>=MENU_LIN)
	    { /* перемещение по пунктам меню set/cancel */
	      pred=curr;
	      curr=calcpred(win,curr);
	      outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    }else{ /* переход по полям битов */
	      if(pos) pos--;
	      outattrs(win, c_win, curr, pos, NULL, NULL); /* установка на col,pos*/
	      wrefresh(c_win);
	    }
	break;

	case K_SU:    /*стрелка вверх*/
	case KEY_UP:    /*стрелка вверх*/
	  if(curr>=MENU_LIN)
	    { /* перемещение по пунктам меню set/cancel */
	      if(curr==MENU_LIN)
		{ /* выход из меню */
		  outwin(win, -2, curr, c_win);  /*погасить старый*/
		  curr--;
		  outattrs(win, c_win, curr, pos, NULL, NULL); /* установка на col,pos*/
		  wrefresh(c_win);
		}else{ /* работа в меню*/
		  pred=curr;
		  curr=calcpred(win,curr);
		  outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
		}
	    }else{ /* переход по полям битов */
	      if(curr>3) curr--;
	      outattrs(win, c_win, curr, pos, NULL, NULL); /* установка на col,pos*/
	      wrefresh(c_win);
	    }
	break;

	case ' '|PECH_SIMB:
	case ' ':
	  if(curr<MENU_LIN)
	    { /* установка/сброс бита */
	      if(pos<3)
		{ /* Set */
		  mode = (1<<(2-pos)) << ((MENU_LIN-curr-1)*3);
/*                  printf("pos=%d curr=%d MENU_LIN-curr-1=%d mode=%o\n",pos,curr,MENU_LIN-curr-1,mode); getchar();*/
		  if(*mode_set & mode) /* искл.или на 1 бит - так понятнее*/
		     *mode_set&=~mode; /* сброс бита */
		    else
		     *mode_set|=mode;  /*установка бита */
		}else{ /* Clear */
		  mode = (1<<(5-pos)) << ((MENU_LIN-curr-1)*3);
		  if(*mode_clr & mode) /* искл.или на 1 бит - так понятнее*/
		     *mode_clr&=~mode;
		    else
		     *mode_clr|=mode;
		}
	      outattrs(win, c_win, curr, pos, mode_set, mode_clr); /* вывод атрибутов с установкой на col,pos*/
	      wrefresh(c_win);
	    }else{ /* В меню - недопустимо */
	      beep();
	    }
	break;

	case K_PAGE_DN:
	case KEY_NPAGE:
	case K_END:
	case KEY_END:
	  if(curr<MENU_LIN)
	    { /* выход в меню */
	      curr=MENU_LIN;
	      outwin(win,curr,-2,c_win);  /*высветить новый пункт*/
	    }
	break;

	case K_ESC:       /*конец работы*/
	case ESC:       /*конец работы*/
	case K_F0:
	case KEY_F(0):
	  curr=(-1);
	  stop++;
	break;

	case K_CR:        /*выбор пункта меню*/
	case K_LF:        /*выбор пункта меню*/
	case ENTER:     /*выбор пункта меню*/
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

/*восстановить экран под окном */
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

