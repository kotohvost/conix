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
/************************************ ���� *********************************/
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
/********* ��।������ ��ਡ�⮢ �⤥�쭮 ��梥稢����� ����⮢ **********/
#define MAIN_PANEL_ATTR          (atr[0 ])   /* �᭮���� ���� ������ */
#define MAIN_PANEL_TOP_ATTR      (atr[1 ])   /* ������ ��ப� ������ */
#define MAIN_PANEL_BOT_OUT_ATTR  (atr[2 ])   /* ������ ��ப� ������ � �⬥�.䠩����*/
#define MAIN_PANEL_PTR_ATTR      (atr[3 ])   /* ��ப�-㪠��⥫� (�� �뤥����� �.)*/
#define MAIN_PANEL_OUTL_ATTR     (atr[4 ])   /* �뤥����� 䠩� �� ��.������*/
#define MAIN_PANEL_OUTL_PTR_ATTR (atr[5 ])   /* ��ப�-㪠��⥫� (�뤥����� �.)*/
#define MAIN_PANEL_BOX_ATTR      (atr[6 ])   /* ࠬ�� �᭮���� ������ */
#define MAIN_PANEL_DIR_ATTR      (atr[7 ])   /* ⥪�騩 ��⠫�� ��⨢��� ������*/
#define BOTT_LINE_TEXT_ATTR      (atr[8 ])   /* ⥪�� ������ ��ப�*/
#define BOTT_LINE_KEYS_ATTR      (atr[9 ])   /* ����� ������ � ������ ��ப�*/
#define TOP_MENU_ATTR            (atr[10])   /* ���孥� ���� (�����.�㭪��)*/
#define TOP_MENU_OUTL_ATTR       (atr[11])   /* ���孥� ���� (��⨢�� �㭪�)*/
#define POPDN_WIN_ATTR           (atr[12])   /* �믠���騥 ���� (��.����)*/
#define POPDN_WIN_BOX_ATTR       (atr[13])   /* �믠���騥 ���� (ࠬ��)*/
#define POPDN_WIN_OUTL_ATTR      (atr[14])   /* �믠���騥 ���� (���.��ப�)*/
#define POPDN_WIN_HDRS_ATTR      (atr[15])   /* �믠���騥 ���� (���������)*/
#define POPUP_WIN_ATTR           (atr[16])   /* �ᯫ뢠�騥 ���� (��.����)*/
#define POPUP_WIN_BOX_ATTR       (atr[17])   /* �ᯫ뢠�騥 ���� (ࠬ��)*/
#define POPUP_WIN_OUTL_ATTR      (atr[18])   /* �ᯫ뢠�騥 ���� (���.��ப�)*/
#define SHADOW_ATTR              (atr[19])   /* ⥭� */
#define ERROR_WIN_ATTR           (atr[20])   /* ���� ᮮ�饭�� �� �訡��� */
#define ERROR_WIN_OUTL_ATTR      (atr[21])   /* ���� ᮮ�饭�� �� �訡��� (���.��ப�)*/
#define NO_DRAW_ATTR             (atr[22])   /* ���뢮����� ���� ���� */
#define SUPER_PROMPT_ATTR        (atr[23])   /* Prompt ��� super-user */
#define NUM_ATTRS                24
/******************************* ��㣨� ����⠭�� ***************************/
#define NPANELS 2       /* ���ᨬ��쭮� ������⢮ �������*/
#define OUTNAMELEN 12   /* ������⢮ �뢮����� ᨬ����� ����� 䠩�� */
#define BEG_NAME   3    /* ������ �뢮�� ����� 䠩�� �� ��砫� ���� */
#define MARKED     1    /* 䠩� �⬥祭 */
#define VERT            1   /*ࠧ�襭� ���⨪��쭮� ��६�饭�� �� ���� */
#define HOR             2   /*ࠧ�襭� ��ਧ��⠫쭮� ��६�饭�� �� ���� */
#define OUT             4   /*ࠧ�襭 ��室 �� ����୮� ���ࠢ����� ���室� �� ���� */
#define A_LEFT          (-2)/*��室 �� ���� �� ��५�� ����� */
#define A_RIGHT         (-3)/*��室 �� ���� �� ��५�� ��ࠢ� */
#define A_UP            (-4)/*��室 �� ���� �� ��५�� ����� */
#define A_DOWN          (-5)/*��室 �� ���� �� ��५�� ���� */
#define KEYCAP  "CONIXKEYS" /*��६����� �।�, ����� �஢������ ��� ����祭�� ����஥� ����������*/
#define FROM_TOP        4 /* ����� ����ࠨ������ �㭪⮢ ᢥ���*/
#define FROM_BOTT       4 /* ����� ����ࠨ������ �㭪⮢ ᭨��*/
#define FILTERLEN       20/* ���ᨬ��쭠� ����� ��ப� 䨫��� */
#define LOGNM_MAX       10 /* ���� ������⢮ ᨬ����� ����� ���짮��⥫� */

