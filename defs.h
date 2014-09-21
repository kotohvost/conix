#ifdef NCURSES
#ifdef FREEBSD
#include <ncurses.h>
#define ulong unsigned long
#else
#include <ncurses/curses.h>
#define SYS_NMLN 256
#endif
#else
#include <curses.h>
#endif
#include "keyboard/keys.h"
#include <sys/types.h>
/************************************ Цвета *********************************/
#define BLACK       (short)COLOR_BLACK
#define BLUE        (short)COLOR_BLUE
#define GREEN       (short)COLOR_GREEN
#define CYAN        (short)COLOR_CYAN
#define RED         (short)COLOR_RED
#define MAGENTA     (short)COLOR_MAGENTA
#define BROWN       (short)COLOR_YELLOW
#define WHITE       (short)COLOR_WHITE

/* #define GRAY        8
 * #define LT_BLUE     9
 * #define LT_GREEN    10
 * #define LT_CYAN     11
 * #define LT_RED      12
 * #define LT_MAGENTA  13
 * #define YELLOW      14
 * #define HI_WHITE    15
 */
/********* Определения атрибутов отдельно расцвечиваемых элементов **********/
#define MAIN_PANEL_ATTR          (atr[0 ])   /* основная часть панели */
#define MAIN_PANEL_TOP_ATTR      (atr[1 ])   /* верхняя строка панели */
#define MAIN_PANEL_BOT_OUT_ATTR  (atr[2 ])   /* нижняя строка панели с отмеч.файлами*/
#define MAIN_PANEL_PTR_ATTR      (atr[3 ])   /* строка-указатель (не выделенный ф.)*/
#define MAIN_PANEL_OUTL_ATTR     (atr[4 ])   /* выделенный файл на осн.панели*/
#define MAIN_PANEL_OUTL_PTR_ATTR (atr[5 ])   /* строка-указатель (выделенный ф.)*/
#define MAIN_PANEL_BOX_ATTR      (atr[6 ])   /* рамка основной панели */
#define MAIN_PANEL_DIR_ATTR      (atr[7 ])   /* текущий каталог активной панели*/
#define BOTT_LINE_TEXT_ATTR      (atr[8 ])   /* текст нижней строки*/
#define BOTT_LINE_KEYS_ATTR      (atr[9 ])   /* номера клавиш в нижней строке*/
#define TOP_MENU_ATTR            (atr[10])   /* верхнее меню (неакт.пункты)*/
#define TOP_MENU_OUTL_ATTR       (atr[11])   /* верхнее меню (активный пункт)*/
#define POPDN_WIN_ATTR           (atr[12])   /* выпадающие окна (осн.часть)*/
#define POPDN_WIN_BOX_ATTR       (atr[13])   /* выпадающие окна (рамка)*/
#define POPDN_WIN_OUTL_ATTR      (atr[14])   /* выпадающие окна (акт.строка)*/
#define POPDN_WIN_HDRS_ATTR      (atr[15])   /* выпадающие окна (заголовки)*/
#define POPUP_WIN_ATTR           (atr[16])   /* всплывающие окна (осн.часть)*/
#define POPUP_WIN_BOX_ATTR       (atr[17])   /* всплывающие окна (рамка)*/
#define POPUP_WIN_OUTL_ATTR      (atr[18])   /* всплывающие окна (акт.строка)*/
#define SHADOW_ATTR              (atr[19])   /* тень */
#define ERROR_WIN_ATTR           (atr[20])   /* окно сообщений об ошибках */
#define ERROR_WIN_OUTL_ATTR      (atr[21])   /* окно сообщений об ошибках (акт.строка)*/
#define NO_DRAW_ATTR             (atr[22])   /* невыводимая часть окна */
#define SUPER_PROMPT_ATTR        (atr[23])   /* Prompt для super-user */
#define NUM_ATTRS                24
/******************************* Другие константы ***************************/
#define NPANELS 2       /* максимальное количество панелей*/
#define OUTNAMELEN 12   /* количество выводимых символов имени файла */
#define BEG_NAME   3    /* позиция вывода имени файла от начала окна */
#define MARKED     1    /* файл отмечен */
#define VERT            1   /*разрешено вертикальное перемещение по меню */
#define HOR             2   /*разрешено горизонтальное перемещение по меню */
#define OUT             4   /*разрешен выход при неверном направлении перехода по меню */
#define A_LEFT          (-2)/*выход из меню по стрелке влево */
#define A_RIGHT         (-3)/*выход из меню по стрелке вправо */
#define A_UP            (-4)/*выход из меню по стрелке вверх */
#define A_DOWN          (-5)/*выход из меню по стрелке вниз */
#define KEYCAP  "CONIXKEYS" /*переменная среды, которая проверяется для получения настроек клавиатуры*/
#define FROM_TOP        4 /* отступ настраиваемых пунктов сверху*/
#define FROM_BOTT       4 /* отступ настраиваемых пунктов снизу*/
#define FILTERLEN       20/* максимальная длина строки фильтра */
#define LOGNM_MAX       10 /* макс количество символов имени пользователя */

