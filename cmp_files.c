#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "defs.h"
/****************************************************************************/
			cmp_files(path1, name1, path2, name2)
/*                      Сравнение содержимого файлов
*****************************************************************************/
	char    *path1; /* полный путь до первого файла */
	char    *name1; /* имя первого файла */
	char    *path2; /* полный путь до второго файла */
	char    *name2; /* имя второго файла */
{
extern  char    *tmp_str;       /* строка для поиска */
extern  size_t  tmp_len;        /* место, выделенное для tmp_str*/
extern  struct  win win60;      /* окно сообщения */
extern  struct  win win61;      /* сообщение об ошибке сравнения */
extern  struct  win     win3;
extern  struct  panel   panels[];
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
extern  char    *tmp_str;       /* строка для временных данных */
extern  char    *inp_str;       /* строка для ввода команд */
extern  int     silent;         /* признак сравнения без сообщения об ошибках*/
	int     panel;  /*обрабатываемая панель */
	int     len;    /* длина выводимой части имени */
	int     len2;
	char    str1[30]; /* имя первого файла (для вывода)*/
	char    str2[30]; /* имя второго файла (для вывода)*/
	pid_t   pid;            /* номер процесса для ожидания */
	char    *ptr1;          /* для фолрмирования аргумента cmp */
	char    *ptr2;          /* для фолрмирования аргумента cmp */
	int     status=1;       /* состояние завершения процесса*/
	int     i;
/*==========================================================================*/
len=strlen(name1);
if(len>=sizeof(str1))
  { /* локальное имя не помещается в строку*/
    strcpy(str1, name1+(len-sizeof(str1)));
  }else{  /*локальное - помещается*/
    len2=strlen(path1);
    if(len+len2+1>=sizeof(str1))
      { /* путь с именем не помещается */
	strcpy(str1, "...");
	strcat(str1, path1+(len+len2+1+3-sizeof(str1)));
      }else{ /* все помещается */
	strcpy(str1, path1);
      }
    strcat(str1, "/");
    strcat(str1, name1);
  }

/* Делать цикл по массивам указателей? Лень! Повторяем кусочек*/
len=strlen(name2);
if(len>=sizeof(str2))
  { /* локальное имя не помещается в строку*/
    strcpy(str2, name2+(len-sizeof(str2)));
  }else{  /*локальное - помещается*/
    len2=strlen(path2);
    if(len+len2+1>=sizeof(str2))
      { /* путь с именем не помещается */
	strcpy(str2, "...");
	strcat(str2, path2+(len+len2+1+3-sizeof(str2)));
      }else{ /* все помещается */
	strcpy(str2, path2);
      }
    strcat(str2, "/");
    strcat(str2, name2);
  }

win60.txts[1].x=win60.txts[3].x=(-1); /* soob() выровняет */
win60.txts[1].txt=str1;
win60.txts[3].txt=str2;

soob(&win60);

switch(pid=fork())
  {
    case 0:  /* потомок */
      /* сформировать имена файлов */
      /* на всякий случай освободим всю память, какую можно*/
      for(panel=0; panel<NPANELS; panel++){
	  delwin(panels[panel].win);
	  /* free(panels[panel].full_dir); - нельзя, т.к. path* ... */
	  free(panels[panel].itembuf);
	  /* free(panels[panel].names);    - нельзя, т.к. name* ... */
      }

      delwin(stdscr);
      delwin(save_scr);
      free(tmp_str);
      free(inp_str);

      ptr1=malloc(strlen(path1)+strlen(name1)+1+1);
      tst_err_malloc(ptr1, 22);
      strcpy(ptr1, path1);
      strcat(ptr1, "/");
      strcat(ptr1, name1);

      ptr2=malloc(strlen(path2)+strlen(name2)+1+1);
      tst_err_malloc(ptr2, 23);
      strcpy(ptr2, path2);
      strcat(ptr2, "/");
      strcat(ptr2, name2);

      for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
      execlp("cmp", "cmp", "-s", ptr1, ptr2, NULL);
      perror("Can't execute 'cmp'");
      exit(-1);
    break;

    case -1: /* не создается процесс */
      ask(&win3,1,0); /* сообщение об ошибке*/
    break;

    default: /* процесс - родитель */
      while(wait(&status)!=pid);
    break;
  }

if(status && status!=(1<<8) && silent==0)
  { /* cmp не смогла прочитать файлы */
    win61.txts[1].x=win61.txts[3].x=(-1); /* ask() выровняет */
    win61.txts[1].txt=str1;
    win61.txts[3].txt=str2;
    switch(ask(&win61, 4, HOR))
      {
	case 5: /* Silent */
	  silent=1;
	  status=2;
	break;
	case 6: /* Cancel */
	  status=3;
	break;
	default: /* Esc, Ok ... */
	  status=2;
	break;
      }

  }else{
    switch(status)
      {
	case 0: /* Ok */
	  /* status=0; */
	break;
	case 1<<8: /* различные файлы */
	  status=1;
	break;
	default: /* код выхода 2 или ошибки несовместимости */
	  status=2;
	break;
      }
  }
return(status);
}
