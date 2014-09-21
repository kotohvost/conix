/*============================================================================
			     Функции листания файла
	      ******************************************************
	      *   viewfile()      -  Просмотр файла                *
	      *   linesback()     -  поиск смещения в файле на     *
	      *                      несколько строк назад         *
	      ******************************************************
============================================================================*/

#include "defs.h"
#include "recode.h"
#include <ctype.h>
#define FIRST   1       /* номер первой строки */
#define NONAME  (22+13) /* кол-во колонок, кот.не могут быть исп.для имени*/
/****************************************************************************/
				viewfile(name)
/*                              Просмотр файла
/****************************************************************************/
  unsigned char *name;          /* имя просматриваемого файла */
{
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
extern  int     use_colors;     /* признак использования цветов */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  chtype  atr[];          /* раскраска */
extern  struct win win57;       /* окно сообщения об ошибке открытия файла */
extern unsigned char *fnd_str;  /* строка для временных данных */
extern  size_t  fnd_len;        /* место, выделенное для fnd_str*/
extern  struct  win     win66;          /* окно с запросом */
extern  struct  win     win67;          /* окно с ошибкой */
extern  struct  win     win69;  /* окно с запросом потока перекодировки */
extern  struct  win     win70;  /* окно с запросом кода для перекодировки */
static  char    *dmenu="1+      - 2+      - 3+Next  - 4+      - 5+      - 6+      - 7+Search- 8+Recode- 9+      - 10+Quit ";
extern  int     fromcode;       /* тип кодировки, из которой перекодировать при просмотре*/
extern  int     tocode;         /* тип кодировки, в которую перекодировать при просмотре*/
	FILE    *file=NULL;
	int     stop=0;         /*признак окончания просмотра */
	int     ch;
	long    firstseek=0;    /* смещение в файле до первой строки */
	long    nextseek;       /* смещение на следующий экран*/
  unsigned char *ptr;           /* имя файла для вывода ошибки и для прохода по шаблону */
	int     len;            /* максимальная длина выводимой части имени*/
	int     x;
	int     y;
	int     x_beg=0;        /* сдвиг выводимой части текста */
	int     lines=LINES-2;    /* количество строк для вывода */
	long    firstline=1;    /* номер первой выводимой строки файла*/
	int     find;           /* признак поиска */
	long    srchlnseek;     /* смещение до начала строки при поиске*/
	long    srchseek;       /* смещение до символа после первого совпадения*/
	long    srchseeklast=(-1);/* смещение предыдущего поиска */
	int     srchln;         /* номер строки при поиске */
	int     old_nosave;     /* станое значение nosave_ask */
	int     i;
	int     next_srch=0;    /* признак продолжения поиска без запроса */
	int     sensitive;      /* признак чувствительности к регистру */
	int     upperch;        /* кол-во UPPER символов в образце*/
	int     lowerch;        /* кол-во lower символов в образце*/
 struct inp_lst inp_lst[2];     /* список ввода для inp_menu()*/
	int     num_indent;     /* количество совпадающих символов при поиске */
	int     find_index;     /* индекс сравниваемого символа в строке поиска*/
	int     reread_index;
	int     reread_chrs;
	int     last_char;
extern  long    linesback();    /* поиск смещения в файле на несколько строк назад */
/*==========================================================================*/

inp_lst[0].str=(char *)fnd_str;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* признак конца списка для inp_menu()*/

len=strlen(name);
if(!(file=fopen((char*)name, "r")))
  { /* не открывается файл */
    if(len>win57.cols-2)
      { /* имя целиком не помещается */
	ptr=name+(len-(win57.cols-2));
      }else{
	ptr=name;
      }
    win57.txts[1].txt=(char*)ptr;
    win57.txts[1].x=(-1); /* ask() выровняет */
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

      /* Заголовок просмотра */
      attrset(MAIN_PANEL_TOP_ATTR);
      move(0,0);
      addstr(" File: ");
      if(len<COLS-NONAME)
	{ /*имя помещается целиком */
	  ptr=name;
	}else{ /* помещается только часть имени */
	  ptr=name+(len-(COLS-NONAME))+3;
	  addstr("...");
	}
      addstr((char*)ptr);
      if(x_beg){
	printw("  Shift: %d", x_beg);
      }
      if(firstline)
	{ /* известен номер первой строки*/
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

#ifdef sparc    /* Сложности с отображением буквы А в альтернативной кодировке*/
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

      /* нарисовать строку с клавишами */
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
	{ /* такое бывает с Symlink на каталог */
	  ch=ESC;
	}else{
	  if(use_keycap)
	    { /*использовать свою настройку на клавиатуру */
	      ch=getkey_();
	    }else{ /* использовать стандартные средства curses */
	      ch=getch();
	    }
	}
      switch(ch)
	{
	  case K_SD:  /*стрелка вниз*/
	  case KEY_DOWN:  /*стрелка вниз*/
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


	  case K_SU:    /*стрелка вверх*/
	  case KEY_UP:    /*стрелка вверх*/
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
	      nextseek=firstseek; /* следующий цикл не должен завершиться раньше присвоения, но на всякий случай...*/
	      for(y=FIRST; y<(lines*2-1) && !feof(file); y++){
		while(getc(file)!='\n' && !feof(file));
		if(y==lines-1) nextseek=ftell(file);
	      }
	      if(!feof(file))
		{ /* нормально прочиталось 2 экрана */
		  firstseek=nextseek;
		  if(firstline) firstline+=lines-1;
		}else{ /* на следующем "экране" - конец файла, сдвинуться */
		  /* в y - количество строк до конца файла */
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
	      firstline=0; /* номер строки неопределен*/
	    }
	    if(firstseek==0L) firstline=1;
	  break;

	  case K_SR:      /* Стрелка вправо */
	  case KEY_RIGHT: /* Стрелка вправо */
	    srchseeklast=(-1);
	    x_beg++;
	  break;

	  case K_SL:      /* Стрелка влево */
	  case KEY_LEFT:  /* Стрелка влево */
	    srchseeklast=(-1);
	    if(x_beg) x_beg--;
	  break;

	  case K_F3:      /* продолжение поиска */
	  case KEY_F(3):
	    next_srch=1;
	  case K_F7:      /* поиск */
	  case KEY_F(7):
	    if(fnd_str[0] && fromcode!=tocode){ /* перекодировать строку обратно в экранный код */
	      for(i=0; fnd_str[i]; i++) fnd_str[i]=recode(fnd_str[i], fromcode, tocode);
	    }
	    if( (next_srch && fnd_str[0]) ||
		(inp_menu(inp_lst, &win66, 1, 0)==1 && fnd_str[0])){
	      if(fnd_str[0] && fromcode!=tocode){ /* перекодировать строку для поиска */
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

		/* кусок ch=getc(file); с откатами: */
		if(reread_chrs)
		  { /* имитация чтения совпадающего с шаблоном куска */
		    reread_chrs--;
		    if(reread_chrs)
		      { /* совпадающая часть */
			ch=fnd_str[reread_index];
			reread_index++;
		      }else{ /* символ, на котором было обнаружено различие */
			ch=last_char;
		      }
		  }else{ /* реальное чтение из файла */
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

		      if(fnd_str[find_index] == '\0'){ /* полное соответствие */
			find=1;
			srchseek=ftell(file)-strlen(fnd_str);
			srchseeklast=srchseek+1;
			firstseek=srchlnseek;
			firstline=srchln;
			if(srchseek-srchlnseek>=COLS-2)
			  { /* образец не виден на экране */
			    x_beg=srchseek-srchlnseek-(COLS/2);
			  }else{
			    x_beg=0;
			  }
		      }
		    }else{ /* несовпадающие символы */
		      if(find_index) { /* раньше были совпадения, откат*/
			reread_index=1;
			reread_chrs=find_index;
			last_char=ch;
			find_index=0;
		      }
		    }
		}
	      }
	      if(!find){ /* предупредить: не найдено */
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

	  case K_F8:      /* перекодировка */
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
		  /* пока ничего не надо*/
		break;
	      }

	  break;

	  case K_ESC:       /*конец работы*/
	  case ESC:         /*конец работы*/
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
    /* из-зи ошибок в curses или в описании терминала очистка вып-ся не тем цветом*/
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
/*              поиск смещения в файле на несколько строк назад
/****************************************************************************/
	FILE    *file;          /* листаемый файл */
	int     lines;          /* на сколько строк отмотать назад */
	long    firstseek;      /* текущее смещение в файле */
{
	int     stop=0;         /* признак останова */
	char    buf[1024];      /* буфер для чтения назад*/
	int     readed;         /* количество прочитенных байт */
	int     findbeg=0;      /* признак поиска начала строки */
	char    *ptr;           /* для поиска в буфере */
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
      if(lines==0) findbeg=1; /* продолжить просмотр для поиска начала строки*/
/* printf("f(1) first=%ld ptr-buf=%d\n", firstseek, ptr-buf);*/
    }
  }
  if(ptr==buf && firstseek==0L)
    { /* в любом случае дошли до начала файла, искать нечего */
      stop++;
    }else{
      if(lines==0)
	{ /* находимся на искомой строке*/
	  switch(findbeg)
	    {
	      case 0: /* начало строки еще не искали */
		findbeg=1;
	      break;
	      case -1: /* найдено начало искомой строки*/
		stop++;
/* printf("final first=%ld ", firstseek);*/
		firstseek+=((short)(ptr-buf))+2;
/* printf("final first=%ld ptr-buf=%d", firstseek, ptr-buf);
getchar();
*/              break;
	      case 1: /* идет поиск начала строки, искать в предыдующем блоке*/
	      break;
	    }
	}
    }
}
return(firstseek);
}

