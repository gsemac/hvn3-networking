#pragma once
#include "net/UdpConnection.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class ReliableUdpConnection : public UdpConnection {

				struct PacketHeader {
					uint32_t ProtocolId;
				};

			public:
				ReliableUdpConnection(const IPEndPoint& local_endpoint);
				ReliableUdpConnection(Port local_port);

				uint32_t ProtocolId() const;
				void SetProtocolId(uint32_t value);
				size_t HeaderSize() const;
				
				virtual size_t Send(Byte data[], size_t length) override;
				virtual size_t Receive(Byte data[], size_t length) override;

			protected:
				virtual bool HandleDatagram(const IPEndPoint& sender, Byte data[], size_t length) override;

			private:
				uint32_t _protocol_id;

				// Writes packet header to the buffer. Assumes that the buffer is large enough to contain the header.
				void _writeHeader(Byte* buf) const;
				size_t _write(Byte* buf, uint32_t value) const;
				// Reads packet header from the buffer. Assumes that the buffer is large enough to contain the header.
				PacketHeader _readHeader(Byte* buf) const;
				size_t _read(Byte* buf, uint32_t& value) const;
				
			};

		}
	}
}