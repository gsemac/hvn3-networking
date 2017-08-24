#pragma once
#include "net/UdpConnection.h"
#include "net/ReliabilitySystem.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class ReliableUdpConnection : public UdpConnection {

				struct PacketHeader {
					uint32_t ProtocolId;
					ReliabilitySystem::sequence_number_type SequenceNumber;
					ReliabilitySystem::sequence_number_type Ack;
					ReliabilitySystem::ack_bits_type AckBits;
				};

			public:
				ReliableUdpConnection(const IPEndPoint& local_endpoint);
				ReliableUdpConnection(Port local_port);

				uint32_t ProtocolId() const;
				void SetProtocolId(uint32_t value);
				size_t HeaderSize() const;
				
				virtual size_t Send(Byte data[], size_t length) override;
				virtual size_t Receive(Byte data[], size_t length) override;

				virtual void OnUpdate(float dt) override;

			protected:
				virtual void OnReset() override;
				virtual bool HandleReceivedDatagram(const IPEndPoint& sender, Byte data[], size_t length) override;

			private:
				uint32_t _protocol_id;
				ReliabilitySystem _reliability_system;

				bool _sequence_numbers_enabled;
				bool _acks_enabled;

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