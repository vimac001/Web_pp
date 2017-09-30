/* 
 * File:   Request.cpp
 * Author: Viktor
 * 
 * Created on 29. Mai 2015, 17:12
 * /
#include "Request_old.h"
#include <sstream>

using namespace wpp;

Request_old::Request_old(FCGX_Request& request) : 
    in(&this->in_buf),
    envData(),
    cookies(),
    getArgs(),
    postArgs(),
    in_buf(request.in)
{
    this->response = nullptr;
    
    this->method = RequestMethod::NONE;
    this->methodStrPointer = nullptr;
    this->userAgentStrPointer = nullptr;
    this->hostPointer = nullptr;
    this->docUriPointer = nullptr;
    this->serverProtocolPointer = nullptr;
    this->contentTypePointer = nullptr;
    this->flUpload = false;
    this->redirectStatus = 0;
    this->contentLength = 0;
    
    std::string::size_type pos;
    char **env = request.envp;
    
    for(int i = 0; env[i] != nullptr; i++){
        std::string row = env[i];
        pos = row.find('=');
        if(pos != std::string::npos){
            std::string key = row.substr(0, pos);
            //to_upper(key);
            std::string val = row.substr(pos+1, row.length() - pos);
            
            env_t::iterator idx = this->envData.find(key);
            if(idx == this->envData.end()){
                this->envData[key] = env_val_t();
            }
            
            this->envData[key].push_back(val);
            
            if(key == REQUEST_METHOD){
                this->methodStrPointer = &this->envData[key].back();
            }else if(key == HTTP_USER_AGENT){
                this->userAgentStrPointer = &this->envData[key].back();
            }else if(key == HTTP_COOKIE){
                std::string::size_type epos = 0;
                
                bool run = true;
                do{
                    pos = val.find("=", epos);
                    if(epos > 0) epos += 2;
                    std::string ckey = val.substr(epos, pos - epos);
                    
                    epos = val.find(";", pos);
                    if(epos == std::string::npos){
                        epos = val.length();
                        run = false;
                    }
                    
                    std::string cval = val.substr(pos+1, epos - pos - 1);
                    
                    this->cookies[ckey] = Cookie(ckey, cval);
                    
                }while(run);
                
            }else if(key == HTTP_HOST){
                this->hostPointer = &this->envData[key].back();
            }else if(key == DOCUMENT_URI){
                this->docUriPointer = &this->envData[key].back();
            }else if(key == CONTENT_TYPE){
                this->contentTypePointer = &this->envData[key].back();
            }else if(key == QUERY_STRING){
                std::string::size_type epos = 0;
                
                bool run = true;
                do{
                    pos = val.find("=", epos);
                    if(epos > 0) epos += 1;
                    std::string ckey = val.substr(epos, pos - epos);
                    
                    epos = val.find("&", pos);
                    if(epos == std::string::npos){
                        epos = val.length();
                        run = false;
                    }
                    
                    std::string cval = val.substr(pos+1, epos - pos - 1);
                    
                    this->getArgs[ckey] = QueryArg(ckey, cval, QueryArgType::Get);
                }while(run);
            }else if(key == REDIRECT_STATUS){
                std::istringstream ss(val);
                ss >> this->redirectStatus;
            }else if(key == CONTENT_LENGTH){
                std::istringstream ss(val);
                ss >> this->contentLength;
            }else if(key == SERVER_PROTOCOL){
                this->serverProtocolPointer = &this->envData[key].back();
            }
        }
    }
    
    this->parseBodyData();
}

inline bool Request_old::findBoundary(const std::string& boundary, char* buffer, const unsigned short& bufferSize, std::string::size_type& pos){
    std::string::size_type len;
    unsigned short i, p = 0;
    
    const char *str = boundary.c_str();
    
    bool run = true;
    do{
        len = this->contentLength - pos;
        if(len > bufferSize)
            len = bufferSize;
        else
            run = false;

        this->in.read(buffer, len);
        for(i = 0; i < len; i++){
            if(buffer[i] != str[p++]){
                ++i -= p;
                p = 0;
            }else if(p == boundary.length()){
                //Found
                pos += (i - (p - 1));
                return true;
            }
        }
        
        pos += len;
    }while(run);
    
    //Not found
    return false;
}

inline void Request_old::parseBodyData(){
    if(this->contentLength <= 0)
        return;
    
    std::string::size_type pos = this->contentTypePointer->find("boundary"), bpos, epos;
    if(pos != std::string::npos){
        pos += 9;
        epos = this->contentTypePointer->find(';', pos);
        if(epos == std::string::npos)
            epos = this->contentTypePointer->length();
        
        std::string boundaryId = this->contentTypePointer->substr(pos, epos - pos);
        
        //Pase data
        char tmp[Request_old::mppBufferSize];
        pos = 0;
        epos = 0;
        
        //  ---------------------------7df6a26305ea
        //-----------------------------7df6a26305ea
        //-----------------------------7df6a26305ea--
        
        const std::string search_bid = "--" + boundaryId;
        std::streampos p = this->in.tellg();
        
        std::cout << "Content-Length: " << this->contentLength << std::endl;
        std::cout << "Boundary-Id: " << search_bid << std::endl << std::endl;
        
        while(this->findBoundary(search_bid, tmp, Request_old::mppBufferSize, epos)){
            //Between pos and epos is arg value.
            //After epos + boundaryLength is start of next arg
            std::cout << "Tellg: " << p << std::endl;
            std::cout << "Found " << epos << std::endl;
            pos = epos;
            
            std::streampos p = epos + search_bid.length();
            p += search_bid.length();
            
            //this->in.seekg(p);
        }
        
        std::cout << "Last pos " << epos << std::endl;
        
        char t[epos - pos + 1];
        this->in.seekg(pos);
        this->in.read(t, epos - pos);
        
        std::cout << "\"" << t << "\"" << std::endl;
        
        
        //Between pos and stream end is arg value.
        
//        std::string tstr = "";
//        std::size_t len = this->contentLength;
//        pos = 0;
//        bpos = 0;
//        
//        bool cont = true;
//        do{            
//            bool run = true;
//            do{
//                if(len <= Request_old::mppBufferSize){
//                    cont = false;
//                    this->in.read(tmp, len);
//                }else{
//                    this->in.read(tmp, Request_old::mppBufferSize);
//                    len -= Request_old::mppBufferSize;
//                }
//
//                tstr += tmp;
//                epos = tstr.find(HTTPH_END, pos);
//                if(epos == std::string::npos){
//                    pos += Request_old::mppBufferSize;
//                }else{
//                    pos = epos;
//                    run = false;
//                }
//            }while(run && cont);
//
//            if(epos != std::string::npos){
//                
//            }else{
//                throw "TODO: better exception. Unexpected request body format!";
//            }
//        }while(cont);
    }else{
        char *tmp = new char[this->contentLength + 1];
        this->in.read(tmp, this->contentLength);
        tmp[this->contentLength] = '\0';

        this->postData = tmp;

        epos = 0;

        std::string val = tmp;

        query_args_t::iterator argidx;

        bool run = true;
        do{
            pos = val.find("=", epos);
            if(epos > 0) epos += 1;
            std::string ckey = val.substr(epos, pos - epos);

            epos = val.find("&", pos);
            if(epos == std::string::npos){
                epos = val.length();
                run = false;
            }

            std::string cval = val.substr(pos + 1, epos - pos - 1);

            this->postArgs[ckey] = QueryArg(ckey, cval, QueryArgType::Post);
        }while(run);

        delete tmp;
    }
}

const env_t& Request_old::getEnv(){
    return this->envData;
}

bool Request_old::envVarExists(const std::string& var, env_t::const_iterator& idx){
    idx = this->envData.find(var);
    return (idx != this->envData.end());
}

bool Request_old::envVarExists(const std::string& var){
    env_t::const_iterator idx;
    return this->envVarExists(var, idx);
}

const env_val_t& Request_old::getEnvVar(const std::string& var){
    env_t::const_iterator idx;
    if(this->envVarExists(var, idx)){
        return idx->second;
    }
    
    //TODO: throw a special exception.
    return env_val_t();
}

const std::string& Request_old::getEnvVarValue(const std::string& key, const size_t& idx){
    return this->getEnvVar(key)[idx];
}

const std::string& Request_old::env(const std::string& key, const size_t& idx){
    return this->getEnvVar(key)[idx];
}

const query_args_t& Request_old::getGetArgs(){
    return this->getArgs;
}

const query_args_t& Request_old::getPostArgs(){
    return this->postArgs;
}

bool Request_old::hasGetArg(const std::string& name, query_args_t::const_iterator& idx){
    idx = this->getArgs.find(name);
    return (idx != this->getArgs.end());
}

bool Request_old::hasGetArg(const std::string& name){
    query_args_t::const_iterator idx;
    return this->hasGetArg(name, idx);
}

bool Request_old::hasPostArg(const std::string& name, query_args_t::const_iterator& idx){
    idx = this->postArgs.find(name);
    return (idx != this->postArgs.end());
}

bool Request_old::hasPostArg(const std::string& name){
    query_args_t::const_iterator idx;
    return this->hasPostArg(name, idx);
}

const QueryArg& Request_old::getGetArg(const std::string& name){
    query_args_t::const_iterator idx;
    if(this->hasGetArg(name, idx)){
        return idx->second;
    }
    
    //TODO: throw a special exception
    return QueryArg();
}

const QueryArg& Request_old::getPostArg(const std::string& name){
    query_args_t::const_iterator idx;
    if(this->hasPostArg(name, idx)){
        return idx->second;
    }
    
    //TODO: throw a special exception
    return QueryArg();
}

const std::string& Request_old::getGetValue(const std::string& name){
    return this->getGetArg(name).value;
}

const std::string& Request_old::getPostValue(const std::string& name){
    return this->getPostArg(name).value;
}

const std::string& Request_old::get(const std::string& name){
    return this->getGetArg(name).value;
}

const std::string& Request_old::post(const std::string& name){
    return this->getPostArg(name).value;
}

const cookies_t& Request_old::getCookies(){
    return this->cookies;
}

bool Request_old::hasCookie(const std::string& key, cookies_t::const_iterator& idx){
    idx = this->cookies.find(key);
    return (idx != this->cookies.end());
}

bool Request_old::hasCookie(const std::string& key){
    cookies_t::const_iterator idx;
    return this->hasCookie(key, idx);
}

const Cookie& Request_old::getCookie(const std::string& key){
    cookies_t::const_iterator idx;
    if(this->hasCookie(key, idx)){
        return idx->second;
    }
    
    throw std::string("Cookie not found! Please use \"hasCookie\" method to check first.");
    
    //TODO: throw exception
    return wpp::Cookie();
}

const std::string& Request_old::getCookieValue(const std::string& key){
    return this->getCookie(key).value;
}

const std::string& Request_old::cookie(const std::string& key){
    return this->getCookie(key).value;
}

const RequestMethod& Request_old::getMethod(){
    if(this->method == RequestMethod::NONE && this->methodStrPointer != NULL){
        std::string tmp = *(this->methodStrPointer);//to_upper(*(this->methodStrPointer));
        if(tmp == METHOD_GET){
            this->method = RequestMethod::GET;
        }else if(tmp == METHOD_POST){
            this->method = RequestMethod::POST;
        }else if(tmp == METHOD_PUT){
            this->method = RequestMethod::PUT;
        }else if(tmp == METHOD_DELETE){
            this->method = RequestMethod::DELETE;
        }else if(tmp == METHOD_HEAD){
            this->method = RequestMethod::HEAD;
        }else if(tmp == METHOD_OPTIONS){
            this->method = RequestMethod::OPTIONS;
        }else if(tmp == METHOD_TRACE){
            this->method = RequestMethod::TRACE;
        }else if(tmp == METHOD_CONNECT){
            this->method = RequestMethod::CONNECT;
        }
    }
    
    return this->method;
}

const std::string& Request_old::getMethodString(){
    if(this->methodStrPointer == nullptr)
        return EMPTY_REQUEST_METHOD;
    
    return *(this->methodStrPointer); //to_upper(*(this->methodStrPointer));
}

const std::string& Request_old::getUserAgent(){
    if(this->userAgentStrPointer == nullptr){
        return EMPTY_STRING;
    }
    
    return *(this->userAgentStrPointer);
}

const std::string& Request_old::getHost(){
    if(this->hostPointer == nullptr){
        return EMPTY_STRING;
    }
    
    return *(this->hostPointer);
}

const std::string& Request_old::getDocumentUri(){
    if(this->docUriPointer == nullptr){
        return EMPTY_STRING;
    }
    
    return *(this->docUriPointer);
}

const std::string& Request_old::getServerProtocol(){
    if(this->serverProtocolPointer == nullptr){
        return EMPTY_STRING;
    }
    
    return *(this->serverProtocolPointer);
}

const unsigned short& Request_old::getRedirectStatus(){
    return this->redirectStatus;
}

const size_t& Request_old::getContentLength(){
    return this->contentLength;
}

const std::string& Request_old::getContentType(){
    if(this->contentTypePointer == nullptr){
        return EMPTY_STRING;
    }
    
    return *(this->contentTypePointer);
}

const bool& Request_old::isFileUpload(){
    return this->flUpload;
}

Request_old::~Request_old(){ }
*/

