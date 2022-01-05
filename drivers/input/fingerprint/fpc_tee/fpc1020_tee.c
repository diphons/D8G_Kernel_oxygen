/*
 * FPC1020 Fingerprint sensor device driver
 *
 * This driver will control the platform resources that the FPC fingerprint
 * sensor needs to operate. The major things are probing the sensor to check
 * that it is actually connected and let the Kernel know this and with that also
 * enabling and disabling of regulators, controlling GPIOs such as sensor reset
 * line, sensor IRQ line.
 *
 * The driver will expose most of its available functionality in sysfs which
 * enables dynamic control of these features from eg. a user space process.
 *
 * The sensor's IRQ events will be pushed to Kernel's event handling system and
 * are exposed in the drivers event node.
 *
 * This driver will NOT send any commands to the sensor it only controls the
 * electrical parts.
 *
 *
 * Copyright (c) 2015 Fingerprint Cards AB <tech@fingerprints.com>
<<<<<<< HEAD
 * Copyright (C) 2021 XiaoMi, Inc.
=======
 * Copyright (C) 2019 XiaoMi, Inc.
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License Version 2
 * as published by the Free Software Foundation.
 */

<<<<<<< HEAD
#define FPC_DRM_INTERFACE_WA

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

=======
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
#include <linux/atomic.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <linux/pm_wakeup.h>
#include <linux/fb.h>
<<<<<<< HEAD
#include <linux/pinctrl/qcom-pinctrl.h>
#include <drm/drm_bridge.h>
#ifndef FPC_DRM_INTERFACE_WA
#include <drm/drm_notifier_mi.h>
#endif

#define FPC_GPIO_NO_DEFAULT -1
#define FPC_GPIO_NO_DEFINED -2
#define FPC_GPIO_REQUEST_FAIL -3
#define FPC_TTW_HOLD_TIME 1000
=======
#include <drm/drm_bridge.h>
#include <drm/drm_notifier.h>

#define FPC_SCREEN_HOLD_TIME 2000
#define FPC_TTW_HOLD_TIME 2000
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
#define FP_UNLOCK_REJECTION_TIMEOUT (FPC_TTW_HOLD_TIME - 500)

#define RESET_LOW_SLEEP_MIN_US 5000
#define RESET_LOW_SLEEP_MAX_US (RESET_LOW_SLEEP_MIN_US + 100)
#define RESET_HIGH_SLEEP1_MIN_US 100
#define RESET_HIGH_SLEEP1_MAX_US (RESET_HIGH_SLEEP1_MIN_US + 100)
#define RESET_HIGH_SLEEP2_MIN_US 5000
#define RESET_HIGH_SLEEP2_MAX_US (RESET_HIGH_SLEEP2_MIN_US + 100)
#define PWR_ON_SLEEP_MIN_US 100
#define PWR_ON_SLEEP_MAX_US (PWR_ON_SLEEP_MIN_US + 900)

#define NUM_PARAMS_REG_ENABLE_SET 2

<<<<<<< HEAD
#define RELEASE_WAKELOCK_W_V "release_wakelock_with_verification"
#define RELEASE_WAKELOCK "release_wakelock"
#define START_IRQS_RECEIVED_CNT "start_irqs_received_counter"


static const char *const pctl_names[] = {
=======

static const char * const pctl_names[] = {
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	"fpc1020_reset_reset",
	"fpc1020_reset_active",
};

struct vreg_config {
	char *name;
	unsigned long vmin;
	unsigned long vmax;
	int ua_load;
<<<<<<< HEAD
	int gpio;
};

struct regulator *vreg;

static struct vreg_config vreg_conf[] = {
	{"vdd_ana", 1800000UL, 1800000UL, 6000, FPC_GPIO_NO_DEFAULT},
	/*{ "vcc_spi", 1800000UL, 1800000UL, 10, }, */
	/*{ "vdd_io", 1800000UL, 1800000UL, 6000, }, */
};

static int power_cfg = 0;

=======
};

static const struct vreg_config const vreg_conf[] = {
	{ "vdd_ana", 1800000UL, 1800000UL, 6000, },
	{ "fp_vdd_vreg", 3000000UL, 3000000UL, 6000, },
	/*{ "vcc_spi", 1800000UL, 1800000UL, 10, },*/
	/*{ "vdd_io", 1800000UL, 1800000UL, 6000, },*/
};

>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
struct fpc1020_data {
	struct device *dev;

	struct pinctrl *fingerprint_pinctrl;
	struct pinctrl_state *pinctrl_state[ARRAY_SIZE(pctl_names)];
<<<<<<< HEAD
	//struct regulator *vreg[ARRAY_SIZE(vreg_conf)];

	struct wakeup_source *ttw_wl;
	struct wakeup_source screen_wl;
	int irq_gpio;
	int rst_gpio;

	/*GPIO for voltage control */
	int vdd1v8_gpio;

	int nbr_irqs_received;
	int nbr_irqs_received_counter_start;

	struct mutex lock;	/* To set/get exported values in sysfs */
	bool prepared;
	bool irq_requested;
	bool gpios_requested;

	atomic_t wakeup_enabled;	/* Used both in ISR and non-ISR */
	int irqf;
#ifndef FPC_DRM_INTERFACE_WA
	struct notifier_block fb_notifier;
#endif
=======
	struct regulator *vreg[ARRAY_SIZE(vreg_conf)];

	struct wakeup_source ttw_wl;
	struct wakeup_source screen_wl;
	int irq_gpio;
	int rst_gpio;
	struct mutex lock; /* To set/get exported values in sysfs */
	bool prepared;
	atomic_t wakeup_enabled; /* Used both in ISR and non-ISR */
	int irqf;
	struct notifier_block fb_notifier;
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	bool fb_black;
	bool wait_finger_down;
	struct work_struct work;
};

<<<<<<< HEAD
static int reset_gpio_res(struct fpc1020_data *fpc1020);

/*
 * request/release GPIO for voltage control.
 *
 */
static int request_vreg_gpio(struct fpc1020_data *fpc1020, bool enable);
static int irq_setup(struct fpc1020_data *fpc1020, bool enable);
static int vreg_setup(struct fpc1020_data *fpc1020, const char *name,
		      bool enable);
static int device_prepare(struct fpc1020_data *fpc1020, bool enable);
static irqreturn_t fpc1020_irq_handler(int irq, void *handle);
static int fpc1020_request_named_gpio(struct fpc1020_data *fpc1020,
				      const char *label, int *gpio);
static int hw_reset(struct fpc1020_data *fpc1020);

