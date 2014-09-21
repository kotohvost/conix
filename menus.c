/* Определение массивов меню    */

#include "defs.h"

extern  chtype  atr[];            /* раскраска */

static  struct win_txt win1_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 4       ,16 ,1,    " Yes ",
  &POPUP_WIN_ATTR , 4       ,23 ,1,    " No ",
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Do you want to quit the Commander?"
  };

struct  win     win1=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_BOX_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   44,
/* строк текста    */   sizeof(win1_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win1_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win2_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't call editor!",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "",    /* Editor name*/
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "If you need other, set up",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     "environment variable",
  &ERROR_WIN_ATTR , 6       ,-1 ,1,     "EDITOR to it's  name",
  &ERROR_WIN_ATTR , 8       ,-1 ,1,     "[ OK ]"
  };

struct  win     win2=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   11,
/* колонок         */   29,
/* строк текста    */   sizeof(win2_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win2_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win3_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't create new process",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ OK ]"
  };

struct  win     win3=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win3_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win3_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win4_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 4       ,8  ,1,     " Delete ",
  &POPUP_WIN_ATTR , 4       ,18 ,1,     " Cancel ",
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "" /* You have selected ... files*/
  };

struct  win     win4=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win4_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win4_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win5_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "The following file will be DELETED:",
  &ERROR_WIN_ATTR , 3       ,6  ,0,     "", /* file name*/
  &ERROR_WIN_ATTR , 4       ,8  ,1,     " Delete ",
  &ERROR_WIN_ATTR , 4       ,17 ,1,     " All ",
  &ERROR_WIN_ATTR , 4       ,23 ,1,     " Skip ",
  &ERROR_WIN_ATTR , 4       ,30 ,1,     " Cancel "
  };

struct  win     win5=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   45,
/* строк текста    */   sizeof(win5_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win5_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win6_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "The following file is marked Read only",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,-1 ,0,     "Do you still wish to delete it?",
  &ERROR_WIN_ATTR , 6       ,8  ,1,     " Delete ",
  &ERROR_WIN_ATTR , 6       ,17 ,1,     " Skip ",
  &ERROR_WIN_ATTR , 6       ,24 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 6       ,32 ,1,     " Cancel ",
  };

struct  win     win6=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   9,
/* колонок         */   50,
/* строк текста    */   sizeof(win6_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win6_txt
	};

/*-----------------------------------------------------*/

/* Сообщение о начале удаления каталога */
static  struct win_txt win7_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Deleting directory",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win7=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win7_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win7_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win8_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,7  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,12 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,21 ,1,     " Cancel ",
  };

struct  win     win8=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win8_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win8_txt
	};
/*-----------------------------------------------------*/


/* Сообщение о начале удаления файла */
static  struct win_txt win9_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Deleting file",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win9=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win9_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win9_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win10_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the file",
  &ERROR_WIN_ATTR , 3       ,4  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,5  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,10 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,19 ,1,     " Cancel ",
  };

struct  win     win10=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   31,
/* строк текста    */   sizeof(win10_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win10_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win11_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Do you wish to delete",
  &POPUP_WIN_ATTR , 3       ,0  ,0,    "", /* ... file, x-настраивается*/
  &POPUP_WIN_ATTR , 4       ,8  ,1,     " Delete ",
  &POPUP_WIN_ATTR , 4       ,18 ,1,     " Cancel "
  };

struct  win     win11=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win11_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win11_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win12_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "The following file is marked Read only",
  &ERROR_WIN_ATTR , 3       ,0  ,0,     "",  /* file name x-настр.*/
  &ERROR_WIN_ATTR , 4       ,9  ,0,     "Do you still wish to delete it?",
  &ERROR_WIN_ATTR , 6       ,16 ,1,     " Delete ",
  &ERROR_WIN_ATTR , 6       ,25 ,1,     " Cancel "
  };

struct  win     win12=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   9,
/* колонок         */   49,
/* строк текста    */   sizeof(win12_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win12_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win13_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the file",
  &ERROR_WIN_ATTR , 3       ,0  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ Ok ]"
  };

struct  win     win13=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   32,
/* строк текста    */   sizeof(win13_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win13_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win14_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     " Ok ",
  };

