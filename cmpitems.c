#include "defs.h"
/****************************************************************************/
			cmpitems(ptr1, ptr2)
/*      �ࠢ����� ����⮢ ���ᠭ�� 䠩��� ��� qsort
*****************************************************************************/
register struct itembuf *ptr1;
register struct itembuf *ptr2;
{
extern  struct  panel   panels[NPANELS];
	unsigned short   type1; /* ⨯ 䠩�� 1*/
	unsigned short   type2; /* ⨯ 䠩�� 2*/
	int     result;
	char    *ch_ptr1;
	char    *ch_ptr2;
	char    *name1;
	char    *name2;
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
/*==========================================================================*/
type1=filtype(ptr1->mode);
type2=filtype(ptr2->mode);
if(type1!=type2)
  { /* 䠩�� ࠧ��� ⨯��, ������ᨬ� �� ���� ���஢�� �ࠢ�������� ⨯�*/
    return(type1-type2);
  }else{
    name1=panels[outpanel].names+ptr1->nname;
    name2=panels[outpanel].names+ptr2->nname;

    switch(panels[outpanel].sort)
      {
	case 1: /*���஢�� �� ������ */
	  return(strcmp(name1, name2));
	break;

	case 2: /* ���஢�� �� ���७��*/
	  for(ch_ptr1=name1; *ch_ptr1; ch_ptr1++);
	  while(*ch_ptr1!='.' && ch_ptr1!=name1) ch_ptr1--;

	  for(ch_ptr2=name2; *ch_ptr2; ch_ptr2++);
	  while(*ch_ptr2!='.' && ch_ptr2!=name2) ch_ptr2--;

	  if(*ch_ptr1=='.' && *ch_ptr2=='.')
	    { /* ��� 䠩�� ����� ���७�� */
	      if( (result=strcmp(ch_ptr1, ch_ptr2)) )
		{ /* ࠧ�� ���७��*/
		  return(result);
		}else{ /* ��������� ���७��, �ࠢ���� �����*/
		  return(strcmp(name1, name2));
		}
	    }else{
	       if(*ch_ptr1=='.') { /*���� 䠩� � ���७���, ��ன-���*/
		   return(1);
	       }else if(*ch_ptr2=='.'){ /*⮫쪮 ��ன - � ���७���*/
		   return(-1);
	       }else{ /* ��� 䠩�� ��� ���७��, �ࠢ���� �����*/
		   return(strcmp(name1, name2));
	       }
	    }
	break;

	case 3: /*���஢�� �� �६��� ����䨪�樨*/
	  return(ptr2->mtime-ptr1->mtime);
	break;

	case 4: /*���஢�� �� ࠧ����*/
	  return(ptr2->size-ptr1->size);
	break;

      }
  }

endwin();
printf("Programmer's error in cmpitems()\n");
exit(-1);
}
