#include "defs.h"
#include <sys/utsname.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define unset_alarms()  if(alarm_set) { alarm_set=0; alarm(0);}
	chtype  atr[NUM_ATTRS];   /* раскраска */
struct  panel   panels[NPANELS];
	int     cpanel=1; /* текущая панель*/
	int     outpanel;
	WINDOW  *dn_menu;       /* нижнее меню */
	WINDOW  *save_scr;      /* окно для сохранения экрана */
	WINDOW  *soob_win;      /* окно выдачи сообщений */
	WINDOW  *inp_win;       /* окно ввода команд */
	WINDOW  *clock_win;     /* окно вывода времени */
	WINDOW  *dbl_win;       /* окно вывода панелей двойной ширины */
	WINDOW  *saved_win[NPANELS]; /* для сохранения указателей на окна панелей */
	unsigned inmenu;        /*для имитации нажатия клавиши */
	char    *fnd_str;       /* строка для временных данных */
	size_t  fnd_len;        /* место, выделенное для fnd_str*/
	char    *tmp_str;       /* строка для временных данных */
	size_t  tmp_len;        /* место, выделенное для tmp_str*/
	char    *inp_str;       /* строка для ввода команд */
	size_t  inp_len;        /* место, выделенное для inp_str*/
	int     inp_first;      /* номер первого выведенного символа команды */
	int     inp_cur;        /* номер текущего символа команды */
	char    *history;       /* буфер хранения истории */
	size_t  his_len;        /* место, выделенное для history*/
	int     his_cur;        /* номер элемента history для заполнения */
	int     his_go=0;       /* номер элемента history */
	int     was_on=0;       /* в битах: 1<<номер_вкл_пан*/
	int     use_keycap=0;   /* признак использования своей настройки на клавиатуру */
	char    *last_search;   /* имя файла с последним результатом поиска */
	short   pred_file=(-1); /* номер передыдущего файла для refr_panel()*/
struct  fields  fields[MAXFIELDS]={
	{ FIELD_TYPE, 2          ,""    },        /* символ типа файла */
	{ FIELD_MARK, 1          ,""    },        /* символ отметки файла */
	{ FIELD_NAME, OUTNAMELEN ,"Name"},
	{ FIELD_SIZE, 9          ,"Size"},
	{ FIELD_DATE, 9          ,"Date"},
	{ FIELD_TIME, 6          ,"Time"},
	{ FIELD_INUM, 7          ,"Inum"},        /* номер inode */
	{ FIELD_MODE, 10         ,"Mode"},
	{ FIELD_OWNR, 6          ,"Owner"},
	{ FIELD_GRP , 6          ,"Group"},
	{ FIELD_NLNK, 5          ,"Link"}   };    /* поля вывода */
	int     nosave_ask=0;   /* признак "несохранения" экрана для ask */
	char    *keycapfile;    /* имя файла с описанием клавиатуры (формат TERMCAP)*/
	int     use_colors;     /* признак использования цветов */
	char    cmp_exist=1;    /* признак сравнения по наличию */
	char    cmp_time =1;    /* признак сравнения по времени */
	char    cmp_size =1;    /* признак сравнения по размеру */
	char    cmp_mode =1;    /* признак сравнения по доступу */
	char    cmp_body =0;    /* признак сравнения по содержимому */
	char    cmp_nlink=1;    /* признак сравнения по кол-ву ссылок */
	int     silent;         /* признак сравнения без сообщения об ошибках*/
	char    mach_name[SYS_NMLN];/* название машины */
	char    user_name[LOGNM_MAX]; /* имя пользователя */
	uid_t   user_eid;       /* эффективный UserID или 0, если getuid()==0*/
	int     prompt;         /* тип приглашения:0-нет, 1-NodeName */
	int     clock_need=0;   /* признак вывода времени 0-нет 1-да */
	int     alarmed=0;      /* признак того, что было прерывание по alarm()*/
	int     alarm_set=0;    /* признак того, что установлен будильник */
	int     fromcode;       /* тип кодировки, из которой перекодировать при просмотре*/
	int     tocode;         /* тип кодировки, в которую перекодировать при просмотре*/
