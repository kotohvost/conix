#include "defs.h"
/****************************************************************************/
			invertselect()
/*                      ������� �⬥⪨ ��㯯� 䠩���
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
	int     nfile;
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
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
