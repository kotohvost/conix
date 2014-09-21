/* ��।������ ���ᨢ�� ����    */

#include "defs.h"

extern  chtype  atr[];            /* ���᪠ */

static  struct win_txt win1_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 4       ,16 ,1,    " Yes ",
  &POPUP_WIN_ATTR , 4       ,23 ,1,    " No ",
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Do you want to quit the Commander?"
  };

struct  win     win1=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_BOX_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   44,
/* ��ப ⥪��    */   sizeof(win1_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win1_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win2_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't call editor!",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "",    /* Editor name*/
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "If you need other, set up",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     "environment variable",
  &ERROR_WIN_ATTR , 6       ,-1 ,1,     "EDITOR to it's  name",
  &ERROR_WIN_ATTR , 8       ,-1 ,1,     "[ OK ]"
  };

struct  win     win2=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   11,
/* �������         */   29,
/* ��ப ⥪��    */   sizeof(win2_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win2_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win3_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't create new process",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ OK ]"
  };

struct  win     win3=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win3_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win3_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win4_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 4       ,8  ,1,     " Delete ",
  &POPUP_WIN_ATTR , 4       ,18 ,1,     " Cancel ",
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "" /* You have selected ... files*/
  };

struct  win     win4=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win4_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win4_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win5_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "The following file will be DELETED:",
  &ERROR_WIN_ATTR , 3       ,6  ,0,     "", /* file name*/
  &ERROR_WIN_ATTR , 4       ,8  ,1,     " Delete ",
  &ERROR_WIN_ATTR , 4       ,17 ,1,     " All ",
  &ERROR_WIN_ATTR , 4       ,23 ,1,     " Skip ",
  &ERROR_WIN_ATTR , 4       ,30 ,1,     " Cancel "
  };

struct  win     win5=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   45,
/* ��ப ⥪��    */   sizeof(win5_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win5_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win6_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   9,
/* �������         */   50,
/* ��ப ⥪��    */   sizeof(win6_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win6_txt
	};

/*-----------------------------------------------------*/

/* ����饭�� � ��砫� 㤠����� ��⠫��� */
static  struct win_txt win7_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Deleting directory",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win7=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win7_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win7_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win8_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,7  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,12 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,21 ,1,     " Cancel ",
  };

struct  win     win8=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win8_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win8_txt
	};
/*-----------------------------------------------------*/


/* ����饭�� � ��砫� 㤠����� 䠩�� */
static  struct win_txt win9_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Deleting file",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win9=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win9_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win9_txt
	};

/*-----------------------------------------------------*/

static  struct win_txt win10_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the file",
  &ERROR_WIN_ATTR , 3       ,4  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,5  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,10 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,19 ,1,     " Cancel ",
  };

struct  win     win10=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   31,
/* ��ப ⥪��    */   sizeof(win10_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win10_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win11_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Do you wish to delete",
  &POPUP_WIN_ATTR , 3       ,0  ,0,    "", /* ... file, x-����ࠨ������*/
  &POPUP_WIN_ATTR , 4       ,8  ,1,     " Delete ",
  &POPUP_WIN_ATTR , 4       ,18 ,1,     " Cancel "
  };

struct  win     win11=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win11_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win11_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win12_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "The following file is marked Read only",
  &ERROR_WIN_ATTR , 3       ,0  ,0,     "",  /* file name x-�����.*/
  &ERROR_WIN_ATTR , 4       ,9  ,0,     "Do you still wish to delete it?",
  &ERROR_WIN_ATTR , 6       ,16 ,1,     " Delete ",
  &ERROR_WIN_ATTR , 6       ,25 ,1,     " Cancel "
  };

struct  win     win12=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   9,
/* �������         */   49,
/* ��ப ⥪��    */   sizeof(win12_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win12_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win13_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the file",
  &ERROR_WIN_ATTR , 3       ,0  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ Ok ]"
  };

