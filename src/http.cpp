#include "http.h"
#include <string>
#include <fstream>
#include <sstream>

std::string buildResponse(const std::string &method, const std::string &path ){
            if(method != "GET"){
                std::ifstream file405("../public/405-MethodNotAllowed.html");
                    if (!file405) {
                    std::string body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
                    std::string http_response =
                        "HTTP/1.1 500 Internal Server Error\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: " + std::to_string(body.length()) + "\r\n"
                        "\r\n" +
                        body;
                         return http_response;
                    }
                    else{
                        std::stringstream buffer405;
                        buffer405 << file405.rdbuf();
                        std::string file405Contents = buffer405.str();
                        std::string http_response =
                            "HTTP/1.1 405 Method Not Allowed\r\n"
                            "Content-Type: text/html\r\n"
                            "Content-Length: " + std::to_string(file405Contents.length()) + "\r\n"
                            "\r\n" + 
                            file405Contents;
                        return http_response;}
            }

            else if(path == "/"){
                std::ifstream file200("../public/200-OK.html");
                    if (!file200) {
                    std::string body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
                    std::string http_response =
                        "HTTP/1.1 500 Internal Server Error\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: " + std::to_string(body.length()) + "\r\n"
                        "\r\n" +
                        body;
                     return http_response;
                    }
                    else{ 
                    std::stringstream buffer200;
                    buffer200 << file200.rdbuf();
                    std::string file200Contents = buffer200.str();
                    std::string http_response =
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: " + std::to_string(file200Contents.length()) + "\r\n"
                        "\r\n" + 
                         file200Contents;
                    return http_response; }
            }

            else{
            std::ifstream file404("../public/404-NotFound.html");
                if (!file404) {
                    std::string body = "<html><body><h1>500 Internal Server Error</h1></body></html>";
                    std::string http_response =
                        "HTTP/1.1 500 Internal Server Error\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: " + std::to_string(body.length()) + "\r\n"
                        "\r\n" +
                        body;
                    return http_response;
                    }
                    else{
                    std::stringstream buffer404;
                    buffer404 << file404.rdbuf();
                    std::string file404Contents = buffer404.str();
                    std::string http_response =
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/html\r\n"
                        "Content-Length: " + std::to_string(file404Contents.length()) + "\r\n"
                        "\r\n" + 
                        file404Contents;
                    return http_response; }               
            }

}