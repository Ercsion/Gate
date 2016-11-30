#文件目录
INC_DIR=../inc
LIB_DIR=../
OBJ_DIR=../obj
SRC_DIR=.
GLOG_INC_DIR=$(INC_DIR)/glog

#修改编译器
ifeq ($(PLATFORM),HISI)
	ARCH = arm-hisiv100nptl-linux-
	CFLAGS += -DARM
else ifeq ($(PLATFORM),PI)
	ARCH = arm-bcm2708-linux-gnueabi-
	CFLAGS += -DPI
	LIB_DIR+=-L../lib/pi
else
	ARCH =
	CFLAGS += -DPC -Wno-unused-result
	LIB_DIR+=-L../lib/pc
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
CFLAGS += -Wall -fPIC -g -fno-strict-aliasing
CFLAGS += -Wno-deprecated -Wno-unused-but-set-variable
CFLAGS += -I$(INC_DIR)
CFLAGS += -I$(GLOG_INC_DIR)

LDFLAGS += -L$(LIB_DIR)
LDFLAGS += -lclient -lpthread

#源文件

all: clean $(send) $(recv)
	cp $(send) $(recv) /opt/nfs

$(send): send.o
	$(CPP) $(CFLAGS) send.o -o $(send) $(LDFLAGS)
	@echo "	CC" $(notdir $@)

send.o: send.c
	$(CPP) $(CFLAGS) -c $< -o $@
	@echo "	CC" $(notdir $@)

$(recv): recv.o
	$(CPP) $(CFLAGS) recv.o -o $(recv) $(LDFLAGS)
	@echo "	CC" $(notdir $@)

recv.o: recv.cc
	$(CPP) $(CFLAGS) -c $< -o $@
	@echo "	CC" $(notdir $@)

clean:
	-@rm -rf send.o recv.o $(send) $(recv)
