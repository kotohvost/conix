#include <unistd.h>
#include <stdlib.h>
#include "defs.h"
/****************************************************************************/
				read_setup()
/*                      �⥭�� ����஥� �� 䠩�� $home/.conix.ini
/****************************************************************************/
{
extern struct panel panels[];
extern  int     cpanel;         /* ⥪��� ������*/
extern  int     outpanel;       /* �뢮����� ������ */
extern  char    cmp_exist;      /* �ਧ��� �ࠢ����� �� ������ */
extern  char    cmp_time;       /* �ਧ��� �ࠢ����� �� �६��� */
extern  char    cmp_size;       /* �ਧ��� �ࠢ����� �� ࠧ���� */
extern  char    cmp_mode;       /* �ਧ��� �ࠢ����� �� ������ */
extern  char    cmp_body;       /* �ਧ��� �ࠢ����� �� ᮤ�ন���� */
extern  char    cmp_nlink;      /* �ਧ��� �ࠢ����� �� ���-�� ��뫮� */
extern  int     prompt;         /* ⨯ �ਣ��襭��:0-���, 1-NodeName */
extern  int     clock_need;     /* �ਧ��� �뢮�� �६��� 0-��� 1-�� */
	char    *home;          /* "����譨�" ��⠫�� ���짮��⥫�*/
	short   len;
static  char    name[]=".conix.ini";
	FILE    *setup;         /* 䠩�, �� ���ண� ���뢠���� ����ன�� */
	int     error=0;        /* �ਧ��� �訡��*/
	short   swaps=1;        /* ��� �஢�ન � ��⠭���� �ਧ���� ��ॢ��� ���⮢ */
/*==========================================================================*/
if(chdir(panels[cpanel].full_dir) || access(name, R_OK))
  { /* � ⥪�饬 ��⠫��� ��� ����㯭��� ��� �⥭�� 䠩�� ����஥� */
    home=getenv("HOME");
    if(!home) home=getenv("home");
  }else{
    home=".";
  }
if((!home) || chdir(home))
  { /* ��� HOME ��� ������㯥� ����譨� ��⠫�� */
  }else{ /* ����� �஡����� ������� */
    if(!(setup=fopen(name, "r")))
      { /* �� ���뢠���� 䠩� */
      }else{
	if(ferror(setup) || feof(setup)) error=1;
	if(!error) fread(&len, sizeof(len), 1, setup);
	if(len==swaps)
	  {
	    swaps=0; /* ����� ����⠢���� �� �㦭� */
	  }else{
	    swap_bin(&len, sizeof(len));
	    if(len!=swaps) error=1; /* � ��ॢ����� ����� �� ᮮ⢥������ */
	  }

	if(ferror(setup) || feof(setup)) error=1;
	if(!error) fread(&len, sizeof(len), 1, setup);
	if(swaps) swap_bin(&len, sizeof(len));
	cpanel=len;
	for(outpanel=0; outpanel<NPANELS; outpanel++){
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&panels[outpanel].on, sizeof(panels[outpanel].on), 1, setup);
	  if(swaps) swap_bin(&panels[outpanel].on, sizeof(panels[outpanel].on));
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&len, sizeof(len), 1, setup);
	  if(swaps) swap_bin(&len, sizeof(len));
	  if(ferror(setup) || feof(setup)) error=1;
	  if(outpanel==cpanel)
	    { /* ⥪��� ������ �ᥣ�� � ⥪�饬 ��⠫��� */
	      if(!error) fseek(setup, (long)len, 1);
	      if(ferror(setup) || feof(setup)) error=1;
	    }else{
	      if((!error) && len>panels[outpanel].full_dir_size){
		free(panels[outpanel].full_dir);
		panels[outpanel].full_dir_size=(len/128+1)*128;
		panels[outpanel].full_dir=malloc(panels[outpanel].full_dir_size);
		if(!panels[outpanel].full_dir) error=1;
	      }
	      if(!error) fread(panels[outpanel].full_dir, len, 1, setup);
	      if(ferror(setup) || feof(setup)) error=1;
	    }
	  if(!error) fread(&panels[outpanel].sort, sizeof(panels[outpanel].sort), 1, setup);
	  if(swaps) swap_bin(&panels[outpanel].sort, sizeof(panels[outpanel].sort));
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(panels[outpanel].outfields, sizeof(panels[outpanel].outfields), 1, setup);
	  if(swaps) {
	    for(len=0;len<sizeof(panels[outpanel].outfields)/sizeof(panels[outpanel].outfields[0]);len++){
	      swap_bin(&panels[outpanel].outfields[len], sizeof(panels[outpanel].outfields[0]));
	    }
	  }
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(panels[outpanel].filter_read, sizeof(panels[outpanel].filter_read), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(panels[outpanel].filter_noread, sizeof(panels[outpanel].filter_noread), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;

	  if(!error) fread(&cmp_exist, sizeof(cmp_exist), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_time , sizeof(cmp_time ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_size , sizeof(cmp_size ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_mode , sizeof(cmp_mode ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_body , sizeof(cmp_body ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(!error) fread(&cmp_nlink, sizeof(cmp_nlink), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;

	  if(!error) fread(&prompt   , sizeof(prompt   ), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(swaps) swap_bin(&prompt, sizeof(prompt));

	  if(!error) fread(&clock_need, sizeof(clock_need), 1, setup);
	  if(ferror(setup) || feof(setup)) error=1;
	  if(swaps) swap_bin(&clock_need, sizeof(clock_need));
	}
	if(!error){
	    getc(setup);
	    if(!feof(setup)) error=1; /* 䠩� �ᯮ�祭 ��� ��ᮢ���⨬� ���ᨨ */
	}
	fclose(setup);
	if(error){
	  unlink(name); /* 䠩� ����ᠭ �����४⭮ */
	  endwin();
	  fprintf(stderr, "Uncompatible version of $HOME/%s or ./%s (unlinked), try once more\n", name, name);
	  exit(-1);
	}
      }
    delsoob();
  }
}
