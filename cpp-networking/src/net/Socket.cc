#include "net/Socket.h"
#include "net/SocketException.h"

#ifdef _WIN32
#define OS_WINDOWS
#else
#define OS_UNIX
#endif

#ifdef OS_WINDOWS
#include <winsock2.h>
typedef int socklen_t;
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#endif

namespace hvn3 {

	inline bool initializeSockets() {

#ifdef OS_WINDOWS
		WSADATA WsaData;
		return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
		return true;
#endif

	}
	inline void deinitializeSockets() {

#ifdef OS_WINDOWS
		WSACleanup();
#endif

	}
	inline int getSocketHandle(int address_family, int socket_type, int protcol_type) {

		// Even though a socket handle is 64 bits when compiling for x64, the function returns a kernel 
		// handle which is limited to 32-bits, so it's safe to cast it to a regular integer.
		return static_cast<int>(::socket(address_family, socket_type, protcol_type));

	}
	inline void closeSocket(int handle) {

#ifdef OS_WINDOWS
		closesocket(handle);
#else
		close(handle);
#endif

	}
	bool setBlocking(int handle, bool value) {

#ifdef OS_WINDOWS
		DWORD non_blocking = value ? 0 : 1;
		return ioctlsocket(handle, FIONBIO, &non_blocking) == 0;
#else
		const int flags = fcntl(handle, F_GETFL, 0);

		if ((flags & O_NONBLOCK) && !value)
			return true; // Already non-blocking
		else if (!(flags & O_NONBLOCK) && value)
			return true; // Already blocking

		return fcntl(handle, F_SETFL, value ? flags ^ O_NONBLOCK : flags | O_NONBLOCK) == 0;
#endif

	}
	bool setEnableBroadcast(int handle, bool value) {

		char enable_broadcast = value ? 1 : 0;
		return setsockopt(handle, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, sizeof(enable_broadcast)) == 0;

	}
	bool getEnableBroadcast(int handle) {

		char enable_broadcast;
		int len = sizeof(enable_broadcast);
		if (getsockopt(handle, SOL_SOCKET, SO_BROADCAST, &enable_broadcast, &len) == 0)
			return enable_broadcast > 0;
		return false;

	}
	int getAddressFamily(Net::Sockets::AddressFamily af) {

		using t = Net::Sockets::AddressFamily;

		switch (af) {
		case t::AppleTalk:
			return AF_APPLETALK;
		case t::Atm:
			return AF_ATM;
		case t::Banyan:
			return AF_BAN;
		case t::Ccitt:
			return AF_CCITT;
		case t::Chaos:
			return AF_CHAOS;
		case t::Cluster:
			return AF_CLUSTER;
		case t::DataKit:
			return AF_DATAKIT;
		case t::DataLink:
			return AF_DLI;
		case t::DecNet:
			return AF_DECnet;
		case t::Ecma:
			return AF_ECMA;
		case t::HyperChannel:
			return AF_HYLINK;
		case t::Ieee12844:
			return AF_12844;
		case t::ImpLink:
			return AF_IMPLINK;
		case t::InterNetwork:
			return AF_INET;
		case t::InterNetworkV6:
			return AF_INET6;
		case t::Ipx:
			return AF_IPX;
		case t::Irda:
			return AF_IRDA;
		case t::Iso:
			return AF_ISO;
		case t::Lat:
			return AF_LAT;
		case t::NetBios:
			return AF_NETBIOS;
		case t::NetworkDesigners:
			return AF_NETDES;
		case t::NS:
			return AF_NS;
		case t::Osi:
			return AF_OSI;
		case t::Pup:
			return AF_PUP;
		case t::Sna:
			return AF_SNA;
		case t::Unix:
			return AF_UNIX;
		case t::Unspecified:
			return AF_UNSPEC;
		case t::VoiceView:
			return AF_VOICEVIEW;
		}

		throw Net::Sockets::SocketException("Invalid or unsupported address family.");

	}
	int getSocketType(Net::Sockets::SocketType st) {

		using t = Net::Sockets::SocketType;

		switch (st) {
		case t::Dgram:
			return SOCK_DGRAM;
		case t::Raw:
			return SOCK_RAW;
		case t::Rdm:
			return SOCK_RDM;
		case t::Seqpacket:
			return SOCK_SEQPACKET;
		case t::Stream:
			return SOCK_STREAM;
		}

		throw Net::Sockets::SocketException("Invalid or unsupported socket type.");

	}
	int getProtocolType(Net::Sockets::ProtocolType pt) {

		using t = Net::Sockets::ProtocolType;

		switch (pt) {
		case t::Ggp:
			return IPPROTO_GGP;
		case t::Icmp:
			return IPPROTO_ICMP;
		case t::IcmpV6:
			return IPPROTO_ICMPV6;
		case t::IP:
			return IPPROTO_IP;
		case t::IPv4:
			return IPPROTO_IPV4;
		case t::IPv6:
			return IPPROTO_IPV6;
		case t::ND:
			return IPPROTO_ND;
		case t::Pup:
			return IPPROTO_PUP;
		case t::Raw:
			return IPPROTO_RAW;
		case t::Tcp:
			return IPPROTO_TCP;
		case t::Udp:
			return IPPROTO_UDP;
		}

		throw Net::Sockets::SocketException("Invalid or unsupported protocol type.");

	}
	int getBytesAvailable(int handle) {

		unsigned long bytes_available = 0;

#ifdef OS_WINDOWS
		ioctlsocket(handle, FIONREAD, &bytes_available);
#else
		ioctl(handle, FIONREAD, &bytes_available);
#endif

		return static_cast<int>(bytes_available);

	}

