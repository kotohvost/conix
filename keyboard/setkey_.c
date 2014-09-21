#include <stdio.h>
#include "keys.h"
/****************************************************************************/
		int     setkey_(inp)
/*      ����ன�� �� ���������� �ନ����.
	�᫨ �ணࠬ�� �� ����� ࠡ���� � ����� ��-
	�������, �㭪�� �����頥� �㫥��� ���.

����: ���⮢� �.�.
��� ᮧ�����: 26.06.90.
*****************************************************************************/
/* ��ࠬ���� */
		int     inp;            /*1 - ����ந�� �ନ���*/
					/*0 - ����� ࠡ��� */
					/*2 - �뤠�� ��᫥����⥫쭮��� ���樠����樨*/
{
#ifndef MS_DOS
/*UNIX-��⥬�*/

/* �室� */

static          char    buftc[1024];    /*���� �� TERMCAP*/
extern          int     nctrl_; /*������⢮ ������ � ����ﭭ�� ���  */
				/*(�ࠢ���騥 � �.�. �� ���� - � ���. */


/* �室/��室*/

extern  struct  cmdkey  stringcom_[];   /*������� � ᮮ�.�� ����*/
auto            int     kod=1;          /*��� ��室�*/

/* ��室 */

extern          int     uc_;            /*=1,�᫨ �� ��.⮫쪮 �����.�㪢�*/

/* ����७���*/

static          int     prsr;           /*=1, �᫨ ����祭 ����ୠ⨢�� ०��*/
auto    struct  cmdkey  *ukr;           /*㪠��⥫� �� ���ᨢ �������*/
auto    struct  cmdkey  *ukw;           /*����室���� �����*/
auto            char    *termname;      /*��� �ନ���� �� "�।�"*/
auto   unsigned char    buf[60];        /*���� ����� ��ப� � ᢮��⢥ �ନ����*/
auto   unsigned char    *area;          /*㪠��⥫� �� ���� �����*/

/* �ᯮ��㥬� �㭪樨 */

extern          char    *getenv();      /* - */
extern          char    *strcpy();      /* - */
extern          int     t_tgetflag();     /*termlib*/
extern          int     t_tgetent();      /*termlib*/
extern          char    *t_tgetstr();     /*termlib*/
extern          char    *malloc();

/*==========================================================================*/

switch(inp)
  {
    case 1:

	if((termname=getenv("TERM"))!=NULL)
	    { /*�⥭�� ���� �� TERMCAP */
		t_tgetent(buftc,termname);

		ukr=ukw=(&(stringcom_[nctrl_]));

		while(ukr->command!=0){
			area=buf;
			if(t_tgetstr(ukr->string,&area)!=0)
			    { /*�ॡ㥬�� ������ ���ᠭ� � TERMCAP*/
				if(ukw->string=(unsigned char*)malloc(area-buf))
				    { /*।���� �������� ��� ������*/
					strcpy(ukw->string,buf);
					ukw->command=ukr->command;
					ukw++;
				}
			}
			ukr++;  /*��३� � ᫥���饩 �����*/
			/*�᫨ ������ �㭪樮���쭮� ���������� �� ���ᠭ� �*/
			/*TERMCAP ��� ।���� �� ����� "���������" ��*/
			/*���, �ਤ���� ࠡ���� � �ࠢ���騬� �����蠬�,   */
			/*��।�����묨 �� 㬮�砭��*/
		}

		/*��䨪�஢��� ����� ���ᨢ� �������*/

		ukw->string=NULL;
		ukw->command=0;


		area=buf;
		if(t_tgetstr("ks",&area)!=0) { /*������� ����ୠ⨢�� ०��*/
			prsr=1;
			for(area=buf;*area!='\0';area++) putchar(*area);
		}

		/*��।. ����稥 �����. � �����. �㪢*/

		uc_=t_tgetflag("UC");

	    }else{
		kod=0;
	}

    break;

    case 0:
	if(prsr)
	    { /*�몫���� ����ୠ⨢�� ०��*/
		area=buf;
		t_tgetstr("ke",&area);
		for(area=buf;*area!='\0';area++) putchar(*area);
	}
    break;

    case 2: /* ��ॢ뤠�� ���樠���������� ��᫥����⥫쭮��� */
      area=buf;
      if(t_tgetstr("ks",&area)!=0) { /*������� ����ୠ⨢�� ०��*/
	      prsr=1;
	      for(area=buf;*area!='\0';area++) putchar(*area);
      }
      my_raw();
    break;

}

return(kod);

#else /* MS-DOS */
return(1);
#endif
}
