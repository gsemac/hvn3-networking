#pragma once
#include <cstdint>
#include <climits>

namespace hvn3 {
	namespace Net {

		template<typename T>
		T HostToNetworkOrder(T value) {
			
			uint8_t bytes[sizeof(T)];
			memcpy(&bytes, &value, sizeof(bytes));

			T result = 0;
			for (size_t i = 0; i < sizeof(bytes); ++i)
				result |= static_cast<T>(bytes[i]) << (CHAR_BIT * (sizeof(bytes) - i - 1U));

			return result;

		}

		template<typename T>
		T NetworkToHostOrder(T value) {

			uint8_t bytes[sizeof(T)];
			memcpy(&bytes, &value, sizeof(bytes));

			T result = 0;
			for (size_t i = 0; i < sizeof(bytes); ++i)
				result |= static_cast<T>(bytes[sizeof(bytes) - 1U - i]) << i * CHAR_BIT;

			return result;

		}
		
	}
}