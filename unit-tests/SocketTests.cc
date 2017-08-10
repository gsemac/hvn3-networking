#include "CppUnitTest.h"
#include "net/Socket.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3::Net::Sockets;

namespace UnitTests {

	TEST_CLASS(SocketTests) {

public:

	TEST_METHOD(TestBindToOSAssignedPort) {

		Socket socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);

		bool bind_success = socket.Bind(PORT_ANY);

		Assert::IsTrue(bind_success);

	}

	};

}