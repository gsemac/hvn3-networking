#include "net/IPEndPoint.h"
#include <sstream>
#ifdef _WIN32
#define sscanf sscanf_s
#endif

namespace hvn3 {
	namespace Net {

		IPEndPoint::IPEndPoint() :
			IPEndPoint(0, 0) {
		}
		IPEndPoint::IPEndPoint(const Net::IPAddress& address, unsigned short port) {

			_address = address.Address();
			_port = port;

		}
		IPEndPoint::IPEndPoint(unsigned int address, unsigned short port) {

			_address = address;
			_port = port;

		}
		IPEndPoint::IPEndPoint(const char* address) {

			unsigned short a, b, c, d, e;
			sscanf_s(address, "%hu.%hu.%hu.%hu:%hu", &a, &b, &c, &d, &e);
			_address = (a << 24) | (b << 16) | (c << 8) | d;
			_port = e;

		}

		IPAddress IPEndPoint::IPAddress() const {

			return Net::IPAddress(_address);

		}
		unsigned int IPEndPoint::Address() const {

			return _address;

		}
		unsigned short IPEndPoint::Port() const {

			return _port;

		}

		bool IPEndPoint::operator==(const IPEndPoint& other) const {

			return (_address == other._address) && (_port == other._port);

		}
		bool IPEndPoint::operator!=(const IPEndPoint& other) const {

			return  !(*this == other);

		}

		std::string IPEndPoint::ToString() const {

			std::stringstream stream;

			stream << IPAddress().ToString();

			if (_port)
				stream << ":" << _port;

			return stream.str();

		}

		std::ostream& operator << (std::ostream& stream, const IPEndPoint& value) {

			stream << value.ToString();

			return stream;

		}

	}
}