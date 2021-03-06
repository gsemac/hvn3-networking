#include <sstream>
#include <cstdio>
#include "net/IPAddress.h"
#ifdef _WIN32
#define sscanf sscanf_s
#endif

namespace hvn3 {
	namespace Net {

		IPAddress::IPAddress() :
			IPAddress(0, 0, 0, 0) {
		}
		IPAddress::IPAddress(unsigned char a, unsigned char b, unsigned char c, unsigned char d) {

			_address = (a << 24) | (b << 16) | (c << 8) | d;

		}
		IPAddress::IPAddress(uint32_t address) {

			_address = address;

		}
		IPAddress::IPAddress(const char* address) {

			unsigned short a, b, c, d;
			sscanf(address, "%hu.%hu.%hu.%hu", &a, &b, &c, &d);
			_address = (a << 24) | (b << 16) | (c << 8) | d;

		}

		IPAddress IPAddress::LocalHost() {

			return IPAddress(127, 0, 0, 1);

		}
		IPAddress IPAddress::Broadcast() {

			return IPAddress(255, 255, 255, 255);

		}
		IPAddress IPAddress::Any() {

			return IPAddress(0, 0, 0, 0);

		}

		uint32_t IPAddress::Address() const {

			return _address;

		}

		bool IPAddress::operator==(const IPAddress& other) const {

			return (_address == other._address);

		}
		bool IPAddress::operator!=(const IPAddress& other) const {

			return  !(*this == other);

		}

		std::string IPAddress::ToString() const {

			std::stringstream stream;

			stream << (_address >> 24) << ".";
			stream << (_address >> 16 & 0xFF) << ".";
			stream << (_address >> 8 & 0xFF) << ".";
			stream << (_address & 0xFF);

			return stream.str();

		}

		std::ostream& operator << (std::ostream& stream, const IPAddress& value) {

			stream << value.ToString();

			return stream;

		}

	}
}