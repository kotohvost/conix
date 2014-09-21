#include "defs.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
/****************************************************************************/
				copyfile()
/*                      ����஢���� 䠩��/䠩���
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  win     win3;
extern  struct  win     win15;  /* ����� �� ���⢥ত���� ���-� ��㯯� �.*/
extern  struct  win     win16;  /* ᮮ�饭�� �� �訡��*/
extern  struct  win     win17;  /* ᮮ�饭�� �� �訡�� ����㯠 � ��⠫��� */
extern  struct  win     win19;  /* ᮮ�饭�� � ����஢���� 䠩��(�)*/
extern  struct  win     win20;  /* ᮮ�饭�� � ����஢����*/
extern  struct  win     win21;  /* ᮮ�饭�� �� �訡�� ����஢����*/
extern  struct  win     win77;  /* ����� �� Overwrite */
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  char    *tmp_str;       /* ��ப� ��� �६����� ������ */
extern  size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
	char    str[80];
	char    *name;          /* ��� 䠩�� */
	pid_t   pid;
	pid_t   pid1;
	pid_t   pid_ready;
	int     wait_stat;      /* ����� �����襭�� ����� */
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
 struct itembuf *item_mark;     /* ��� ��ࠡ�⪨ �����⢥����� ��࠭���� 䠩��*/
	int     tested;         /* ������⢮ �஢�७��� 䠩���*/
	int     incl_dir;       /* �ਧ��� ������ ��⠫���� � ᯨ᪥*/
	int     fd[2];
	int     fd_err[2];      /* ��� ��७����祭�� ��⮪��� �訡��*/
	char    **spis;         /* ᯨ᮪ ��㬥�⮢ ��� �ணࠬ�� ����஢����*/
	char    **spis_p;       /* ᯨ᮪ ��㬥�⮢ ��� �ணࠬ�� ����஢����*/
	int     i;
	int     status;         /* ���ﭨ� �����襭�� �����*/
	int     copy=0;         /* �ਧ��� � ⨯ ����஢���� */
	int     panel;          /* "�� ⥪���" ������ */
	int     err=0;          /* �ਧ��� �訡�� ����஢���� */
	int     say_error=1;    /* ᮮ���� �� �訡��� */
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
	int     exists=0;       /* �ਧ��� �����-�� ����� Overwrite */
	char    *ptr;           /* ��� ��ࠧ������ ����� � ��⠫��� */
 struct stat    stbuf;
 struct inp_lst inp_lst[2];       /* ᯨ᮪ ����� ��� inp_menu()*/
 static char    *copyprog[]={"", "tar", "ln", "ln", "cp"}; /*�ணࠬ�� ����஢����*/
/*==========================================================================*/
inp_lst[0].str=tmp_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=tmp_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

win15.txts[4].active=1;   /* �㭪� Link ��� ���� ��襭 �।��騬 �맮���*/
if(cpanel)
  {
    panel=0;
  }else{
    panel=1;
  }

if(panels[panel].on==1 || panels[panel].on==3 || panels[cpanel].selected)
  { /* ��⨢��������� ������ ����祭� ��� ���� �⬥祭�� 䠩�� */
    name=panels[panel].full_dir;
  }else{
    name=panels[cpanel].names+panels[cpanel].itembuf[panels[cpanel].curfile].nname;
  }
if(tmp_len<=strlen(name)){
  free(tmp_str);
  tmp_len=(strlen(name)/128+1)*128;
  tmp_str=malloc(tmp_len);
  tst_err_malloc(tmp_str,9);
}

strcpy(tmp_str,name);

