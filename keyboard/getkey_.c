#include <curses.h>
#include "keys.h"

#ifndef MS_DOS
/* UNIX - ��⥬� */
/****************************************************************************/
		int getkey_()
/*      ���� ���� � ���������� � ��।������ �� ⨯�� �
		����஢���
���� ���⮢ � �
��� ᮧ����� 26.06.90.
*****************************************************************************/
{
extern struct   cmdkey  stringcom_[];   /* */
#ifdef ALRM_KB
extern int      alarmed;                /* �ਧ��� ⮣�, �� �뫮 ���뢠��� �� alarm()*/
#else
       int      alarmed=0;              /* �ਧ��� ⮣�, �� �뫮 ���뢠��� �� alarm()*/
#endif
       unsigned char    str_[256];      /*��ப�-�⢥� �-��*/
auto            int     priznpovt;      /*�ਧ�. �த���. ���᪠*/
auto   unsigned char    *simb;          /*㪠��⥫� �� �������� ᨬ���*/
auto            int     cmd;            /*������� - १-� getkey*/
auto            int     povtlstr;       /*�ਧ�.����� ���᪠ �� ��ப��*/
	 unsigned char    *ukch;        /*�-�� �� ⥪��. �ࠢ�. ᨬ���*/
	 unsigned char    *ukch1;       /*�-�� �� ⥪��. �ࠢ�. ᨬ���*/
	 struct cmdkey  *uk;            /*�-�� �� ⥪�騩 �-� stringcom*/
/*==========================================================================*/

str_[0]='\0'; /* Not in INDIS-2 !!!*/
priznpovt=1;
for(simb=str_;priznpovt;simb++) {
    do{
      alarmed=0;
#ifdef ALRM_KB
      if(read(0, simb, 1) <= 0 && alarmed==0) exit(-1);
#else
      if(read(0, simb, 1) <= 0 && !alarmed) exit(-1);
#endif
    }while(alarmed && simb!=str_);

    if(alarmed) return(ALARMED);
    if(!*simb) return(ERROR_KEY);

  *(simb+1)='\0';

  /*�.�. ��᫥����⥫쭮��� �� stringcom; � �� �⭮����� CR � �.�.*/
  povtlstr=1;
  for(uk=stringcom_;uk->command && povtlstr;uk++){/*���� �� 蠡����� ������*/
    if(uk->string)
      { /*��᫥�-�� �� stringcom*/
	for(ukch1=str_,ukch=(uk->string);
	    (*ukch==*ukch1) && *ukch;
	    ukch++,ukch1++);
	if(*ukch1=='\0') { /*ᮢ������� ��� ���� ᨬ�����*/
	    if(*ukch=='\0')
	      { /*������ ᮢ�������*/
		cmd=uk->command;
		priznpovt=povtlstr=0;
	      }else{ /*���筮� ᮢ�������*/
		povtlstr=0;
	    } /* (*ukch=='\0') */
	}  /* *ukch1=='\0'*/
    } /*(uk->string!=0) */
  } /*�� ��������*/
  if(povtlstr) { /*��ᮢ�������*/
      if(my_isprint(str_[0]) && str_[1]=='\0')
	{ /*������� ᨬ��� */
	  cmd=PECH_SIMB|str_[0];
	}else{
	  cmd=ERROR_KEY|str_[0];
      }
      priznpovt=0;
  }
}
return(cmd);
}

#else /* MS-DOS */

#include <ctype.h>
#include <stdio.h>
#include "../includes/keys.h"
#include <bios.h>
/****************************************************************************/
		int getkey_()
/*      ���� ���� � ���������� � ������ӿ� �� ����� �
		��껿�������
����� ��� � �
���� ꫺��ӿ� 28.08.92.
*****************************************************************************/
{
extern struct   cmdkey  stringcom_[];    /*ꫫ���굑�� ����� ������ _bios_keybrd (_NKEYBRD_READ) */

register struct cmdkey  *uk;            /*��-�� Ӏ �זּ��� ��-� stringcom*/
	union
	      {
		struct
		  {
		    unsigned char ascii;
		    unsigned char scan;
		  } keycode;
		unsigned scan_code;
	      }    key;
static  unsigned int    savecode;       /*��� ������� ��� ������Ӿ� ��������*/
auto            int     cmd;            /*����ӟ� - ��-� getkey*/
/*==========================================================================*/
if(stringcom_ && stringcom_->command) {
     key.scan_code=_bios_keybrd (_NKEYBRD_READ);

     for(uk=stringcom_;uk->command && key.scan_code!=uk->code; uk++);
     if(key.scan_code==uk->code)
       { /*Ӏڟ�Ӏ ������� �� stringcom_*/
	 cmd=uk->command;
       }else{ /*���������� ������*/
	 if(key.keycode.ascii>=' ' && key.keycode.ascii!=0177)
	   {
	     cmd=PECH_SIMB | key.keycode.ascii;
	   }else{
	     cmd=ERROR_KEY;
	 }
     }
}
return(cmd);

}

#endif
