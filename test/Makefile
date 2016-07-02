#文件目录
INC_DIR=../inc
LIB_DIR=..
OBJ_DIR=../obj
SRC_DIR=.
GLOG_INC_DIR=$(INC_DIR)/glog

#修改编译器
ifneq ($(PLATFORM),PC)
ARCH = arm-hisiv100nptl-linux-
CFLAGS += -DARM
else
ARCH =
CFLAGS += -DPC  -Wno-unused-result
endif
CC=$(ARCH)gcc
CPP=$(ARCH)g++
AR=$(ARCH)ar
STRIP=$(ARCH)strip

send=send
recv=recv

ifeq ($(DEBUG),ON)
CFLAGS += -DDEBUG
endif
CFLAGS += -Wall -fPIC -g -O2 -fno-strict-aliasing
CFLAGS += -Wno-deprecated -Wno-unused-but-set-variable
CFLAGS += -I$(INC_DIR)
CFLAGS += -I$(GLOG_INC_DIR)

LDFLAGS += -L$(LIB_DIR)
LDFLAGS += -lclient -lpthread

#源文件

all: clean $(send) $(recv)
	cp $(send) $(recv) ~/nfs

$(send): send.o
	$(CPP) $(CFLAGS) send.o -o $(send) $(LDFLAGS)

send.o: send.c
	$(CPP) $(CFLAGS) -c $< -o $@

$(recv): recv.o
	$(CPP) $(CFLAGS) recv.o -o $(recv) $(LDFLAGS)

recv.o: recv.cc
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	-rm -f send.o recv.o $(send) $(recv)
