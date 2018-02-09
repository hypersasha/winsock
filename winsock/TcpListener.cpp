#include "TcpListener.h"

TcpListener::TcpListener(std::string ipAddress, int port, MessageRecievedHandler handler)
	: m_ipAddress(ipAddress), m_port(port), MessageReceived(handler)
{

}

TcpListener::~TcpListener()
{
	CleanUp();
}

// Send a message to the specified client
void TcpListener::Send(int clientSocket, std::string msg)
{
	send(clientSocket, msg.c_str(), msg.size() + 1, 0);
}

// Initialize socket
bool TcpListener::Init()
{
	return true;
}

// Main processing loop
void TcpListener::Run()
{
	char buf[MAX_BUFFER_SIZE];

	while (true)
	{
		// Create a listening socket
		int listening = CreateSocket();
		if (listening < 0) {
			break;
		}

		// Wait for connection
		int client = WaitForConnection(listening);
		if (client < 0) {
			close(listening);

			// Loop receive/ send
			int bytesReceived = 0;
			do {
				bytesReceived = recv(client, buf, MAX_BUFFER_SIZE, 0);
				if (bytesReceived > 0) {
					if (MessageReceived != NULL)
					{
						MessageReceived(this, client, std::string(buf, 0, bytesReceived));
					}
				}
			} while (bytesReceived > 0);
			close(client);
		}
	}
}

// Create Socket
int TcpListener::CreateSocket()
{
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening < 0) {
		exit(1);
	}
	else {
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(m_port);
		hint.sin_addr.s_addr = htons(INADDR_ANY);

		// Binding socket
		int bindOk = bind(listening, (sockaddr*)&hint, sizeof(hint));
		if (bindOk > 0) {
			int listenOk = listen(listening, SOMAXCONN);
			if (listenOk < 0) {
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	return listening;
}

// Wait for connections
int TcpListener::WaitForConnection(int listening)
{
	int client = accept(listening, NULL, NULL);
	return client;
}