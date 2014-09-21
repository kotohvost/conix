#include "defs.h"
#include <unistd.h>

#define SHIFT   2       /* сдвиг для вывода бестолковой информации */
/****************************************************************************/
				show_inf()
/*              Вывод в противоположном окне информации о файловой системе
*****************************************************************************/
{
extern  int     outpanel;       /*обрабатываемая панель */
extern  struct  panel   panels[];
extern  chtype  atr[];          /* раскраска */
extern  char    shrt_title[];
	WINDOW  *win=panels[1-outpanel].win;
	short   y;
	char    tmp[100];       /* строка для преобразований */
	short   len;
	short   maxy=panels[outpanel].lines-1;
	int     fldnmlen;       /* размер места для числа */
	dev_data tmpnum;        /* для расчета больших значений */

/*==========================================================================*/
mywbkgd(win, MAIN_PANEL_ATTR);
wattrset(win, MAIN_PANEL_BOX_ATTR);
box(win, 0,0);

wattrset(win, MAIN_PANEL_ATTR);

wmove(win, 0, (panels[outpanel].cols-6)/2);
waddstr(win, " Info ");

y=1;

/*------------  Информация о CONIX --------------*/
wmove(win, y, (panels[outpanel].cols-strlen(shrt_title))/2);
waddstr(win, shrt_title);
y++;

wmove(win, y, (panels[outpanel].cols-strlen(__DATE__)-strlen(__TIME__)-1)/2);
waddstr(win, __DATE__);
waddch(win, ' ');
waddstr(win, __TIME__);
y++;

if(y+1<maxy){
  /*------------------*/
  wmove(win, y, 1);
  whline(win, 0, panels[outpanel].cols-2);
  y++;
}

/*------------ Размер диска ------------*/
y++;
if(y<maxy && panels[outpanel].total != -1){
  mknumber(tmp, panels[outpanel].cols-4-22, panels[outpanel].total);
  len=strlen(tmp)+22;
  wmove(win, y, (panels[outpanel].cols-len)/2);
  waddstr(win, tmp);
  waddstr(win, " bytes total on device");
  y++;
}

/*------------ Размер доступного пространства ------------*/
if(y<maxy && panels[outpanel].avail != -1){
  mknumber(tmp, panels[outpanel].cols-4-16, panels[outpanel].avail);
  len=strlen(tmp)+16;
  wmove(win, y, (panels[outpanel].cols-len)/2);
  waddstr(win, tmp);
  waddstr(win, " bytes available");
  y++;
}

y++;
if(y+1<maxy){
  /*------------------*/
  wmove(win, y, 1);
  whline(win, 0, panels[outpanel].cols-2);
  y++;
}

/*---------  размер места для числа ---------*/
fldnmlen=panels[outpanel].cols-23-4;

/*------------ Размер свободного пространства ------------*/
y++;
if(y<maxy && panels[outpanel].free != -1){
  mknumber(tmp, fldnmlen, panels[outpanel].free);
  wmove(win, y, SHIFT);
  waddstr(win, "Total free:      ");
  waddstr(win, tmp);
  waddstr(win, " bytes");
  y++;
}

/*------------ Количество i-nodes ------------*/
if(y<maxy && panels[outpanel].itotal != -1){
  mknumber(tmp, fldnmlen+6, panels[outpanel].itotal);
  wmove(win, y, SHIFT);
  waddstr(win, "Total i-nodes:   ");
  waddstr(win, tmp);
  y++;
}

/*------------ Количество доступных i-nodes ------------*/
if(y<maxy && panels[outpanel].iavail != -1){
  mknumber(tmp, fldnmlen+6, panels[outpanel].iavail);
  wmove(win, y, SHIFT);
  waddstr(win, "Avail. i-nodes:  ");
  waddstr(win, tmp);
  y++;
}

/*------------ Количество свободных i-nodes ------------*/
if(y<maxy && panels[outpanel].ifree != -1){
  mknumber(tmp, fldnmlen+6, panels[outpanel].ifree);
  wmove(win, y, SHIFT);
  waddstr(win, "Free i-nodes:    ");
  waddstr(win, tmp);
  y++;
}

/*------------ Размер блока ------------*/
if(y<maxy && panels[outpanel].bs != -1){
  mknumber(tmp, fldnmlen, panels[outpanel].bs);
  wmove(win, y, SHIFT);
  waddstr(win, "Block size:      ");

  waddstr(win, tmp);
  waddstr(win, " bytes");
  y++;
}

/*------------ Размер фрагмента ------------*/
if(y<maxy && panels[outpanel].fragm != -1){
  mknumber(tmp, fldnmlen, panels[outpanel].fragm);
  wmove(win, y, SHIFT);
  waddstr(win, "Fragment size:   ");
  waddstr(win, tmp);
  waddstr(win, " bytes");
  y++;
}

#if defined( __svr4__ ) || defined( __SVR4 )
/*------------ Физическая память ------------*/
if(y<maxy){
  tmpnum  = sysconf(_SC_PHYS_PAGES);
  tmpnum *= sysconf(_SC_PAGESIZE);
  mknumber(tmp, fldnmlen, tmpnum);
  wmove(win, y, SHIFT);
  waddstr(win, "Physical memory: ");
  waddstr(win, tmp);
  waddstr(win, " bytes");
  y++;
}

/*------------ Свободная память ------------*/
if(y<maxy){
  tmpnum  = sysconf(_SC_AVPHYS_PAGES);
  tmpnum *= sysconf(_SC_PAGESIZE);
  mknumber(tmp, fldnmlen, tmpnum);
  wmove(win, y, SHIFT);
  waddstr(win, "Avail. physical: ");
  waddstr(win, tmp);
  waddstr(win, " bytes");
  y++;
}
#elif defined( FREEBSD )
/*------------ Физическая память ------------*/

/* Глюкатень, блин! Не компилится с <sys/sysctl.h>, вытащил из него, Sorry */

#define CTL_HW          6               /* generic cpu/io */
#define HW_PHYSMEM       5              /* int: total memory */

if(y<maxy){
  int mib[2];
  size_t len = sizeof(tmpnum);

  tmpnum = -1;
  mib[0] = CTL_HW;
  mib[1] = HW_PHYSMEM;
  sysctl(mib, 2, &tmpnum, &len, NULL, 0);

  if( tmpnum != -1 ){
    mknumber(tmp, fldnmlen, tmpnum);
    wmove(win, y, SHIFT);
    waddstr(win, "Physical memory: ");
    waddstr(win, tmp);
    waddstr(win, " bytes");
    y++;
  }
}
#endif

y++;
if(y+1<maxy){
  /*------------------*/
  wmove(win, y, 1);
  whline(win, 0, panels[outpanel].cols-2);
  y++;
}

/*------------ Тип файловой системы  ------------*/
if(y<maxy && panels[outpanel].fstype[0]){
  len=strlen(panels[outpanel].fstype)+14;
  wmove(win, y, (panels[outpanel].cols-len)/2);
  waddstr(win, "FileSys type: ");
  waddstr(win, panels[outpanel].fstype);
  y++;
}

/*------------ Наименование файловой системы  ------------*/
if(y<maxy && panels[outpanel].fsname[0]){
  len=strlen(panels[outpanel].fsname)+14;
  wmove(win, y, (panels[outpanel].cols-len)/2);
  waddstr(win, "FileSys name: ");
  waddstr(win, panels[outpanel].fsname);
  y++;
}

/* wrefresh(win); */
}

