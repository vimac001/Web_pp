/* 
 * File:   RouteTree.cpp
 * Author: Viktor
 * 
 * Created on 10. Juli 2015, 15:22
 */

#include "RouteTree.h"


using namespace wpp;


//RouteLeaf
RouteLeaf::RouteLeaf() : managers(), writeLock() {
    this->lmgr = nullptr;
}

void RouteLeaf::addManager(std::regex*& rx, CoreManager<CoreController>*& mgr){
	std::lock_guard<std::mutex> lg(this->writeLock);
    this->managers[rx] = mgr;
}

CoreManager<CoreController>* RouteLeaf::getManager(const std::string& subject){
    this->lmgr = nullptr;
    for(std::map<std::regex*, CoreManager<CoreController>*>::iterator i = this->managers.begin(); i != this->managers.end(); i++){
        if(regex_match(subject, *(i->first))){
            this->lmgr = i->second;
            break;
        }
    }
    
    return this->lmgr;
}

CoreManager<CoreController>* RouteLeaf::getLastManager(){
    return this->lmgr;
}

RouteLeaf::~RouteLeaf(){
    
}

RouteLeaf& RouteLeaf::operator +=(const RouteLeaf& rhs){
	std::lock_guard<std::mutex> lg(this->writeLock);
    for(std::map<std::regex*, CoreManager<CoreController>*>::const_iterator i = rhs.managers.begin(); i != rhs.managers.end(); i++){
        this->managers[i->first] = i->second;
    }

    return *this;
}
//End RouteLeaf
//##############################################################################
//CharNode
const unsigned short CharNode::NodeArrayLength = 256;

CharNode::CharNode(const std::string& str, std::size_t& pos, RouteLeaf*& value) : 
    nodes(CharNode::NodeArrayLength, nullptr), 
    appendLock(),
    writeLock()
{
    this->level = pos;
    if(pos < str.length() - 1){
        unsigned char idx = (unsigned char)str[++pos];
        this->nodes[idx] = new CharNode(str, pos , value);
        this->value = nullptr;
    }else{
        this->value = value;
    }
}


void CharNode::append(const std::string& str, std::size_t& pos, RouteLeaf*& value){
    if(pos < str.length() - 1){
        unsigned char idx = (unsigned char)str[++pos];
        if(this->nodes[idx] == nullptr){
			std::unique_lock<std::mutex> lg(this->appendLock, std::defer_lock);
            if(lg.try_lock()){
                this->nodes[idx] = new CharNode(str, pos, value);
				lg.unlock();
            }else{
				lg.lock();
                if(this->nodes[idx] == nullptr){
                    this->nodes[idx] = new CharNode(str, pos, value);
					lg.unlock();
                }else{
					lg.unlock();
                    this->nodes[idx]->append(str, pos, value);
                }
            }
        }else
            this->nodes[idx]->append(str, pos, value);
    }else{
        if(this->value == nullptr){
			std::unique_lock<std::mutex> lg(this->writeLock, std::defer_lock);
            if(lg.try_lock()){
                this->value = value;
                lg.unlock();
            }else{
                lg.lock();
                if(this->value == nullptr){
                    this->value = value;
                    lg.unlock();
                }else{
                    lg.unlock();
                    (*this->value) += *value;
                    delete value;
                }
            }
        }else{
            (*this->value) += *value;
            delete value;
        }
    }
}

RouteLeaf*& CharNode::find(const std::string& str, std::string& out){
    if(this->level < (str.length() - 1)){
        unsigned char idx = (unsigned char)str[this->level + 1];
        if(this->nodes[idx] != nullptr){
            if(this->value != nullptr){
                RouteLeaf*& t = this->nodes[idx]->find(str, out);
                if(t == nullptr || t->getManager(out) == nullptr){
                    std::string tout = str.substr(this->level + 1, str.length() - this->level);
                    if(this->value->getManager(tout) != nullptr){
                        out = tout;
                        return this->value;
                    }
                }

                return t;
            }else return this->nodes[idx]->find(str, out);
        }
        else
            out = str.substr(this->level + 1, str.length() - this->level);
    }

    return this->value;
}

bool CharNode::find(const std::string& str, RouteLeaf*& out){
    if(this->level < (str.length() - 1)){
        unsigned char idx = (unsigned char)str[this->level + 1];
        if(this->nodes[idx] != nullptr){
            return this->nodes[idx]->find(str, out);
        }else return false;
    }else{
        if(this->value != nullptr){
            out = this->value;
            return true;
        }
        
        return false;
    }
}

