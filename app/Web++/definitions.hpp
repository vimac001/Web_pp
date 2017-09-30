/* 
 * File:   definitions.hpp
 * Author: Viktor
 *
 * Created on 31. Mai 2015, 12:32
 */

#ifndef DEFINITIONS_HPP
#define	DEFINITIONS_HPP

#include "messages.hpp"

#include "Cookie.h"
#include "QueryArg.h"

#include <map>
#include <list>
#include <vector>

#include <thread>
#include <chrono>
#include <mutex>

namespace wpp{
    
    //Web++ Framework Version 0.36 (First Prototype)
    const unsigned char MAJOR_VERSION       = 0;
    const unsigned char MINOR_VERSION       = 42;

    const std::string EMPTY_REQUEST_METHOD  = "NONE";

    const std::string METHOD_GET            = "GET";
    const std::string METHOD_POST           = "POST";
    const std::string METHOD_PUT            = "PUT";
    const std::string METHOD_DELETE         = "DELETE";
    const std::string METHOD_HEAD           = "HEAD";
    const std::string METHOD_OPTIONS        = "OPTIONS";
    const std::string METHOD_TRACE          = "TRACE";
    const std::string METHOD_CONNECT        = "CONNECT";
    
    
    //Required FastCGI env. values.
    const std::string HTTP_COOKIE           = "HTTP_COOKIE";
    const std::string HTTP_USER_AGENT       = "HTTP_USER_AGENT";
    const std::string REQUEST_METHOD        = "REQUEST_METHOD";
    const std::string QUERY_STRING          = "QUERY_STRING";
    const std::string HTTP_HOST             = "HTTP_HOST";
    const std::string DOCUMENT_URI          = "DOCUMENT_URI";
    const std::string REDIRECT_STATUS       = "REDIRECT_STATUS";
    const std::string CONTENT_LENGTH        = "CONTENT_LENGTH";
    const std::string SERVER_PROTOCOL       = "SERVER_PROTOCOL";
    const std::string CONTENT_TYPE          = "CONTENT_TYPE";
//    const std::string REMOTE_ADDR           = "REMOTE_ADDR";
//    const std::string REMOTE_PORT           = "REMOTE_PORT";
//    const std::string SERVER_ADDR           = "SERVER_ADDR";
//    const std::string SERVER_PORT           = "SERVER_PORT";
    
    //Used Response Header
    const std::string HTTPH_CONTENT_TYPE    = "Content-Type";
    const std::string HTTPH_SET_COOKIE      = "Set-Cookie";
    const std::string HTTPH_LOCATION        = "Location";
    const std::string HTTPH_STATUS          = "Status";
    
    const std::string HTTPH_LINE_END        = "\r\n";
    const std::string HTTPH_END             = "\r\n\r\n";

	/**
	* String Key-Value Map
	*/
	typedef std::map<std::string, std::string> skvmap;

    typedef std::map<std::string, Cookie> cookies_t;
    typedef std::vector<std::string> httph_val_t;
    typedef std::map<std::string, httph_val_t> httph_t;
    //typedef std::map<std::string, QueryArg> query_args_t;
    typedef std::vector<std::string> args_t;
    
    enum RequestMethod : unsigned char{
        NONE        = 0,
        OPTIONS     = 1,
        GET         = 2,
        HEAD        = 4,
        POST        = 8,
        PUT         = 16,
        DELETE      = 32,
        TRACE       = 64,
        CONNECT     = 128,
        ANY         = 255,
        STD         = 10
    };
    
