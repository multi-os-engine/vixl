# NOTE: `Android.mk` is automatically generated via the
# `third_party/android/generate_android_mk.py` script. Changes should be applied
# to `third_party/android/Android.mk.template`, and the script run to regenerate
# `Android.mk`.

# Copyright (C) 2015 The Android Open Source Project
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
# AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
# OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
# SUCH DAMAGE.
#
#
# Copyright (c) 2015 ARM Ltd
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. The name of the company may not be used to endorse or promote
#    products derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY ARM LTD ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL ARM LTD BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

LOCAL_PATH:= $(call my-dir)

vixl_root := $(LOCAL_PATH)/
vixl_src_include_path := $(vixl_root)/src/
vixl_test_include_path := $(vixl_src_include_path) $(vixl_root)/test/

vixl_common_files := \
  src/code-buffer-vixl.cc \
  src/compiler-intrinsics-vixl.cc \
  src/utils-vixl.cc

vixl_src_files_a32 := \
  src/a32/assembler-a32.cc \
  src/a32/disasm-a32.cc \
  src/a32/instructions-a32.cc \
  src/a32/macro-assembler-a32.cc \
  src/a32/operand-a32.cc

vixl_src_files_a64 := \
  src/a64/assembler-a64.cc \
  src/a64/cpu-a64.cc \
  src/a64/debugger-a64.cc \
  src/a64/decoder-a64.cc \
  src/a64/disasm-a64.cc \
  src/a64/instructions-a64.cc \
  src/a64/instrument-a64.cc \
  src/a64/logic-a64.cc \
  src/a64/macro-assembler-a64.cc \
  src/a64/simulator-a64.cc

