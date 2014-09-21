#include "defs.h"
/****************************************************************************/
		inp_menu(inp_lst, win, curr, type)
/*              Работа в запросе со строкой ввода
/****************************************************************************/
struct  inp_lst *inp_lst;       /* список ввода */
struct  win     *win;           /*данные по запросу*/
	int     curr;           /*номер текущего пункта меню*/
	int     type;           /*тип переходов: HOR-горизонт.,VERT-вертикальн, OUT-с выходом*/
{
extern  unsigned inmenu;                /*для имитации нажатия клавиши */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  chtype  atr[];          /* раскраска */
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
	int     ch;             /*код нажатой клавиши */
	int     stop=0;
	int     pred=(-1);      /*номер предыдущего пункта меню*/
	WINDOW  *c_win;
	short   y;              /* для рисования тени */
	short   x;              /* для рисования тени и перемещения по строке ввода*/
	short   edited=0;       /* признак редактирования строки ввода */
	short   first=0;        /* номер первого выводимого символа в строке ввода */
struct  inp_lst *inp;           /* для поиска по списку ввода */
	struct  win_txt *txts;  /*тексты*/
	int     ntext;          /*номер строки*/
	int     i;
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

/* инициализация полей inp_lst */
for(inp=inp_lst; inp->str; inp++){
  inp->fld_len=strlen(win->txts[inp->npunkt].txt);
  for(x=0;inp->str[x];x++);
  while(x<inp->str_len) inp->str[x++]=' ';
  inp->str[(inp->str_len)-1]='\0';
  str_to_field(win->txts[inp->npunkt].txt, inp->str, 0);
}

if(nosave_ask==0){
  delsoob();
  overwrite(curscr, save_scr);
  overwrite(curscr, stdscr);
  clearok(stdscr,FALSE);
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

c_win=subwin(stdscr,win->lines,win->cols,win->y,win->x);
mywbkgd(c_win, *(win->attr));
leaveok(c_win, FALSE);

wattrset(c_win, *(win->attr_box));
box(c_win, 0,0);
refresh();

x=0;    /* теперь это позиция в строке ввода*/
outwin(win,curr,pred,c_win); /*вывод текста в окне, раньше не выделенного*/

while(!stop){
    for(inp=inp_lst; inp->str && curr!=inp->npunkt; inp++);
    if(inp->str){ /* текущий пункт - поле ввода */
      wmove(c_win, win->txts[inp->npunkt].y, win->txts[inp->npunkt].x+x-first);
      wrefresh(c_win);
    }

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
	  if(inp->str) /* текущий пункт - поле ввода curr==inp->npunkt */
	    { /* перемещения по строке ввода */
	      edited=1;
	      if(x < inp->str_len){ /* есть куда перемещаться вправо */
		x++;
		if(x>first+inp->fld_len){ /* сместить видимую область строки */
		  first++;
		  str_to_field(win->txts[curr].txt, inp->str, first);
		  outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
		}
	      }
	    }else{ /* перемещения по пунктам меню */
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
	    }
	break;

	case K_SD:  /*стрелка вниз*/
	case KEY_DOWN:  /*стрелка вниз*/
	  if(type&VERT)
	    { /*разрешено перемещение по стрелкам вверх/вниз*/
	      edited=0;
	      first=0;
	      x=0;
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
	  if(inp->str) /* текущий пункт - поле ввода curr==inp->npunkt */
	    { /* перемещения по строке ввода */
	      edited=1;
	      if(x){ /* есть куда перемещаться влево */
		x--;
		if(x<first){ /* сместить видимую область строки */
		  first--;
		  str_to_field(win->txts[curr].txt, inp->str, first);
		  outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
		}
	      }
	    }else{ /* перемещения по пунктам меню */
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
	    }
	break;

	case K_SU:    /*стрелка вверх*/
	case KEY_UP:    /*стрелка вверх*/
	  if(type&VERT)
	    { /*разрешено перемещение по стрелкам вверх/вниз*/
	      edited=0;
	      first=0;
	      x=0;
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

	case K_PAGE_UP:
	case KEY_PPAGE:
	  pred=curr;
	  for(y=0; y<win->npunkt; y++){
	    if(win->txts[y].active && win->txts[y].y<win->txts[curr].y) curr=y;
	  }
	  outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	break;


	case K_PAGE_DN:
	case KEY_NPAGE:
	  pred=curr;
	  for(y=0; y<win->npunkt; y++){
	    if(win->txts[y].active && win->txts[y].y>win->txts[curr].y) curr=y;
	  }
	  outwin(win,curr,pred,c_win);  /*высветить новый пункт и погасить старый*/
	break;

	case K_HOME:
	case KEY_HOME:
	  if(inp->str) /* текущий пункт - поле ввода curr==inp->npunkt */
	    { /* перемещения по строке ввода */
	      edited=1;
	      if(x){ /* есть куда перемещаться влево */
		x=0;
		if(first){ /* сместить видимую область строки */
		  first=0;
		  str_to_field(win->txts[curr].txt, inp->str, first);
		  outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
		}
	      }
	    }else{ /* перемещения по пунктам меню */
	       beep();
	    }
	break;

	case K_END:
	case KEY_END:
	  if(inp->str) /* текущий пункт - поле ввода curr==inp->npunkt */
	    { /* перемещения по строке ввода */
	      edited=1;
	      for(x=(inp->str_len)-2 ; inp->str[x]==' ' && x ; x--);
	      if(inp->str[x]!=' ') x++;
	      if(x==inp->str_len) x=0;
	      if(x<first){ /* сместить видимую область строки */
		first=x;
		if(first) first--;
		str_to_field(win->txts[curr].txt, inp->str, first);
		outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
	      }else if(x>=first+inp->fld_len){ /* сместить видимую область строки */
		first=x-inp->fld_len;
		str_to_field(win->txts[curr].txt, inp->str, first);
		outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
	      }
	    }else{ /* перемещения по пунктам меню */
	       beep();
	    }
	break;

	case K_DELETE: /* клавиша DELETE */
	case DELETE: /* клавиша DELETE */
	  if(inp->str) /* текущий пункт - поле ввода curr==inp->npunkt */
	    { /* перемещения по строке ввода */
	      edited=1;
	      for(y=x; y<inp->str_len-2; y++) inp->str[y]=inp->str[y+1];
	      inp->str[inp->str_len-2]=' ';
	      inp->str[inp->str_len-1]='\0';
	      str_to_field(win->txts[curr].txt, inp->str, first);
	      outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
	    }else{
	       beep();
	    }

	break;

	case K_BACKSPACE:
	case BACKSP: /* клавиша BACKSPACE */
	  if(inp->str && x) /* текущий пункт - поле ввода curr==inp->npunkt */
	    { /* перемещения по строке ввода */
	      edited=1;
	      for(y=x; y<inp->fld_len-1; y++) inp->str[y-1]=inp->str[y];
	      inp->str[inp->str_len-2]=' ';
	      inp->str[inp->str_len-1]='\0';
	      x--;
	      if(x<first) first--;
	      str_to_field(win->txts[curr].txt, inp->str, first);
	      outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
	    }else{
	       beep();
	    }

	break;

	case K_ESC:
	case K_F0:
	case ESC:       /*конец работы*/
	case KEY_F(0):
	  curr=(-1);
	  stop++;
	break;

	case K_CR:        /*выбор пункта меню*/
	case K_LF:        /*выбор пункта меню*/
	case ENTER:     /*выбор пункта меню*/
	   stop++;
	break;

	default:
	  if(use_keycap && (ch&SIMBTYPE)==PECH_SIMB) ch&=SIMBOL;

	  if(inp->str && my_isprint(ch)) /* текущий пункт - поле ввода curr==inp->npunkt */
	    { /* работа в строке ввода */
	      if(x<inp->str_len){ /* есть куда перемещаться вправо */
		if(!edited){ /* ввод первого символа - очистка строки*/
		  edited=1;
		  for(x=0;x<inp->str_len;x++) inp->str[x]=' ';
		  inp->str[inp->str_len-1]='\0';
		  x=0;
		}
		for(i=inp->str_len-2; i>x; i--) inp->str[i]=inp->str[i-1];
		inp->str[x]=ch;
		x++;
		if(x>first+inp->fld_len){ /* сместить видимую область строки */
		  first++;
		}
		str_to_field(win->txts[curr].txt, inp->str, first);
		outwin(win,curr,-2,c_win); /* вывод только строки ввода*/
	      }
	    }else{ /* перемещения по пунктам меню */
	      beep();
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
for(inp=inp_lst; inp->str; inp++){
  for(x=(inp->str_len)-2 ; inp->str[x]==' ' && x ; x--);
  if(inp->str[x]!=' ') x++;
  inp->str[x]='\0';
}
return(curr);
}

/****************************************************************************/
		str_to_field(field, str, first)
/*              Копирование видимой части строки в область ввода
/****************************************************************************/
	char    *field;         /* место области ввода */
	char    *str;           /* строка, из которой берется видимая часть */
	int     first;          /* номер первого символа */
{
/*==========================================================================*/
str+=first;
while(*field){
  *field++ = *str++;
}
}
