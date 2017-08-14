#include "net/Socket.h"
#include "net/Beacon.h"
using namespace hvn3;
using namespace hvn3::Net;
using namespace hvn3::Net::Sockets;

void UdpSocketSendAndReceiveExample();

int main(int argc, char* argv[]) {

	Socket listener(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	listener.Bind(25565);
	listener.SetBlocking(false);
	listener.SetEnableBroadcast(true);

	Beacon beacon(PORT_ANY, 25565);
	beacon.SetMessage("Hello, world!");
	beacon.SetMessageDelay(1000);
	beacon.StartBroadcasting();

	while (1) {
		IPEndPoint sender;
		Byte buf[256];
		if (listener.ReceiveFrom(sender, buf, sizeof(buf)) > 0)
			std::cout << "Received '" << buf << "' from " << sender << '\n';
	}

	getchar();

	return 0;

}

void UdpSocketSendAndReceiveExample() {

	Socket sender(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	if (!sender.Bind(PORT_ANY))
		std::cout << "failed to bind sender\n";
	if (!sender.SetBlocking(false))
		std::cout << "failed to make sender non-blocking\n";

	Socket receiver(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	if (!receiver.Bind(PORT_ANY))
		std::cout << "failed to bind receiver\n";
	receiver.SetBlocking(false);

	const char* msg = "Hello, World!";
	const size_t msg_len = 14;
	int bytes_sent = sender.SendTo(receiver.LocalEndPoint(), msg, msg_len);

	std::cout << "Bytes sent: " << bytes_sent << " bytes to " << receiver.LocalEndPoint() << '\n';

	char buf[msg_len];
	IPEndPoint received_from(0, 0);
	int bytes_received = receiver.ReceiveFrom(received_from, buf, msg_len);

	std::cout << "Bytes received: " << bytes_received << " bytes from " << received_from << '\n';
	std::cout << "Message received: " << buf << '\n';

}