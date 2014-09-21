#include <stdio.h>
#include "keys.h"

#ifndef MS_DOS
/* для UNIX-систем */
#define U       (unsigned char*)
#include <sgtty.h>
int     ospeed=B9600;           /*зачем-то нужно для t_tdecode, разбираться лень*/
	int     uc_;            /*=1,если на терм.только мален.буквы*/

	int     nctrl_=17;      /*количество команд в постоянной части  */
				/*(управляющие и т.п. Эти коды - в нач. */

struct  cmdkey  stringcom_[]=   /*соответствие строковых последователь- */
				/*ностей командам редактора             */
				/*элементы могут повторяться           */
	{
	/* общее число эл-тов stringcom = ncomm  */

	/* постоянная часть (число эл-тов=nctrl) */
	K_BACKSPACE,      U "\010",
	K_TAB,            U "\t",
	K_BTAB,           U "\002",  /*клавиша ^B*/
	K_DELETE,         U "\177",
	K_LF,             U "\n",
	K_CR,             U "\r",
	K_ESC,            U "\033\033",
	K_F1,             U "\0331",
	K_F2,             U "\0332",
	K_F3,             U "\0333",
	K_F4,             U "\0334",
	K_F5,             U "\0335",
	K_F6,             U "\0336",
	K_F7,             U "\0337",
	K_F8,             U "\0338",
	K_F9,             U "\0339",
	K_F0,             U "\0330",

	/* переменная часть */
	/* настраивается из termcap */
	K_BACKSPACE,      U "DC",
	K_DELETE,         U "kD",
	K_F1,             U "f1",
	K_F2,             U "f2",
	K_F3,             U "f3",
	K_F4,             U "f4",
	K_F5,             U "f5",
	K_F6,             U "f6",
	K_F7,             U "f7",
	K_F8,             U "f8",
	K_F9,             U "f9",
	K_F0,             U "f0",
	K_SR,             U "kr",
	K_SL,             U "kl",
	K_SD,             U "kd",
	K_SU,             U "ku",
	K_HOME,           U "kh",
	K_HOME,           U "Kh",
	K_END,            U "k1",
	K_END,            U "kE",
	K_BTAB,           U "BT",
	K_BTAB,           U "kB",
	K_PAGE_UP,        U "PU",
	K_PAGE_DN,        U "PD",
	K_PAGE_UP,        U "kP",
	K_PAGE_DN,        U "kN",
	K_INSERT,         U "kI",
	K_INSERT,         U "k4",
	K_SHIFT_HOME,     U "SH",
	K_SHIFT_SU,       U "Su",
	K_SHIFT_PU,       U "SU",
	K_SHIFT_SL,       U "Sl",
	K_SHIFT_SR,       U "Sr",
	K_SHIFT_END,      U "SE",
	K_SHIFT_SD,       U "Sd",
	K_SHIFT_PD,       U "SD",
	K_SHIFT_INS,      U "SI",
	K_SHIFT_DEL,      U "SB",
	K_CTRL_HOME,      U "KH",
	K_CTRL_SU,        U "Ku",
	K_CTRL_PU,        U "KU",
	K_CTRL_SL,        U "Kl",
	K_CTRL_SR,        U "Kr",
	K_CTRL_END,       U "KE",
	K_CTRL_SD,        U "Kd",
	K_CTRL_PD,        U "KD",
	K_CTRL_INS,       U "KI",

	0,              U 0
	};

#else /* MS-DOS */

struct  cmdkey  stringcom_[]=    /*соответствие кодов клавиш _bios_keybrd (_NKEYBRD_READ) */
				/*командам */
	{
	K_BACKSPACE,      3592,
	K_TAB,            3849,
	K_BTAB,           3840,
	K_DELETE,         21472,
	K_ENTER,          7181,
	K_ENTER,          9226,
	K_ESC,            283,
	K_BACKSPACE,      8968,
	K_F1,             15104,
	K_F2,             15360,
	K_F3,             15616,
	K_F4,             15872,
	K_F5,             16128,
	K_F6,             16384,
	K_F7,             16640,
	K_F8,             16896,
	K_F9,             17152,
	K_F0,             17408,
	K_SR,             19936,
	K_SL,             19424,
	K_SD,             20704,
	K_SU,             18656,
	K_HOME,           18400,
	K_END,            20448,
	K_BTAB,           12290,
/*      K_BTAB,           ,     */
	K_PAGE_UP,        18912,
	K_PAGE_DN,        20960,
/*      K_PAGE_UP,        ,     */
/*      K_PAGE_DN,        ,     */
	K_INSERT,         21216,
	K_SHIFT_HOME,     -26880,
	K_SHIFT_SU,       -26624,
	K_SHIFT_PU,       -26362,
	K_SHIFT_SL,       -25856,
	K_SHIFT_SR,       -25344,
	K_SHIFT_END,      -24832,
	K_SHIFT_SD,       -24576,
	K_SHIFT_PD,       -24320,
	K_SHIFT_INS,      -24064,
	K_SHIFT_DEL,      -23808,
	K_CTRL_HOME,      30688,
	K_CTRL_SU,        -29216,
	K_CTRL_PU,        -31520,
	K_CTRL_SL,        29664,
	K_CTRL_SR,        29920,
	K_CTRL_END,       30176,
	K_CTRL_SD,        -28192,
	K_CTRL_PD,        30432,
	K_CTRL_INS,       -27936,
	0,              0
	};

#endif /* MS-DOS */