struct  win     win13=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   32,
/* ��ப ⥪��    */   sizeof(win13_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win13_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win14_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't delete the directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     " Ok ",
  };

struct  win     win14=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win14_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win14_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win15_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "",  /* Copy ** files...*/
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456789012345678901234567890123456789012345678",
  &POPUP_WIN_ATTR ,      5       ,9  ,1,     "[ Copy ]",
  &POPUP_WIN_ATTR ,      5       ,19 ,1,     "[ Cancel ]",
  &POPUP_WIN_ATTR ,      5       ,31 ,1,     "[ Link ]", /* ��⨢����� ��������� �ணࠬ���*/
  &POPUP_WIN_ATTR ,      5       ,41 ,1,     "[ SymLink ]",
  &POPUP_WIN_ATTR ,      4       ,1  ,0,     "------------------------------------------------------------"  /* ... files*/
  };

struct  win     win15=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   62,
/* ��ப ⥪��    */   sizeof(win15_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win15_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win16_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Error while copying",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     " Ok ",
  };

struct  win     win16=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win16_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win16_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win17_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't write to directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,    "", /* DirName */
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ Ok ]",
  };

struct  win     win17=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win17_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win17_txt
	};
/*-----------------------------------------------------*/
/*-----------------------------------------------------*/

static  struct win_txt win19_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Copying",
  &POPUP_WIN_ATTR , 3       ,23 ,0,    ""  /* ... files*/
  };

struct  win     win19=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win19_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win19_txt
	};
/*-----------------------------------------------------*/
/* ����饭�� � ��砫� ����஢���� */
static  struct win_txt win20_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Copying",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win20=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win20_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win20_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win21_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't copy/link",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,7  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,12 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,21 ,1,     " Cancel ",
  };

struct  win     win21=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win21_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win21_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win22_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Create the directory",  /* Copy ** files...*/
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456789012345678901234567890123456789012345678",
  &POPUP_WIN_ATTR ,      5       ,8  ,1,     "[ Create ]",
  &POPUP_WIN_ATTR ,      5       ,20 ,1,     "[ Cancel ]"
  };

struct  win     win22=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   62,
/* ��ப ⥪��    */   sizeof(win22_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win22_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win23_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't create the directory",
  &ERROR_WIN_ATTR , 3       ,13 ,0,     "",    /* directory name*/
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     "[ OK ]"
  };

struct  win     win23=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   30,
/* ��ப ⥪��    */   sizeof(win23_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win23_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win24_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &TOP_MENU_ATTR  , 0       ,2  ,1,    "  Left  ",
  &TOP_MENU_ATTR  , 0       ,10 ,1,    "  Files  ",
  &TOP_MENU_ATTR  , 0       ,19 ,1,    "  Commands  ",
  &TOP_MENU_ATTR  , 0       ,31 ,1,    "  Options  ",
  &TOP_MENU_ATTR  , 0       ,42 ,1,    "  Right  ",
  };

