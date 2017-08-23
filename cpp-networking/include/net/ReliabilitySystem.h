#pragma once
#include <cstdint>

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class ReliabilitySystem {

			public:
				ReliabilitySystem();

			private:
				uint16_t _local_sequence_number;
				uint16_t _remote_sequence_number;
				
			};

		}
	}
}