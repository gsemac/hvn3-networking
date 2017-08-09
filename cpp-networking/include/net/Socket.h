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

			class Socket {

			public:
				Socket(AddressFamily address_family, SocketType socket_type, ProtocolType protocol_type);
				//Socket(SocketType socket_type, ProtocolType protocol_type);
				~Socket();

				bool Bind(unsigned short port);
				bool Bind(const IPEndPoint& local_endpoint);

				bool Blocking() const;
				bool SetBlocking(bool value);
				bool IsBound() const;

				void Close();

				AddressFamily AddressFamily() const;
				SocketType SocketType() const;
				ProtocolType ProtocolType() const;

			private:
				int _handle;
				bool _blocking;
				bool _bound;
				Sockets::AddressFamily _address_family;
				Sockets::SocketType _socket_type;
				Sockets::ProtocolType _protocol_type;

			};

		}
	}
}