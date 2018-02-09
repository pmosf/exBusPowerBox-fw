#include "CMAX6639.hpp"
#include "pca9543a.h"

namespace MAX6639
{
	CMAX6639::CMAX6639() :
			isPresent_(true)
	{
		//i2cStatus_ = HAL_I2C_Mem_Read(&hi2c1, IIC_MAX6639_ADDR, MAX6639_DEVID_ADDR, 1, i2cBuffer_, 1, 1);
		if (i2cStatus_ != HAL_OK || *i2cBuffer_ != MAX6639_DEVICE_ID)
		{
			isPresent_ = false;
			return;
		}

		// set 2nd sensor as external
		*i2cBuffer_ = 0x20;
		//i2cStatus_ = HAL_I2C_Mem_Write(&hi2c1, IIC_MAX6639_ADDR, MAX6639_GCONF_ADDR, 1, i2cBuffer_, 1, 1);
		if (i2cStatus_ != HAL_OK)
			isPresent_ = false;
	}

	float CMAX6639::GetExtTemperature(std::uint8_t sensorIndex)
	{
		if (!isPresent_)
			return 254;

		//i2cStatus_ = HAL_I2C_Mem_Read(&hi2c1, IIC_MAX6639_ADDR, MAX6639_TEMP1_ADDR + sensorIndex, 1, &i2cBuffer_[0], 1, 1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return 254;
		}

		//i2cStatus_ = HAL_I2C_Mem_Read(&hi2c1, IIC_MAX6639_ADDR, MAX6639_EXT_TEMP1_ADDR + sensorIndex, 1, &i2cBuffer_[1], 1, 1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return 254;
		}

		// checking diode fault flag
		if (i2cBuffer_[1] & 1)
			return 255;

		return (i2cBuffer_[0] + (i2cBuffer_[1] >> 5)) * 0.125;
	}

	float CMAX6639::GetLocalTemperature(void)
	{
		if (!isPresent_)
			return 254;

		// set 2nd sensor as local
		*i2cBuffer_ = 0x30;
		//i2cStatus_ = HAL_I2C_Mem_Write(&hi2c1, IIC_MAX6639_ADDR, MAX6639_GCONF_ADDR, 1, i2cBuffer_, 1, 1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return 254;
		}

		//i2cStatus_ = HAL_I2C_Mem_Read(&hi2c1, IIC_MAX6639_ADDR, MAX6639_TEMP2_ADDR, 1, &i2cBuffer_[0], 1, 1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return 254;
		}

		//i2cStatus_ = HAL_I2C_Mem_Read(&hi2c1, IIC_MAX6639_ADDR, MAX6639_EXT_TEMP2_ADDR, 1, &i2cBuffer_[1], 1, 1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return 254;
		}

		return (i2cBuffer_[0] + (i2cBuffer_[1] >> 5)) * 0.125;
	}

	std::uint16_t CMAX6639::GetRpm(void)
	{
		if (!isPresent_)
			return 0;
	}
}
