#include "defs.h"
/****************************************************************************/
				refr_inp()
/*              Вывод в окно inp_win (части) строки inp_str
*****************************************************************************/
{
extern  WINDOW  *inp_win;       /* окно ввода команд */
extern  char    *inp_str;       /* строка для ввода команд */
extern  size_t  inp_len;        /* место, выделенное для inp_str*/
extern  int     inp_first;      /* номер первого выведенного символа команды */
extern  int     inp_cur;        /* номер текущего символа команды */
extern  char    mach_name[];    /* название машины */
extern  char    user_name[];    /* имя пользователя */
extern  int     prompt;         /* тип приглашения:0-нет, 1-NodeName 2-User@Node*/
extern  uid_t   user_eid;       /* эффективный UserID или 0, если getuid()==0*/
extern  chtype  atr[];          /* раскраска */
	int     curr;           /*текущая координата x в окне вывода*/
	char    *ptr;
	int     prompt_size;    /* размер строки приглашения*/
/*==========================================================================*/
werase(inp_win);
wmove(inp_win, 0,0);
switch(prompt)
  {
    case 0: /* Short prompt */
      prompt_size=1;
    break;
    case 1: /* NodeName prompt */
      prompt_size=strlen(mach_name)+1;
      if(user_eid==0) wattrset(inp_win, SUPER_PROMPT_ATTR); /* индикация super-user*/
      waddstr(inp_win, mach_name);
      if(user_eid==0) wattrset(inp_win, A_NORMAL);
    break;
    case 2: /* User@Node prompt */
      prompt_size=strlen(user_name)+strlen(mach_name)+2;
      if(user_eid==0) wattrset(inp_win, SUPER_PROMPT_ATTR); /* индикация super-user*/
      waddstr(inp_win, user_name);
      if(user_eid==0) wattrset(inp_win, A_NORMAL);
      waddch( inp_win, '@');
      waddstr(inp_win, mach_name);
    break;
  }

if(inp_cur<=inp_first && inp_first)
  { /* надо сдвигать влево */
    if(inp_cur>=inp_win->_maxx-1-prompt_size)
      { /* текущая позиция не помещается в окне при нулевом сдвиге*/
	inp_first=inp_cur-1;
      }else{ /* помещается весь кусок слева от текущей позиции */
	inp_first=0;
      }
  }else{
    if(inp_cur>=inp_first+inp_win->_maxx-prompt_size) inp_first=inp_cur-(inp_win->_maxx-prompt_size)+1;
  }

if(inp_first)
  {
    waddch(inp_win,'<');
  }else{
    waddch(inp_win,'>');
  }
for(curr=0,ptr=&inp_str[inp_first]; curr < inp_win->_maxx-1-prompt_size &&
				    *ptr; curr++,ptr++){
  waddch(inp_win, *ptr);
}
wmove(inp_win, 0, inp_cur-inp_first+prompt_size);
}