#ifdef SOLARIS1
#define SYS_NMLN    20
#endif

#if defined(__svr4__) || defined(__SVR4)
#define dev_data offset_t
#else
#define dev_data daddr_t
#endif

/*********************************** Типы файлов ****************************/
/* Значения определений FTYPE_* используются при сортировке */
#define FTYPE_DIR     1
#define FTYPE_SLN     2
#define FTYPE_CHR     3
#define FTYPE_BLK     4
#define FTYPE_FIFO    5
#define FTYPE_NAM     6
#define FTYPE_UNKNOWN 7
#define FTYPE_SOFIFO  8	/* Socket & Fifo simultaneously :( */
#define FTYPE_REG     9

/*********************************** Типы полей  ****************************/
#define MAXFIELDS     11        /* макс. количество полей (и их вариантов)*/
#define FIELD_TYPE    1
#define FIELD_MARK    2
#define FIELD_NAME    3
#define FIELD_SIZE    4
#define FIELD_DATE    5
#define FIELD_TIME    6
#define FIELD_INUM    7
#define FIELD_MODE    8
#define FIELD_OWNR    9
#define FIELD_GRP     10
#define FIELD_NLNK    11

/*********************************** Клавиатура *****************************/
#define ESC     033
#define TAB     '\t'
#define ENTER   '\n'
#ifndef CTRL
#define CTRL(key) (037 & key)
#endif
#define INSERT  CTRL('T')
#define K_INSRT (ERROR_KEY|CTRL('T'))
#define PRED    CTRL('E')            /* прерыдущая команда (по истории)*/
#define K_PRED  (ERROR_KEY|CTRL('E'))
#define NEXT    CTRL('X')            /* следующия команда (по истории)*/
#define K_NEXT  (ERROR_KEY|CTRL('X'))
#define BACKSP  CTRL('H')            /* клавиша BACKSPACE */
#define ONOFF   CTRL('O')            /* клавиша вкл/выкл панелей */
#define K_ONOFF (ERROR_KEY|CTRL('O'))
#define SWAPP   CTRL('U')            /* клавиша смены местами панелей */
#define K_SWAPP (ERROR_KEY|CTRL('U'))
#define CDROOT  CTRL('\\')           /* клавиша перехода в корневой каталог*/
#define K_CDROOT (ERROR_KEY|CDROOT)
#define REFRESH CTRL(']')            /* клавиша перевывода экрана */
#define K_REFRESH (ERROR_KEY|REFRESH)
#define REREAD  CTRL('R')            /* клавиша пересчитывания каталога */
#define K_REREAD  (ERROR_KEY|REREAD)
#define FIND    CTRL('K')            /* клавиша поиска в текущем каталоге */
#define K_FIND    (ERROR_KEY|FIND)
#define QUIT    CTRL('C')            /* клавиша выхода */
#define K_QUIT    (ERROR_KEY|QUIT)
#define DELETE  0177      /* клавиша DELETE */
#define INFO    CTRL('L')            /* клавиша вывода состояния (Info) */
#define K_INFO  (ERROR_KEY|CTRL('L'))
#define GOOTH    CTRL('G')            /* клавиша вывода состояния (Info) */
#define K_GOOTH  (ERROR_KEY|GOOTH)
#define PROC     CTRL('P')            /* клавиша вывода состояния (Info) */
#define K_PROC   (ERROR_KEY|PROC)
#define LONGNM   CTRL('N')            /* клавиша вывода длинных имен */
#define K_LONGNM (ERROR_KEY|LONGNM)
#define DFLT     CTRL('D')            /* клавиша вывода колонок панели по умолчанию */
#define K_DFLT   (ERROR_KEY|DFLT)
#define DWIDTH   CTRL('W')            /* клавиша вывода панелей во весь экран */
#define K_DWIDTH (ERROR_KEY|DWIDTH)
/********************************* Структуры ********************************/
struct itembuf {
  unsigned short    nname; /* имя файла (номер первого симв. в panel.names[])*/
  unsigned short    attrs; /* внутренние атрибуты*/
	   mode_t   mode;  /* атрибуты файла (fstat) */
	   ino_t    inum;  /* номер i-узла */
	   nlink_t  nlink; /* количество HardLink */
	   off_t    size;  /* размер файла */
	   time_t   mtime; /* время модификации */
	   uid_t    owner; /* владелец файла */
	   gid_t    group; /* группа владельца файла */
};
struct panel {
  WINDOW *win;          /* рабочее окно панели */
  short  on;            /* 0-выключено, 1-Full, 2-Info, 3-Brief */
  short  save_on;       /* сохраненное значение on */
  short  curfile;       /* номер текущего файла*/
  short  firstfile;     /* номер первого выводимого файла*/
  short  lastfile;      /* номер последнего выводимого файла*/
  char   *full_dir;     /* полное имя текущего каталога */
  int    full_dir_size; /* размер памяти, отведенной для full_dir*/
  struct itembuf *itembuf;/* буфер записей текущего каталога*/
  short  nitems;        /* кол-во выделенных элементов в dirbuf */
  short  nfiles;        /* кол-во задействованных элементов в dirbuf */
  short  lines;         /* количество строк в окне панели*/
  short  cols;          /* количество позиций в окне панели*/
  short  lnused;        /* количество неиспользуемых строк в окне панели*/
  short  sort;          /* тип сортировки 0-не сортировать */
  short  outfields[MAXFIELDS];/* номера выводимых элементов fields */
  char   *names;        /* буфер хранения имен файлов текущего каталога*/
  int    names_len;     /* размер names */
  int    selected;      /* количество отмеченных файлов*/
  long   size_sel;      /* суммарный размер отмеченных файлов*/
  char   filter_read[FILTERLEN];/* фильтр для читаемых файлов */
  char   filter_noread[FILTERLEN];/* фильтр для нечитаемых файлов */
  dev_data      total;  /* всего места на диске */
  dev_data      avail;  /* доступно для распределения не-super*/
  dev_data      free;   /* свободно */
  dev_data      bs;     /* размер блока */
  dev_data      fragm;  /* fragment size */
  dev_data      itotal; /* i-nodes всего */
  dev_data      iavail; /* i-nodes доступно для распределения не-super */
  dev_data      ifree;  /* i-nodes свободно */
  char          *fstype;/* тип файловой системы */
  char          *fsname;/* имя файловой системы */
};

