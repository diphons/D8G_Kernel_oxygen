// SPDX-License-Identifier: GPL-2.0 OR BSD-2-Clause
/*
 * OxygenOS DT2W interfaces
 *
 * Copyright (C) 2022, Rudi Setiyawan <diphons@gmail.com>
 */

#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/seq_file.h>
#include <linux/irq.h>
#include <linux/gpio.h>
#ifdef CONFIG_ARCH_SDM845
#include <linux/slab.h>
#include <linux/input/tp_common.h>
#endif

MODULE_LICENSE("GPL");
MODULE_AUTHOR("diphons");
MODULE_DESCRIPTION("oxygen os touch gesture");
MODULE_VERSION("0.0.5");

#define tpdir "touchpanel"

#define d_tap "touchpanel/double_tap_enable"
#define tp_dt "/touchpanel/double_tap"
#define tp_g "touchpanel/gesture_enable"
#define tpg "tp_gesture"
#define tpdir_fts "devices/platform/soc/a98000.i2c/i2c-3/3-0038/fts_gesture_mode"
#define input_boost "/sys/module/frame_boost_group/parameters/sysctl_input_boost_enabled"
#define slide_boost "/sys/module/frame_boost_group/parameters/sysctl_slide_boost_enabled"

#ifdef CONFIG_ARCH_SDM845
bool capacitive_keys_enabled;
struct kobject *touchpanel_kobj;
static int tpa = 0;
module_param(tpa, int, 0644);

#define TS_ENABLE_FOPS(type)                                                   \
	int tp_common_set_##type##_ops(struct tp_common_ops *ops)              \
	{                                                                      \
		static struct kobj_attribute kattr =                           \
			__ATTR(type, (S_IWUSR | S_IRUGO), NULL, NULL);         \
		kattr.show = ops->show;                                        \
		kattr.store = ops->store;                                      \
		return sysfs_create_file(touchpanel_kobj, &kattr.attr);        \
	}

TS_ENABLE_FOPS(capacitive_keys)
TS_ENABLE_FOPS(double_tap)
TS_ENABLE_FOPS(reversed_keys)

static char *tp_files_array[] = {
	"gesture_enable",
	"double_tap",
};

static char *tp_paths_array[] = {
	"/proc/touchpanel",
	"/sys/touchpanel"
};

bool inline str_cmp(const char *arg1, const char *arg2)
{
	return !strncmp(arg1, arg2, strlen(arg2));
}

bool inline tp_check_file(const char *name)
{
	int i, f;
	for (f = 0; f < ARRAY_SIZE(tp_paths_array); ++f) {
		const char *path_to_check = tp_paths_array[f];

		if (unlikely(str_cmp(name, path_to_check))) {
			for (i = 0; i < ARRAY_SIZE(tp_files_array); ++i) {
				const char *filename = name + strlen(path_to_check) + 1;
				const char *filename_to_check = tp_files_array[i];

				if (str_cmp(filename, filename_to_check)) {
					return 1;
				}
			}
		}
	}
	return 0;
}
#endif

static int __init d8g_touch_oos_init(void) {
//#if defined(CONFIG_BOARD_XIAOMI_SM8250) || defined(CONFIG_MACH_XIAOMI_SM8250) || defined(CONFIG_ARCH_SM8150)
	char *driver_path;
//#endif
    static struct proc_dir_entry *tp_dir;
 //   static struct proc_dir_entry *tp_dir_entry;
    static struct proc_dir_entry *tp_oos;
	int ret = 0;

	printk(KERN_INFO "oxygen os touch gesture initial");

#if defined(CONFIG_BOARD_XIAOMI_SM8250) || defined(CONFIG_MACH_XIAOMI_SM8250) || defined(CONFIG_ARCH_SM8150)
	tp_dir = proc_mkdir(tpdir, NULL);
	driver_path = kzalloc(PATH_MAX, GFP_KERNEL);
	if (!driver_path) {
		pr_err("%s: failed to allocate memory\n", __func__);
		ret = -ENOMEM;
	}

	sprintf(driver_path, "/sys%s", tp_dt);

	pr_err("%s: driver_path=%s\n", __func__, driver_path);

	tp_oos = proc_symlink(d_tap, NULL, driver_path);

	if (!tp_oos) {
		ret = -ENOMEM;
	}
	kfree(driver_path);
	tp_oos = proc_symlink(tp_g, NULL, "double_tap_enable");
	if (!tp_oos)
		ret = -ENOMEM;
#ifdef CONFIG_OPLUS_FEATURE_INPUT_BOOST_V4
#ifndef OPLUS_FEATURE_SCHED_ASSIST
	tp_oos = proc_symlink("input_boost_enabled", NULL, input_boost);
	if (!tp_oos)
		ret = -ENOMEM;
	tp_oos = proc_symlink("slide_boost_enabled", NULL, slide_boost);
	if (!tp_oos)
		ret = -ENOMEM;
#endif
#endif
#else
	touchpanel_kobj = kobject_create_and_add("touchpanel", NULL);
	if (!touchpanel_kobj)
		ret = -ENOMEM;

	tp_dir = proc_mkdir("d8g", NULL);
	if (unlikely(tp_check_file("gesture_enable"))) {
		tpa = 0;
		if (unlikely(tp_check_file("double_tap"))) {
			sprintf(driver_path, "/sys%s", tp_dt);
			pr_err("%s: driver_path=%s\n", __func__, driver_path);
			tp_oos = proc_symlink(tp_g, NULL, driver_path);
			if (!tp_oos) {
				ret = -ENOMEM;
			}
			kfree(driver_path);
		}
	} else
		tpa = 1;
		
	if (unlikely(tp_check_file("double_tap_enable"))) {
		tp_oos = proc_symlink("double_tap_enable", NULL, tp_g);
		if (!tp_oos)
			ret = -ENOMEM;
	}
		
	/*
	sprintf(driver_path, "/sys%s", tp_dt);
	pr_err("%s: driver_path=%s\n", __func__, driver_path);
	tp_oos = proc_symlink(tp_g, NULL, driver_path);
	if (!tp_oos) {
		ret = -ENOMEM;
	}
	kfree(driver_path);*/
	tp_oos = proc_symlink(d_tap, NULL, "gesture_enable");
	if (!tp_oos)
		ret = -ENOMEM;
#endif
	tp_oos = proc_symlink(tpg, NULL, tp_g);
	if (!tp_oos) {
		ret = -ENOMEM;
		printk(KERN_INFO "oxygen os touch gesture initial failed");
	} else
		printk(KERN_INFO "oxygen os touch gesture initialized");

	return ret;
}

static void __exit d8g_touch_oos_exit(void) {
	printk(KERN_INFO "oxygen os touch gesture exit");
}

module_init(d8g_touch_oos_init);
module_exit(d8g_touch_oos_exit);
