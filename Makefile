#文件目录
INC_DIR=./inc
LIB_DIR=./lib
OBJ_DIR=./obj
SRC_DIR=./src
GLOG_INC_DIR=$(INC_DIR)/glog

#修改编译器
ifeq ($(PLATFORM),HISI)
	ARCH = arm-hisiv100nptl-linux-
	CFLAGS += -DARM
else ifeq ($(PLATFORM),PI)
	ARCH = arm-bcm2708-linux-gnueabi-
	CFLAGS += -DPI
	LIB_DIR=./lib/pi
else
	ARCH =
	CFLAGS += -DPC -Wno-unused-result
	LIB_DIR=./lib/pc
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

LDFLAGS += -L$(LIB_DIR)
LDFLAGS += -lpthread -lglog

#源文件
SRCS=$(wildcard $(SRC_DIR)/*.cc )
OBJS:=$(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRCS))

LIB_OBJS = $(OBJ_DIR)/CirBuffer.o \
	$(OBJ_DIR)/RecBuf.o \
	$(OBJ_DIR)/Client.o \
	$(OBJ_DIR)/Connect.o \
	$(OBJ_DIR)/Crc.o

TAR_OBJS = $(OBJ_DIR)/Server.o \
	$(OBJ_DIR)/CirBuffer.o \
	$(OBJ_DIR)/RecBuf.o \
	$(OBJ_DIR)/GLogHelper.o \
	$(OBJ_DIR)/Crc.o

all:  clean $(OBJS) $(TARGET) $(STATIC_LIB)
	@cp $(TARGET) /opt/nfs/

$(TARGET): $(TAR_OBJS)
	@$(CPP) $(CFLAGS) $(TAR_OBJS) -o $(TARGET) $(LDFLAGS)
	@echo "	LD" $(notdir $@)

$(STATIC_LIB): $(LIB_OBJS)
	@$(AR) crus $@ $(LIB_OBJS)
	@echo "	AR" $(notdir $@)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@$(CPP) $(CFLAGS) -c $< -o $@
	@echo "	CC" $(notdir $@)

clean:
	-@rm -rf  $(TARGET) $(STATIC_LIB) $(OBJS)