struct  win_txt
	{
	chtype  *attrs;         /*атрибуты строки*/
	short   y;              /*y места начала текста*/
	short   x;              /*x места начала текста*/
	short   active;         /*признак возможности выбора пункта*/
	char    *txt;           /*строка текста*/
	};

struct  win
	{
	chtype  *attr;          /*атрибуты окна*/
	chtype  *attr_box;      /*атрибуты рамки окна*/
	chtype  *attr_astr;     /*атрибуты активногй строки*/
	short   y;              /*Y верхней строки окна*/
	short   x;              /*X левой колонки окна*/
	short   lines;          /*кол-во строк окна*/
	short   cols;           /*кол-во колонок в окне*/
	short   npunkt;         /*количество строк текста*/
	struct  win_txt *txts;  /*тексты*/
	};
struct  inp_lst                 /* структура списка ввода для inp_menu() */
	{
	char    *str;           /* строка для ввода */
	char    fld_len;        /* размер поля ввода */
	size_t  str_len;        /* размер памяти str */
	int     npunkt;         /* номер пункта меню со строкой ввода */
	};

struct  fields
	{
	short   fieldtype;      /* тип поля */
	short   fieldsize;      /* размер поля */
	char    *fieldname;     /* символьное имя поля для вывода */
	};
#ifdef NOCOLORCURSES
#define start_color() {}
#define has_colors() {0};
#define COLORS        2
#define can_change_color()  {0}
#define init_pair(npair, col1, col2) {}
#endif