    const std::string DEFAULT_RESPONSE_MESSAGE_100 = "Continue";
    const std::string DEFAULT_RESPONSE_MESSAGE_101 = "Switching Protocols";
    const std::string DEFAULT_RESPONSE_MESSAGE_200 = "OK";
    const std::string DEFAULT_RESPONSE_MESSAGE_201 = "Created";
    const std::string DEFAULT_RESPONSE_MESSAGE_202 = "Accepted";
    const std::string DEFAULT_RESPONSE_MESSAGE_203 = "Non-Authoritative Information";
    const std::string DEFAULT_RESPONSE_MESSAGE_204 = "No Content";
    const std::string DEFAULT_RESPONSE_MESSAGE_205 = "Reset Content";
    const std::string DEFAULT_RESPONSE_MESSAGE_206 = "Partial Content";
    const std::string DEFAULT_RESPONSE_MESSAGE_300 = "Multiple Choices";
    const std::string DEFAULT_RESPONSE_MESSAGE_301 = "Moved Permanently";
    const std::string DEFAULT_RESPONSE_MESSAGE_302 = "Found";
    const std::string DEFAULT_RESPONSE_MESSAGE_303 = "See Other";
    const std::string DEFAULT_RESPONSE_MESSAGE_304 = "Not Modified";
    const std::string DEFAULT_RESPONSE_MESSAGE_305 = "Use Proxy";
    const std::string DEFAULT_RESPONSE_MESSAGE_307 = "Temporary Redirect";
    const std::string DEFAULT_RESPONSE_MESSAGE_400 = "Bad Request";
    const std::string DEFAULT_RESPONSE_MESSAGE_401 = "Unauthorized";
    const std::string DEFAULT_RESPONSE_MESSAGE_402 = "Payment Required";
    const std::string DEFAULT_RESPONSE_MESSAGE_403 = "Forbidden";
    const std::string DEFAULT_RESPONSE_MESSAGE_404 = "Not Found";
    const std::string DEFAULT_RESPONSE_MESSAGE_405 = "Method Not Allowed";
    const std::string DEFAULT_RESPONSE_MESSAGE_406 = "Not Acceptable";
    const std::string DEFAULT_RESPONSE_MESSAGE_407 = "Proxy Authentication Required";
    const std::string DEFAULT_RESPONSE_MESSAGE_408 = "Request Time-out";
    const std::string DEFAULT_RESPONSE_MESSAGE_409 = "Conflict";
    const std::string DEFAULT_RESPONSE_MESSAGE_410 = "Gone";
    const std::string DEFAULT_RESPONSE_MESSAGE_411 = "Length Required";
    const std::string DEFAULT_RESPONSE_MESSAGE_412 = "Precondition Failed";
    const std::string DEFAULT_RESPONSE_MESSAGE_413 = "Request Entity Too Large";
    const std::string DEFAULT_RESPONSE_MESSAGE_414 = "Request-URI Too Large";
    const std::string DEFAULT_RESPONSE_MESSAGE_415 = "Unsupported Media Type";
    const std::string DEFAULT_RESPONSE_MESSAGE_416 = "Requested range not satisfiable";
    const std::string DEFAULT_RESPONSE_MESSAGE_417 = "Expectation Failed";
    const std::string DEFAULT_RESPONSE_MESSAGE_500 = "Internal Server Error";
    const std::string DEFAULT_RESPONSE_MESSAGE_501 = "Not Implemented";
    const std::string DEFAULT_RESPONSE_MESSAGE_502 = "Bad Gateway";
    const std::string DEFAULT_RESPONSE_MESSAGE_503 = "Service Unavailable";
    const std::string DEFAULT_RESPONSE_MESSAGE_504 = "Gateway Time-out";
    const std::string DEFAULT_RESPONSE_MESSAGE_505 = "HTTP Version not supported";
    
    
    enum ResponseStatus : unsigned short{
        Continue                        = 100,
        SwitchingProtocols              = 101,
        OK                              = 200,
        Created                         = 201,
        Accepted                        = 202,
        NonAuthoritativeInformation     = 203,
        NoContent                       = 204,
        ResetContent                    = 205,
        PartialContent                  = 206,
        MultipleChoices                 = 300,
        MovedPermanently                = 301,
        Found                           = 302,
        SeeOther                        = 303,
        NotModified                     = 304,
        UseProxy                        = 305,
        TemporaryRedirect               = 307,
        
        BadRequest                      = 400,
        Unauthorized                    = 401,
        PaymentRequired                 = 402,
        Forbidden                       = 403,
        NotFound                        = 404,
        MethodNotAllowed                = 405,
        NotAcceptable                   = 406,
        ProxyAuthenticationRequired     = 407,
        RequestTimeOut                  = 408,
        Conflict                        = 409,
        Gone                            = 410,
        LengthRequired                  = 411,
        PreconditionFailed              = 412,
        RequestEntityTooLarge           = 413,
        RequestURITooLarge              = 414,
        UnsupportedMediaType            = 415,
        RequestedRangeNotSatisfiable    = 416,
        ExpectationFailed               = 417,
        InternalServerError             = 500,
        NotImplemented                  = 501,
        BadGateway                      = 502,
        ServiceUnavailable              = 503,
        GatewayTimeOut                  = 504,
        HTTPVersionNotSupported         = 505
    };
}
#endif	/* DEFINITIONS_HPP */

