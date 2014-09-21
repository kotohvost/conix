#include <stdlib.h>
#include "defs.h"
/****************************************************************************/
				save_setup()
/*                      Запись настроек в файл $home/.conix.ini
 *                      или ./.conix.ini
/****************************************************************************/
{
extern  struct  win     win51;  /* сообщение о записи */
extern  struct  win     win52;  /* сообщение об ошибке перехода в $home*/
extern  struct  win     win53;  /* сообщение об ошибке записи в файл */
extern  struct  win     win58;  /* запрос о месте сохранения setup */
extern struct panel panels[];
extern  int     cpanel;         /* текущая панель*/
extern  int     outpanel;       /* выводимая панель */

extern  char    cmp_exist;      /* признак сравнения по наличию */
extern  char    cmp_time;       /* признак сравнения по времени */
extern  char    cmp_size;       /* признак сравнения по размеру */
extern  char    cmp_mode;       /* признак сравнения по доступу */
extern  char    cmp_body;       /* признак сравнения по содержимому */
extern  char    cmp_nlink;      /* признак сравнения по кол-ву ссылок */

extern  int     prompt;         /* тип приглашения:0-нет, 1-NodeName */
extern  int     clock_need;     /* признак вывода времени 0-нет 1-да */

	char    *home;          /* "домашний" каталог пользователя*/
	char    str[80];        /* строка для имени */
	short   len;
static  char    name[]=".conix.ini";
	FILE    *setup;         /* файл, в котором сохраняются настройки */
	int     error=0;        /* признак ошибки*/
	short   swaps=1;        /* для проверки и установки признака переворота байтов */
/*==========================================================================*/
switch(ask(&win58, 2, HOR|VERT))
  {
    case 2: /* в домашнем каталоге */
      home=getenv("HOME");
      if(!home) home=getenv("home");
    break;
    case 3: /* в текущем каталоге */
      home=".";
      if(chdir(panels[cpanel].full_dir)) home=NULL;
    break;
    default:
      error=1;
    break;
  }
if(!error){ /* в данном месте error-признак сохранения (п-ль мог отказаться)*/
  if((!home) || chdir(home))
    { /* нет HOME или недоступен домашний каталог */
      ask(&win52, 2, 0);
    }else{ /* можно пробовать записать */
      /* сформировать имя */
      len=strlen(home);
      if(len+sizeof(name)+1>=win51.cols)
	{ /* полное имя не помещается */
	  strcpy(str, "...");
	  strcat(str, home+(len+sizeof(name)+1-win51.cols)+3);
	  strcat(str, "/");
	  strcat(str, name);
	}else{
	  strcpy(str, home);
	  strcat(str, "/");
	  strcat(str, name);
	}
      win51.txts[1].txt=str;
      win51.txts[1].x=(win51.cols-strlen(str))/2;
      win53.txts[1].txt=str;
      win53.txts[1].x=(win53.cols-strlen(str))/2;
      soob(&win51);
      if(!(setup=fopen(name, "w")))
	{ /* не открывается файл */
	  ask(&win53, 2, 0);
	}else{
	  if(!error) fwrite(&swaps, sizeof(swaps), 1, setup);
	  len=cpanel;
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fwrite(&len, sizeof(len), 1, setup);
	  for(outpanel=0; outpanel<NPANELS; outpanel++){
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&panels[outpanel].on, sizeof(panels[outpanel].on), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    len=strlen(panels[outpanel].full_dir)+1;
	    if(!error) fwrite(&len, sizeof(len), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].full_dir, len, 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&panels[outpanel].sort, sizeof(panels[outpanel].sort), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].outfields, sizeof(panels[outpanel].outfields), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].filter_read, sizeof(panels[outpanel].filter_read), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(panels[outpanel].filter_noread, sizeof(panels[outpanel].filter_noread), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;

	    if(!error) fwrite(&cmp_exist, sizeof(cmp_exist), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_time , sizeof(cmp_time ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_size , sizeof(cmp_size ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_mode , sizeof(cmp_mode ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_body , sizeof(cmp_body ), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	    if(!error) fwrite(&cmp_nlink, sizeof(cmp_nlink), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;

	    if(!error) fwrite(&prompt, sizeof(prompt), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;

	    if(!error) fwrite(&clock_need, sizeof(clock_need), 1, setup);
	    if(ferror(setup) || feof(setup)) error=1;
	  }
	  if(error){
	    unlink(name); /* файл записан некорректно */
	    ask(&win53, 2, 0);
	  }
	  fclose(setup);
	}
      delsoob();
    }
}
}
