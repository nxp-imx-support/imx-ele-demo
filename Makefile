# Copyright 2024 NXP
# NXP Confidential and Proprietary. This software is owned or controlled by NXP and may only be used strictly in
# accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
# activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
# comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
# terms, then you may not retain, install, activate or otherwise use the software.

CC ?= gcc
CXX ?= g++
BIN = eledemo
LVGL_DIR_NAME ?= lvgl
LVGL_DIR ?= .
PRJ_DIR ?= .
ELELIBDIR := imx-secure-enclave
ELESRCDIR := ele
 
INCLUDE_PATHS :=	-I$(LVGL_DIR)/ \
					-I$(LVGL_DIR)/lv_modules/src/lv_demo_init_icon/ \
					-I$(LVGL_DIR)/lvgl/ \
					-I$(LVGL_DIR)/lv_drivers/wayland/ \
					-I$(LVGL_DIR)/$(ELELIBDIR)/include \
					-I$(LVGL_DIR)/$(ELELIBDIR)/include/hsm \
					-I$(LVGL_DIR)/$(ELELIBDIR)/include/she \
					-I$(LVGL_DIR)/$(ELELIBDIR)/include/common \
					-I$(LVGL_DIR)/$(ELESRCDIR)/include \
					-I$(LVGL_DIR)/custom \
					-I$(LVGL_DIR)/generated

DEFINES =   -D PSA_COMPLIANT \
			-D SECONDARY_API_SUPPORTED

LDFLAGS := -L $(LVGL_DIR)/$(ELELIBDIR) -lcrypto -l:libele_hsm.so.1.0  -lwayland-client -lxkbcommon -lwayland-cursor \

CFLAGS += ${DEFINES}
CFLAGS += ${INCLUDE_PATHS}

#Collect the files to compile
MAINSRC = ./main.c

include $(LVGL_DIR)/lvgl/lvgl.mk
include $(LVGL_DIR)/lv_drivers/lv_drivers.mk
#include $(LVGL_DIR)/lv_modules/lv_modules.mk
include $(LVGL_DIR)/generated/generated.mk
include $(LVGL_DIR)/custom/custom.mk
#include $(LVGL_DIR)/lv_100ask_app/lv_100ask_app.mk

OBJEXT ?= .o

CSRCS += $(GEN_CSRCS)
CSRCS += $(LVGL_DIR)/$(ELESRCDIR)/src/read_rng.c
CSRCS += $(LVGL_DIR)/$(ELESRCDIR)/src/cryptoExample.c
CSRCS += $(LVGL_DIR)/$(ELESRCDIR)/src/passWD.c
CSRCS += lv_drivers/wayland/protocols/wayland-xdg-shell-client-protocol.c

AOBJS := $(ASRCS:.S=$(OBJEXT))
COBJS = $(CSRCS:.c=$(OBJEXT))
MAINOBJ = $(MAINSRC:.c=$(OBJEXT))

SRCS = $(ASRCS) $(CSRCS) $(MAINSRC)
OBJS = $(AOBJS) $(COBJS)

all: default

%.o: %.c
	@echo "  "CC"\t"$<
	@$(CC)  $(CFLAGS)  -c $< -o $@

%.o: %.cpp
	@echo "  "CXX"\t"$@
	@${CXX} -c ${CPPFLAGS}   $< -o $@

default: $(AOBJS) $(COBJS) $(MAINOBJ)
	@echo "  "CXX"\t"$(BIN)
	@$(CXX) -o $(BIN) $(MAINOBJ) $(AOBJS) $(COBJS) $(LDFLAGS) $(CPPFLAGS)

	@mkdir -p $(LVGL_DIR)/obj $(LVGL_DIR)/bin
	@mv *.o $(LVGL_DIR)/obj/
	@mv $(BIN) $(LVGL_DIR)/bin/

clean: 
	@rm -f $(BIN) $(AOBJS) $(COBJS) $(MAINOBJ) ./bin/* ./obj/*
