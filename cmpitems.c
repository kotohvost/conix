#include "defs.h"
/****************************************************************************/
			cmpitems(ptr1, ptr2)
/*      Сравнение элементов описаний файлов для qsort
*****************************************************************************/
register struct itembuf *ptr1;
register struct itembuf *ptr2;
{
extern  struct  panel   panels[NPANELS];
	unsigned short   type1; /* тип файла 1*/
	unsigned short   type2; /* тип файла 2*/
	int     result;
	char    *ch_ptr1;
	char    *ch_ptr2;
	char    *name1;
	char    *name2;
extern  int     outpanel;       /*обрабатываемая панель */
/*==========================================================================*/
type1=filtype(ptr1->mode);
type2=filtype(ptr2->mode);
if(type1!=type2)
  { /* файлы разных типов, независимо от вида сортировки сравниваются типы*/
    return(type1-type2);
  }else{
    name1=panels[outpanel].names+ptr1->nname;
    name2=panels[outpanel].names+ptr2->nname;

    switch(panels[outpanel].sort)
      {
	case 1: /*сортировка по именам */
	  return(strcmp(name1, name2));
	break;

	case 2: /* сортировка по расширениям*/
	  for(ch_ptr1=name1; *ch_ptr1; ch_ptr1++);
	  while(*ch_ptr1!='.' && ch_ptr1!=name1) ch_ptr1--;

	  for(ch_ptr2=name2; *ch_ptr2; ch_ptr2++);
	  while(*ch_ptr2!='.' && ch_ptr2!=name2) ch_ptr2--;

	  if(*ch_ptr1=='.' && *ch_ptr2=='.')
	    { /* оба файла имеют расширение */
	      if( (result=strcmp(ch_ptr1, ch_ptr2)) )
		{ /* разные расширения*/
		  return(result);
		}else{ /* одинаковые расширения, сравнить имена*/
		  return(strcmp(name1, name2));
		}
	    }else{
	       if(*ch_ptr1=='.') { /*первый файл с расширением, второй-нет*/
		   return(1);
	       }else if(*ch_ptr2=='.'){ /*только второй - с расширением*/
		   return(-1);
	       }else{ /* оба файла без расширения, сравнить имена*/
		   return(strcmp(name1, name2));
	       }
	    }
	break;

	case 3: /*сортировка по времени модификации*/
	  return(ptr2->mtime-ptr1->mtime);
	break;

	case 4: /*сортировка по размеру*/
	  return(ptr2->size-ptr1->size);
	break;

      }
  }

endwin();
printf("Programmer's error in cmpitems()\n");
exit(-1);
}