/* Do not remove copyright, add your own if needed! */
static  char    long_title[]="Commander for Unix by S.Revtov V1.10 1997 (sir@cb.tver.ru)\n";
	char    shrt_title[]="conix V1.10";

	char    *helpfile;      /* файл со справкой */
	int     in_viewer;      /* признак вызова execcmd() в режиме просмотра */
	int     dwidth = 0;     /* признак панелей двойной ширины */


/****************************************************************************/
				main(argc, argv)
/****************************************************************************/
	int     argc;
	char    *argv[];
{
extern  struct  win     win1;   /* описание окна подтверждения выхода*/
	short   stop=0;         /* признак конца работы */
	int     ch;             /* символ с клавиатуры*/
 struct itembuf *item;          /* для обработки выбранного файла*/
	int     nfile;          /* номер текущего файла*/
	int     nfirst;         /* номер первого выводимого файла*/
	char    *ptr;
	int     find;           /* признак для поиска */
	int     i;
	struct stat stbuf;
	int     wasbs=0;        /* признак, что уже был звонок по BACKSPACE */
	DIR     *dirp;          /* для проверки читаемости каталога */
	struct dirent *dp;      /* аналогично */
static  char    name[]={ 'c'-'a','o'-'b','n'-'c','i'-'d','x'-'e'};
/*==========================================================================*/
printf(long_title);
for(ptr=argv[0]; *ptr;ptr++);
while(ptr!=argv[0] && *ptr!='/') ptr--;
if(*ptr=='/') ptr++;
for(ch='a',i=0; i<5; i++, ch++){
  if(name[i]+ch != ptr[i]){
    fprintf(stderr,"Illegal commander name, must begin with \"conix\"\n");
    exit(-1);
  }
}

init(); /* инициализация curses, настройка цветов, формирование панелей */

#ifdef NCURSES
touchwin(stdscr);
#endif

refresh();
wrefresh(dn_menu);
wrefresh(inp_win); /* курсор должен торчать в месте ввода команды! */

while(!stop){
  if(alarm_set==0 && clock_need){ /* запрос на alarm() был сброшен, а теперь нужен*/
    alarm_set=1;
    alarm(1);
  }

  if(use_keycap)
    { /*использовать свою настройку на клавиатуру */
      ch=getkey_();
    }else{ /* использовать стандартные средства curses */
      ch=getch();
      if(ch==ERR && alarmed) ch=ALARMED;
    }

  unset_alarms();

  if(wasbs &&
     ch!=K_BACKSPACE && ch!=BACKSP &&
     ch!=K_SL        && ch!=KEY_LEFT
    ) wasbs=0; /* теперь по BACKSPACE и стрелке влево в начале строки - звонок*/

  switch(ch)
    {
     case ALARMED: /* во время работы getkey_() пришел сигнал*/
       /* обработка результатов - в конце цикла */
       /* здеь не обрабатывается т.к. сигнал м.б. во время разборки */
     break;

     case K_SU:
     case KEY_UP:
       if(panels[cpanel].on>0)
	 { /* панель включена, можно перемещаться по файлам*/
	   if(panels[cpanel].curfile){
	     panels[cpanel].curfile--;
	     if(panels[cpanel].curfile<panels[cpanel].firstfile)
	       {
		 panels[cpanel].firstfile--;
	       }else{ /* изменяется только место указателя */
		 pred_file=panels[cpanel].curfile+1;
	       }
	     outpanel=cpanel;
	     refr_panel();
	     wrefresh(panels[cpanel].win);
	   }
	 }else{ /* панель выключена, перемещение по истории */
	   his_go=hispred(his_go);

	   if(his_go==his_cur)
	     { /* дошли по кольцу до текущей команды*/
	       inp_str[0]='\0';
	     }else{ /* найдена предыдущая/следующая команда */
	       strcpy(inp_str, &history[his_go]);
	     }
	   inp_cur=strlen(inp_str);
	   for(inp_first=inp_cur;inp_first<inp_len;inp_first++) inp_str[inp_first]=' ';
	   inp_first=0;  /* в refr_inp() это значение может поменяться*/
	   inp_str[inp_len-1]='\0';
	   refr_inp();
	 }
     break;

     case K_SD:
     case KEY_DOWN:
       if(panels[cpanel].on>0)
	 { /* панель включена, можно перемещаться по файлам*/
	   if(panels[cpanel].nfiles-1>panels[cpanel].curfile){
	     panels[cpanel].curfile++;
	     if(panels[cpanel].curfile>panels[cpanel].lastfile)
	       {
		 panels[cpanel].firstfile++;
	       }else{ /* изменяется только место указателя */
		 pred_file=panels[cpanel].curfile-1;
	       }
	     outpanel=cpanel;
	     refr_panel();
	     wrefresh(panels[cpanel].win);
	   }
	 }else{ /* панель выключена, перемещение по истории */
	   his_go=hisnext(his_go);

	   if(his_go==his_cur)
	     { /* дошли по кольцу до текущей команды*/
	       inp_str[0]='\0';
	     }else{ /* найдена предыдущая/следующая команда */
	       strcpy(inp_str, &history[his_go]);
	     }
	   inp_cur=strlen(inp_str);
	   for(inp_first=inp_cur;inp_first<inp_len;inp_first++) inp_str[inp_first]=' ';
	   inp_first=0;  /* в refr_inp() это значение может поменяться*/
	   inp_str[inp_len-1]='\0';
	   refr_inp();
	 }
     break;

     case K_PAGE_UP:
     case KEY_PPAGE:
       if(panels[cpanel].on>0){
	if(panels[cpanel].curfile){

	  i = files_on_page(cpanel);

	  panels[cpanel].curfile -= i;
	  panels[cpanel].firstfile -= i;
	  if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;
	  if(panels[cpanel].curfile <= 0) panels[cpanel].curfile=1;
	  outpanel=cpanel;
	  refr_panel();
	  wrefresh(panels[cpanel].win);
	}
       }
     break;

     case K_PAGE_DN:
     case KEY_NPAGE:
       if(panels[cpanel].on>0){
	if(panels[cpanel].nfiles-1>panels[cpanel].curfile){

	  i = files_on_page(cpanel);

	  panels[cpanel].curfile += i;
	  if(panels[cpanel].curfile>panels[cpanel].nfiles-1) /* проскочили */
		panels[cpanel].curfile=panels[cpanel].nfiles-1;

	  panels[cpanel].firstfile += i;
	  if(panels[cpanel].firstfile + i > panels[cpanel].nfiles-1)
	    panels[cpanel].firstfile = panels[cpanel].nfiles - i;

	  if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;

	  outpanel=cpanel;
	  refr_panel();
	  wrefresh(panels[cpanel].win);
	}
       }
     break;

     case K_HOME:
     case KEY_HOME:
       if(panels[cpanel].on>0)
	 { /* панель включена, можно перемещаться по файлам*/
	   if(panels[cpanel].curfile){
	     panels[cpanel].curfile=1;
	     panels[cpanel].firstfile=0;
	     outpanel=cpanel;
	     refr_panel();
	     wrefresh(panels[cpanel].win);
	   }
	 }else{ /* панель выключена, перемещение по команде */
	   inp_cur=0;
	   refr_inp();
	 }
     break;

     case K_END:
     case KEY_END:
       if(panels[cpanel].on>0)
	 { /* панель включена, можно перемещаться по файлам*/
	   panels[cpanel].firstfile = panels[cpanel].nfiles-files_on_page(cpanel);

	   if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;

	   panels[cpanel].curfile=panels[cpanel].nfiles-1;

	   outpanel=cpanel;
	   refr_panel();
	   wrefresh(panels[cpanel].win);
	 }else{ /* панель выключена, перемещение по команде */
	   inp_cur=inp_len-2;
	   while(inp_cur && inp_str[inp_cur]==' ') inp_cur--;
	   if(inp_cur && inp_cur<inp_len-2) inp_cur++;
	   refr_inp();
	 }
     break;

     case K_TAB:
     case TAB:
       if( (panels[cpanel].on==1 || panels[cpanel].on==3) &&
	   (panels[1-cpanel].on==1 || panels[1-cpanel].on==3)){
	outpanel = cpanel;
	cpanel = 1 - cpanel;
	refr_panel(); /* при выводе сравнит cpanel и outpanel*/
	wrefresh(panels[outpanel].win);

	outpanel=cpanel;
	refr_panel();
	wrefresh(panels[cpanel].win);
       }
     break;

     case K_LF: /* ^J */
       if(panels[cpanel].on>0){
	 ptr=panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname;
	 if(strcmp(ptr, ".")==0){ /* имя файла - текущий каталог, поместить полное его имя*/
	   ptr=panels[cpanel].full_dir;
	 }
	 while(inp_cur<inp_len-1 && *ptr){
	     /* этот кусок нужно переделать более эффективно */
	     for(i=inp_len-2; i>inp_cur; i--) inp_str[i]=inp_str[i-1];
	     inp_str[inp_cur]=*ptr++;
	     inp_cur++;
	 }
	 if(inp_cur<inp_len-1){ /* добавить пробел */
	     for(i=inp_len-2; i>inp_cur; i--) inp_str[i]=inp_str[i-1];
	     inp_str[inp_cur]=' ';
	     inp_cur++;
	 }
	 refr_inp();
       }
     break;

     case K_CR:
     case ENTER:
	if(inp_str[0]!=' ')
	  { /* введена команда */
	    if(chdir(panels[cpanel].full_dir) &&
		    !( inp_str[0]=='c' && inp_str[1]=='d' &&
		      (inp_str[2]==' ' || inp_str[2]=='\0')
		     ))
	      { /* не получилось перейти в текущий каталог панели */
		beep();
	      }else{
		for(inp_cur=inp_len-2; inp_str[inp_cur]==' ' && inp_cur; inp_cur--);
		inp_cur++;
		inp_str[inp_cur]='\0';

		stop=execcmd(inp_str);

		for(inp_cur=0;inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';
		inp_str[inp_len-1]='\0';
		inp_cur=inp_first=0;
		refr_inp();
		/* wrefresh(inp_win); выполняется на каждом шаге */
	      }
	  }else{ /* работа в панелях */
	    if(panels[cpanel].on>0){
	      if(chdir(panels[cpanel].full_dir)) beep();
	      item= &panels[cpanel].itembuf[panels[cpanel].curfile];

	      i = 1;  /* теперь это признак неперехода в каталог*/

	      stbuf.st_mode=0;
	      stat(panels[cpanel].names + item->nname, &stbuf);
	      if((stbuf.st_mode & S_IFMT) == S_IFDIR)
		{ /* каталог */

		 if(access(panels[cpanel].names + item->nname, X_OK|R_OK)==0) /* каталог, доступный для перехода и чтения*/
		   i = 0;

		 /* проверка читаемости каталога нужна для того, чтобы не ловить root в нечитаемые (NFS) каталоги */
		 if(i==0){
		   if(dirp = opendir(panels[cpanel].names + item->nname))
		     {
		       if ( (dp=readdir(dirp))==NULL ) i++;
		       if ( i || (dp=readdir(dirp))==NULL ) i++;
		       closedir(dirp);
		       if(i) { beep(); beep(); }
		     }else{
		       i++;
		     }
		 }
		 if ( i || chdir( panels[cpanel].names + item->nname )!=0) i++;

		 if ( i ) beep();

		}else{ /* файл */

		  if(access(panels[cpanel].names + item->nname, X_OK)==0)
		    {
		      stop=execcmd(panels[cpanel].names + item->nname);
		      refr_inp();
		      /* wrefresh(inp_win); выполняется на каждом шаге */
		    }else{
		      if(ext_enter(panels[cpanel].names+item->nname) != 0) beep();  /* нет файла обработки расширений или нет строки по данному расширению*/

		    }
		}
		if(i==0){ /* перешли в каталог */

		  outpanel=cpanel;

		  if(strcmp(panels[cpanel].names + item->nname, "..")==0 &&
		     strcmp(panels[cpanel].full_dir, "/"))
		    { /* переход на каталог выше*/

		      for(ptr=panels[cpanel].full_dir; *ptr; ptr++);
		      while(*ptr!='/' && ptr!=panels[cpanel].full_dir) ptr--;
		      if(*ptr=='/') ptr++;
		      if(strlen(ptr)<tmp_len)
			{
			  strcpy(tmp_str, ptr);
			}else{
			  tmp_str[0]='\0';
			}

		      read_to_panel();  /* extern outpanel */

		      if(tmp_str[0]){
			/* поиск каталога с запомненными именем */
			for(nfile=0,item= panels[cpanel].itembuf;
			   nfile<panels[cpanel].nfiles &&
			   strcmp(tmp_str,panels[cpanel].names+item->nname)
			   ;nfile++,item++);

			if(nfile<panels[cpanel].nfiles){ /* найден*/
			  panels[cpanel].curfile=nfile;
			  if(nfile>=(panels[cpanel].lines-panels[cpanel].lnused))
			    { /* файл - не на первой странице*/
			      panels[cpanel].firstfile=nfile-(panels[cpanel].lines-panels[cpanel].lnused)/2;
			      if(panels[cpanel].firstfile<0) panels[cpanel].firstfile=0;
			    }else{ /* на первой странице*/
			      panels[cpanel].firstfile=0;
			    }
			  refr_panel();
			}
		      }
		    }else{
		      read_to_panel();  /* extern outpanel */
		    }
		  wrefresh(panels[cpanel].win);
		  if(panels[1-cpanel].on == 2) wrefresh(panels[1-cpanel].win);
		} /* отработка перехода в каталог */
	    } /* if panel is on */
	  }
     break;

     case K_INSRT: /* Insert */
     case K_INSERT:/* Insert */
     case INSERT:  /* Insert */
     case KEY_IC:  /* Insert */
       if(panels[cpanel].on>0){
	item= &panels[cpanel].itembuf[panels[cpanel].curfile];
	if( strcmp(panels[cpanel].names+item->nname, ".") &&
	    strcmp(panels[cpanel].names+item->nname, "..") ){
	      item->attrs ^= MARKED;
	      if(item->attrs & MARKED)
		{ /*отметка нового файла*/
		  panels[cpanel].selected++;
		  panels[cpanel].size_sel+=item->size;
		}else{
		  panels[cpanel].selected--;
		  panels[cpanel].size_sel-=item->size;
		}
	}
	if(panels[cpanel].nfiles-1>panels[cpanel].curfile){
	  panels[cpanel].curfile++;
	  if(panels[cpanel].curfile>panels[cpanel].lastfile){
	    panels[cpanel].firstfile++;
	  }
	}
	outpanel=cpanel;
	refr_panel();
	wrefresh(panels[cpanel].win);
       }
     break;

     case K_SL:     /* Стрелка влево */
     case KEY_LEFT: /* Стрелка влево */
	if(panels[cpanel].on==3)
	  { /* Brief */
	    i = panels[cpanel].lines-panels[cpanel].lnused;

	    panels[cpanel].curfile -= i;
	    if(panels[cpanel].curfile <= panels[cpanel].firstfile) panels[cpanel].firstfile -= i;
	    if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;
	    if(panels[cpanel].curfile <= 0) panels[cpanel].curfile=1;
	    outpanel=cpanel;
	    refr_panel();
	    wrefresh(panels[cpanel].win);

	  }else{
	    if(inp_cur)
	      {
		inp_cur--;
		refr_inp();
	      }else{
		if(!wasbs){
		  beep();
		  wasbs++;
		}
	      }
	  }
     break;

     case K_SR: /* Стрелка вправо */
     case KEY_RIGHT: /* Стрелка вправо */
	if(panels[cpanel].on==3)
	  { /* Brief */
	    i = panels[cpanel].lines-panels[cpanel].lnused;

	    panels[cpanel].curfile += i;
	    if(panels[cpanel].curfile>panels[cpanel].nfiles-1) /* проскочили */
		  panels[cpanel].curfile=panels[cpanel].nfiles-1;

	    if(panels[cpanel].curfile >= panels[cpanel].lastfile) panels[cpanel].firstfile += i;
	    if(panels[cpanel].firstfile + files_on_page(cpanel) > panels[cpanel].nfiles-1)
	      panels[cpanel].firstfile = panels[cpanel].nfiles - files_on_page(cpanel);

	    if(panels[cpanel].firstfile < 0) panels[cpanel].firstfile=0;

	    outpanel=cpanel;
	    refr_panel();
	    wrefresh(panels[cpanel].win);

	  }else{ /* работа в командной строке */

	   if(inp_cur<inp_len-1)
	     {
	       inp_cur++;
	       refr_inp();
	     }else{
	       beep();
	     }
	  }
     break;

     case PRED:   /* прерыдущая команда (по истории)*/
     case K_PRED: /* прерыдущая команда (по истории)*/
     case K_NEXT: /* следующая команда (по истории)*/
     case NEXT:   /* следующая команда (по истории)*/
       switch(ch)
	 {
	   case PRED: /* прерыдущая команда (по истории)*/
	   case K_PRED: /* прерыдущая команда (по истории)*/
	     his_go=hispred(his_go);
	   break;
	   case K_NEXT: /* следующая команда (по истории)*/
	   case NEXT: /* следующая команда (по истории)*/
	     his_go=hisnext(his_go);
	   break;
	 }

       if(his_go==his_cur)
	 { /* дошли по кольцу до текущей команды*/
	   inp_str[0]='\0';
	 }else{ /* найдена предыдущая/следующая команда */
	   strcpy(inp_str, &history[his_go]);
	 }
       inp_cur=strlen(inp_str);
       for(inp_first=inp_cur;inp_first<inp_len;inp_first++) inp_str[inp_first]=' ';
       inp_first=0;  /* в refr_inp() это значение может поменяться*/
       inp_str[inp_len-1]='\0';
       refr_inp();
     break;

     case K_ONOFF: /* клавиша вкл/выкл панелей CTRL-O */
     case ONOFF: /* клавиша вкл/выкл панелей CTRL-O */
       on_off_panels();
     break;

     case K_SWAPP: /* клавиша смены местами панелей CTRL-U */
     case SWAPP: /* клавиша смены местами панелей CTRL-U */
       if ( dwidth )
	 { /* При двойной ширине отрабатывает как TAB */
	   /* кусок скопирован из отработки табуляции */
	   if( (panels[cpanel].on==1 || panels[cpanel].on==3) &&
	       (panels[1-cpanel].on==1 || panels[1-cpanel].on==3)){
	    outpanel = cpanel;
	    cpanel = 1 - cpanel;
	    refr_panel(); /* при выводе сравнит cpanel и outpanel*/
	    wrefresh(panels[outpanel].win);

	    outpanel=cpanel;
	    refr_panel();
	    wrefresh(panels[cpanel].win);
	   }
	 }else{
	   swappanels();   /* Эти функции находятся в файле commands.c */
	 }
     break;

     case K_CDROOT: /* клавиша перехода в корневой каталог*/
     case CDROOT:   /* клавиша перехода в корневой каталог*/
       if(panels[cpanel].on>0){
	 chdir("/");
	 outpanel=cpanel;
	 read_to_panel();
	 wrefresh(panels[cpanel].win);
	 if(panels[1-cpanel].on == 2) wrefresh(panels[1-cpanel].win);
       }
     break;

     case K_INFO:  /* вывода окна Info*/
     case INFO:
       pred_file = -1;      /* номер передыдущего файла для refr_panel()*/

       if(panels[cpanel].on>0 && !dwidth){
	 if(panels[1-cpanel].on==2)
	   { /* вывести неактивную панель с файлами */
	     outpanel=1-cpanel;
	     panels[outpanel].on = panels[outpanel].save_on;
	   }else{ /* вывести неактивную панель с Info */
	     /* для этого нужно вызвать вывод активной */
	     outpanel=cpanel;
	     panels[1-cpanel].save_on = panels[1-cpanel].on;
	     panels[1-cpanel].on=2;
	   }
	 refr_panel();
	 wrefresh(panels[cpanel].win);
	 wrefresh(panels[1-cpanel].win);
       }
     break;

     case K_REFRESH: /* клавиша перевывода экрана */
     case REFRESH:   /* клавиша перевывода экрана */
       touchwin(stdscr);
       touchwin(dn_menu);
       touchwin(inp_win);
       clearok(stdscr, TRUE);
       refresh();
       wrefresh(dn_menu);
       /* wrefresh(inp_win); - в конце цикла */
     break;

     case K_REREAD:  /* клавиша пересчитывания каталога */
     case REREAD:    /* клавиша пересчитывания каталога */
       if(panels[cpanel].on>0){
	 outpanel=cpanel;
	 if(chdir(panels[outpanel].full_dir)) beep();
	 nfile=panels[outpanel].curfile;
	 nfirst=panels[outpanel].firstfile;
	 read_to_panel();
	 if(nfile<panels[outpanel].nfiles){ /* найден*/
	   panels[outpanel].curfile=nfile;
	   panels[outpanel].firstfile=nfirst;
	   refr_panel();
	 }
	 wrefresh(panels[outpanel].win);
	 if(panels[1-cpanel].on == 2) wrefresh(panels[1-cpanel].win);
       }
     break;

     case K_FIND:   /* клавиша поиска в текущем каталоге */
     case FIND:
       if(panels[cpanel].on>0){
	 findcdir();
       }
     break;

     case K_GOOTH:   /* клавиша перехода неактивной панели в каталог активной */
     case GOOTH:
       if(panels[cpanel].on>0){
	 if(chdir(panels[cpanel].full_dir)) beep();
	 outpanel=(cpanel) ? 0 : 1;
	 read_to_panel();
	 wrefresh(panels[outpanel].win);
       }
     break;

     case K_PROC:    /* клавиша перехода в режим pRocess control */
     case PROC:
       viewkill();
     break;

     case K_LONGNM:  /* клавиша вывода длинных имен */
     case LONGNM:
       panels[cpanel].outfields[0]=0;
       panels[cpanel].outfields[1]=1;
       panels[cpanel].outfields[2]=2;
       panels[cpanel].outfields[3]=3;
       for(i=4; i<MAXFIELDS; i++) panels[cpanel].outfields[i]=(-1);

       outpanel=cpanel;
       refr_panel();  /* extern outpanel */
       wrefresh(panels[cpanel].win);
     break;

     case K_DFLT: /* клавиша вывода колонок панели по умолчанию */
     case DFLT:
       for(i=0; i<MAXFIELDS; i++) panels[cpanel].outfields[i]=i;
       outpanel=cpanel;
       refr_panel();  /* extern outpanel */
       wrefresh(panels[cpanel].win);
     break;

     case K_DWIDTH: /* клавиша вывода панелей во весь экран */
     case DWIDTH:
       if(panels[0].on>0 || panels[1].on>0 ) doublewidth();
     break;

     case K_F1:         /* выдача справки, если она есть */
     case KEY_F(1):
       if(helpfile)
	 {
	    viewfile(helpfile);
	  }else{
	    beep();
	  }
     break;

     case K_F3:
     case KEY_F(3):
       if(panels[cpanel].on>0){
	item= &panels[cpanel].itembuf[panels[cpanel].curfile];
	if(chdir(panels[cpanel].full_dir) || filtype(item->mode)==FTYPE_DIR)
	  {
	    beep();
	  }else{
	    if(ext_view(panels[cpanel].names+item->nname) != 0){
	      /* нет файла обработки расширений или нет строки по данному расширению*/
	      viewfile(panels[cpanel].names+item->nname);
	    }
	  }
       }
     break;

     case K_F4:
     case KEY_F(4):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir) ||
	   filtype(panels[cpanel].itembuf[panels[cpanel].curfile].mode)==FTYPE_DIR)
	  {
	    beep();
	  }else{
	    editfile(panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname);
	  }
       }
     break;

     case K_F5:
     case KEY_F(5):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* какие-то проблемы с переходом в текущий каталог */
	    beep();
	  }else{
	    copyfile();     /* копирование [отмеченных] файлов*/
	  }
       }
     break;

     case K_F6:
     case KEY_F(6):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* какие-то проблемы с переходом в текущий каталог */
	    beep();
	  }else{
	    renmove();     /* перемещение [отмеченных] файлов*/
	  }
       }
     break;

     case K_F7:
     case KEY_F(7):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* какие-то проблемы с переходом в текущий каталог */
	    beep();
	  }else{
	    makedir();
	  }
       }
     break;

     case K_F8:
     case KEY_F(8):
       if(panels[cpanel].on>0){
	if(chdir(panels[cpanel].full_dir))
	  { /* какие-то проблемы с переходом в текущий каталог */
	    beep();
	  }else{
	    deletefile();     /* удаление [отмеченных] файлов*/
	  }
       }
     break;

     case K_F9:
     case KEY_F(9):
       popdn();
     break;

     case K_F0:
     case KEY_F(10):
     case QUIT:
     case K_QUIT:
       if(ask(&win1,0,HOR)==0) stop++;
     break;

     case K_ESC:
     case ESC:
       for(inp_cur=0;inp_cur<inp_len;inp_cur++) inp_str[inp_cur]=' ';
       inp_str[inp_len-1]='\0';
       inp_cur=inp_first=0;
       refr_inp();
     break;

     case K_BACKSPACE:
     case BACKSP: /* клавиша BACKSPACE */
	if(inp_cur)
	  {
	    for(i=inp_cur; i<inp_len-1; i++) inp_str[i-1]=inp_str[i];
	    inp_str[inp_len-2]=' ';
	    inp_str[inp_len-1]='\0';
	    inp_cur--;
	    refr_inp();
	  }else{
	    if(!wasbs){
	      beep();
	      wasbs++;
	    }
	  }

     break;

     case K_DELETE: /* клавиша DELETE */
     case DELETE: /* клавиша DELETE */
	for(i=inp_cur; i<inp_len-2; i++) inp_str[i]=inp_str[i+1];
	inp_str[inp_len-2]=' ';
	inp_str[inp_len-1]='\0';
	refr_inp();
     break;

     default:
	if(use_keycap && (ch&SIMBTYPE)==PECH_SIMB) ch&=SIMBOL;
	if(my_isprint(ch))
	  { /* в строку ввода команды */
	    if((ch=='+'||ch=='-'||ch=='*') && inp_cur==0)
	      { /* спец. символ отметки */
		switch(ch)
		  {
		   case '+':
		     selectgroup();
		   break;
		   case '-':
		     deselectgroup();
		   break;
		   case '*':
		     invertselect();
		   break;
		  }
	      }else{
		if(inp_cur<inp_len-1)
		  {
		    /* if(ch=='\\') ch='/'; */
		    for(i=inp_len-2; i>inp_cur; i--) inp_str[i]=inp_str[i-1];
		    inp_str[inp_cur]=ch;
		    inp_cur++;
		    refr_inp();
		  }else{
		    beep();
		  }
	      }
	  }else{
/*          fprintf(stderr, "\r %o %d  ", ch,ch); */
	    beep();
	  }
     break;
    }

  if(alarmed){ /* был сигнал по alarm() */
    alarmed=0;
    do_alarms();
  }

  wrefresh(inp_win); /* курсор должен торчать в месте ввода команды! */
}
clear();
refresh();
nocbreak();
echo();
endwin();
if(use_keycap){
  setkey_(0); /*может понадобиться последовательность выключения цифровой кл.*/
}
if(last_search) unlink(last_search);
exit(0);
}
