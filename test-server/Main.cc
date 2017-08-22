#include "net/Socket.h"
#include "net/Beacon.h"
#include "net/UdpConnection.h"
#include <cassert>
#define PORT_SERVER 25565
using namespace hvn3;
using namespace hvn3::Net;
using namespace hvn3::Net::Sockets;

void UdpSocketSendAndReceiveExample();
void UdpBeaconExample();
void UdpConnectionExample();

int main(int argc, char* argv[]) {

	UdpConnectionExample();

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
void UdpBeaconExample() {

	Socket listener(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp);
	listener.Bind(PORT_SERVER);
	listener.SetBlocking(false);
	listener.SetEnableBroadcast(true);

	Beacon beacon(PORT_ANY, PORT_SERVER);
	beacon.SetMessage("Hello, world!");
	beacon.SetMessageDelay(1000);
	beacon.StartBroadcasting();

	while (1) {
		IPEndPoint sender;
		Byte buf[256];
		if (listener.ReceiveFrom(sender, buf, sizeof(buf)) > 0)
			std::cout << "Received '" << buf << "' from " << sender << '\n';
	}

}
void UdpConnectionExample() {

	UdpConnection server_conn(PORT_SERVER);
	server_conn.Listen();

	assert(server_conn.IsListening());
	std::cout << "Server is listening for connection attempts.\n";

	UdpConnection client_conn(PORT_ANY);
	client_conn.Connect(server_conn.LocalEndPoint());
	std::cout << "Client sent " << client_conn.Send((Byte*)"Hello, Server!", 15) << " bytes to " << client_conn.RemoteEndPoint() << '\n';

	assert(client_conn.IsConnecting());
	std::cout << "Client is attempting to form a connection with the server.\n";

	Byte buf[256];
	if (server_conn.Receive(buf, sizeof(buf)) > 0)
		std::cout << "Server recieved '" << buf << "' from " << server_conn.RemoteEndPoint() << '\n';

	assert(server_conn.IsConnected());
	std::cout << "Server completes connection with the client.\n";

	std::cout << "Server sent " << server_conn.Send((Byte*)"Hello, Client!", 15) << " bytes to " << server_conn.RemoteEndPoint() << '\n';

	if (client_conn.Receive(buf, sizeof(buf)) > 0)
		std::cout << "Client recieved '" << buf << "' from " << client_conn.RemoteEndPoint() << '\n';
	
	assert(client_conn.IsConnected());
	std::cout << "Client completes connection with the server.\n";

} 