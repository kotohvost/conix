/* результаты разбора полученных с клавиатурв команд */
#define PECH_SIMB       (1<<15) /* просто печатаемый символ*/
#define CMD_KEY         (1<<14) /* распознаваемая клавиша */
#define ERROR_KEY       (1<<13) /* неопознанная клавиша */
#define ALARMED         (1<<12) /* ожидание прервано по сигналу alarm */
#define SIMBOL          0377    /* маска символа */
#define SIMBTYPE        (PECH_SIMB | CMD_KEY | ERROR_KEY | ALARMED) /*маска для типа символа*/

#define K_F1             (1 |CMD_KEY)
#define K_F2             (2 |CMD_KEY)
#define K_F3             (3 |CMD_KEY)
#define K_F4             (4 |CMD_KEY)
#define K_F5             (5 |CMD_KEY)
#define K_F6             (6 |CMD_KEY)
#define K_F7             (7 |CMD_KEY)
#define K_F8             (8 |CMD_KEY)
#define K_F9             (9 |CMD_KEY)
#define K_F0             (10|CMD_KEY)
#define K_SR             (11|CMD_KEY)
#define K_SL             (12|CMD_KEY)
#define K_SD             (13|CMD_KEY)
#define K_SU             (14|CMD_KEY)
#define K_INSERT         (15|CMD_KEY)
#define K_BACKSPACE      (16|CMD_KEY)
#define K_SHIFT_HOME     (17|CMD_KEY)
#define K_SHIFT_SU       (18|CMD_KEY)
#define K_SHIFT_PU       (19|CMD_KEY)
#define K_SHIFT_SL       (20|CMD_KEY)
#define K_SHIFT_SR       (21|CMD_KEY)
#define K_SHIFT_END      (22|CMD_KEY)
#define K_SHIFT_SD       (23|CMD_KEY)
#define K_SHIFT_PD       (24|CMD_KEY)
#define K_SHIFT_INS      (25|CMD_KEY)
#define K_CTRL_HOME      (26|CMD_KEY)
#define K_CTRL_SU        (27|CMD_KEY)
#define K_CTRL_PU        (28|CMD_KEY)
#define K_CTRL_SL        (29|CMD_KEY)
#define K_CTRL_SR        (30|CMD_KEY)
#define K_CTRL_END       (31|CMD_KEY)
#define K_CTRL_SD        (32|CMD_KEY)
#define K_CTRL_PD        (33|CMD_KEY)
#define K_CTRL_INS       (34|CMD_KEY)
#define K_TAB            (35|CMD_KEY)
#define K_ESC            (36|CMD_KEY)
#define K_DELETE         (37|CMD_KEY)
#define K_HOME           (38|CMD_KEY)
#define K_END            (39|CMD_KEY)
#define K_PAGE_UP        (40|CMD_KEY)
#define K_PAGE_DN        (41|CMD_KEY)
#define K_BTAB           (42|CMD_KEY)
#define K_CR             (43|CMD_KEY)
#define K_SHIFT_DEL      (44|CMD_KEY)
#define K_LF             (45|CMD_KEY)

#define my_isprint(c)    (((unsigned)(c))>=' ' && (c)!=0177 && ((unsigned)(c))<=0377)

/**************************** cmdkey ****************************************/
#ifndef MS_DOS
/* ------------- UNIX - системы ----------- */

struct  cmdkey                          /*соотв-е посл-стей кодов и команд */
		{
		short   command;        /*код кл. или ном.альт.по hot-key*/
       unsigned char    *string;        /*соотв-я клавише последовательность*/
		};

#else /* ---------- MS-DOS --------- */

struct  cmdkey                          /*соотв-е посл-стей кодов и команд */
		{
		int     command;        /*код кл. или ном.альт.по hot-key*/
       unsigned int     code;           /*код клавиши, выдаваемый _bios_keybrd (_NKEYBRD_READ)*/
		};
#endif
/****************************************************************************/
