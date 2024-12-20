//
// MockTime.hpp
//
//  Created on: Sep 28, 2018
//      Author: Alexander
//

#ifndef DRONEDEVICE_STUBS_MOCKTIME_HPP_
#define DRONEDEVICE_STUBS_MOCKTIME_HPP_

#include <chrono>
#include <cstdint>

namespace Device {

struct MockTime {
	MockTime() = delete;
	MockTime(const MockTime &) = delete;
	MockTime &operator=(const MockTime &) = delete;

	static uint64_t time()
	{
		return 0;
	}

	static std::chrono::microseconds microseconds()
	{
		return std::chrono::microseconds{0};
	}

	static std::chrono::milliseconds milliseconds()
	{
		return std::chrono::milliseconds{0};
	}

	static std::chrono::seconds seconds()
	{
		return std::chrono::seconds{0};
	}
};

} // namespace Device

#endif // DRONEDEVICE_STUBS_MOCKTIME_HPP_