	namespace Net {
		namespace Sockets {

			Socket::Socket(Sockets::AddressFamily address_family, Sockets::SocketType socket_type, Sockets::ProtocolType protocol_type) {

				_initializeMembers();

				_address_family = address_family;
				_socket_type = socket_type;
				_protocol_type = protocol_type;

				initializeSockets();

				_handle = getSocketHandle(getAddressFamily(_address_family), getSocketType(_socket_type), getProtocolType(_protocol_type));

				if (_handle <= 0) {
					_handle = 0;
					throw Net::Sockets::SocketException("Failed to obtain a socket handle from the operating system.");
				}

			}
			Socket::Socket(Socket&& other) {

				_initializeMembers();

				_handle = other._handle;
				_blocking = other._blocking;
				_bound = other._bound;
				_connected = other._connected;
				_local_endpoint = other._local_endpoint;
				_remote_endpoint = other._remote_endpoint;
				_address_family = other._address_family;
				_socket_type = other._socket_type;
				_protocol_type = other._protocol_type;

				// Reset members of the other object (avoids closing socket twice).
				other._initializeMembers();

			}
			Socket::~Socket() {

				Close();

				deinitializeSockets();

			}

			bool Socket::Bind(Port port) {

				return Bind(IPEndPoint(INADDR_ANY, port));

			}
			bool Socket::Bind(const IPAddress& address, Port port) {

				return(Bind(IPEndPoint(address, port)));

			}
			bool Socket::Bind(const IPEndPoint& local_endpoint) {

				if (IsBound())
					throw Net::Sockets::SocketException("Socket is already bound to a local endpoint.");

				if (_handle <= 0) {
					_handle = 0;
					return false;
				}

				sockaddr_in address;
				address.sin_family = getAddressFamily(AddressFamily());
				address.sin_addr.s_addr = htonl(local_endpoint.Address());
				address.sin_port = htons(local_endpoint.Port());

				if (bind(_handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
					Close();
					return false;
				}

				if (local_endpoint.Port() == PORT_ANY) {

					// If bound to an OS-assigned port, get the the port that was assigned.
					socklen_t addr_length = sizeof(address);

					if (getsockname(_handle, (struct sockaddr*)&address, &addr_length) != 0) {
						Close();
						return false;
					}

					_local_endpoint = IPEndPoint(local_endpoint.Address(), ntohs(address.sin_port));

				}
				else
					_local_endpoint = local_endpoint;

				_bound = true;

				return _bound;

			}

			int Socket::Available() const {

				return getBytesAvailable(_handle);

			}
			bool Socket::Blocking() const {

				return _blocking;

			}
			bool Socket::SetBlocking(bool value) {

				if (Handle() <= 0)
					throw Net::Sockets::SocketException("Socket handle has not been obtained.");

				_blocking = setBlocking(Handle(), value);

				return _blocking;

			}
			bool Socket::EnableBroadcast() const {

				if (Handle() <= 0)
					return false;

				return getEnableBroadcast(Handle());

			}
			bool Socket::SetEnableBroadcast(bool value) {

				if (Handle() <= 0)
					throw Net::Sockets::SocketException("Socket handle has not been obtained.");

				return setEnableBroadcast(Handle(), value);

			}
			bool Socket::IsBound() const {

				return _bound;

			}
			bool Socket::Connected() const {

				return _connected;

			}
			int Socket::Handle() const {

				return _handle;

			}

			void Socket::Close() {

				if (_handle > 0)
					closeSocket(_handle);

				_initializeMembers();

			}

			bool Socket::Connect(const IPEndPoint& remote_endpoint) {

				if (!IsBound() && !Bind(PORT_ANY))
					return false;

				sockaddr_in address;
				address.sin_family = getAddressFamily(AddressFamily());
				address.sin_addr.s_addr = htonl(remote_endpoint.Address());
				address.sin_port = htons(remote_endpoint.Port());

				if (connect(Handle(), (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
					return false;

				_remote_endpoint = remote_endpoint;
				_connected = true;

				return _connected;

			}
			bool Socket::Listen(int backlog) {

				if (!IsBound() && !Bind(PORT_ANY))
					return false;

				return listen(Handle(), backlog) == 0;

			}
			Socket Socket::Accept() {

				sockaddr_in from;
				socklen_t from_length = sizeof(from);

				int accepted_handle = static_cast<int>(accept(Handle(), (sockaddr*)&from, &from_length));

				if (accepted_handle != 0)
					throw SocketException("Error occurred when attempting to accept new socket connection.");

				unsigned int address = ntohl(from.sin_addr.s_addr);
				unsigned int port = ntohs(from.sin_port);

				Socket new_socket;
				new_socket._address_family = AddressFamily();
				new_socket._socket_type = SocketType();
				new_socket._protocol_type = ProtocolType();
				new_socket._handle = accepted_handle;
				new_socket.SetBlocking(Blocking());
				new_socket._bound = true;
				new_socket._connected = true;
				new_socket._local_endpoint = LocalEndPoint();
				new_socket._remote_endpoint = IPEndPoint(address, port);

				return new_socket;

			}

			int Socket::SendTo(const IPEndPoint& destination, const void* buffer, int length) {

				if (buffer == nullptr || length <= 0)
					return 0;

				if (!IsBound() && !Bind(PORT_ANY))
					return 0;

				// If the destination IP is 0.0.0.0, send the data to localhost.
				unsigned int destination_address = destination.Address();
				if (destination_address == 0)
					destination_address = IPAddress::LocalHost().Address();

				sockaddr_in address;
				address.sin_family = getAddressFamily(AddressFamily());
				address.sin_addr.s_addr = htonl(destination_address);
				address.sin_port = htons(destination.Port());

				int bytes_sent = sendto(Handle(), (const char*)buffer, length, 0, (sockaddr*)&address, sizeof(sockaddr_in));

				return bytes_sent < 0 ? 0 : bytes_sent;

			}
			int Socket::ReceiveFrom(IPEndPoint& sender, void* buffer, int length) const {

				if (buffer == nullptr || length <= 0)
					return 0;

				if (Handle() <= 0)
					throw Net::Sockets::SocketException("Socket has not yet been bound to a local endpoint.");

				sockaddr_in from;
				socklen_t from_length = sizeof(from);

				int bytes_recieved = recvfrom(Handle(), (char*)buffer, length, 0, (sockaddr*)&from, &from_length);

				if (bytes_recieved <= 0)
					return 0;

				unsigned int address = ntohl(from.sin_addr.s_addr);
				unsigned int port = ntohs(from.sin_port);
				sender = IPEndPoint(address, port);

				return bytes_recieved;

			}

			AddressFamily Socket::AddressFamily() const {

				return _address_family;

			}
			SocketType Socket::SocketType() const {

				return _socket_type;

			}
			ProtocolType Socket::ProtocolType() const {

				return _protocol_type;

			}
			const IPEndPoint& Socket::LocalEndPoint() const {

				return _local_endpoint;

			}


			void Socket::_initializeMembers() {

				_handle = 0;
				_blocking = true;
				_bound = false;
				_connected = false;

			}


			Socket::Socket() {

				_initializeMembers();

				initializeSockets();

			}

		}
	}
}