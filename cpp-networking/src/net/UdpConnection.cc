#include "net/UdpConnection.h"

namespace hvn3 {
	namespace Net {
		namespace Sockets {

			UdpConnection::UdpConnection(const IPEndPoint& local_endpoint) :
				_socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::Udp) {

				_state = IDLE;
				_mode = NONE;
				_timeout = 1.0f;
				_timeout_accumulator = 0.0f;

				_socket.Bind(local_endpoint);

			}
			UdpConnection::UdpConnection(Port local_port) :
				UdpConnection(IPEndPoint(IPAddress::LocalHost(), local_port)) {
			}

			void UdpConnection::Listen() {

				_disconnect();

				_mode = SERVER;
				_state = LISTENING;

			}
			void UdpConnection::Connect(const IPEndPoint& remote_endpoint) {

				_disconnect();

				_mode = CLIENT;
				_state = CONNECTING;
				_remote_endpoint = remote_endpoint;

			}

			size_t UdpConnection::Send(Byte data[], size_t length) {

				// If there is no remote endpoint, we cannot send any data.
				if (_remote_endpoint.Address() == 0)
					return 0;

				return _socket.SendTo(_remote_endpoint, data, length);

			}
			size_t UdpConnection::Receive(Byte data[], size_t length) {

				IPEndPoint sender;
				int bytes_received = _socket.ReceiveFrom(sender, data, length);

				if (bytes_received == 0)
					return 0;

				// Only return the number of bytes if the datagram was sent from the remote endpoint. Otherwise, the datagram is ignored.
				if (HandleDatagram(sender, data, bytes_received))
					return bytes_received;

				return 0;

			}

			float UdpConnection::Timeout() const {

				return _timeout;

			}
			void UdpConnection::SetTimeout(float value) {

				_timeout = value;

			}
			bool UdpConnection::IsListening() const {

				return _state == LISTENING;

			}
			bool UdpConnection::IsConnecting() const {

				return _state == CONNECTING;

			}
			bool UdpConnection::IsConnected() const {

				return _state == CONNECTED;

			}
			bool UdpConnection::ConnectionTimedOut() const {

				return _state == TIMED_OUT;

			}
			const IPEndPoint& UdpConnection::LocalEndPoint() const {

				return _socket.LocalEndPoint();

			}
			const IPEndPoint& UdpConnection::RemoteEndPoint() const {

				return _remote_endpoint;

			}

			void UdpConnection::OnUpdate(float dt) {

				if (_remote_endpoint == 0)
					return;

				// If the timeout is set to 0, the connection should never time out.
				if (_timeout == 0.0f) {
					_timeout_accumulator = 0.0f;
					return;
				}				

				_timeout_accumulator += dt;
				if (_timeout_accumulator > _timeout && _state == CONNECTING || _state == CONNECTED) {

					_resetData();
					_state = TIMED_OUT;
					OnDisconnect();

				}

			}



			void UdpConnection::OnConnect() {
			}
			void UdpConnection::OnDisconnect() {
			}
			bool UdpConnection::HandleDatagram(const IPEndPoint& sender, Byte data[], size_t length) {

				if (_mode == SERVER && IsListening()) {

					_state = CONNECTED;
					_remote_endpoint = sender;
					OnConnect();

				}

				if (sender == _remote_endpoint) {

					if (_mode == CLIENT && IsConnecting()) {

						_state = CONNECTED;
						OnConnect();

					}

					_timeout_accumulator = 0.0f;

					return true;

				}

				return false;

			}

			UdpConnection::CONNECTION_STATE UdpConnection::State() const {

				return _state;

			}
			UdpConnection::CONNECTION_MODE UdpConnection::Mode() const {

				return _mode;

			}



			void UdpConnection::_disconnect() {

				bool was_connected = IsConnected();

				_resetData();

				if (was_connected)
					OnDisconnect();

			}
			void UdpConnection::_resetData() {

				_timeout_accumulator = 0.0f;
				_state = IDLE;
				_mode = NONE;
				_remote_endpoint = IPEndPoint(0, 0);

			}

		}
	}
}