vixl_test_files := \
  test/a32/test-assembler-a32.cc \
  test/a32/test-assembler-cond-rd-memop-immediate-512-a32.cc \
  test/a32/test-assembler-cond-rd-memop-immediate-8192-a32.cc \
  test/a32/test-assembler-cond-rd-memop-rs-a32.cc \
  test/a32/test-assembler-cond-rd-memop-rs-shift-amount-1to31-a32.cc \
  test/a32/test-assembler-cond-rd-memop-rs-shift-amount-1to32-a32.cc \
  test/a32/test-assembler-cond-rd-operand-const-a32.cc \
  test/a32/test-assembler-cond-rd-operand-const-t32.cc \
  test/a32/test-assembler-cond-rd-operand-imm16-t32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-a32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-ror-amount-a32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-ror-amount-t32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-amount-1to31-a32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-amount-1to31-t32-in-it-block.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-amount-1to31-t32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-amount-1to32-a32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-amount-1to32-t32-in-it-block.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-amount-1to32-t32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-rs-a32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-rs-t32-in-it-block.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-rs-t32-narrow-out-it-block.cc \
  test/a32/test-assembler-cond-rd-operand-rn-shift-rs-t32.cc \
  test/a32/test-assembler-cond-rd-operand-rn-t32-identical-low-registers-in-it-block.cc \
  test/a32/test-assembler-cond-rd-operand-rn-t32-in-it-block.cc \
  test/a32/test-assembler-cond-rd-operand-rn-t32-low-registers-in-it-block.cc \
  test/a32/test-assembler-cond-rd-operand-rn-t32.cc \
  test/a32/test-assembler-cond-rd-pc-operand-imm12-t32.cc \
  test/a32/test-assembler-cond-rd-pc-operand-imm8-t32.cc \
  test/a32/test-assembler-cond-rd-rn-a32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-const-a32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-const-t32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-imm12-t32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-a32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-ror-amount-a32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-ror-amount-t32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to31-a32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to31-t32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to32-a32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-shift-amount-1to32-t32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-shift-rs-a32.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-t32-all-low-in-it-block.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-t32-all-low-rd-is-rn-in-it-block.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-t32-rd-is-rn-in-it-block.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-t32-rd-is-rn-is-sp-in-it-block.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-t32-rn-is-sp-in-it-block.cc \
  test/a32/test-assembler-cond-rd-rn-operand-rm-t32.cc \
  test/a32/test-assembler-cond-rd-rn-rm-a32.cc \
  test/a32/test-assembler-cond-rd-rn-rm-t32.cc \
  test/a32/test-assembler-cond-rd-rn-t32.cc \
  test/a32/test-assembler-cond-rd-sp-operand-imm8-t32.cc \
  test/a32/test-assembler-cond-rdlow-operand-imm8-t32-in-it-block.cc \
  test/a32/test-assembler-cond-rdlow-operand-imm8-t32.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-operand-immediate-t32-imm3-in-it-block.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-operand-immediate-t32-imm3.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-operand-immediate-t32-imm8-in-it-block.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-operand-immediate-t32-imm8.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-operand-immediate-t32-zero-in-it-block.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-operand-immediate-t32-zero.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-rmlow-t32-in-it-block.cc \
  test/a32/test-assembler-cond-rdlow-rnlow-rmlow-t32.cc \
  test/a32/test-assembler-cond-sp-sp-operand-imm7-t32.cc \
  test/a32/test-assembler-rd-rn-rm-a32.cc \
  test/a32/test-assembler-rd-rn-rm-t32.cc \
  test/a32/test-disasm-a32.cc \
  test/a32/test-simulator-cond-rd-memop-immediate-512-a32.cc \
  test/a32/test-simulator-cond-rd-memop-immediate-8192-a32.cc \
  test/a32/test-simulator-cond-rd-memop-rs-a32.cc \
  test/a32/test-simulator-cond-rd-memop-rs-shift-amount-1to31-a32.cc \
  test/a32/test-simulator-cond-rd-memop-rs-shift-amount-1to32-a32.cc \
  test/a32/test-simulator-cond-rd-operand-const-a32.cc \
  test/a32/test-simulator-cond-rd-operand-const-t32.cc \
  test/a32/test-simulator-cond-rd-operand-imm16-t32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-a32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-ror-amount-a32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-ror-amount-t32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-shift-amount-1to31-a32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-shift-amount-1to31-t32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-shift-amount-1to32-a32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-shift-amount-1to32-t32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-shift-rs-a32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-shift-rs-t32.cc \
  test/a32/test-simulator-cond-rd-operand-rn-t32.cc \
  test/a32/test-simulator-cond-rd-rn-a32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-const-a32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-const-t32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-imm12-t32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-a32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-ror-amount-a32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-ror-amount-t32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to31-a32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to31-t32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to32-a32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-shift-amount-1to32-t32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-shift-rs-a32.cc \
  test/a32/test-simulator-cond-rd-rn-operand-rm-t32.cc \
  test/a32/test-simulator-cond-rd-rn-rm-a32-ge.cc \
  test/a32/test-simulator-cond-rd-rn-rm-a32-q.cc \
  test/a32/test-simulator-cond-rd-rn-rm-a32-sel.cc \
  test/a32/test-simulator-cond-rd-rn-rm-a32.cc \
  test/a32/test-simulator-cond-rd-rn-rm-t32-ge.cc \
  test/a32/test-simulator-cond-rd-rn-rm-t32-q.cc \
  test/a32/test-simulator-cond-rd-rn-rm-t32-sel.cc \
  test/a32/test-simulator-cond-rd-rn-rm-t32.cc \
  test/a32/test-simulator-cond-rd-rn-t32.cc \
  test/a32/test-simulator-cond-rdlow-operand-imm8-t32.cc \
  test/a32/test-simulator-cond-rdlow-rnlow-operand-immediate-t32.cc \
  test/a32/test-simulator-cond-rdlow-rnlow-rmlow-t32.cc \
  test/a32/test-simulator-rd-rn-rm-a32.cc \
  test/a32/test-simulator-rd-rn-rm-t32.cc \
  test/a32/test-utils-a32.cc \
  test/a64/test-assembler-a64.cc \
  test/a64/test-disasm-a64.cc \
  test/a64/test-fuzz-a64.cc \
  test/a64/test-simulator-a64.cc \
  test/a64/test-trace-a64.cc \
  test/a64/test-utils-a64.cc \
  test/test-invalset.cc \
  test/test-runner.cc \
  test/test-utils.cc

