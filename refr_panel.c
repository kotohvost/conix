#include "defs.h"
#include <time.h>
#include <sys/stat.h>
#define FIRST_STR 2
/****************************************************************************/
			refr_panel()
/*              Вывод на экран панели outpanel
*****************************************************************************/
{
extern  int     outpanel;       /*обрабатываемая панель */
extern  int     cpanel;
extern  struct  panel   panels[];
extern  chtype  atr[];          /* раскраска */
extern struct fields fields[];  /* характеристики возможных полей вывода*/
extern  short   pred_file;      /* номер передыдущего файла для refr_panel()*/
extern  int     clock_need;     /* признак вывода времени 0-нет 1-да */
extern  int     dwidth;         /* признак панелей двойной ширины */
	int     nfile=0;        /* номер текущего файла*/
	int     lastfile;
	char    str[128];       /* строка для формирования вывода */
	int     nstr=FIRST_STR;
	WINDOW  *win=panels[outpanel].win;
	char    *ptr;
	short   dirlen;         /*длина имени каталога*/
	short   maxlen;         /* макс длина выводимого имени каталога*/
	char    tmp[128];       /* выводимая часть пути текущего каталога и для др.врем.исп.*/
	char    ftch;           /* символ обозначения типа файла*/
	int     nfield;
	int     nf;
	short   x;
	int     i;
 struct itembuf *item;          /* для обработки выбранного файла*/
 struct tm      *tm;            /* для работы с датой/временем */
	short   fieldsize;      /* размер поля */
	chtype  attrib;         /* раскраска */
	short   size;           /* для расчета количества полей в панелях */
	short   save_size;      /* для сохранения размера поля имени */
	short   outfields[MAXFIELDS+1];
	short   name_count;     /* количество полей с именем (по хорошему==1)*/
	short   name_num;       /* номер элемента fields[] с именем */
	ushort  mode;
	int     stop;           /* признак прекращения вывода при перемещении указетеля */
/*==========================================================================*/

if ( dwidth && outpanel!=cpanel ) return; /* предотвращение вывода неактивной панели двойной ширины */

if(panels[outpanel].on==1)
  {

    lastfile=files_on_page(outpanel) + panels[outpanel].firstfile -1;
    if(panels[outpanel].nfiles<=lastfile) lastfile=panels[outpanel].nfiles-1;
    panels[outpanel].lastfile=lastfile;

    if(pred_file<0){ /* вывод панели целиком */
      mywbkgd(win, MAIN_PANEL_ATTR);
      wattrset(win, MAIN_PANEL_BOX_ATTR);
      box(win, 0,0);
    }

    /* Сделать расчет количества помещающихся полей */
    name_count=0;
    size=panels[outpanel].cols-2;
    for(nfield=0,i=0; nfield<MAXFIELDS; nfield++){
      nf=panels[outpanel].outfields[nfield];
      if(nf>=0 && ((fieldsize=fields[nf].fieldsize)<=size+1)){
	outfields[i]=nf;
	size-=fieldsize;
	if(fields[nf].fieldtype==FIELD_NAME){
	  name_count++;
	  name_num=nf;
	}
	i++;
      }
    }
    outfields[i]=(-1);

    save_size=fields[name_num].fieldsize;
    if(name_count && size/name_count>=0){
      /* в поле имени можно добавить символов */
      fields[name_num].fieldsize+=(size+1)/name_count;
    }

    if(pred_file<0){ /* вывод панели целиком */
      /* Заголовок */
      for(x=1,nfield=0; outfields[nfield]>=0; nfield++){
	nf=outfields[nfield];
	fieldsize=fields[nf].fieldsize;

	if(x+fieldsize>3 && fieldsize>2 && outfields[nfield+1]>=0){ /* нарисовать разделитель полей*/
	  wmove(win, 2, x+fieldsize-1);
	  wattrset(win, MAIN_PANEL_BOX_ATTR);
	  wvline(win, 0, panels[outpanel].lines-4);
	}

	wmove(win, 1, x);
	wattrset(win, MAIN_PANEL_TOP_ATTR);
	waddstr(win, fields[nf].fieldname);
	if(fields[nf].fieldtype==FIELD_NAME &&
	   (panels[outpanel].filter_read[0] || panels[outpanel].filter_noread[0])){
	   /* установлен фильтр */
	   waddstr(win, "(Filter)");
	}
	x+=fieldsize;
      }


      wattrset(win, MAIN_PANEL_BOX_ATTR);
      wmove(win, panels[outpanel].lines-3, 1);
      whline(win, 0, panels[outpanel].cols-2);

      if(clock_need)
	{
	  maxlen=panels[outpanel].cols-11;
	}else{
	  maxlen=panels[outpanel].cols-2;
	}
      name_to_out(panels[outpanel].full_dir, tmp, maxlen);

      if(outpanel==cpanel)
	{ /* текущая панель */
	  wattrset(win, MAIN_PANEL_DIR_ATTR);
	}else{
	  wattrset(win, MAIN_PANEL_ATTR);
	}
      dirlen=strlen(tmp);
      wmove(win, 0, (panels[outpanel].cols-dirlen)/2);
      waddstr(win, tmp);
    } /* if(pred_file... */

    wattrset(win, MAIN_PANEL_ATTR);


    if(pred_file<0)
      { /* вывод панели целиком */
	nfile=panels[outpanel].firstfile;
      }else{ /* только перемещение указателя */
	nfile=pred_file;
	nstr+=pred_file-panels[outpanel].firstfile;
      }

    item= &panels[outpanel].itembuf[nfile];
    stop=0;

    for(; nfile<=lastfile && stop==0; nfile++,nstr++,item++){
      if((nfile==panels[outpanel].curfile) && (outpanel==cpanel))
	{ /* Текущий файл на текущей панели*/
	  if((item->attrs) & MARKED)
	    { /* файл отмечен */
	      attrib=MAIN_PANEL_OUTL_PTR_ATTR;
	    }else{ /* не отмечен */
	      attrib=MAIN_PANEL_PTR_ATTR;
	    }
	}else{
	  if((item->attrs) & MARKED)
	    { /* файл отмечен */
	      attrib=MAIN_PANEL_OUTL_ATTR;
	    }else{ /* не отмечен */
	      attrib=MAIN_PANEL_ATTR;
	    }
	}

      tm=NULL;

      for(x=1,nfield=0; outfields[nfield]>=0; nfield++){
	nf=outfields[nfield];
	fieldsize=fields[nf].fieldsize;
	if(outfields[nfield+1]<0 && fieldsize==1)
	  { /* последнее поле - 1 символ (без разграничителя) */
	    wmove(win, nstr, x-1); /* нарисуем его на разрграничитель предыд.*/
	  }else{
	    wmove(win, nstr, x);
	  }

	wattrset(win, attrib);
	switch(fields[nf].fieldtype)
	  {
	    case FIELD_MARK: /*------- Дублирование отметки символом ---------*/
	      wattrset(win, MAIN_PANEL_ATTR);
	      str[0]= (item->attrs & MARKED) ? '+' : ' ';
	      str[1]='\0';
	    break;

	    case FIELD_NAME: /*---------------- имя файла --------------------*/
	      str[fieldsize-1]='\0';
	      strncpy(str, panels[outpanel].names+item->nname, fieldsize);

	      if(str[fieldsize-1])
		{ /* длинное имя файла, на месте верт.линии рисуем '>' */
		  str[fieldsize-2]='>';
		  str[fieldsize-1]='\0';
		}else{
		  for(ptr=str; *ptr; ptr++);
		  for(;ptr!=&str[fieldsize-1]; ptr++) *ptr=' ';
		}

	      for(i=0; str[i]; i++){
		if(((unsigned char*)str)[i]<' ') str[i]='?';
	      }
	    break;

	    case FIELD_TYPE: /*-------------- поле типа файла ----------------*/
	      switch(filtype(item->mode))
		{
		  case FTYPE_DIR:
		    str[0]='/';
		  break;

		  case FTYPE_CHR:
		    str[0]='$';
		  break;

		  case FTYPE_BLK:
		    str[0]='#';
		  break;

		  case FTYPE_FIFO:
		    str[0]='|';
		  break;

		  case FTYPE_NAM:
		    str[0]='~';
		  break;

		  case FTYPE_SLN:
		    str[0]='>';
		  break;

		  case FTYPE_REG:
		    if(item->nlink>1)
		      {
			str[0]='L';
		      }else{
			str[0]=' ';
		      }
		  break;

		  case FTYPE_UNKNOWN:
		    str[0]='?';
		  break;

		  case FTYPE_SOFIFO:
		    str[0]='=';
		  break;

		  default:
		    endwin();
		    printf("Programmer's error in refr_panel()\n");
		    exit(-1);
		  break;
		}
	      str[1]='\0';
	      wattrset(win, MAIN_PANEL_ATTR);
	    break;

	    case FIELD_SIZE: /*-------------- поле размера файла -------------*/
	      sprintf(str, "%ld", item->size);
	      if(strlen(str)>fieldsize-1){ /* попробовать в Kb */
		sprintf(str, "%ldK", item->size/1024);
	      }
	      if(strlen(str)>fieldsize-1){ /* попробовать в Mb */
		sprintf(str, "%ldM", item->size/1024/1024);
	      }

	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_DATE:
	      if(!tm) tm=localtime(&(item->mtime));
	      while(tm->tm_year >= 100) tm->tm_year-=100; /* а вдруг доживет до 2000 года?*/
	      sprintf(str, "%02d.%02d.%02d", tm->tm_mday, tm->tm_mon+1, tm->tm_year);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_TIME:
	      if(!tm) tm=localtime(&(item->mtime));
	      sprintf(str, "%02d:%02d", tm->tm_hour, tm->tm_min);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_INUM:
	      sprintf(str, "%d", item->inum);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_MODE:
	      mode=item->mode;
	      ptr=str;

	      /*-------- Owner ---------*/
	      *ptr++= (mode&S_IRUSR) ? 'r' : '-';
	      *ptr++= (mode&S_IWUSR) ? 'w' : '-';

	      if( (mode&S_ISUID) && (mode&S_IXUSR) ){
		*ptr++= 's';
	      }else if(mode&S_ISUID){
		*ptr++= 'S';
	      }else if(mode&S_IXUSR){
		*ptr++= 'x';
	      }else{
		*ptr++= '-';
	      }

	      /*-------- Group ---------*/
	      *ptr++= (mode&S_IRGRP) ? 'r' : '-';
	      *ptr++= (mode&S_IWGRP) ? 'w' : '-';

	      if( (mode&S_ISGID) && (mode&S_IXGRP) ){
		*ptr++= 's';
	      }else if(mode&S_ISGID){
		*ptr++= 'S';
	      }else if(mode&S_IXGRP){
		*ptr++= 'x';
	      }else{
		*ptr++= '-';
	      }

	      /*-------- Others ---------*/
	      *ptr++= (mode&S_IROTH) ? 'r' : '-';
	      *ptr++= (mode&S_IWOTH) ? 'w' : '-';

	      if( (mode&S_ISVTX) && (mode&S_IXOTH) ){
		*ptr++= 't';
	      }else if(mode&S_ISVTX){
		*ptr++= 'T';
	      }else if(mode&S_IXOTH){
		*ptr++= 'x';
	      }else{
		*ptr++= '-';
	      }

	      *ptr='\0';

	    break;

	    case FIELD_OWNR:
	      sprintf(str, "%d", item->owner);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_GRP:
	      sprintf(str, "%d", item->group);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_NLNK:
	      sprintf(str, "%d", item->nlink);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	  } /* switch */

	/* Вывод сформированной строки поля */
	waddstr(win, str);
	if(nfile==panels[outpanel].curfile && panels[outpanel].selected==0){
	  /* дублирование текущего файла в status-строке*/
	  if(outfields[nfield+1]<0 && fieldsize==1)
	    { /* последнее поле - 1 символ (без разграничителя) */
	      wmove(win, panels[outpanel].lines-2, x-1); /* нарисуем его на разрграничитель предыд.*/
	    }else{
	      wmove(win, panels[outpanel].lines-2, x);
	    }
	  wattrset(win, MAIN_PANEL_ATTR);
	  waddstr(win, str);
	}

	x+=fieldsize;

      } /* for по полям */
      if(pred_file>=0){ /* только перемещение указателя*/
	if(nfile==pred_file && cpanel==outpanel)
	  { /* сейчас погашено выделение предыдущего файла*/
	    nfile=panels[outpanel].curfile-1; /* т.к. в конце цикла nfile++...*/
	    item= &panels[outpanel].itembuf[nfile];
	    nstr=FIRST_STR+nfile-panels[outpanel].firstfile;
	  }else{ /* включено выделение текущего файла*/
	    stop=1;
	  }
      }
    } /* for по файлам */

    if(panels[outpanel].selected){ /* есть отмеченные файлы*/
      wmove(win, panels[outpanel].lines-2, 1);
      wattrset(win, MAIN_PANEL_BOT_OUT_ATTR);
      /* Подсчет длины строки, занимаемой числами*/
      sprintf(tmp,"%ld%d", panels[outpanel].size_sel, panels[outpanel].selected);
      maxlen=strlen(tmp);
      if(maxlen<=10)
	{ /* размер пишем в байтах*/
	  devt_str(tmp, (dev_data)panels[outpanel].size_sel);
	  wprintw(win, "%s bytes in %d selected file%c",
		      tmp,
		      panels[outpanel].selected,
		      (panels[outpanel].selected==1)?' ':'s');
	}else  if(maxlen<=13) {
	  devt_str(tmp, (dev_data)panels[outpanel].size_sel/1024);
	  wprintw(win, "%sK bytes in %d selected file%c",
		      tmp,
		      panels[outpanel].selected,
		      (panels[outpanel].selected==1)?' ':'s');

	}else{
	  devt_str(tmp, (dev_data)panels[outpanel].size_sel/(1024L*1024L));
	  wprintw(win, "%sM bytes in %d selected file%c",
		      tmp,
		      panels[outpanel].selected,
		      (panels[outpanel].selected==1)?' ':'s');
	}
    }
    fields[name_num].fieldsize=save_size; /* размер мог быть изменен*/

    if(panels[1-outpanel].on==2 && pred_file<0) {
      /* вывод в противоположном окне информации о файловой системе*/
      show_inf();
    }

  }else if(panels[outpanel].on==3){ /* вывод в формате Brief */
    refr_brief();
  }


pred_file=(-1);
}
