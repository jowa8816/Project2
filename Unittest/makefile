#*****************************************************************************
# Copyright (C) 2019 by Jon Warriner
#
# Redistribution, modification or use of this software in source or binary
# forms is permitted as long as the files maintain this copyright. Users are
# permitted to modify this and use it to learn about the field of embedded
# software. Jon Warriner and the University of Colorado are not liable for
# any misuse of this material.
#
#****************************************************************************
#
# @file makefile
# @brief makefile for the ring buffer test project
#
# This file defines the make steps to build the
# ring buffer test project.  
#
# @author Jon Warriner
# @date March 14, 2019
# @version 1.0
#

INC = ../inc/
SRC = ../src/

ringtest : unittest.o ring.o
	cc -o ringtest unittest.o ring.o -lcunit
	
longtest : unittest_long.o ring.o
	cc -o ringtest unittest.o ring.o -lcunit

unittest.o : unittest.c $(INC)ring.h
	cc -Wall -Werror -I$(INC) -c unittest.c

ring.o : $(SRC)ring.c $(INC)ring.h
	cc -Wall -Werror -I$(INC) -c $(SRC)ring.c

unittest_long.o : unittest_long.c $(INC)ring.h
	cc -Wall -Werror -I$(INC) -c unittest_long.c

clean :
	rm ringtest unittest.o ring.o unittest_long.o

test : ringtest
	./ringtest

test_long : longtest
	./longtest

