#文件目录
INC_DIR=./inc
LIB_DIR=./lib
OBJ_DIR=./obj
SRC_DIR=./src
GLOG_INC_DIR=$(INC_DIR)/glog

#修改编译器
ifeq ($(PLATFORM),ARM)
ARCH = arm-hisiv100nptl-linux-
CFLAGS += -DARM
else
ARCH =
CFLAGS += -DPC -Wno-unused-result
endif
CC=$(ARCH)gcc
CPP=$(ARCH)g++
AR=$(ARCH)ar
STRIP=$(ARCH)strip

TARGET=gate
TEST=test
STATIC_LIB=libclient.a

ifeq ($(DEBUG),ON)
CFLAGS += -DDEBUG
endif
CFLAGS += -Wall -fPIC -g -O2 -fno-strict-aliasing
CFLAGS += -Wno-deprecated -Wno-unused-but-set-variable
CFLAGS += -I$(INC_DIR)
CFLAGS += -I$(GLOG_INC_DIR)

LDFLAGS += -lpthread $(LIB_DIR)/libglog.a

#源文件
SRCS=$(wildcard $(SRC_DIR)/*.cc )
OBJS:=$(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRCS))

LIB_OBJS = $(OBJ_DIR)/CirBuffer.o \
	$(OBJ_DIR)/RecBuf.o \
	$(OBJ_DIR)/Client.o \
	$(OBJ_DIR)/Connect.o \
	$(OBJ_DIR)/Crc.o

TAR_OBJS = $(OBJ_DIR)/Server.o \
	$(OBJ_DIR)/GLogHelper.o \
	$(OBJ_DIR)/Crc.o

all:  $(OBJS) $(TARGET) $(STATIC_LIB)
	cp $(TARGET) ./test
	mv $(TARGET) ~/nfs
	mv $(STATIC_LIB) ./test

$(TARGET): $(TAR_OBJS)
	$(CPP) $(CFLAGS) $(TAR_OBJS) -o $(TARGET) $(LDFLAGS)

$(STATIC_LIB): $(LIB_OBJS)
	$(AR) crus $@ $(LIB_OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	-rm -f  $(TARGET) $(STATIC_LIB) $(OBJS)
