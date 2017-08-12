#include "net/Beacon.h"
#include <cstring>

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			Beacon::Beacon(unsigned short beacon_port, unsigned short listener_port) :
				_socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp) {
			
				_buf = nullptr;
				_brocasting = false;

			}
			Beacon::~Beacon() {

				if (_buf != nullptr)
					delete[] _buf;

				_buf = nullptr;
				_buffer_length = 0;
				
			}

			void Beacon::SetMessage(void* buffer, size_t buffer_length) {

				_buf = new char[buffer_length];
				_buffer_length = buffer_length;
				
				memcpy(_buf, buffer, buffer_length);

			}

			void Beacon::Start() {

				_brocasting = true;

			}
			void Beacon::Stop() {

				_brocasting = false;

			}
			void Beacon::Update(float dt) {

				if (!_brocasting)
					return;



			}
			
			bool Beacon::Broadcasting() const {

				return _brocasting;

			}

		};

	}
}