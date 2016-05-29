#文件目录
COM_INC_DIR=./inc
COM_LIB_DIR=./lib
COM_OBJ_DIR=./obj
COM_SRC_DIR=./src

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

STATIC_LIB=libcommon.a
SHARE_LIB=libcommon.so
INSTALL_PATH ?= .

ifeq ($(DEBUG),ON)
CFLAGS += -DDEBUG
endif
CFLAGS += -Wall -fPIC -g -O2 -fno-strict-aliasing
CFLAGS += $(foreach dir,$(COM_INC_DIR),-I$(dir))
CFLAGS += $(foreach dir,$(COM_INC_DIR),-I$(dir))

#源文件
COM_SRCS=$(wildcard $(COM_SRC_DIR)/*.cc )
COM_OBJS:=$(patsubst $(COM_SRC_DIR)/%.cc,$(COM_OBJ_DIR)/%.o,$(COM_SRCS))

all: $(clean) $(STATIC_LIB) $(SHARE_LIB)
	-mv $(STATIC_LIB) $(SHARE_LIB) $(COM_LIB_DIR)

$(COM_OBJ_DIR)/%.o: $(COM_SRC_DIR)/%.cc
	$(CPP) $(CFLAGS) -c $< -o $@

$(STATIC_LIB): $(COM_OBJS)
	$(AR) crus $@ $^

$(SHARE_LIB): $(COM_OBJS)
	$(CPP) -shared -o $@ $^

clean:
	-rm -f $(COM_OBJS) $(COM_LIB_DIR)/$(STATIC_LIB) $(COM_LIB_DIR)/$(SHARE_LIB)
