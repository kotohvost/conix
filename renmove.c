#include "defs.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
/****************************************************************************/
			renmove()
/*              ��२���������/��६�饭�� 䠩��/䠩���
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  win     win3;
extern  struct  win     win44;  /* ����� �� ���⢥ত���� ��२�.��㯯� �.*/
extern  struct  win     win45;  /* ᮮ�饭�� �� �訡��*/
extern  struct  win     win46;  /* ᮮ�饭�� �� �訡�� ����㯠 � ��⠫��� */
extern  struct  win     win47;  /* ᮮ�饭�� � ����஢���� 䠩��(�)*/
extern  struct  win     win48;  /* ᮮ�饭�� � ����஢����*/
extern  struct  win     win49;  /* ᮮ�饭�� �� �訡�� */
extern  struct  win     win77;  /* ����� �� Overwrite */
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
	int     copy=0;         /* �ਧ��� � ⨯ */
	int     panel;          /* "�� ⥪���" ������ */
	int     err=0;          /* �ਧ��� �訡�� */
	int     say_error=1;    /* ᮮ���� �� �訡��� */
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
	int     exists=0;       /* �ਧ��� �����-�� ����� Overwrite */
	char    *ptr;           /* ��� ��ࠧ������ ����� � ��⠫��� */
 struct stat    stbuf;
 struct inp_lst inp_lst[2];      /* ᯨ᮪ ����� ��� inp_menu()*/
 static char    copyprog[]="mv"; /*�ணࠬ�� ����஢����*/
/*==========================================================================*/
inp_lst[0].str=tmp_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=tmp_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

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
	sprintf(str, "Move %d files and dirs to", panels[cpanel].selected);
      }else{ /* ⮫쪮 䠩�� */
	sprintf(str, "Move %d files to", panels[cpanel].selected);
      }
    win44.txts[0].txt = str;
    /*win44.txts[0].x=(win44.cols-strlen(str))/2;*/
    switch(inp_menu(inp_lst, &win44, 1, HOR|VERT))
      {
	case 1: /* ������ Enter �� ����� ��⠫��� */
	case 2: /* ������ Enter �� <Move> */
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
		  ask(&win46, 2, HOR);
		}
	    }
	break;
	default: /* ESC � �訡�� */
	case 3: /* ������ Enter �� <Cancel> */
	  copy=0;
	break;
      }
    /* if(ask(&win44, 0, HOR) == 0) copy=1; */
  }else{ /* ����஢���� ������ 䠩��/��⠫��� ��� �⬥⪨ */
    item_mark=item= &panels[cpanel].itembuf[panels[cpanel].curfile];
    if(filtype(item->mode)==FTYPE_DIR)
      { /* ��⠫�� */
	incl_dir=1;
	strcpy(str, "Move DIRECTORY \"");
      }else{ /* 䠩� */
	incl_dir=0;
	strcpy(str, "Move file \"");
      }
    i=win44.cols-2*win44.txts[0].x-5; /* ����. ���� ��� ��ப� */
    strncat(str, panels[cpanel].names+item->nname, i-strlen(str));
    str[i]='\0';
    if(strlen(str)==i){ /* ��� �����⨫��� �� ��*/
      str[i-1]=str[i-2]=str[i-3]='.';
    }
    strcat(str, "\" to");

    win44.txts[0].txt = str;
    /*win44.txts[0].x=(win44.cols-strlen(str))/2;*/
    switch(inp_menu(inp_lst, &win44, 1, HOR|VERT))
      {
	case 1: /* ������ Enter �� ����� ��⠫��� */
	case 2: /* ������ Enter �� <Move> */
	  copy=1;
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

if(copy){

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
	    win48.txts[1].x=(win48.cols-strlen(str))/2;
	    win48.txts[1].txt=str;
	    soob(&win48);

	    switch(pid=fork())
	      {
		case 0:  /* ��⮬�� - �㤥� ��뢠�� *copyprog */
		  /* ⥪�騩 ��⠫�� ��⠭����� � main() */
		  if((i=open("/dev/null", O_WRONLY))>=0){
		    dup2(i,2); /* ��७����祭�� ��⮪��� �訡�� */
		    close(i);
		  }

		  for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		  execlp(copyprog, copyprog, "-f",
			 panels[cpanel].names+item->nname,
			 tmp_str,
			 NULL);

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
			   win49.txts[1].x=(win49.cols-strlen(str))/2;
			   win49.txts[1].txt=str;
			   switch(ask(&win49,2,HOR))
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
}
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
