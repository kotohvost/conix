#include "defs.h"

/*============================================================================
			     Функции меню Commands
	      ******************************************************
	      *   swappanels()    - Смена панелей местами          *
	      *   on_off_panels() - Включение/выключение панелей   *
	      *   compare_dirs() - Сравнение каталогов             *
	      *   histlist()     - Листание и выбор истории        *
	      *   doublewidth()  - Одинарная/двойная ширина панелей*
	      ******************************************************
============================================================================*/

/****************************************************************************/
			swappanels()
/*                      Смена панелей местами
*****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  WINDOW  *dn_menu;       /* нижнее меню */
extern  WINDOW  *inp_win;       /* окно ввода команд */
	struct  panel   tmp_panel;
	WINDOW  *tmp_win;
/*==========================================================================*/
  tmp_panel=panels[0];
  panels[0]=panels[1];
  panels[1]=tmp_panel;

  tmp_win=panels[0].win;
  panels[0].win=panels[1].win;
  panels[1].win=tmp_win;

  outpanel=cpanel;
  cpanel++;
  if(cpanel==NPANELS) cpanel=0;
  if(panels[cpanel].on<=0) cpanel=outpanel;

  erase();
  for(outpanel=0;outpanel<NPANELS;outpanel++){
    refr_panel();  /* extern outpanel */
    if(panels[outpanel].on>0){
      /* wrefresh(panels[outpanel].win);*/
    }
  }
  touchwin(stdscr);
  refresh();
  touchwin(dn_menu);
  wrefresh(dn_menu);
  refr_inp();
  touchwin(inp_win);
  wrefresh(inp_win); /* курсор должен торчать в месте ввода команды! */
}


/****************************************************************************/
			on_off_panels()
/*                      Включение/выключение панелей
*****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  WINDOW  *dn_menu;       /* нижнее меню */
extern  WINDOW  *inp_win;       /* окно ввода команд */
extern  int     was_on;         /* в битах: 1<<номер_вкл_пан*/
	int     is_on;          /* в битах: 1<<номер_вкл_пан*/
/*==========================================================================*/

  is_on=0;
  for(outpanel=0;outpanel<NPANELS;outpanel++){
    if(panels[outpanel].on>0) is_on|=1<<outpanel;
  }

  if(is_on)
    { /* какие-то панели включены, выключить */
      was_on=is_on;
      erase();
      for(outpanel=0;outpanel<NPANELS;outpanel++){
	if(panels[outpanel].on>0) panels[outpanel].on *= -1;
      }
    }else{ /* нет включенных панелей, включить */
      erase();
      touchwin(stdscr);
      for(outpanel=0;outpanel<NPANELS;outpanel++){
	if( was_on==0 || (was_on&(1<<outpanel)) ){
	      panels[outpanel].on *= -1;
	      refr_panel();  /* extern outpanel */
	}
      }
    }
  refr_inp();
  refresh();
  touchwin(dn_menu);
  wrefresh(dn_menu);
  touchwin(inp_win);
  wrefresh(inp_win); /* курсор должен торчать в месте ввода команды! */
}

/****************************************************************************/
			compare_dirs()
/*                      Сравнение каталогов
*****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  win     win37;  /* сообщение об одинаковых каталогах */

extern  char    cmp_exist;      /* признак сравнения по наличию */
extern  char    cmp_time;       /* признак сравнения по времени */
extern  char    cmp_size;       /* признак сравнения по размеру */
extern  char    cmp_mode;       /* признак сравнения по доступу */
extern  char    cmp_body;       /* признак сравнения по содержимому */
extern  char    cmp_nlink;      /* признак сравнения по кол-ву ссылок */

 struct itembuf *item1;         /* элемент, с которым сравнивается */
 struct itembuf *item2;         /* элементы вторичной панели */
 struct itembuf *item2_end;     /* конец просмотра вторичного списка */
 struct itembuf *item2_beg;     /* начало просмотра вторичного списка */
	char    *name1;         /* имя файла из основной панели */
	char    *names2;        /* начало массива имен вторичной панели */
	int     second;         /* номер вторичной панели */
	int     nfile;          /* номер файла основной панели */
	int     differs=0;      /* количество различающихся файлов */
	int     find;           /* признак поиска файла с одинаковым именем */
	int     stop=0;
