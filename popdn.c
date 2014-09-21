#include <stdlib.h>
#include "defs.h"
/****************************************************************************/
				popdn()
/*                      Работа с выпадающими меню
/****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  win     win24;  /* окно верхнего меню */
extern  struct  win     win25;  /* окно настройки левой панели */
extern  struct  win     win26;  /* окно Files */
extern  struct  win     win27;  /* окно Commands */
extern  struct  win     win28;  /* окно Options */
extern  struct  win     win29;  /* окно настройки правой панели */
extern  struct  win     win36;  /* сообщение о потере отметки при Unsorted */
extern  struct  win     win62;  /* Files are identical */
extern  struct  win     win63;  /* Files differs */
extern  struct  win     win64;  /* Error while comparing */
extern  struct  win     win65;  /* Error: Non-regular */
extern  struct  win     win78;  /* About conix... */
extern  chtype  atr[];          /* раскраска */
extern  WINDOW  *dn_menu;       /* нижнее меню */
extern  WINDOW  *inp_win;       /* окно ввода команд */
extern  int     was_on;         /* в битах: 1<<номер_вкл_пан*/
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  int     nosave_ask;     /* признак "несохранения" экрана для ask */
extern  unsigned inmenu;        /*для имитации нажатия клавиши */
extern  int     silent;         /* признак сравнения без сообщения об ошибках*/
extern  int     prompt;         /* тип приглашения:0-нет, 1-NodeName 2-User@Node*/
extern  int     clock_need;     /* признак вывода времени 0-нет 1-да */
extern  int     dwidth;         /* признак панелей двойной ширины */
extern  char    *helpfile;      /* файл со справкой */
	int     stop=0;
	int     curr_1;         /* номер пункта меню на уровне 1 */
	int     numsort;        /* для вычисления типа сортировки */
	int     read_panels=0;  /* признак необходимости перечитать панели (их номера - в битах)*/
	int     out_oth=0;      /* признак: вывести остальные окна*/
	int     nfile;          /* номер текущего файла*/
	int     nfirst;         /* номер первого выводимого файла*/
static  struct win *wins[]={&win25, &win26, &win27, &win28, &win29};
extern  int     cmpitems();     /*функция сравнения х-к файлов для сортировки*/
/*==========================================================================*/
overwrite(curscr, save_scr);
overwrite(curscr, stdscr);
#ifdef FREEBSD
  touchwin(stdscr);
#endif
clearok(stdscr,FALSE);

silent=0;
nosave_ask=1;   /* признак "несохранения" экрана для ask */

if(cpanel)
  {
   curr_1=4;
  }else{
   curr_1=0;
  }

win24.cols=COLS-2;