struct  win     win24=
	{
/* ���.����        */   &TOP_MENU_ATTR,
/* ���.ࠬ��       */   &NO_DRAW_ATTR,
/* ���.��⨢�.���. */   &TOP_MENU_OUTL_ATTR,
/* Y ����.         */   0,
/* X ���.          */   1,
/* ��ப           */   1,
/* �������         */   78,  /* ������ ����ࠨ������ */
/* ��ப ⥪��    */   sizeof(win24_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win24_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win25_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
  };  /* **** �� ��������� win25_txt �� ������ win29 !!! **** */

struct  win     win25=
	{
/* ���.����        */   &POPDN_WIN_ATTR,
/* ���.ࠬ��       */   &POPDN_WIN_BOX_ATTR,
/* ���.��⨢�.���. */   &POPDN_WIN_OUTL_ATTR,
/* Y ����.         */   1,
/* X ���.          */   3,
/* ��ப           */   14,
/* �������         */   17,
/* ��ப ⥪��    */   sizeof(win25_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win25_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win26_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &POPDN_WIN_ATTR,
/* ���.ࠬ��       */   &POPDN_WIN_BOX_ATTR,
/* ���.��⨢�.���. */   &POPDN_WIN_OUTL_ATTR,
/* Y ����.         */   1,
/* X ���.          */   10,
/* ��ப           */   15,
/* �������         */   26,
/* ��ப ⥪��    */   sizeof(win26_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win26_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win27_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &POPDN_WIN_ATTR,
/* ���.ࠬ��       */   &POPDN_WIN_BOX_ATTR,
/* ���.��⨢�.���. */   &POPDN_WIN_OUTL_ATTR,
/* Y ����.         */   1,
/* X ���.          */   19,
/* ��ப           */   14,
/* �������         */   27,
/* ��ப ⥪��    */   sizeof(win27_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win27_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win28_txt[]=    /* Options */
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &POPDN_WIN_ATTR,
/* ���.ࠬ��       */   &POPDN_WIN_BOX_ATTR,
/* ���.��⨢�.���. */   &POPDN_WIN_OUTL_ATTR,
/* Y ����.         */   1,
/* X ���.          */   31,
/* ��ப           */   10,
/* �������         */   26,
/* ��ப ⥪��    */   sizeof(win28_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win28_txt
	};
/*-----------------------------------------------------*/
struct  win     win29=
	{
/* ���.����        */   &POPDN_WIN_ATTR,
/* ���.ࠬ��       */   &POPDN_WIN_BOX_ATTR,
/* ���.��⨢�.���. */   &POPDN_WIN_OUTL_ATTR,
/* Y ����.         */   1,
/* X ���.          */   43, /* �����⢥���� �⫨稥 �� win25 */
/* ��ப           */   14,
/* �������         */   17,
/* ��ப ⥪��    */   sizeof(win25_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win25_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win30_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't change mode for",
  &ERROR_WIN_ATTR , 3       ,4  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,5  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,10 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,19 ,1,     " Cancel ",
  };

struct  win     win30=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   31,
/* ��ப ⥪��    */   sizeof(win30_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win30_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win31_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
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
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   12,
/* �������         */   30,
/* ��ப ⥪��    */   sizeof(win31_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win31_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win32_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
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
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   11,
/* �������         */   30,
/* ��ப ⥪��    */   sizeof(win32_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win32_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win33_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Select the files",
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456"
  };

struct  win     win33=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   22,
/* ��ப ⥪��    */   sizeof(win33_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win33_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win34_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Could not find a match:",
  &ERROR_WIN_ATTR , 3       ,2  ,0,    "", /* ������ */
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Ok ]"
  };

struct  win     win34=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   26,
/* ��ப ⥪��    */   sizeof(win34_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win34_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win35_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Deselect the files",
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "123456789012345678"
  };

struct  win     win35=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   24,
/* ��ப ⥪��    */   sizeof(win35_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win35_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win36_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Selection will be",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "LOST!",
  &ERROR_WIN_ATTR , 5       ,3  ,1,    "[ Ok ]",
  &ERROR_WIN_ATTR , 5       ,10 ,1,    "[ Cancel ]"
  };

struct  win     win36=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   24,
/* ��ப ⥪��    */   sizeof(win36_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win36_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win37_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "The two directories appear",
  &POPUP_WIN_ATTR ,      3       ,-1 ,0,     "to be identical.",
  &POPUP_WIN_ATTR ,      4       ,-1 ,1,     "[ OK ]",
  };

struct  win     win37=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   30,
/* ��ப ⥪��    */   sizeof(win37_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win37_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win38_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &POPDN_WIN_ATTR,
/* ���.ࠬ��       */   &POPDN_WIN_BOX_ATTR,
/* ���.��⨢�.���. */   &POPDN_WIN_OUTL_ATTR,
/* Y ����.         */   1,
/* X ���.          */   10,
/* ��ப           */   13,
/* �������         */   40,
/* ��ப ⥪��    */   sizeof(win38_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win38_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win39_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "The history if empty",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,    "[ Ok ]"
  };

struct  win     win39=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   26,
/* ��ப ⥪��    */   sizeof(win39_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win39_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win40_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Find file in current directory",
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      5       ,6  ,1,     "[ Find ]",
  &POPUP_WIN_ATTR ,      5       ,16 ,1,     "[ Cancel ]"
  };

struct  win     win40=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win40_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win40_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win41_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "File not found",
  &ERROR_WIN_ATTR , 4       ,3  ,1,    "[ Ok ]",
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Repeat ]"
  };

struct  win     win41=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   23,
/* ��ப ⥪��    */   sizeof(win41_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win41_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win42_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
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
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   10,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win42_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win42_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win43_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Start directory uncorrect!",
  &ERROR_WIN_ATTR , 4       ,3  ,1,    "[ Repeat ]",
  &ERROR_WIN_ATTR , 4       ,14 ,1,    "[ Cancel ]"
  };

struct  win     win43=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   30,
/* ��ப ⥪��    */   sizeof(win43_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win43_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win44_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "",  /* Move ** files...*/
  &POPUP_WIN_OUTL_ATTR , 3       ,2  ,1,     "1234567890123456789012345678901234567890123456789012345678",
  &POPUP_WIN_ATTR ,      5       ,9  ,1,     "[ Move ]",
  &POPUP_WIN_ATTR ,      5       ,19 ,1,     "[ Cancel ]",
  &POPUP_WIN_ATTR ,      4       ,1  ,0,     "------------------------------------------------------------",
  &POPUP_WIN_ATTR ,      1       ,-1 ,0,     "Moving just with simple \"mv\"!!!"
  };

struct  win     win44=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   62,
/* ��ப ⥪��    */   sizeof(win44_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win44_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win45_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Error while moving",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,     " Ok ",
  };

struct  win     win45=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win45_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win45_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win46_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't write to directory",
  &ERROR_WIN_ATTR , 3       ,5  ,0,    "", /* DirName */
  &ERROR_WIN_ATTR , 4       ,16 ,1,     "[ Ok ]",
  };

struct  win     win46=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win46_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win46_txt
	};
/*-----------------------------------------------------*/

static  struct win_txt win47_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Moving",
  &POPUP_WIN_ATTR , 3       ,23 ,0,    ""  /* ... files*/
  };

struct  win     win47=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win47_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win47_txt
	};
/*-----------------------------------------------------*/
/* ����饭�� � ��砫� ����஢���� */
static  struct win_txt win48_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Moving",
  &POPUP_WIN_ATTR , 3       ,5  ,0,     ""   /* file name */
  };

struct  win     win48=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win48_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win48_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win49_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Can't move",
  &ERROR_WIN_ATTR , 3       ,5  ,0,     "",  /* file name */
  &ERROR_WIN_ATTR , 4       ,7  ,1,     " Ok ",
  &ERROR_WIN_ATTR , 4       ,12 ,1,     " Silent ",
  &ERROR_WIN_ATTR , 4       ,21 ,1,     " Cancel ",
  };

