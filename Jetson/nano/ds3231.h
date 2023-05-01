#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/rtc.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/bcd.h>
#include <linux/clk.h>
#include <linux/debugfs.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/regmap.h>

/* DS3231 I2C Time Registers Address */
#define DS3231_REG_ADDR_SECONDS 0x00
#define DS3231_REG_ADDR_MINUTES 0x01
#define DS3231_REG_ADDR_HOURS 0x02
#define DS3231_REG_ADDR_DAY 0x03
#define DS3231_REG_ADDR_DATE 0x04
#define DS3231_REG_ADDR_MONTH_CENTURY 0x05
#define DS3231_REG_ADDR_YEAR 0x06

/* DS3231 I2C Alarm Registers Address */
#define DS3231_REG_ADDR_ALARM_1_SECONDS 0x07
#define DS3231_REG_ADDR_ALARM_1_MINUTES 0x08
#define DS3231_REG_ADDR_ALARM_1_HOURS 0x09
#define DS3231_REG_ADDR_ALARM_1_DAY_DATE 0x0A
#define DS3231_REG_ADDR_ALARM_2_MINUTES 0x0B
#define DS3231_REG_ADDR_ALARM_2_HOURS 0x0C
#define DS3231_REG_ADDR_ALARM_2_DAY_DATE 0x0D

/* DS3231 I2C Control Registers Address */
#define DS3231_REG_ADDR_CONTROL 0x0E
#define DS3231_REG_ADDR_CONTROL_STATUS 0x0F
#define DS3231_REG_ADDR_AGING_OFFSET 0x10
#define DS3231_REG_ADDR_MSB_TEMP 0x11
#define DS3231_REG_ADDR_LSB_TEMP 0x12

/* DS3231 time data structure */
typedef struct _ds3231_time
{
    u8 second;  /* Second part of the time (0-59) */
    u8 minute;  /* Minute part of the time (0-59) */
    u8 hour;    /* Hour part of the time (0-23) */

    u8 month;   /* Month part of the time (1-12) */
    u8 day;     /* Day of the month part of the time (0-32) */
    u16 year;   /* Year part of the time (2000-2199) */
} ds3231_time;

/**
 * ds3231_write_reg() - Write a value to a register on the DS3231 RTC
 * @dev: Pointer to the device structure
 * @addr: The address of the register to write to
 * @val: The value to write to the register
 *
 * This function writes a value to a register on the DS3231 RTC using the
 * provided device pointer, register address, and value. It uses the regmap
 * subsystem to perform the actual I2C write operation, and includes error
 * handling to report any I2C write failures.
 *
 * Returns: 0 on success, or an error code on failure
 */
static int ds3231_write_reg(struct device *dev, u16 addr, u8 val);

/**
 * ds3231_read_reg() - read a register value from the DS3231 RTC
 *
 * @dev: pointer to the device structure
 * @addr: the register address to read from
 * @val: pointer to store the value read from the register
 *
 * Return: 0 on success, negative error code on failure
 */
// static int ds3231_read_reg(struct device *dev, u16 addr, unsigned int *val)

//static int ds3231_write_time(ds3231_time time);


//static int ds3231_read_time(ds3231_time time);