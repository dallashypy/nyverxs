# Theos root. Override without editing this file:
#   make THEOS=/opt/theos package        (WSL2 / Linux)
#   THEOS=$HOME/theos make package       (macOS)
THEOS ?= $(HOME)/theos
export THEOS

ARCHS = arm64
DEBUG = 0
FINALPACKAGE = 1
FOR_RELEASE = 1
IGNORE_WARNINGS = 1
THEOS_PACKAGE_SCHEME = rootless
TARGET = iphone:clang:latest:13.6


THEOS_MAKE_PATH ?= $(THEOS)/makefiles

TWEAK_NAME = NYVERXS
$(TWEAK_NAME)_CCFLAGS = -std=c++17 -fobjc-arc -fno-rtti -DNDEBUG -Wall -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-value -Wno-unused-function -fvisibility=hidden
$(TWEAK_NAME)_CFLAGS = -fobjc-arc -Wall -Wno-deprecated-declarations -Wno-unused-variable -Wno-unused-value -Wno-unused-function -fvisibility=hidden

ifeq ($(IGNORE_WARNINGS),1)
  $(TWEAK_NAME)_CFLAGS += -w
  $(TWEAK_NAME)_CCFLAGS += -w
endif
$(TWEAK_NAME)_FRAMEWORKS =  AudioToolbox UIKit Foundation Security QuartzCore CoreGraphics CoreText  AVFoundation Accelerate GLKit SystemConfiguration GameController Metal MetalKit
$(TWEAK_NAME)_LDFLAGS += JRMemory.framework/JRMemory 
$(TWEAK_NAME)_FILES = lostwq.mm $(wildcard Esp/*.mm) $(wildcard Esp/*.m) $(wildcard Remote/*.mm) $(wildcard IMGUI/*.cpp) $(wildcard IMGUI/*.mm) $(wildcard hook/*.c)

include $(THEOS)/makefiles/common.mk
include $(THEOS)/makefiles/tweak.mk