struct  win     win49=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win49_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win49_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win50_txt[MAXFIELDS+1]; /* �㤥� ���樠����஢��� � columns()*/

struct  win     win50=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   7,
/* X ���.          */   35,
/* ��ப           */   MAXFIELDS+3,
/* �������         */   9,
/* ��ப ⥪��    */   sizeof(win50_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win50_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win51_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Saving setup in",
  &POPUP_WIN_ATTR , 3       ,3  ,0,     ""   /* file name */
  };

struct  win     win51=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   28,
/* ��ப ⥪��    */   sizeof(win51_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win51_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win52_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't find",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "$HOME directory",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win52=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win52_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win52_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win53_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't write to",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "",  /* File name */
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win53=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   28,  /* ࠧ��� ������ ���� �� �����, 祬 � win51 */
/* ��ப ⥪��    */   sizeof(win53_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win53_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win54_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't open current",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "directory. Try \"/\"",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win54=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   24,
/* ��ப ⥪��    */   sizeof(win54_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win54_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win55_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't open",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "$HOME/.conix.goto",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win55=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win55_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win55_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win56_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
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
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   31,
/* ��ப ⥪��    */   sizeof(win56_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win56_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win57_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,     "Can't open",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,     "", /* Filename*/
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     " Ok "
  };

struct  win     win57=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   36,
/* ��ப ⥪��    */   sizeof(win57_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win57_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win58_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,     "Write setup to",
  &POPUP_WIN_ATTR , 3       ,-1 ,0,     ".conix.ini in directory:",
  &POPUP_WIN_ATTR , 5       ,6  ,1,     " Home ",
  &POPUP_WIN_ATTR , 5       ,13 ,1,     " Current "
  };

struct  win     win58=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   28,
/* ��ப ⥪��    */   sizeof(win58_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win58_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win59_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   12,
/* �������         */   28,
/* ��ப ⥪��    */   sizeof(win59_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win59_txt
	};
/*-----------------------------------------------------*/
/* ����饭�� � �ࠢ����� 䠩��� */
static  struct win_txt win60_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 2       ,-1 ,0,    "Comparing files",
  &POPUP_WIN_ATTR , 3       ,-1 ,0,     "",   /* file name */
  &POPUP_WIN_ATTR , 4       ,-1 ,0,     "and",
  &POPUP_WIN_ATTR , 5       ,-1 ,0,     "",   /* file name */
  };

struct  win     win60=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win60_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win60_txt
	};

/*-----------------------------------------------------*/
static  struct win_txt win61_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   10,
/* �������         */   35,
/* ��ப ⥪��    */   sizeof(win61_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win61_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win62_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "The two files appear to be",
  &POPUP_WIN_ATTR ,      4       ,-1 ,0,     "IDENTICAL",
  &POPUP_WIN_ATTR ,      6       ,-1 ,1,     "[ OK ]",
  };

struct  win     win62=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   9,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win62_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win62_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win63_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "The two files appear to be",
  &ERROR_WIN_ATTR ,      4       ,-1 ,0,     " DIFFERENT ",
  &POPUP_WIN_ATTR ,      6       ,-1 ,1,     "[ OK ]",
  };

struct  win     win63=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   9,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win63_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win63_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win64_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Error",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "Comparing two files",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     "[ OK ]",
  };

