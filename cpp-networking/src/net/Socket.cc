#include "net/Socket.h"
#include "net/SocketException.h"

#ifdef _WIN32
#define OS_WINDOWS
#else
#define OS_UNIX
#endif

#ifdef OS_WINDOWS
#include <winsock2.h>
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

	namespace Net {
		namespace Sockets {

			Socket::Socket(Sockets::AddressFamily address_family, Sockets::SocketType socket_type, Sockets::ProtocolType protocol_type) :
				_local_endpoint(0, 0) {

				_handle = 0;
				_blocking = true;
				_bound = false;
				_address_family = address_family;
				_socket_type = socket_type;
				_protocol_type = protocol_type;

				initializeSockets();

			}
			Socket::~Socket() {

				Close();

				deinitializeSockets();

			}

			bool Socket::Bind(unsigned short port) {

				Bind(IPEndPoint(INADDR_ANY, port));

			}
			bool Socket::Bind(const IPEndPoint& local_endpoint) {

				if (IsBound())
					throw Net::Sockets::SocketException("Socket is already bound.");

				_handle = getSocketHandle(getAddressFamily(_address_family), getSocketType(_socket_type), getProtocolType(_protocol_type));

				if (_handle <= 0) {
					_handle = 0;
					return false;
				}

				sockaddr_in address;
				address.sin_family = AF_INET;
				address.sin_addr.s_addr = local_endpoint.IPAddress().Address();
				address.sin_port = htons(local_endpoint.Port());

				if (bind(_handle, (const sockaddr*)&address, sizeof(sockaddr_in)) < 0) {
					Close();
					return false;
				}

				_bound = true;
				_local_endpoint = local_endpoint;

				return _bound;

			}

			bool Socket::Blocking() const {

				return _blocking;

			}
			bool Socket::SetBlocking(bool value) {

				if (Handle() <= 0)
					throw Net::Sockets::SocketException("Socket has not yet been bound to a local endpoint.");

				_blocking = setBlocking(_handle, value);

				return _blocking;

			}
			bool Socket::IsBound() const {

				return _bound;

			}
			int Socket::Handle() const {

				return _handle;

			}

			void Socket::Close() {

				if (_handle > 0)
					closeSocket(_handle);

				_handle = 0;
				_bound = false;
				_blocking = false;

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

		}
	}
}