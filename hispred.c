#include "defs.h"
/****************************************************************************/
			hispred(his_go)
/*              ���� �।��饩 ������� � ���ਨ
*****************************************************************************/
	int     his_go;         /* ����� ����� history, �� ���ண� �饬*/
{
extern  char    *history;       /* ���� �࠭���� ���ਨ */
extern  size_t  his_len;        /* ����, �뤥������ ��� history*/
extern  int     his_cur;        /* ����� ����� history ��� ���������� */
	int     find;           /* �ਧ��� ��� ���᪠ */
/*==========================================================================*/
find=0;
his_go--; /* ���筮 - ���室 �� \0 */
while(!find){
  if(his_go<=0)
    { /* � ��砫� ���� */
      his_go=his_len-1; /* �� \0*/
    }
  his_go--; /* �室 � \0 */
  while(his_go!=his_cur && history[his_go] && his_go) his_go--;
  if(his_go==his_cur)
    {
      find=1; /* ��諨 �� ������ �� ⥪�饩 �������*/
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
