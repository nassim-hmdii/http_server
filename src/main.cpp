#include <iostream> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

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
    std::cout << "Address family: " << servaddrinfo->ai_family << '\n'; // if the function successful, print the address family of the first result in the linked list








    freeaddrinfo(servaddrinfo);
    return 0;
}
