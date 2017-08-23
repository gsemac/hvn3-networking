#include "net/ReliableUdpConnection.h"
#include <cstring>

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			ReliableUdpConnection::ReliableUdpConnection(const IPEndPoint& local_endpoint) :
				UdpConnection(local_endpoint) {

				_protocol_id = 0;

			}
			ReliableUdpConnection::ReliableUdpConnection(Port local_port) :
				ReliableUdpConnection(IPEndPoint(IPAddress::LocalHost(), local_port)) {
			}

			uint32_t ReliableUdpConnection::ProtocolId() const {

				return _protocol_id;

			}
			void ReliableUdpConnection::SetProtocolId(uint32_t value) {

				_protocol_id = value;

			}
			size_t ReliableUdpConnection::HeaderSize() const {

				size_t total_size = 0;

				if (_protocol_id != 0)
					total_size += sizeof(_protocol_id);

				return total_size;

			}

			size_t ReliableUdpConnection::Send(Byte data[], size_t length) {

				if (length <= 0)
					return 0;

				Byte* datagram = new Byte[length + HeaderSize()];

				_writeHeader(datagram);
				memcpy(datagram + HeaderSize(), data, length);

				size_t bytes_sent = UdpConnection::Send(datagram, length + HeaderSize());

				delete[] datagram;

				if (bytes_sent <= HeaderSize())
					return 0;
				else
					return bytes_sent - HeaderSize();

			}
			size_t ReliableUdpConnection::Receive(Byte data[], size_t length) {

				if (length <= 0)
					return 0;

				Byte* datagram = new Byte[length + HeaderSize()];

				// UdpConnection's Receive method will call our virtual HandleDatagram method.
				size_t bytes_received = UdpConnection::Receive(datagram, length + HeaderSize());

				if (bytes_received <= 0) {
					delete[] datagram;
					return 0;
				}
	
				memcpy(data, datagram + HeaderSize(), bytes_received - HeaderSize());

				delete[] datagram;

				return bytes_received - HeaderSize();

			}


			bool ReliableUdpConnection::HandleDatagram(const IPEndPoint& sender, Byte data[], size_t length) {

				if (length <= HeaderSize())
					return false;

				PacketHeader header = _readHeader(data);

				if (_protocol_id != 0 && _protocol_id != header.ProtocolId)
					return false;

				// If UdpConnection's HandleDatagram accepts the datagram, it will also complete a pending connection.
				if (!UdpConnection::HandleDatagram(sender, data, length))
					return false;

				return true;

			}


			void ReliableUdpConnection::_writeHeader(Byte* buf) const {

				if (_protocol_id != 0)
					_write(buf, _protocol_id);

			}
			size_t ReliableUdpConnection::_write(Byte* buf, uint32_t value) const {

				buf[0] = static_cast<Byte>(value >> 24);
				buf[1] = static_cast<Byte>((value >> 16) & 0xFF);
				buf[2] = static_cast<Byte>((value >> 8) & 0xFF);
				buf[3] = static_cast<Byte>(value & 0xFF);

				return sizeof(value);

			}
			ReliableUdpConnection::PacketHeader ReliableUdpConnection::_readHeader(Byte* buf) const {

				PacketHeader header;

				if (_protocol_id != 0)
					_read(buf, header.ProtocolId);
				else
					header.ProtocolId = 0;

				return header;

			}
			size_t ReliableUdpConnection::_read(Byte* buf, uint32_t& value) const {

				value = (((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | ((uint32_t)buf[3]));

				return sizeof(value);

			}

		}
	}
}