static int reset_gpio_res(struct fpc1020_data *fpc1020)
{
	int rc = 0;
	struct device *dev;

	if (!fpc1020) {
		rc = -ENOMEM;
		printk
		    ("fpc %s: failed to allocate memory for struct fpc1020_data!\n",
		     __func__);
		return rc;
	}

	dev = fpc1020->dev;
	dev_info(dev, "fpc %s --->: enter!\n", __func__);

	fpc1020->vdd1v8_gpio = FPC_GPIO_NO_DEFAULT;
	fpc1020->irq_gpio = FPC_GPIO_NO_DEFAULT;
	dev_info(dev, "fpc %s <---: exit!\n", __func__);

	return rc;
}

static int request_vreg_gpio(struct fpc1020_data *fpc1020, bool enable)
{
	int rc = 0;
	struct device *dev = fpc1020->dev;

	dev_info(dev, "fpc %s --->: enter!\n", __func__);

	mutex_lock(&fpc1020->lock);

	if (enable && !fpc1020->gpios_requested) {
		rc = fpc1020_request_named_gpio(fpc1020, "fpc,gpio_irq",
						&fpc1020->irq_gpio);
		if (rc) {
			pr_err("fpc irq gpio request failed!\n");
			goto release_irq_gpio;
		} else {
			dev_info(dev, "fpc irq gpio applied at  %d\n",
				 fpc1020->irq_gpio);
		}

		dev_info(dev, "fpc irq gpio requested successfully!\n");

		if (fpc1020->irq_requested) {
			devm_free_irq(dev, gpio_to_irq(fpc1020->irq_gpio), fpc1020);
			fpc1020->irq_requested = false;
			dev_info(dev,
				 "fpc irq has been requested already, free firstly!\n");
		}

		rc = devm_request_threaded_irq(dev,
					       gpio_to_irq(fpc1020->irq_gpio),
					       NULL, fpc1020_irq_handler,
					       fpc1020->irqf, dev_name(dev),
					       fpc1020);
		if (rc) {
			pr_err("fpc could not request irq %d\n",
			       gpio_to_irq(fpc1020->irq_gpio));

			goto release_irq_gpio;
		}

		fpc1020->irq_requested = true;
		fpc1020->gpios_requested = true;
		dev_info(dev, "fpc requested irq %d\n",
			 gpio_to_irq(fpc1020->irq_gpio));
	} else if (!enable && fpc1020->gpios_requested) {
		if (fpc1020->irq_requested) {
			devm_free_irq(dev, gpio_to_irq(fpc1020->irq_gpio),
				      fpc1020);
			dev_info(dev, "fpc irq free successfully!\n");
			fpc1020->irq_requested = false;
		}

release_irq_gpio:
		if (gpio_is_valid(fpc1020->irq_gpio)) {
			devm_gpio_free(dev, fpc1020->irq_gpio);
			fpc1020->irq_gpio = FPC_GPIO_NO_DEFAULT;
			dev_info(dev, "fpc irq gpio released successfully!\n");
		}

		fpc1020->gpios_requested = false;
	} else {
		dev_info(dev, "%s: enable: %d, gpios_requested: %d ???\n",
			 __func__, enable, fpc1020->gpios_requested);
	}

	mutex_unlock(&fpc1020->lock);
	dev_info(dev, "fpc %s <---: exit!\n", __func__);
	return rc;
}

static int irq_setup(struct fpc1020_data *fpc1020, bool enable)
{

	int rc = 0;

	struct device *dev = fpc1020->dev;
	if (enable) {
		dev_info(dev, "fpc %s --->: enter, for enable irq!\n",
			 __func__);
	} else {
		dev_info(dev, "fpc %s --->: enter, for disable irq!\n",
			 __func__);
	}

	if (gpio_is_valid(fpc1020->irq_gpio)) {

		dev_info(dev, "fpc %s irq_gpio is validate!\n", __func__);

		if (enable) {
			enable_irq_wake(gpio_to_irq(fpc1020->irq_gpio));
			dev_info(dev, "fpc irq_gpio is enable. \n");
		} else {
			disable_irq_wake(gpio_to_irq(fpc1020->irq_gpio));
			dev_info(dev, "fpc irq_gpio is disable. \n");
		}
	} else {
		dev_info(dev, "fpc %s irq_gpio is invalidate!\n", __func__);
		rc = -EINVAL;
	}

	dev_info(dev, "fpc %s <---: exit!\n", __func__);
	return rc;
}