struct  win     win14=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win14_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win14_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win15_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "",  /* Copy ** files...*/
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456789012345678901234567890123456789012345678",
  &POPUP_WIN_ATTR ,      5       ,9  ,1,     "[ Copy ]",
  &POPUP_WIN_ATTR ,      5       ,19 ,1,     "[ Cancel ]",
  &POPUP_WIN_ATTR ,      5       ,31 ,1,     "[ Link ]", /* активность изменяется программой*/
  &POPUP_WIN_ATTR ,      5       ,41 ,1,     "[ SymLink ]",
  &POPUP_WIN_ATTR ,      4       ,1  ,0,     "------------------------------------------------------------"  /* ... files*/
  };

struct  win     win15=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   62,
/* строк текста    */   sizeof(win15_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win15_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win16_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Error while copying",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     " Ok ",
  };

struct  win     win16=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win16_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win16_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win17_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't write to directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,    "", /* DirName */
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ Ok ]",
  };

struct  win     win17=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win17_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win17_txt
	};
/*-----------------------------------------------------*/
/*-----------------------------------------------------*/

static  struct win_txt win19_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Copying",
  &POPUP_WIN_ATTR , 3       ,23 ,0,    ""  /* ... files*/
  };

struct  win     win19=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   33,
/* строк текста    */   sizeof(win19_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win19_txt
	};
/*-----------------------------------------------------*/
/* Сообщение о начале копирования */
static  struct win_txt win20_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Copying",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win20=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win20_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win20_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win21_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't copy/link",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,7  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,12 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,21 ,1,     " Cancel ",
  };

struct  win     win21=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win21_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win21_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win22_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Create the directory",  /* Copy ** files...*/
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456789012345678901234567890123456789012345678",
  &POPUP_WIN_ATTR ,      5       ,8  ,1,     "[ Create ]",
  &POPUP_WIN_ATTR ,      5       ,20 ,1,     "[ Cancel ]"
  };

struct  win     win22=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   62,
/* строк текста    */   sizeof(win22_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win22_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win23_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't create the directory",
  &ERROR_WIN_ATTR , 3       ,13 ,0,     "",    /* directory name*/
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ OK ]"
  };

struct  win     win23=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   30,
/* строк текста    */   sizeof(win23_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win23_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win24_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &TOP_MENU_ATTR  , 0       ,2  ,1,    "  Left  ",
  &TOP_MENU_ATTR  , 0       ,10 ,1,    "  Files  ",
  &TOP_MENU_ATTR  , 0       ,19 ,1,    "  Commands  ",
  &TOP_MENU_ATTR  , 0       ,31 ,1,    "  Options  ",
  &TOP_MENU_ATTR  , 0       ,42 ,1,    "  Right  ",
  };

