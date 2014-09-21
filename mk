#!/bin/csh
setenv SHELL /bin/sh
switch ( $SYSTEM )
  case sco*:
  case pc*:
    make pc   COMMENT= |& tee Prot.pc
  breaksw
  case SunOS:
    rm -f Prot.sun
    date >Prot.sun
    pmake sun  COMMENT='-mr,"`date`"'  |& tee -a Prot.sun
    date >>Prot.sun
  breaksw
  case i86pc:
    make i86pc COMMENT='-mr,"`date`"'  |& tee Prot.x86
  breaksw
  case sol1:
    make sol1  |& tee Prot.sol1
  breaksw
  case VAX:
    make sun  COMMENT=1 NOCOLORCURSES=1 |& tee Prot.vax
  breaksw
  case Linux:
    date >Prot.lin
    make linux COMMENT= |& tee -a Prot.lin
    date >>Prot.lin
  breaksw
  case FreeBSD:
    date >Prot.fbsd
    make FreeBSD COMMENT= |& tee -a Prot.fbsd
    date >>Prot.fbsd
  breaksw
  default
    echo '========================================================'
    echo '=   Unknown system type, use "make pc" or "make sun"   ='
    echo '========================================================'
  breaksw
endsw
