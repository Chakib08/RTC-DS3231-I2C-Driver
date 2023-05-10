// SPDX-License-Identifier: GPL-2.0-only
/*
 * DS3231 I2C Real Time Clock driver
 */

#include "ds3231.h"

/* DS3231 data structure */
struct ds3231
{
    struct i2c_client *client; 
    struct regmap *regmap;
};

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
static int ds3231_write_reg(struct device *dev, u16 addr, u8 val)
{
    struct ds3231 *priv = dev_get_drvdata(dev);   // Get the pointer to the DS3231 private data
    int err;

    err = regmap_write(priv->regmap, addr, val);  // Write the value to the specified register using regmap
    if (err)
        dev_err(dev, "%s:i2c write failed, 0x%x = %x\n", __func__, addr, val); // Report any I2C write errors
    
    dev_info(dev, "%s:i2c write, 0x%x = %x\n", __func__, addr, val); // Report any I2C write

    /* delay before next i2c command */
    usleep_range(100, 110);  // Add a delay to ensure the I2C bus has enough time to settle

    return err; // Return the error code (if any)
}

/**
 * ds3231_read_reg() - read a register value from the DS3231 RTC
 *
 * @dev: pointer to the device structure
 * @addr: the register address to read from
 * @val: pointer to store the value read from the register
 *
 * Return: 0 on success, negative error code on failure
 */
static int ds3231_read_reg(struct device *dev, u16 addr, u32 *val)
{
    struct ds3231 *priv = dev_get_drvdata(dev);
    int err;

    /* Read the value from the register at the specified address */
    err = regmap_read(priv->regmap, addr, val);
    dev_info(dev, "%s:i2c read data, 0x%x = 0x%x", __func__, addr, *val);

    if (err)
        dev_err(dev, "%s:i2c read failed, 0x%x\n", __func__, addr);

    /* Delay before the next I2C command to avoid overloading the bus */
    usleep_range(100, 110);

    return err;
}

void ds3231_initialize(struct device *dev)
{
    ds3231_write_reg(dev, DS3231_REG_ADDR_SECONDS, 0x04);
    ds3231_write_reg(dev, DS3231_REG_ADDR_MINUTES, 0x03);
    ds3231_write_reg(dev, DS3231_REG_ADDR_HOURS, 0x02);
    ds3231_write_reg(dev, DS3231_REG_ADDR_DAY, 0x07);
}

static struct regmap_config ds3231_regmap_config = {
	.reg_bits = 16,
	.val_bits = 8,
	.cache_type = REGCACHE_RBTREE,
};

static int ds3231_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    struct ds3231 *priv;
    int err = 0;
    u32 data;

    //struct device_node *node = client->dev.of_node;

    dev_info(&client->dev, "[DS3231]: probing Real Time Clock\n");

    priv = devm_kzalloc(&client->dev, sizeof(*priv), GFP_KERNEL);
    priv->client = client;
    priv->regmap = devm_regmap_init_i2c(priv->client,
                                        &ds3231_regmap_config);
    if(IS_ERR(priv->regmap))
    {
        dev_err(&client->dev, "regmap init failed: %ld\n", PTR_ERR(priv->regmap));
        return -ENODEV;
    }

    dev_set_drvdata(&client->dev, priv);


    ds3231_initialize(&client->dev);
    ds3231_read_reg(&client->dev, DS3231_REG_ADDR_SECONDS, &data);
    ds3231_read_reg(&client->dev, DS3231_REG_ADDR_MINUTES, &data);
    ds3231_read_reg(&client->dev, DS3231_REG_ADDR_HOURS,  &data);
    ds3231_read_reg(&client->dev, DS3231_REG_ADDR_DAY, &data);

	dev_info(&client->dev, "%s:  success\n", __func__);

	return err;
}

static int ds3231_remove(struct i2c_client *client)
{
	struct ds3231 *priv;

	if (client != NULL) {
		priv = dev_get_drvdata(&client->dev);
		i2c_unregister_device(client);
		client = NULL;
	}

	return 0;
}

/* List of compatible devices that should be the same as in the device tree */
static const struct of_device_id ds3231_of_match[] = {
    {.compatible = "maxim,ds3231"},
    {},
};

static const struct i2c_device_id ds3231_id[] = {
	{ "ds3231", 0 },
	{ },
};

MODULE_DEVICE_TABLE(of, ds3231_of_match);
MODULE_DEVICE_TABLE(i2c, ds3231_id);

static struct i2c_driver ds3231_i2c_driver = {
	.driver = {
		.name = "ds3231",
		.owner = THIS_MODULE,
	},
	.probe = ds3231_probe,
	.remove = ds3231_remove,
	.id_table = ds3231_id,
};

static int __init ds3231_init(void)
{
	return i2c_add_driver(&ds3231_i2c_driver);
}

static void __exit ds3231_exit(void)
{
	i2c_del_driver(&ds3231_i2c_driver);
}

module_init(ds3231_init);
module_exit(ds3231_exit);

MODULE_DESCRIPTION("RTC driver ds3231");
MODULE_AUTHOR("Mohamed Lamine Kartobi <chakibdace@gmail.com>");
MODULE_LICENSE("GPL v2");
