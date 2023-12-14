# Android makefile for audio kernel modules
MY_LOCAL_PATH := $(call my-dir)

UAPI_OUT := $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/include

ifeq ($(call is-board-platform-in-list,msmnile $(MSMSTEPPE) $(TRINKET) sdm845),true)
$(shell mkdir -p $(UAPI_OUT)/linux;)
$(shell mkdir -p $(UAPI_OUT)/sound;)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/ipc/Module.symvers)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/dsp/Module.symvers)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/dsp/codecs/Module.symvers)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/soc/Module.symvers)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/asoc/Module.symvers)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/asoc/codecs/Module.symvers)
ifeq ($(call is-board-platform-in-list, msm8953 msm8937),true)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/asoc/codecs/wcd934x/Module.symvers)
endif

include $(MY_LOCAL_PATH)/include/uapi/Android.mk
include $(MY_LOCAL_PATH)/ipc/Android.mk
include $(MY_LOCAL_PATH)/dsp/Android.mk
include $(MY_LOCAL_PATH)/dsp/codecs/Android.mk
include $(MY_LOCAL_PATH)/soc/Android.mk
include $(MY_LOCAL_PATH)/asoc/Android.mk
include $(MY_LOCAL_PATH)/asoc/codecs/Android.mk
endif

ifeq ($(call is-board-platform-in-list,msmnile $(MSMSTEPPE) $(TRINKET) sdmshrike),true)
ifneq ($(TARGET_BOARD_AUTO),true)
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/asoc/codecs/wcd934x/Module.symvers)
include $(MY_LOCAL_PATH)/asoc/codecs/wcd934x/Android.mk
$(shell rm -rf $(PRODUCT_OUT)/obj/vendor/qcom/opensource/audio-kernel/soc/Module.symvers)
include $(MY_LOCAL_PATH)/soc/Android.mk
endif
endif
