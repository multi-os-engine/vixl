LOCAL_PATH:= $(call my-dir)

vixl_include_files := $(LOCAL_PATH)/src/ \

vixl_src_files := \
		src/utils-vixl.cc \
		src/a64/disasm-a64.cc \
		src/a64/assembler-a64.cc \
		src/a64/instructions-a64.cc \
		src/a64/macro-assembler-a64.cc \
		src/a64/decoder-a64.cc \

# VIXL simulator only works on 64bit platforms
ifeq ($(TARGET_IS_64_BIT), true)
vixl_src_files += \
		src/a64/simulator-a64.cc \
		src/a64/instrument-a64.cc \
		src/a64/debugger-a64.cc
endif

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := -O2 -Wall -Wextra
LOCAL_C_INCLUDES := $(vixl_include_files)
LOCAL_SRC_FILES :=  $(vixl_src_files)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixl
include external/stlport/libstlport.mk
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := -O2 -Wall -Wextra -DDEBUG=1
LOCAL_C_INCLUDES := $(vixl_include_files)
LOCAL_SRC_FILES :=  $(vixl_src_files)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixlg
include external/stlport/libstlport.mk
include $(BUILD_SHARED_LIBRARY)

# We need -m64 for 64bit Host builds
include $(CLEAR_VARS)
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := -O2 -Wall -Wextra
ifeq ($(TARGET_IS_64_BIT), true)
LOCAL_CPPFLAGS += -m64
endif

LOCAL_C_INCLUDES := $(vixl_include_files)
LOCAL_SRC_FILES :=  $(vixl_src_files)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixl
include $(BUILD_HOST_STATIC_LIBRARY)

