#include "defs.h"

/****************************************************************************/
			mknumber(str, maxlen, value)
/*              �८�ࠧ������ ���祭�� value � �뢮�� � ��ப� str
		ࠧ��஬ len. �� ������������ ��������, �८�ࠧ����
		� Kb, Mb, Gb. �⤥������ �����.
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
    /* ������ ����� ��ப�, ���������� �᫠��*/
    devt_str(str, value);
    len=strlen(str);

    if(len <= maxlen)
      { /* ࠧ��� ��襬 � �����*/
	/* ��ப� 㦥 ��ନ஢��� */
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

