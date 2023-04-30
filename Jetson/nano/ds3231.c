// SPDX-License-Identifier: GPL-2.0-only
/*
 * DS3231 I2C Real Time Clock driver
 */

#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/rtc.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/bcd.h>

#define RTC_DRV_DS3231 "ds3231"

#define DS3231_REG_SECONDS 0x00
#define DS3231_REG_MINUTES 0x01
#define DS3231_REG_HOURS 0x02
#define DS3231_REG_DAY 0x03
#define DS3231_REG_DATE 0x04
#define DS3231_REG_MONTH 0x05
#define DS3231_REG_YEAR 0x06

struct ds3231_data
{
    struct i2c_client *client;
    struct rtc_device *rtc;
};

static const struct of_device_id ds3231_of_match[] = {
    {.compatible = "maxim,ds3231"},
    {},
};
MODULE_DEVICE_TABLE(of, ds3231_of_match);

static int ds3231_read_time(struct device *dev, struct rtc_time *tm)
{
    struct ds3231_data *data = dev_get_drvdata(dev);
    struct i2c_client *client = data->client;
    u8 buf[7];

    /* Read RTC time registers */
    if (i2c_smbus_read_i2c_block_data(client, DS3231_REG_SECONDS, 7, buf) < 0)
        return -EIO;

    /* Convert RTC time to Linux format */
    tm->tm_sec = bcd2bin(buf[0] & 0x7f);
    tm->tm_min = bcd2bin(buf[1] & 0x7f);
    tm->tm_hour = bcd2bin(buf[2] & 0x3f);
    tm->tm_mday = bcd2bin(buf[4] & 0x3f);
    tm->tm_mon = bcd2bin(buf[5] & 0x1f) - 1;
    tm->tm_year = bcd2bin(buf[6]) + 100;

    return 0;
}

static int ds3231_set_time(struct device *dev, struct rtc_time *tm)
{
    struct ds3231_data *data = dev_get_drvdata(dev);
    struct i2c_client *client = data->client;
    u8 buf[7];

    /* Convert Linux format time to RTC format */
    buf[0] = bin2bcd(tm->tm_sec);
    buf[1] = bin2bcd(tm->tm_min);
    buf[2] = bin2bcd(tm->tm_hour);
    buf[3] = bin2bcd(tm->tm_wday + 1);
    buf[4] = bin2bcd(tm->tm_mday);
    buf[5] = bin2bcd(tm->tm_mon + 1);
    buf[6] = bin2bcd(tm->tm_year - 100);

    /* Write RTC time registers */
    if (i2c_smbus_write_i2c_block_data(client, DS3231_REG_SECONDS, 7, buf) < 0)
        return -EIO;

    return 0;
}

static const struct rtc_class_ops ds3231_rtc_ops = {
    .read_time = ds3231_read_time,
    .set_time = ds3231_set_time,
};

static int ds3231_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct ds3231_data *data;
    struct rtc_device *rtc;
    int ret;

    dev_info(&client->dev, "[DS3231] probing \n");

    data = devm_kzalloc(&client->dev, sizeof(struct ds3231_data), GFP_KERNEL);
    if (!data)
    {
        dev_err(&client->dev, "Failed to allocate memory\n");
        return -ENOMEM;
    }

    data->client = client;

    i2c_set_clientdata(client, data);

    rtc = rtc_device_register(client->name, &client->dev, &ds3231_rtc_ops, THIS_MODULE);
    if (IS_ERR(rtc))
    {
        ret = PTR_ERR(rtc);
        dev_err(&client->dev, "Failed to register rtc device\n");
        return ret;
    }

    data->rtc = rtc;

    rtc_update_irq(data->rtc, 1, RTC_AF | RTC_UF);

    if (ret < 0)
    {
        dev_err(&client->dev, "Failed to update RTC IRQ\n");
        return ret;
    }

    dev_info(&client->dev, "DS3231 RTC driver initialized\n");

    return 0;
}

static int ds3231_remove(struct i2c_client *client)
{
    struct ds3231_data *data = i2c_get_clientdata(client);

    rtc_device_unregister(data->rtc);

    return 0;
}

static const struct i2c_device_id ds3231_id[] = {
    {"ds3231", 0},
    {}};

static struct i2c_driver ds3231_driver = {
    .driver = {
        .name = "ds3231",
        .of_match_table = ds3231_of_match,
    },
    .probe = ds3231_probe,
    .remove = ds3231_remove,
    .id_table = ds3231_id,
};

module_i2c_driver(ds3231_driver);

MODULE_AUTHOR("Mohamed Lamine Kartobi");
MODULE_DESCRIPTION("DS3231 RTC driver");
MODULE_LICENSE("GPL");
