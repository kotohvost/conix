#include "defs.h"
/****************************************************************************/
			deselectgroup()
/*                      снятие отметки с группы файлов
/****************************************************************************/
{
extern  struct  win     win35;          /* окно с запросом deselect */
extern  struct  win     win34;          /* окно с ошибкой */
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  char    *fnd_str;       /* строка для временных данных */
extern  size_t  fnd_len;        /* место, выделенное для fnd_str*/
	int     deselected=0;   /* количество "разотмеченных" файлов*/
	char    str[20];
	int     nfile;
 struct inp_lst inp_lst[2];       /* список ввода для inp_menu()*/
 struct itembuf *item;          /* для обработки выбранного файла*/
/*==========================================================================*/
inp_lst[0].str=fnd_str;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* признак конца списка для inp_menu()*/

if(!fnd_str[0]) strcpy(fnd_str, "*");
if(inp_menu(inp_lst, &win35, 1, 0)==1){
  for(nfile=2, item=panels[cpanel].itembuf+2;
      nfile<panels[cpanel].nfiles;
      item++,nfile++){
    if(inregular(panels[cpanel].names+item->nname, fnd_str)){
      deselected++;
      if(item->attrs & MARKED){
	item->attrs ^= MARKED;
	panels[cpanel].selected--;
	panels[cpanel].size_sel-=item->size;
      }
    }
  }
  if(!deselected){
    str[sizeof(str)-1]='\0';
    strncpy(str, fnd_str, sizeof(str));
    if(str[sizeof(str)-1]){ /* не поместилось*/
      str[sizeof(str)-1]='\0';
      str[sizeof(str)-2]=str[sizeof(str)-3]=str[sizeof(str)-4]='.';
    }
    win34.txts[1].txt=str;
    win34.txts[1].x=(win34.cols-strlen(str))/2;
    ask(&win34, 2, 0);
  }
  outpanel=cpanel;
  refr_panel();
  if(panels[cpanel].on>0) wrefresh(panels[cpanel].win);
}
}

