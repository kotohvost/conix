#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "defs.h"
/****************************************************************************/
			read_to_panel()
/*              �⥭�� ⥪�饣� ��⠫��� � �뢮�
*****************************************************************************/
{
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  panel   panels[NPANELS];
extern struct   win     win54;  /* ᮮ�饭�� � ������������ ������ ��⠫��*/
	struct dirent *dp;
	DIR     *dirp;
	struct stat stbuf;
	int     nnames_end=0;
	int     nfile=0;        /* ����� ⥪�饣� 䠩��*/
	int     lastfile;
	int     refreshed=0;    /* �ਧ��� ⮣�, �� ᯨ᮪ 䠩��� �뢥���*/
	int     i;
	int     readed;         /* �ਧ��� �⥭�� 䠩�� � १���� 䨫��஢���� */
	char    *filter_read;   /* 䨫��� �⥭�� */
	char    *filter_noread; /* 䨫��� ���⥭�� */
	int     wasbeep;        /* �ਧ��� ⮣�, �� �� ��㪮��� ᨣ��� � �����।������ �����*/

extern  int     cmpitems();     /*�㭪�� �ࠢ����� �-� 䠩��� ��� ���஢��*/
extern  int     my_getcwd();    /* ��뢠�� pwd � ����� 0-Ok, !0-���� ����*/
/*==========================================================================*/
  filesys_inf(); /* ���ଠ�� �� 䠩����� ��⥬� */

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
  /* ��७���� � chdir() �����, �.�. panels[outpanel].full_dir
   * ����� ���� �����४��!
   */
  /* ��।����� ⥪�騩 ��⠫�� (����� ���� ᨬ�����᪨� ��뫪�?)*/
  i=0;  /* �ਬ�⨢��� �।�⢮ �� ��横������� � �뫥� �� ����� */
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
    tst_err_malloc(panels[outpanel].full_dir,5); /* �믮���� exit, �᫨ NULL*/
  }
  if(strcmp(panels[outpanel].full_dir,"/")==0)
    { /* � ��୥��� ��⠫��� */
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
      {/* ��⠫��� ������� �� 䨫��������*/
	readed=1;
      }else{
	if(filter_read)
	  { /* ���� 䨫��� �⥭�� */
	    if(inregular(dp->d_name, filter_read)) readed=1;
	  }else{
	    readed=1;
	  }
	if(filter_noread)
	  { /* ���� 䨫��� ���⥭�� */
	    if(inregular(dp->d_name, filter_noread)) readed=0;
	  }
      }

    if(readed){

      if(nfile==panels[outpanel].nitems){
	panels[outpanel].nitems+=100;
	panels[outpanel].itembuf=realloc(panels[outpanel].itembuf,
				       panels[outpanel].nitems*sizeof(struct itembuf));
	tst_err_malloc(panels[outpanel].itembuf,6); /* �믮���� exit, �᫨ NULL*/
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
	tst_err_malloc(panels[outpanel].names,7); /* �믮���� exit, �᫨ NULL*/
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
/* ��祬 �� �뫮 - �� �����, �� � Unsorted ������ ��� bug
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

  panels[outpanel].nfiles=nfile; /* ������⢮ 䠩���*/
  nfile--;                       /* ����� ��᫥����� 䠩��*/
  panels[outpanel].lastfile= ( nfile < lastfile ) ? nfile : lastfile;

  if(panels[outpanel].sort){ /* ����஢�� ᯨ᪠ 䠩��� */
    qsort(panels[outpanel].itembuf+2,
    (size_t)panels[outpanel].nfiles-2,
    sizeof(struct itembuf),
    cmpitems);
  }


  if(!refreshed) refr_panel();
}