/*==========================================================================*/
second=(cpanel)?0:1;
names2=panels[second].names;
item2_end= &(panels[second].itembuf[panels[second].nfiles]);
item2_beg= &(panels[second].itembuf[2     ]);

for(nfile=2,item1=&(panels[cpanel].itembuf[2]);
    nfile<panels[cpanel].nfiles && stop==0;
    nfile++,item1++){
  name1=panels[cpanel].names+item1->nname;

  find=0;
  for(item2=item2_beg; item2!=item2_end && !find;){
    if(strcmp(name1, names2+item2->nname) == 0)
      {
	find++;
      }else{
	item2++;
      }
  }

  if(find)
    { /* найден файл с таким-же именем, как и name1 */

      if(item1->mtime > item2->mtime && cmp_time){
	/* item1 новее, чем item2 */
	  if(! item1->attrs&MARKED){
	    item1->attrs |= MARKED;
	    panels[cpanel].selected++;
	    panels[cpanel].size_sel+=item1->size;
	  }
	  differs++;
      }else if(item1->mtime < item2->mtime && cmp_time){
	/* item1 старее, чем item2 */
	  if(! item2->attrs&MARKED){
	    item2->attrs |= MARKED;
	    panels[second].selected++;
	    panels[second].size_sel+=item2->size;
	  }
	  differs++;
      }else if( (item1->size!=item2->size   && cmp_size)  ||
		(item1->nlink!=item2->nlink && cmp_nlink) ||
		(item1->mode!=item2->mode   && cmp_mode)){

	  /* другие различия при одинаковой дате */
	  if(! item1->attrs&MARKED){
	    item1->attrs |= MARKED;
	    panels[cpanel].selected++;
	    panels[cpanel].size_sel+=item1->size;
	  }
	  if(! item2->attrs&MARKED){
	    item2->attrs |= MARKED;
	    panels[second].selected++;
	    panels[second].size_sel+=item2->size;
	  }
	  differs++;
      }else if(cmp_body && filtype(item1->mode)==FTYPE_REG &&
			   filtype(item2->mode)==FTYPE_REG){
	switch(cmp_files(panels[cpanel].full_dir, name1,
			 panels[second].full_dir, names2+item2->nname))
	  {
	    case 0: /* одинаковые файлы */
	    break;
	    case 1: /* Различные файлы */
	    case 2: /* ошибка сравнения */
	      if(! item1->attrs&MARKED){
		item1->attrs |= MARKED;
		panels[cpanel].selected++;
		panels[cpanel].size_sel+=item1->size;
	      }
	      if(! item2->attrs&MARKED){
		item2->attrs |= MARKED;
		panels[second].selected++;
		panels[second].size_sel+=item2->size;
	      }
	      differs++;
	    break;
	    case 3: /* прекратить сравнение */
	      stop++;
	    break;
	    default:
	      endwin();
	      fprintf(stderr, "Programmer's error in compare_dirs()\n");
	    break;
	  }
      }
    }else{ /* такого файла не найдено */

      if(cmp_exist){
	if(cmp_exist && ! item1->attrs&MARKED){
	  item1->attrs |= MARKED;
	  panels[cpanel].selected++;
	  panels[cpanel].size_sel+=item1->size;
	}
	differs++;
      }
    }
}

if(cmp_exist){ /* найти файлы, имеющиеся во второй панели и отсутствующие в первой */
  names2=panels[cpanel].names;
  item2_end= &(panels[cpanel].itembuf[panels[cpanel].nfiles]);
  item2_beg= &(panels[cpanel].itembuf[2     ]);

  for(nfile=2,item1=&(panels[second].itembuf[2]);
      nfile<panels[second].nfiles;
      nfile++,item1++){
    name1=panels[second].names+item1->nname;

    find=0;
    for(item2=item2_beg; item2!=item2_end && !find;){
      if(strcmp(name1, names2+item2->nname) == 0)
	{
	  find++;
	}else{
	  item2++;
	}
    }

    if(!find){
      if(! item1->attrs&MARKED){
	item1->attrs |= MARKED;
	panels[second].selected++;
	panels[second].size_sel+=item1->size;
      }
      differs++;
    }
  }
}

if(differs)
  { /* есть различия */
    if(panels[cpanel].on<0) panels[cpanel].on *= -1;
    if(panels[second].on<0) panels[second].on *= -1;
    /* вывод изменений - в popdn() */
  }else{ /* различий не найдено */
    ask(&win37,2,0);
  }