#ifdef SOLARIS1
#define SYS_NMLN    20
#endif

#if defined(__svr4__) || defined(__SVR4)
#define dev_data offset_t
#else
#define dev_data daddr_t
#endif

/*********************************** ���� 䠩��� ****************************/
/* ���祭�� ��।������ FTYPE_* �ᯮ������� �� ���஢�� */
#define FTYPE_DIR     1
#define FTYPE_SLN     2
#define FTYPE_CHR     3
#define FTYPE_BLK     4
#define FTYPE_FIFO    5
#define FTYPE_NAM     6
#define FTYPE_UNKNOWN 7
#define FTYPE_SOFIFO  8	/* Socket & Fifo simultaneously :( */
#define FTYPE_REG     9

/*********************************** ���� �����  ****************************/
#define MAXFIELDS     11        /* ����. ������⢮ ����� (� �� ��ਠ�⮢)*/
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

/*********************************** ��������� *****************************/
#define ESC     033
#define TAB     '\t'
#define ENTER   '\n'
#ifndef CTRL
#define CTRL(key) (037 & key)
#endif
#define INSERT  CTRL('T')
#define K_INSRT (ERROR_KEY|CTRL('T'))
#define PRED    CTRL('E')            /* ������� ������� (�� ���ਨ)*/
#define K_PRED  (ERROR_KEY|CTRL('E'))
#define NEXT    CTRL('X')            /* ᫥����� ������� (�� ���ਨ)*/
#define K_NEXT  (ERROR_KEY|CTRL('X'))
#define BACKSP  CTRL('H')            /* ������ BACKSPACE */
#define ONOFF   CTRL('O')            /* ������ ���/�몫 ������� */
#define K_ONOFF (ERROR_KEY|CTRL('O'))
#define SWAPP   CTRL('U')            /* ������ ᬥ�� ���⠬� ������� */
#define K_SWAPP (ERROR_KEY|CTRL('U'))
#define CDROOT  CTRL('\\')           /* ������ ���室� � ��୥��� ��⠫��*/
#define K_CDROOT (ERROR_KEY|CDROOT)
#define REFRESH CTRL(']')            /* ������ ��ॢ뢮�� �࠭� */
#define K_REFRESH (ERROR_KEY|REFRESH)
#define REREAD  CTRL('R')            /* ������ ������뢠��� ��⠫��� */
#define K_REREAD  (ERROR_KEY|REREAD)
#define FIND    CTRL('K')            /* ������ ���᪠ � ⥪�饬 ��⠫��� */
#define K_FIND    (ERROR_KEY|FIND)
#define QUIT    CTRL('C')            /* ������ ��室� */
#define K_QUIT    (ERROR_KEY|QUIT)
#define DELETE  0177      /* ������ DELETE */
#define INFO    CTRL('L')            /* ������ �뢮�� ���ﭨ� (Info) */
#define K_INFO  (ERROR_KEY|CTRL('L'))
#define GOOTH    CTRL('G')            /* ������ �뢮�� ���ﭨ� (Info) */
#define K_GOOTH  (ERROR_KEY|GOOTH)
#define PROC     CTRL('P')            /* ������ �뢮�� ���ﭨ� (Info) */
#define K_PROC   (ERROR_KEY|PROC)
#define LONGNM   CTRL('N')            /* ������ �뢮�� ������� ���� */
#define K_LONGNM (ERROR_KEY|LONGNM)
#define DFLT     CTRL('D')            /* ������ �뢮�� ������� ������ �� 㬮�砭�� */
#define K_DFLT   (ERROR_KEY|DFLT)
#define DWIDTH   CTRL('W')            /* ������ �뢮�� ������� �� ���� �࠭ */
#define K_DWIDTH (ERROR_KEY|DWIDTH)
/********************************* �������� ********************************/
struct itembuf {
  unsigned short    nname; /* ��� 䠩�� (����� ��ࢮ�� ᨬ�. � panel.names[])*/
  unsigned short    attrs; /* ����७��� ��ਡ���*/
	   mode_t   mode;  /* ��ਡ��� 䠩�� (fstat) */
	   ino_t    inum;  /* ����� i-㧫� */
	   nlink_t  nlink; /* ������⢮ HardLink */
	   off_t    size;  /* ࠧ��� 䠩�� */
	   time_t   mtime; /* �६� ����䨪�樨 */
	   uid_t    owner; /* �������� 䠩�� */
	   gid_t    group; /* ��㯯� �������� 䠩�� */
};
struct panel {
  WINDOW *win;          /* ࠡ�祥 ���� ������ */
  short  on;            /* 0-�몫�祭�, 1-Full, 2-Info, 3-Brief */
  short  save_on;       /* ��࠭����� ���祭�� on */
  short  curfile;       /* ����� ⥪�饣� 䠩��*/
  short  firstfile;     /* ����� ��ࢮ�� �뢮������ 䠩��*/
  short  lastfile;      /* ����� ��᫥����� �뢮������ 䠩��*/
  char   *full_dir;     /* ������ ��� ⥪�饣� ��⠫��� */
  int    full_dir_size; /* ࠧ��� �����, �⢥������ ��� full_dir*/
  struct itembuf *itembuf;/* ���� ����ᥩ ⥪�饣� ��⠫���*/
  short  nitems;        /* ���-�� �뤥������ ����⮢ � dirbuf */
  short  nfiles;        /* ���-�� ������⢮������ ����⮢ � dirbuf */
  short  lines;         /* ������⢮ ��ப � ���� ������*/
  short  cols;          /* ������⢮ ����権 � ���� ������*/
  short  lnused;        /* ������⢮ ���ᯮ��㥬�� ��ப � ���� ������*/
  short  sort;          /* ⨯ ���஢�� 0-�� ���஢��� */
  short  outfields[MAXFIELDS];/* ����� �뢮����� ����⮢ fields */
  char   *names;        /* ���� �࠭���� ���� 䠩��� ⥪�饣� ��⠫���*/
  int    names_len;     /* ࠧ��� names */
  int    selected;      /* ������⢮ �⬥祭��� 䠩���*/
  long   size_sel;      /* �㬬��� ࠧ��� �⬥祭��� 䠩���*/
  char   filter_read[FILTERLEN];/* 䨫��� ��� �⠥��� 䠩��� */
  char   filter_noread[FILTERLEN];/* 䨫��� ��� ���⠥��� 䠩��� */
  dev_data      total;  /* �ᥣ� ���� �� ��᪥ */
  dev_data      avail;  /* ����㯭� ��� ��।������ ��-super*/
  dev_data      free;   /* ᢮����� */
  dev_data      bs;     /* ࠧ��� ����� */
  dev_data      fragm;  /* fragment size */
  dev_data      itotal; /* i-nodes �ᥣ� */
  dev_data      iavail; /* i-nodes ����㯭� ��� ��।������ ��-super */
  dev_data      ifree;  /* i-nodes ᢮����� */
  char          *fstype;/* ⨯ 䠩����� ��⥬� */
  char          *fsname;/* ��� 䠩����� ��⥬� */
};

