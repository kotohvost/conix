#include "defs.h"
/****************************************************************************/
			hispred(his_go)
/*              Поиск предыдущей команды в истории
*****************************************************************************/
	int     his_go;         /* номер элемента history, от которого ищем*/
{
extern  char    *history;       /* буфер хранения истории */
extern  size_t  his_len;        /* место, выделенное для history*/
extern  int     his_cur;        /* номер элемента history для заполнения */
	int     find;           /* признак для поиска */
/*==========================================================================*/
find=0;
his_go--; /* обычно - переход на \0 */
while(!find){
  if(his_go<=0)
    { /* в начале буфера */
      his_go=his_len-1; /* на \0*/
    }
  his_go--; /* уход с \0 */
  while(his_go!=his_cur && history[his_go] && his_go) his_go--;
  if(his_go==his_cur)
    {
      find=1; /* дошли по кольцу до текущей команды*/
    }else{
      if(his_go==0)
	{
	  find=1;
	}else if(history[his_go+1]!=1 && history[his_go+1]){
	  his_go++;
	  find=1;
	}
    }
}
return(his_go);
}
