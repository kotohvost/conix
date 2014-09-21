#include "defs.h"
/****************************************************************************/
			int     files_on_page(npanel)
/*              Определение количества файлов на странице панели npanel
/****************************************************************************/
	int     npanel;
{
extern  struct  panel   panels[];
	int     i;
/*==========================================================================*/
if(panels[npanel].on==1)
  { /* одна колонка файлов */
    i=panels[npanel].lines-panels[npanel].lnused;
  }else{ /* Brief */
    i=panels[npanel].cols/3;
    if(i>OUTNAMELEN+3 || i<8) i = OUTNAMELEN+3;

    i = (panels[npanel].lines-panels[npanel].lnused) *
	(panels[npanel].cols/i);

  }
return ( i );
}
