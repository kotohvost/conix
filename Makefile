# Makefile.build может быть перегенерирован mymkmf, поэтому он сделан отдельно
# Все изголения с Obj_* - для возможности одновременной компиляции на разных
# "платформах" с использованием NFS
SHELL=/bin/sh
default:
	@echo You have to set up SYSTEM variable and run mk
pc:
	cd Obj_pc; \
	make -f ../Makefile.build PRG=../conix_pc COMMENT=" " CFLAGS="-O -DSCO"
sun:
	cd Obj_sun; \
	pmake -f ../Makefile.build PRG=../conix_sun
#       make -f ../Makefile.build PRG=../conix_sun CC=gcc CFLAGS="-traditional -O" LDFLAGS=" "
#       /opt/SUNWspro/ParallelMake/bin/make -f ../Makefile.build PRG=../conix_sun
sol1:
	cd Obj_sol1; \
	make -f ../Makefile.build CFLAGS="-O -DNOIFNAM -DSOLARIS1 -DNOCOLORCURSES" PRG=../conix_sol1
i86pc:
	cd Obj_x86; \
	make -f ../Makefile.build CC=gcc CFLAGS="-traditional -O" LDFLAGS=" " PRG=../conix_x86
linux:
	cd Obj_lin; \
	make -f ../Makefile.build PRG=../conix_lin CFLAGS='-traditional -O -I/usr/include/ncurses -I/usr/include/bsd -DNCURSES -DNOIFNAM' LIBS='-lncurses'
FreeBSD:
	cd Obj_fbsd; \
	make -f ../Makefile.build PRG=../conix_fbsd CFLAGS='-traditional -O -DFREEBSD -DNCURSES -DNOIFNAM -Dconst=' LIBS='-lncurses'

instpc:
	mv conix_pc ${HOME}/tools_pc/conix
instsol1:
	mv conix_sol1 ${HOME}/tools_sol1/conix
instsun:
	mv conix_sun /usr/local/bin/conix
instx86:
	mv conix_x86 /network/local_solaris2_x86/bin/conix
