#include "defs.h"
/****************************************************************************/
			int     files_on_page(npanel)
/*              ��।������ ������⢠ 䠩��� �� ��࠭�� ������ npanel
/****************************************************************************/
	int     npanel;
{
extern  struct  panel   panels[];
	int     i;
/*==========================================================================*/
if(panels[npanel].on==1)
  { /* ���� ������� 䠩��� */
    i=panels[npanel].lines-panels[npanel].lnused;
  }else{ /* Brief */
    i=panels[npanel].cols/3;
    if(i>OUTNAMELEN+3 || i<8) i = OUTNAMELEN+3;

    i = (panels[npanel].lines-panels[npanel].lnused) *
	(panels[npanel].cols/i);

  }
return ( i );
}