struct  win_txt
	{
	chtype  *attrs;         /*��ਡ��� ��ப�*/
	short   y;              /*y ���� ��砫� ⥪��*/
	short   x;              /*x ���� ��砫� ⥪��*/
	short   active;         /*�ਧ��� ���������� �롮� �㭪�*/
	char    *txt;           /*��ப� ⥪��*/
	};

struct  win
	{
	chtype  *attr;          /*��ਡ��� ����*/
	chtype  *attr_box;      /*��ਡ��� ࠬ�� ����*/
	chtype  *attr_astr;     /*��ਡ��� ��⨢���� ��ப�*/
	short   y;              /*Y ���孥� ��ப� ����*/
	short   x;              /*X ����� ������� ����*/
	short   lines;          /*���-�� ��ப ����*/
	short   cols;           /*���-�� ������� � ����*/
	short   npunkt;         /*������⢮ ��ப ⥪��*/
	struct  win_txt *txts;  /*⥪���*/
	};
struct  inp_lst                 /* ������� ᯨ᪠ ����� ��� inp_menu() */
	{
	char    *str;           /* ��ப� ��� ����� */
	char    fld_len;        /* ࠧ��� ���� ����� */
	size_t  str_len;        /* ࠧ��� ����� str */
	int     npunkt;         /* ����� �㭪� ���� � ��ப�� ����� */
	};

struct  fields
	{
	short   fieldtype;      /* ⨯ ���� */
	short   fieldsize;      /* ࠧ��� ���� */
	char    *fieldname;     /* ᨬ���쭮� ��� ���� ��� �뢮�� */
	};
#ifdef NOCOLORCURSES
#define start_color() {}
#define has_colors() {0};
#define COLORS        2
#define can_change_color()  {0}
#define init_pair(npair, col1, col2) {}
#endif
