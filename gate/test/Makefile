#文件目录
INC_DIR=../inc
LIB_DIR=../lib
OBJ_DIR=../obj
SRC_DIR=.
GLOG_INC_DIR=$(INC_DIR)/glog

#修改编译器
ifeq ($(PLATFORM),ARM)
ARCH = arm-hismall-linux-
CFLAGS += -DARM
else
ARCH =
CFLAGS += -DPC
endif
CC=$(ARCH)gcc
CPP=$(ARCH)g++
AR=$(ARCH)ar
STRIP=$(ARCH)strip

BZQ=bzq
SCHEDUL=schedul

ifeq ($(DEBUG),ON)
CFLAGS += -DDEBUG
endif
CFLAGS += -Wall -fPIC -g -O2 -fno-strict-aliasing
CFLAGS += -Wno-deprecated -Wno-unused-but-set-variable -Wno-unused-result
CFLAGS += -I$(INC_DIR)
CFLAGS += -I$(GLOG_INC_DIR)

LDFLAGS += ./libclient.a $(LIB_DIR)/libglog.a  -lpthread 

#源文件

all: clean $(BZQ) $(SCHEDUL)

$(BZQ): bzq.o
	$(CPP) $(CFLAGS) bzq.o -o $(BZQ) $(LDFLAGS)
	
bzq.o: bzq.c
	$(CPP) $(CFLAGS) -c $< -o $@

$(SCHEDUL): schedul.o
	$(CPP) $(CFLAGS) schedul.o -o $(SCHEDUL) $(LDFLAGS)

schedul.o: schedul.cc
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	-rm -f bzq.o schedul.o $(BZQ) $(SCHEDUL)
