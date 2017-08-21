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

				size_t Send(Byte data[], size_t length);
				size_t Receive(Byte data[], size_t length);

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