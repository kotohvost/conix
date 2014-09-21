#include "defs.h"
#define SETCHECK(var, num) (win59.txts[num].txt[0]= (var) ? 'x' : ' ')
#define GETCHECK(var, num) ((var)= win59.txts[num].txt[0]=='x' ? 1 : 0)
/****************************************************************************/
				config()
/*                      Настройка конфигурации
*****************************************************************************/
{
extern  struct  win win59;      /* запрос задания конфигурации */
extern  char    cmp_exist;      /* признак сравнения по наличию */
extern  char    cmp_time;       /* признак сравнения по времени */
extern  char    cmp_size;       /* признак сравнения по размеру */
extern  char    cmp_mode;       /* признак сравнения по доступу */
extern  char    cmp_body;       /* признак сравнения по содержимому */
extern  char    cmp_nlink;      /* признак сравнения по кол-ву ссылок */
/*==========================================================================*/

SETCHECK( cmp_exist , 7 );
SETCHECK( cmp_time  , 8 );
SETCHECK( cmp_size  , 9 );
SETCHECK( cmp_mode  , 10);
SETCHECK( cmp_body  , 11);
SETCHECK( cmp_nlink , 12);

switch(ask(&win59, 7, VERT))
  {
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13: /* подтверждение */
      GETCHECK( cmp_exist , 7 );
      GETCHECK( cmp_time  , 8 );
      GETCHECK( cmp_size  , 9 );
      GETCHECK( cmp_mode  , 10);
      GETCHECK( cmp_body  , 11);
      GETCHECK( cmp_nlink , 12);
    break;
  }
}
