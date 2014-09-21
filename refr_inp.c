#include "defs.h"
/****************************************************************************/
				refr_inp()
/*              �뢮� � ���� inp_win (���) ��ப� inp_str
*****************************************************************************/
{
extern  WINDOW  *inp_win;       /* ���� ����� ������ */
extern  char    *inp_str;       /* ��ப� ��� ����� ������ */
extern  size_t  inp_len;        /* ����, �뤥������ ��� inp_str*/
extern  int     inp_first;      /* ����� ��ࢮ�� �뢥������� ᨬ���� ������� */
extern  int     inp_cur;        /* ����� ⥪�饣� ᨬ���� ������� */
extern  char    mach_name[];    /* �������� ��設� */
extern  char    user_name[];    /* ��� ���짮��⥫� */
extern  int     prompt;         /* ⨯ �ਣ��襭��:0-���, 1-NodeName 2-User@Node*/
extern  uid_t   user_eid;       /* ��䥪⨢�� UserID ��� 0, �᫨ getuid()==0*/
extern  chtype  atr[];          /* ���᪠ */
	int     curr;           /*⥪��� ���न��� x � ���� �뢮��*/
	char    *ptr;
	int     prompt_size;    /* ࠧ��� ��ப� �ਣ��襭��*/
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
      if(user_eid==0) wattrset(inp_win, SUPER_PROMPT_ATTR); /* �������� super-user*/
      waddstr(inp_win, mach_name);
      if(user_eid==0) wattrset(inp_win, A_NORMAL);
    break;
    case 2: /* User@Node prompt */
      prompt_size=strlen(user_name)+strlen(mach_name)+2;
      if(user_eid==0) wattrset(inp_win, SUPER_PROMPT_ATTR); /* �������� super-user*/
      waddstr(inp_win, user_name);
      if(user_eid==0) wattrset(inp_win, A_NORMAL);
      waddch( inp_win, '@');
      waddstr(inp_win, mach_name);
    break;
  }

if(inp_cur<=inp_first && inp_first)
  { /* ���� ᤢ����� ����� */
    if(inp_cur>=inp_win->_maxx-1-prompt_size)
      { /* ⥪��� ������ �� ����頥��� � ���� �� �㫥��� ᤢ���*/
	inp_first=inp_cur-1;
      }else{ /* ����頥��� ���� ��᮪ ᫥�� �� ⥪�饩 ����樨 */
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
