#include <curses.h>
#include "keys.h"

#ifndef MS_DOS
/* UNIX - системы */
/****************************************************************************/
		int getkey_()
/*      ввод кода с клавиатуры и определение их типов с
		расшифровкой
Автор Ревтов С И
Дата создания 26.06.90.
*****************************************************************************/
{
extern struct   cmdkey  stringcom_[];   /* */
#ifdef ALRM_KB
extern int      alarmed;                /* признак того, что было прерывание по alarm()*/
#else
       int      alarmed=0;              /* признак того, что было прерывание по alarm()*/
#endif
       unsigned char    str_[256];      /*строка-ответ п-ля*/
auto            int     priznpovt;      /*призн. продолж. поиска*/
auto   unsigned char    *simb;          /*указатель на введенный символ*/
auto            int     cmd;            /*команда - рез-т getkey*/
auto            int     povtlstr;       /*призн.повтора поиска по строкам*/
	 unsigned char    *ukch;        /*ук-ль на текущ. сравн. символ*/
	 unsigned char    *ukch1;       /*ук-ль на текущ. сравн. символ*/
	 struct cmdkey  *uk;            /*ук-ль на текущий эл-т stringcom*/
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

  /*м.б. последовательность из stringcom; сюда же относятся CR и т.п.*/
  povtlstr=1;
  for(uk=stringcom_;uk->command && povtlstr;uk++){/*поиск по шаблонам команд*/
    if(uk->string)
      { /*послед-ть из stringcom*/
	for(ukch1=str_,ukch=(uk->string);
	    (*ukch==*ukch1) && *ukch;
	    ukch++,ukch1++);
	if(*ukch1=='\0') { /*совпадение или мало символов*/
	    if(*ukch=='\0')
	      { /*полное совпадение*/
		cmd=uk->command;
		priznpovt=povtlstr=0;
	      }else{ /*частичное совпадение*/
		povtlstr=0;
	    } /* (*ukch=='\0') */
	}  /* *ukch1=='\0'*/
    } /*(uk->string!=0) */
  } /*по командам*/
  if(povtlstr) { /*несовпадение*/
      if(my_isprint(str_[0]) && str_[1]=='\0')
	{ /*одиночный символ */
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
/*      ССлЯ млЯА ъ м╜АС┐А╡╢│╛ ┐ лп│яЯя╜я╙┐я ┐╕ ╡┐плС ъ
		│Аъ╗┐╖│лСмл┌
гС╡л│ ияС╡лС Ф Й
ДА╡А ъл║ЯА╙┐┬ 28.08.92.
*****************************************************************************/
{
extern struct   cmdkey  stringcom_[];    /*ълл╡Ся╡ъ╡С┐я млЯлС м╜АС┐╗ _bios_keybrd (_NKEYBRD_READ) */

register struct cmdkey  *uk;            /*╢м-╜┐ ╙А ╡ям╢╝┐┌ └╜-╡ stringcom*/
	union
	      {
		struct
		  {
		    unsigned char ascii;
		    unsigned char scan;
		  } keycode;
		unsigned scan_code;
	      }    key;
static  unsigned int    savecode;       /*млЯ м╜АС┐╗┐ Я╜┬ плС╡л│╙╛╕ │А║Мл│лС*/
auto            int     cmd;            /*мл╝А╙ЯА - │я║-╡ getkey*/
/*==========================================================================*/
if(stringcom_ && stringcom_->command) {
     key.scan_code=_bios_keybrd (_NKEYBRD_READ);

     for(uk=stringcom_;uk->command && key.scan_code!=uk->code; uk++);
     if(key.scan_code==uk->code)
       { /*╙А┌Яя╙А м╜АС┐╗А ┐║ stringcom_*/
	 cmd=uk->command;
       }else{ /*п│лЯл╜к┐╡┐ │А║Мл│*/
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
