#include "net/ReliabilitySystem.h"
#include <limits>

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			// Returns true if s2 is newer than s1, taking into account sequence number wrap-around.
			inline bool _sequenceNumberIsGreater(uint16_t s1, uint16_t s2) {
				
				return ((s1 > s2) && (s1 - s2 <= UINT16_MAX)) || ((s1 < s2) && (s2 - s1  > UINT16_MAX));

			}



		}
	}
}