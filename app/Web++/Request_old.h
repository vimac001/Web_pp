/* 
 * File:   Request.h
 * Author: Viktor
 *
 * Created on 29. Mai 2015, 17:12
 * /

#ifndef REQUESTO_H
#define	REQUESTO_H

#include <iostream>

#ifdef _WIN32
	#include "libfcgi\fcgio.h"
#else
	#include <fcgio.h>
#endif

#include "definitions.hpp"
#include "Response.h"

namespace wpp{
    class Request_old{
        friend class CoreApp;
        friend class BasicApp;
        friend class MainApp;
        
    public:
        std::istream in;
        
        Request_old(FCGX_Request& request);
        virtual ~Request_old();
        
        const env_t& getEnv();
        bool envVarExists(const std::string& var);
        bool envVarExists(const std::string& var, env_t::const_iterator& idx);
        const env_val_t& getEnvVar(const std::string& var);
        const std::string& getEnvVarValue(const std::string& key, const size_t& idx = 0);
        const std::string& env(const std::string& key, const size_t& idx = 0);
        
        const query_args_t& getGetArgs();
        const query_args_t& getPostArgs();
        
        bool hasGetArg(const std::string& name, query_args_t::const_iterator& idx);
        bool hasGetArg(const std::string& name);
        
        bool hasPostArg(const std::string& name, query_args_t::const_iterator& idx);
        bool hasPostArg(const std::string& name);
        
        const QueryArg& getGetArg(const std::string& name);
        const QueryArg& getPostArg(const std::string& name);
        
        const std::string& getGetValue(const std::string& name);
        const std::string& getPostValue(const std::string& name);
        
        const std::string& get(const std::string& name);
        const std::string& post(const std::string& name);
        
        
        const cookies_t& getCookies();
        bool hasCookie(const std::string& key);
        bool hasCookie(const std::string& key, cookies_t::const_iterator& idx);
        const std::string& getCookieValue(const std::string& key);
        const std::string& cookie(const std::string& key);
        const Cookie& getCookie(const std::string& key);
        
        const RequestMethod& getMethod();
        const std::string& getMethodString();
        const std::string& getUserAgent();
        const std::string& getHost();
        const std::string& getDocumentUri();
        const std::string& getServerProtocol();
        const unsigned short& getRedirectStatus();
        const size_t& getContentLength();
        const std::string& getContentType();
        
        const bool& isFileUpload(); //TODO: Rem
        
        std::string postData;
        
    protected:
        RequestMethod method;
        env_t envData;
        cookies_t cookies;
        query_args_t getArgs;
        query_args_t postArgs;
        
        bool flUpload;
        
        unsigned short redirectStatus;
        std::size_t contentLength;
        
        fcgi_streambuf in_buf;
        
        Response *response;
        
    private:
        Request_old(const Request_old& orig) = delete;
        std::string *methodStrPointer;
        std::string *userAgentStrPointer;
        std::string *hostPointer;
        std::string *docUriPointer;
        std::string *serverProtocolPointer;
        std::string *contentTypePointer;
        
        const unsigned short mppBufferSize = 255;
        
        void parseBodyData();
        bool findBoundary(const std::string& boundary, char *buffer, const unsigned short& bufferSize, std::string::size_type& pos);
        void receiveFile();
    };
}

#endif	/ * REQUEST_H */

