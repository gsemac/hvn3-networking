#include "CppUnitTest.h"
#include "net/Socket.h"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace hvn3::Net;
using namespace hvn3::Net::Sockets;

namespace UnitTests {

	TEST_CLASS(SocketTests) {

public:
	// Tests that binding a socket to an OS-assigned port is successful.
	TEST_METHOD(TestBindToOsAssignedPort) {

		Socket socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);

		bool bind_success = socket.Bind(PORT_ANY);

		Assert::IsTrue(bind_success);

	}
	// Tests that getting the port of a socket bound to an OS-assigned port returns the OS-assigned port.
	TEST_METHOD(TestGetOsAssignedPortAfterBind) {

		Socket socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);

		Assert::IsTrue(socket.Bind(PORT_ANY));
		Assert::IsTrue(socket.LocalEndPoint().Port() != PORT_ANY);

	}
	TEST_METHOD(TestSendAndRecieveUdpPacketOnLocalHost) {

		Socket sender(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
		sender.Bind(PORT_ANY);
		sender.SetBlocking(false);

		Socket receiver(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
		receiver.Bind(PORT_ANY);
		receiver.SetBlocking(false);

		const char* msg = "Hello, World!";
		const size_t msg_len = 14;
		sender.SendTo(receiver.LocalEndPoint(), msg, msg_len);

		char buf[msg_len];
		IPEndPoint received_from(0, 0);
		int bytes_received = receiver.ReceiveFrom(received_from, buf, msg_len);

		Assert::AreEqual(14, bytes_received);

	}

	};

}