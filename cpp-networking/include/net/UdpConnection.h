#pragma once
#include "net/Socket.h"
#include <cstddef>

namespace hvn3 {

	typedef uint8_t Byte;

	namespace Net {
		namespace Sockets {

			class UdpConnection {

				enum CONNECTION_STATE {
					IDLE,
					LISTENING,
					CONNECTING,
					CONNECTED,
					TIMED_OUT
				};

				enum CONNECTION_MODE {
					NONE,
					SERVER,
					CLIENT
				};

			public:
				UdpConnection(const IPEndPoint& local_endpoint);
				UdpConnection(Port local_port);
				virtual ~UdpConnection() = default;

				void Listen();
				void Connect(const IPEndPoint& remote_endpoint);

				virtual size_t Send(Byte data[], size_t length);
				virtual size_t Receive(Byte data[], size_t length);

				float Timeout() const;
				void SetTimeout(float value);
				bool IsListening() const;
				bool IsConnecting() const;
				bool IsConnected() const;
				bool ConnectionTimedOut() const;
				const IPEndPoint& LocalEndPoint() const;
				const IPEndPoint& RemoteEndPoint() const;

				virtual void OnUpdate(float dt);

			protected:
				virtual void OnConnect();
				virtual void OnDisconnect();
				virtual void OnReset();
				// Handles a received datagram (accepts connection attempt, etc.). Returns true if the datagram is accepted, and false if it is rejected.
				virtual bool HandleReceivedDatagram(const IPEndPoint& sender, Byte data[], size_t length);

				CONNECTION_STATE State() const;
				CONNECTION_MODE Mode() const;


			private:
				float _timeout;
				float _timeout_accumulator;
				Socket _socket;
				CONNECTION_STATE _state;
				CONNECTION_MODE _mode;
				IPEndPoint _remote_endpoint;

				void _disconnect();
				void _resetData();

			};

		}
	}
}