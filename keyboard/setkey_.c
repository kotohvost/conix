#include <stdio.h>
#include "keys.h"
/****************************************************************************/
		int     setkey_(inp)
/*      настройка на клавиатуру терминала.
	Если программа не может работать с данным тер-
	миналом, функция возвращает нулевой код.

Автор: Ревтова С.В.
Дата создания: 26.06.90.
*****************************************************************************/
/* Параметры */
		int     inp;            /*1 - настроить терминал*/
					/*0 - конец работы */
					/*2 - выдать последовательность инициализации*/
{
#ifndef MS_DOS
/*UNIX-системы*/

/* Входы */

static          char    buftc[1024];    /*коды из TERMCAP*/
extern          int     nctrl_; /*количество команд в постоянной части  */
				/*(управляющие и т.п. Эти коды - в нач. */


/* Вход/выход*/

extern  struct  cmdkey  stringcom_[];   /*команды и соотв.им коды*/
auto            int     kod=1;          /*код выхода*/

/* Выход */

extern          int     uc_;            /*=1,если на терм.только мален.буквы*/

/* Внутренние*/

static          int     prsr;           /*=1, если включен альтернативный режим*/
auto    struct  cmdkey  *ukr;           /*указатели на массив структур*/
auto    struct  cmdkey  *ukw;           /*необходимых кодов*/
auto            char    *termname;      /*имя терминала из "среды"*/
auto   unsigned char    buf[60];        /*буфер ввода строки о свойстве терминала*/
auto   unsigned char    *area;          /*указатель на буфер ввода*/

/* Используемые функции */

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
	    { /*чтение кода из TERMCAP */
		t_tgetent(buftc,termname);

		ukr=ukw=(&(stringcom_[nctrl_]));

		while(ukr->command!=0){
			area=buf;
			if(t_tgetstr(ukr->string,&area)!=0)
			    { /*требуемая клавиша описана в TERMCAP*/
				if(ukw->string=(unsigned char*)malloc(area-buf))
				    { /*редактор запомнил код клавиши*/
					strcpy(ukw->string,buf);
					ukw->command=ukr->command;
					ukw++;
				}
			}
			ukr++;  /*перейти к следующей записи*/
			/*если клавиши функциональной клавиатуры не описаны в*/
			/*TERMCAP или редактор не может "запомнить" их*/
			/*код, придется работать с управляющими клавишами,   */
			/*определенными по умолчанию*/
		}

		/*зафиксировать конец массива структур*/

		ukw->string=NULL;
		ukw->command=0;


		area=buf;
		if(t_tgetstr("ks",&area)!=0) { /*включить альтернативный режим*/
			prsr=1;
			for(area=buf;*area!='\0';area++) putchar(*area);
		}

		/*опред. наличие больш. и мален. букв*/

		uc_=t_tgetflag("UC");

	    }else{
		kod=0;
	}

    break;

    case 0:
	if(prsr)
	    { /*выключить альтернативный режим*/
		area=buf;
		t_tgetstr("ke",&area);
		for(area=buf;*area!='\0';area++) putchar(*area);
	}
    break;

    case 2: /* перевыдать инициализирующую последовательность */
      area=buf;
      if(t_tgetstr("ks",&area)!=0) { /*включить альтернативный режим*/
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
