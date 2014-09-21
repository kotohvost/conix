/*============================================================================
			     �㭪樨 ���⠭�� 䠩��
	      ******************************************************
	      *   viewfile()      -  ��ᬮ�� 䠩��                *
	      *   linesback()     -  ���� ᬥ饭�� � 䠩�� ��     *
	      *                      ��᪮�쪮 ��ப �����         *
	      ******************************************************
============================================================================*/

#include "defs.h"
#include "recode.h"
#include <ctype.h>
#define FIRST   1       /* ����� ��ࢮ� ��ப� */
#define NONAME  (22+13) /* ���-�� �������, ���.�� ����� ���� ��.��� �����*/
/****************************************************************************/
				viewfile(name)
/*                              ��ᬮ�� 䠩��
/****************************************************************************/
  unsigned char *name;          /* ��� ��ᬠ�ਢ������ 䠩�� */
{
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  int     use_colors;     /* �ਧ��� �ᯮ�짮����� 梥⮢ */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  chtype  atr[];          /* ���᪠ */
extern  struct win win57;       /* ���� ᮮ�饭�� �� �訡�� ������ 䠩�� */
extern unsigned char *fnd_str;  /* ��ப� ��� �६����� ������ */
extern  size_t  fnd_len;        /* ����, �뤥������ ��� fnd_str*/
extern  struct  win     win66;          /* ���� � ����ᮬ */
extern  struct  win     win67;          /* ���� � �訡��� */
extern  struct  win     win69;  /* ���� � ����ᮬ ��⮪� ��४���஢�� */
extern  struct  win     win70;  /* ���� � ����ᮬ ���� ��� ��४���஢�� */
static  char    *dmenu="1+      - 2+      - 3+Next  - 4+      - 5+      - 6+      - 7+Search- 8+Recode- 9+      - 10+Quit ";
extern  int     fromcode;       /* ⨯ ����஢��, �� ���ன ��४���஢��� �� ��ᬮ��*/
extern  int     tocode;         /* ⨯ ����஢��, � ������ ��४���஢��� �� ��ᬮ��*/
	FILE    *file=NULL;
	int     stop=0;         /*�ਧ��� ����砭�� ��ᬮ�� */
	int     ch;
	long    firstseek=0;    /* ᬥ饭�� � 䠩�� �� ��ࢮ� ��ப� */
	long    nextseek;       /* ᬥ饭�� �� ᫥���騩 �࠭*/
  unsigned char *ptr;           /* ��� 䠩�� ��� �뢮�� �訡�� � ��� ��室� �� 蠡���� */
	int     len;            /* ���ᨬ��쭠� ����� �뢮����� ��� �����*/
	int     x;
	int     y;
	int     x_beg=0;        /* ᤢ�� �뢮����� ��� ⥪�� */
	int     lines=LINES-2;    /* ������⢮ ��ப ��� �뢮�� */
	long    firstline=1;    /* ����� ��ࢮ� �뢮����� ��ப� 䠩��*/
	int     find;           /* �ਧ��� ���᪠ */
	long    srchlnseek;     /* ᬥ饭�� �� ��砫� ��ப� �� ���᪥*/
	long    srchseek;       /* ᬥ饭�� �� ᨬ���� ��᫥ ��ࢮ�� ᮢ�������*/
	long    srchseeklast=(-1);/* ᬥ饭�� �।��饣� ���᪠ */
	int     srchln;         /* ����� ��ப� �� ���᪥ */
	int     old_nosave;     /* �⠭�� ���祭�� nosave_ask */
	int     i;
	int     next_srch=0;    /* �ਧ��� �த������� ���᪠ ��� ����� */
	int     sensitive;      /* �ਧ��� ���⢨⥫쭮�� � ॣ����� */
	int     upperch;        /* ���-�� UPPER ᨬ����� � ��ࠧ�*/
	int     lowerch;        /* ���-�� lower ᨬ����� � ��ࠧ�*/
 struct inp_lst inp_lst[2];     /* ᯨ᮪ ����� ��� inp_menu()*/
	int     num_indent;     /* ������⢮ ᮢ������� ᨬ����� �� ���᪥ */
	int     find_index;     /* ������ �ࠢ��������� ᨬ���� � ��ப� ���᪠*/
	int     reread_index;
	int     reread_chrs;
	int     last_char;
extern  long    linesback();    /* ���� ᬥ饭�� � 䠩�� �� ��᪮�쪮 ��ப ����� */
/*==========================================================================*/

inp_lst[0].str=(char *)fnd_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

len=strlen(name);
if(!(file=fopen((char*)name, "r")))
  { /* �� ���뢠���� 䠩� */
    if(len>win57.cols-2)
      { /* ��� 楫���� �� ����頥��� */
	ptr=name+(len-(win57.cols-2));
      }else{
	ptr=name;
      }
    win57.txts[1].txt=(char*)ptr;
    win57.txts[1].x=(-1); /* ask() ��஢��� */
    ask(&win57, 2, 0);
  }else{
    old_nosave=nosave_ask;
    nosave_ask=1;
    if(old_nosave==0){
      delsoob();
      overwrite(curscr, save_scr);
    }
    attrset(MAIN_PANEL_ATTR);
    mywbkgd(stdscr, MAIN_PANEL_ATTR);
    clearok(stdscr, TRUE);

    stop=0;
    while(!stop){
      mywbkgd(stdscr, MAIN_PANEL_ATTR);

      /* ��������� ��ᬮ�� */
      attrset(MAIN_PANEL_TOP_ATTR);
      move(0,0);
      addstr(" File: ");
      if(len<COLS-NONAME)
	{ /*��� ����頥��� 楫���� */
	  ptr=name;
	}else{ /* ����頥��� ⮫쪮 ���� ����� */
	  ptr=name+(len-(COLS-NONAME))+3;
	  addstr("...");
	}
      addstr((char*)ptr);
      if(x_beg){
	printw("  Shift: %d", x_beg);
      }
      if(firstline)
	{ /* �����⥭ ����� ��ࢮ� ��ப�*/
	  printw(" Line: %d", firstline);
	}else{
	  printw(" Line: ?", firstline);
	}
      getyx(stdscr, y, x);
      while(x++<COLS) addch(' ');

      attrset(MAIN_PANEL_ATTR);

      y=FIRST;
      fseek(file, firstseek, 0);
      while(feof(file)==0 && ferror(file)==0 && y<FIRST+lines){
	x=0;
	do{
	  ch=getc(file);
	  if(feof(file)==0 && ferror(file)==0){
	    if((x-x_beg)==COLS)
	      {
		attrset(MAIN_PANEL_PTR_ATTR);
		move(y, COLS-1);
		addch('>');
		attrset(MAIN_PANEL_ATTR);
	      }else{
		if((x-x_beg)<COLS && ch!='\n'){
		  if(ch=='\t'){
		    x=((x+8)/8)*8-1;
		    ch=' ';
		  }
		  if(x>=x_beg){
		    move(y, (x-x_beg));
		    if(ch<' ' || ch==0177)
		      {
			attrset(MAIN_PANEL_PTR_ATTR);
			ch=CTRL(ch) +'@';
			addch(ch);
			attrset(MAIN_PANEL_ATTR);
		      }else{
			if(fromcode!=tocode) ch=recode(ch, fromcode, tocode);

#ifdef sparc    /* �������� � �⮡ࠦ����� �㪢� � � ����ୠ⨢��� ����஢��*/
			if((unsigned)ch == 0200) ch='A';
#endif
			addch(ch);
		      }
		  }
		}
	      }
	    x++;
	  }
	}while(ch!='\n' && feof(file)==0 && ferror(file)==0);
	y++;
      }

      /* ���ᮢ��� ��ப� � �����蠬� */
      move(LINES-1, 0);
      attrset(BOTT_LINE_KEYS_ATTR);
      for(ptr=(unsigned char*)dmenu;*ptr;ptr++){
	switch(*ptr)
	  {
	    case '+':
	      attrset(BOTT_LINE_TEXT_ATTR);
	    break;
	    case '-':
	      attrset(BOTT_LINE_KEYS_ATTR);
	    break;
	    default:
	      addch(*ptr);
	    break;
	  }
      }

      refresh();

      if(ferror(file))
	{ /* ⠪�� �뢠�� � Symlink �� ��⠫�� */
	  ch=ESC;
	}else{
	  if(use_keycap)
	    { /*�ᯮ�짮���� ᢮� ����ன�� �� ���������� */
	      ch=getkey_();
	    }else{ /* �ᯮ�짮���� �⠭����� �।�⢠ curses */
	      ch=getch();
	    }
	}
      switch(ch)
	{
	  case K_SD:  /*��५�� ����*/
	  case KEY_DOWN:  /*��५�� ����*/
	    srchseeklast=(-1);
	    if(!feof(file)){
		fseek(file, firstseek, 0);
		do{
		  ch=getc(file);
		}while(ch!='\n' && !feof(file));
		if(!feof(file)){
		  firstseek=ftell(file);
		  if(firstline) firstline++;
		}
	    }
	  break;


	  case K_SU:    /*��५�� �����*/
	  case KEY_UP:    /*��५�� �����*/
	    srchseeklast=(-1);
	    if(firstseek){
	      firstseek=linesback(file, 1, firstseek);
	      if(firstline>1) firstline--;
	    }
	    if(firstseek==0L) firstline=1;
	  break;

	  case K_PAGE_UP:
	  case KEY_PPAGE:
	    srchseeklast=(-1);
	    if(firstseek){
	      firstseek=linesback(file, lines-1, firstseek);
	      if(firstline){
		firstline-=lines-1;
		if(firstline<=0) firstline=1;
	      }
	    }
	    if(firstseek==0L) firstline=1;
	  break;

	  case K_PAGE_DN:
	  case KEY_NPAGE:
	    srchseeklast=(-1);
	    if(!feof(file)){
	      fseek(file, firstseek, 0);
	      nextseek=firstseek; /* ᫥���騩 横� �� ������ ���������� ࠭�� ��᢮����, �� �� ��直� ��砩...*/
	      for(y=FIRST; y<(lines*2-1) && !feof(file); y++){
		while(getc(file)!='\n' && !feof(file));
		if(y==lines-1) nextseek=ftell(file);
	      }
	      if(!feof(file))
		{ /* ��ଠ�쭮 ���⠫��� 2 �࠭� */
		  firstseek=nextseek;
		  if(firstline) firstline+=lines-1;
		}else{ /* �� ᫥���饬 "�࠭�" - ����� 䠩��, ᤢ������� */
		  /* � y - ������⢮ ��ப �� ���� 䠩�� */
		  fseek(file, firstseek, 0);
		  for(; y>lines; y--){
		    while(getc(file)!='\n' && !feof(file));
		    if(firstline) firstline++;
		  }
		  firstseek=ftell(file);
		}
	    }
	  break;

	  case K_HOME:
	  case KEY_HOME:
	    srchseeklast=(-1);
	    firstseek=0L;
	    firstline=1;
	  break;

	  case K_END:
	  case KEY_END:
	    srchseeklast=(-1);
	    if(!feof(file)){
	      fseek(file, 0L, 2);
	      firstseek=ftell(file);
	      firstseek=linesback(file, lines-1, firstseek);
	      firstline=0; /* ����� ��ப� ����।����*/
	    }
	    if(firstseek==0L) firstline=1;
	  break;

	  case K_SR:      /* ��५�� ��ࠢ� */
	  case KEY_RIGHT: /* ��५�� ��ࠢ� */
	    srchseeklast=(-1);
	    x_beg++;
	  break;

	  case K_SL:      /* ��५�� ����� */
	  case KEY_LEFT:  /* ��५�� ����� */
	    srchseeklast=(-1);
	    if(x_beg) x_beg--;
	  break;

	  case K_F3:      /* �த������� ���᪠ */
	  case KEY_F(3):
	    next_srch=1;
	  case K_F7:      /* ���� */
	  case KEY_F(7):
	    if(fnd_str[0] && fromcode!=tocode){ /* ��४���஢��� ��ப� ���⭮ � �࠭�� ��� */
	      for(i=0; fnd_str[i]; i++) fnd_str[i]=recode(fnd_str[i], fromcode, tocode);
	    }
	    if( (next_srch && fnd_str[0]) ||
		(inp_menu(inp_lst, &win66, 1, 0)==1 && fnd_str[0])){
	      if(fnd_str[0] && fromcode!=tocode){ /* ��४���஢��� ��ப� ��� ���᪠ */
		for(i=0; fnd_str[i]; i++) fnd_str[i]=recode(fnd_str[i], tocode, fromcode);
	      }
	      for(upperch=lowerch=i=0; fnd_str[i]; i++){
		if(isalpha(fnd_str[i])){
		  if(isupper(fnd_str[i])) upperch++;
		  if(islower(fnd_str[i])) lowerch++;
		}
	      }
	      sensitive = upperch && lowerch;
	      if(!sensitive){
		for(i=0; fnd_str[i]; i++){
		  if(isalpha(fnd_str[i]) && isupper(fnd_str[i])) fnd_str[i] = tolower(fnd_str[i]);
		}
	      }

	      srchlnseek=firstseek;
	      srchln=firstline;
	      find=0;
	      if(srchseeklast>0)
		{
		  fseek(file, srchseeklast, 0);
		}else{
		  fseek(file, srchlnseek, 0);
		}
	      find_index  = 0;
	      reread_index= 0;
	      reread_chrs = 0;
	      last_char   = '\0';
	      while(feof(file)==0 && ferror(file)==0 && find==0){

		/* ��᮪ ch=getc(file); � �⪠⠬�: */
		if(reread_chrs)
		  { /* ������ �⥭�� ᮢ�����饣� � 蠡����� ��᪠ */
		    reread_chrs--;
		    if(reread_chrs)
		      { /* ᮢ������� ���� */
			ch=fnd_str[reread_index];
			reread_index++;
		      }else{ /* ᨬ���, �� ���஬ �뫮 �����㦥�� ࠧ��稥 */
			ch=last_char;
		      }
		  }else{ /* ॠ�쭮� �⥭�� �� 䠩�� */
		    ch=getc(file);
		  }
		/*----------------------------------*/

		if(!feof(file)){

		  if(sensitive==0 && isalpha(ch) ) ch=tolower(ch);

		  if(ch=='\n'){
		    if(srchln) srchln++;
		    srchlnseek=ftell(file);
		  }


		  if( ch == fnd_str[find_index] )
		    {
		      find_index++;

		      if(fnd_str[find_index] == '\0'){ /* ������ ᮮ⢥��⢨� */
			find=1;
			srchseek=ftell(file)-strlen(fnd_str);
			srchseeklast=srchseek+1;
			firstseek=srchlnseek;
			firstline=srchln;
			if(srchseek-srchlnseek>=COLS-2)
			  { /* ��ࠧ�� �� ����� �� �࠭� */
			    x_beg=srchseek-srchlnseek-(COLS/2);
			  }else{
			    x_beg=0;
			  }
		      }
		    }else{ /* ��ᮢ�����騥 ᨬ���� */
		      if(find_index) { /* ࠭�� �뫨 ᮢ�������, �⪠�*/
			reread_index=1;
			reread_chrs=find_index;
			last_char=ch;
			find_index=0;
		      }
		    }
		}
	      }
	      if(!find){ /* �।�।���: �� ������� */
		if(sensitive)
		  {
		    win67.txts[1].txt="(Case sensitive)";
		  }else{
		    win67.txts[1].txt="";
		  }
		win67.txts[1].x = -1;

		ask(&win67, 2, 0);
	      }
	    }
	    next_srch=0;
	  break;

	  case K_F8:      /* ��४���஢�� */
	  case KEY_F(8):
	    switch(ask(&win69, 1, VERT))
	      {
		case 1: /* From */
		  win70.txts[0].txt="Recode FROM";
		  win70.txts[0].x=(-1);
		  ch=ask(&win70, fromcode+1, VERT);
		  if(ch>=1 && ch<=NUMCODES){
		    fnd_str[0]='\0';
		    fromcode=ch-1;
		  }
		break;

		case 2: /* To */
		  win70.txts[0].txt="Recode TO";
		  win70.txts[0].x=(-1);
		  ch=ask(&win70, tocode+1, VERT);
		  if(ch>=1 && ch<=NUMCODES){
		    fnd_str[0]='\0';
		    tocode=ch-1;
		  }
		break;

		default:
		  /* ���� ��祣� �� ����*/
		break;
	      }

	  break;

	  case K_ESC:       /*����� ࠡ���*/
	  case ESC:         /*����� ࠡ���*/
	  case K_F0:
	  case KEY_F(0):
	    stop++;
	  break;

	}

    } /* while !stop */

    fclose(file);

    mywbkgd(stdscr, A_NORMAL);
    wattrset(stdscr, A_NORMAL);
/* #ifdef sun */
    /* ��-�� �訡�� � curses ��� � ���ᠭ�� �ନ���� ���⪠ ��-�� �� ⥬ 梥⮬*/
    move(0,0);
    addch(' ');
    refresh();
/* #endif */
    clear();
    refresh();
    overwrite(save_scr, stdscr);
#ifdef FREEBSD
    touchwin(stdscr);
#endif
    clearok(stdscr, TRUE);
    refresh();
    if(old_nosave==0) nosave_ask=0;
  }
}