static int vreg_setup(struct fpc1020_data *fpc1020, const char *name,
		      bool enable)
{
	size_t i;
	int rc = 0;
	int gpio = 0;
	struct device *dev = fpc1020->dev;

	if (enable) {
		dev_info(dev, "fpc %s --->: enter, for power on at %s.\n",
			 __func__, name);
	} else {
		dev_info(dev, "fpc %s --->: enter, for power off at %s.\n",
			 __func__, name);
	}

	for (i = 0; i < ARRAY_SIZE(vreg_conf); i++) {
		const char *n = vreg_conf[i].name;

		if (!memcmp(n, name, strlen(n)))
			goto found;
	}

	dev_err(dev, "fpc %s: Regulator %s not found\n", __func__, name);
=======
static irqreturn_t fpc1020_irq_handler(int irq, void *handle);
static int fpc1020_request_named_gpio(struct fpc1020_data *fpc1020,
	const char *label, int *gpio);

static int vreg_setup(struct fpc1020_data *fpc1020, const char *name,
	bool enable)
{
	size_t i;
	int rc;
	struct regulator *vreg;
	struct device *dev = fpc1020->dev;

	for (i = 0; i < ARRAY_SIZE(fpc1020->vreg); i++) {
		const char *n = vreg_conf[i].name;

		if (!strncmp(n, name, strlen(n)))
			goto found;
	}

	dev_err(dev, "Regulator %s not found\n", name);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

	return -EINVAL;

found:
<<<<<<< HEAD
	gpio = vreg_conf[i].gpio;
	if (enable) {
		if (gpio_is_valid(gpio)) {
			rc = gpio_direction_output(gpio, 1);
			if (rc) {
				dev_err(dev, "fpc %s: fail to set gpio %d !\n",
					__func__, gpio);
				return rc;
			}
		} else {
			dev_err(dev, "fpc %s: unable to get gpio %d!\n",
				__func__, gpio);
		}
	} else {
		if (gpio_is_valid(gpio)) {
			rc = gpio_direction_output(gpio, 0);
			if (rc) {
				dev_err(dev, "fpc %s: fail to clear gpio %d!\n",
					__func__, gpio);
				return rc;
			}
		} else {
			dev_err(dev, "fpc %s: unable to get gpio %d!\n",
				__func__, gpio);
		}
	}

	dev_info(dev, "fpc %s <---: exit!\n", __func__);
	return rc;
}

/*
 * sysfs node for release GPIO.
 *
 */

static ssize_t request_vreg_gpio_set(struct device *dev,
				     struct device_attribute *attr,
				     const char *buf, size_t count)
{
	int rc;
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	if (!memcmp(buf, "enable", strlen("enable")))
		rc = request_vreg_gpio(fpc1020, true);
	else if (!memcmp(buf, "disable", strlen("disable")))
		rc = request_vreg_gpio(fpc1020, false);
	else
		return -EINVAL;

	return rc ? rc : count;
}

static DEVICE_ATTR(request_vreg, S_IWUSR, NULL, request_vreg_gpio_set);

/*
=======
	vreg = fpc1020->vreg[i];
	if (enable) {
		if (!vreg) {
			vreg = regulator_get(dev, name);
			if (IS_ERR(vreg)) {
				dev_err(dev, "Unable to get %s\n", name);
				return PTR_ERR(vreg);
			}
		}
#if 0
		if (regulator_count_voltages(vreg) > 0) {
			rc = regulator_set_voltage(vreg, vreg_conf[i].vmin,
					vreg_conf[i].vmax);
			if (rc)
				dev_err(dev,
					"Unable to set voltage on %s, %d\n",
					name, rc);
		}

		rc = regulator_set_optimum_mode(vreg, vreg_conf[i].ua_load);
		if (rc < 0)
			dev_err(dev, "Unable to set current on %s, %d\n",
					name, rc);
#endif
		rc = regulator_enable(vreg);
		if (rc) {
			dev_err(dev, "error enabling %s: %d\n", name, rc);
			regulator_put(vreg);
			vreg = NULL;
		}
		fpc1020->vreg[i] = vreg;
		dev_err(dev, "fp_vdd_vreg is enabled!\n");
	} else {
		if (vreg) {
			if (regulator_is_enabled(vreg)) {
				regulator_disable(vreg);
				dev_err(dev, "disabled %s\n", name);
			}
			regulator_put(vreg);
			fpc1020->vreg[i] = NULL;
		}
		rc = 0;
	}

	return rc;
}

/**
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
 * sysfs node for controlling clocks.
 *
 * This is disabled in platform variant of this driver but kept for
 * backwards compatibility. Only prints a debug print that it is
 * disabled.
 */
static ssize_t clk_enable_set(struct device *dev,
<<<<<<< HEAD
			      struct device_attribute *attr,
			      const char *buf, size_t count)
{
	dev_dbg(dev, "clk_enable sysfs node not enabled in platform driver\n");

	return count;
}

=======
	struct device_attribute *attr,
	const char *buf, size_t count)
{
	dev_dbg(dev,
		"clk_enable sysfs node not enabled in platform driver\n");

	return count;
}
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
static DEVICE_ATTR(clk_enable, S_IWUSR, NULL, clk_enable_set);

/**
 * Will try to select the set of pins (GPIOS) defined in a pin control node of
 * the device tree named @p name.
 *
 * The node can contain several eg. GPIOs that is controlled when selecting it.
 * The node may activate or deactivate the pins it contains, the action is
 * defined in the device tree node itself and not here. The states used
 * internally is fetched at probe time.
 *
 * @see pctl_names
 * @see fpc1020_probe
 */
static int select_pin_ctl(struct fpc1020_data *fpc1020, const char *name)
{
	size_t i;
	int rc;
	struct device *dev = fpc1020->dev;
<<<<<<< HEAD
	dev_dbg(dev, "fpc %s --->: enter! \n", __func__);

	for (i = 0; i < ARRAY_SIZE(pctl_names); i++) {
		const char *n = pctl_names[i];

		if (!memcmp(n, name, strlen(n))) {
			rc = pinctrl_select_state(fpc1020->fingerprint_pinctrl,
						  fpc1020->pinctrl_state[i]);
			if (rc)
				dev_err(dev, "fpc %s: cannot select '%s'\n",
					__func__, name);
			else
				dev_dbg(dev, "fpc %s: selected '%s'\n",
					__func__, name);
=======

	for (i = 0; i < ARRAY_SIZE(fpc1020->pinctrl_state); i++) {
		const char *n = pctl_names[i];

		if (!strncmp(n, name, strlen(n))) {
			rc = pinctrl_select_state(fpc1020->fingerprint_pinctrl,
					fpc1020->pinctrl_state[i]);
			if (rc)
				dev_err(dev, "cannot select '%s'\n", name);
			else
				dev_dbg(dev, "Selected '%s'\n", name);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
			goto exit;
		}
	}

	rc = -EINVAL;
<<<<<<< HEAD
	dev_err(dev, "%s:'%s' not found gpio\n", __func__, name);
=======
	dev_err(dev, "%s:'%s' not found\n", __func__, name);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

exit:
	return rc;
}

static ssize_t pinctl_set(struct device *dev,
<<<<<<< HEAD
			  struct device_attribute *attr, const char *buf,
			  size_t count)
=======
	struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);
	int rc;

	mutex_lock(&fpc1020->lock);
	rc = select_pin_ctl(fpc1020, buf);
	mutex_unlock(&fpc1020->lock);

	return rc ? rc : count;
}
<<<<<<< HEAD

static DEVICE_ATTR(pinctl_set, S_IWUSR, NULL, pinctl_set);

static ssize_t regulator_enable_set(struct device *dev,
				    struct device_attribute *attr,
				    const char *buf, size_t count)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);
	char op;
	char name[16];
	int rc;
	bool enable;

	if (NUM_PARAMS_REG_ENABLE_SET != sscanf(buf, "%15[^,],%c", name, &op))
		return -EINVAL;
	if (op == 'e')
		enable = true;
	else if (op == 'd')
		enable = false;
	else
		return -EINVAL;

	mutex_lock(&fpc1020->lock);
	rc = vreg_setup(fpc1020, name, enable);
	mutex_unlock(&fpc1020->lock);

	return rc ? rc : count;
}

=======
static DEVICE_ATTR(pinctl_set, S_IWUSR, NULL, pinctl_set);

static ssize_t regulator_enable_set(struct device *dev,
	struct device_attribute *attr, const char *buf, size_t count)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);
	int rc = 0;

	if (!strncmp(buf, "e", strlen("e"))) {
		mutex_lock(&fpc1020->lock);
		rc = vreg_setup(fpc1020, "fp_vdd_vreg", true);
		mutex_unlock(&fpc1020->lock);
		dev_err(dev, "enable fp_vdd_vreg!, rc = %d\n", rc);
	} else if (!strncmp(buf, "d", strlen("d"))) {
		mutex_lock(&fpc1020->lock);
		rc = vreg_setup(fpc1020, "fp_vdd_vreg", false);
		mutex_unlock(&fpc1020->lock);
		dev_err(dev, "disable fp_vdd_vreg!, rc = %d\n", rc);
	} else {
		dev_err(dev, "operation disable!\n");
		rc = 1;
	}

	return rc ? rc : count;
}
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
static DEVICE_ATTR(regulator_enable, S_IWUSR, NULL, regulator_enable_set);

