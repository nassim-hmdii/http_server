#include <iostream> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>

int main(){
// Fetching the addrress info thats gonna be used by the socket API 
    struct addrinfo hints{};   //call the stucture a zero it
    addrinfo * servaddrinfo;    // set the pointer to the result linked list
    // Setting the 3 members values 
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int result = getaddrinfo(nullptr, "8080", &hints, &servaddrinfo); // passing the parameters to the function

    if (result != 0){
    std::cerr<< "getaddrinfo error " <<gai_strerror(result) << '\n' ;
    return(1);
    }
    // Creating the socket and the socket descriptor for futher operations done by the kernel
    int sockfd;
    int bindres;

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

    std::cout << "Socket created and binded successfully" << '\n';
    freeaddrinfo(servaddrinfo);
    return 0;
}
