#
# Copyright (C) 1988-2010, Nantian Co., Ltd.
#
# $Header: /atmph/atmp20/tsp30/MessageQ/makefile,v 1.2 2010/11/04 15:12:20 mymtom Exp $
#
# $Log: makefile,v $
# Revision 1.2  2010/11/04 15:12:20  mymtom
# REL_1_2
#
# Revision 1.1.1.1  2010/08/14 20:19:29  cvsadmin
# Initial import
#
# Revision 1.2  2010/07/27 06:18:27  mymtom
# inc30 to relative direcotry
#
#
 
#forLinux
CC=    gcc -g -fPIC


AR=     ar
DEBUG=
#PLAT_CFLAGS= -qcpluscmt

CFLAGS= $(DEBUG) $(PLAT_CFLAGS) -I../../inc30

#SCO
SO_FLAG = -shared

.SUFFIXES:	 .c .o .a .so

#all:	libdmqapi.a libmspapi20.a
all:	libmspapi30.a

.c.a:
	rm -f $(HOME)/lib30/$*.so
	$(CC) $(SO_FLAG)  $(CFLAGS)  -o $(HOME)/lib30/$*.so $*.c
#	@rm $*.o 

clean:
	$(RM) *.o     