while(!stop){
      if(inmenu)
	{
	  inmenu=0;
	}else{
	  curr_1=ask(&win24,curr_1,HOR);
	}
      switch(curr_1)
	{
	 case 0: /*-------------------------- Left -------------------------*/
	 case 4: /*-------------------------- Right ------------------------*/
	   if(curr_1)
	     {
	      outpanel=1;
	     }else{
	      outpanel=0;
	     }

	   wins[curr_1]->txts[0].txt[0]=
	   wins[curr_1]->txts[1].txt[0]=
	   wins[curr_1]->txts[2].txt[0]= ' ';
	   switch(panels[outpanel].on)
	     {
		case 1: /* Full */
		case -1: /* Full/Off */
		  wins[curr_1]->txts[1].txt[0]='+';
		break;

		case 2: /* Info */
		case -2: /* Info/Off */
		  wins[curr_1]->txts[2].txt[0]='+';
		break;

		case 3: /* Brief */
		case -3: /* Brief/Off */
		  wins[curr_1]->txts[0].txt[0]='+';
		break;

		default: /* вообще-то это ошибка или старый .conix.ini с выключенной панелью */
		  beep();
		break;
	     }

	   if(panels[outpanel].on>0)
	     { /* Панель включена - выделить Off */
	       wins[curr_1]->txts[3].txt[2]='o';
	       wins[curr_1]->txts[3].txt[5]='O';
	     }else{ /* Панель выключена - выделить On */
	       wins[curr_1]->txts[3].txt[2]='O';
	       wins[curr_1]->txts[3].txt[5]='o';
	     }

	   wins[curr_1]->txts[4].txt[0]=
	   wins[curr_1]->txts[5].txt[0]=
	   wins[curr_1]->txts[6].txt[0]=
	   wins[curr_1]->txts[7].txt[0]=
	   wins[curr_1]->txts[8].txt[0]=' ';
	   wins[curr_1]->txts[11].txt[0]=' '; /* фильтр */
	   switch(panels[outpanel].sort)
	     {
	      case 0: /* Unsorted */
		wins[curr_1]->txts[8].txt[0]='+';
	      break;
	      case 1: /* Name */
		wins[curr_1]->txts[4].txt[0]='+';
	      break;
	      case 2: /* Extension */
		wins[curr_1]->txts[5].txt[0]='+';
	      break;
	      case 3: /* Time */
		wins[curr_1]->txts[6].txt[0]='+';
	      break;
	      case 4: /* Size */
		wins[curr_1]->txts[7].txt[0]='+';
	      break;
	     }

	   if(panels[outpanel].filter_read[0] ||
	      panels[outpanel].filter_noread[0] )
		 wins[curr_1]->txts[11].txt[0]='+'; /* фильтр */

	   numsort=0;
	   switch(ask(wins[curr_1],0,VERT|OUT))
	     {

	      case 0: /* Brief */
		cpanel=outpanel;
		if(panels[outpanel].on == 3 || panels[outpanel].on == -3)
		  { /* с положением файла на панели ничего делать не надо*/
		    panels[outpanel].on = 3;
		  }else{ /* м.б. было Full -> посчитать firstfile */
		    panels[outpanel].on = 3;
		    if(panels[outpanel].curfile<files_on_page(outpanel)){
		      panels[outpanel].firstfile=0;
		    }
		    if(panels[outpanel].firstfile + files_on_page(outpanel) >
		       panels[outpanel].nfiles){/* попробовать показать больше файлов*/
		      panels[outpanel].firstfile = panels[outpanel].nfiles -
						   files_on_page(outpanel);
		      if(panels[outpanel].firstfile<0) panels[outpanel].firstfile=0;
		    }
		  }

		stop++;
	      break;

	      case 1: /* Full */
		cpanel=outpanel;
		if(panels[outpanel].on == 1 || panels[outpanel].on == -1)
		  { /* с положением файла на панели ничего делать не надо*/
		    panels[outpanel].on = 1;
		  }else{ /* м.б. было Brief -> посчитать firstfile */
		    panels[outpanel].on = 1;
		    panels[outpanel].firstfile = panels[outpanel].curfile;
		    if(panels[outpanel].firstfile + files_on_page(outpanel) >
		       panels[outpanel].nfiles){/* попробовать показать больше файлов*/
		      panels[outpanel].firstfile = panels[outpanel].nfiles -
						   files_on_page(outpanel);
		      if(panels[outpanel].firstfile<0) panels[outpanel].firstfile=0;
		    }
		    if(panels[outpanel].curfile<files_on_page(outpanel)){
		      panels[outpanel].firstfile=0;
		    }
		  }
		stop++;
	      break;

	      case 2: /* Info */
		if((panels[1-outpanel].on==1 || panels[1-outpanel].on==3) &&
		   dwidth == 0)
		  {
		    cpanel=1-outpanel;
		    panels[outpanel].save_on = panels[outpanel].on;
		    panels[outpanel].on = 2;
		    outpanel=cpanel;
		  }else{
		    beep();
		  }
		stop++;
	      break;

	      case 3: /* On/Off */
		was_on=0;
		cpanel=outpanel;
		panels[outpanel].on *= -1;
		if(panels[cpanel].on<=0) {
		  for(cpanel=0; cpanel<NPANELS && panels[cpanel].on<=0; cpanel++);
		  if(cpanel==NPANELS) cpanel=0;
		}
		wattrset(save_scr, A_NORMAL);
		werase(save_scr);
		out_oth=1;
		/* вывод обновления будет выполнен позже */
		stop++;
	      break;

	      /* Сортировки - для последующего изменения номера пункта - numsort */
	      case 7: /* Size */
		numsort++;
	      case 6: /* Time */
		numsort++;
	      case 5: /* Extension */
		numsort++;
	      case 4: /* Name */
		numsort++;
	      case 8: /* Unsorted */
		cpanel=outpanel;
		if(numsort==0)
		  { /* неотсортировано */
		    if(panels[cpanel].selected)
		      { /* есть отмеченные файлы */
			if(ask(&win36, 2, HOR|VERT)==2){
			   /* п-ль согласен потерять отм.*/
			   panels[cpanel].sort=0;
			   if(panels[cpanel].on<0) panels[cpanel].on *= -1;
			   read_panels|=1<<cpanel;
			}
		      }else{ /* отмеченных файлов нет */
			   panels[cpanel].sort=0;
			   if(panels[cpanel].on<0) panels[cpanel].on *= -1;
			   read_panels|=1<<cpanel;
		      }
		  }else{ /*при сортировках можно оставить отметку */
		    panels[outpanel].sort=numsort;
		    if(panels[outpanel].on<0) panels[outpanel].on *= -1;
		    qsort(panels[outpanel].itembuf+2,
		    (size_t)panels[outpanel].nfiles-2,
		    sizeof(struct itembuf),
		    cmpitems);

		  }
		stop++;
	      break;

	      case 9: /* Re-read */
		cpanel=outpanel;
		if(panels[cpanel].on<0) panels[cpanel].on *= -1;
		read_panels=1<<cpanel;
		stop++;
	      break;

	      case 10: /* Columns */
		cpanel=outpanel;
		if(panels[cpanel].on<0) panels[cpanel].on *= -1;
		columns();
		stop++;
	      break;

	      case 11: /* Filters */
		cpanel=outpanel;
		if(panels[cpanel].on<0) panels[cpanel].on *= -1;
		filters();
		stop++;
	      break;

	      default:
		if(inmenu)
		  { /* вышли из меню по стрелке влево/вправо */
		    /* обработка - в конце цикла */
		  }else{
		    stop++;
		  }
	      break;
	     }
	 break; /* Left, Right */

	 case 1: /*------------------------- Files -------------------------*/
	   switch(ask(wins[curr_1],0,VERT|OUT))
	     {
	      case 0: /* Help */
		if(helpfile)
		  {
		    viewfile(helpfile);
		  }else{
		    beep();
		  }
		stop++;
	      break;

	      case 2: /* View */
		if(panels[cpanel].on>0){
		  if(chdir(panels[cpanel].full_dir) ||
		     filtype(panels[cpanel].itembuf[panels[cpanel].curfile].mode)==FTYPE_DIR)
		    {
		      beep();
		    }else{
		      viewfile(panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname);
		    }
		  stop++;
		}
	      break;

	      case 3: /* Edit */
		if(panels[cpanel].on>0){
		  if(chdir(panels[cpanel].full_dir) ||
		     filtype(panels[cpanel].itembuf[panels[cpanel].curfile].mode)==FTYPE_DIR)
		    {
		      beep();
		    }else{
		      editfile(panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname);
		      stop++;
		    }
		}
	      break;

	      case 4: /* Copy */
		if(panels[cpanel].on>0){
		  if(chdir(panels[cpanel].full_dir))
		    {
		      beep();
		    }else{
		      copyfile();     /* копирование [отмеченных] файлов*/
		      out_oth=1;
		      stop++;
		    }
		}
	      break;

	      case 5: /* RenMove */
		if(panels[cpanel].on>0){
		  if(chdir(panels[cpanel].full_dir))
		    {
		      beep();
		    }else{
		      renmove();     /* переименование [отмеченных] файлов*/
		      out_oth=1;
		      stop++;
		    }
		}
	      break;

	      case 6: /* Make directory */
		if(panels[cpanel].on>0){
		  if(chdir(panels[cpanel].full_dir))
		    {
		      beep();
		    }else{
		      makedir();
		      out_oth=1;
		      stop++;
		    }
		}
	      break;

	      case 7: /* Delete */
		if(panels[cpanel].on>0){
		  if(chdir(panels[cpanel].full_dir))
		    {
		      beep();
		    }else{
		      deletefile();     /* удаление [отмеченных] файлов*/
		      out_oth=1;
		      stop++;
		    }
		}
	      break;

	      case 8: /* File Attributes */
		if(panels[cpanel].on>0){
		  if(chdir(panels[cpanel].full_dir))
		    {
		      beep();
		    }else{
		      fileattrs();     /* задание режимов доступа [отмеченных] файлов*/
		      stop++;
		    }
		}
	      break;

	      case 9: /* select Group */
		if(panels[cpanel].on>0){
		  selectgroup();
		  stop++;
		}
	      break;

	      case 10: /* deselect Group */
		if(panels[cpanel].on>0){
		  deselectgroup();
		  stop++;
		}
	      break;

	      case 11: /* invert selection */
		if(panels[cpanel].on>0){
		  invertselect();
		  stop++;
		}
	      break;

	      case 12: /* About conix */
		ask(&win78,0,0);
		stop++;
	      break;

	      default:
		if(inmenu)
		  { /* вышли из меню по стрелке влево/вправо */
		    /* обработка - в конце цикла */
		  }else{
		    stop++;
		  }
	      break;
	     }
	 break;

	 case 2: /*------------------------ Commands -----------------------*/
	   if(panels[0].on>0 && panels[1].on>0)
	     { /* сравнивать можно только при вкл.панелях */
	       wins[curr_1]->txts[7].active=1;
	     }else{
	       wins[curr_1]->txts[7].active=0;
	     }
	   switch(ask(wins[curr_1],0,VERT|OUT))
	     {
	      case 0: /* Find file */
		findfile();
		stop++;
	      break;

	      case 1: /* Find in curr.dir */
		findcdir();
		stop++;
	      break;

	      case 2: /* History */
		histlist();   /* Эти функции находятся в файле commands.c */
		out_oth=1;
		stop++;
	      break;

	      case 4: /* Swap panels */
		swappanels();   /* Эти функции находятся в файле commands.c */
		overwrite(stdscr, save_scr);
		out_oth=1;
		stop++;
	      break;

	      case 5: /* Panels on/off*/
		on_off_panels();/* Эти функции находятся в файле commands.c */
		overwrite(stdscr, save_scr);
		out_oth=1;
		stop++;
	      break;

	      case 6: /* Double width */
		if(panels[0].on>0 || panels[1].on>0 )
		  {
		    doublewidth(); /* Эти функции находятся в файле commands.c */
		    overwrite(stdscr, save_scr);
		    out_oth=1;
		  }else{
		    beep();
		  }
		stop++;
	      break;

	      case 7: /* Compare directories */
		compare_dirs();/* Эти функции находятся в файле commands.c */
		stop++;
	      break;

	      case 8: /* compare Two files */
		if(panels[0].on>0 && panels[1].on>0){ /* сравнивать можно только при вкл.панелях */
		  if(filtype(panels[0].itembuf[panels[0].curfile].mode)==FTYPE_REG &&
		     filtype(panels[0].itembuf[panels[0].curfile].mode)==FTYPE_REG)
		    {
		      silent=1; /* для того, чтобы cmp_files() не выдавал предупреждения с Silent и т.п.*/
		      switch(cmp_files(panels[0].full_dir, panels[0].names+panels[0].itembuf[panels[0].curfile].nname,
				       panels[1].full_dir, panels[1].names+panels[1].itembuf[panels[1].curfile].nname))
			{
			  case 0: /* одинаковые файлы */
			    ask(&win62, 2, 0);
			  break;
			  case 1: /* Различные файлы */
			    ask(&win63, 2, 0);
			  break;
			  case 2: /* ошибка сравнения */
			    ask(&win64, 2, 0);
			  break;
			  default:
			    endwin();
			    fprintf(stderr, "Compar.code error in popdn()\n");
			    exit(-1);
			  break;
			}
		      silent=0;
		    }else{ /* сообщить о том, что можно сравнивать только рег.файлы*/
		      ask(&win65, 2, 0);
		    }
		}
		stop++;
	      break;

	      case 10: /* Go to... */
		go_to();
		stop++;
	      break;

	      case 11: /* pRocess control */
		viewkill();
		stop++;
	      break;


	      default:
		if(inmenu)
		  { /* вышли из меню по стрелке влево/вправо */
		    /* обработка - в конце цикла */
		  }else{
		    stop++;
		  }
	      break;
	     }
	 break;

	 case 3: /*------------------------ Options -----------------------*/
	   wins[curr_1]->txts[4].txt[0]= (prompt==0) ? '+' : ' ';
	   wins[curr_1]->txts[5].txt[0]= (prompt==1) ? '+' : ' ';
	   wins[curr_1]->txts[6].txt[0]= (prompt==2) ? '+' : ' ';
	   wins[curr_1]->txts[7].txt[0]= (clock_need)? '+' : ' ';

	   switch(nfile=ask(wins[curr_1],0,VERT|OUT))
	     {
	      case 0: /* Configuration */
		config();
		stop++;
	      break;

	      case 1: /* Save setup */
		save_setup();
		stop++;
	      break;

	      case 2: /* Read setup */
		read_setup();
		out_oth=1;
		read_panels=(1<<0)|(1<<1);
		stop++;
	      break;

	      case 4: /* Short prompt */
	      case 5: /* Node name prompt */
	      case 6: /* User@Node name prompt */
		prompt=nfile-4;
		out_oth=1;
		stop++;
	      break;

	      case 7: /* cLock */
		clock_need=1-clock_need;
		stop++;
	      break;

	      default:
		if(inmenu)
		  { /* вышли из меню по стрелке влево/вправо */
		    /* обработка - в конце цикла */
		  }else{
		    stop++;
		  }
	      break;
	     }
	 break;

	 default:
	   stop++;
	 break;
	}
      if(inmenu){ /* был выход из подменю по стрелке влево/вправо*/
	switch(inmenu)
	  {
	    case K_SR:         /*стрелка вправо*/
	    case KEY_RIGHT:    /*стрелка вправо*/
	      curr_1=calcnext(&win24,curr_1);
	    break;
	    case K_SL:  /*стрелка влево*/
	    case KEY_LEFT:  /*стрелка влево*/
	      curr_1=calcpred(&win24,curr_1);
	    break;
	    default:
	      fprintf(stderr,"Programmer error in popdn()");
	      getchar();
	    break;
	  }
	overwrite(save_scr, stdscr);
#ifdef FREEBSD
	touchwin(stdscr);
#endif
	/* очистить верхнюю строку цветом меню (outwin() этого не сделает)*/
	attrset(*(win24.attr));
	move(0,1);
	for(nfile=win24.cols; nfile--;) addch(' ');
	outwin(&win24, curr_1, -1, stdscr);
	attrset(A_NORMAL);
      }

} /* while */

overwrite(save_scr, stdscr);
#ifdef FREEBSD
  touchwin(stdscr);
#endif

if(read_panels){
    for(outpanel=0;outpanel<NPANELS;outpanel++){
      if(read_panels & (1<<outpanel)){
	chdir(panels[outpanel].full_dir);
	nfile=panels[outpanel].curfile;
	nfirst=panels[outpanel].firstfile;
	read_to_panel();  /* extern outpanel */
	if(nfile<panels[outpanel].nfiles){ /* найден*/
	  panels[outpanel].curfile=nfile;
	  panels[outpanel].firstfile=nfirst;
	}
      }
    }
}

for(outpanel=0;outpanel<NPANELS;outpanel++){
  refr_panel();  /* extern outpanel */
}
touchwin(stdscr);

refresh();
if(out_oth){
  touchwin(dn_menu);
  wrefresh(dn_menu);
  refr_inp();
  touchwin(inp_win);
  wrefresh(inp_win); /* курсор должен торчать в месте ввода команды! */
}
nosave_ask=0;   /* признак "несохранения" экрана для ask */
}
