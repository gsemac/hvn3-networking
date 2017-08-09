#include "Exception.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			class SocketException : public System::Exception {

			public:
				using Exception::Exception;

			};

		}
	}
}