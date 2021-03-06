#pragma once
#include "net/IPAddress.h"

namespace hvn3 {
	namespace Net {

		class IPEndPoint {

		public:
			IPEndPoint();
			IPEndPoint(const IPAddress& address, unsigned short port);
			IPEndPoint(uint32_t address, unsigned short port);
			IPEndPoint(const char* address);

			// Returns IP address of the endpoint.
			Net::IPAddress IPAddress() const;
			// Returns integer representation of the address.
			uint32_t Address() const;
			// Returns port number of the endpoint.
			unsigned short Port() const;

			bool operator==(const IPEndPoint& other) const;
			bool operator!=(const IPEndPoint& other) const;

			// Returns string representation of the endpoint.
			std::string ToString() const;

		private:
			uint32_t _address;
			unsigned short _port;

		};

		std::ostream& operator << (std::ostream& stream, const IPEndPoint& value);

	}
}