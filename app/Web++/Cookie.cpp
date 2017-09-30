/* 
 * File:   Cookie.cpp
 * Author: Viktor
 * 
 * Created on 10. Juni 2015, 13:18
 */

#include "Cookie.h"

using namespace wpp;

const char* Cookie::DEFAULT_PATH = "/";

Cookie::Cookie() :
        name(),
        value(),
        path(),
        domain()
{
    
}

Cookie::Cookie(const std::string& name, const std::string& value, const std::time_t& expires, 
               const std::string& path, const std::string& domain, const CookieFlag& flag){
    
    this->name = name;
    this->value = value;
    this->path = path;
    this->secure = (flag & CookieFlag::Secure) == CookieFlag::Secure;
    this->httpOnly = (flag & CookieFlag::HttpOnly) == CookieFlag::HttpOnly;
    this->domain = domain;
    
    this->expires = expires;
    
}

Cookie::Cookie(const Cookie& orig){ 
    this->name = orig.name;
    this->value = orig.value;
    this->expires = orig.expires;
    this->domain = orig.domain;
    this->path = orig.path;
    this->secure = orig.secure;
    this->httpOnly = orig.httpOnly;
}

void Cookie::setDomain(const std::string& domain){
    this->domain = domain;
}

void Cookie::setPath(const std::string& path){
    this->path = path;
}

void Cookie::setExpireTime(const std::time_t& expires){
    this->expires = expires;
}

void Cookie::setSecure(bool secure){
    this->secure = secure;
}

void Cookie::setHttpOnly(bool httpOnly){
    this->httpOnly = httpOnly;
}

const std::string& Cookie::getName(){
    return this->name;
}

const std::string& Cookie::getValue(){
    return this->value;
}

const std::string& Cookie::getPath(){
    return this->path;
}

const std::string& Cookie::getDomain(){
    return this->domain;
}

const std::time_t& Cookie::getExpireTime(){
    return this->expires;
}

const bool& Cookie::isSecure(){
    return this->secure;
}

const bool& Cookie::isHttpOnly(){
    return this->httpOnly;
}

std::string Cookie::str(){
    std::string ret = this->name;
    ret.append("=");
    ret.append(this->value);
    
    if(this->expires != 0){
        std::string strTime = asctime(localtime(&this->expires));
        std::size_t p = strTime.rfind("\n");
        
        if(p != std::string::npos){
            strTime = strTime.substr(0, p);
        }
        
        ret.append("; expires=");
        ret.append(strTime);
        //ret.append("; Max-Age=");
        //ret.append(std::to_string(static_cast<long int>(this->expires)));
    }
    
    if(this->path != "/"){
        ret.append("; path=");
        ret.append(this->path);
    }
    
    if(!this->domain.empty()){
        ret.append("; domain=");
        ret.append(this->domain);
    }
    
    if(this->secure){
        ret.append("; secure");
    }
    
    if(this->httpOnly){
        ret.append("; httponly");
    }
    
    return ret;
}

Cookie::~Cookie(){
    
}

