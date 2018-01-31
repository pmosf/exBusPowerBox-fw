#ifndef __EXSENSORLIST_H
#define __EXSENSORLIST_H

#include <cstdint>
#include <map>
#include <string>

namespace Jeti
{
namespace Sensor
{
enum class IndexList
{
	V_BAT1,
	V_BAT2,
	I_BAT1,
	I_BAT2,
	T_LOCAL,
	T_EXT1,
	T_EXT2,
	RPM1,
	RPM2,
	V_EXT1,
	V_EXT2
};

class CList
{
public:
	CList();
	const std::map<std::string, int> Get();

private:
	std::map<std::string, int> list_;
};
}
}

#endif
