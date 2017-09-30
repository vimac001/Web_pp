/* 
 * File:   Cookie.h
 * Author: Viktor
 *
 * Created on 10. Juni 2015, 13:18
 */

#ifndef COOKIE_H
#define	COOKIE_H

#include "string.h"
#include <ctime>

namespace wpp{
    enum CookieFlag : unsigned char {
        Default = 0,
        Secure = 1,
        HttpOnly = 2
    };
    
//    class Request;
//    class Response;
    
    class Cookie{
        friend class Request;
        friend class Response;
        
        friend class Request_old; //TODO: rem me
        
    public:
        static const char* DEFAULT_PATH;
        
        Cookie();
        Cookie(const std::string& name, const std::string& value, const std::time_t& expires = 0, 
               const std::string& path = Cookie::DEFAULT_PATH, const std::string& domain = "", const CookieFlag& flag = CookieFlag::Default);
        
        Cookie(const Cookie& orig);
        
        void setPath(const std::string& path);
        void setDomain(const std::string& domain);
        void setExpireTime(const std::time_t& expires);
        void setSecure(bool secure);
        void setHttpOnly(bool httpOnly);
        
        const std::string& getName();
        const std::string& getValue();
        const std::string& getPath();
        const std::string& getDomain();
        const std::time_t& getExpireTime();
        const bool& isSecure();
        const bool& isHttpOnly();
        
        std::string str();
        
        virtual ~Cookie();
        
    protected:
        std::string name;
        std::string value;
        std::string path;
        std::string domain;
        std::time_t expires;
        bool secure;
        bool httpOnly;
        
    };
}
#endif	/* COOKIE_H */

