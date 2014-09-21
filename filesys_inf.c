#include "defs.h"

/****************************************************************************/
		filesys_inf()
/*              �⥭�� ���ଠ樨 � 䠩����� ��⥬�
		� �⢥�������� �������� panels[]
		⥪�騩 ��⠫�� ��⠭���������� � read_to_panel()
	panels[outpanel].total    * �ᥣ� ���� �� ��᪥ *
			.avail    * ����㯭� ��� ��।������ ��-super*
			.free     * ᢮����� *
			.bs       * ࠧ��� ����� *
			.fragm    * fragment size *
			.itotal   * i-nodes �ᥣ� *
			.iavail   * i-nodes ����㯭� ��� ��।������ ��-super *
			.ifree    * i-nodes ᢮����� *
			.*fstype  * ⨯ 䠩����� ��⥬� *
			.*fsname  * ��� 䠩����� ��⥬� *
*****************************************************************************/

{
extern  int     outpanel;       /*��ࠡ��뢠���� ������ */
extern  struct  panel   panels[NPANELS];

#if defined( __svr4__ ) || defined( __SVR4 )
/*######################## SOLARIS 2.x & other SVR4 ########################*/

#include <sys/statvfs.h>
#define BUF2_SIZ (sizeof(buf.f_fstr)+1)

static  struct  statvfs buf;            /* ���� ��� statvfs() */
static  char    *name=".";            /* ��� 䠩�� �� 䠩����� ��⥬� */
static  char    buf_fstr [BUF2_SIZ];    /*���� ��� �뢮�� buf.f_fstr*/
/*==========================================================================*/

if ( statvfs (name, &buf) != 0 ){
  panels[outpanel].total  = panels[outpanel].avail  =
  panels[outpanel].free   = panels[outpanel].bs     =
  panels[outpanel].fragm  = panels[outpanel].itotal =
  panels[outpanel].iavail = panels[outpanel].ifree  = 0L;

  panels[outpanel].fstype = panels[outpanel].fsname = "\0";
}

if(buf.f_frsize)
  {
    panels[outpanel].total = buf.f_frsize*(dev_data)buf.f_blocks;
    panels[outpanel].avail = buf.f_frsize*(dev_data)buf.f_bavail;
    panels[outpanel].free  = buf.f_frsize*(dev_data)buf.f_bfree;
  }else if(buf.f_bsize){
    panels[outpanel].total = buf.f_bsize*(dev_data)buf.f_blocks;
    panels[outpanel].avail = buf.f_bsize*(dev_data)buf.f_bavail;
    panels[outpanel].free  = buf.f_bsize*(dev_data)buf.f_bfree;
  }else{
    panels[outpanel].total = 1024*(dev_data)buf.f_blocks;
    panels[outpanel].avail = 1024*(dev_data)buf.f_bavail;
    panels[outpanel].free  = 1024*(dev_data)buf.f_bfree;
  }

panels[outpanel].bs     = buf.f_bsize;
panels[outpanel].fragm  = buf.f_frsize;
panels[outpanel].itotal = buf.f_files;
panels[outpanel].iavail = buf.f_favail;
panels[outpanel].ifree  = buf.f_ffree;
panels[outpanel].fstype = buf.f_basetype;

strncpy(buf_fstr , buf.f_fstr    , BUF2_SIZ-1 );
buf_fstr[BUF2_SIZ-1] = '\0';
panels[outpanel].fsname = buf_fstr;

#elif defined( LINUX )
/*########################### Linux ########################################*/
#include <sys/vfs.h>

	struct  statfs  buf;            /* ���� ��� statfs() */
	int     i;
static  char    *name=".";            /* ��� 䠩�� �� 䠩����� ��⥬� */
struct  fstypes
	{
	char    *name;
	long    type;
	};
static  struct fstypes fstypes[]=
	{
	      {"EXT2_OLD_SUPER_MAGIC" , 0xEF51    },
	      {"EXT2_SUPER_MAGIC"     , 0xEF53    },
	      {"EXT_SUPER_MAGIC"      , 0x137D    },
	      {"ISOFS_SUPER_MAGIC"    , 0x9660    },
	      {"MINIX_SUPER_MAGIC"    , 0x137F    },/* orig. minix */
	      {"MINIX_SUPER_MAGIC2"   , 0x138F    },/* 30 char minix */
	      {"NEW_MINIX_SUPER_MAGIC", 0x2468    },/* minix V2 */
	      {"MSDOS_SUPER_MAGIC"    , 0x4d44    },
	      {"NFS_SUPER_MAGIC"      , 0x6969    },
	      {"PROC_SUPER_MAGIC"     , 0x9fa0    },
	      {"_XIAFS_SUPER_MAGIC"   , 0x012FD16D},
	      {NULL                   , 0         }
	};
/*==========================================================================*/

panels[outpanel].total  = panels[outpanel].avail  =
panels[outpanel].free   = panels[outpanel].bs     =
panels[outpanel].fragm  = panels[outpanel].itotal =
panels[outpanel].iavail = panels[outpanel].ifree  = -1L;
panels[outpanel].fstype = panels[outpanel].fsname = "\0";

if ( statfs (name, &buf) == 0 ) {
    if(buf.f_bsize > 0){
      if(buf.f_blocks>0) set_check(&panels[outpanel].total, (dev_data)buf.f_blocks,  (dev_data)buf.f_bsize);
      if(buf.f_bavail>0) set_check(&panels[outpanel].avail, (dev_data)buf.f_bavail, (dev_data)buf.f_bsize);
      if(buf.f_bfree>0)  set_check(&panels[outpanel].free , (dev_data)buf.f_bfree, (dev_data)buf.f_bsize);
    }

    panels[outpanel].bs     = buf.f_bsize;
    panels[outpanel].itotal = buf.f_files;
    panels[outpanel].ifree  = buf.f_ffree;

    for ( i=0; fstypes[i].name && fstypes[i].type!=buf.f_type; i++);
    if ( fstypes[i].name )
      {
	panels[outpanel].fstype = fstypes[i].name; /* �㫥��� 㪠��⥫� ����� �� �ன��� */
      }else{
	panels[outpanel].fstype = "Some Unknown!";
      }

}

#elif defined( FREEBSD )
/*########################### FreeBSD ########################################*/
#include <sys/param.h>
#include <sys/mount.h>

	struct  statfs  buf;            /* ���� ��� statfs() */
	int     i;
static  char    *name=".";            /* ��� 䠩�� �� 䠩����� ��⥬� */
struct  fstypes
	{
	char    *name;
	long    type;
	};
static  struct fstypes fstypes[]=
	{
	      {"UFS"    , MOUNT_UFS   },
	      {"NFS"    , MOUNT_NFS   },
	      {"MFS"    , MOUNT_MFS   },
	      {"MSDOS"  , MOUNT_MSDOS },
	      {"LFS"    , MOUNT_LFS   },
	      {"LOFS"   , MOUNT_LOFS  },
	      {"FDESC"  , MOUNT_FDESC },
	      {"PORTAL" , MOUNT_PORTAL},
	      {"NULL"   , MOUNT_NULL  },
	      {"UMAP"   , MOUNT_UMAP  },
	      {"KERNFS" , MOUNT_KERNFS},
	      {"PROCFS" , MOUNT_PROCFS},
	      {"AFS"    , MOUNT_AFS   },
	      {"CD9660" , MOUNT_CD9660},
	      {"UNION"  , MOUNT_UNION },
	      {NULL     , 0           }
	};
/*==========================================================================*/

panels[outpanel].total  = panels[outpanel].avail  =
panels[outpanel].free   = panels[outpanel].bs     =
panels[outpanel].fragm  = panels[outpanel].itotal =
panels[outpanel].iavail = panels[outpanel].ifree  = -1L;
panels[outpanel].fstype = panels[outpanel].fsname = "\0";

if ( statfs (name, &buf) == 0 ) {
    if(buf.f_bsize > 0){
      if(buf.f_blocks>0) set_check(&panels[outpanel].total, (dev_data)buf.f_blocks,  (dev_data)buf.f_bsize);
      if(buf.f_bavail>0) set_check(&panels[outpanel].avail, (dev_data)buf.f_bavail, (dev_data)buf.f_bsize);
      if(buf.f_bfree>0)  set_check(&panels[outpanel].free , (dev_data)buf.f_bfree, (dev_data)buf.f_bsize);
    }

    panels[outpanel].bs     = buf.f_bsize;
    if(buf.f_files>0) panels[outpanel].itotal = buf.f_files;
    if(buf.f_ffree>0) panels[outpanel].ifree  = buf.f_ffree;

    for ( i=0; fstypes[i].name && fstypes[i].type!=buf.f_type; i++);
    if ( fstypes[i].name )
      {
	panels[outpanel].fstype = fstypes[i].name; /* �㫥��� 㪠��⥫� ����� �� �ன��� */
      }else{
	panels[outpanel].fstype = "Some Unknown!";
      }

}

#elif defined( SCO )

/*######################### SCO UNIX 3.2 & others ##########################*/
#include <sys/statfs.h>

/* �� ����⨧�, �� SCO ������, �� buf.f_bsize==1024,
   � ࠧ��� ����砥��� � 2 ࠧ� �����, 祬 ����!
 */
#define BLKSIZE 512

#define BUFNAME_SIZ (sizeof(buf.f_fname)+1)
#define BUFPACK_SIZ (sizeof(buf.f_fpack)+1)

	struct  statfs  buf;            /* ���� ��� statfs() */
static  char    *name=".";            /* ��� 䠩�� �� 䠩����� ��⥬� */
static  char    buf_name[BUFNAME_SIZ];
static  char    buf_pack[BUFPACK_SIZ];
static  char    buf_fstype[10];         /* �᫮��� ���祭�� ⨯� 䠩����� ��⥬�*/
/*==========================================================================*/

if ( statfs (name, &buf, sizeof(buf), 0) != 0 ){
  panels[outpanel].total  = panels[outpanel].avail  =
  panels[outpanel].free   = panels[outpanel].bs     =
  panels[outpanel].fragm  = panels[outpanel].itotal =
  panels[outpanel].iavail = panels[outpanel].ifree  = 0L;

  panels[outpanel].fstype = panels[outpanel].fsname = "\0";
}

strncpy(buf_name , buf.f_fname    , BUFNAME_SIZ-1 );
strncpy(buf_pack , buf.f_fpack    , BUFPACK_SIZ-1 );

buf_pack[BUFPACK_SIZ-1] = buf_name[BUFNAME_SIZ-1] = '\0';

if(buf.f_frsize)
  {
    set_check(&panels[outpanel].total, (dev_data)buf.f_frsize, (dev_data)buf.f_blocks);
    set_check(&panels[outpanel].avail, (dev_data)buf.f_frsize, (dev_data)buf.f_bfree);
    panels[outpanel].free  = -1; /* � ������� statfs �⤥�쭮 ���, ���⮬� ���⠢�� ==free */
  }else{
    set_check(&panels[outpanel].total, (dev_data)BLKSIZE, (dev_data)buf.f_blocks);
    set_check(&panels[outpanel].avail, (dev_data)BLKSIZE, (dev_data)buf.f_bfree);
    panels[outpanel].free  = -1; /* � ������� statfs �⤥�쭮 ���, ���⮬� ���⠢�� ==free */
  }

panels[outpanel].bs     = buf.f_bsize;
panels[outpanel].fragm  = buf.f_frsize;
panels[outpanel].itotal = buf.f_files;
panels[outpanel].iavail = /* � ������� statfs �⤥�쭮 ���, ���⮬� ���⠢�� ==ifree */
panels[outpanel].ifree  = buf.f_ffree;

if(buf_name[0])
  {
    panels[outpanel].fsname = buf_name;
  }else{
    panels[outpanel].fsname = buf_pack;
  }

sprintf(buf_fstype, "%u", buf.f_fstyp);
panels[outpanel].fstype = buf_fstype;

#endif
}

#if !(defined( __svr4__ ) || defined( __SVR4 ))
/*######################### SCO UNIX 3.2 & others ��� long long ##########################*/

/****************************************************************************/
			set_check(targ, mul1, mul2)
/*                      ����⪠ ��६������ mul1 � mul2
			� �������� १���� � *targ
*****************************************************************************/
	dev_data        *targ;
	dev_data        mul1;
	dev_data        mul2;
{
	ulong        tmp;
/*==========================================================================*/
tmp = mul1 * (ulong) mul2;
if(tmp/mul1 != mul2) tmp=(-1);
*targ = tmp;
}
#endif
