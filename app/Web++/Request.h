/* 
 * File:   Request.h
 * Author: vikto
 *
 * Created on 3. September 2015, 11:16
 */

#ifndef REQUEST_H
#define REQUEST_H

#include <iostream>

#ifdef _WIN32
	#include "libfcgi\fcgio.h"
#else
	#include <fcgio.h>
#endif

#include "definitions.hpp"

#include <future>

namespace wpp{
    class Response;

    class Request{
        friend class CoreApp;
        friend class BasicApp;
        friend class MainApp;
        
    public:
        std::istream in;
        
        Request(FCGX_Request& request);
        virtual ~Request();
        
        
        const RequestMethod& getMethod();
		const std::string& getDocumentUri();
		const std::string& getMethodStr() const;
        
		bool hasCookie(const std::string& key, skvmap::const_iterator& out_citer) const;
        bool hasCookie(const std::string& key) const;

		bool hasQueryArg(const std::string& key, skvmap::const_iterator& out_citer) const;
		bool hasQueryArg(const std::string& key) const;
		const std::string& getQueryArg(const std::string& key) const;
		const skvmap& getQueryArgs() const;
        
    protected:
        fcgi_streambuf in_buf;
        
        RequestMethod method;
        
        Response *response;

		std::promise<std::string&> p_docUri;
		std::promise<char*> p_queryArgLine;

		std::future<void> f_envDataReady;
		std::future<void> f_queryArgsReady;

		std::future<std::string&> f_docUri;
		std::future<char*> f_queryArgLine;

		skvmap env;
		skvmap httph;
		skvmap queryArgs;
		skvmap cookies;
        
    private:
        Request(const Request& orig) = delete;

		bool docUriFound = false;
		bool queryArgsFound = false;

		const unsigned int httpInt				= *((unsigned int*)"HTTP");
		const unsigned int cookInt				= *((unsigned int*)"COOK");
		const unsigned short ieSht				= *((unsigned short*)"IE");
		const unsigned long long query_stLng	= *((unsigned long long*)"QUERY_ST");
		const unsigned int ringInt				= *((unsigned int*)"RING");
		const unsigned long long documentLng	= *((unsigned long long*)"DOCUMENT");
		const unsigned int _uriInt				= *((unsigned int*)"_URI");

        std::string *hostPointer = nullptr;
		std::string *docUri = nullptr;

		void parseEnvData(char**);

		inline static void splitLine(char*& line, char*& key, char*& val, size_t& keyLength);
		inline static void splitCookieLine(char*& line, char*& key, char*& val, size_t& keyLength, size_t& valLength);
		inline static void splitQueryArgLine(char*& line, char*& key, char*& val, size_t& keyLength, size_t& valLength);

		inline void addEnvVar(char* line);
		inline void addHttpArg(char* line);
		void parseCookies(char* line);
		void parseQueryArgs();
    };
}

#endif /* REQUEST_H */

