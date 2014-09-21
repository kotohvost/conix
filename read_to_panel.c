#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "defs.h"
/****************************************************************************/
			read_to_panel()
/*              Чтение текущего каталога и вывод
*****************************************************************************/
{
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  panel   panels[NPANELS];
extern struct   win     win54;  /* сообщение о невозможности открыть каталог*/
	struct dirent *dp;
	DIR     *dirp;
	struct stat stbuf;
	int     nnames_end=0;
	int     nfile=0;        /* номер текущего файла*/
	int     lastfile;
	int     refreshed=0;    /* признак того, что список файлов выведен*/
	int     i;
	int     readed;         /* признак чтения файла в результате фильтрования */
	char    *filter_read;   /* фильтр чтения */
	char    *filter_noread; /* фильтр нечтения */
	int     wasbeep;        /* признак того, что был звуковой сигнал о перераспределении памяти*/

extern  int     cmpitems();     /*функция сравнения х-к файлов для сортировки*/
extern  int     my_getcwd();    /* вызывает pwd и возвр 0-Ok, !0-мало места*/
/*==========================================================================*/
  filesys_inf(); /* информация по файловой системе */

  if(!(dirp = opendir("."))){
    beep();
    ask(&win54, 2, 0);
    chdir("/");
    if(!(dirp = opendir("."))){
      clear();
      refresh();
      nocbreak();
      echo();
      endwin();
      printf("Can not open dirs . and /\n");
      exit(-1);
    }
  }
  /* переносить сюда chdir() нельзя, т.к. panels[outpanel].full_dir
   * может быть некорректным!
   */
  /* определить текущий каталог (могут быть символические ссылки?)*/
  i=0;  /* примитивное средство от зацикливания и вылета по памяти */
  wasbeep=0;
  while(i++<10 && my_getcwd(panels[outpanel].full_dir,panels[outpanel].full_dir_size)==(-3)){
    free(panels[outpanel].full_dir);
    panels[outpanel].full_dir_size +=128;
    panels[outpanel].full_dir=malloc(panels[outpanel].full_dir_size);
    /* fprintf(stderr," Allocating memory %d\n",panels[outpanel].full_dir_size);getchar(); */
    if(!wasbeep){
      wasbeep++;
      /* beep(); */
    }
    tst_err_malloc(panels[outpanel].full_dir,5); /* выполняет exit, если NULL*/
  }
  if(strcmp(panels[outpanel].full_dir,"/")==0)
    { /* в корневом каталоге */
      panels[outpanel].curfile=2;
    }else{
      panels[outpanel].curfile=1;
    }
  panels[outpanel].firstfile=0;
  panels[outpanel].selected=0;
  panels[outpanel].size_sel=0L;

  lastfile=panels[outpanel].lines-panels[outpanel].lnused;

  filter_read= (panels[outpanel].filter_read[0]) ? panels[outpanel].filter_read : NULL;
  filter_noread= (panels[outpanel].filter_noread[0]) ? panels[outpanel].filter_noread : NULL;

  for(nfile=0; dp=readdir(dirp); ){
    stbuf.st_mode=0;
    lstat(dp->d_name,&stbuf);

    readed=0;
    if(nfile<=1 || filtype(stbuf.st_mode)==FTYPE_DIR)
      {/* каталоги никогда не фильтруются*/
	readed=1;
      }else{
	if(filter_read)
	  { /* есть фильтр чтения */
	    if(inregular(dp->d_name, filter_read)) readed=1;
	  }else{
	    readed=1;
	  }
	if(filter_noread)
	  { /* есть фильтр нечтения */
	    if(inregular(dp->d_name, filter_noread)) readed=0;
	  }
      }

    if(readed){

      if(nfile==panels[outpanel].nitems){
	panels[outpanel].nitems+=100;
	panels[outpanel].itembuf=realloc(panels[outpanel].itembuf,
				       panels[outpanel].nitems*sizeof(struct itembuf));
	tst_err_malloc(panels[outpanel].itembuf,6); /* выполняет exit, если NULL*/
	/* fprintf(stderr," Allocating items %d\n",panels[outpanel].nitems);getchar(); */
	if(!wasbeep){
	  wasbeep++;
	  /* beep(); */
	}
      }
      if(strlen(dp->d_name)+nnames_end >= panels[outpanel].names_len){

	panels[outpanel].names_len+=1024;
	panels[outpanel].names=realloc(panels[outpanel].names,
				       panels[outpanel].names_len);
	tst_err_malloc(panels[outpanel].names,7); /* выполняет exit, если NULL*/
	/* fprintf(stderr," Allocating names %d\n",panels[outpanel].names_len); getchar(); */
	if(!wasbeep){
	  wasbeep++;
	  /* beep(); */
	}
      }
      strcpy(panels[outpanel].names+nnames_end, dp->d_name);
      panels[outpanel].itembuf[nfile].nname=nnames_end;
      nnames_end+=strlen(dp->d_name)+1;

      panels[outpanel].itembuf[nfile].mode=stbuf.st_mode;
      panels[outpanel].itembuf[nfile].mtime=stbuf.st_mtime;
      panels[outpanel].itembuf[nfile].size=stbuf.st_size;
      panels[outpanel].itembuf[nfile].inum=stbuf.st_ino;
      panels[outpanel].itembuf[nfile].nlink=stbuf.st_nlink;
      panels[outpanel].itembuf[nfile].owner=stbuf.st_uid;
      panels[outpanel].itembuf[nfile].group=stbuf.st_gid;

      panels[outpanel].itembuf[nfile].attrs=0;
/* Зачем это было - не помню, но в Unsorted проявилось как bug
 *     if(nfile==lastfile && panels[outpanel].sort==0){
 *       refreshed=1;
 *       panels[outpanel].nfiles=nfile;
 *       refr_panel();
 *       wrefresh(panels[outpanel].win);
 *       if(panels[1-outpanel].on==2){
 *         show_inf();
 *         wrefresh(panels[1-outpanel].win);
 *       }
 *     }
 */
      nfile++;
    }
  }
  closedir(dirp);

  panels[outpanel].nfiles=nfile; /* количество файлов*/
  nfile--;                       /* номер последнего файла*/
  panels[outpanel].lastfile= ( nfile < lastfile ) ? nfile : lastfile;

  if(panels[outpanel].sort){ /* Сортировка списка файлов */
    qsort(panels[outpanel].itembuf+2,
    (size_t)panels[outpanel].nfiles-2,
    sizeof(struct itembuf),
    cmpitems);
  }


  if(!refreshed) refr_panel();
}
