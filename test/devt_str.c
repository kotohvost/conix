#include "../defs.h"
main()
{
char s[128];
dev_data a=3*1024*1024;

a *= 2*1024;
printf("size:%d\n", sizeof(a));

while(a>0){
  devt_str(s, a++);
  fprintf(stderr, "%s   \r", s);
}
}

/****************************************************************************/
			devt_str(str, value)
/*              �८�ࠧ������ ���祭�� value � �뢮�� � ��ப� str
*****************************************************************************/
	char    *str;
	dev_data value;
{
	char    tmp[128];
	char    *ptr= &tmp[sizeof(tmp)-2];
	int     len;
	int     num;    /* ������⢮ "�஥�"*/
	char    *targ;  /* ����, � ���஥ �����뢠���� ᨬ���*/
	char    *src;   /* ����, �� ���ண� ���뢠���� ᨬ���*/
	int     i;      /* ���稪 ��९�ᠭ��� ᨬ�����*/
/*==========================================================================*/
if(1 || sizeof(dev_data) > sizeof(long) && ((dev_data)((ulong)value)) != value)
  { /* ᪮॥ �ᥣ� - �� long long */
    *ptr-- = '\0';
    while(value>0){
      *ptr-- = ( value % 10 ) + '0';
      value /= 10;
    }
    ptr++;
    strcpy( str, ptr );
  }else{ /* ������饥�� ���祭�� */

    sprintf(str,"%lu", (ulong) value);
  }

/* ��᮪ ���� �� decimals() */
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