struct  win     win24=
	{
/* атр.окна        */   &TOP_MENU_ATTR,
/* атр.рамки       */   &NO_DRAW_ATTR,
/* атр.активн.стр. */   &TOP_MENU_OUTL_ATTR,
/* Y верх.         */   0,
/* X лев.          */   1,
/* строк           */   1,
/* колонок         */   78,  /* должно настраиваться */
/* строк текста    */   sizeof(win24_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win24_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win25_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPDN_WIN_ATTR , 1       ,1  ,1,    "  Brief        ",
  &POPDN_WIN_ATTR , 2       ,1  ,1,    "  Full         ",
  &POPDN_WIN_ATTR , 3       ,1  ,1,    "  Info         ",
  &POPDN_WIN_ATTR , 4       ,1  ,1,    "  On/Off       ",
  &POPDN_WIN_ATTR , 5       ,1  ,1,    "  Name         ",
  &POPDN_WIN_ATTR , 6       ,1  ,1,    "  eXtension    ",
  &POPDN_WIN_ATTR , 7       ,1  ,1,    "  tiMe         ",
  &POPDN_WIN_ATTR , 8       ,1  ,1,    "  Size         ",
  &POPDN_WIN_ATTR , 9       ,1  ,1,    "  Unsorted     ",
  &POPDN_WIN_ATTR , 10      ,1  ,1,    "  Re-read   ^R ",
  &POPDN_WIN_ATTR , 11      ,1  ,1,    "  Columns      ",
  &POPDN_WIN_ATTR , 12      ,1  ,1,    "  fiLters      "
  };  /* **** При изменении win25_txt не забыть win29 !!! **** */

struct  win     win25=
	{
/* атр.окна        */   &POPDN_WIN_ATTR,
/* атр.рамки       */   &POPDN_WIN_BOX_ATTR,
/* атр.активн.стр. */   &POPDN_WIN_OUTL_ATTR,
/* Y верх.         */   1,
/* X лев.          */   3,
/* строк           */   14,
/* колонок         */   17,
/* строк текста    */   sizeof(win25_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win25_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win26_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPDN_WIN_ATTR , 1       ,1  ,1,    "  Help              F1  ",
  &POPDN_WIN_ATTR , 2       ,1  ,0,    "  User menu         F2  ",
  &POPDN_WIN_ATTR , 3       ,1  ,1,    "  View              F3  ",
  &POPDN_WIN_ATTR , 4       ,1  ,1,    "  Edit              F4  ",
  &POPDN_WIN_ATTR , 5       ,1  ,1,    "  Copy              F5  ",
  &POPDN_WIN_ATTR , 6       ,1  ,1,    "  Rename or move    F6  ",
  &POPDN_WIN_ATTR , 7       ,1  ,1,    "  Make directory    F7  ",
  &POPDN_WIN_ATTR , 8       ,1  ,1,    "  Delete            F8  ",
  &POPDN_WIN_ATTR , 9       ,1  ,1,    "  file Attributes       ",
  &POPDN_WIN_ATTR , 10      ,1  ,1,    "  select Group      +   ",
  &POPDN_WIN_ATTR , 11      ,1  ,1,    "  deselecT group    -   ",
  &POPDN_WIN_ATTR , 12      ,1  ,1,    "  Invert selection  *   ",
  &POPDN_WIN_ATTR , 13      ,1  ,1,    "  about conix...        ",
  };

struct  win     win26=
	{
/* атр.окна        */   &POPDN_WIN_ATTR,
/* атр.рамки       */   &POPDN_WIN_BOX_ATTR,
/* атр.активн.стр. */   &POPDN_WIN_OUTL_ATTR,
/* Y верх.         */   1,
/* X лев.          */   10,
/* строк           */   15,
/* колонок         */   26,
/* строк текста    */   sizeof(win26_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win26_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win27_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPDN_WIN_ATTR , 1       ,1  ,1,    "  Find file              ",
  &POPDN_WIN_ATTR , 2       ,1  ,1,    "  find in curr.Dir    ^K ",
  &POPDN_WIN_ATTR , 3       ,1  ,1,    "  History                ",
  &POPDN_WIN_ATTR , 4       ,1  ,0,    "-------------------------",
  &POPDN_WIN_ATTR , 5       ,1  ,1,    "  Swap panels         ^U ",
  &POPDN_WIN_ATTR , 6       ,1  ,1,    "  Panels on/off       ^O ",
  &POPDN_WIN_ATTR , 7       ,1  ,1,    "  double Width        ^W ",
  &POPDN_WIN_ATTR , 8       ,1  ,1,    "  Compare directories    ",
  &POPDN_WIN_ATTR , 9       ,1  ,1,    "  compare Two files      ",
  &POPDN_WIN_ATTR , 10      ,1  ,0,    "-------------------------",
  &POPDN_WIN_ATTR , 11      ,1  ,1,    "  Go to ...              ",
  &POPDN_WIN_ATTR , 12      ,1  ,1,    "  pRocess control     ^P "
  };

struct  win     win27=
	{
/* атр.окна        */   &POPDN_WIN_ATTR,
/* атр.рамки       */   &POPDN_WIN_BOX_ATTR,
/* атр.активн.стр. */   &POPDN_WIN_OUTL_ATTR,
/* Y верх.         */   1,
/* X лев.          */   19,
/* строк           */   14,
/* колонок         */   27,
/* строк текста    */   sizeof(win27_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win27_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win28_txt[]=    /* Options */
  {/* Атрибуты      Y        X   Act.    Text */
  &POPDN_WIN_ATTR , 1       ,1  ,1,    "  Comparation...        ",
  &POPDN_WIN_ATTR , 2       ,1  ,1,    "  Save setup            ",
  &POPDN_WIN_ATTR , 3       ,1  ,1,    "  Re-read setup         ",
  &POPDN_WIN_ATTR , 4       ,1  ,0,    "------------------------",
  &POPDN_WIN_ATTR , 5       ,1  ,1,    "  short Prompt          ",
  &POPDN_WIN_ATTR , 6       ,1  ,1,    "  Node name prompt      ",
  &POPDN_WIN_ATTR , 7       ,1  ,1,    "  User@Node name prompt ",
#ifdef NCURSES
  &POPDN_WIN_ATTR , 8       ,1  ,0,    "  cLock :(Sorry,Linux)  "
#else
  &POPDN_WIN_ATTR , 8       ,1  ,1,    "  cLock                 "
#endif
  };

struct  win     win28=
	{
/* атр.окна        */   &POPDN_WIN_ATTR,
/* атр.рамки       */   &POPDN_WIN_BOX_ATTR,
/* атр.активн.стр. */   &POPDN_WIN_OUTL_ATTR,
/* Y верх.         */   1,
/* X лев.          */   31,
/* строк           */   10,
/* колонок         */   26,
/* строк текста    */   sizeof(win28_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win28_txt
	};
/*-----------------------------------------------------*/
struct  win     win29=
	{
/* атр.окна        */   &POPDN_WIN_ATTR,
/* атр.рамки       */   &POPDN_WIN_BOX_ATTR,
/* атр.активн.стр. */   &POPDN_WIN_OUTL_ATTR,
/* Y верх.         */   1,
/* X лев.          */   43, /* единственное отличие от win25 */
/* строк           */   14,
/* колонок         */   17,
/* строк текста    */   sizeof(win25_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win25_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win30_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't change mode for",
  &ERROR_WIN_ATTR , 3       ,4  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,5  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,10 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,19 ,1,     " Cancel ",
  };

struct  win     win30=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   31,
/* строк текста    */   sizeof(win30_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win30_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win31_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "Change file attributes for",
  &POPUP_WIN_ATTR ,      3       ,2  ,0,     "", /* File name */
  &POPUP_WIN_ATTR ,      5       ,5  ,0,     " Set  Clear",
  &POPUP_WIN_ATTR ,      6       ,5  ,0,     "[   ] [   ] Owner",
  &POPUP_WIN_ATTR ,      7       ,5  ,0,     "[   ] [   ] Group",
  &POPUP_WIN_ATTR ,      8       ,5  ,0,     "[   ] [   ] Other",
  &POPUP_WIN_ATTR ,      10      ,3  ,1,     "[ Set ]",
  &POPUP_WIN_ATTR ,      10      ,13 ,1,     "[ Cancel ]"
  };

struct  win     win31=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   12,
/* колонок         */   30,
/* строк текста    */   sizeof(win31_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win31_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win32_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "Change file attributes for",
  &POPUP_WIN_ATTR ,      3       ,2  ,0,     "", /* File name */
  &POPUP_WIN_ATTR ,      4       ,7  ,0,     "",
  &POPUP_WIN_ATTR ,      5       ,7  ,0,     "[   ]  Owner",
  &POPUP_WIN_ATTR ,      6       ,7  ,0,     "[   ]  Group",
  &POPUP_WIN_ATTR ,      7       ,7  ,0,     "[   ]  Other",
  &POPUP_WIN_ATTR ,      9       ,3  ,1,     "[ Set ]",
  &POPUP_WIN_ATTR ,      9       ,13 ,1,     "[ Cancel ]"
  };

struct  win     win32=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   11,
/* колонок         */   30,
/* строк текста    */   sizeof(win32_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win32_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win33_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Select the files",
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456"
  };

struct  win     win33=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   22,
/* строк текста    */   sizeof(win33_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win33_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win34_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Could not find a match:",
  &ERROR_WIN_ATTR , 3       ,2  ,0,    "", /* Шаблон */
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Ok ]"
  };

struct  win     win34=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   26,
/* строк текста    */   sizeof(win34_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win34_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win35_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Deselect the files",
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "123456789012345678"
  };

struct  win     win35=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   24,
/* строк текста    */   sizeof(win35_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win35_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win36_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Selection will be",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "LOST!",
  &ERROR_WIN_ATTR , 5       ,3  ,1,    "[ Ok ]",
  &ERROR_WIN_ATTR , 5       ,10 ,1,    "[ Cancel ]"
  };

struct  win     win36=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   24,
/* строк текста    */   sizeof(win36_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win36_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win37_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "The two directories appear",
  &POPUP_WIN_ATTR ,      3       ,-1 ,0,     "to be identical.",
  &POPUP_WIN_ATTR ,      4       ,-1 ,1,     "[ OK ]",
  };

struct  win     win37=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   30,
/* строк текста    */   sizeof(win37_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win37_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win38_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPDN_WIN_ATTR , 1       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 2       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 3       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 4       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 5       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 6       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 7       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 8       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 9       ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 10      ,1  ,1,  "                                      ",
  &POPDN_WIN_ATTR , 11      ,1  ,1,  "                                      "
  };

struct  win     win38=
	{
/* атр.окна        */   &POPDN_WIN_ATTR,
/* атр.рамки       */   &POPDN_WIN_BOX_ATTR,
/* атр.активн.стр. */   &POPDN_WIN_OUTL_ATTR,
/* Y верх.         */   1,
/* X лев.          */   10,
/* строк           */   13,
/* колонок         */   40,
/* строк текста    */   sizeof(win38_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win38_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win39_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "The history if empty",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,    "[ Ok ]"
  };

struct  win     win39=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   26,
/* строк текста    */   sizeof(win39_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win39_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win40_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Find file in current directory",
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      5       ,6  ,1,     "[ Find ]",
  &POPUP_WIN_ATTR ,      5       ,16 ,1,     "[ Cancel ]"
  };

struct  win     win40=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win40_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win40_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win41_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "File not found",
  &ERROR_WIN_ATTR , 4       ,3  ,1,    "[ Ok ]",
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Repeat ]"
  };

struct  win     win41=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   23,
/* строк текста    */   sizeof(win41_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win41_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win42_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "File(s) to find:",
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      4       ,2  ,0,     "Start in directory:",
  &POPUP_WIN_OUTL_ATTR , 5       ,2  ,1,     "123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      7       ,3  ,1,     "[ Find ]",
  &POPUP_WIN_ATTR ,      7       ,13 ,1,     "[ Cancel ]",
  &POPUP_WIN_ATTR ,      7       ,25 ,0,     "[ Last ]"
  };

struct  win     win42=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   10,
/* колонок         */   36,
/* строк текста    */   sizeof(win42_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win42_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win43_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Start directory uncorrect!",
  &ERROR_WIN_ATTR , 4       ,3  ,1,    "[ Repeat ]",
  &ERROR_WIN_ATTR , 4       ,14 ,1,    "[ Cancel ]"
  };

struct  win     win43=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   30,
/* строк текста    */   sizeof(win43_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win43_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win44_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "",  /* Move ** files...*/
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456789012345678901234567890123456789012345678",
  &POPUP_WIN_ATTR ,      5       ,9  ,1,     "[ Move ]",
  &POPUP_WIN_ATTR ,      5       ,19 ,1,     "[ Cancel ]",
  &POPUP_WIN_ATTR ,      4       ,1  ,0,     "------------------------------------------------------------",
  &POPUP_WIN_ATTR ,      1       ,-1 ,0,     "Moving just with simple \"mv\"!!!"
  };

struct  win     win44=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   62,
/* строк текста    */   sizeof(win44_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win44_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win45_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Error while moving",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     " Ok ",
  };

struct  win     win45=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win45_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win45_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win46_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't write to directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,    "", /* DirName */
  &ERROR_WIN_ATTR , 4       ,16 ,1,     "[ Ok ]",
  };

struct  win     win46=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win46_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win46_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win47_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Moving",
  &POPUP_WIN_ATTR , 3       ,23 ,0,    ""  /* ... files*/
  };

struct  win     win47=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   33,
/* строк текста    */   sizeof(win47_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win47_txt
	};
/*-----------------------------------------------------*/
/* Сообщение о начале копирования */
static  struct win_txt win48_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Moving",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win48=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win48_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win48_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win49_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't move",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,7  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,12 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,21 ,1,     " Cancel ",
  };

struct  win     win49=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   36,
/* строк текста    */   sizeof(win49_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win49_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win50_txt[MAXFIELDS+1]; /* будет инициализировано в columns()*/

struct  win     win50=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   7,
/* X лев.          */   35,
/* строк           */   MAXFIELDS+3,
/* колонок         */   9,
/* строк текста    */   sizeof(win50_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win50_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win51_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Saving setup in",
  &POPUP_WIN_ATTR , 3       ,3  ,0,     ""   /* file name */
  };

struct  win     win51=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   28,
/* строк текста    */   sizeof(win51_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win51_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win52_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't find",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "$HOME directory",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win52=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   36,
/* строк текста    */   sizeof(win52_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win52_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win53_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't write to",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "",  /* File name */
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win53=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   28,  /* размер должен быть не меньше, чем в win51 */
/* строк текста    */   sizeof(win53_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win53_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win54_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't open current",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "directory. Try \"/\"",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win54=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   24,
/* строк текста    */   sizeof(win54_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win54_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win55_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't open",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "$HOME/.conix.goto",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win55=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   36,
/* строк текста    */   sizeof(win55_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win55_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win56_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "Set filters",
  &POPUP_WIN_ATTR ,      3       ,2  ,0,     "Read:",
  &POPUP_WIN_OUTL_ATTR , 3       ,9  ,1,     "12345678901234567890",
  &POPUP_WIN_ATTR ,      4       ,2  ,0,     "NoRead:",
  &POPUP_WIN_OUTL_ATTR , 4       ,9  ,1,     "12345678901234567890",
  &POPUP_WIN_ATTR ,      6       ,9  ,1,     "[ Set ]",
  &POPUP_WIN_ATTR ,      6       ,19 ,1,     "[ Cancel ]",
  };

struct  win     win56=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   31,
/* строк текста    */   sizeof(win56_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win56_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win57_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't open",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "", /* Filename*/
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win57=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   36,
/* строк текста    */   sizeof(win57_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win57_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win58_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Write setup to",
  &POPUP_WIN_ATTR , 3       ,-1 ,0,     ".conix.ini in directory:",
  &POPUP_WIN_ATTR , 5       ,6  ,1,     " Home ",
  &POPUP_WIN_ATTR , 5       ,13 ,1,     " Current "
  };

struct  win     win58=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   28,
/* строк текста    */   sizeof(win58_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win58_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win59_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,6  ,0,     "Compare",
  &POPUP_WIN_ATTR , 3       ,2  ,0,     "[x] Existence",
  &POPUP_WIN_ATTR , 4       ,2  ,0,     "[x] Time",
  &POPUP_WIN_ATTR , 5       ,2  ,0,     "[x] Size",
  &POPUP_WIN_ATTR , 6       ,2  ,0,     "[x] Mode",
  &POPUP_WIN_ATTR , 7       ,2  ,0,     "[x] Contence",
  &POPUP_WIN_ATTR , 8       ,2  ,0,     "[x] Links#",
  &POPUP_WIN_ATTR , 3       ,3  ,-1,    "x",  /* active==-1 - Check box */
  &POPUP_WIN_ATTR , 4       ,3  ,-1,    "x",
  &POPUP_WIN_ATTR , 5       ,3  ,-1,    "x",
  &POPUP_WIN_ATTR , 6       ,3  ,-1,    "x",
  &POPUP_WIN_ATTR , 7       ,3  ,-1,    "x",
  &POPUP_WIN_ATTR , 8       ,3  ,-1,    "x",
  &POPUP_WIN_ATTR , 10      ,6  ,1,     "[ OK ]",
  &POPUP_WIN_ATTR , 10      ,13 ,1,     "[ Cancel ]"
  };

struct  win     win59=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   12,
/* колонок         */   28,
/* строк текста    */   sizeof(win59_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win59_txt
	};
/*-----------------------------------------------------*/
/* Сообщение о сравнении файлов */
static  struct win_txt win60_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Comparing files",
  &POPUP_WIN_ATTR , 3       ,-1 ,0,     "",   /* file name */
  &POPUP_WIN_ATTR , 4       ,-1 ,0,     "and",
  &POPUP_WIN_ATTR , 5       ,-1 ,0,     "",   /* file name */
  };

struct  win     win60=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   33,
/* строк текста    */   sizeof(win60_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win60_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win61_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Error Comparing files",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "",   /* file name */
  &ERROR_WIN_ATTR , 4       ,-1 ,0,     "and",
  &ERROR_WIN_ATTR , 5       ,-1 ,0,     "",   /* file name */
  &ERROR_WIN_ATTR , 7       ,5  ,1,     "[ OK ]",
  &ERROR_WIN_ATTR , 7       ,12 ,1,     "[ Silent ]",
  &ERROR_WIN_ATTR , 7       ,23 ,1,     "[ Cancel ]",
  };

struct  win     win61=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   10,
/* колонок         */   35,
/* строк текста    */   sizeof(win61_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win61_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win62_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "The two files appear to be",
  &POPUP_WIN_ATTR ,      4       ,-1 ,0,     "IDENTICAL",
  &POPUP_WIN_ATTR ,      6       ,-1 ,1,     "[ OK ]",
  };

struct  win     win62=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   9,
/* колонок         */   33,
/* строк текста    */   sizeof(win62_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win62_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win63_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "The two files appear to be",
  &ERROR_WIN_ATTR ,      4       ,-1 ,0,     " DIFFERENT ",
  &POPUP_WIN_ATTR ,      6       ,-1 ,1,     "[ OK ]",
  };

struct  win     win63=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   9,
/* колонок         */   33,
/* строк текста    */   sizeof(win63_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win63_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win64_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Error",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "Comparing two files",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     "[ OK ]",
  };

struct  win     win64=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   33,
/* строк текста    */   sizeof(win64_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win64_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win65_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Sorry. Can't compare",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "NON-REGULAR files",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     "[ OK ]",
  };

struct  win     win65=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   33,
/* строк текста    */   sizeof(win65_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win65_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win66_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Search for the string",
  &POPUP_WIN_OUTL_ATTR , 3       ,-1 ,1,     "12345678901234567890123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      0       ,-1 ,0,     "View"
  };

struct  win     win66=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   54,
/* строк текста    */   sizeof(win66_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win66_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win67_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Could not find a match",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "",
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Ok ]"
  };

struct  win     win67=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   26,
/* строк текста    */   sizeof(win67_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win67_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win68_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 0       ,-1 ,0,     "Template",
  &POPUP_WIN_ATTR , 2       ,-1 ,1,     " Defaults  ",
  &POPUP_WIN_ATTR , 3       ,-1 ,1,     " Long name ",
  };

struct  win     win68=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   17,
/* строк текста    */   sizeof(win68_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win68_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win69_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 0       ,-1 ,0,     "Recoding",
  &POPUP_WIN_ATTR , 2       ,-1 ,1,     " From... ",
  &POPUP_WIN_ATTR , 3       ,-1 ,1,     " To..... ",
  };

struct  win     win69=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   17,
/* строк текста    */   sizeof(win69_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win69_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win70_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 0       ,-1 ,0,     "",   /* From... To...*/
  &POPUP_WIN_ATTR , 2       ,-1 ,1,     " Alternative ",
  &POPUP_WIN_ATTR , 3       ,-1 ,1,     " ISO 8859-5  ",
  &POPUP_WIN_ATTR , 4       ,-1 ,1,     " KOI-8       ",
  &POPUP_WIN_ATTR , 5       ,-1 ,1,     " MS-Windows  ",
  };

struct  win     win70=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   17,
/* строк текста    */   sizeof(win70_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win70_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win71_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Search for the string",
  &POPUP_WIN_OUTL_ATTR , 3       ,-1 ,1,     "12345678901234567890123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      0       ,-1 ,0,     "View"
  };

struct  win     win71=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   54,
/* строк текста    */   sizeof(win71_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win71_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win72_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Could not find a match",
  &ERROR_WIN_ATTR , 3       ,2  ,0,    "", /* Шаблон */
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Ok ]"
  };

struct  win     win72=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   26,
/* строк текста    */   sizeof(win72_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win72_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win73_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      0       ,-1 ,0,     "Send Signal",
  &POPUP_WIN_ATTR ,      1       ,2  ,1,     " 1   SIGHUP    hangup                                      ",
  &POPUP_WIN_ATTR ,      2       ,2  ,1,     " 2   SIGINT    interrupt (rubout)                          ",
  &POPUP_WIN_ATTR ,      3       ,2  ,1,     " 3   SIGQUIT   quit (ASCII FS)                             ",
  &POPUP_WIN_ATTR ,      4       ,2  ,1,     " 4   SIGILL    illegal instruction (not reset when caught) ",
  &POPUP_WIN_ATTR ,      5       ,2  ,1,     " 5   SIGTRAP   trace trap (not reset when caught)          ",
  &POPUP_WIN_ATTR ,      6       ,2  ,1,     " 6   SIGABRT   used by abort, replace SIGIOT               ",
  &POPUP_WIN_ATTR ,      7       ,2  ,1,     " 7   SIGEMT    EMT instruction                             ",
  &POPUP_WIN_ATTR ,      8       ,2  ,1,     " 8   SIGFPE    floating point exception                    ",
  &POPUP_WIN_ATTR ,      9       ,2  ,1,     " 9   SIGKILL   kill (cannot be caught or ignored)          ",
  &POPUP_WIN_ATTR ,      10      ,2  ,1,     " 10  SIGBUS    bus error                                   ",
  &POPUP_WIN_ATTR ,      11      ,2  ,1,     " 11  SIGSEGV   segmentation violation                      ",
  &POPUP_WIN_ATTR ,      12      ,2  ,1,     " 12  SIGSYS    bad argument to system call                 ",
  &POPUP_WIN_ATTR ,      13      ,2  ,1,     " 13  SIGPIPE   write on a pipe with no one to read it      ",
  &POPUP_WIN_ATTR ,      14      ,2  ,1,     " 14  SIGALRM   alarm clock                                 ",
  &POPUP_WIN_ATTR ,      15      ,2  ,1,     " 15  SIGTERM   software termination signal from kill       ",
  &POPUP_WIN_ATTR ,      16      ,2  ,1,     " 16  SIGUSR1   user defined signal 1                       ",
  &POPUP_WIN_ATTR ,      17      ,2  ,1,     " 17  SIGUSR2   user defined signal 2                       "
  };

struct  win     win73=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   19,
/* колонок         */   63,
/* строк текста    */   sizeof(win73_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win73_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win74_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "File not found",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,    "[ Ok ]"
  };

struct  win     win74=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   23,
/* строк текста    */   sizeof(win74_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win74_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win75_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Show user (all - empty)",
  &POPUP_WIN_OUTL_ATTR , 3       ,-1 ,1,     "123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      0       ,-1 ,0,     "Process Control"
  };

struct  win     win75=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   6,
/* колонок         */   34,
/* строк текста    */   sizeof(win75_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win75_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win76_txt[]=
  {/* Атрибуты           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "Loading viewer",
  &POPUP_WIN_ATTR ,      4       ,-1 ,0,     "Please wait",
  };

struct  win     win76=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   7,
/* колонок         */   20,
/* строк текста    */   sizeof(win76_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win76_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win77_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 5       ,12 ,1,    " Overwrite ",
  &ERROR_WIN_ATTR , 5       ,25 ,1,    " Cancel ",
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Some files/dirs in specified target path",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "already EXISTs. Overwrite them?"
  };

struct  win     win77=
	{
/* атр.окна        */   &ERROR_WIN_ATTR,
/* атр.рамки       */   &ERROR_WIN_ATTR,
/* атр.активн.стр. */   &ERROR_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   8,
/* колонок         */   46,
/* строк текста    */   sizeof(win77_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win77_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win78_txt[]=
  {/* Атрибуты      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 10      ,-1 ,1,     " OK ",
  &POPUP_WIN_ATTR , 0       ,-1 ,0,     " About conix (COmmander for uNIXs) ",
  &POPUP_WIN_ATTR , 2       ,2  ,0,     "You can distribute conix without  restrictions.",
  &POPUP_WIN_ATTR , 3       ,2  ,0,     "Of course, you get and use conix  \"AS IS\"  with",
  &POPUP_WIN_ATTR , 4       ,2  ,0,     "NO warranty...  You can modify help file,  just",
  &POPUP_WIN_ATTR , 5       ,2  ,0,     "send your changes to me,  they can  be  useful.",
  &POPUP_WIN_ATTR , 6       ,2  ,0,     "I have no time enough to  make  corrections  in",
  &POPUP_WIN_ATTR , 7       ,2  ,0,     "conix, but sometimes I do that. ;^)   Feel free",
  &POPUP_WIN_ATTR , 8       ,2  ,0,     "to send me your comments and notes about conix.",
  &POPUP_WIN_ATTR , 9       ,2  ,0,     "             Serguei I. Revtov (sir@cb.tver.ru)"
  };

struct  win     win78=
	{
/* атр.окна        */   &POPUP_WIN_ATTR,
/* атр.рамки       */   &POPUP_WIN_ATTR,
/* атр.активн.стр. */   &POPUP_WIN_OUTL_ATTR,
/* Y верх.         */   -1,
/* X лев.          */   -1,
/* строк           */   12,
/* колонок         */   51,
/* строк текста    */   sizeof(win78_txt)/sizeof(struct win_txt),
/* ссылка на текст */   win78_txt
	};
/*-----------------------------------------------------*/
