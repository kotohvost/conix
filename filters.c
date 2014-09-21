#include "defs.h"
#include <fcntl.h>
/****************************************************************************/
				filters()
/*                      Установка фильтра чтения каталогов
/****************************************************************************/
{
extern  int     cpanel;                 /* текущая панель */
extern  struct  panel   panels[];
extern  int     outpanel;               /*обрабатываемая панель */
extern  struct  win     win56;          /* запрос задания фильтра */
extern  WINDOW  *save_scr;      /* окно для сохранения экрана */
	char    str_read[FILTERLEN];    /* строка для задания фильтра (читать)*/
	char    str_noread[FILTERLEN];  /* строка для задания фильтра (не читать)*/
 struct inp_lst inp_lst[3];             /* список ввода для inp_menu()*/
/*==========================================================================*/
inp_lst[0].str=str_read;
/* inp_lst[0].fld_len=...;  выставит inp_menu()*/
inp_lst[0].str_len=sizeof(str_read);
inp_lst[0].npunkt=2;
inp_lst[1].str=str_noread;
/* inp_lst[1].fld_len=...;  выставит inp_menu()*/
inp_lst[1].str_len=sizeof(str_noread);
inp_lst[1].npunkt=4;
inp_lst[2].str=NULL; /* признак конца списка для inp_menu()*/

strcpy(str_read,   panels[cpanel].filter_read  );
strcpy(str_noread, panels[cpanel].filter_noread);

switch(inp_menu(inp_lst, &win56, 2, HOR|VERT))
  {
    case 2:
    case 4:
    case 5:
      strcpy(panels[cpanel].filter_read  , str_read  );
      strcpy(panels[cpanel].filter_noread, str_noread);
      outpanel=cpanel;
      if(panels[cpanel].on<0)panels[cpanel].on *= -1;
      overwrite(save_scr, stdscr);
#ifdef FREEBSD
      touchwin(stdscr);
#endif
      if(chdir(panels[cpanel].full_dir)) beep();
      read_to_panel();
      overwrite(stdscr, save_scr); /* для обмана программ верхнего уровня */
    break;
  }
}
