#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <unistd.h>

#define DEFAULT_PORT 27080

int main(int argc, char *argv[]) {
  
  int usedPort{};
  
  if (argc > 1 && atoi(argv[1]) != 0) {
    usedPort = atoi(argv[1]);
    std::cout << "Using port: " << usedPort << '\n'; 
  } else {
    usedPort = DEFAULT_PORT;
    std::cout << "Using the default port: " << DEFAULT_PORT << '\n';
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    std::cout << "Error has occured while creating a socket...\n";
    return 1;
  } else {
    std::cout << "Successfully created a socked!\n";
  }

  sockaddr_in sockaddr;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(usedPort);

  int bindSock = bind(sock, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
  if (bindSock < 0) {
    std::cout << "Error has occured while binding to the socket...\n";
    return 1;
  } else {
    std::cout << "Successfully binded to the socked!\n";
  }
  
  if (listen(sock, 10) < 0) {
    std::cout << "Failed to listen the socket...\n";
    return 1;
  }
  
  std::string requestString{};
  int ret;
  unsigned int addrlen;
  char buffer[512];
  for (;;) {
    addrlen = sizeof(sockaddr);
    int connection = accept(sock, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if (connection < 0) {
      std::cout << "Failed to get a conection...\n";
      return 1;
    }
    read(connection, buffer, 512);
    //buffer[ret] = 0x00;
    //requestString += buffer;
    //std::cout << "Request received: \n" << requestString << '\n';
    sleep(1);
    std::string response = "\nHTTP/1.x 200 OK\n"
                          "Content-Length: 12\n"
                          "Content-Type: text/html; charset=UTF-8\n"
                          "Connection: Closed\n"
                          "Server: flibserver/0.0.1\r\n\r\n"
                          "Hello World!";


    ret = send(connection, response.c_str(), response.size(), 0);
    if (ret > 0) {
      std::cout << "Sent a response " << ret << ":\n" << response.c_str() << '\n';
    }
  
    close(connection);

  }
  
  close(sock);
  
  return 0;
}
