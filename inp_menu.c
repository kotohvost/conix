#include "defs.h"
/****************************************************************************/
		inp_menu(inp_lst, win, curr, type)
/*              ����� � ����� � ��ப�� �����
/****************************************************************************/
struct  inp_lst *inp_lst;       /* ᯨ᮪ ����� */
struct  win     *win;           /*����� �� ������*/
	int     curr;           /*����� ⥪�饣� �㭪� ����*/
	int     type;           /*⨯ ���室��: HOR-��ਧ���.,VERT-���⨪���, OUT-� ��室��*/
{
extern  unsigned inmenu;                /*��� ����樨 ������ ������ */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  chtype  atr[];          /* ���᪠ */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
	int     ch;             /*��� ����⮩ ������ */
	int     stop=0;
	int     pred=(-1);      /*����� �।��饣� �㭪� ����*/
	WINDOW  *c_win;
	short   y;              /* ��� �ᮢ���� ⥭� */
	short   x;              /* ��� �ᮢ���� ⥭� � ��६�饭�� �� ��ப� �����*/
	short   edited=0;       /* �ਧ��� ।���஢���� ��ப� ����� */
	short   first=0;        /* ����� ��ࢮ�� �뢮������ ᨬ���� � ��ப� ����� */
struct  inp_lst *inp;           /* ��� ���᪠ �� ᯨ�� ����� */
	struct  win_txt *txts;  /*⥪���*/
	int     ntext;          /*����� ��ப�*/
	int     i;
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

/* ���樠������ ����� inp_lst */
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

c_win=subwin(stdscr,win->lines,win->cols,win->y,win->x);
mywbkgd(c_win, *(win->attr));
leaveok(c_win, FALSE);

wattrset(c_win, *(win->attr_box));
box(c_win, 0,0);
refresh();

x=0;    /* ⥯��� �� ������ � ��ப� �����*/
outwin(win,curr,pred,c_win); /*�뢮� ⥪�� � ����, ࠭�� �� �뤥�������*/

while(!stop){
    for(inp=inp_lst; inp->str && curr!=inp->npunkt; inp++);
    if(inp->str){ /* ⥪�騩 �㭪� - ���� ����� */
      wmove(c_win, win->txts[inp->npunkt].y, win->txts[inp->npunkt].x+x-first);
      wrefresh(c_win);
    }

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
	  if(inp->str) /* ⥪�騩 �㭪� - ���� ����� curr==inp->npunkt */
	    { /* ��६�饭�� �� ��ப� ����� */
	      edited=1;
	      if(x < inp->str_len){ /* ���� �㤠 ��६������ ��ࠢ� */
		x++;
		if(x>first+inp->fld_len){ /* ᬥ���� ������� ������� ��ப� */
		  first++;
		  str_to_field(win->txts[curr].txt, inp->str, first);
		  outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
		}
	      }
	    }else{ /* ��६�饭�� �� �㭪⠬ ���� */
	      if(type&HOR)
		{ /*ࠧ�襭� ��६�饭�� �� ��५��� ��ࠢ�/�����*/
		  pred=curr;
		  curr=calcnext(win,curr);
		  outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
		}else{
		  if(type&OUT)
		    { /*ࠧ�襭 ��室 �� ����饭�� ���ࠢ�����*/
		      stop++;
		      inmenu=ch;
		      curr=(-1);
		    }else{
		      beep();
		    }
		}
	    }
	break;

	case K_SD:  /*��५�� ����*/
	case KEY_DOWN:  /*��५�� ����*/
	  if(type&VERT)
	    { /*ࠧ�襭� ��६�饭�� �� ��५��� �����/����*/
	      edited=0;
	      first=0;
	      x=0;
	      pred=curr;
	      curr=calcnext(win,curr);
	      outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	    }else{
	      if(type&OUT)
		{ /*ࠧ�襭 ��室 �� ����饭�� ���ࠢ�����*/
		  stop++;
		  inmenu=ch;
		  curr=(-1);
		}else{
		  beep();
		}
	    }
	break;


	case K_SL:  /*��५�� �����*/
	case KEY_LEFT:  /*��५�� �����*/
	  if(inp->str) /* ⥪�騩 �㭪� - ���� ����� curr==inp->npunkt */
	    { /* ��६�饭�� �� ��ப� ����� */
	      edited=1;
	      if(x){ /* ���� �㤠 ��६������ ����� */
		x--;
		if(x<first){ /* ᬥ���� ������� ������� ��ப� */
		  first--;
		  str_to_field(win->txts[curr].txt, inp->str, first);
		  outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
		}
	      }
	    }else{ /* ��६�饭�� �� �㭪⠬ ���� */
	      if(type&HOR)
		{ /*ࠧ�襭� ��६�饭�� �� ��५��� ��ࠢ�/�����*/
		  pred=curr;
		  curr=calcpred(win,curr);
		  outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
		}else{
		  if(type&OUT)
		    { /*ࠧ�襭 ��室 �� ����饭�� ���ࠢ�����*/
		      stop++;
		      inmenu=ch;
		      curr=(-1);
		    }else{
		      beep();
		    }
		}
	    }
	break;

	case K_SU:    /*��५�� �����*/
	case KEY_UP:    /*��५�� �����*/
	  if(type&VERT)
	    { /*ࠧ�襭� ��६�饭�� �� ��५��� �����/����*/
	      edited=0;
	      first=0;
	      x=0;
	      pred=curr;
	      curr=calcpred(win,curr);
	      outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	    }else{
	      if(type&OUT)
		{ /*ࠧ�襭 ��室 �� ����饭�� ���ࠢ�����*/
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
	  outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	break;


	case K_PAGE_DN:
	case KEY_NPAGE:
	  pred=curr;
	  for(y=0; y<win->npunkt; y++){
	    if(win->txts[y].active && win->txts[y].y>win->txts[curr].y) curr=y;
	  }
	  outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	break;

	case K_HOME:
	case KEY_HOME:
	  if(inp->str) /* ⥪�騩 �㭪� - ���� ����� curr==inp->npunkt */
	    { /* ��६�饭�� �� ��ப� ����� */
	      edited=1;
	      if(x){ /* ���� �㤠 ��६������ ����� */
		x=0;
		if(first){ /* ᬥ���� ������� ������� ��ப� */
		  first=0;
		  str_to_field(win->txts[curr].txt, inp->str, first);
		  outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
		}
	      }
	    }else{ /* ��६�饭�� �� �㭪⠬ ���� */
	       beep();
	    }
	break;

	case K_END:
	case KEY_END:
	  if(inp->str) /* ⥪�騩 �㭪� - ���� ����� curr==inp->npunkt */
	    { /* ��६�饭�� �� ��ப� ����� */
	      edited=1;
	      for(x=(inp->str_len)-2 ; inp->str[x]==' ' && x ; x--);
	      if(inp->str[x]!=' ') x++;
	      if(x==inp->str_len) x=0;
	      if(x<first){ /* ᬥ���� ������� ������� ��ப� */
		first=x;
		if(first) first--;
		str_to_field(win->txts[curr].txt, inp->str, first);
		outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
	      }else if(x>=first+inp->fld_len){ /* ᬥ���� ������� ������� ��ப� */
		first=x-inp->fld_len;
		str_to_field(win->txts[curr].txt, inp->str, first);
		outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
	      }
	    }else{ /* ��६�饭�� �� �㭪⠬ ���� */
	       beep();
	    }
	break;

	case K_DELETE: /* ������ DELETE */
	case DELETE: /* ������ DELETE */
	  if(inp->str) /* ⥪�騩 �㭪� - ���� ����� curr==inp->npunkt */
	    { /* ��६�饭�� �� ��ப� ����� */
	      edited=1;
	      for(y=x; y<inp->str_len-2; y++) inp->str[y]=inp->str[y+1];
	      inp->str[inp->str_len-2]=' ';
	      inp->str[inp->str_len-1]='\0';
	      str_to_field(win->txts[curr].txt, inp->str, first);
	      outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
	    }else{
	       beep();
	    }

	break;

	case K_BACKSPACE:
	case BACKSP: /* ������ BACKSPACE */
	  if(inp->str && x) /* ⥪�騩 �㭪� - ���� ����� curr==inp->npunkt */
	    { /* ��६�饭�� �� ��ப� ����� */
	      edited=1;
	      for(y=x; y<inp->fld_len-1; y++) inp->str[y-1]=inp->str[y];
	      inp->str[inp->str_len-2]=' ';
	      inp->str[inp->str_len-1]='\0';
	      x--;
	      if(x<first) first--;
	      str_to_field(win->txts[curr].txt, inp->str, first);
	      outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
	    }else{
	       beep();
	    }

	break;

	case K_ESC:
	case K_F0:
	case ESC:       /*����� ࠡ���*/
	case KEY_F(0):
	  curr=(-1);
	  stop++;
	break;

	case K_CR:        /*�롮� �㭪� ����*/
	case K_LF:        /*�롮� �㭪� ����*/
	case ENTER:     /*�롮� �㭪� ����*/
	   stop++;
	break;

	default:
	  if(use_keycap && (ch&SIMBTYPE)==PECH_SIMB) ch&=SIMBOL;

	  if(inp->str && my_isprint(ch)) /* ⥪�騩 �㭪� - ���� ����� curr==inp->npunkt */
	    { /* ࠡ�� � ��ப� ����� */
	      if(x<inp->str_len){ /* ���� �㤠 ��६������ ��ࠢ� */
		if(!edited){ /* ���� ��ࢮ�� ᨬ���� - ���⪠ ��ப�*/
		  edited=1;
		  for(x=0;x<inp->str_len;x++) inp->str[x]=' ';
		  inp->str[inp->str_len-1]='\0';
		  x=0;
		}
		for(i=inp->str_len-2; i>x; i--) inp->str[i]=inp->str[i-1];
		inp->str[x]=ch;
		x++;
		if(x>first+inp->fld_len){ /* ᬥ���� ������� ������� ��ப� */
		  first++;
		}
		str_to_field(win->txts[curr].txt, inp->str, first);
		outwin(win,curr,-2,c_win); /* �뢮� ⮫쪮 ��ப� �����*/
	      }
	    }else{ /* ��६�饭�� �� �㭪⠬ ���� */
	      beep();
	    }
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
for(inp=inp_lst; inp->str; inp++){
  for(x=(inp->str_len)-2 ; inp->str[x]==' ' && x ; x--);
  if(inp->str[x]!=' ') x++;
  inp->str[x]='\0';
}
return(curr);
}

/****************************************************************************/
		str_to_field(field, str, first)
/*              ����஢���� ������� ��� ��ப� � ������� �����
/****************************************************************************/
	char    *field;         /* ���� ������ ����� */
	char    *str;           /* ��ப�, �� ���ன ������ ������� ���� */
	int     first;          /* ����� ��ࢮ�� ᨬ���� */
{
/*==========================================================================*/
str+=first;
while(*field){
  *field++ = *str++;
}
}
