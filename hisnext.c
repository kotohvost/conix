#include "defs.h"
/****************************************************************************/
			hisnext(his_go)
/*              ���� ᫥���饩 ������� � ���ਨ
*****************************************************************************/
	int     his_go;         /* ����� ����� history, �� ���ண� �饬*/
{
extern  char    *history;       /* ���� �࠭���� ���ਨ */
extern  size_t  his_len;        /* ����, �뤥������ ��� history*/
extern  int     his_cur;        /* ����� ����� history ��� ���������� */
	int     find;           /* �ਧ��� ��� ���᪠ */
/*==========================================================================*/
his_go++;
while(his_go!=his_cur && history[his_go] && his_go<his_len-1) his_go++;
if(his_go>=his_len-1 || (history[his_go+1]==1 && his_go+1!=his_cur))
  { /* � ���� ���� */
    his_go=0;
  }else{
    if(!history[his_go]) his_go++;
  }

return(his_go);
}
