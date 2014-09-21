#include "defs.h"

/****************************************************************************/
			mknumber(str, maxlen, value)
/*              Преобразование значения value к выводу в строке str
		размером len. При невозможности поместить, преобразуется
		к Kb, Mb, Gb. Отделяются тысячи.
*****************************************************************************/
	char    *str;
	int     maxlen;
	dev_data value;
{
	int     len;
	dev_data tmpval;
/*==========================================================================*/
if( value == (dev_data) -1)
  {
    strncpy(str,"Unknown", maxlen);
    str[maxlen-1]='\0';
  }else{
    /* Подсчет длины строки, занимаемой числами*/
    devt_str(str, value);
    len=strlen(str);

    if(len <= maxlen)
      { /* размер пишем в байтах*/
	/* строка уже сформирована */
      }else{
	value /= 1024;
	devt_str(str, value);
	len=strlen(str);

	if(len <= maxlen)
	  {
	    devt_str(str, value);
	    strcat(str, "K");
	  }else{
	    value /= 1024;
	    devt_str(str, value);
	    len=strlen(str);

	    if(len <= maxlen)
	      {
		devt_str(str, value);
		strcat(str, "M");
	      }else{
		value /= 1024;
		devt_str(str, value);
		len=strlen(str);

		if(len <= maxlen)
		  {
		    devt_str(str, value);
		    strcat(str, "G");
		  }else{
		    strcpy(str, "?");
		  }
	      }
	  }
      }
  }
}

