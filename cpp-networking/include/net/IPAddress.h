#pragma once
#include <string>
#include <iostream>
#include <cstdint>

namespace hvn3 {
	namespace Net {

		class IPAddress {

		public:
			IPAddress();
			IPAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d);
			IPAddress(uint32_t address);
			IPAddress(const char* address);

			// Returns object representing the address 127.0.0.1.
			static IPAddress LocalHost();
			// Returns object representing the address 255.255.255.255.
			static IPAddress Broadcast();
			// Returns object representing the address 0.0.0.0.
			static IPAddress Any();

			// Returns integer representation of the address.
			uint32_t Address() const;

			bool operator==(const IPAddress& other) const;
			bool operator!=(const IPAddress& other) const;

			// Returns string representation of the address.
			std::string ToString() const;

		private:
			uint32_t _address;

		};

		std::ostream& operator << (std::ostream& stream, const IPAddress& value);

	}
}