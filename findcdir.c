#include "defs.h"
/****************************************************************************/
				findcdir()
/*                      Поиск файла в текущем каталоге
/****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  win     win40;  /* окно запроса шаблона поиска*/
extern  struct  win     win41;  /* сообщение-запрос (файл не найден)*/
extern  char    *fnd_str;       /* строка для поиска */
extern  size_t  fnd_len;        /* место, выделенное для fnd_str*/
	int     find;           /*признак поиска*/
	int     stop=0;         /*признак повторных поисков */
 struct itembuf *item;          /* для обработки выбранного файла*/
 struct inp_lst inp_lst[2];       /* список ввода для inp_menu()*/
	int     nfile;          /* номер текущего файла*/
/*==========================================================================*/
/* fnd_str[0]='\0'; - не включено для повторных поисков */

inp_lst[0].str=fnd_str;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* признак конца списка для inp_menu()*/

while(!stop){
  find=inp_menu(inp_lst, &win40, 1, HOR|VERT);
  if(find==1 || find==2)
    { /* искать файл */
      find=strlen(fnd_str);
      if(find==0 || fnd_str[find-1]!='*') strcpy(fnd_str+find, "*");
      find=0;
      /* сначала ищем ниже */
      for(nfile=panels[cpanel].curfile+1, item=panels[cpanel].itembuf+nfile;
	  nfile<panels[cpanel].nfiles && !find;
	  item++,nfile++){
	if(inregular(panels[cpanel].names+item->nname, fnd_str)) find=1;
      }
      if(!find){ /* ниже не найден, ищем с начала панели*/
	for(nfile=2, item=panels[cpanel].itembuf+nfile;
	    nfile<panels[cpanel].nfiles && !find;
	    item++,nfile++){
	  if(inregular(panels[cpanel].names+item->nname, fnd_str)) find=1;
	}
      }
      if(find)
	{ /* найден файл, соответствующий шаблону */
	  panels[cpanel].curfile=nfile-1;

	  if(nfile<=panels[cpanel].firstfile ||
	     nfile>panels[cpanel].firstfile+
	       (panels[cpanel].lines-panels[cpanel].lnused-1)){ /*файл - не на тек.странице*/
	      /* попробовать поместить в середину страницы*/
	      panels[cpanel].firstfile=nfile-(panels[cpanel].lines-panels[cpanel].lnused)/2;

	      /* если последняя страница не выведется вся, сместить ее*/
	      if(panels[cpanel].firstfile+panels[cpanel].lines-
		 panels[cpanel].lnused > panels[cpanel].nfiles-1)
		   panels[cpanel].firstfile=panels[cpanel].nfiles-
			  (panels[cpanel].lines-panels[cpanel].lnused);

	      /* если файлов на панели мало, вывести сначала (м.б.это проверено?)*/
	      if(panels[cpanel].firstfile<0) panels[cpanel].firstfile=0;
	  }

	  if(panels[cpanel].on<0) panels[cpanel].on *= -1; /* включить панель, если она была выключена*/
	  outpanel=cpanel;
	  refr_panel();
	  wrefresh(panels[cpanel].win);
	  stop++;
	}else{ /* файл не найден */
	  if(ask(&win41,1,HOR)!=2) stop++;
	}
    }else{ /* отказ от поиска */
      stop++;
    }
}
}
