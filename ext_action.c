#include <unistd.h>
#include <stdlib.h>
#include "defs.h"
/****************************************************************************/
				ext_action(name, config)
/*                      �믮������ ����⢨� � ᮮ⢥��⢨� � ���७���
			�᫨ ����⢨� �믮�����, � �����頥��� 0,
			���� - �� 0
*****************************************************************************/
	char    *name;          /* ��� ��ࠡ��뢠����� 䠩��*/
	char    *config;        /* 䠩� ���䨣��樨 */
{
extern  char    *tmp_str;       /* ��ப� ��� ���᪠ */
extern  size_t  tmp_len;        /* ����, �뤥������ ��� tmp_str*/
static  char    *fullname=NULL; /* ������ ��� 䠩�� ���䨣��樨*/
static  size_t  fullnamelen=0;  /* ࠧ��� �����, �뤥������ ��� fullname*/
	char    *home;          /* ����譨� ��⠫�� ���짮��⥫� */
	FILE    *f;             /* ��� �⥭�� 䠩�� ���䨣��樨 */
	int     i;
	int     cmd_i;
	int     find;
	int     ch;
	int     nch;
/*==========================================================================*/
if(access(config, R_OK) != 0){ /* � ⥪�饬 ��⠫��� ��� ����㯭��� ��� �⥭�� 䠩�� ����஥� */
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
	{ /* ������ 䠩� ���䨣��樨 */
	  config=fullname;
	}else{
	  config=NULL;
	}
    }else{
	  config=NULL;
    }
}

if(config) { /* ���� 䠩� ���䨣��樨 � ���� �믮������� ��㣨� �᫮��� ࠡ���*/
  if(!(f=fopen(config, "r")))
    {
      config=NULL;
    }else{
      find=0;
      while(find==0 && feof(f)==0){
	fgets(tmp_str, tmp_len, f);
	if(!feof(f)){
	  for(i=0; i!=tmp_len-1 && !isspace(tmp_str[i]); i++);
	  if(i && isspace(tmp_str[i])){ /*��-� ������� */
	    tmp_str[i] = '\0';
	    if(inregular(name, tmp_str)) find++;
	  }
	}
      }
      if(find)
	{ /* ������ 蠡��� */
	  i++; /* tmp_str[i] �� ��⠭����� �� \0 */
	  while(i!=tmp_len-2 && isspace(tmp_str[i])) i++;
	  cmd_i = i; /* ����� ��稭����� ������� */
	  while(i!=tmp_len-2 && tmp_str[i]!='\n') i++;
	  if(i == tmp_len-2) beep();

	  if(i+strlen(name)+2 > tmp_len){
	      tmp_len += strlen(name)+2;
	      tmp_str=realloc(tmp_str, tmp_len);
	      tst_err_malloc(tmp_str,29); /* �믮���� exit, �᫨ NULL*/
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
    /* �믮������ ������� */
    execcmd(&tmp_str[cmd_i]);
    return(0);
  }else{
    return(-1);
  }
}