static int hw_reset(struct fpc1020_data *fpc1020)
{
	int irq_gpio;
<<<<<<< HEAD
	int rc;
	struct device *dev = fpc1020->dev;

	dev_dbg(dev, "fpc %s --->: enter! \n", __func__);

	rc = select_pin_ctl(fpc1020, "fpc1020_reset_active");
=======
	struct device *dev = fpc1020->dev;
	int rc = select_pin_ctl(fpc1020, "fpc1020_reset_active");

>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	if (rc)
		goto exit;
	usleep_range(RESET_HIGH_SLEEP1_MIN_US, RESET_HIGH_SLEEP1_MAX_US);

	rc = select_pin_ctl(fpc1020, "fpc1020_reset_reset");
	if (rc)
		goto exit;
	usleep_range(RESET_LOW_SLEEP_MIN_US, RESET_LOW_SLEEP_MAX_US);

	rc = select_pin_ctl(fpc1020, "fpc1020_reset_active");
	if (rc)
		goto exit;
	usleep_range(RESET_HIGH_SLEEP2_MIN_US, RESET_HIGH_SLEEP2_MAX_US);

	irq_gpio = gpio_get_value(fpc1020->irq_gpio);
	dev_info(dev, "IRQ after reset %d\n", irq_gpio);

exit:
<<<<<<< HEAD
	dev_dbg(dev, "fpc %s <---: exit! \n", __func__);
=======
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	return rc;
}

static ssize_t hw_reset_set(struct device *dev,
<<<<<<< HEAD
			    struct device_attribute *attr, const char *buf,
			    size_t count)
=======
	struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
{
	int rc;
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	if (!strncmp(buf, "reset", strlen("reset"))) {
		mutex_lock(&fpc1020->lock);
		rc = hw_reset(fpc1020);
		mutex_unlock(&fpc1020->lock);
	} else {
		return -EINVAL;
	}

	return rc ? rc : count;
}
<<<<<<< HEAD

=======
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
static DEVICE_ATTR(hw_reset, S_IWUSR, NULL, hw_reset_set);

/**
 * Will setup GPIOs, and regulators to correctly initialize the touch sensor to
 * be ready for work.
 *
 * In the correct order according to the sensor spec this function will
 * enable/disable regulators, and reset line, all to set the sensor in a
 * correct power on or off state "electrical" wise.
 *
 * @see  device_prepare_set
 * @note This function will not send any commands to the sensor it will only
 *       control it "electrically".
 */
static int device_prepare(struct fpc1020_data *fpc1020, bool enable)
{
<<<<<<< HEAD
	int rc = 0;

	struct device *dev = fpc1020->dev;

	if (enable) {
		dev_info(dev, "fpc %s --->: enter, for enable power! \n",
			 __func__);
	} else {
		dev_info(dev, "fpc %s --->: enter, for disable power! \n",
			 __func__);
	}

	mutex_lock(&fpc1020->lock);

	if (enable && !fpc1020->prepared) {

		rc = irq_setup(fpc1020, true);

		if (rc) {
			dev_dbg(dev, "fpc irq setup failed。 \n");
		}

		select_pin_ctl(fpc1020, "fpc1020_reset_reset");

		if (power_cfg == 1) {
			pr_info("Try to enable fp_vdd_vreg\n");
			vreg = regulator_get(dev, "fp_vdd_vreg");

			if (vreg == NULL) {
				dev_err(dev, "fp_vdd_vreg regulator get failed!\n");
				goto exit;
			}

			if (regulator_is_enabled(vreg)) {
				pr_info("fp_vdd_vreg is already enabled!\n");
			} else {
				rc = regulator_enable(vreg);
				if (rc) {
					dev_err(dev, "error enabling fp_vdd_vreg!\n");
					regulator_put(vreg);
					vreg = NULL;
					goto exit;
				}
			}

			pr_info("fp_vdd_vreg is enabled!\n");
		} else {
			rc = vreg_setup(fpc1020, "vdd_ana", true);
			if (rc) {
				dev_dbg(dev, "fpc power on failed。 \n");
				goto exit;
			}
		}
=======
	int rc;
	struct device *dev = fpc1020->dev;

	mutex_lock(&fpc1020->lock);
	if (enable && !fpc1020->prepared) {
/*
		rc = select_pin_ctl(fpc1020, "fpc1020_irq_active");
		if (rc) {
			pr_err("irq gpio set active failed\n");
			goto exit;
		}
*/
		rc = fpc1020_request_named_gpio(fpc1020, "fpc,gpio_irq",
					&fpc1020->irq_gpio);
		if (rc) {
			pr_err("irq gpio request failed\n");
			goto exit;
		}
/*
		rc = fpc1020_request_named_gpio(fpc1020, "fpc,gpio_rst",
					&fpc1020->rst_gpio);
		if (rc) {
			pr_err("reset gpio request failed\n");
			goto irq_gpio_exit;
		}
*/
		rc = devm_request_threaded_irq(dev, gpio_to_irq(fpc1020->irq_gpio),
				NULL, fpc1020_irq_handler, fpc1020->irqf,
				dev_name(dev), fpc1020);
		if (rc) {
			pr_err("could not request irq %d\n",
					gpio_to_irq(fpc1020->irq_gpio));
			goto rst_gpio_exit;
		}

		dev_dbg(dev, "requested irq %d\n", gpio_to_irq(fpc1020->irq_gpio));

		/* Request that the interrupt should be wakeable */
		enable_irq_wake(gpio_to_irq(fpc1020->irq_gpio));
		select_pin_ctl(fpc1020, "fpc1020_reset_reset");
#if 0
		rc = vreg_setup(fpc1020, "vcc_spi", true);
		if (rc)
			goto exit;

		rc = vreg_setup(fpc1020, "vdd_io", true);
		if (rc)
			goto exit_1;
#endif
		rc = vreg_setup(fpc1020, "vdd_ana", true);
		if (rc)
			goto free_irq_exit;

#ifdef CONFIG_FINGERPRINT_FP_VREG_CONTROL
		dev_err(dev, "fp_vdd_reg enabling...\n");
		rc = vreg_setup(fpc1020, "fp_vdd_vreg", true);
		if (rc) {
			pr_err("fp_vdd_vreg config failed, rc = %d\n", rc);
			goto free_irq_exit;
		}
		dev_err(dev, "fp_vdd_reg enabled success\n");
#endif
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

		usleep_range(PWR_ON_SLEEP_MIN_US, PWR_ON_SLEEP_MAX_US);

		/* As we can't control chip select here the other part of the
		 * sensor driver eg. the TEE driver needs to do a _SOFT_ reset
		 * on the sensor after power up to be sure that the sensor is
		 * in a good state after power up. Okeyed by ASIC. */

		(void)select_pin_ctl(fpc1020, "fpc1020_reset_active");
		hw_reset(fpc1020);
<<<<<<< HEAD

		fpc1020->prepared = true;
		dev_dbg(dev, "fpc power on success. \n");
	} else if (!enable && fpc1020->prepared) {

		rc = irq_setup(fpc1020, false);

		if (rc) {
			dev_dbg(dev, "fpc irq setup failed。 \n");
		}

=======
		fpc1020->prepared = true;
	} else if (!enable && fpc1020->prepared) {
		rc = 0;
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
		(void)select_pin_ctl(fpc1020, "fpc1020_reset_reset");

		usleep_range(PWR_ON_SLEEP_MIN_US, PWR_ON_SLEEP_MAX_US);

<<<<<<< HEAD
		if (power_cfg == 1) {
			rc = regulator_disable(vreg);
			if (rc) {
				dev_dbg(dev, "error disabling fp_vdd_vreg!\n");
				goto exit;
			}
		} else {
			rc = vreg_setup(fpc1020, "vdd_ana", false);
			if (rc) {
				dev_dbg(dev, "fpc vreg power off failed. \n");
				goto exit;
			}
		}
=======
		(void)vreg_setup(fpc1020, "vdd_ana", false);
#if 0
exit_2:
		(void)vreg_setup(fpc1020, "vdd_io", false);
exit_1:
		(void)vreg_setup(fpc1020, "vcc_spi", false);
#endif
free_irq_exit:
		disable_irq(gpio_to_irq(fpc1020->irq_gpio));
		devm_free_irq(dev, gpio_to_irq(fpc1020->irq_gpio), fpc1020);
rst_gpio_exit:
/*
		devm_gpio_free(dev, fpc1020->rst_gpio);
*/
		devm_gpio_free(dev, fpc1020->irq_gpio);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
exit:
		fpc1020->prepared = false;
	} else {
		rc = 0;
	}
	mutex_unlock(&fpc1020->lock);

<<<<<<< HEAD
	dev_dbg(dev, "fpc %s <---: exit! \n", __func__);
=======
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	return rc;
}

/**
 * sysfs node to enable/disable (power up/power down) the touch sensor
 *
 * @see device_prepare
 */
static ssize_t device_prepare_set(struct device *dev,
<<<<<<< HEAD
				  struct device_attribute *attr,
				  const char *buf, size_t count)
=======
	struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
{
	int rc;
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

<<<<<<< HEAD
	if (!strncmp(buf, "enable", strlen("enable")))
		rc = device_prepare(fpc1020, true);
	else if (!strncmp(buf, "disable", strlen("disable")))
		rc = device_prepare(fpc1020, false);
	else
		return -EINVAL;

	return rc ? rc : count;
}

=======
	if (!strncmp(buf, "enable", strlen("enable"))) {
		rc = device_prepare(fpc1020, true);
	} else if (!strncmp(buf, "disable", strlen("disable"))) {
		rc = device_prepare(fpc1020, false);
	} else {
		return -EINVAL;
	}

	return rc ? rc : count;
}
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
static DEVICE_ATTR(device_prepare, S_IWUSR, NULL, device_prepare_set);

