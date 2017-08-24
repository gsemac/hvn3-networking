#pragma once
#include <cstdint>
#include <list>

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class ReliabilitySystem {

			public:
				typedef uint32_t sequence_number_type;
				typedef uint32_t ack_bits_type;

			private:
				struct DatagramInfo {
					sequence_number_type SequenceNumber;
					float Age;
					size_t Length;
				};

			public:
				ReliabilitySystem();

				sequence_number_type LocalSequenceNumber() const;
				sequence_number_type RemoteSequenceNumber() const;
				sequence_number_type MaxSequenceNumber() const;
				ack_bits_type GetPendingAckBits() const;

				void DatagramSent(size_t length);
				void DatagramReceived(sequence_number_type sequence_number, size_t length);

				void OnUpdate(float dt);

				void Reset();

			private:
				sequence_number_type _local_sequence_number;
				sequence_number_type _remote_sequence_number;
				std::list<DatagramInfo> _sent; // Used to calculate sent bandwidth.
				std::list<DatagramInfo> _sent_awaiting_ack; // Datagrams that have not yet been acked.
				std::list<DatagramInfo> _received; // Datagrams received for generating ack bitfield.

				unsigned int _datagrams_sent;
				unsigned int _datagrams_received;

				// Returns true if s2 is newer than s1, taking into account sequence number wrap-around.
				bool _sequenceNumberIsMoreRecent(sequence_number_type s1, sequence_number_type s2) const;
				bool _seqenceNumberExists(const std::list<DatagramInfo>& info, sequence_number_type s) const;

			};

		}
	}
}