struct  win     win64=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win64_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win64_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win65_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Sorry. Can't compare",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "NON-REGULAR files",
  &ERROR_WIN_ATTR , 5       ,-1 ,1,     "[ OK ]",
  };

struct  win     win65=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   33,
/* ��ப ⥪��    */   sizeof(win65_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win65_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win66_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Search for the string",
  &POPUP_WIN_OUTL_ATTR , 3       ,-1 ,1,     "12345678901234567890123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      0       ,-1 ,0,     "View"
  };

struct  win     win66=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   54,
/* ��ப ⥪��    */   sizeof(win66_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win66_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win67_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Could not find a match",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "",
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Ok ]"
  };

struct  win     win67=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   26,
/* ��ப ⥪��    */   sizeof(win67_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win67_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win68_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 0       ,-1 ,0,     "Template",
  &POPUP_WIN_ATTR , 2       ,-1 ,1,     " Defaults  ",
  &POPUP_WIN_ATTR , 3       ,-1 ,1,     " Long name ",
  };

struct  win     win68=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   17,
/* ��ப ⥪��    */   sizeof(win68_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win68_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win69_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 0       ,-1 ,0,     "Recoding",
  &POPUP_WIN_ATTR , 2       ,-1 ,1,     " From... ",
  &POPUP_WIN_ATTR , 3       ,-1 ,1,     " To..... ",
  };