/**
 * sysfs node for controlling whether the driver is allowed
 * to wake up the platform on interrupt.
 */
static ssize_t wakeup_enable_set(struct device *dev,
<<<<<<< HEAD
				 struct device_attribute *attr, const char *buf,
				 size_t count)
=======
	struct device_attribute *attr, const char *buf, size_t count)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);
	ssize_t ret = count;

	mutex_lock(&fpc1020->lock);
/*
	if (!strncmp(buf, "enable", strlen("enable")))
		atomic_set(&fpc1020->wakeup_enabled, 1);
	else if (!strncmp(buf, "disable", strlen("disable")))
		atomic_set(&fpc1020->wakeup_enabled, 0);
	else
		ret = -EINVAL;
*/
	mutex_unlock(&fpc1020->lock);

	return ret;
}
<<<<<<< HEAD

static DEVICE_ATTR(wakeup_enable, S_IWUSR, NULL, wakeup_enable_set);

/**
 * sysfs node for controlling the wakelock.
 */
static ssize_t handle_wakelock_cmd(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);
	ssize_t ret = count;

	mutex_lock(&fpc1020->lock);
	if (!strncmp(buf, RELEASE_WAKELOCK_W_V,
		     min(count, strlen(RELEASE_WAKELOCK_W_V)))) {
		if (fpc1020->nbr_irqs_received_counter_start ==
		    fpc1020->nbr_irqs_received) {
			__pm_relax(fpc1020->ttw_wl);
		} else {
			dev_dbg(dev, "Ignore releasing of wakelock %d != %d",
				fpc1020->nbr_irqs_received_counter_start,
				fpc1020->nbr_irqs_received);
		}
	} else if (!strncmp(buf, RELEASE_WAKELOCK, min(count,
						       strlen
						       (RELEASE_WAKELOCK)))) {
		__pm_relax(fpc1020->ttw_wl);
	} else if (!strncmp(buf, START_IRQS_RECEIVED_CNT,
			    min(count, strlen(START_IRQS_RECEIVED_CNT)))) {
		fpc1020->nbr_irqs_received_counter_start =
		    fpc1020->nbr_irqs_received;
	} else
		ret = -EINVAL;
	mutex_unlock(&fpc1020->lock);

	return ret;
}

static DEVICE_ATTR(handle_wakelock, S_IWUSR, NULL, handle_wakelock_cmd);

/**
=======
static DEVICE_ATTR(wakeup_enable, S_IWUSR, NULL, wakeup_enable_set);

/**
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
 * sysf node to check the interrupt status of the sensor, the interrupt
 * handler should perform sysf_notify to allow userland to poll the node.
 */
static ssize_t irq_get(struct device *dev,
<<<<<<< HEAD
		       struct device_attribute *attr, char *buf)
=======
	struct device_attribute *attr,
	char *buf)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);
	int irq = gpio_get_value(fpc1020->irq_gpio);

	return scnprintf(buf, PAGE_SIZE, "%i\n", irq);
}

