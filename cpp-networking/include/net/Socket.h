#pragma once
#include "net/IPEndPoint.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			enum class AddressFamily {
				AppleTalk,
				Atm,
				Banyan,
				Ccitt,
				Chaos,
				Cluster,
				DataKit,
				DataLink,
				DecNet,
				Ecma,
				HyperChannel,
				Ieee12844,
				ImpLink,
				InterNetwork,
				InterNetworkV6,
				Ipx,
				Irda,
				Iso,
				Lat,
				NetBios,
				NetworkDesigners,
				NS,
				Osi,
				Pup,
				Sna,
				Unix,
				Unspecified,
				VoiceView
			};

			enum class SocketType {
				Dgram,
				Raw,
				Rdm,
				Seqpacket,
				Stream
			};

			enum class ProtocolType {
				Ggp,
				Icmp,
				IcmpV6,
				Igmp,
				IP,
				IPSecAuthenticationHeader,
				IPSecEncapsulatingSecurityPayload,
				IPv4,
				IPv6,
				IPv6DestinationOptions,
				IPv6FragmentHeader,
				IPv6HopByHopOptions,
				IPv6NoNextHeader,
				IPv6RoutingHeader,
				Ipx,
				ND,
				Pup,
				Raw,
				Spx,
				Spxll,
				Tcp,
				Udp,
				Unspecified
			};

			enum class SocketOptionLevel {
				IP,
				IPv6,
				Socket,
				Tcp,
				Udp
			};

			enum class SocketOptionName {
				AcceptConnection,
				AddMembership,
				AddSourceMembership,
				BlockSource,
				Broadcast,
				BsdUrgent,
				ChecksumCoverage,
				Debug,
				DontFragment,
				DontLinger,
				DontRoute,
				DropMembership,
				DropSourceMembership,
				Error,
				ExclusiveAddressUse,
				Expedited,
				HeaderIncluded,
				HopLimit,
				IpOptions,
				IpProtectionLevel,
				IpTimeToLive,
				IPv6Only,
				KeepAlive,
				Linger,
				MaxConnections,
				MulticastInterface,
				MulticastLoopback,
				MulticastTimeToLive,
				NoChecksum,
				NoDelay,
				OutOfBandInline,
				PacketInformation,
				ReceiveBuffer,
				ReceiveLowWater,
				ReceiveTimeout,
				ReuseAddress,
				ReuseUnicastPort,
				SendBuffer,
				SendLowWater,
				SendTimeout,
				Type,
				TypeOfService,
				UnblockSource,
				UpdateAcceptContext,
				UpdateConnectContext,
				UseLoopback
			};

			enum : unsigned short {
				PORT_ANY = 0U
			};

			class Socket {

			public:
				Socket(AddressFamily address_family, SocketType socket_type, ProtocolType protocol_type);
				//Socket(SocketType socket_type, ProtocolType protocol_type);
				Socket(Socket&& other);
				~Socket();

				bool Bind(unsigned short port);
				bool Bind(const IPEndPoint& local_endpoint);

				int Available() const;
				bool Blocking() const;
				bool SetBlocking(bool value);
				bool IsBound() const;
				bool Connected() const;
				int Handle() const;

				void Close();

				bool Connect(const IPEndPoint& remote_endpoint);
				bool Listen(int backlog);
				Socket Accept();

				int SendTo(const IPEndPoint& destination, const void* buffer, int length);
				int ReceiveFrom(IPEndPoint& sender, void* buffer, int length) const;

				AddressFamily AddressFamily() const;
				SocketType SocketType() const;
				ProtocolType ProtocolType() const;
				const IPEndPoint& LocalEndPoint() const;

			protected:
				void _initializeMembers();

			private:
				Socket();

				int _handle;
				bool _blocking;
				bool _bound;
				bool _connected;
				Sockets::AddressFamily _address_family;
				Sockets::SocketType _socket_type;
				Sockets::ProtocolType _protocol_type;
				IPEndPoint _local_endpoint;
				IPEndPoint _remote_endpoint;

			};

		}
	}
}