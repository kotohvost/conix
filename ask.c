#include "defs.h"
/****************************************************************************/
		int     ask(win,curr,type)
/*              ����� �� �㭪�� ����
*                                       ���⮢ �.�.
*****************************************************************************/
       struct  win     *win;           /*����� �� ������*/
       int     curr;                   /*����� ⥪�饣� �㭪� ����*/
       int      type;           /*⨯ ���室��: HOR-��ਧ���.,VERT-���⨪���, OUT-� ��室��*/
{
extern  unsigned inmenu;                /*��� ����樨 ������ ������ */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  chtype  atr[];          /* ���᪠ */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  int     use_colors;     /* �ਧ��� �ᯮ�짮����� 梥⮢ */
	chtype  ch;                     /*��� ����⮩ ������ */
	int     stop=0;
	int     pred=-1;                /*����� �।��饣� �㭪� ����*/
	WINDOW  *c_win;
	short   y;
	short   x;
	int     i;
	char    *txt;
	struct  win_txt *txts;  /*⥪���*/
	int     ntext;          /*����� ��ப�*/
/*==========================================================================*/
inmenu=0;
if(nosave_ask==0){
  delsoob();
  overwrite(curscr, save_scr);
  overwrite(curscr, stdscr);
  clearok(stdscr,FALSE);
}

/* ���᫨�� ���न���� ��ࠢ������� */
if(win->x==(-1)){ /* ��஢���� ���� �� �।���*/
  win->x=(COLS-win->cols)/2;
}
if(win->y==(-1)){ /* ��஢���� ���� �� �।���*/
  win->y=(LINES-win->lines-3)/2;
  if(win->y<0) win->y=0;
}
txts=win->txts;
for(ntext=0;ntext<win->npunkt;ntext++){
  if(txts[ntext].x==(-1)){ /* ��஢���� �� �।��� ���� */
    txts[ntext].x=(win->cols-strlen(txts[ntext].txt))/2;
  }
}

c_win=subwin(stdscr,win->lines,win->cols,win->y,win->x);
if(use_colors || *(win->attr)==A_NORMAL)
  { /* � 梥⭮� ०��� 䮭 ��ࠫ쭮 ���⠢����� �� 梥�� */
    mywbkgd(c_win, *(win->attr));
  }else{ /* �� �୮-����� ����� �������� 䮭 A_STANOUT */
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
  }

  wattrset(c_win, *(win->attr_box));
  box(c_win, 0,0);
}

outwin(win,curr,pred,c_win); /*�뢮� ⥪�� � ����, ࠭�� �� �뤥�������*/
refresh();

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
	break;

	case K_SD:  /*��५�� ����*/
	case KEY_DOWN:  /*��५�� ����*/
	  if(type&VERT)
	    { /*ࠧ�襭� ��६�饭�� �� ��५��� �����/����*/
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
	break;

	case K_SU:    /*��५�� �����*/
	case KEY_UP:    /*��५�� �����*/
	  if(type&VERT)
	    { /*ࠧ�襭� ��६�饭�� �� ��५��� �����/����*/
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

	case K_PAGE_DN:
	case KEY_NPAGE:
	  pred=curr;
	  for(y=0; y<win->npunkt; y++){
	    if(win->txts[y].active && win->txts[y].y>win->txts[curr].y) curr=y;
	  }
	  outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
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
	case ENTER:       /*�롮� �㭪� ����*/
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
	    if(stop) outwin(win,curr,pred,c_win);  /*��ᢥ��� ���� �㭪� � ������� ����*/
	    if(stop>1)  stop=0;
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
return(curr);
}
