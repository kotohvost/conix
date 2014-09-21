#include "defs.h"
/****************************************************************************/
			hisnext(his_go)
/*              Поиск следующей команды в истории
*****************************************************************************/
	int     his_go;         /* номер элемента history, от которого ищем*/
{
extern  char    *history;       /* буфер хранения истории */
extern  size_t  his_len;        /* место, выделенное для history*/
extern  int     his_cur;        /* номер элемента history для заполнения */
	int     find;           /* признак для поиска */
/*==========================================================================*/
his_go++;
while(his_go!=his_cur && history[his_go] && his_go<his_len-1) his_go++;
if(his_go>=his_len-1 || (history[his_go+1]==1 && his_go+1!=his_cur))
  { /* в конце буфера */
    his_go=0;
  }else{
    if(!history[his_go]) his_go++;
  }

return(his_go);
}
