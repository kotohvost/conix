#include "defs.h"
/****************************************************************************/
			selectgroup()
/*                      Отметка группы файлов
/****************************************************************************/
{
extern  struct  win     win33;          /* окно с запросом select */
extern  struct  win     win34;          /* окно с ошибкой */
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
extern  char    *fnd_str;       /* строка для временных данных */
extern  size_t  fnd_len;        /* место, выделенное для fnd_str*/
	int     selected=0;             /* количество отмеченных файлов*/
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
if(inp_menu(inp_lst, &win33, 1, 0)==1){
  for(nfile=2, item=panels[cpanel].itembuf+2;
      nfile<panels[cpanel].nfiles;
      item++,nfile++){
    if(inregular(panels[cpanel].names+item->nname, fnd_str)){
      selected++;
      if(!(item->attrs & MARKED)){
	item->attrs |= MARKED;
	panels[cpanel].selected++;
	panels[cpanel].size_sel+=item->size;
      }
    }
  }
  if(!selected){
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
  if(panels[cpanel].on<0) panels[cpanel].on *= -1;
  refr_panel();
  wrefresh(panels[cpanel].win);
}
}
