#include <unistd.h>
#include <stdlib.h>
#include "defs.h"
/****************************************************************************/
				ext_action(name, config)
/*                      Выполнение действий в соответствии с расширением
			Если действие выполнено, то возвращается 0,
			иначе - не 0
*****************************************************************************/
	char    *name;          /* имя обрабатываемого файла*/
	char    *config;        /* файл конфигурации */
{
extern  char    *tmp_str;       /* строка для поиска */
extern  size_t  tmp_len;        /* место, выделенное для tmp_str*/
static  char    *fullname=NULL; /* полное имя файла конфигурации*/
static  size_t  fullnamelen=0;  /* размер памяти, выделенной для fullname*/
	char    *home;          /* домашний каталог пользователя */
	FILE    *f;             /* для чтения файла конфигурации */
	int     i;
	int     cmd_i;
	int     find;
	int     ch;
	int     nch;
/*==========================================================================*/
if(access(config, R_OK) != 0){ /* в текущем каталоге нет доступного для чтения файла настроек */
  home=getenv("HOME");
  if(!home) home=getenv("home");
  if(home)
    {
      if((strlen(home) + strlen(name) +2) > fullnamelen){
	if(fullname) free(fullname);
	fullnamelen=strlen(home) + strlen(config) +2;
	fullname = malloc(fullnamelen);
	tst_err_malloc(fullname, 28);
      }
      strcpy(fullname, home  );
      strcat(fullname, "/"   );
      strcat(fullname, config);
      if(access(fullname, R_OK) == 0)
	{ /* найден файл конфигурации */
	  config=fullname;
	}else{
	  config=NULL;
	}
    }else{
	  config=NULL;
    }
}

if(config) { /* есть файл конфигурации и пока выполняются другие условия работы*/
  if(!(f=fopen(config, "r")))
    {
      config=NULL;
    }else{
      find=0;
      while(find==0 && feof(f)==0){
	fgets(tmp_str, tmp_len, f);
	if(!feof(f)){
	  for(i=0; i!=tmp_len-1 && !isspace(tmp_str[i]); i++);
	  if(i && isspace(tmp_str[i])){ /*что-то найдено */
	    tmp_str[i] = '\0';
	    if(inregular(name, tmp_str)) find++;
	  }
	}
      }
      if(find)
	{ /* найден шаблон */
	  i++; /* tmp_str[i] был установлен на \0 */
	  while(i!=tmp_len-2 && isspace(tmp_str[i])) i++;
	  cmd_i = i; /* здесь начинается команда */
	  while(i!=tmp_len-2 && tmp_str[i]!='\n') i++;
	  if(i == tmp_len-2) beep();

	  if(i+strlen(name)+2 > tmp_len){
	      tmp_len += strlen(name)+2;
	      tmp_str=realloc(tmp_str, tmp_len);
	      tst_err_malloc(tmp_str,29); /* выполняет exit, если NULL*/
	  }

	  tmp_str[i++] = ' ';

	  strcpy(&tmp_str[i], name);

	}else{
	  config=NULL;
	}
      fclose(f);
    } /* if(fopen...) */
}
if(config)
  {
    /* выполнение команды */
    execcmd(&tmp_str[cmd_i]);
    return(0);
  }else{
    return(-1);
  }
}
