/* 
 * File:   Response.h
 * Author: Viktor
 *
 * Created on 31. Mai 2015, 16:01
 */

#ifndef RESPONSE_H
#define	RESPONSE_H

#include <iostream>
#include <sstream>

#ifdef _WIN32
	#include "libfcgi\fcgio.h"
#else
	#include <fcgio.h>
#endif

#include "definitions.hpp"
#include "Request.h"

namespace wpp{
    class Response{
        friend class CoreApp;
        friend class BasicApp;
        friend class MainApp;
        
    public:
        std::ostringstream out;
        std::ostringstream err;
        
        static const std::string& getStatusMessage(const ResponseStatus& statusCode);
        
//        static std::map<ResponseStatus, std::string> responseStatusMessage;
        
        Response(FCGX_Request& request);
        
        void addHeader(const std::string& header, const std::string& value);
        void setContentType(const std::string& type);
        void setStatus(const unsigned short& statusCode, const std::string& statusMessage);
        void setStatus(const ResponseStatus& statusCode);
        void redirect(const std::string& location);
        
        bool headerIsSet(const std::string& header, httph_t::const_iterator& idx) const;
        bool headerIsSet(const std::string& header) const;
        const httph_val_t& getHeaderValues(const std::string& header) const;
        const std::string& getHeader(const std::string& header, long unsigned int pos = 0) const;
        
        void addCookie(Cookie cookie);
        void setCookie(const std::string& key, const std::string& value, const std::time_t& expires = 0, 
                       const std::string& path = Cookie::DEFAULT_PATH, const std::string& domain = wpp::EMPTY_STRING, const CookieFlag& flag = CookieFlag::Default);
        
        void setCookie(const std::string& key, const std::string& value, const CookieFlag& flag);
        void setCookie(const std::string& key, const std::string& value, const std::time_t& expires, const CookieFlag& flag);
        void setCookie(const std::string& key, const std::string& value, const std::string& path, const std::string& domain = wpp::EMPTY_STRING,
                       const CookieFlag& flag = CookieFlag::Default);
        
        bool hasCookie(const std::string& key, cookies_t::const_iterator& idx) const;
        bool hasCookie(const std::string& key) const;
        
        bool initCookie(const std::string& key, const std::string& value, const std::time_t& expires = 0, 
                        const std::string& path = Cookie::DEFAULT_PATH, const std::string& domain = wpp::EMPTY_STRING, const CookieFlag& flag = CookieFlag::Default);
        
        bool initCookie(const std::string& key, const std::string& value, const CookieFlag& flag);
        bool initCookie(const std::string& key, const std::string& value, const std::time_t& expires, const CookieFlag& flag);
        bool initCookie(const std::string& key, const std::string& value, const std::string& path, const std::string& domain = wpp::EMPTY_STRING,
                        const CookieFlag& flag = CookieFlag::Default);
        
        virtual ~Response();
        
    protected:
        fcgi_streambuf out_buf;
        fcgi_streambuf err_buf;
        
        httph_t headers;
        cookies_t cookies;
        
        Request *request;
        
        std::string getRawHeader();
        std::string getRawBody();
        std::string getRawResponse();
        
    private:
        std::string *hostPointer;
        Response(const Response&) = delete;
        Response(const Response&&) = delete;
    };
}

#endif	/* RESPONSE_H */

