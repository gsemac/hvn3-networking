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

				if (!_socket.Bind(_beacon_port))
					throw SocketException("Failed to bind socket.");

				if (!_socket.SetEnableBroadcast(true))
					throw SocketException("Failed to enable broadcast.");

			}
			Beacon::~Beacon() {

				if (IsBroadcasting())
					StopBroadcasting();

				if (_message_buffer != nullptr)
					delete[] _message_buffer;
				_message_buffer = nullptr;
				_message_buffer_length = 0;

			}

			void Beacon::SetMessage(const char* message) {

				size_t length = strlen(message) + 1;

				SetMessage((Byte*)message, length);

			}
			void Beacon::SetMessage(Byte data[], size_t length) {

				std::lock_guard<std::mutex> guard(_mutex);

				_message_buffer = new Byte[length];
				_message_buffer_length = length;

				memcpy(_message_buffer, data, length);

			}

			void Beacon::StartBroadcasting() {

				if (IsBroadcasting())
					return;

				_brocasting = true;
				_last_broadcast = std::chrono::steady_clock::now() - std::chrono::milliseconds(_message_delay); // Send first message immediately
				_broadcast_thread = std::thread(&Beacon::BroadcastLoop, this);

			}
			void Beacon::StopBroadcasting() {

				if (!IsBroadcasting())
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

				std::lock_guard<std::mutex> guard(_mutex);

				while (IsBroadcasting()) {				

					if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - _last_broadcast).count() < _message_delay)
						continue;

					_last_broadcast = std::chrono::steady_clock::now();

					if (_message_buffer_length == 0)
						continue;

					_socket.SendTo(IPEndPoint(IPAddress::Broadcast(), _broadcast_port), _message_buffer, _message_buffer_length);

				}

			}

		};

	}
}