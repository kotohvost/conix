#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include "defs.h"
/****************************************************************************/
			execcmd(cmd)
/*                      �믮������ �������
/****************************************************************************/
	char    *cmd;           /* ������� */
{
extern  char    *history;       /* ���� �࠭���� ���ਨ */
extern  size_t  his_len;        /* ����, �뤥������ ��� history*/
extern  int     his_cur;        /* ����� ����� history ��� ���������� */
extern  int     his_go;         /* ����� ����� history */
extern  struct  win     win1;   /* ���ᠭ�� ���� ���⢥ত���� ��室�*/
extern  struct  win     win76;  /* ᮮ�饭�� � ࠡ�� ���譥�� ��ᬮ��騪� */
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  int     use_keycap;     /* �ਧ��� �ᯮ�짮����� ᢮�� ����ன�� �� ���������� */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  int     in_viewer;      /* �ਧ��� �맮�� execcmd() � ०��� ��ᬮ�� */
static  char    *shell=NULL;    /* ⥪�騩 �������� ������ */
extern  int     nosave_ask;     /* �ਧ��� "����࠭����" �࠭� ��� ask */
extern  WINDOW  *soob_win;      /* ���� �뤠� ᮮ�饭�� */
	pid_t   pid;            /* ����� ����� ��� �������� */
	pid_t   pid1;           /* ���祭��, �����頥��� wait() */
	int     i;
	chtype  ch;
	char    *ptr;
	int     stop=0;
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
	int     save;           /* �ਧ��� ��࠭���� ������� � ���ਨ*/
	int     old_nosave_ask;
	int     cantchdir=1;
	DIR     *dirp;          /* ��� �஢�ન �⠥���� ��⠫��� */
	struct dirent *dp;      /* �������筮 */
/*==========================================================================*/

if(cmd[0]=='c' && cmd[1]=='d' &&
  (cmd[2]==' ' || cmd[2]=='\0'))
  { /* ������� cd ��ࠡ��뢠�� ᠬ����⥫쭮 */
    for(i=2;cmd[i]==' ';i++);
    if(cmd[i])
      { /* ����� ��⠫�� */

	cantchdir = 1;  /* ⥯��� �� �ਧ��� �����室� � ��⠫��*/

	if(access(cmd+i, X_OK|R_OK)==0) /* ��⠫��, ����㯭� ��� ���室� � �⥭��*/
	  cantchdir = 0;

	/* �஢�ઠ �⠥���� ��⠫��� �㦭� ��� ⮣�, �⮡� �� ������ root � ���⠥�� (NFS) ��⠫��� */
	if(cantchdir==0){
	  if(dirp = opendir(cmd+i))
	    {
	      if ( (dp=readdir(dirp))==NULL ) cantchdir++;
	      if ( cantchdir || (dp=readdir(dirp))==NULL ) cantchdir++;
	      closedir(dirp);
	      if(cantchdir) { beep(); beep(); }
	    }else{
	      cantchdir++;
	    }
	}
	if ( cantchdir || chdir( cmd+i )!=0) cantchdir++;

	if ( cantchdir ) beep();
      }else{ /* ���⮩ ��㬥�� - ���室 � $HOME */
	if(!(ptr=getenv("HOME")))
	  { /* ��� ��६����� HOME */
	    beep();
	  }else{
	    if(chdir(ptr)) beep();
	  }
      }
    outpanel=cpanel;
    read_to_panel();  /* extern outpanel */

  }else if(strcmp(cmd, "exit")==0){
    /* ������� exit ��ࠡ��뢠�� ᠬ����⥫쭮 */
    if(ask(&win1,0,HOR)==0) stop++;
  }else{  /* ���筠� ������� */
    overwrite(curscr, save_scr);
    if(in_viewer)
      { /* ������� ᮮ�饭�� �� ��ࠡ�⪥ */
	old_nosave_ask=nosave_ask;
	nosave_ask=1;
	soob(&win76);
	delwin(soob_win);
	soob_win=NULL;
	nosave_ask=old_nosave_ask;
      }else{ /* � ०��� ��ᬮ�� �� ������ �룫拉�� ��� ���஥��� ��ᬮ��騥*/
	clear();
	refresh();
      }
    overwrite(save_scr, stdscr);
#ifdef FREEBSD
    touchwin(stdscr);
#endif
    endwin();

    if(!in_viewer){ /* � ०��� ��ᬮ�� �� ������ �룫拉�� ��� ���஥��� ��ᬮ��騥*/
      /* for(i=COLS;i--;) putc('\n',stderr);*/
      fprintf(stderr,"%s\n", cmd);
    }
    if(!shell) shell=getenv("SHELL");
    if(!shell) shell="sh";

    switch(pid=fork())
      {
	case 0:  /* ��⮬�� */
	  for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
	  execlp(shell, shell, "-c", cmd, NULL);
	  perror("Can't execute 'shell'");
	  exit(-1);
	break;

	case -1:  /* �� ᮧ������ ����� */
	  perror("Can't fork");
	break;

	default:
	  do{
	   pid1=wait(NULL);
	  }while(pid1!=pid && pid1!=(-1));
	  /* ��� ��ࠡ�⪠ - ��᫥ switch()*/
	break;
      }

    if(!in_viewer){ /* � ०��� ��ᬮ�� �� ������ �룫拉�� ��� ���஥��� ��ᬮ��騪*/
      fprintf(stderr,"\r\nPress ENTER to continue...");
    }
    for(outpanel=0;outpanel<NPANELS;outpanel++){
      nfile=panels[outpanel].curfile;
      nfirst=panels[outpanel].firstfile;
      chdir(panels[outpanel].full_dir);
      read_to_panel();  /* extern outpanel */
      if(nfile<panels[outpanel].nfiles) {
	panels[outpanel].firstfile=nfirst;
	panels[outpanel].curfile=nfile;
	refr_panel();
      }
    }
    if(!in_viewer){ /* � ०��� ��ᬮ�� �� ������ �룫拉�� ��� ���஥��� ��ᬮ��騥*/
      do{
	ch=getchar();
      }while(ch!='\r' && ch!='\n');
    }

    doupdate();
    refresh();
    if(use_keycap) setkey_(2); /* ���樠������ (�१ ESC)*/
  } /* if ( == "cd ...") else */

if(!in_viewer){ /* � ०��� ��ᬮ�� �� ������ �룫拉�� ��� ���஥��� ��ᬮ��騥*/
  /* ������ ������� � ����� */
  save=1;
  his_go=hispred(his_cur);
  if(his_go!=his_cur && strcmp(cmd, &history[his_go])==0) {
    /* ⥪��� ������� �� �⫨砥��� �� �।��饩 */
    save=0; /* �� �����뢠�� */
  }

  if(save){ /* ������� ������� � ����� */
    his_go=strlen(cmd);
    if(his_go>his_len-his_cur-1){ /* ������� �� ����頥��� � ����� ����*/
      while(his_cur<his_len-1) history[his_cur++]=1;
      his_cur=0;
    }
    strcpy(history+his_cur, cmd);
    his_cur+=his_go+1;
  }
  his_go=his_cur;
}

for(outpanel=0;outpanel<NPANELS;outpanel++){
  if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
}
return(stop);
}
