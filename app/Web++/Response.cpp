/* 
 * File:   Response.cpp
 * Author: Viktor
 * 
 * Created on 31. Mai 2015, 16:01
 */

#include "Response.h"

using namespace wpp;

//std::map<ResponseStatus, std::string> Response::responseStatusMessage;
//Response::responseStatusMessage[ResponseStatus::Continue] = "Continue";

const std::string& Response::getStatusMessage(const ResponseStatus& statusCode)
{
    if(statusCode > ResponseStatus::BadRequest){
        if(statusCode < ResponseStatus::ExpectationFailed){
            if(statusCode < ResponseStatus::NotAcceptable){
                if(statusCode > ResponseStatus::Forbidden){
                    if(statusCode == ResponseStatus::NotFound)
                        return DEFAULT_RESPONSE_MESSAGE_404;
                    else return DEFAULT_RESPONSE_MESSAGE_405;
                }else if(statusCode < ResponseStatus::Forbidden){
                    if(statusCode == ResponseStatus::Unauthorized)
                        return DEFAULT_RESPONSE_MESSAGE_401;
                    else return DEFAULT_RESPONSE_MESSAGE_402;
                }else return DEFAULT_RESPONSE_MESSAGE_403;
            }else if(statusCode > ResponseStatus::NotAcceptable){
                if(statusCode < ResponseStatus::PreconditionFailed){
                    if(statusCode < ResponseStatus::Conflict){
                        if(statusCode == ResponseStatus::RequestTimeOut)
                            return DEFAULT_RESPONSE_MESSAGE_408;
                        else return DEFAULT_RESPONSE_MESSAGE_407;
                    }else if(statusCode > ResponseStatus::Conflict){
                        if(statusCode == ResponseStatus::LengthRequired)
                            return DEFAULT_RESPONSE_MESSAGE_411;
                        else return DEFAULT_RESPONSE_MESSAGE_410;
                    }else return DEFAULT_RESPONSE_MESSAGE_409;
                }else if(statusCode > ResponseStatus::PreconditionFailed){
                    if(statusCode < ResponseStatus::UnsupportedMediaType){
                        if(statusCode == ResponseStatus::RequestEntityTooLarge)
                            return DEFAULT_RESPONSE_MESSAGE_413;
                        else return DEFAULT_RESPONSE_MESSAGE_414;
                    }else if(statusCode == ResponseStatus::UnsupportedMediaType)
                        return DEFAULT_RESPONSE_MESSAGE_415;
                    else return DEFAULT_RESPONSE_MESSAGE_416;
                }else return DEFAULT_RESPONSE_MESSAGE_412;
            }else return DEFAULT_RESPONSE_MESSAGE_406;
        }else if(statusCode > ResponseStatus::ExpectationFailed){
            if(statusCode < ResponseStatus::ServiceUnavailable){
                if(statusCode < ResponseStatus::NotImplemented)
                    return DEFAULT_RESPONSE_MESSAGE_500;
                else if(statusCode > ResponseStatus::NotImplemented)
                    return DEFAULT_RESPONSE_MESSAGE_502;
                else return DEFAULT_RESPONSE_MESSAGE_501;
            }else if(statusCode > ResponseStatus::ServiceUnavailable){
                if(statusCode == ResponseStatus::GatewayTimeOut)
                    return DEFAULT_RESPONSE_MESSAGE_504;
                else return DEFAULT_RESPONSE_MESSAGE_505;
            }else return DEFAULT_RESPONSE_MESSAGE_503;
        }else return DEFAULT_RESPONSE_MESSAGE_417;
    }else if(statusCode < ResponseStatus::BadRequest){
        if(statusCode > ResponseStatus::PartialContent){
            if(statusCode > ResponseStatus::SeeOther){
                if(statusCode < ResponseStatus::UseProxy)
                    return DEFAULT_RESPONSE_MESSAGE_304;
                else if(statusCode > ResponseStatus::UseProxy)
                    return DEFAULT_RESPONSE_MESSAGE_307;
                else return DEFAULT_RESPONSE_MESSAGE_305;
            }else if(statusCode < ResponseStatus::SeeOther){
                if(statusCode > ResponseStatus::MovedPermanently)
                    return DEFAULT_RESPONSE_MESSAGE_302;
                else if(statusCode < ResponseStatus::MovedPermanently)
                    return DEFAULT_RESPONSE_MESSAGE_300;
                else return DEFAULT_RESPONSE_MESSAGE_301;
            }else return DEFAULT_RESPONSE_MESSAGE_303;
        }else if(statusCode < ResponseStatus::PartialContent){
            if(statusCode < ResponseStatus::Created){
                if(statusCode > ResponseStatus::SwitchingProtocols)
                    return DEFAULT_RESPONSE_MESSAGE_200;
                else if(statusCode == ResponseStatus::SwitchingProtocols)
                    return DEFAULT_RESPONSE_MESSAGE_101;
                else return DEFAULT_RESPONSE_MESSAGE_100;
            }else if(statusCode > ResponseStatus::Created){
                if(statusCode < ResponseStatus::NonAuthoritativeInformation)
                    return DEFAULT_RESPONSE_MESSAGE_202;
                else if(statusCode > ResponseStatus::NonAuthoritativeInformation){
                    if(statusCode > ResponseStatus::NoContent)
                        return DEFAULT_RESPONSE_MESSAGE_205;
                    else return DEFAULT_RESPONSE_MESSAGE_204;
                }else return DEFAULT_RESPONSE_MESSAGE_203;
            }else return DEFAULT_RESPONSE_MESSAGE_201;
        }else return DEFAULT_RESPONSE_MESSAGE_206;
    }else return DEFAULT_RESPONSE_MESSAGE_400;
}