if(panels[cpanel].selected)
  { /* �뤥���� ��᪮�쪮 䠩���/��⠫���� */
    /* �஢����, ���� �� �।� ��. 䠩��� ��⠫���*/
    for(nfile=0, item=panels[cpanel].itembuf, tested=0, incl_dir=0;
	tested<panels[cpanel].selected && nfile<panels[cpanel].nfiles && incl_dir==0;
	item++,nfile++){
	if(item->attrs & MARKED){
	  item_mark=item;
	  tested++;
	  if(filtype(item->mode)==FTYPE_DIR) incl_dir=1;
	}
    }
    if(incl_dir)
      { /* ���� ��⠫��� */
	win15.txts[4].active=0;   /* �� ��⫮� ����� ᤥ���� HardLink */
	sprintf(str, "Copy %d files and dirs to", panels[cpanel].selected);
      }else{ /* ⮫쪮 䠩�� */
	sprintf(str, "Copy %d files to", panels[cpanel].selected);
      }
    win15.txts[0].txt = str;
    /*win15.txts[0].x=(win15.cols-strlen(str))/2;*/
    switch(inp_menu(inp_lst, &win15, 1, HOR|VERT))
      {
	case 1: /* ������ Enter �� ����� ��⠫��� */
	case 2: /* ������ Enter �� <Copy> */
	  if(panels[cpanel].selected==1)
	    { /*����᫮��� ���஡����� */
	      copy=1;
	    }else{ /* ��᪮�쪮 䠩��� ����� ����஢��� � �����. ��⠫��*/
	      if(stat(tmp_str, &stbuf)==0 &&
		 filtype(stbuf.st_mode)==FTYPE_DIR &&
		 access(tmp_str, W_OK)==0)
		{ /* ���� ����㯭� �� ������ ��⠫�� */

		  copy=1;
		}else{ /* ��-� �� � ���浪� � ��⠫����, � ����� �����㥬*/
		  copy=0;
		  ask(&win17, 2, HOR);
		}
	    }
	break;
	case 4: /* ������ Enter �� <Link> */
	  copy=2;
	break;
	case 5: /* ������ Enter �� <SymLink> */
	  copy=3;
	break;
	default: /* ESC � �訡�� */
	case 3: /* ������ Enter �� <Cancel> */
	  copy=0;
	break;
      }
    /* if(ask(&win15, 0, HOR) == 0) copy=1; */
  }else{ /* ����஢���� ������ 䠩��/��⠫��� ��� �⬥⪨ */
    item_mark=item= &panels[cpanel].itembuf[panels[cpanel].curfile];
    if(filtype(item->mode)==FTYPE_DIR)
      { /* ��⠫�� */
	win15.txts[4].active=0;   /* �� ��⫮� ����� ᤥ���� HardLink */
	strcpy(str, "Copy DIRECTORY \"");
      }else{ /* 䠩� */
	strcpy(str, "Copy file \"");
      }
    i=win15.cols-2*win15.txts[0].x-5; /* ����. ���� ��� ��ப� */
    strncat(str, panels[cpanel].names+item->nname, i-strlen(str));
    str[i]='\0';
    if(strlen(str)==i){ /* ��� �����⨫��� �� ��*/
      str[i-1]=str[i-2]=str[i-3]='.';
    }
    strcat(str, "\" to");

    win15.txts[0].txt = str;
    /*win15.txts[0].x=(win15.cols-strlen(str))/2;*/
    switch(inp_menu(inp_lst, &win15, 1, HOR|VERT))
      {
	case 1: /* ������ Enter �� ����� ��⠫��� */
	case 2: /* ������ Enter �� <Copy> */
	  copy=1;
	break;
	case 4: /* ������ Enter �� <Link> */
	  copy=2;
	break;
	case 5: /* ������ Enter �� <SymLink> */
	  copy=3;
	break;
	default: /* ESC � �訡�� */
	case 3: /* ������ Enter �� <Cancel> */
	  copy=0;
	break;
      }

    if(copy){
      /* �����㥬 �⬥�� */
      /* item= &panels[cpanel].itembuf[panels[cpanel].curfile]; - �뫮 ���*/
      item->attrs ^= MARKED;
      panels[cpanel].selected++;
      panels[cpanel].size_sel+=item->size;
    }
  }
if(copy==1 &&
   panels[cpanel].selected==1 &&
   filtype(item_mark->mode)==FTYPE_REG){
   /* ����஢���� ������ 䠩��, ��������, � ��२���������� */
   stbuf.st_mode=0;
   if(stat(tmp_str, &stbuf)!=0 || filtype(stbuf.st_mode)!=FTYPE_DIR){
     copy=4;  /* ����஢��� �� cp */
   }
}

if(copy && stat(tmp_str, &stbuf)==0){ /* �஢���� �� overwrite */

  if(filtype(stbuf.st_mode)==FTYPE_DIR)
    { /* �����㥬 � ��⠫�� */
      for(ptr=tmp_str; *ptr; ptr++);
      *ptr='/';

      for(nfile=0, item=panels[cpanel].itembuf, tested=0;
	tested<panels[cpanel].selected && nfile<panels[cpanel].nfiles && exists==0;
	item++,nfile++){
	if(item->attrs & MARKED){
	  strcpy(ptr+1, panels[cpanel].names+item->nname);
	  if(access(tmp_str, 0)==0) exists++;
	}
      }
      *ptr='\0';
    }else{ /* �����㥬 � �������騩 䠩�*/
      exists++;
    }
}

