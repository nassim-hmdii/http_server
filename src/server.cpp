#include <iostream> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>
#include <unistd.h>

int main(){
// Fetching the addrress info thats gonna be used by the socket API 
    struct addrinfo hints{};   //call the stucture a zero it
    addrinfo * servaddrinfo;    // set the pointer to the result linked list
    // Creating the socket and the socket descriptor for futher operations done by the kernel
    int sockfd;
    int bindres;
    struct sockaddr_storage clientaddr;
    socklen_t addrsize = sizeof clientaddr;
    char buffer[1024];
    int recvbytes;
    int sendmsg;
    const char *response = "Hello, First contact w/ the server was successful !";


    // Setting the 3 members values 
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(nullptr, "8080", &hints, &servaddrinfo); // passing the parameters to the function

    if (result != 0){
    std::cerr<< "getaddrinfo error " <<gai_strerror(result) << '\n' ;
    return(1);
    }

    sockfd = socket(servaddrinfo -> ai_family, servaddrinfo -> ai_socktype, servaddrinfo -> ai_protocol);
    if(sockfd == -1){
        std::cerr<< "Socket Error: "<< strerror(errno) << '\n';
        return(1);
    }

    // Binding the socket to the port that was set using the socket descriptor
    bindres = bind(sockfd, servaddrinfo -> ai_addr, servaddrinfo ->ai_addrlen);
    if(bindres == -1){
        std::cerr<< "Bind Error: "<< strerror(errno) << '\n';
        return(1);
    }

    // set the socket to listen for incoming connections
    int listenres = listen(sockfd, 10); //setting the connection queue to 10 connections 
    if (listenres == -1){
        std::cerr<<"Listening Error: " << strerror(errno) << '\n';
        return(1);
    } 

    std::cout << "Listening on port 8080..." << '\n';

    // ACCEPT NEW CONNECTIONS

    while(true){
        int newfd = accept(sockfd, (struct sockaddr*) &clientaddr, &addrsize);
        if(newfd == -1){
            std::cerr << "Accept Error: " << strerror(errno) << '\n';
            continue;
        }
        std::cout << "Got a New Connection !" << '\n';

        // RECV AND SEND, MAKE THE SERVER ECHO BACK THE MESSAGE RECEIVED FROM THE CLIENT
        recvbytes = recv(newfd, buffer, sizeof(buffer) - 1, 0 );

        if(recvbytes == -1){
            std::cerr << "Receive Error: " << strerror(errno) << '\n';
        }

        else if (recvbytes == 0 ){
            std::cout << "Client Closed the Connection !!" << '\n';
        }
        
        else{
            buffer[recvbytes] = '\0';
            std::cout <<"Recieved " << recvbytes << " bytes." << '\n';
            std::cout << "--> " << buffer << '\n';
            sendmsg = send(newfd, response, strlen(response), 0);
            if(sendmsg == -1) {
                std::cerr << "Unsuccessful Response !" << strerror(errno) << '\n';
            }
        }

        close(newfd);
    }

    freeaddrinfo(servaddrinfo);
    return 0;
}