Response::Response(FCGX_Request& request) :
    out(std::ios_base::out),
    err(std::ios_base::out),
    out_buf(request.out),
    err_buf(request.err),
    headers()
{
    this->request = nullptr;
    this->setContentType("text/html");
}

//Response::Response(const Response&& org){
////    this->cookies = org.cookies;
////    this->err = org.err;
////    this->err_buf = org.err_buf;
////    this->headers = org.headers;
////    this->hostPointer = org.hostPointer;
////    this->out = org.out;
////    this->out_buf = org.out_buf;
////    this->request = org.request;
//}

bool Response::headerIsSet(const std::string& header, httph_t::const_iterator& idx) const
{
    idx = this->headers.find(header);
    return (idx != this->headers.end());
}

bool Response::headerIsSet(const std::string& header) const
{
    httph_t::const_iterator idx;
    return this->headerIsSet(header, idx);
}

void Response::addHeader(const std::string& header, const std::string& value)
{
    httph_t::const_iterator idx;
    if(!this->headerIsSet(header, idx)){
        this->headers[header] = httph_val_t();
    }
    
    this->headers[header].push_back(value);
}

const httph_val_t& Response::getHeaderValues(const std::string& header) const
{
    httph_t::const_iterator idx;
    if(this->headerIsSet(header, idx)){
        return idx->second;
    }
    
    //TODO: throw special exception
    return httph_val_t();
}

const std::string& Response::getHeader(const std::string& header, long unsigned int pos) const
{
    return this->getHeaderValues(header)[pos];
}

void Response::addCookie(Cookie cookie){
    this->cookies[cookie.getName()] = cookie;
}

void Response::setCookie(const std::string& key, const std::string& value, const std::time_t& expires, 
                         const std::string& path, const std::string& domain, const CookieFlag& flag){
    this->cookies[key] = Cookie(key, value, expires, path, domain, flag);
//    if(domain.empty()){
//        this->cookies[key].setDomain("." + (*this->hostPointer));
//    }
}

void Response::setCookie(const std::string& key, const std::string& value, const CookieFlag& flag){
    this->setCookie(key, value, 0, Cookie::DEFAULT_PATH, EMPTY_STRING, flag);
}

void Response::setCookie(const std::string& key, const std::string& value, const std::time_t& expires, const CookieFlag& flag){
    this->setCookie(key, value, expires, Cookie::DEFAULT_PATH, EMPTY_STRING, flag);
}

void Response::setCookie(const std::string& key, const std::string& value, const std::string& path, 
                         const std::string& domain, const CookieFlag& flag){
    this->setCookie(key, value, 0, path, domain, flag);
}

bool Response::initCookie(const std::string& key, const std::string& value, const std::time_t& expires, 
                          const std::string& path, const std::string& domain, const CookieFlag& flag){
    
    if(this->request->hasCookie(key) || this->hasCookie(key))
        return false;
    
    this->setCookie(key, value, expires, path, domain, flag);
    return true;
}

bool Response::initCookie(const std::string& key, const std::string& value, const std::time_t& expires, const CookieFlag& flag){
    if(this->request->hasCookie(key) || this->hasCookie(key))
        return false;
    
    this->setCookie(key, value, expires, flag);
    return true;
}

bool Response::initCookie(const std::string& key, const std::string& value, const CookieFlag& flag){
    if(this->request->hasCookie(key) || this->hasCookie(key))
        return false;
    
    this->setCookie(key, value, flag);
    return true;
}

bool Response::initCookie(const std::string& key, const std::string& value, const std::string& path,
                          const std::string& domain, const CookieFlag& flag){

    if(this->request->hasCookie(key) || this->hasCookie(key))
        return false;
    
    this->setCookie(key, value, path, domain, flag);
    return true;
}

bool Response::hasCookie(const std::string& key, cookies_t::const_iterator& idx) const
{
    idx = this->cookies.find(key);
    return (idx != this->cookies.end());
}

bool Response::hasCookie(const std::string& key) const
{
    cookies_t::const_iterator idx;
    return this->hasCookie(key, idx);
}

void Response::setContentType(const std::string& type){
    this->addHeader(HTTPH_CONTENT_TYPE, type);
}

void Response::setStatus(const unsigned short& statusCode, const std::string& statusMessage){
    std::string val = std::to_string(statusCode);
    val += ' ';
    val += statusMessage;
    this->addHeader(HTTPH_STATUS, val);
}

void Response::setStatus(const ResponseStatus& statusCode){
    this->setStatus(statusCode, Response::getStatusMessage(statusCode));
}

void Response::redirect(const std::string& location){
    this->addHeader(HTTPH_LOCATION, location);
}

std::string Response::getRawHeader(){
    std::stringstream retStrm;
    
    httph_t::const_iterator i = this->headers.begin();
    httph_val_t::const_iterator h;
    
    for(; i != this->headers.end(); i++){
        for(h = i->second.begin(); h != i->second.end(); h++){
            retStrm << i->first << ": " << (*h) << HTTPH_LINE_END;
        }
    }
    
    if(this->cookies.size() > 0){
        wpp::cookies_t::iterator i;
        for(i = this->cookies.begin(); i != this->cookies.end(); i++){
            retStrm << HTTPH_SET_COOKIE << ": " << i->second.str() << HTTPH_LINE_END;
        }
    }
    
    return retStrm.str();
}

std::string Response::getRawBody(){
    return this->out.str();
}

std::string Response::getRawResponse(){
    return this->getRawHeader() + HTTPH_LINE_END + this->getRawBody();
}

Response::~Response(){ }