/**
 * writing to the irq node will just drop a printk message
 * and return success, used for latency measurement.
 */
static ssize_t irq_ack(struct device *dev,
<<<<<<< HEAD
		       struct device_attribute *attr,
		       const char *buf, size_t count)
=======
	struct device_attribute *attr,
	const char *buf, size_t count)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	dev_dbg(fpc1020->dev, "%s\n", __func__);

	return count;
}
<<<<<<< HEAD

static DEVICE_ATTR(irq, S_IRUSR | S_IWUSR, irq_get, irq_ack);

static ssize_t fingerdown_wait_set(struct device *dev,
				   struct device_attribute *attr,
				   const char *buf, size_t count)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	dev_info(fpc1020->dev, "%s -> %s\n", __func__, buf);
	if (!strncmp(buf, "enable", strlen("enable")) && fpc1020->prepared)
		fpc1020->wait_finger_down = true;
	else if (!strncmp(buf, "disable", strlen("disable"))
		 && fpc1020->prepared)
=======
static DEVICE_ATTR(irq, S_IRUSR | S_IWUSR, irq_get, irq_ack);

static ssize_t fingerdown_wait_set(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t count)
{
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	dev_dbg(fpc1020->dev, "%s\n", __func__);
	if (!strncmp(buf, "enable", strlen("enable")) && fpc1020->prepared)
		fpc1020->wait_finger_down = true;
	else if (!strncmp(buf, "disable", strlen("disable")) && fpc1020->prepared)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
		fpc1020->wait_finger_down = false;
	else
		return -EINVAL;

	return count;
}
<<<<<<< HEAD

static DEVICE_ATTR(fingerdown_wait, S_IWUSR, NULL, fingerdown_wait_set);

static ssize_t vendor_update(struct device *dev,
			     struct device_attribute *attr,
			     const char *buf, size_t count)
{
	int rc;
	return rc ? rc : count;
}

static DEVICE_ATTR(vendor, S_IWUSR, NULL, vendor_update);

static ssize_t irq_enable_set(struct device *dev,
			      struct device_attribute *attr,
			      const char *buf, size_t count)
=======
static DEVICE_ATTR(fingerdown_wait, S_IWUSR, NULL, fingerdown_wait_set);

static ssize_t irq_enable_set(struct device *dev,
	struct device_attribute *attr,
	const char *buf, size_t count)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
{
	int rc = 0;
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	if (!strncmp(buf, "1", strlen("1"))) {
		mutex_lock(&fpc1020->lock);
		enable_irq(gpio_to_irq(fpc1020->irq_gpio));
		mutex_unlock(&fpc1020->lock);
		pr_debug("fpc enable irq\n");
	} else if (!strncmp(buf, "0", strlen("0"))) {
		mutex_lock(&fpc1020->lock);
		disable_irq(gpio_to_irq(fpc1020->irq_gpio));
		mutex_unlock(&fpc1020->lock);
		pr_debug("fpc disable irq\n");
	}

	return rc ? rc : count;
}
<<<<<<< HEAD

static DEVICE_ATTR(irq_enable, S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP, NULL,
		   irq_enable_set);

static ssize_t power_cfg_set(struct device *dev,
				  struct device_attribute *attr,
				  const char *buf, size_t count)
{
	int rc = 0;
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	mutex_lock(&fpc1020->lock);

	if (!strncmp(buf, "1V8", strlen("1V8")))
		power_cfg = 0;
	else if (!strncmp(buf, "3V3", strlen("3V3")))
		power_cfg = 1;
	else
		rc = -EINVAL;

	mutex_unlock(&fpc1020->lock);

	dev_info(fpc1020->dev, "fpc set power_cfg: %d, rc: %d\n", power_cfg, rc);

	return rc ? rc : count;
}

static DEVICE_ATTR(power_cfg, S_IWUSR, NULL, power_cfg_set);

