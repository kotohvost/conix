#include "defs.h"
/****************************************************************************/
			invertselect()
/*                      инверсия отметки группы файлов
/****************************************************************************/
{
extern  int     cpanel;         /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;       /*обрабатываемая панель */
	int     nfile;
 struct itembuf *item;          /* для обработки выбранного файла*/
/*==========================================================================*/

for(nfile=2, item=panels[cpanel].itembuf+2;
  nfile<panels[cpanel].nfiles;
  item++,nfile++){
  item->attrs ^= MARKED;
  if(item->attrs & MARKED)
    {
      panels[cpanel].selected++;
      panels[cpanel].size_sel+=item->size;
    }else{
      panels[cpanel].selected--;
      panels[cpanel].size_sel-=item->size;
    }
}

outpanel=cpanel;
refr_panel();
if(panels[cpanel].on>0) wrefresh(panels[cpanel].win);
}
