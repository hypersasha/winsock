#include <string>
#include <array>
#include <memory>
#include <type_traits>
#include <iostream>
#include <ctime>
#include <string.h>
#include<sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;
const unsigned int N1 = 3, M1 = 3;
int *a;

int main() {
	a = new int[N1*M1];
	string ipAddress = "127.0.0.1";
	int port = 2000;

	// Create Socket
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		cerr << "Can't create socket" << endl;
		exit(1);
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);

	hint.sin_addr.s_addr = htons(INADDR_ANY);
	if (bind(sock, (sockaddr*)&hint, sizeof(hint)) < 0) {
		cout << "Error binding socket..." << endl;
		exit(1);
	}

	listen(sock, 1);

	// Wait for connections
	sockaddr_in client;
	socklen_t clientSize = sizeof(client);

	int server = accept(sock, (sockaddr*)&client, &clientSize);
	if (server < 0) {
		cout << "Error on accepting client..." << endl;
		exit(1);
	}

	char buffer[4096];
	while (true) {
		int income[3];
		int bytesReceived = recv(server, buffer, sizeof(a), 0);
		memcpy(&a, buffer, sizeof(a));

		cout << "Recieved income:" << bytesReceived << endl;
		//cout << *(a + 0 * M1 + 0) << ", " << *(a + 0 * M1 + 1) << ", " << *(a + 0 * M1 + 2) << endl;
		for (int i = 0; i < N1; ++i) {
			for (int j = 0; j < M1; ++j) {
				*(a + i * M1 + j) = rand() % 10;
				cout << *(a + i * M1 + j) << '\t';
			}
			cout << endl;
		}

		if (bytesReceived < 0) {
			cout << "Error in recv()" << endl;
			break;
		}

		if (bytesReceived == 0) {
			cout << "Client disconnected..." << endl;
			break;
		}

		int arr[3];

		// Echo message back to client
		// send(server, buffer, bytesReceived + 1, 0);
	}

	close(sock);

	return 0;

}