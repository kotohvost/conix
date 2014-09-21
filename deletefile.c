#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "defs.h"
/****************************************************************************/
				deletefile()
/*                      �������� 䠩��/䠩���
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  win     win4;   /* ����� �� 㤠����� ��㯯� 䠩��� */
extern  struct  win     win5;   /* ����� �� 㤠����� 䠩�� �� ��㯯�*/
extern  struct  win     win6;   /* ����� �� 㤠����� read only - 䠩��*/
extern  struct  win     win3;   /* ���ᠭ�� ���� �訡�� ᮧ����� �����*/
extern  struct  win     win7;   /* ᮮ�饭�� � ��砫� 㤠����� ��⠫���*/
extern  struct  win     win8;   /* ᮮ�饭�� �� �訡�� 㤠����� ��⠫���*/
extern  struct  win     win9;
extern  struct  win     win10;
extern  struct  win     win11;
extern  struct  win     win12;
extern  struct  win     win13;  /* ᮮ�饭�� �� �訡�� 㤠����� �����筮�� �.*/
extern  struct  win     win14;  /* ᮮ�饭�� �� �訡�� 㤠����� ��⠫���*/
	char    str[40];
	int     del_all=0;      /* �ਧ��� �⢥� delete all*/
	int     stop=0;         /* �ਧ��� �४�饭�� 㤠����� */
	int     del_file;       /* �ਧ��� 㤠����� 䠩�� */
	int     ask_ronly=1;    /* ����訢��� ���⢥ত���� ��� read only*/
	int     say_error=1;    /* ᮮ���� �� �訡��� */
	char    *name;          /* ��� 䠩�� */
	pid_t   pid;
	int     wait_stat;      /* ����� �����襭�� ����� */
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
	int     i;
	int     deleted=0;      /* �ਧ��� ⮣�, �� ��-� 㤠�﫮�� */
	int     namelen;        /* ����� ����� 䠩�� */
	int     outlen;         /* ��������� ࠧ��� ���ᨬ��쭮� ����� ����� 䠩��, ������饣��� � ���� ����ᮢ*/
unsigned char   *ptr;           /* ��� �஢�ન ����� �� CTRL-ᨬ���� */
/*==========================================================================*/
/* ��������� ࠧ��� ���ᨬ��쭮� ����� ����� 䠩��, ������饣��� � ���� ����ᮢ*/
outlen=sizeof(str)-1;
if(outlen>win5.cols) outlen=win5.cols;
if(outlen>win6.cols) outlen=win6.cols;
if(outlen>win7.cols) outlen=win7.cols;
if(outlen>win8.cols) outlen=win8.cols;
if(outlen>win9.cols) outlen=win9.cols;
if(outlen>win10.cols) outlen=win10.cols;
if(outlen>win11.cols) outlen=win11.cols;
if(outlen>win12.cols) outlen=win12.cols;
if(outlen>win13.cols) outlen=win13.cols;
if(outlen>win14.cols) outlen=win14.cols;
outlen-=4; /* �� ࠬ�� � �஡��� ����� ��� */
/*-------------------------------------*/

