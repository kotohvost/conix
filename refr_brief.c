#include "defs.h"
#include <time.h>
#include <sys/stat.h>
#define FIRST_STR 2
/****************************************************************************/
			refr_brief()
/*              �뢮� �� �࠭ ������ outpanel � ०��� brief
*****************************************************************************/
{
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  int     cpanel;
extern  struct  panel   panels[];
extern  chtype  atr[];          /* ���᪠ */
extern  short   pred_file;      /* ����� ��।��饣� 䠩�� ��� refr_panel()*/
extern  int     clock_need;     /* �ਧ��� �뢮�� �६��� 0-��� 1-�� */
extern  int     dwidth;         /* �ਧ��� ������� ������� �ਭ� */
	int     nfile=0;        /* ����� ⥪�饣� 䠩��*/
	int     lastfile;
	char    str[128];       /* ��ப� ��� �ନ஢���� �뢮�� */
	WINDOW  *win=panels[outpanel].win;
	char    *ptr;
	short   dirlen;         /*����� ����� ��⠫���*/
	short   maxlen;         /* ���� ����� �뢮������ ����� ��⠫���*/
	char    tmp[128];       /* �뢮����� ���� ��� ⥪�饣� ��⠫��� � ��� ��.�६.��.*/
	char    ftch;           /* ᨬ��� ������祭�� ⨯� 䠩��*/
	int     nf=0;
	short   x;
	int     i;
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
	short   fieldsize;      /* ࠧ��� ���� */
	chtype  attrib;         /* ���᪠ */
	short   size;           /* ��� ���� ������⢠ ����� � ������� */
	ushort  mode;
	int     files_in_col;   /* ������⢮ 䠩��� � ������� */
	int     stop;           /* �ਧ��� �४�饭�� �뢮�� �� ��६�饭�� 㪠��⥫� */
/*==========================================================================*/

if ( dwidth && outpanel!=cpanel ) return; /* �।���饭�� �뢮�� ����⨢��� ������ ������� �ਭ� */

if(panels[outpanel].on==3){
  size=panels[outpanel].cols/3;
  if(size>OUTNAMELEN+3 || size<8) size = OUTNAMELEN+3;
  files_in_col=panels[outpanel].lines-panels[outpanel].lnused;
  lastfile = files_on_page(outpanel)+panels[outpanel].firstfile-1;
  if(panels[outpanel].nfiles<=lastfile) lastfile=panels[outpanel].nfiles-1;
  panels[outpanel].lastfile=lastfile;

  if(pred_file<0){ /* �뢮� ������ 楫���� */
    mywbkgd(win, MAIN_PANEL_ATTR);
    wattrset(win, MAIN_PANEL_BOX_ATTR);
    box(win, 0,0);
  }

  if(pred_file<0){ /* �뢮� ������ 楫���� */
    /* ��������� */
    strcpy(str, " Brief ");
    if(panels[outpanel].filter_read[0] || panels[outpanel].filter_noread[0]){
       /* ��⠭����� 䨫��� */
       strcat(str, "(Filter) ");
    }
    wattrset(win, MAIN_PANEL_TOP_ATTR);
    wmove(win, 1, panels[outpanel].cols/2 - strlen(str)/2);
    waddstr(win, str);

    /* ���⨪���� ����� ࠧ������� �������*/
    wattrset(win, MAIN_PANEL_BOX_ATTR);
    for(i=1; i<=panels[outpanel].cols/size-1; i++){
      wmove(win, 2, i*size);
      wvline(win, 0, panels[outpanel].lines-4);
    }

    /* ��ਧ��⠫쭠� ����� status-��ப� */
    wmove(win, panels[outpanel].lines-3, 1);
    whline(win, 0, panels[outpanel].cols-2);

    if(clock_need)
      {
	maxlen=panels[outpanel].cols-11;
      }else{
	maxlen=panels[outpanel].cols-2;
      }
    name_to_out(panels[outpanel].full_dir, tmp, maxlen);

    if(outpanel==cpanel)
      { /* ⥪��� ������ */
	wattrset(win, MAIN_PANEL_DIR_ATTR);
      }else{
	wattrset(win, MAIN_PANEL_ATTR);
      }
    dirlen=strlen(tmp);
    wmove(win, 0, (panels[outpanel].cols-dirlen)/2);
    waddstr(win, tmp);
  } /* if(pred_file... */

  wattrset(win, MAIN_PANEL_ATTR);

  if(pred_file<0)
    { /* �뢮� ������ 楫���� */
      nfile=panels[outpanel].firstfile;
    }else{ /* ⮫쪮 ��६�饭�� 㪠��⥫� */
      nfile=pred_file;
    }

  item= &panels[outpanel].itembuf[nfile];
  stop=0;
  for(; nfile<=lastfile && stop==0; nfile++,item++){
    nf=nfile-panels[outpanel].firstfile;

    wmove(win, FIRST_STR+nf%files_in_col, nf/files_in_col*size+1);

    /* FIELD_TYPE: -------------- ���� ⨯� 䠩�� ----------------*/
    switch(filtype(item->mode))
      {
	case FTYPE_DIR:
	  str[0]='/';
	break;

	case FTYPE_CHR:
	  str[0]='$';
	break;

	case FTYPE_BLK:
	  str[0]='#';
	break;

	case FTYPE_FIFO:
	  str[0]='|';
	break;

	case FTYPE_NAM:
	  str[0]='~';
	break;

	case FTYPE_SLN:
	  str[0]='>';
	break;

	case FTYPE_REG:
	  if(item->nlink>1)
	    {
	      str[0]='L';
	    }else{
	      str[0]=' ';
	    }
	break;

	case FTYPE_UNKNOWN:
	  str[0]='?';
	break;

	case FTYPE_SOFIFO:
	  str[0]='=';
	break;

	default:
	  endwin();
	  printf("Programmer's error in refr_panel()\n");
	  exit(-1);
	break;
      }

    if((nfile==panels[outpanel].curfile) && (outpanel==cpanel))
      { /* ����騩 䠩� �� ⥪�饩 ������*/
	if((item->attrs) & MARKED)
	  { /* 䠩� �⬥祭 */
	    attrib=MAIN_PANEL_OUTL_PTR_ATTR;
	  }else{ /* �� �⬥祭 */
	    attrib=MAIN_PANEL_PTR_ATTR;
	  }
      }else{
	if((item->attrs) & MARKED)
	  { /* 䠩� �⬥祭 */
	    attrib=MAIN_PANEL_OUTL_ATTR;
	  }else{ /* �� �⬥祭 */
	    attrib=MAIN_PANEL_ATTR;
	  }
      }

    /* case FIELD_MARK: ------- �㡫�஢���� �⬥⪨ ᨬ����� ---------*/
    str[1]= (item->attrs & MARKED) ? '+' : ' ';
    str[2]='\0';
    wattrset(win, MAIN_PANEL_ATTR);
    waddstr(win, str);

    /* FIELD_NAME: ---------------- ��� 䠩�� --------------------*/
    str[size-3]='\0';
    strncpy(str, panels[outpanel].names+item->nname, size-2);

    if(str[size-3])
      { /* ������� ��� 䠩��, �� ���� ����.����� ��㥬 '>' */
	str[size-4]='>';
	str[size-3]='\0';
      }else{
	for(ptr=str; *ptr; ptr++);
	for(;ptr != &str[size-3]; ptr++) *ptr=' ';
      }

    for(i=0; str[i]; i++){
      if(((unsigned char*)str)[i]<' ') str[i]='?';
    }
    wattrset(win, attrib);
    waddstr(win, str);

    if(pred_file>=0){ /* ⮫쪮 ��६�饭�� 㪠��⥫�*/
      if(nfile==pred_file && cpanel==outpanel)
	{ /* ᥩ�� ����襭� �뤥����� �।��饣� 䠩��*/
	  nfile=panels[outpanel].curfile-1; /* �.�. � ���� 横�� nfile++...*/
	  item= &panels[outpanel].itembuf[nfile];
	}else{ /* ����祭� �뤥����� ⥪�饣� 䠩��*/
	  stop=1;
	}
    }
  } /* for �� 䠩��� */

  if(panels[outpanel].selected)
    { /* ���� �⬥祭�� 䠩��*/
      wmove(win, panels[outpanel].lines-2, 1);
      wattrset(win, MAIN_PANEL_BOT_OUT_ATTR);
      /* ������ ����� ��ப�, ���������� �᫠��*/
      sprintf(tmp,"%ld%d", panels[outpanel].size_sel, panels[outpanel].selected);
      maxlen=strlen(tmp);
      if(maxlen<=10)
	{ /* ࠧ��� ��襬 � �����*/
	  devt_str(tmp, (dev_data)panels[outpanel].size_sel);
	  wprintw(win, "%s bytes in %d selected file%c",
		      tmp,
		      panels[outpanel].selected,
		      (panels[outpanel].selected==1)?' ':'s');
	}else  if(maxlen<=13) {
	  devt_str(tmp, (dev_data)panels[outpanel].size_sel/1024);
	  wprintw(win, "%sK bytes in %d selected file%c",
		      tmp,
		      panels[outpanel].selected,
		      (panels[outpanel].selected==1)?' ':'s');

	}else{
	  devt_str(tmp, (dev_data)panels[outpanel].size_sel/(1024L*1024L));
	  wprintw(win, "%sM bytes in %d selected file%c",
		      tmp,
		      panels[outpanel].selected,
		      (panels[outpanel].selected==1)?' ':'s');
      }
    }else{ /* �⬥祭��� ���, �뢥�� ��� ⥪�饣� 䠩�� */
      size=panels[outpanel].cols-4;
      if(size>sizeof(str)-1) size=sizeof(str)-1;
      strncpy(str,
	      panels[outpanel].names+
		panels[outpanel].itembuf[panels[outpanel].curfile].nname,
	      size);
      str[size]='\0';
      for(i=0; str[i]; i++);
      while(i<size) str[i++] = ' ';
      wmove(win, panels[outpanel].lines-2, 2);
      wattrset(win, MAIN_PANEL_ATTR);
      waddstr(win, str);
    }
  if(panels[1-outpanel].on==2 && pred_file<0) {
    /* �뢮� � ��⨢��������� ���� ���ଠ樨 � 䠩����� ��⥬�*/
    show_inf();
  }
}


pred_file=(-1);
}
