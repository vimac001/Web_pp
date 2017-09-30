/* 
 * File:   Request.cpp
 * Author: vikto
 * 
 * Created on 3. September 2015, 11:16
 */

#include "Request.h"
#include <sstream>

using namespace wpp;

Request::Request(FCGX_Request& request) :
	env(),
	httph(),
	queryArgs(),
	cookies(),

	p_docUri(),
	p_queryArgLine(),

	f_docUri(p_docUri.get_future()),
	f_queryArgLine(p_queryArgLine.get_future()),

	f_envDataReady(std::async(std::launch::async, &Request::parseEnvData, this, request.envp)),
	f_queryArgsReady(std::async(std::launch::async, &Request::parseQueryArgs, this)),

    in(&this->in_buf),
    in_buf(request.in)
{
	this->method = RequestMethod::NONE;
}

void Request::parseEnvData(char** envp)
{
	for (unsigned short i = 0; envp[i] != nullptr; i++)
	{
		if (*(unsigned int*)envp[i] == Request::httpInt) {
			//std::async(std::launch::async, &Request::addHttpArg, this, &envp[i][5]);
			this->addHttpArg(&envp[i][5]);
		}
		else {
			//std::async(std::launch::async, &Request::addEnvVar, this, &envp[i][0]);
			this->addEnvVar(&envp[i][0]);
		}
	}
	
	if(!docUriFound)
		this->p_docUri.set_exception(std::exception_ptr());

	if(!queryArgsFound)
		this->p_queryArgLine.set_exception(std::exception_ptr());
}

inline void Request::splitLine(char*& line, char*& key, char*& val, size_t& i)
{
	for (i = 0; line[i] != '='; i++)
		if (line[i] == NULLCHR)
		{
			key = nullptr;
			i = 0;

			return;
		}

	line[i] = NULLCHR;
	key = line;
	val = &line[i + 1];
}

inline void Request::splitCookieLine(char*& line, char*& key, char*& val, size_t& i, size_t& c)
{
	for (i = 0; line[i] != '='; i++)
		if (line[i] == NULLCHR)
		{
			key = nullptr;
			i = 0;

			return;
		}

	line[i] = NULLCHR;
	key = line;
	val = &line[i + 1];

	for (c = 0; val[c] != NULLCHR; c++)
	{
		if (val[c] == ';')
		{
			val[c] = NULLCHR;
			line = &val[c + 2];
			return;
		}
	}

	line = nullptr;
}

inline void Request::splitQueryArgLine(char*& line, char*& key, char*& val, size_t& i, size_t& c)
{
	for (i = 0; line[i] != '='; i++)
		if (line[i] == NULLCHR)
		{
			line = nullptr;
			key = nullptr;
			return;
		}

	line[i] = NULLCHR;
	key = line;
	val = &line[i + 1];

	for (c = 0; val[c] != NULLCHR; c++)
	{
		if (val[c] == '&')
		{
			val[c] = NULLCHR;
			line = &val[c + 1];
			return;
		}
	}

	line = nullptr;
}


inline void Request::addEnvVar(char* line)
{
	size_t kl;
	char *key, *val;
	Request::splitLine(line, key, val, kl);

	if (*(unsigned long long*)key == this->query_stLng && *(unsigned int*)(key + sizeof(unsigned long long)) == this->ringInt)
	{
		//std::async(std::launch::async, &Request::parseQueryArgs, this, val);
		p_queryArgLine.set_value(val);
		queryArgsFound = true;
	}
	else 
	{
		std::string str_key(key, kl);

		{
			//std::lock_guard<std::mutex>(this->m_env);
			this->env[str_key] = std::string(val);
		}

		if (*(unsigned long long*)key == this->documentLng && *(unsigned int*)(key + sizeof(unsigned long long)) == this->_uriInt)
		{
			this->p_docUri.set_value(this->env[str_key]);
			docUriFound = true;
		}
	}
}

inline void Request::addHttpArg(char* line)
{
	size_t kl;
	char *key, *val;
	Request::splitLine(line, key, val, kl);

	if (*(unsigned int*)key == this->cookInt && *(unsigned short*)(key + sizeof(unsigned int)) == this->ieSht) 
	{
		std::async(std::launch::async, &Request::parseCookies, this, val);
	}
	else 
	{
		//std::lock_guard<std::mutex>(this->m_httph);
		this->httph[std::string(key, kl)] = std::string(val);
	}
}

void Request::parseCookies(char* line)
{
	size_t klen, vlen;
	char *key, *val;

	//std::lock_guard<std::mutex>(this->m_cookies);

	do {
		Request::splitCookieLine(line, key, val, klen, vlen);
		this->cookies[std::string(key, klen)] = std::string(val, vlen);
	} while (line != nullptr);
}

void Request::parseQueryArgs()
{
	char *line;
	try
	{
		line = this->f_queryArgLine.get();
		

		if (line == nullptr || line[0] == NULLCHR)
			return;
	}
	catch (...)
	{
		return;
	}

	size_t klen, vlen;
	char *key, *val;

	do {
		Request::splitQueryArgLine(line, key, val, klen, vlen);
		if (key == nullptr)
			return;
		this->queryArgs[std::string(key, klen)] = std::string(val, vlen);
	} while (line != nullptr);
}

const RequestMethod& Request::getMethod(){
    if(this->method == RequestMethod::NONE){
        const std::string tmp = this->getMethodStr();//to_upper(*(this->methodStrPointer));
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

bool Request::hasCookie(const std::string& key, skvmap::const_iterator& out_citer) const
{
	out_citer = this->cookies.find(key);
	return out_citer != this->cookies.cend();
}

bool Request::hasCookie(const std::string& key) const{
	skvmap::const_iterator citer;
	return this->hasCookie(key, citer);
}

bool Request::hasQueryArg(const std::string& key, skvmap::const_iterator& out_citer) const
{
	f_queryArgsReady.wait();

	out_citer = this->queryArgs.find(key);
	return out_citer != this->queryArgs.cend();
}

bool Request::hasQueryArg(const std::string& key) const
{
	skvmap::const_iterator citer;
	return this->hasQueryArg(key, citer);
}

const std::string& Request::getQueryArg(const std::string& key) const
{
	skvmap::const_iterator citer;
	if (this->hasQueryArg(key, citer))
	{
		return citer->second;
	}

	//TODO: throw error exception
	return EMPTY_STRING;
}

const skvmap& Request::getQueryArgs() const
{
	f_queryArgsReady.wait();
	return this->queryArgs;
}

const std::string& Request::getDocumentUri() 
{
	if (this->docUri == nullptr)
	{
		try 
		{
			this->docUri = &this->f_docUri.get();
		}
		catch (...)
		{
			//throw std::exception(ERR_DOCUMENT_URI_NOT_SET, ERR_DOCUMENT_URI_NOT_SET_NR);
			return EMPTY_STRING;
		}
	}
	
	return *this->docUri;
}

const std::string& Request::getMethodStr() const {
	this->f_envDataReady.wait();

	skvmap::const_iterator ci = this->env.find(REQUEST_METHOD);
	if (ci != this->env.cend())
	{
		return ci->second;
	}

	return EMPTY_STRING;
}

Request::~Request() {
    
}

