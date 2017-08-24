#include "net/ReliabilitySystem.h"
#include <limits>
#include <iostream>

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			ReliabilitySystem::ReliabilitySystem() {

				Reset();

			}

			ReliabilitySystem::sequence_number_type ReliabilitySystem::LocalSequenceNumber() const {

				return _local_sequence_number;

			}
			ReliabilitySystem::sequence_number_type ReliabilitySystem::RemoteSequenceNumber() const {

				return _remote_sequence_number;

			}
			ReliabilitySystem::sequence_number_type ReliabilitySystem::MaxSequenceNumber() const {

				return std::numeric_limits<sequence_number_type>::max();

			}
			ReliabilitySystem::ack_bits_type ReliabilitySystem::GetPendingAckBits() const {

				ack_bits_type ack_bits = 0;
				sequence_number_type remote_sequence_number = RemoteSequenceNumber();

				for (auto i = _received.begin(); i != _received.end(); ++i) {

					if (i->SequenceNumber == remote_sequence_number || _sequenceNumberIsMoreRecent(i->SequenceNumber, remote_sequence_number))
						break;

					ack_bits_type bit_index;
					if (i->SequenceNumber > remote_sequence_number)
						bit_index = remote_sequence_number + (MaxSequenceNumber() - i->SequenceNumber);
					else
						bit_index = remote_sequence_number - 1 - i->SequenceNumber;

					if (bit_index < sizeof(ack_bits_type) * CHAR_BIT)
						ack_bits |= 1 << bit_index;

				}

				return ack_bits;

			}

			void ReliabilitySystem::DatagramSent(size_t length) {

				DatagramInfo info;
				info.Age = 0.0f;
				info.Length = length;
				info.SequenceNumber = LocalSequenceNumber();

				_sent.push_back(info);
				_sent_awaiting_ack.push_back(info);

				if (LocalSequenceNumber() >= MaxSequenceNumber())
					_local_sequence_number = 0;
				else
					++_local_sequence_number;

				++_datagrams_sent;

			}
			void ReliabilitySystem::DatagramReceived(sequence_number_type sequence_number, size_t length) {

				// If we've received this datagram before, ignore it.
				if (_seqenceNumberExists(_received, sequence_number))
					return;

				DatagramInfo info;
				info.Age = 0.0f;
				info.Length = length;
				info.SequenceNumber = sequence_number;

				_received.push_back(info);

				if (_sequenceNumberIsMoreRecent(sequence_number, RemoteSequenceNumber()))
					_remote_sequence_number = sequence_number;

				++_datagrams_received;

			}

			void ReliabilitySystem::OnUpdate(float dt) {



			}

			void ReliabilitySystem::Reset() {

				_local_sequence_number = 0;
				_remote_sequence_number = 0;
				_datagrams_sent = 0;

				_sent.clear();
				_sent_awaiting_ack.clear();
				_received.clear();

			}



			bool ReliabilitySystem::_sequenceNumberIsMoreRecent(sequence_number_type s1, sequence_number_type s2) const {

				return ((s1 > s2) && (s1 - s2 <= MaxSequenceNumber())) || ((s1 < s2) && (s2 - s1  > MaxSequenceNumber()));

			}
			bool ReliabilitySystem::_seqenceNumberExists(const std::list<DatagramInfo>& info, sequence_number_type sequence_number) const {

				for (auto i = info.begin(); i != info.end(); ++i)
					if (i->SequenceNumber == sequence_number)
						return true;

				return false;

			}

		}
	}
}