CharNode::~CharNode(){
    for(unsigned short i = 0; i < CharNode::NodeArrayLength; i++){
        delete this->nodes[i];
    }
}
//End CharNode
//##############################################################################
//RouteTree
const std::regex::flag_type RouteTree::RegexFlag = std::regex::ECMAScript | std::regex::icase | std::regex::optimize;

RouteTree::RouteTree() : 
    nodes(CharNode::NodeArrayLength, nullptr), 
    regexs(), 
    appendLock() 
{ 
    
}

RouteTree::RouteTree(const RouteTree&& orig){
    this->nodes = orig.nodes;
    this->regexs = orig.regexs;
}


inline void RouteTree::parseRoute(const std::string& fullRoute, std::string& route, std::string& regex){
    size_t pos, epos;
    pos = fullRoute.find('{');
    epos = fullRoute.rfind('}');
    
    if(pos == std::string::npos || epos == std::string::npos){
        route = fullRoute;
    }else{
        route = fullRoute.substr(0, pos++);
        regex = fullRoute.substr(pos, epos - pos);
    }
}

inline std::regex*& RouteTree::getRegexObject(const std::string& pattern){
    std::map<std::string, std::regex*>::iterator i = this->regexs.find(pattern);
    if(i == this->regexs.end()){
        this->regexs[pattern] = new std::regex(pattern, RouteTree::RegexFlag);
        return this->regexs[pattern];
    }else{
        return i->second;
    }
}

void RouteTree::add(const std::string& route, CoreManager<CoreController>* mgr){
    std::string rt, rx;
    RouteTree::parseRoute(route, rt, rx);
    
    std::regex *t = this->getRegexObject(rx);
    
    RouteLeaf *leaf = new RouteLeaf();
    leaf->addManager(t, mgr);
    
    size_t pos = 0;
    
    unsigned char idx = (unsigned char)rt[pos];
    
    if(this->nodes[idx] == nullptr){
		std::unique_lock<std::mutex> lg(this->appendLock, std::defer_lock);

        if(lg.try_lock()){
            this->nodes[idx] = new CharNode(rt, pos, leaf);
			lg.unlock();
        }else{
			lg.lock();
            if(this->nodes[idx] == nullptr){
                this->nodes[idx] = new CharNode(rt, pos, leaf);
				lg.unlock();
            }else{
				lg.unlock();
                this->nodes[idx]->append(rt, pos, leaf);
            }
        }
    }else{
        this->nodes[idx]->append(rt, pos, leaf);
    }
}

void RouteTree::addAlias(const std::string& route, const std::string& alias){
    std::string rt, rx;
    RouteTree::parseRoute(route, rt, rx);
    
    RouteLeaf *leaf;
    
    unsigned char idx = (unsigned char)rt[0];
    if(this->nodes[idx] == nullptr || !this->nodes[idx]->find(rt, leaf))
        throw "TODO: make real exception. Base route not found!";
    
    std::map<std::regex*, CoreManager<CoreController>*>::iterator i;
    if((i = leaf->managers.find(this->regexs[rx])) == leaf->managers.end())
        throw "TODO: make real exception. Base route expression not found!";
    
    this->add(alias, i->second);
}

CoreManager<CoreController>* RouteTree::find(const std::string& uri, args_t& args_out){
    size_t pos = 0, lpos;
    unsigned char idx = (unsigned char)uri[pos];
    
    if(this->nodes[idx] == nullptr)
        return nullptr;
    
    std::string tmp;
    RouteLeaf*& leaf = this->nodes[idx]->find(uri, tmp);
    
    if(leaf == nullptr){
        return nullptr;
    }
    
    CoreManager<CoreController>* mgr = leaf->getManager(tmp);
    if(mgr != nullptr){
        while(tmp[pos] == '/') pos++;
        
        do{
            lpos = tmp.find('/', pos);
            if(lpos == std::string::npos){
                args_out.push_back(tmp.substr(pos, tmp.length() - pos));
                break;
            }
            
            args_out.push_back(tmp.substr(pos, lpos - pos));
            pos = ++lpos;
        }while(true);
    }
    
    return mgr;
}

RouteTree::~RouteTree(){
    for(unsigned short i = 0; i < CharNode::NodeArrayLength; i++){
        if(this->nodes[i] != nullptr)
            delete this->nodes[i]->value;
        delete this->nodes[i];
    }
    
    for(std::map<std::string, std::regex*>::iterator i = this->regexs.begin(); i != this->regexs.end(); i++){
        delete i->second;
    }
}
//End RouteTree