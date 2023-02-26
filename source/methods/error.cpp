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

/**
 *
 *
 *
 * TODO
 4xx Client Error

* * 400 Bad Request: The request cannot be fulfilled due to bad syntax.
401 Unauthorized: The request requires user authentication.
402 Payment Required: Reserved for future use.
* * 403 Forbidden: The server has refused to fulfill the request.
* * 404 Not Found: The server cannot find the requested resource.
* * 405 Method Not Allowed: The requested HTTP method is not supported for the requested resource.
* ? 406 Not Acceptable: The requested resource is not available in a format that would be acceptable to the client.
407 Proxy Authentication Required: The client must first authenticate itself with the proxy.
* * 408 Request Timeout: The server timed out waiting for the request.
409 Conflict: The request could not be completed due to a conflict with the current state of the resource.
410 Gone: The requested resource is no longer available.
* *411 Length Required: The request did not specify the length of its content, which is required by the requested resource.
412 Precondition Failed: The server does not meet one of the preconditions that the client put on the request.
413 Payload Too Large: The request is larger than the server is willing or able to process.
* *414 URI Too Long: The requested URI is longer than the server is willing or able to process.
415 Unsupported Media Type: The request is in a format not supported by the requested resource for the requested method.
416 Range Not Satisfiable: The server cannot provide the requested range of the resource; the range either overlaps with another range or it is out of the bounds of the resource.
417 Expectation Failed: The server cannot meet the requirements of the Expect request-header field.
418 I'm a teapot: This code was defined in 1998 as one of the traditional IETF April Fools' jokes. It is not expected to be implemented or used.
421 Misdirected Request: The request was directed at a server that is not able to produce a response.
422 Unprocessable Entity: The request was well-formed but was unable to be followed due to semantic errors.
423 Locked: The resource that is being accessed is locked.
424 Failed Dependency: The request failed due to a failure of a previous request.
426 Upgrade Required: The client should switch to a different protocol such as TLS/1.0.
428 Precondition Required: The origin server requires the request to be conditional.
* *429 Too Many Requests: The user has sent too many requests in a given amount of time.
431 Request Header Fields Too Large: The server is unwilling to process the request because its header fields are too large.
451 Unavailable For Legal Reasons: The user has requested a resource that is not available for legal reasons.
*/
Error::Error(request rhs)
{
    (void)rhs;
}
int Error::execute_method(request _request)
{
    (void)_request;
    return true;
}