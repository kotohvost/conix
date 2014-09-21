#include "defs.h"
/****************************************************************************/
		int     ask(win,curr,type)
/*              Запрос по пункту меню
*                                       Ревтов С.И.
*****************************************************************************/
       struct  win     *win;           /*данные по запросу*/
       int     curr;                   /*номер текущего пункта меню*/
       int      type;           /*тип переходов: HOR-горизонт.,VERT-вертикальн, OUT-с выходом*/
{
extern  unsigned inmenu;                /*для имитации нажатия клавиши */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  chtype  atr[];          /* раскраска */
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  int     use_colors;     /* признак использования цветов */
	chtype  ch;                     /*код нажатой клавиши */
	int     stop=0;
	int     pred=-1;                /*номер предыдущего пункта меню*/
	WINDOW  *c_win;
	short   y;
	short   x;
	int     i;
	char    *txt;
	struct  win_txt *txts;  /*тексты*/
	int     ntext;          /*номер строки*/
/*==========================================================================*/
inmenu=0;
if(nosave_ask==0){
  delsoob();
  overwrite(curscr, save_scr);
  overwrite(curscr, stdscr);
  clearok(stdscr,FALSE);
}

/* Вычислить координаты выравнивания */
if(win->x==(-1)){ /* выровнять окно на середину*/
  win->x=(COLS-win->cols)/2;
}
if(win->y==(-1)){ /* выровнять окно на середину*/
  win->y=(LINES-win->lines-3)/2;
  if(win->y<0) win->y=0;
}
txts=win->txts;
for(ntext=0;ntext<win->npunkt;ntext++){
  if(txts[ntext].x==(-1)){ /* выровнять на середину окна */
    txts[ntext].x=(win->cols-strlen(txts[ntext].txt))/2;
  }
}

c_win=subwin(stdscr,win->lines,win->cols,win->y,win->x);
if(use_colors || *(win->attr)==A_NORMAL)
  { /* в цветном режиме фон норально выставляется по цвету */
    mywbkgd(c_win, *(win->attr));
  }else{ /* на черно-белом нельзя задавать фон A_STANOUT */
    scrollok(c_win, FALSE);
    wattrset(c_win, *(win->attr_box));
    for(y=0;y<win->lines;y++){
      wmove(c_win, y, 0);
      for(x=0;x<win->cols;x++){
	waddch(c_win, ' ');
      }
    }
  }
if(win->attr_box != &NO_DRAW_ATTR){
  if(use_colors){
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
  }

  wattrset(c_win, *(win->attr_box));
  box(c_win, 0,0);
}

outwin(win,curr,pred,c_win); /*вывод текста в окне, раньше не выделенного*/
refresh();

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
	  if(type&HOR)
	    { /*разрешено перемещение по стрелкам вправо/влево*/
	      pred=curr;
	      curr=calcnext(win,curr);
	      outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    }else{
	      if(type&OUT)
		{ /*разрешен выход по запрещенным направлениям*/
		  stop++;
		  inmenu=ch;
		  curr=(-1);
		}else{
		  beep();
		}
	    }
	break;

	case K_SD:  /*стрелка вниз*/
	case KEY_DOWN:  /*стрелка вниз*/
	  if(type&VERT)
	    { /*разрешено перемещение по стрелкам вверх/вниз*/
	      pred=curr;
	      curr=calcnext(win,curr);
	      outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    }else{
	      if(type&OUT)
		{ /*разрешен выход по запрещенным направлениям*/
		  stop++;
		  inmenu=ch;
		  curr=(-1);
		}else{
		  beep();
		}
	    }
	break;


	case K_SL:  /*стрелка влево*/
	case KEY_LEFT:  /*стрелка влево*/
	  if(type&HOR)
	    { /*разрешено перемещение по стрелкам вправо/влево*/
	      pred=curr;
	      curr=calcpred(win,curr);
	      outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    }else{
	      if(type&OUT)
		{ /*разрешен выход по запрещенным направлениям*/
		  stop++;
		  inmenu=ch;
		  curr=(-1);
		}else{
		  beep();
		}
	    }
	break;

	case K_SU:    /*стрелка вверх*/
	case KEY_UP:    /*стрелка вверх*/
	  if(type&VERT)
	    { /*разрешено перемещение по стрелкам вверх/вниз*/
	      pred=curr;
	      curr=calcpred(win,curr);
	      outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    }else{
	      if(type&OUT)
		{ /*разрешен выход по запрещенным направлениям*/
		  stop++;
		  inmenu=ch;
		  curr=(-1);
		}else{
		  beep();
		}
	    }
	break;

	case K_PAGE_DN:
	case KEY_NPAGE:
	  pred=curr;
	  for(y=0; y<win->npunkt; y++){
	    if(win->txts[y].active && win->txts[y].y>win->txts[curr].y) curr=y;
	  }
	  outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
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
	case ENTER:       /*выбор пункта меню*/
	   stop++;
	break;

	default:
	  if(use_keycap && (ch&SIMBTYPE)==PECH_SIMB) ch&=SIMBOL;

	  if(ch==' ' && win->txts[curr].active==(-1)){ /* Check box */
	    win->txts[curr].txt[0]= (win->txts[curr].txt[0]=='x') ? ' ' : 'x';
	    outwin(win, curr, -2, c_win);
	  }else if(my_isprint(ch)){
	    ch=toupper(ch);
	    pred=curr;
	    for(i=0;i<win->npunkt;i++){
	      if(win->txts[i].active){
		  txt=win->txts[i].txt;
		  for(x=0;*txt && *txt!='['; x++,txt++){
		    if( *txt==ch ){
		      curr=i;
		      stop++;
		    }
		  }
	      }
	    }
	    if(stop) outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	    if(stop>1)  stop=0;
	  }
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