if(panels[cpanel].selected)
  {
   sprintf(str, "You have selected %d files.", panels[cpanel].selected);
   win4.txts[2].txt = str;
   if(ask(&win4, 1, HOR) == 0){ /* ����祭� ���⢥ত����*/
     for(nfile=0; stop==0 && panels[cpanel].selected && nfile<=panels[cpanel].nfiles; nfile++){
       item= &panels[cpanel].itembuf[nfile];
       if(item->attrs & MARKED){ /* ��࠭�� 䠩� */
	 /* ��ନ஢��� ��� 䠩�� ��� ����ᮢ*/
	 name=panels[cpanel].names+item->nname;
	 name_to_out(name, str, outlen);

	 if(del_all)
	  { /* �������� ��� ����ᮢ*/
	    del_file=1;
	  }else{ /* 㤠����� � ����ᠬ� */
	     win5.txts[1].x=(win5.cols-strlen(str))/2;
	     win5.txts[1].txt=str;
	     switch(ask(&win5, 2, HOR))
	       {
		 case 2: /* Delete */
		   del_file=1;
		 break;

		 case 3: /* All */
		   del_file=1;
		   del_all=1;
		 break;

		 case 4: /* Skip */
		   del_file=0;
		 break;

		 default:
		 case 5: /* Cancel */
		   del_file=0;
		   stop=1;
		 break;
	       }
	 } /* if(!del_all) */

	 /* ���⢥ত���� ��� read only 䠩��� */
	 if(del_file && ask_ronly){ /* ��࠭� 㤠����� ��� ०�� del_all*/
	     if( access(name,W_OK)){ /* � 䠩� ����饭� �����*/
	       win6.txts[1].x=(-1);
	       win6.txts[1].txt=str;
	       switch(ask(&win6,3,HOR))
		 {
		   case 3: /* Delete */
		     /* del_file=1;*/
		   break;
		   default:
		   case 4: /* Skip (� nc-Cancel, �� �� ���ࠢ��)*/
		     del_file=0;
		   break;
		   case 5: /* Silently - �� ��訢��� (� nc �⮣� ���)*/
		     ask_ronly=0;
		   break;
		   case 6: /* Cancel (� nc �⮣� ���)*/
		     del_file=0;
		     stop=1;
		   break;
		 }
	     }
	 }

	 /* �������� */
	 if(del_file){
	   deleted++;
	   if(filtype(item->mode)==FTYPE_DIR)
	     { /* ��⠫�� */
	       win7.txts[1].x=(-1);
	       win7.txts[1].txt=str;
	       soob(&win7);
	       switch(pid=fork())
		 {
		   case 0: /* ��஦����� �����*/
		     close(2);
		     open("/dev/null",O_WRONLY);
		     for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		     execlp("rm", "rm", "-rf", name, NULL);
		     exit(-1); /* �訡�� �맮�� - �㤥� �஢�७ ���*/
		   break;
		   case -1: /* �� ᮧ������ ����� */
		     ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
		   break;
		   default: /* ����� - த�⥫� */
		     while(wait(&wait_stat)!=pid);
		     if(wait_stat)
		       { /* �뫠 �訡�� ��࠭��*/
			 if(say_error){
			   win8.txts[1].x=(-1);
			   win8.txts[1].txt=str;
			   switch(ask(&win8,2,HOR))
			     {
			       default:
			       case 2: /* OK */
			       break;

			       case 3: /* Silently - �� ��訢��� (� nc �⮣� ���)*/
				 say_error=0;
			       break;

			       case 4: /* Cancel (� nc �⮣� ���)*/
				 del_file=0;
				 stop=1;
			       break;
			     }
			 }
		       }else{ /* ��⠫�� ��ଠ�쭮 ���� */
			 item->attrs ^= MARKED;
			 panels[cpanel].selected--;
			 panels[cpanel].size_sel-=item->size;
			 /* refr_panel(); wrefresh(panels[cpanel].win) */
		       }
		   break;
		 }
	     }else{ /* 䠩� ��� ��-� ᯥ樠�쭮� */
	       win9.txts[1].x=(-1);
	       win9.txts[1].txt=str;
	       soob(&win9);
	       if(unlink(name))
		 { /* �訡�� 㤠����� 䠩�� */
		   if(say_error){
		     win10.txts[1].x=(-1);
		     win10.txts[1].txt=str;
		     switch(ask(&win10,2,HOR))
		       {
			 default:
			 case 2: /* OK */
			 break;

			 case 3: /* Silently - �� ��訢��� (� nc �⮣� ���)*/
			   say_error=0;
			 break;

			 case 4: /* Cancel (� nc �⮣� ���)*/
			   del_file=0;
			   stop=1;
			 break;
		       }
		   }
		 }else{ /* 䠩� ��ଠ�쭮 ���� */
		   item->attrs ^= MARKED;
		   panels[cpanel].selected--;
		   panels[cpanel].size_sel-=item->size;
		   outpanel=cpanel;
		   refr_panel();
		 }
	     }
	 } /* if(del_file) */
       } /* if MARKED */
     } /* for �� 䠩��� */
   } /* ���⢥ত���� ��࠭�� */

  }else{/*---------------------- ��� �⬥祭��� 䠩��� ---------------------*/

    item= &panels[cpanel].itembuf[panels[cpanel].curfile];
    /* ��ନ஢��� ��� 䠩�� ��� ����ᮢ*/
    name=panels[cpanel].names+item->nname;
    name_to_out(name, str, outlen);

    win11.txts[1].x=(-1);
    win11.txts[1].txt=str;
    switch(ask(&win11, 3, HOR))
      {
	case 2: /* Delete */
	  del_file=1;
	break;

	default:
	case 3: /* Cancel */
	  del_file=0;
	break;
      }

    /* ���⢥ত���� ��� read only 䠩��� */
    if(del_file){
	if( access(name,W_OK)){ /* � 䠩� ����饭� �����*/
	  win12.txts[1].x=(-1);
	  win12.txts[1].txt=str;
	  switch(ask(&win12,3,HOR))
	    {
	      case 3: /* Delete */
		/* del_file=1;*/
	      break;
	      default:
	      case 4: /* Cancel */
		del_file=0;
		stop=1;
	      break;
	    }
	}
    }

    /* �������� */
    if(del_file){
      deleted++;
      if(filtype(item->mode)==FTYPE_DIR)
	{ /* ��⠫�� */
	  win7.txts[1].x=(-1);
	  win7.txts[1].txt=str;
	  soob(&win7);
	  switch(pid=fork())
	    {
	      case 0: /* ��஦����� �����*/
		close(2);
		open("/dev/null",O_WRONLY);
		for(i=1;i<NSIG;i++) signal(i, SIG_DFL);
		execlp("rm", "rm", "-rf", name, NULL);
		exit(-1); /* �訡�� �맮�� - �㤥� �஢�७ ���*/
	      break;
	      case -1: /* �� ᮧ������ ����� */
		ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
	      break;
	      default: /* ����� - த�⥫� */
		while(wait(&wait_stat)!=pid);
		if(wait_stat)
		  { /* �뫠 �訡�� ��࠭��*/
		    if(say_error){
		      win14.txts[1].x=(-1);
		      win14.txts[1].txt=str;
		      switch(ask(&win14,2,HOR))
			{
			  default:
			  case 2: /* OK */
			  break;

			  case 3: /* Silently - �� ��訢��� (� nc �⮣� ���)*/
			    say_error=0;
			  break;

			  case 4: /* Cancel (� nc �⮣� ���)*/
			    del_file=0;
			    stop=1;
			  break;
			}
		    }
		  }else{ /* ��⠫�� ��ଠ�쭮 ���� */
		    item->attrs ^= MARKED;
		    panels[cpanel].selected--;
		    panels[cpanel].size_sel-=item->size;
		    outpanel=cpanel;
		    refr_panel();
		  }
	      break;
	    }
	}else{ /* 䠩� ��� ��-� ᯥ樠�쭮� */
	  soob(&win9);
	  if(unlink(name))
	    { /* �訡�� 㤠����� 䠩�� */
	      if(say_error){
		win13.txts[1].x=(-1);
		win13.txts[1].txt=str;
		switch(ask(&win13,2,HOR))
		  {
		    default:
		    case 2: /* OK */
		    break;

		    case 3: /* Silently - �� ��訢��� (� nc �⮣� ���)*/
		      say_error=0;
		    break;

		    case 4: /* Cancel (� nc �⮣� ���)*/
		      del_file=0;
		      stop=1;
		    break;
		  }
	      }
	    }else{ /* 䠩� ��ଠ�쭮 ���� */
	      item->attrs ^= MARKED;
	      panels[cpanel].selected--;
	      panels[cpanel].size_sel-=item->size;
	      outpanel=cpanel;
	      refr_panel();
	    }
	}
    } /* if(del_file) */
  } /* if ... selected ... else */
delsoob();

for(outpanel=0;outpanel<NPANELS;outpanel++){
    if(deleted)
      { /* ��-� 㤠�﫮�� */
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

