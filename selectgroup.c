#include "defs.h"
/****************************************************************************/
			selectgroup()
/*                      �⬥⪠ ��㯯� 䠩���
/****************************************************************************/
{
extern  struct  win     win33;          /* ���� � ����ᮬ select */
extern  struct  win     win34;          /* ���� � �訡��� */
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  char    *fnd_str;       /* ��ப� ��� �६����� ������ */
extern  size_t  fnd_len;        /* ����, �뤥������ ��� fnd_str*/
	int     selected=0;             /* ������⢮ �⬥祭��� 䠩���*/
	char    str[20];
	int     nfile;
 struct inp_lst inp_lst[2];       /* ᯨ᮪ ����� ��� inp_menu()*/
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
/*==========================================================================*/

inp_lst[0].str=fnd_str;
/* inp_lst[0].fld_len=...;  ���⠢�� inp_menu()*/
inp_lst[0].str_len=fnd_len;
inp_lst[0].npunkt=1;
inp_lst[1].str=NULL; /* �ਧ��� ���� ᯨ᪠ ��� inp_menu()*/

if(!fnd_str[0]) strcpy(fnd_str, "*");
if(inp_menu(inp_lst, &win33, 1, 0)==1){
  for(nfile=2, item=panels[cpanel].itembuf+2;
      nfile<panels[cpanel].nfiles;
      item++,nfile++){
    if(inregular(panels[cpanel].names+item->nname, fnd_str)){
      selected++;
      if(!(item->attrs & MARKED)){
	item->attrs |= MARKED;
	panels[cpanel].selected++;
	panels[cpanel].size_sel+=item->size;
      }
    }
  }
  if(!selected){
    str[sizeof(str)-1]='\0';
    strncpy(str, fnd_str, sizeof(str));
    if(str[sizeof(str)-1]){ /* �� �����⨫���*/
      str[sizeof(str)-1]='\0';
      str[sizeof(str)-2]=str[sizeof(str)-3]=str[sizeof(str)-4]='.';
    }
    win34.txts[1].txt=str;
    win34.txts[1].x=(win34.cols-strlen(str))/2;
    ask(&win34, 2, 0);
  }
  outpanel=cpanel;
  if(panels[cpanel].on<0) panels[cpanel].on *= -1;
  refr_panel();
  wrefresh(panels[cpanel].win);
}
}
