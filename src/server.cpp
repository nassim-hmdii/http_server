#include <iostream> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <errno.h>
#include <unistd.h>
#include <string>

int main(){
// Fetching the addrress info thats gonna be used by the socket API 
    struct addrinfo hints{};   //call the stucture a zero it
    addrinfo * servaddrinfo;    // set the pointer to the result linked list
    // Creating the socket and the socket descriptor for futher operations done by the kernel
    int sockfd;
    int bindres;
    struct sockaddr_storage clientaddr;
    socklen_t addrsize = sizeof clientaddr;
    char buffer[4096];
    int recvbytes;
    int sendmsg;

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
    freeaddrinfo(servaddrinfo);
    
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

            std::string request(buffer);
            size_t pos = request.find('\n');
            std::string first_line = request.substr(0, pos);
            std::cout << "First line is: " << first_line << '\n';

            size_t m_pos = first_line.find(' ');
            size_t p_pos = first_line.find(' ', m_pos + 1);
            std::string method = first_line.substr(0, m_pos);
            std::string path = first_line.substr(m_pos + 1, p_pos - m_pos - 1);
            std::string version = first_line.substr(p_pos + 1); 
            // the last 5 lines are used to parse the http response and get the method, path and version of the request 
            // using find and substr methods of the string class  

            std::cout << "method: [" << method << "]\n";
            std::cout << "path: [" << path << "]\n";
            std::cout << "version: [" << version << "]\n";

            //Build a response that the server can actually understand
            std::string body = "<html><body><h1>Hello, First contact with the server was successful !</h1></body></html>";
            std::string http_response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: " + std::to_string(body.length()) + "\r\n"
                "\r\n" + 
                body;

            sendmsg = send(newfd, http_response.c_str(), http_response.length(), 0);
            if(sendmsg == -1) {
                std::cerr << "Unsuccessful Response !" << strerror(errno) << '\n';
            }
        }

        close(newfd);   
    }   

    return 0;
}
