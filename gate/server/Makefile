#文件目录
CURRENT_PATH=$(shell pwd)
COMMON_DIR=$(CURRENT_PATH)/../common
COM_INC_DIR=$(COMMON_DIR)/inc
COM_LIB_DIR=$(COMMON_DIR)/lib
COM_OBJ_DIR=$(COMMON_DIR)/obj
COM_SRC_DIR=$(COMMON_DIR)/src

GLOG_INC_DIR=$(COM_INC_DIR)/glog

INC_DIR=$(CURRENT_PATH)/inc
LIB_DIR=$(CURRENT_PATH)/lib
OBJ_DIR=$(CURRENT_PATH)/obj
SRC_DIR=$(CURRENT_PATH)/src

#目标文件名
TARGET=$(CURRENT_PATH)/gate

#修改编译器
ifeq ($(PLATFORM),ARM)
ARCH = arm-hismall-linux-
else
ARCH =
endif
CC=$(ARCH)gcc
CPP=$(ARCH)g++
AR=$(ARCH)ar
STRIP=$(ARCH)strip

ifeq ($(DEBUG),ON)
CFLAGS += -DDEBUG
endif
CFLAGS += -Wall -fPIC -g -O2 -fno-strict-aliasing
CFLAGS += -Wno-deprecated -Wno-unused-but-set-variable -Wno-unused-result
CFLAGS += -I$(COM_INC_DIR)
CFLAGS += -I$(INC_DIR)

LDFLAGS += -lpthread -lglog
LDFLAGS += -L$(COM_LIB_DIR)
LDFLAGS += -L$(LIB_DIR)

#源文件
SRCS=$(wildcard $(SRC_DIR)/*.cc )
OBJS:=$(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(SRCS))

COM_SRC=$(wildcard $(COM_SRC_DIR)/*.cc )
COM_OBJS:=$(patsubst $(COM_SRC_DIR)/%.cc,$(COM_OBJ_DIR)/%.o,$(COM_SRC))

all: clean $(TARGET)

$(TARGET): $(OBJS) $(COM_OBJS)
	$(CPP) $(OBJS) $(COM_OBJS) -o $(TARGET) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CPP) $(CFLAGS) -c $< -o $@

$(COM_OBJ_DIR)/%.o: $(COM_SRC_DIR)/%.cc
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	-rm -f $(OBJS) $(COM_OBJS) $(TARGET)
