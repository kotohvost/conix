#include "defs.h"
/****************************************************************************/
				fileattrs()
/*                      ��⠭���� ०���� ����㯠 � [�⬥祭��] 䠩���
/****************************************************************************/
{
extern  int     cpanel;         /* ⥪��� ������ */
extern  struct  panel   panels[];
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  win     win30;  /* ���� �訡�� ��������� ०���*/
extern  struct  win     win31;  /* ���� ����� ��������� ०��� */
extern  struct  win     win32;  /* ���� ����� ��������� ०��� */
	char    str[30];
	int     stop=0;         /* �ਧ��� �४�饭�� 㤠����� */
	int     say_error=1;    /* ᮮ���� �� �訡��� */
	char    *name;          /* ��� 䠩�� */
 struct itembuf *item;          /* ��� ��ࠡ�⪨ ��࠭���� 䠩��*/
	int     nfile;          /* ����� ⥪�饣� 䠩��*/
	int     nfile_s;        /* ����� ⥪�饣� 䠩��*/
	int     nfirst;         /* ����� ��ࢮ�� �뢮������ 䠩��*/
	ushort  mode_set;       /* ०�� ����㯠 (set) */
	ushort  mode_clr;       /* ०�� ����㯠 (clear) */
	ushort  mode;           /* ०�� ����㯠 (�����) */
/*==========================================================================*/
nfile_s=panels[cpanel].curfile;
nfirst=panels[cpanel].firstfile;

if(panels[cpanel].selected>1)
  {
   sprintf(str, "%d files.", panels[cpanel].selected);
   win31.txts[1].txt = str;
   win31.txts[1].x=(win31.cols-strlen(str))/2;
   mode_clr=mode_set=0;
   if(set_clear(&win31, &mode_set, &mode_clr)==6){

     for( nfile=0,item= panels[cpanel].itembuf;
	  stop==0 && panels[cpanel].selected && nfile<=panels[cpanel].nfiles;
	  nfile++, item++){
	if(item->attrs & MARKED){ /* ��࠭�� 䠩� */
	  mode= (item->mode | mode_set) & (~mode_clr);

	  name=panels[cpanel].names+item->nname;
	  if(strlen(name) >= sizeof(str))
	    { /* ������ ��� �� ����頥��� */
	      strcpy(str,"...");
	      strncpy(str+3, name, sizeof(str)-3);
	      str[sizeof(str)-1]='\0';
	    }else{ /* ��� 䠩�� ����頥��� 楫����*/
	      strcpy(str,name);
	    }

	  if(chmod(name, mode))
	    { /* �訡�� ��������� ०��� */
	      if(say_error){
		win30.txts[1].x=(win30.cols-strlen(str))/2;
		win30.txts[1].txt=str;
		switch(ask(&win30,2,HOR))
		  {
		    default:
		    case 2: /* OK */
		    break;

		    case 3: /* Silently - �� ��訢��� (� nc �⮣� ���)*/
		      say_error=0;
		    break;

		    case 4: /* Cancel (� nc �⮣� ���)*/
		      stop=1;
		    break;
		  }
	      }
	    }else{ /* ०�� ����㯠 ��ଠ�쭮 ������� */
	      item->attrs ^= MARKED;
	      panels[cpanel].selected--;
	      panels[cpanel].size_sel-=item->size;
	    }
	} /* if MARKED */
     } /* for �� 䠩��� */
   }
  }else{/*----------- ��� �⬥祭��� 䠩��� ��� �⬥祭 1 䠩� -------------*/

   if(panels[cpanel].selected)
     { /* �⬥祭 1 䠩� */
       for( nfile=0,item= panels[cpanel].itembuf;
	    nfile<=panels[cpanel].nfiles && !(item->attrs & MARKED);
	    nfile++, item++);
     }else{ /* ��� �⬥祭��� */
       item= &panels[cpanel].itembuf[panels[cpanel].curfile];
     }
    /* ��ନ஢��� ��� 䠩�� ��� ����ᮢ*/
    name=panels[cpanel].names+item->nname;
    if(strlen(name) >= sizeof(str))
      { /* ������ ��� �� ����頥��� */
	strcpy(str,"...");
	strncpy(str+3, name, sizeof(str)-3);
	str[sizeof(str)-1]='\0';
      }else{ /* ��� 䠩�� ����頥��� 楫����*/
	strcpy(str,name);
      }

    win32.txts[1].txt = str;
    win32.txts[1].x=(win32.cols-strlen(str))/2;
    mode_set=item->mode;
    if(set_clear(&win32, &mode_set, NULL)==6){

      if(chmod(name, mode_set))
	{ /* �訡�� ��������� ०��� */
	  win30.txts[1].x=(win30.cols-strlen(str))/2;
	  win30.txts[1].txt=str;
	  ask(&win30,2,HOR);
	}
    }
  } /* if ... selected ... else */


outpanel=cpanel;
read_to_panel();  /* extern outpanel */
if(nfile_s<panels[cpanel].nfiles) {
  panels[cpanel].firstfile=nfirst;
  panels[cpanel].curfile=nfile_s;
  refr_panel();
}
wrefresh(panels[cpanel].win);
for(outpanel=0;outpanel<NPANELS;outpanel++){
  if(outpanel!=cpanel &&
     strcmp(panels[cpanel].full_dir,panels[outpanel].full_dir)==0){
    read_to_panel();
    if(panels[outpanel].on>0) wrefresh(panels[outpanel].win);
  }
}
}

