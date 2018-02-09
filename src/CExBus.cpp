#include "CExBus.hpp"
#include "crc.h"

namespace Jeti
{
	namespace ExBus
	{
    packet_t CExBus::telemetryData_ __attribute__((aligned(4))) =
		{ 0x3B, 0x01, 0, 0, 0x3A };
		packet_t CExBus::telemetryText_[32] __attribute__((aligned(4)));

		CExBus::CExBus(std::string exTelemetryDeviceName) :
				telemetryDevice_(exTelemetryDeviceName, 0xA400, 0)
		{
			textDescriptorIndex_ = 0;
			dataDescriptorIndex_ = 0;
			exTelemetryTextPacketLen_ = 0;
			//servoValues_ = (std::uint16_t*) packet_.data;

			// create telemetry text packet for device
			telemetryText_[0].header[0] = 0x3B;
			telemetryText_[0].header[1] = 0x01;
			telemetryText_[0].pktLen = 8 + telemetryDevice_.GetTextDescriptor().size() - 1; // 0x7E separator is not sent
			telemetryText_[0].packetId = 0;
			telemetryText_[0].dataId = 0x3A;
			telemetryText_[0].dataLength = telemetryDevice_.GetTextDescriptor().size() - 1;

			for (size_t i = 0; i < telemetryDevice_.GetTextDescriptor().size(); ++i)
			{
				telemetryText_[0].data[i] = telemetryDevice_.GetTextDescriptor()[i + 1]; // 0x7E separator is skipped
			}
		}

		CExBus::~CExBus()
		{
		}

		std::uint16_t& CExBus::GetServoValue(int index)
		{
			return servoValues_[index];
		}

		bool CExBus::checkHeader(packet_t& pkt)
		{
			if ((pkt.header[0] == 0x3E || pkt.header[0] == 0x3D) && (pkt.header[1] == 0x01 || pkt.header[1] == 0x03))
				return JETI_EXBUS_PKT_HEADER_OK;
			else
				return !JETI_EXBUS_PKT_HEADER_OK;
		}

		bool CExBus::checkCRC(packet_t& pkt)
		{
			uint16_t crc = (pkt.data[pkt.pktLen - 7] << 8) | pkt.data[pkt.pktLen - 8];

			if (crc != get_crc16((uint8_t*) &pkt, pkt.pktLen - 2))
				return !JETI_EXBUS_PKT_CRC_OK;
			else
				return JETI_EXBUS_PKT_CRC_OK;
		}

		std::uint8_t* CExBus::GetExTelemetryTextPacket()
		{
			if (textDescriptorIndex_ > telemetryDevice_.GetSensorCollection().size())
				textDescriptorIndex_ = 0;

			telemetryText_[textDescriptorIndex_].packetId = packet_.packetId;

			uint16_t crc = get_crc16(telemetryText_[textDescriptorIndex_].header, telemetryText_[textDescriptorIndex_].pktLen - 2);

			telemetryText_[textDescriptorIndex_].data[telemetryText_[textDescriptorIndex_].dataLength] = crc;

			telemetryText_[textDescriptorIndex_].data[telemetryText_[textDescriptorIndex_].dataLength + 1] = crc >> 8;

			exTelemetryTextPacketLen_ = telemetryText_[textDescriptorIndex_].pktLen;

			return telemetryText_[textDescriptorIndex_++].header;
		}

		std::uint8_t CExBus::GetExTelemetryTextPacketSize()
		{
			return exTelemetryTextPacketLen_;
		}

		std::uint8_t* CExBus::GetExTelemetryDataPacket()
		{
			if (dataDescriptorIndex_ >= telemetryDevice_.GetDataDescriptor().size())
				dataDescriptorIndex_ = 0;

			std::vector<std::uint8_t>& currentDesc = telemetryDevice_.GetDataDescriptor()[dataDescriptorIndex_++];

			currentDesc[currentDesc.size() - 1] = get_crc8(&(currentDesc[2]), currentDesc.size() - 3);

			telemetryData_.packetId = packet_.packetId;
			telemetryData_.pktLen = 8 + currentDesc.size() - 1;
			telemetryData_.dataLength = currentDesc.size() - 1;

			for (size_t i = 0; i < currentDesc.size() - 1; ++i)
			{
				telemetryData_.data[i] = currentDesc[i + 1]; // 0x7E separator is skipped
			}

			uint16_t crc = get_crc16((std::uint8_t*) &telemetryData_, telemetryData_.pktLen - 2);
			telemetryData_.data[telemetryData_.dataLength] = crc;
			telemetryData_.data[telemetryData_.dataLength + 1] = crc >> 8;

			return telemetryData_.header;
		}

		std::uint8_t CExBus::GetExTelemetryDataPacketSize()
		{
			return telemetryData_.pktLen;
		}

		Device::CExDevice& CExBus::GetTelemetryDevice()
		{
			return telemetryDevice_;
		}

		void CExBus::AddTelemetryVoltage(std::string name, bool last)
		{
			telemetryDevice_.AddVoltageSensor(name, last);
			AddTelemetryText();
		}

		void CExBus::AddTelemetryCurrent(std::string name, bool last)
		{
			telemetryDevice_.AddCurrentSensor(name, "A", last);
			AddTelemetryText();
		}

		void CExBus::AddTelemetryCurrent(std::string name, std::string unit, bool last)
		{
			telemetryDevice_.AddCurrentSensor(name, unit, last);
			AddTelemetryText();
		}

		void CExBus::AddTelemetryTemperature(std::string name, bool last)
		{
			telemetryDevice_.AddTemperatureSensor(name, last);
			AddTelemetryText();
		}

		void CExBus::AddTelemetryText()
		{
			telemetryText_[telemetryDevice_.GetSensorCollection().size()].header[0] = 0x3B;
			telemetryText_[telemetryDevice_.GetSensorCollection().size()].header[1] = 0x01;
			telemetryText_[telemetryDevice_.GetSensorCollection().size()].dataId = 0x3A;

			telemetryText_[telemetryDevice_.GetSensorCollection().size()].pktLen = 8 + telemetryDevice_.GetSensorCollection().back().get()->GetTextDescriptor().size()
					- 1; // 0x7F separator is not sent
			telemetryText_[telemetryDevice_.GetSensorCollection().size()].dataLength = telemetryDevice_.GetSensorCollection().back().get()->GetTextDescriptor().size()
					- 1;

			for (int i = 0; i < telemetryText_[telemetryDevice_.GetSensorCollection().size()].dataLength; ++i)
				telemetryText_[telemetryDevice_.GetSensorCollection().size()].data[i] = telemetryDevice_.GetSensorCollection().back().get()->GetTextDescriptor()[i + 1];
		}
	}
}
