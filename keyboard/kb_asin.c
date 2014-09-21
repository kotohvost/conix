/*              �㭪樨 �ᨭ�஭��� ࠡ��� � ��������ன        */
/*****************************************************************************
 *      kbstart() - �������� �����-��⥫� ����������
 *      kbhit()   - �஢�ઠ ������ ��������� ᨬ�����.
 *                  �����頥� �� ������⢮
 *      kbget()   - ���� ����� � ���������� � ��।������ �� ⨯�� �
 *                  ����஢���. ��।����� �� getkey_()
 *      kbstop()  - ��⠭���� �����-��⥫� ����������
*****************************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdio.h>
#include <ctype.h>
#include "keys.h"

static  fd[2];  /* ��� ᮧ����� �ணࠬ����� ������ */
static pid_t pid; /* ����� �����-��⥫� ���������� */

/****************************************************************************/
		kbstart()
/*      �������� �����-��⥫� ����������

*****************************************************************************/
{
	char    ch;
static  char    nul=0;

if(pid){
  fprintf(stderr, "Programmer error (double kbstart())\n");
  exit(-1);
}

if(pipe(fd)!=0){
    perror("kbstart:Can't create pipe");
    exit(-1);
}

switch(pid=fork())
  {
    case 0: /* ��஦����� ����� */
      signal(SIGINT, SIG_DFL); /* curses ���⠢��� ᢮� ��ࠡ��� (endwin() ��� ��-� ��������)*/
      close(fd[0]);
      while(1){
	if(read(0, &ch, 1)==1)
	  {
	    write(fd[1], &ch, 1);
	  }else{
	    close(fd[1]);
	    exit(-1);
	  }
      }
    break;
    case -1: /* �� ᮧ������ ����� */
      perror("kbstart:Can't create process");
      exit(-1);
    break;
    default: /* ��⥫� ᮧ��� */
      close(fd[1]);
    break;
  }
}


/****************************************************************************/
		size_t kbhit()
/*      �஢�ઠ ������ ��������� ᨬ�����
	�����頥� �� ������⢮
*****************************************************************************/
{
	struct stat stbuf;
/*==========================================================================*/
if(!pid) kbstart();
stbuf.st_size=0;
if(fstat(fd[0], &stbuf)!=0){
  perror("kbhit:Can't fstat pipe");
  exit(-1);
}
return(stbuf.st_size);
}

/****************************************************************************/
		int kbget()
/*      ���� ����� � ���������� � ��।������ �� ⨯�� � ����஢���
 *      ��।����� �� getkey_()
*****************************************************************************/
{
extern struct   cmdkey  stringcom_[];   /* */
       unsigned char    str_[256];      /*��ப�-�⢥� �-��*/
auto            int     priznpovt;      /*�ਧ�. �த���. ���᪠*/
auto   unsigned char    *simb;          /*㪠��⥫� �� �������� ᨬ���*/
auto            int     cmd;            /*������� - १-� getkey*/
auto            int     povtlstr;       /*�ਧ�.����� ���᪠ �� ��ப��*/
	 unsigned char    *ukch;        /*�-�� �� ⥪��. �ࠢ�. ᨬ���*/
	 unsigned char    *ukch1;       /*�-�� �� ⥪��. �ࠢ�. ᨬ���*/
	 struct cmdkey  *uk;            /*�-�� �� ⥪�騩 �-� stringcom*/
/*==========================================================================*/
str_[0]='\0'; /* Not in INDIS-2 !!!*/
priznpovt=1;
for(simb=str_;priznpovt;simb++) {
  if(*simb=='\0') {
    read(fd[0],simb,1);

    if(!*simb) return(ERROR_KEY);

/*  *simb=getch(); */
    *(simb+1)='\0';
  }
  /*�.�. ��᫥����⥫쭮��� �� stringcom; � �� �⭮����� CR � �.�.*/
  povtlstr=1;
  for(uk=stringcom_;uk->command && povtlstr;uk++){/*���� �� 蠡����� ������*/
    if(uk->string)
      { /*��᫥�-�� �� stringcom*/
	for(ukch1=str_,ukch=(uk->string);
	    (*ukch==*ukch1) && *ukch;
	    ukch++,ukch1++);
	if(*ukch1=='\0') { /*ᮢ������� ��� ���� ᨬ�����*/
	    if(*ukch=='\0')
	      { /*������ ᮢ�������*/
		cmd=uk->command;
		priznpovt=povtlstr=0;
	      }else{ /*���筮� ᮢ�������*/
		povtlstr=0;
	    } /* (*ukch=='\0') */
	}  /* *ukch1=='\0'*/
    } /*(uk->string!=0) */
  } /*�� ��������*/
  if(povtlstr) { /*��ᮢ�������*/
      if(isprint(str_[0]) && str_[1]=='\0')
	{ /*������� ᨬ��� (isprint ������ ���ਭ����� ��ਫ����)*/
	  cmd=PECH_SIMB|str_[0];
	}else{
	  cmd=ERROR_KEY|str_[0];
      }
      priznpovt=0;
  }
}
return(cmd);
}

/****************************************************************************/
		kbstop()
/*      ��⠭���� �����-��⥫� ����������
*****************************************************************************/
{
/*==========================================================================*/
kill(pid, SIGINT);
while(wait(NULL) != pid);       /* ��� ⮣�, �⮡� �� �뫮 ����� */
pid=0;
}
