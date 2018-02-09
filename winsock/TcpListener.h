#pragma once

#include <string>
#include<sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE (4096)

// Forward declaration of class
class TcpListener;

typedef void(*MessageRecievedHandler)(TcpListener* listener, int socketId, std::string msg);


class TcpListener
{
public:
	TcpListener(std::string ipAddress, int port, MessageRecievedHandler handler);
	~TcpListener();

	// Send a message to the specified client
	void Send(int clientSocket, std::string msg);
	
	// Initialize socket
	bool Init();

	// Main processing loop
	void Run();

private:

	int CreateSocket();
	int WaitForConnection(int listening);

	std::string            m_ipAddress;
	int                    m_port;
	MessageRecievedHandler MessageReceived;



};