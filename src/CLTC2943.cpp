
#include "CLTC2943.hpp"
#include "pca9543a.h"

namespace LTC2943
{
	CLTC2943::CLTC2943(std::uint8_t batIndex)
	{
		batIndex_ = batIndex;
		isPresent_ = false;
		
		// select slave
		//i2cStatus_ = HAL_I2C_Master_Transmit(&hi2c1, PCA9543A_I2C_ADDR, &batIndex_, 1, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return;
		}
		
		// power down to set capacity register
		i2cBuffer_[0] = LTC2943_CTRL_ADDR;
		i2cBuffer_[1] = 1;
		//i2cStatus_ = HAL_I2C_Master_Transmit(&hi2c1, LTC2943_I2C_ADDR, i2cBuffer_, 2, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return;
		}
			
		// set capacity register to max
		i2cBuffer_[0] = LTC2943_ACC_MSB_ADDR;
		i2cBuffer_[1] = 0xFF;
		i2cBuffer_[2] = 0xFF;
		//i2cStatus_ = HAL_I2C_Master_Transmit(&hi2c1, LTC2943_I2C_ADDR, i2cBuffer_, 3, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return;
		}
		
		// set ADC in automatic mode and prescaler
		i2cBuffer_[0] = LTC2943_CTRL_ADDR;
		i2cBuffer_[1] = LTC2943_ADC_MODE_AUTO | LTC2943_PRESCALER_1024;
	
		//i2cStatus_ = HAL_I2C_Master_Transmit(&hi2c1, LTC2943_I2C_ADDR, i2cBuffer_, 2, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return;
		}
		
		isPresent_ = true;
	}
	
	CLTC2943::~CLTC2943()
	{

	}

	void CLTC2943::StartConv()
	{
		if (!isPresent_)
			return;
		
		// select slave
		//i2cStatus_ = HAL_I2C_Master_Transmit(&hi2c1, PCA9543A_I2C_ADDR, &batIndex_, 1, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return;
		}
		
		i2cBuffer_[0] = LTC2943_CTRL_ADDR;
		i2cBuffer_[1] = LTC2943_ADC_MODE_MAN | LTC2943_PRESCALER_1024;
	
		//i2cStatus_ = HAL_I2C_Master_Transmit(&hi2c1, LTC2943_I2C_ADDR, i2cBuffer_, 2, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return;
		}
	}
	
	float CLTC2943::GetCapacity()
	{
		if (!isPresent_)
			return 0.;
			
		std::int16_t capacity = 0xFFFF - GetData(LTC2943_ACC_MSB_ADDR);

		return (LTC2943_Q_LSB * (float)capacity);
	}
	
	float CLTC2943::GetVoltage()
	{
		if (!isPresent_)
			return 0.;
		
		return (23.6 * GetData(LTC2943_VOLTAGE_MSB_ADDR) / 0xFFFF);
	}
	
	float CLTC2943::GetCurrent()
	{
		if (!isPresent_)
			return 0.;

		std::int16_t current = 32767 - GetData(LTC2943_CURRENT_MSB_ADDR);
		float d = 60.0 / LTC2943_RSENSE * (float)current / 32767.0;

		return d;
	}
	
	float CLTC2943::GetTemperature()
	{
		if (!isPresent_)
			return 0.;
		
		return (510 * (std::uint16_t)GetData(LTC2943_TEMP_MSB_ADDR) / 0xFFFF - 273.15);
	}
	
	std::int16_t CLTC2943::GetData(std::uint8_t addr)
	{
		// select slave
		//i2cStatus_ = HAL_I2C_Master_Transmit(&hi2c1, PCA9543A_I2C_ADDR, &batIndex_, 1, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return 0;
		}

		//i2cStatus_ = HAL_I2C_Mem_Read(&hi2c1, LTC2943_I2C_ADDR, addr, 1, i2cBuffer_, 2, -1);
		if (i2cStatus_ != HAL_OK)
		{
			isPresent_ = false;
			return 0;
		}
		
		return ((i2cBuffer_[0] << 8) | i2cBuffer_[1]);
	}
}
