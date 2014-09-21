#include "defs.h"
/****************************************************************************/
			devt_str(str, value)
/*              Преобразование значения value к выводу в строке str
*****************************************************************************/
	char    *str;
	dev_data value;
{
	char    tmp[128];
	char    *ptr= &tmp[sizeof(tmp)-2];
	int     len;
	int     num;    /* количество "троек"*/
	char    *targ;  /* место, в которое записывается символ*/
	char    *src;   /* место, из которого считывается символ*/
	int     i;      /* счетчик переписанных символов*/
/*==========================================================================*/
if(sizeof(dev_data) > sizeof(long) && ((dev_data)((ulong)value)) != value)
  { /* скорее всего - это long long */
    *ptr-- = '\0';
    while(value>0){
      *ptr-- = ( value % 10 ) + '0';
      value /= 10;
    }
    ptr++;
    strcpy( str, ptr );
  }else{ /* помещающееся значение */

    sprintf(str,"%lu", (ulong) value);
  }

/* кусок взят из decimals() */
len=strlen(str);
num=(len-1)/3;

targ=str+num+len-1;
if(num>0) targ[1]='\0';
src=str+len-1;
i=0;
while(num>0){
  *targ--=*src--;
  i++;
  if(i%3==0){
    *targ--=' ';
    num--;
  }
}

}