vixl_cpp_flags := \
  -DVIXL_GENERATE_SIMULATOR_INSTRUCTIONS_VALUE=0 \
  -Wall \
  -Wextra \
  -Werror \
  -fdiagnostics-show-option \
  -Wredundant-decls \
  -Wunreachable-code \
  -Wmissing-noreturn \
  -pedantic \
  -std=c++11 \

# Explicitly enable the write-strings warning. VIXL uses
# const correctly when handling string constants.
vixl_cpp_flags += \
  -Wwrite-strings \

vixl_cpp_flags_release := \
  $(vixl_cpp_flags) \
  -O3 \

vixl_cpp_flags_debug := \
  $(vixl_cpp_flags) \
  -DVIXL_DEBUG \
  -UNDEBUG \
  -O2 \
  -ggdb3 \

VIXL_COVERAGE := false

ifeq ($(ART_COVERAGE), true)
  VIXL_COVERAGE := true
endif

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_release) -DVIXL_INCLUDE_SIMULATOR
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a64)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixl-arm64
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_debug) -DVIXL_INCLUDE_SIMULATOR
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a64)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixld-arm64
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
include $(BUILD_SHARED_LIBRARY)


include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_release) -DVIXL_INCLUDE_SIMULATOR
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a64)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixl-arm64
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_debug) -DVIXL_INCLUDE_SIMULATOR
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a64)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixld-arm64
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_SHARED_LIBRARY)

# Static libraries for host
include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_release) -DVIXL_INCLUDE_SIMULATOR
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a64)
LOCAL_STATIC_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixl-arm64
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_debug) -DVIXL_INCLUDE_SIMULATOR
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a64)
LOCAL_STATIC_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixld-arm64
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_release)
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a32)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixl-arm
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_debug)
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a32)
LOCAL_SHARED_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixld-arm
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_SHARED_LIBRARY)

# Static libraries for host
include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_release)
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a32)
LOCAL_STATIC_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixl-arm
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_debug)
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_src_include_path)
LOCAL_SRC_FILES :=  $(vixl_common_files) $(vixl_src_files_a32)
LOCAL_STATIC_LIBRARIES := liblog
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvixld-arm
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
LOCAL_MULTILIB := both
include $(BUILD_HOST_STATIC_LIBRARY)


######### VIXL HOST TESTS #########
#
# We only support 64bit host builds for now.
# To run all the tests: vixl-test-runner --run_all
#
include $(CLEAR_VARS)
LOCAL_CLANG := true
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := $(vixl_cpp_flags_debug) -DVIXL_INCLUDE_SIMULATOR
LOCAL_CLANG_CFLAGS := -Wimplicit-fallthrough
LOCAL_NATIVE_COVERAGE := $(VIXL_COVERAGE)
LOCAL_C_INCLUDES := $(vixl_test_include_path)
LOCAL_SRC_FILES :=  $(vixl_test_files) $(vixl_common_files) $(vixl_src_files_a32) $(vixl_src_files_a64)
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_HOST_ARCH := x86_64
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE := vixl-test-runner
LOCAL_SANITIZE_RECOVER := shift-exponent
LOCAL_ADDITIONAL_DEPENDENCIES := $(LOCAL_PATH)/Android.mk
include $(BUILD_HOST_EXECUTABLE)

vixl_host_out := $(if $(ANDROID_HOST_OUT), $(ANDROID_HOST_OUT), ../../$(HOST_OUT))

.PHONY: run-vixl-tests
run-vixl-tests: vixl-test-runner
	$(vixl_root)/third_party/android/check_top_level_android_mk.sh
	# TODO: Once available, use the appropriate option of vixl-test-runner
	# to point to the trace files instead of running from the vixl root.
	cd $(vixl_root) && $(vixl_host_out)/bin/vixl-test-runner --run_all
	cd $(vixl_root) && $(vixl_host_out)/bin/vixl-test-runner --run_all --debugger
	@echo vixl tests PASSED
