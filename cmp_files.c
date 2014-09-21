#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "defs.h"
/****************************************************************************/
			cmp_files(path1, name1, path2, name2)
/*                      �ࠢ����� ᮤ�ন���� 䠩���
*****************************************************************************/
	char    *path1; /* ����� ���� �� ��ࢮ�� 䠩�� */
	char    *name1; /* ��� ��ࢮ�� 䠩�� */
	char    *path2; /* ����� ���� �� ��ண� 䠩�� */
	char    *name2; /* ��� ��ண� 䠩�� */
{
extern  char    *tmp_str;       /* ��ப� ��� ���᪠ */
extern  size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
extern  struct  win win60;      /* ���� ᮮ�饭�� */
extern  struct  win win61;      /* ᮮ�饭�� �� �訡�� �ࠢ����� */
extern  struct  win     win3;
extern  struct  panel   panels[];
extern  WINDOW  *save_scr;      /* ���� ��� ��࠭���� �࠭� */
extern  char    *tmp_str;       /* ��ப� ��� �६����� ������ */
extern  char    *inp_str;       /* ��ப� ��� ����� ������ */
extern  int     silent;         /* �ਧ��� �ࠢ����� ��� ᮮ�饭�� �� �訡���*/
	int     panel;  /*��ࠡ��뢠���� ������ */
	int     len;    /* ����� �뢮����� ��� ����� */
	int     len2;
	char    str1[30]; /* ��� ��ࢮ�� 䠩�� (��� �뢮��)*/
	char    str2[30]; /* ��� ��ண� 䠩�� (��� �뢮��)*/
	pid_t   pid;            /* ����� ����� ��� �������� */
	char    *ptr1;          /* ��� 䮫ନ஢���� ��㬥�� cmp */
	char    *ptr2;          /* ��� 䮫ନ஢���� ��㬥�� cmp */
	int     status=1;       /* ���ﭨ� �����襭�� �����*/
	int     i;
/*==========================================================================*/
len=strlen(name1);
if(len>=sizeof(str1))
  { /* �����쭮� ��� �� ����頥��� � ��ப�*/
    strcpy(str1, name1+(len-sizeof(str1)));
  }else{  /*�����쭮� - ����頥���*/
    len2=strlen(path1);
    if(len+len2+1>=sizeof(str1))
      { /* ���� � ������ �� ����頥��� */
	strcpy(str1, "...");
	strcat(str1, path1+(len+len2+1+3-sizeof(str1)));
      }else{ /* �� ����頥��� */
	strcpy(str1, path1);
      }
    strcat(str1, "/");
    strcat(str1, name1);
  }

/* ������ 横� �� ���ᨢ�� 㪠��⥫��? ����! �����塞 ���祪*/
len=strlen(name2);
if(len>=sizeof(str2))
  { /* �����쭮� ��� �� ����頥��� � ��ப�*/
    strcpy(str2, name2+(len-sizeof(str2)));
  }else{  /*�����쭮� - ����頥���*/
    len2=strlen(path2);
    if(len+len2+1>=sizeof(str2))
      { /* ���� � ������ �� ����頥��� */
	strcpy(str2, "...");
	strcat(str2, path2+(len+len2+1+3-sizeof(str2)));
      }else{ /* �� ����頥��� */
	strcpy(str2, path2);
      }
    strcat(str2, "/");
    strcat(str2, name2);
  }

win60.txts[1].x=win60.txts[3].x=(-1); /* soob() ��஢��� */
win60.txts[1].txt=str1;
win60.txts[3].txt=str2;

soob(&win60);

switch(pid=fork())
  {
    case 0:  /* ��⮬�� */
      /* ��ନ஢��� ����� 䠩��� */
      /* �� ��直� ��砩 �᢮����� ��� ������, ����� �����*/
      for(panel=0; panel<NPANELS; panel++){
	  delwin(panels[panel].win);
	  /* free(panels[panel].full_dir); - �����, �.�. path* ... */
	  free(panels[panel].itembuf);
	  /* free(panels[panel].names);    - �����, �.�. name* ... */
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

    case -1: /* �� ᮧ������ ����� */
      ask(&win3,1,0); /* ᮮ�饭�� �� �訡��*/
    break;

    default: /* ����� - த�⥫� */
      while(wait(&status)!=pid);
    break;
  }

if(status && status!=(1<<8) && silent==0)
  { /* cmp �� ᬮ��� ������ 䠩�� */
    win61.txts[1].x=win61.txts[3].x=(-1); /* ask() ��஢��� */
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
	case 1<<8: /* ࠧ���� 䠩�� */
	  status=1;
	break;
	default: /* ��� ��室� 2 ��� �訡�� ��ᮢ���⨬��� */
	  status=2;
	break;
      }
  }
return(status);
}
