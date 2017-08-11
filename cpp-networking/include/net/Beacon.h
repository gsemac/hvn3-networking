#include "net/Socket.h"

// #todo finish implementing this class when more aware of requirements

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class Beacon {

			public:
				Beacon(unsigned short beacon_port, unsigned short listener_port);
				~Beacon();

				void SetMessage(void* buffer, size_t buffer_length);
				
				void Start();
				void Stop();
				void Update(float dt);

				bool Broadcasting() const;

			private:
				char* _buf;
				size_t _buffer_length;
				Socket _socket;
				bool _brocasting;

			};

		}
	}
}