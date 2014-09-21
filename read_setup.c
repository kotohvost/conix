#include <unistd.h>
#include <stdlib.h>
#include "defs.h"
/****************************************************************************/
				read_setup()
/*                      Чтение настроек из файла $home/.conix.ini
/****************************************************************************/
{
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
	short   len;
static  char    name[]=".conix.ini";
	FILE    *setup;         /* файл, из которого считываются настройки */
	int     error=0;        /* признак ошибки*/
	short   swaps=1;        /* для проверки и установки признака переворота байтов */
/*==========================================================================*/
if(chdir(panels[cpanel].full_dir) || access(name, R_OK))
  { /* в текущем каталоге нет доступного для чтения файла настроек */
    home=getenv("HOME");
    if(!home) home=getenv("home");
  }else{
    home=".";
  }
if((!home) || chdir(home))
  { /* нет HOME или недоступен домашний каталог */
  }else{ /* можно пробовать записать */
    if(!(setup=fopen(name, "r")))
      { /* не открывается файл */
      }else{
	if(ferror(setup) || feof(setup)) error=1;
	if(!error) fread(&len, sizeof(len), 1, setup);
	if(len==swaps)
	  {
	    swaps=0; /* байты переставлять не нужно */
	  }else{
	    swap_bin(&len, sizeof(len));
	    if(len!=swaps) error=1; /* и перевернутые данные не соответствуют */
	  }

	if(ferror(setup) || feof(setup)) error=1;
	if(!error) fread(&len, sizeof(len), 1, setup);
	if(swaps) swap_bin(&len, sizeof(len));
	cpanel=len;
	for(outpanel=0; outpanel<NPANELS; outpanel++){
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&panels[outpanel].on, sizeof(panels[outpanel].on), 1, setup);
	  if(swaps) swap_bin(&panels[outpanel].on, sizeof(panels[outpanel].on));
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&len, sizeof(len), 1, setup);
	  if(swaps) swap_bin(&len, sizeof(len));
	  if(ferror(setup) || feof(setup)) error=1;
	  if(outpanel==cpanel)
	    { /* текущая панель всегда в текущем каталоге */
	      if(!error) fseek(setup, (long)len, 1);
	      if(ferror(setup) || feof(setup)) error=1;
	    }else{
	      if((!error) && len>panels[outpanel].full_dir_size){
		free(panels[outpanel].full_dir);
		panels[outpanel].full_dir_size=(len/128+1)*128;
		panels[outpanel].full_dir=malloc(panels[outpanel].full_dir_size);
		if(!panels[outpanel].full_dir) error=1;
	      }
	      if(!error) fread(panels[outpanel].full_dir, len, 1, setup);
	      if(ferror(setup) || feof(setup)) error=1;
	    }
	  if(!error) fread(&panels[outpanel].sort, sizeof(panels[outpanel].sort), 1, setup);
	  if(swaps) swap_bin(&panels[outpanel].sort, sizeof(panels[outpanel].sort));
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(panels[outpanel].outfields, sizeof(panels[outpanel].outfields), 1, setup);
	  if(swaps) {
	    for(len=0;len<sizeof(panels[outpanel].outfields)/sizeof(panels[outpanel].outfields[0]);len++){
	      swap_bin(&panels[outpanel].outfields[len], sizeof(panels[outpanel].outfields[0]));
	    }
	  }
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(panels[outpanel].filter_read, sizeof(panels[outpanel].filter_read), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(panels[outpanel].filter_noread, sizeof(panels[outpanel].filter_noread), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;

	  if(!error) fread(&cmp_exist, sizeof(cmp_exist), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_time , sizeof(cmp_time ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_size , sizeof(cmp_size ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_mode , sizeof(cmp_mode ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_body , sizeof(cmp_body ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_nlink, sizeof(cmp_nlink), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;

	  if(!error) fread(&prompt   , sizeof(prompt   ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(swaps) swap_bin(&prompt, sizeof(prompt));

	  if(!error) fread(&clock_need, sizeof(clock_need), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(swaps) swap_bin(&clock_need, sizeof(clock_need));
	}
	if(!error){
	    getc(setup);
	    if(!feof(setup)) error=1; /* файл испорчен или несовместимые версии */
	}
	fclose(setup);
	if(error){
	  unlink(name); /* файл записан некорректно */
	  endwin();
	  fprintf(stderr, "Uncompatible version of $HOME/%s or ./%s (unlinked), try once more\n", name, name);
	  exit(-1);
	}
      }
    delsoob();
  }
}
