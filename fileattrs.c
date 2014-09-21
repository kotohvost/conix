#include "defs.h"
/****************************************************************************/
				fileattrs()
/*                      Установка режимов доступа к [отмеченным] файлам
/****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  win     win30;  /* окно ошибки изменения режима*/
extern  struct  win     win31;  /* окно запроса изменения режима */
extern  struct  win     win32;  /* окно запроса изменения режима */
	char    str[30];
	int     stop=0;         /* признак прекращения удаления */
	int     say_error=1;    /* сообщать об ошибках */
	char    *name;          /* имя файла */
 struct itembuf *item;          /* для обработки выбранного файла*/
	int     nfile;          /* номер текущего файла*/
	int     nfile_s;        /* номер текущего файла*/
	int     nfirst;         /* номер первого выводимого файла*/
	ushort  mode_set;       /* режим доступа (set) */
	ushort  mode_clr;       /* режим доступа (clear) */
	ushort  mode;           /* режим доступа (полный) */
/*==========================================================================*/
nfile_s=panels[cpanel].curfile;
nfirst=panels[cpanel].firstfile;

if(panels[cpanel].selected>1)
  {
   sprintf(str, "%d files.", panels[cpanel].selected);
   win31.txts[1].txt = str;
   win31.txts[1].x=(win31.cols-strlen(str))/2;
   mode_clr=mode_set=0;
   if(set_clear(&win31, &mode_set, &mode_clr)==6){

     for( nfile=0,item= panels[cpanel].itembuf;
	  stop==0 && panels[cpanel].selected && nfile<=panels[cpanel].nfiles;
	  nfile++, item++){
	if(item->attrs & MARKED){ /* выбранный файл */
	  mode= (item->mode | mode_set) & (~mode_clr);

	  name=panels[cpanel].names+item->nname;
	  if(strlen(name) >= sizeof(str))
	    { /* полное имя не помещается */
	      strcpy(str,"...");
	      strncpy(str+3, name, sizeof(str)-3);
	      str[sizeof(str)-1]='\0';
	    }else{ /* имя файла помещается целиком*/
	      strcpy(str,name);
	    }

	  if(chmod(name, mode))
	    { /* ошибка изменения режима */
	      if(say_error){
		win30.txts[1].x=(win30.cols-strlen(str))/2;
		win30.txts[1].txt=str;
		switch(ask(&win30,2,HOR))
		  {
		    default:
		    case 2: /* OK */
		    break;

		    case 3: /* Silently - не спрашивать (в nc этого нет)*/
		      say_error=0;
		    break;

		    case 4: /* Cancel (в nc этого нет)*/
		      stop=1;
		    break;
		  }
	      }
	    }else{ /* режим доступа нормально изменен */
	      item->attrs ^= MARKED;
	      panels[cpanel].selected--;
	      panels[cpanel].size_sel-=item->size;
	    }
	} /* if MARKED */
     } /* for по файлам */
   }
  }else{/*----------- Нет отмеченных файлов или отмечен 1 файл -------------*/

   if(panels[cpanel].selected)
     { /* отмечен 1 файл */
       for( nfile=0,item= panels[cpanel].itembuf;
	    nfile<=panels[cpanel].nfiles && !(item->attrs & MARKED);
	    nfile++, item++);
     }else{ /* нет отмеченных */
       item= &panels[cpanel].itembuf[panels[cpanel].curfile];
     }
    /* Сформировать имя файла для запросов*/
    name=panels[cpanel].names+item->nname;
    if(strlen(name) >= sizeof(str))
      { /* полное имя не помещается */
	strcpy(str,"...");
	strncpy(str+3, name, sizeof(str)-3);
	str[sizeof(str)-1]='\0';
      }else{ /* имя файла помещается целиком*/
	strcpy(str,name);
      }

    win32.txts[1].txt = str;
    win32.txts[1].x=(win32.cols-strlen(str))/2;
    mode_set=item->mode;
    if(set_clear(&win32, &mode_set, NULL)==6){

      if(chmod(name, mode_set))
	{ /* ошибка изменения режима */
	  win30.txts[1].x=(win30.cols-strlen(str))/2;
	  win30.txts[1].txt=str;
	  ask(&win30,2,HOR);
	}
    }
  } /* if ... selected ... else */


outpanel=cpanel;
read_to_panel();  /* extern outpanel */
if(nfile_s<panels[cpanel].nfiles) {
  panels[cpanel].firstfile=nfirst;
  panels[cpanel].curfile=nfile_s;
  refr_panel();
}
wrefresh(panels[cpanel].win);
for(outpanel=0;outpanel<NPANELS;outpanel++){
  if(outpanel!=cpanel &&
     strcmp(panels[cpanel].full_dir,panels[outpanel].full_dir)==0){
    read_to_panel();
    if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
  }
}
}

