#include "defs.h"
#include <sys/stat.h>
/****************************************************************************/
				makedir()
/*                      Создание каталога
/****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  win     win22;  /* запрос на задание имени каталога */
extern  struct  win     win23;  /* сообщение об ошибке*/
extern  char    *tmp_str;       /* строка для временных данных */
extern  size_t  tmp_len;        /* место, выделенное для tmp_str*/
 struct itembuf *item;          /* для обработки выбранного файла*/
	int     retcode;        /* код выхода 0-OK, !0-ошибка */
	char    str[25];        /* строка для сообщения*/
	int     nfile;          /* для поиска текущего файла */
	int     nfirst;         /* номер первого выводимого файла*/
 struct inp_lst inp_lst[2];       /* список ввода для inp_menu()*/
/*==========================================================================*/
inp_lst[0].str=tmp_str;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=tmp_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* признак конца списка для inp_menu()*/

switch(inp_menu(inp_lst, &win22, 1, HOR|VERT))
  {
    case 1:  /* имя */
    case 2:  /* Create */
      if( (retcode=mkdir(tmp_str,0777))!=0)
	{ /* каталог не создан */
	  if(strlen(tmp_str) >= sizeof(str))
	    { /* полное имя не помещается */
	      strcpy(str,"...");
	      strncpy(str+3, tmp_str, sizeof(str)-3);
	      str[sizeof(str)-1]='\0';
	    }else{ /* имя файла помещается целиком*/
	      strcpy(str,tmp_str);
	    }
	  win23.txts[1].x=(win23.cols-strlen(str))/2;
	  win23.txts[1].txt=str;
	  ask(&win23,2,0); /* сообщение об ошибке*/
	}
    break;

    default:
      retcode=(-1);
    break;
  }

if(retcode==0){
  /* Восстановление рабочего состояния экрана и установка текущего положения */
  outpanel=cpanel;
  read_to_panel();  /* extern outpanel */
  if(tmp_str[0]){ /* makedir() записала в tmp_str имя созданного каталога*/
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

  for(outpanel=0; outpanel<NPANELS; outpanel++){
    if(outpanel!=cpanel && strcmp(panels[outpanel].full_dir,panels[cpanel].full_dir)==0){
      if(chdir(panels[outpanel].full_dir)) beep();
      nfile=panels[outpanel].curfile;
      nfirst=panels[outpanel].firstfile;
      read_to_panel();  /* extern outpanel */
      if(nfile<panels[outpanel].nfiles) {
	panels[outpanel].firstfile=nfirst;
	panels[outpanel].curfile=nfile;
	refr_panel();
      }
    }
    if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
  }
}
return(retcode);
}
