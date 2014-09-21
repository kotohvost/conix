#ifdef NCURSES
/* В linux не удается средствами ncurses выставить все режимы */
#include <termios.h>
#include <unistd.h>
/****************************************************************************/
				my_raw()
/*              Выставление raw и т.п. режима (вместо raw из curses)
*****************************************************************************/
{
static  int     first=1;
static  struct termios ios;
/*==========================================================================*/
/*
 < NORAW
 brkint ignpar istrip icrnl ixon opost isig icanon
 ---
 > RAW
 -brkint -ignpar -istrip -icrnl -ixon -opost -isig -icanon
*/

if ( first ) {
  first = 0;
  tcgetattr ( 0, &ios );
  ios.c_iflag = ios.c_iflag & ~(BRKINT | IGNPAR | ISTRIP | ICRNL | IXON);
  ios.c_lflag = ios.c_lflag & ~(ISIG | ICANON | ECHO);
}

tcsetattr ( 0, TCSANOW, &ios );
}

#else /* -LINUX */

#include "../defs.h"
/****************************************************************************/
				my_raw()
/*              Выставление raw и т.п. режима
*****************************************************************************/
{
/*==========================================================================*/
 raw();
 cbreak();
 noecho();
}

#endif
