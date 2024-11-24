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

/* DS3231 I2C Bit Masks Registers Values */
#define DS3231_MASK_SECONDS 0b00001111u
#define DS3231_MASK_10_SECONDS 0b01110000u
#define DS3231_MASK_MINUTES 0b00001111u
#define DS3231_MASK_10_MINUTES 0b01110000u
#define DS3231_MASK_HOURS 0b00001111u
#define DS3231_MASK_10_HOURS 0b00010000u
#define DS3231_MASK_20_HOURS 0b00100000u
#define DS3231_MASK_SELECT_HOURS 0b01000000u 
#define DS3231_MASK_DAY 0b00000111u
#define DS3231_MASK_DATE 0b00001111u
#define DS3231_MASK_10_DATE 0b00110000u
#define DS3231_MASK_MONTH 0b00001111u
#define DS3231_MASK_10_MONTH 0b00010000u
#define DS3231_MASK_CENTURY_MONTH 0b10000000
#define DS3231_MASK_YEAR 0b00001111u
#define DS3231_MASK_10_YEAR 0b11110000u
#define DS3231_MASK_EOSC 0b10000000u
#define DS3231_MASK_INTCN 0b00000100u
#define DS3231_MASK_A2IE 0b00000010u
#define DS3231_MASK_A1IE 0b00000001u
#define DS3231_MASK_OSF 0b10000000u
#define DS3231_MASK_BSY 0b00000100u

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

//static int ds3231_write_time(ds3231_time time);

//static int ds3231_read_time(ds3231_time time);