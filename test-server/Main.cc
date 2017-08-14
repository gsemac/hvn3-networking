#include "net/Socket.h"
using namespace hvn3::Net;
using namespace hvn3::Net::Sockets;

int main(int argc, char* argv[]) {

	Socket sender(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	if (!sender.Bind(PORT_ANY))
		std::cout << "failed to bind sender\n";
	if(!sender.SetBlocking(false))
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

	getchar();

	return 0;

}