#pragma once
#include "net/Socket.h"
#include <thread>
#include <mutex>
#include <chrono>

// #todo finish implementing this class when more aware of requirements

namespace hvn3 {

	typedef uint8_t Byte;

	namespace Net {
		namespace Sockets {

			class Beacon {

			public:
				Beacon(Port beacon_port, Port broadcast_port);
				~Beacon();

				void SetMessage(const char* message);
				void SetMessage(Byte data[], size_t length);

				void StartBroadcasting();
				void StopBroadcasting();
				bool IsBroadcasting() const;

				unsigned long MessageDelay() const;
				void SetMessageDelay(unsigned long value);

			protected:
				void BroadcastLoop();

			private:
				Socket _socket;
				Byte* _message_buffer;
				size_t _message_buffer_length;
				bool _brocasting;
				Port _beacon_port;
				Port _broadcast_port;

				std::thread _broadcast_thread;
				std::mutex _mutex;
				std::chrono::time_point<std::chrono::steady_clock> _last_broadcast;
				unsigned long _message_delay;

			};

		}
	}
}