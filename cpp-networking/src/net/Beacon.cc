#include "net/Beacon.h"
#include "net/SocketException.h"
#include <cstring>


namespace hvn3 {
	namespace Net {
		namespace Sockets {

			Beacon::Beacon(Port beacon_port, Port broadcast_port) :
				_socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp) {

				_message_buffer = nullptr;
				_message_buffer_length = 0;
				_brocasting = false;
				_beacon_port = beacon_port;
				_broadcast_port = broadcast_port;

				_message_delay = 1000;

				if(!_socket.Bind(_beacon_port))
					throw SocketException("Failed to bind socket.");

			}
			Beacon::~Beacon() {

				if (_message_buffer != nullptr)
					delete[] _message_buffer;

				_message_buffer = nullptr;
				_message_buffer_length = 0;

			}

			void Beacon::SetMessage(Byte data[], size_t length) {

				std::lock_guard<std::mutex> guard(_mutex);

				_message_buffer = new Byte[length];
				_message_buffer_length = length;

				memcpy(data, _message_buffer, length);

			}

			void Beacon::StartBroadcasting() {

				if (_brocasting)
					return;
				
				_last_broadcast = std::chrono::steady_clock::now() + std::chrono::milliseconds(_message_delay);
				_broadcast_thread = std::thread(&BroadcastLoop, this);
				_brocasting = true;

			}
			void Beacon::StopBroadcasting() {

				if (!_brocasting)
					return;

				_brocasting = false;
				
				_broadcast_thread.join();

			}
			bool Beacon::IsBroadcasting() const {

				return _brocasting;

			}

			unsigned long Beacon::MessageDelay() const {

				return _message_delay;

			}
			void Beacon::SetMessageDelay(unsigned long value) {

				std::lock_guard<std::mutex> guard(_mutex);

				_message_delay = value;

			}


			void Beacon::BroadcastLoop() {

				while (IsBroadcasting()) {

					std::lock_guard<std::mutex> guard(_mutex);

					if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _last_broadcast).count() < _message_delay)
						continue;					

					if (_socket.SendTo(IPEndPoint(IPAddress::Broadcast(), _beacon_port), _message_buffer, _message_buffer_length) <= 0)
						throw SocketException("Failed to send broadcast packet.");

					std::cout << "Broadcasted message...\n";

				}

			}

		};

	}
}