struct  win     win69=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   17,
/* ��ப ⥪��    */   sizeof(win69_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win69_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win70_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &POPUP_WIN_ATTR , 0       ,-1 ,0,     "",   /* From... To...*/
  &POPUP_WIN_ATTR , 2       ,-1 ,1,     " Alternative ",
  &POPUP_WIN_ATTR , 3       ,-1 ,1,     " ISO 8859-5  ",
  &POPUP_WIN_ATTR , 4       ,-1 ,1,     " KOI-8       ",
  &POPUP_WIN_ATTR , 5       ,-1 ,1,     " MS-Windows  ",
  };

struct  win     win70=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   17,
/* ��ப ⥪��    */   sizeof(win70_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win70_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win71_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Search for the string",
  &POPUP_WIN_OUTL_ATTR , 3       ,-1 ,1,     "12345678901234567890123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      0       ,-1 ,0,     "View"
  };

struct  win     win71=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   54,
/* ��ப ⥪��    */   sizeof(win71_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win71_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win72_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Could not find a match",
  &ERROR_WIN_ATTR , 3       ,2  ,0,    "", /* ������ */
  &ERROR_WIN_ATTR , 4       ,10 ,1,    "[ Ok ]"
  };

struct  win     win72=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   26,
/* ��ப ⥪��    */   sizeof(win72_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win72_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win73_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
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
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   19,
/* �������         */   63,
/* ��ப ⥪��    */   sizeof(win73_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win73_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win74_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "File not found",
  &ERROR_WIN_ATTR , 4       ,-1 ,1,    "[ Ok ]"
  };

struct  win     win74=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   23,
/* ��ப ⥪��    */   sizeof(win74_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win74_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win75_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,2  ,0,     "Show user (all - empty)",
  &POPUP_WIN_OUTL_ATTR , 3       ,-1 ,1,     "123456789012345678901234567890",
  &POPUP_WIN_ATTR ,      0       ,-1 ,0,     "Process Control"
  };

struct  win     win75=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   6,
/* �������         */   34,
/* ��ப ⥪��    */   sizeof(win75_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win75_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win76_txt[]=
  {/* ��ਡ���           Y        X   Act.    Text */
  &POPUP_WIN_ATTR ,      2       ,-1 ,0,     "Loading viewer",
  &POPUP_WIN_ATTR ,      4       ,-1 ,0,     "Please wait",
  };

struct  win     win76=
	{
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   7,
/* �������         */   20,
/* ��ப ⥪��    */   sizeof(win76_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win76_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win77_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
  &ERROR_WIN_ATTR , 5       ,12 ,1,    " Overwrite ",
  &ERROR_WIN_ATTR , 5       ,25 ,1,    " Cancel ",
  &ERROR_WIN_ATTR , 2       ,-1 ,0,    "Some files/dirs in specified target path",
  &ERROR_WIN_ATTR , 3       ,-1 ,0,    "already EXISTs. Overwrite them?"
  };

struct  win     win77=
	{
/* ���.����        */   &ERROR_WIN_ATTR,
/* ���.ࠬ��       */   &ERROR_WIN_ATTR,
/* ���.��⨢�.���. */   &ERROR_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   8,
/* �������         */   46,
/* ��ப ⥪��    */   sizeof(win77_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win77_txt
	};
/*-----------------------------------------------------*/
static  struct win_txt win78_txt[]=
  {/* ��ਡ���      Y        X   Act.    Text */
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
/* ���.����        */   &POPUP_WIN_ATTR,
/* ���.ࠬ��       */   &POPUP_WIN_ATTR,
/* ���.��⨢�.���. */   &POPUP_WIN_OUTL_ATTR,
/* Y ����.         */   -1,
/* X ���.          */   -1,
/* ��ப           */   12,
/* �������         */   51,
/* ��ப ⥪��    */   sizeof(win78_txt)/sizeof(struct win_txt),
/* ��뫪� �� ⥪�� */   win78_txt
	};
/*-----------------------------------------------------*/
