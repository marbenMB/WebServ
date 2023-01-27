#include "../../include/method.hpp"

// ! GET :
/**
* * 400 Bad Request: The request was invalid or cannot be fulfilled by the server.
* ? 401 Unauthorized: The request requires user authentication.
* ? 403 Forbidden: The server understands the request, but refuses to authorize it.
* ? 404 Not Found: The requested resource could not be found on the server.
* ! 408 Request Timeout: The server did not receive a complete request from the client within a certain period of time.
* ? 500 Internal Server Error: An error occurred on the server while processing the request.
* * 503 Service Unavailable: The server is currently unable to handle the request due to a temporary overload or maintenance.
* * 504 Gateway Timeout: The server did not receive a timely response from an upstream server while processing the request.
*/
void method::error(int statusCode, std::string reason_phrase)
{
    this->setStatuscode(statusCode);
    this->setreason_phrase(reason_phrase);
}
