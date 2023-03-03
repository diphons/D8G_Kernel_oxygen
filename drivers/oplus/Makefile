# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2018-2020 Oplus. All rights reserved.
obj-$(CONFIG_OPLUS_FEATURE_DUMP_DEVICE_INFO)	+=  dump_device_info.o
obj-$(CONFIG_OPLUS_FEATURE_OPROJECT) 	+= oplus_project.o
#obj-y     += charger/
obj-y     += system/cmdline_parser/
obj-y     += system/acm/
obj-y     += system/misc/
obj-y     += system/project_info/
obj-y     += system/selinux/
obj-y     += system/shutdown_detect/
obj-y     += system/qcom_minidump/
obj-y     += system/qcom_pmicwd/
obj-$(CONFIG_OPLUS_FEATURE_PMIC_MONITOR)     += system/oplus_pmic_monitor/
obj-$(CONFIG_OPLUS_FEATURE_CPU_JANKINFO)     += oplus_performance/cpu_jankinfo/
obj-$(CONFIG_OPLUS_HEALTHINFO)     += oplus_performance/healthinfo/main/
obj-$(CONFIG_PROCESS_RECLAIM_ENHANCE)     += oplus_performance/oplus_mm/process_reclaim/
obj-$(CONFIG_OPLUS_FEATURE_ZRAM_OPT)     += oplus_performance/oplus_mm/zram_opt/
obj-$(CONFIG_ZWB_HANDLE) += oplus_performance/zwb_handle/
obj-$(CONFIG_OPLUS_FEATURE_FDLEAK_CHECK) += oplus_fdleak_check.o
obj-$(CONFIG_COLOR_CTRL_V2) += colorctrl.o
obj-$(CONFIG_OPLUS_ION_BOOSTPOOL) += oplus_performance/ion_boost_pool/oplus_ion_boost_pool.o

obj-y	+= charger/oplus_chg_symbol.o
obj-$(CONFIG_OPLUS_FEATURE_CHG_BASIC) += charger/oplus_gauge.o
obj-y += network/
obj-y += wakelock_profiler/
#obj-y += oplus_performance/foreground_io_opt/
obj-y += oplus_performance/healthinfo/fs/
obj-y += oplus_performance/healthinfo/mm/
obj-y += oplus_performance/tpp/

obj-y	+= device_info/device_info.o
obj-$(CONFIG_OPLUS_FEATURE_GAME_OPT) += game_opt/
obj-$(CONFIG_OPLUS_FEATURE_INPUT_BOOST_V4) += oplus_performance/inputboost_v4/
obj-y += oplus_performance/im/
obj-y += power/cpufreq_effiency/
obj-y += power/midas/
#obj-y += system/oplus_midas/

obj-$(CONFIG_OPLUS_MULTI_KSWAPD) += oplus_performance/multi_kswapd/
obj-$(CONFIG_OPLUS_FEATURE_UIFIRST) += oplus_performance/uifirst/

#Add for NandSwap
obj-$(CONFIG_NANDSWAP)		+= oplus_performance/oplus_nandswap/

#obj-y += oplus_performance/sched_assist/
#obj-y += oplus_performance/special_opt/
#obj-y += oplus_performance/uxio_first/
