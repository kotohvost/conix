#include <sys/types.h>
#include <sys/stat.h>
#include "defs.h"
/****************************************************************************/
			filtype(mode)
/*              Определение типа файла
*****************************************************************************/
	unsigned mode;
{
	int     type;
/*==========================================================================*/
switch(mode & S_IFMT)
  {
   case S_IFDIR:
     type=FTYPE_DIR;
   break;

   case S_IFCHR:
     type=FTYPE_CHR;
   break;

   case S_IFBLK:
     type=FTYPE_BLK;
   break;

   case S_IFIFO:
     type=FTYPE_FIFO;
   break;

#ifndef NOIFNAM
   case S_IFNAM:
     type=FTYPE_NAM;
   break;
#endif

   case S_IFIFO | S_IFSOCK:
     type=FTYPE_SOFIFO;
   break;

   case S_IFLNK:
     type=FTYPE_SLN;
   break;

   case S_IFREG:
     type=FTYPE_REG;
   break;

   default:
     type=FTYPE_UNKNOWN;
     fprintf(stderr, "type=%d %u 0%o 0x%X\n",
     (mode & S_IFMT),
     (mode & S_IFMT),
     (mode & S_IFMT),
     (mode & S_IFMT)
     );
     getchar();
   break;
}
return(type);
}
