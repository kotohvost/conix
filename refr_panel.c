#include "defs.h"
#include <time.h>
#include <sys/stat.h>
#define FIRST_STR 2
/****************************************************************************/
			refr_panel()
/*              �뢮� �� �࠭ ������ outpanel
*****************************************************************************/
{
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  int     cpanel;
extern  struct  panel   panels[];
extern  chtype  atr[];          /* ���᪠ */
extern struct fields fields[];  /* �ࠪ���⨪� ��������� ����� �뢮��*/
extern  short   pred_file;      /* ����� ��।��饣� 䠩�� ��� refr_panel()*/
extern  int     clock_need;     /* �ਧ��� �뢮�� �६��� 0-��� 1-�� */
extern  int     dwidth;         /* �ਧ��� ������� ������� �ਭ� */
	int     nfile=0;        /* ����� ⥪�饣� 䠩��*/
	int     lastfile;
	char    str[128];       /* ��ப� ��� �ନ஢���� �뢮�� */
	int     nstr=FIRST_STR;
	WINDOW  *win=panels[outpanel].win;
	char    *ptr;
	short   dirlen;         /*����� ����� ��⠫���*/
	short   maxlen;         /* ���� ����� �뢮������ ����� ��⠫���*/
	char    tmp[128];       /* �뢮����� ���� ��� ⥪�饣� ��⠫��� � ��� ��.�६.��.*/
	char    ftch;           /* ᨬ��� ������祭�� ⨯� 䠩��*/
	int     nfield;
	int     nf;
	short   x;
	int     i;
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
 struct tm      *tm;            /* ��� ࠡ��� � ��⮩/�६���� */
	short   fieldsize;      /* ࠧ��� ���� */
	chtype  attrib;         /* ���᪠ */
	short   size;           /* ��� ���� ������⢠ ����� � ������� */
	short   save_size;      /* ��� ��࠭���� ࠧ��� ���� ����� */
	short   outfields[MAXFIELDS+1];
	short   name_count;     /* ������⢮ ����� � ������ (�� ��襬�==1)*/
	short   name_num;       /* ����� ����� fields[] � ������ */
	ushort  mode;
	int     stop;           /* �ਧ��� �४�饭�� �뢮�� �� ��६�饭�� 㪠��⥫� */
/*==========================================================================*/

if ( dwidth && outpanel!=cpanel ) return; /* �।���饭�� �뢮�� ����⨢��� ������ ������� �ਭ� */

if(panels[outpanel].on==1)
  {

    lastfile=files_on_page(outpanel) + panels[outpanel].firstfile -1;
    if(panels[outpanel].nfiles<=lastfile) lastfile=panels[outpanel].nfiles-1;
    panels[outpanel].lastfile=lastfile;

    if(pred_file<0){ /* �뢮� ������ 楫���� */
      mywbkgd(win, MAIN_PANEL_ATTR);
      wattrset(win, MAIN_PANEL_BOX_ATTR);
      box(win, 0,0);
    }

    /* ������� ���� ������⢠ ���������� ����� */
    name_count=0;
    size=panels[outpanel].cols-2;
    for(nfield=0,i=0; nfield<MAXFIELDS; nfield++){
      nf=panels[outpanel].outfields[nfield];
      if(nf>=0 && ((fieldsize=fields[nf].fieldsize)<=size+1)){
	outfields[i]=nf;
	size-=fieldsize;
	if(fields[nf].fieldtype==FIELD_NAME){
	  name_count++;
	  name_num=nf;
	}
	i++;
      }
    }
    outfields[i]=(-1);

    save_size=fields[name_num].fieldsize;
    if(name_count && size/name_count>=0){
      /* � ���� ����� ����� �������� ᨬ����� */
      fields[name_num].fieldsize+=(size+1)/name_count;
    }

    if(pred_file<0){ /* �뢮� ������ 楫���� */
      /* ��������� */
      for(x=1,nfield=0; outfields[nfield]>=0; nfield++){
	nf=outfields[nfield];
	fieldsize=fields[nf].fieldsize;

	if(x+fieldsize>3 && fieldsize>2 && outfields[nfield+1]>=0){ /* ���ᮢ��� ࠧ����⥫� �����*/
	  wmove(win, 2, x+fieldsize-1);
	  wattrset(win, MAIN_PANEL_BOX_ATTR);
	  wvline(win, 0, panels[outpanel].lines-4);
	}

	wmove(win, 1, x);
	wattrset(win, MAIN_PANEL_TOP_ATTR);
	waddstr(win, fields[nf].fieldname);
	if(fields[nf].fieldtype==FIELD_NAME &&
	   (panels[outpanel].filter_read[0] || panels[outpanel].filter_noread[0])){
	   /* ��⠭����� 䨫��� */
	   waddstr(win, "(Filter)");
	}
	x+=fieldsize;
      }


      wattrset(win, MAIN_PANEL_BOX_ATTR);
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
	nstr+=pred_file-panels[outpanel].firstfile;
      }

    item= &panels[outpanel].itembuf[nfile];
    stop=0;

    for(; nfile<=lastfile && stop==0; nfile++,nstr++,item++){
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

      tm=NULL;

      for(x=1,nfield=0; outfields[nfield]>=0; nfield++){
	nf=outfields[nfield];
	fieldsize=fields[nf].fieldsize;
	if(outfields[nfield+1]<0 && fieldsize==1)
	  { /* ��᫥���� ���� - 1 ᨬ��� (��� ࠧ�࠭��⥫�) */
	    wmove(win, nstr, x-1); /* ����㥬 ��� �� ࠧ�࠭��⥫� �।�.*/
	  }else{
	    wmove(win, nstr, x);
	  }

	wattrset(win, attrib);
	switch(fields[nf].fieldtype)
	  {
	    case FIELD_MARK: /*------- �㡫�஢���� �⬥⪨ ᨬ����� ---------*/
	      wattrset(win, MAIN_PANEL_ATTR);
	      str[0]= (item->attrs & MARKED) ? '+' : ' ';
	      str[1]='\0';
	    break;

	    case FIELD_NAME: /*---------------- ��� 䠩�� --------------------*/
	      str[fieldsize-1]='\0';
	      strncpy(str, panels[outpanel].names+item->nname, fieldsize);

	      if(str[fieldsize-1])
		{ /* ������� ��� 䠩��, �� ���� ����.����� ��㥬 '>' */
		  str[fieldsize-2]='>';
		  str[fieldsize-1]='\0';
		}else{
		  for(ptr=str; *ptr; ptr++);
		  for(;ptr!=&str[fieldsize-1]; ptr++) *ptr=' ';
		}

	      for(i=0; str[i]; i++){
		if(((unsigned char*)str)[i]<' ') str[i]='?';
	      }
	    break;

	    case FIELD_TYPE: /*-------------- ���� ⨯� 䠩�� ----------------*/
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
	      str[1]='\0';
	      wattrset(win, MAIN_PANEL_ATTR);
	    break;

	    case FIELD_SIZE: /*-------------- ���� ࠧ��� 䠩�� -------------*/
	      sprintf(str, "%ld", item->size);
	      if(strlen(str)>fieldsize-1){ /* ���஡����� � Kb */
		sprintf(str, "%ldK", item->size/1024);
	      }
	      if(strlen(str)>fieldsize-1){ /* ���஡����� � Mb */
		sprintf(str, "%ldM", item->size/1024/1024);
	      }

	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_DATE:
	      if(!tm) tm=localtime(&(item->mtime));
	      while(tm->tm_year >= 100) tm->tm_year-=100; /* � ���� ������� �� 2000 ����?*/
	      sprintf(str, "%02d.%02d.%02d", tm->tm_mday, tm->tm_mon+1, tm->tm_year);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_TIME:
	      if(!tm) tm=localtime(&(item->mtime));
	      sprintf(str, "%02d:%02d", tm->tm_hour, tm->tm_min);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_INUM:
	      sprintf(str, "%d", item->inum);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_MODE:
	      mode=item->mode;
	      ptr=str;

	      /*-------- Owner ---------*/
	      *ptr++= (mode&S_IRUSR) ? 'r' : '-';
	      *ptr++= (mode&S_IWUSR) ? 'w' : '-';

	      if( (mode&S_ISUID) && (mode&S_IXUSR) ){
		*ptr++= 's';
	      }else if(mode&S_ISUID){
		*ptr++= 'S';
	      }else if(mode&S_IXUSR){
		*ptr++= 'x';
	      }else{
		*ptr++= '-';
	      }

	      /*-------- Group ---------*/
	      *ptr++= (mode&S_IRGRP) ? 'r' : '-';
	      *ptr++= (mode&S_IWGRP) ? 'w' : '-';

	      if( (mode&S_ISGID) && (mode&S_IXGRP) ){
		*ptr++= 's';
	      }else if(mode&S_ISGID){
		*ptr++= 'S';
	      }else if(mode&S_IXGRP){
		*ptr++= 'x';
	      }else{
		*ptr++= '-';
	      }

	      /*-------- Others ---------*/
	      *ptr++= (mode&S_IROTH) ? 'r' : '-';
	      *ptr++= (mode&S_IWOTH) ? 'w' : '-';

	      if( (mode&S_ISVTX) && (mode&S_IXOTH) ){
		*ptr++= 't';
	      }else if(mode&S_ISVTX){
		*ptr++= 'T';
	      }else if(mode&S_IXOTH){
		*ptr++= 'x';
	      }else{
		*ptr++= '-';
	      }

	      *ptr='\0';

	    break;

	    case FIELD_OWNR:
	      sprintf(str, "%d", item->owner);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_GRP:
	      sprintf(str, "%d", item->group);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	    case FIELD_NLNK:
	      sprintf(str, "%d", item->nlink);
	      if(strlen(str)>fieldsize-1) strcpy(str, "Large!");
	      for(i=0; str[i]; i++);
	      while(i<fieldsize-1) str[i++]=' ';
	      str[i]='\0';
	    break;

	  } /* switch */

	/* �뢮� ��ନ஢����� ��ப� ���� */
	waddstr(win, str);
	if(nfile==panels[outpanel].curfile && panels[outpanel].selected==0){
	  /* �㡫�஢���� ⥪�饣� 䠩�� � status-��ப�*/
	  if(outfields[nfield+1]<0 && fieldsize==1)
	    { /* ��᫥���� ���� - 1 ᨬ��� (��� ࠧ�࠭��⥫�) */
	      wmove(win, panels[outpanel].lines-2, x-1); /* ����㥬 ��� �� ࠧ�࠭��⥫� �।�.*/
	    }else{
	      wmove(win, panels[outpanel].lines-2, x);
	    }
	  wattrset(win, MAIN_PANEL_ATTR);
	  waddstr(win, str);
	}

	x+=fieldsize;

      } /* for �� ���� */
      if(pred_file>=0){ /* ⮫쪮 ��६�饭�� 㪠��⥫�*/
	if(nfile==pred_file && cpanel==outpanel)
	  { /* ᥩ�� ����襭� �뤥����� �।��饣� 䠩��*/
	    nfile=panels[outpanel].curfile-1; /* �.�. � ���� 横�� nfile++...*/
	    item= &panels[outpanel].itembuf[nfile];
	    nstr=FIRST_STR+nfile-panels[outpanel].firstfile;
	  }else{ /* ����祭� �뤥����� ⥪�饣� 䠩��*/
	    stop=1;
	  }
      }
    } /* for �� 䠩��� */

    if(panels[outpanel].selected){ /* ���� �⬥祭�� 䠩��*/
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
    }
    fields[name_num].fieldsize=save_size; /* ࠧ��� ��� ���� �������*/

    if(panels[1-outpanel].on==2 && pred_file<0) {
      /* �뢮� � ��⨢��������� ���� ���ଠ樨 � 䠩����� ��⥬�*/
      show_inf();
    }

  }else if(panels[outpanel].on==3){ /* �뢮� � �ଠ� Brief */
    refr_brief();
  }


pred_file=(-1);
}