static struct attribute *attributes[] = {
	&dev_attr_request_vreg.attr,
=======
static DEVICE_ATTR(irq_enable, S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP , NULL, irq_enable_set);

static ssize_t screen_status_get(struct device *dev, struct device_attribute *attr, char *buf)
{
	int retval = 0;
	struct fpc1020_data *fpc1020 = dev_get_drvdata(dev);

	retval = (fpc1020->fb_black == true) ? 0 : 1;

	return scnprintf(buf, PAGE_SIZE, "%d\n", retval);
}
static DEVICE_ATTR(screen_status, S_IRUSR | S_IRGRP, screen_status_get, NULL);

static ssize_t vreg_op_cnt_set(struct device *dev, struct device_attribute *attr,
		const char *buf, size_t count)
{
	char value[64] = {0};

	snprintf(value, sizeof(value), "%s", buf);
	return count;
}
static DEVICE_ATTR(vreg_op_cnt, S_IWUSR, NULL, vreg_op_cnt_set);

static struct attribute *attributes[] = {
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	&dev_attr_pinctl_set.attr,
	&dev_attr_device_prepare.attr,
	&dev_attr_regulator_enable.attr,
	&dev_attr_hw_reset.attr,
	&dev_attr_wakeup_enable.attr,
<<<<<<< HEAD
	&dev_attr_handle_wakelock.attr,
	&dev_attr_clk_enable.attr,
	&dev_attr_irq_enable.attr,
	&dev_attr_irq.attr,
	&dev_attr_vendor.attr,
	&dev_attr_fingerdown_wait.attr,
	&dev_attr_power_cfg.attr,
=======
	&dev_attr_clk_enable.attr,
	&dev_attr_irq_enable.attr,
	&dev_attr_irq.attr,
	&dev_attr_screen_status.attr,
	&dev_attr_fingerdown_wait.attr,
	&dev_attr_vreg_op_cnt.attr,
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	NULL
};

static const struct attribute_group attribute_group = {
	.attrs = attributes,
};

<<<<<<< HEAD
#ifndef FPC_DRM_INTERFACE_WA
static void notification_work(struct work_struct *work)
{
	pr_info("%s: unblank\n", __func__);
	dsi_bridge_interface_enable(FP_UNLOCK_REJECTION_TIMEOUT);
}
#endif
=======
static void notification_work(struct work_struct *work)
{
	pr_debug("%s: unblank\n", __func__);
	dsi_bridge_interface_enable(FP_UNLOCK_REJECTION_TIMEOUT);
}

>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

static irqreturn_t fpc1020_irq_handler(int irq, void *handle)
{
	struct fpc1020_data *fpc1020 = handle;

	dev_dbg(fpc1020->dev, "%s\n", __func__);

	if (atomic_read(&fpc1020->wakeup_enabled)) {
<<<<<<< HEAD
		fpc1020->nbr_irqs_received++;
		__pm_wakeup_event(fpc1020->ttw_wl, FPC_TTW_HOLD_TIME);
=======
		__pm_wakeup_event(&fpc1020->ttw_wl, FPC_TTW_HOLD_TIME);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	}

	sysfs_notify(&fpc1020->dev->kobj, NULL, dev_attr_irq.attr.name);
	if (fpc1020->wait_finger_down && fpc1020->fb_black && fpc1020->prepared) {
<<<<<<< HEAD
		pr_info("%s enter fingerdown & fb_black then schedule_work\n", __func__);
		fpc1020->wait_finger_down = false;
#ifndef FPC_DRM_INTERFACE_WA
		schedule_work(&fpc1020->work);
#endif
=======
		pr_debug("%s enter\n", __func__);
		fpc1020->wait_finger_down = false;
		schedule_work(&fpc1020->work);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	}

	return IRQ_HANDLED;
}

static int fpc1020_request_named_gpio(struct fpc1020_data *fpc1020,
<<<<<<< HEAD
				      const char *label, int *gpio)
{
	int rc;
	struct device *dev = fpc1020->dev;
	struct device_node *np = dev->of_node;

	dev_dbg(dev, "fpc %s --->: enter!\n", __func__);

	rc = of_get_named_gpio(np, label, 0);

	if (rc < 0) {
		dev_err(dev, "failed to get '%s'\n", label);
		return FPC_GPIO_NO_DEFINED;
=======
	const char *label, int *gpio)
{
	struct device *dev = fpc1020->dev;
	struct device_node *np = dev->of_node;
	int rc = of_get_named_gpio(np, label, 0);

	if (rc < 0) {
		dev_err(dev, "failed to get '%s'\n", label);
		return rc;
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	}
	*gpio = rc;

	rc = devm_gpio_request(dev, *gpio, label);
	if (rc) {
		dev_err(dev, "failed to request gpio %d\n", *gpio);
<<<<<<< HEAD
		return FPC_GPIO_REQUEST_FAIL;
	}

	dev_dbg(dev, "fpc %s <--- %s at %d\n", __func__, label, *gpio);
=======
		return rc;
	}
	dev_dbg(dev, "%s %d\n", label, *gpio);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

	return 0;
}

<<<<<<< HEAD
static void set_fingerprintd_nice(int nice)
{
	struct task_struct *p;

	read_lock(&tasklist_lock);
	for_each_process(p) {
		if (strstr(p->comm, "erprint"))
			set_user_nice(p, nice);
	}
	read_unlock(&tasklist_lock);
}

#ifndef FPC_DRM_INTERFACE_WA
static int fpc_fb_notif_callback(struct notifier_block *nb,
				 unsigned long val, void *data)
{
	struct fpc1020_data *fpc1020 = container_of(nb, struct fpc1020_data,
						    fb_notifier);
=======
static int fpc_fb_notif_callback(struct notifier_block *nb,
		unsigned long val, void *data)
{
	struct fpc1020_data *fpc1020 = container_of(nb, struct fpc1020_data,
			fb_notifier);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	struct fb_event *evdata = data;
	unsigned int blank;

	if (!fpc1020)
		return 0;

<<<<<<< HEAD
	if (val != MI_DRM_EVENT_BLANK || fpc1020->prepared == false)
=======
	if (val != DRM_EVENT_BLANK || fpc1020->prepared == false)
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
		return 0;

	pr_debug("[info] %s value = %d\n", __func__, (int)val);

<<<<<<< HEAD
	if (evdata && evdata->data && val == MI_DRM_EVENT_BLANK) {
		blank = *(int *)(evdata->data);
		switch (blank) {
		case MI_DRM_BLANK_POWERDOWN:
                        set_fingerprintd_nice(MIN_NICE);
			fpc1020->fb_black = true;
			break;
		case MI_DRM_BLANK_UNBLANK:
                        set_fingerprintd_nice(0);
			fpc1020->fb_black = false;
=======
	if (evdata && evdata->data && val == DRM_EVENT_BLANK) {
		blank = *(int *)(evdata->data);
		switch (blank) {
		case DRM_BLANK_POWERDOWN:
			fpc1020->fb_black = true;
#ifdef CONFIG_FINGERPRINT_FPC_SCREEN_NOTIFY
			__pm_wakeup_event(&fpc1020->screen_wl, FPC_SCREEN_HOLD_TIME);
			sysfs_notify(&fpc1020->dev->kobj, NULL, dev_attr_screen_status.attr.name);
#endif
			break;
		case DRM_BLANK_UNBLANK:
			fpc1020->fb_black = false;
#ifdef CONFIG_FINGERPRINT_FPC_SCREEN_NOTIFY
			__pm_wakeup_event(&fpc1020->screen_wl, FPC_SCREEN_HOLD_TIME);
			sysfs_notify(&fpc1020->dev->kobj, NULL, dev_attr_screen_status.attr.name);
#endif
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
			break;
		default:
			pr_debug("%s defalut\n", __func__);
			break;
		}
	}
	return NOTIFY_OK;
}

<<<<<<< HEAD
static struct notifier_block fpc_notif_block = {
	.notifier_call = fpc_fb_notif_callback,
};
#endif
=======

static struct notifier_block fpc_notif_block = {
	.notifier_call = fpc_fb_notif_callback,
};
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

static int fpc1020_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int rc = 0;
	size_t i;

	struct device_node *np = dev->of_node;
	struct fpc1020_data *fpc1020 = devm_kzalloc(dev, sizeof(*fpc1020),
<<<<<<< HEAD
						    GFP_KERNEL);

	dev_info(dev, "fpc %s --->: enter! \n", __func__);
=======
			GFP_KERNEL);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

	if (!fpc1020) {
		dev_err(dev,
			"failed to allocate memory for struct fpc1020_data\n");
		rc = -ENOMEM;
		goto exit;
	}

	fpc1020->dev = dev;
	platform_set_drvdata(pdev, fpc1020);

	if (!np) {
		dev_err(dev, "no of node found\n");
		rc = -EINVAL;
		goto exit;
	}

	fpc1020->fingerprint_pinctrl = devm_pinctrl_get(dev);
	if (IS_ERR(fpc1020->fingerprint_pinctrl)) {
		if (PTR_ERR(fpc1020->fingerprint_pinctrl) == -EPROBE_DEFER) {
			dev_info(dev, "pinctrl not ready\n");
			rc = -EPROBE_DEFER;
			goto exit;
		}
		dev_err(dev, "Target does not use pinctrl\n");
		fpc1020->fingerprint_pinctrl = NULL;
		rc = -EINVAL;
		goto exit;
	}

	for (i = 0; i < ARRAY_SIZE(fpc1020->pinctrl_state); i++) {
		const char *n = pctl_names[i];
		struct pinctrl_state *state =
<<<<<<< HEAD
		    pinctrl_lookup_state(fpc1020->fingerprint_pinctrl, n);
=======
			pinctrl_lookup_state(fpc1020->fingerprint_pinctrl, n);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
		if (IS_ERR(state)) {
			dev_err(dev, "cannot find '%s'\n", n);
			rc = -EINVAL;
			goto exit;
		}
		dev_info(dev, "found pin control %s\n", n);
		fpc1020->pinctrl_state[i] = state;
	}

	atomic_set(&fpc1020->wakeup_enabled, 1);

<<<<<<< HEAD
	fpc1020->irqf = IRQF_TRIGGER_RISING | IRQF_ONESHOT | IRQF_LITTLE_AFFINE;
	fpc1020->irq_requested = false;
	fpc1020->gpios_requested = false;
	device_init_wakeup(dev, 1);
/*
	if (of_property_read_bool(dev->of_node, "fpc,enable-wakeup")) {
		irqf |= IRQF_NO_SUSPEND;
=======
	fpc1020->irqf = IRQF_TRIGGER_RISING | IRQF_ONESHOT | IRQF_NO_SUSPEND;
/*
	if (of_property_read_bool(dev->of_node, "fpc,enable-wakeup")) {
		fpc1020->irqf |= IRQF_NO_SUSPEND;
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
		device_init_wakeup(dev, 1);
	}
*/
	mutex_init(&fpc1020->lock);

<<<<<<< HEAD
	fpc1020->ttw_wl = wakeup_source_register(dev, "fpc_ttw_wl");
	if (!fpc1020->ttw_wl)
		return -ENOMEM;

	rc = sysfs_create_group(&dev->kobj, &attribute_group);
	if (rc) {
		dev_err(dev, "fpc could not create sysfs\n");
		goto exit;
	}

	rc = reset_gpio_res(fpc1020);
	if (rc) {
		dev_err(dev, "fpc %s: could not initialize GPIOs No.!\n",
			__func__);
=======
	wakeup_source_init(&fpc1020->ttw_wl, "fpc_ttw_wl");
	wakeup_source_init(&fpc1020->screen_wl, "fpc_screen_wl");

	rc = sysfs_create_group(&dev->kobj, &attribute_group);
	if (rc) {
		dev_err(dev, "could not create sysfs\n");
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
		goto exit;
	}

	if (of_property_read_bool(dev->of_node, "fpc,enable-on-boot")) {
		dev_info(dev, "Enabling hardware\n");
		(void)device_prepare(fpc1020, true);
	}

	fpc1020->fb_black = false;
	fpc1020->wait_finger_down = false;
<<<<<<< HEAD
#ifndef FPC_DRM_INTERFACE_WA
	INIT_WORK(&fpc1020->work, notification_work);
	fpc1020->fb_notifier = fpc_notif_block;
	mi_drm_register_client(&fpc1020->fb_notifier);
#endif

	//rc = hw_reset(fpc1020);
=======
	INIT_WORK(&fpc1020->work, notification_work);
	fpc1020->fb_notifier = fpc_notif_block;
	drm_register_client(&fpc1020->fb_notifier);
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)

	dev_info(dev, "%s: ok\n", __func__);

exit:
<<<<<<< HEAD
	dev_info(dev, "fpc %s <---: exit! \n", __func__);
=======
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	return rc;
}

static int fpc1020_remove(struct platform_device *pdev)
{
	struct fpc1020_data *fpc1020 = platform_get_drvdata(pdev);

<<<<<<< HEAD
#ifndef FPC_DRM_INTERFACE_WA
	mi_drm_unregister_client(&fpc1020->fb_notifier);
#endif
	sysfs_remove_group(&pdev->dev.kobj, &attribute_group);
	mutex_destroy(&fpc1020->lock);
	wakeup_source_unregister(fpc1020->ttw_wl);
	(void)vreg_setup(fpc1020, "vdd_ana", false);
	(void)reset_gpio_res(fpc1020);
	/*
	   (void)vreg_setup(fpc1020, "vdd_io", false);
	   (void)vreg_setup(fpc1020, "vcc_spi", false);
	 */
=======
	drm_unregister_client(&fpc1020->fb_notifier);
	sysfs_remove_group(&pdev->dev.kobj, &attribute_group);
	mutex_destroy(&fpc1020->lock);
	wakeup_source_trash(&fpc1020->ttw_wl);
	wakeup_source_trash(&fpc1020->screen_wl);
	(void)vreg_setup(fpc1020, "vdd_ana", false);
    /*
	(void)vreg_setup(fpc1020, "vdd_io", false);
	(void)vreg_setup(fpc1020, "vcc_spi", false);
    */
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	dev_info(&pdev->dev, "%s\n", __func__);

	return 0;
}

static struct of_device_id fpc1020_of_match[] = {
<<<<<<< HEAD
	{.compatible = "fpc,fpc1020",},
	{}
};

=======
	{ .compatible = "fpc,fpc1020", },
	{}
};
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
MODULE_DEVICE_TABLE(of, fpc1020_of_match);

static struct platform_driver fpc1020_driver = {
	.driver = {
<<<<<<< HEAD
		   .name = "fpc1020",
		   .owner = THIS_MODULE,
		   .of_match_table = fpc1020_of_match,
		   },
	.probe = fpc1020_probe,
	.remove = fpc1020_remove,
=======
		.name	= "fpc1020",
		.owner	= THIS_MODULE,
		.of_match_table = fpc1020_of_match,
	},
	.probe	= fpc1020_probe,
	.remove	= fpc1020_remove,
>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
};

static int __init fpc1020_init(void)
{
<<<<<<< HEAD
	int rc;

	rc = platform_driver_register(&fpc1020_driver);
=======
	int rc = platform_driver_register(&fpc1020_driver);

>>>>>>> af11559817c0 (drivers: input: fingerprint: Import goodix and fpc drivers)
	if (!rc)
		pr_info("%s OK\n", __func__);
	else
		pr_err("%s %d\n", __func__, rc);

	return rc;
}

static void __exit fpc1020_exit(void)
{
	pr_info("%s\n", __func__);
	platform_driver_unregister(&fpc1020_driver);
}

module_init(fpc1020_init);
module_exit(fpc1020_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Aleksej Makarov");
MODULE_AUTHOR("Henrik Tillman <henrik.tillman@fingerprints.com>");
MODULE_DESCRIPTION("FPC1020 Fingerprint sensor device driver.");