if(exists){ /* ���⢥ত���� �� Overwrite */
  if(ask(&win77, 0, HOR) != 0) copy=0;
}

switch(copy)
  {
    case 1: /* ���� ����஢���� */
      if(pipe(fd)){
	endwin();
	perror("Can't make pipe");
	exit(-1);
      }
      if(pipe(fd_err)){
	endwin();
	perror("Can't make errors pipe");
	exit(-1);
      }

      sprintf(str, "%d files", panels[cpanel].selected);
      win19.txts[1].x=(win19.cols-strlen(str))/2;
      win19.txts[1].txt=str;
      soob(&win19);

      /* �ନ஢���� ��㬥�⮢ � �.�. �㤥� ��-�� � �⤥�쭮� �����*/
      /* ��� ⮣�, �⮡� �� ��������� � �����।������ ����� */
      switch(pid=fork())
	{
	  case 0:  /* ��⮬�� - �㤥� ᮧ������ ��娢 */
	    close(fd[0]);
	    close(fd_err[0]);
	    dup2(fd_err[1],2);
	    close(fd_err[1]);
	    /* �᢮����� ������ ��� �᪫.���몮� �� �ନ஢���� ᯨ᪠ ��㬥�⮢*/
	    for(panel=0; panel<NPANELS; panel++){
	      if(panel!=cpanel){
		delwin(panels[panel].win);
		free(panels[panel].full_dir);
		free(panels[panel].itembuf);
		free(panels[panel].names);
	      }
	    }

	    delwin(stdscr);
	    delwin(save_scr);

	    if(!(spis=malloc((panels[cpanel].selected+4)*sizeof(char*)))) exit(-1);

	    spis_p=spis;

	    *spis_p++="tar";
	    *spis_p++="cfp";
	    *spis_p++="-";

	    for(nfile=0, item=panels[cpanel].itembuf, tested=panels[cpanel].selected;
		tested && nfile<panels[cpanel].nfiles;
		item++,nfile++){
		if(item->attrs & MARKED){
		  *spis_p++=panels[cpanel].names+item->nname;
		  tested--;
		}
	    }
	    *spis_p=NULL;
	  close(2);/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
	    dup2(fd[1],1); /* ��७����祭�� �⠭���⭮�� �뢮��*/
	    close(fd[1]);
	    for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
	    execvp(*spis, spis);
	    exit(-1); /* �訡�� �맮�� - �㤥� �஢�७ ���*/
	  break;

	  case -1: /* �� ᮧ������ ����� */
	    close(fd_err[0]);
	    close(fd_err[1]);
	    ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
	  break;

	  default: /* ����� - த�⥫� */
	    switch(pid1=fork())
	      {
		case 0: /* �� ���� ��⮬�� - �㤥� ���� */
		  close(fd_err[0]);
		  dup2(fd_err[1],2);
		  close(fd_err[1]);

		  if(chdir(tmp_str)) exit(-1);
		  close(fd[1]);
		  dup2(fd[0],0); /* ��७����祭�� �⠭���⭮�� �����*/
		  close(fd[0]);
		  for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		  execlp("tar", "tar", "xfp", "-", NULL);
		  exit(-1); /* �訡�� �맮�� - �㤥� �஢�७ ���*/
		break;

		case -1: /* �� ᮧ������ ����� */
		  close(fd[0]);
		  close(fd[1]);
		  close(fd_err[0]);
		  close(fd_err[1]);
		  ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
		break;

		default: /* ᮢᥬ த�⥫�, �㤥� �����, ����� ��������� tar'� */
		  close(fd[0]);
		  close(fd[1]);
		  close(fd_err[1]);

		  if(read(fd_err[0], str, 1)==1) err++;
		  close(fd_err[0]);

		  i=2; /* ������⢮ ��������� ����ᮢ */
		  do{
		   status=0;
		   pid_ready=wait(&status);
		   if(pid_ready==pid){ /* �����稫�� ����*/
		     if(status) err++;
		     i--;
		   }else if(pid_ready==pid1){ /* �����稫�� ��ன*/
		     if(status) err++;
		     i--;
		   }
		  }while(i);

		  if(err) ask(&win16,1,0); /* ᮮ�饭�� �� �訡��*/

		break;
	      } /* switch(fork..*/
	  break;
	} /* switch(fork..*/
    break;

    case 2: /*============================ Link ============================*/
    case 3: /*========================== SymLink ===========================*/
    case 4: /*====================== ����஢���� �� cp =====================*/

      for(nfile=0, item=panels[cpanel].itembuf;
	  panels[cpanel].selected && nfile<panels[cpanel].nfiles && copy;
	  item++,nfile++){
	  if(item->attrs & MARKED){

	    name=panels[cpanel].names+item->nname;
	    if(strlen(name) >= sizeof(str))
	      { /* ������ ��� �� ����頥��� */
		strcpy(str,"...");
		strncpy(str+3, name, sizeof(str)-3);
		str[sizeof(str)-1]='\0';
	      }else{ /* ��� 䠩�� ����頥��� 楫����*/
		strcpy(str,name);
	      }
	    win20.txts[1].x=(win20.cols-strlen(str))/2;
	    win20.txts[1].txt=str;
	    soob(&win20);

	    switch(pid=fork())
	      {
		case 0:  /* ��⮬�� - �㤥� ��뢠�� *copyprog */
		  /* ⥪�騩 ��⠫�� ��⠭����� � main() */
		  if((i=open("/dev/null", O_WRONLY))>=0){
		    dup2(i,2); /* ��७����祭�� ��⮪��� �訡�� */
		    close(i);
		  }

		  if(copy==3)
		    { /* ��� SymLink �㦥� ���� � ������ ��� ��⠫���*/
		      /* �᢮����� ������ ��� �᪫.���몮� �� �ନ஢����*/
		      for(panel=0; panel<NPANELS; panel++){
			if(panel!=cpanel){
			  delwin(panels[panel].win);
			  free(panels[panel].full_dir);
			  free(panels[panel].itembuf);
			  free(panels[panel].names);
			}
		      }

		      delwin(stdscr);
		      delwin(save_scr);

		      if( !(name=malloc(strlen(panels[cpanel].full_dir)+
				     strlen(panels[cpanel].names+item->nname)+
				     2 )) ) exit(-1);

		      strcpy(name,panels[cpanel].full_dir);
		      strcat(name,"/");
		      strcat(name,panels[cpanel].names+item->nname);


		      for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		      execlp(copyprog[copy], copyprog[copy], "-s",
			     name,
			     tmp_str,
			     NULL);

		    }else{ /* Link ��� Copy */
		      for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		      execlp(copyprog[copy], copyprog[copy],
			     panels[cpanel].names+item->nname,
			     tmp_str,
			     NULL);
		    }

		  exit(-1); /* �訡�� �맮�� - �㤥� �஢�७ ���*/
		break;

		case -1: /* �� ᮧ������ ����� */
		  ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
		break;

		default: /* ����� - த�⥫� */
		     while(wait(&wait_stat)!=pid);
		     if(wait_stat)
		       { /* �뫠 �訡�� */
			 if(say_error){
			   win21.txts[1].x=(win21.cols-strlen(str))/2;
			   win21.txts[1].txt=str;
			   switch(ask(&win21,2,HOR))
			     {
			       default:
			       case 2: /* OK */
			       break;

			       case 3: /* Silently - �� ��訢��� (� nc �⮣� ���)*/
				 say_error=0;
			       break;

			       case 4: /* Cancel (� nc �⮣� ���)*/
				 copy=0;
			       break;
			     }
			 }
		       }
		     item->attrs ^= MARKED;
		     panels[cpanel].selected--;
		     panels[cpanel].size_sel-=item->size;
		     outpanel=cpanel;
		     refr_panel();

		break;
	      } /* switch */
	  } /* if(�⬥祭) */
      }   /* for �� 䠩��� */
    break;

  } /* switch(copy) */
delsoob();

/* ����⠭������� ��ଠ�쭮�� �࠭� */
for(outpanel=0;outpanel<NPANELS;outpanel++){
    if(copy)
      { /* ��-� ����஢����� */
	nfile=panels[outpanel].curfile;
	nfirst=panels[outpanel].firstfile;
	if(chdir(panels[outpanel].full_dir)) beep();
	read_to_panel();
	if(nfile<panels[outpanel].nfiles) {
	  panels[outpanel].firstfile=nfirst;
	  panels[outpanel].curfile=nfile;
	  refr_panel();
	}
      }else{
#ifdef FREEBSD
	if(panels[outpanel].on>0) refr_panel();
#else
	if(panels[outpanel].on>0) touchwin(panels[outpanel].win);
#endif
      }
    if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
}

}