/****************************************************************************/
			long    linesback(file, lines, firstseek)
/*              ���� ᬥ饭�� � 䠩�� �� ��᪮�쪮 ��ப �����
/****************************************************************************/
	FILE    *file;          /* ���⠥�� 䠩� */
	int     lines;          /* �� ᪮�쪮 ��ப �⬮��� ����� */
	long    firstseek;      /* ⥪�饥 ᬥ饭�� � 䠩�� */
{
	int     stop=0;         /* �ਧ��� ��⠭��� */
	char    buf[1024];      /* ���� ��� �⥭�� �����*/
	int     readed;         /* ������⢮ ���⥭��� ���� */
	int     findbeg=0;      /* �ਧ��� ���᪠ ��砫� ��ப� */
	char    *ptr;           /* ��� ���᪠ � ���� */
/*==========================================================================*/
while(!stop){
/* printf("first=%ld ", firstseek);*/
  firstseek-=sizeof(buf);
/* printf("first=%ld\n", firstseek);*/
  if(firstseek<0)
    {
      ptr=buf+(firstseek+sizeof(buf))-1;
      firstseek=0L;
      rewind(file);
      readed=fread(buf, 1, sizeof(buf), file);
    }else{
      fseek(file, firstseek, 0);
      readed=fread(buf, 1, sizeof(buf), file);
      ptr=buf+readed-1;
    }
  if(!readed){
    endwin();
    fprintf(stderr,"Programmer's error in linesback()\n");
    exit(-1);
  }

  for(;(lines || findbeg>0) && ptr!=buf; ptr--){
    if(findbeg && *ptr=='\n'){
 findbeg=(-1);
/* printf("f(-1) first=%ld ptr-buf=%d\n", firstseek, ptr-buf);*/
}
    if(findbeg==0 && *ptr=='\n'){
      lines--;
      if(lines==0) findbeg=1; /* �த������ ��ᬮ�� ��� ���᪠ ��砫� ��ப�*/
/* printf("f(1) first=%ld ptr-buf=%d\n", firstseek, ptr-buf);*/
    }
  }
  if(ptr==buf && firstseek==0L)
    { /* � �� ��砥 ��諨 �� ��砫� 䠩��, �᪠�� ��祣� */
      stop++;
    }else{
      if(lines==0)
	{ /* ��室���� �� �᪮��� ��ப�*/
	  switch(findbeg)
	    {
	      case 0: /* ��砫� ��ப� �� �� �᪠�� */
		findbeg=1;
	      break;
	      case -1: /* ������� ��砫� �᪮��� ��ப�*/
		stop++;
/* printf("final first=%ld ", firstseek);*/
		firstseek+=((short)(ptr-buf))+2;
/* printf("final first=%ld ptr-buf=%d", firstseek, ptr-buf);
getchar();
*/              break;
	      case 1: /* ���� ���� ��砫� ��ப�, �᪠�� � �।���饬 �����*/
	      break;
	    }
	}
    }
}
return(firstseek);
}

