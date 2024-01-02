# SPDX-License-Identifier: GPL-2.0-only
#
# Makefile for USB Power Delivery drivers
#

ifeq ($(CONFIG_ARCH_SDM845), y)
obj-$(CONFIG_USB_PD_POLICY)	+= policy_engine_sdm845.o
else
obj-$(CONFIG_USB_PD_POLICY)	+= policy_engine.o
endif
obj-$(CONFIG_QPNP_USB_PDPHY)	+= qpnp-pdphy.o
obj-$(CONFIG_PS5169)			+= ps5169.o
