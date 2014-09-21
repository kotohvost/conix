#include <sys/types.h>
#include <time.h>
#include "defs.h"

/****************************************************************************/
			void    do_alarms()
/*                      обработчик сигнала alarm
*****************************************************************************/
{
extern  WINDOW  *clock_win;     /* окно вывода времени */
	time_t  cur_time;       /* текущее время */
	time_t  time();
 struct tm      *tm;            /* для работы с датой/временем */
/*==========================================================================*/
cur_time=time((time_t*)0);

tm=localtime(&cur_time);
wmove(clock_win, 0, 0);
if(cur_time & 1)
  {
    wprintw(clock_win, "%02d:%02d", tm->tm_hour, tm->tm_min);
  }else{
    wprintw(clock_win, "%02d %02d", tm->tm_hour, tm->tm_min);
  }

wrefresh(clock_win);
}