delsoob();
}

/****************************************************************************/
			histlist()
/*                      Листание и выбор истории
*****************************************************************************/
{
extern  char    *history;       /* буфер хранения истории */
extern  size_t  his_len;        /* место, выделенное для history*/
extern  int     his_cur;        /* номер элемента history для заполнения */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  chtype  atr[];          /* раскраска */
extern struct   win win38;      /* окно истории */
extern struct   win win39;      /* окно сообщения об ошибке */
extern  char    *inp_str;       /* строка для ввода команд */
extern  size_t  inp_len;        /* место, выделенное для inp_str*/
extern  int     inp_first;      /* номер первого выведенного символа команды */
extern  int     inp_cur;        /* номер текущего символа команды */
extern  int     use_keycap;     /* признак использования своей настройки на клавиатуру */
	int     his_go;         /* номер элемента history */
	int     cnstr;          /* количество строк history */
	int     curnstr=0;      /* номер текущей строки history от конца */
	int     lastcnstr=0;    /* номер последней строки history от конца */
	WINDOW  *w;             /* рабочее окно */
	int     len;            /* длина выводимой строки истории */
	char    *ptr;
	short   lines;          /* макс количество строк в окне истории (все)*/
	short   npunkt;         /* макс количество строк в окне истории (активных)*/
	int     ch;                     /*код нажатой клавиши */
	int     stop=0;
	short   y;
	short   x;
	int     nstr;
/*==========================================================================*/
if(win38.x==(-1)){ /* выровнять окно на середину*/
  win38.x=(COLS-win38.cols)/2;
}
if(win38.y==(-1)){ /* выровнять окно на середину*/
  win38.y=(LINES-win38.lines)/2;
}

len=strlen(win38.txts[0].txt);
his_go=his_cur;

/* посчитать общее количество строк истории */
for(cnstr=0; (his_go=hispred(his_go))!=his_cur;cnstr++);

if(cnstr==0)
  { /* нет истории */
    ask(&win39, 1, 0);
  }else{
    if(nosave_ask==0){
      delsoob();
      overwrite(curscr, save_scr);
      overwrite(curscr, stdscr);
      clearok(stdscr,FALSE);
    }

    lines=win38.lines;
    npunkt=win38.npunkt;
    if(npunkt>cnstr)
      {
	win38.npunkt=cnstr;
	win38.lines-=npunkt-cnstr;
      }
    win38.y=(LINES-win38.lines)/2;
    win38.x=(COLS-win38.cols)/2;


    w=subwin(stdscr,win38.lines,win38.cols,win38.y,win38.x);
    mywbkgd(w, *(win38.attr));

    if(win38.attr_box != &NO_DRAW_ATTR){
      /* Рисование тени */
      /* вертикальная часть*/
      x=win38.x+win38.cols;
      for(y=win38.y+1; y<=win38.y+win38.lines; y++){
	  move(y,x);
	  ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	  addch(ch);
	  ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	  addch(ch);
      }

      /* горизонтальная часть*/
      y=win38.y+win38.lines;
      for(x=win38.x+2; x<=win38.x+win38.cols+1; x++){
	  move(y,x);
	  ch=(inch() & (A_CHARTEXT|A_ALTCHARSET)) | SHADOW_ATTR;
	  addch(ch);
      }

      wattrset(w, *(win38.attr_box));
      box(w, 0,0);
    }

    lastcnstr=0;

    while(!stop){

      his_go=his_cur;
      /* пропустить строки, не входящие в окно (снизу) */
      for(nstr=0;nstr!=lastcnstr;nstr++){
	his_go=hispred(his_go);
      }
      /* формирование строк в окне win38 */
      for(nstr=0; nstr<win38.npunkt; nstr++){
	his_go=hispred(his_go);
	strncpy(win38.txts[win38.npunkt-nstr-1].txt, &history[his_go], len);
	win38.txts[win38.npunkt-nstr-1].txt[len]='\0';
	for(x=strlen(win38.txts[win38.npunkt-nstr-1].txt);x<len;x++) win38.txts[win38.npunkt-nstr-1].txt[x]=' ';
      }
      outwin(&win38, win38.npunkt-(curnstr-lastcnstr)-1, -1, w);
      refresh();

      if(use_keycap)
	{ /*использовать свою настройку на клавиатуру */
	  ch=getkey_();
	}else{ /* использовать стандартные средства curses */
	  ch=getch();
	}
      switch(ch)
	{
	  case K_SD:  /*стрелка вниз*/
	  case KEY_DOWN:  /*стрелка вниз*/
	    if(curnstr) {
	      curnstr--;
	      if(curnstr<lastcnstr) lastcnstr--;
	    }
	  break;

	  case K_SU:    /*стрелка вверх*/
	  case KEY_UP:    /*стрелка вверх*/
	    if(curnstr<cnstr-1) {
	      curnstr++;
	      if(curnstr>=lastcnstr+win38.npunkt) lastcnstr++;
	    }
	  break;

	  case K_ESC:       /*конец работы*/
	  case ESC:       /*конец работы*/
	  case K_F0:
	  case KEY_F(0):
	    stop++;
	  break;

	  case K_CR:        /*выбор пункта меню*/
	  case K_LF:        /*выбор пункта меню*/
	  case ENTER:     /*выбор пункта меню*/
	     stop++;

	     his_go=his_cur;
	     for(nstr=0;nstr!=curnstr+1;nstr++){
	       his_go=hispred(his_go);
	     }

	     strcpy(inp_str, &history[his_go]);
	     for(inp_cur=strlen(inp_str);inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';

	     inp_str[inp_len-1]='\0';
	     inp_cur=0;
	     inp_first=0;
	     /* refr_inp(); - будет после восстановления экрана */
	  break;

	  default:
	    beep();
	  break;
	}
    } /*while(!stop)*/

    /*восстановить экран под окном */
    delwin(w);
    if(nosave_ask==0){
      overwrite(save_scr, stdscr);
      refresh();
    }
    refr_inp();
    /* восстановить параметры win38 для последующих вызовов*/
    win38.npunkt=npunkt;
    win38.lines=lines;
  }
}

/****************************************************************************/
			doublewidth()
/*                      Одинарная/двойная ширина панелей
*****************************************************************************/
{
extern  chtype  atr[NUM_ATTRS];   /* раскраска */
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  WINDOW  *dn_menu;       /* нижнее меню */
extern  WINDOW  *inp_win;       /* окно ввода команд */

extern  WINDOW  *dbl_win;       /* окно вывода панелей двойной ширины */
extern  int     dwidth;         /* признак панелей двойной ширины */
extern  WINDOW  *saved_win[NPANELS]; /* для сохранения указателей на окна панелей */

/*==========================================================================*/
dwidth = 1 - dwidth;
outpanel=cpanel;

if ( dwidth )
  { /* будет вывод с двойной шириной панелей */

    if ( !dbl_win ) { /* создать окно для двойной ширины панелей */
      dbl_win=subwin(stdscr, panels[outpanel].lines, COLS, 0, 0);
      tst_err_malloc(dbl_win,32); /* выполняет exit, если NULL*/
      wattrset(dbl_win, MAIN_PANEL_ATTR);
      mywbkgd(dbl_win, MAIN_PANEL_ATTR);
    }

    for ( outpanel=0; outpanel<NPANELS; outpanel++){
      saved_win[outpanel] = panels[outpanel].win;
      panels[outpanel].win = dbl_win;
      panels[outpanel].cols =COLS;
      if ( panels[outpanel].on == 2 ) panels[outpanel].on=1;
      /* panels[outpanel].firstfile=0; */
      /* panels[outpanel].lastfile=0; */
    }
    outpanel=cpanel;
    refr_panel();  /* extern outpanel */
    wrefresh(panels[cpanel].win);

  }else{ /* переключение с режима широких панелей на нормальные */

    erase();
    for ( outpanel=0; outpanel<NPANELS; outpanel++){
      panels[outpanel].cols = COLS/NPANELS;
      panels[outpanel].win = saved_win[outpanel];
      /* panels[outpanel].firstfile=0; */
      panels[outpanel].lastfile=0;

      refr_panel();  /* extern outpanel */
    }

    outpanel=cpanel;
    touchwin(stdscr);

  }

refr_inp();
refresh();
touchwin(dn_menu);
wrefresh(dn_menu);
touchwin(inp_win);
wrefresh(inp_win); /* курсор должен торчать в месте ввода команды! */

}

