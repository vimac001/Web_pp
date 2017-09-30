/* 
 * File:   RouteTree.h
 * Author: Viktor
 *
 * Created on 10. Juli 2015, 15:22
 */

#ifndef ROUTETREE_HPP
#define	ROUTETREE_HPP

#include <map>
#include <regex>
#include <string>
#include <vector>

#include <mutex>

#include "definitions.hpp"
#include "CoreManager.hpp"

namespace wpp{
    class RouteLeaf{
        friend class RouteTree;
    public:
        RouteLeaf();
        void addManager(std::regex*& rx, CoreManager<CoreController>*& mgr);
        CoreManager<CoreController>* getManager(const std::string& subject);
        CoreManager<CoreController>* getLastManager();
        virtual ~RouteLeaf();
        
        RouteLeaf& operator +=(const RouteLeaf& rhs);
    protected:
        std::map<std::regex*, CoreManager<CoreController>*> managers;
        CoreManager<CoreController>* lmgr;
        
        std::mutex writeLock;
    };
    
    
    class CharNode{
        friend class RouteTree;
        
    public:
        static const unsigned short NodeArrayLength;
        
        CharNode(const std::string& str, std::size_t& pos, RouteLeaf*& value);
           
        void append(const std::string& str, std::size_t& pos, RouteLeaf*& value);
        RouteLeaf*& find(const std::string& str, std::string& out);
        
        bool find(const std::string& str, RouteLeaf*& out);
        
        ~CharNode();
        
    protected:
        RouteLeaf* value;

        std::size_t level;
        std::vector<CharNode*> nodes;
        
        std::mutex appendLock;
        std::mutex writeLock;
    };
    
    class RouteTree{
    public:
        static const std::regex::flag_type RegexFlag;
        
        RouteTree();
        
        RouteTree(const RouteTree&& orig);
        
        void add(const std::string& route, CoreManager<CoreController>* mgr);
        void addAlias(const std::string& route, const std::string& alias);
        
        CoreManager<CoreController>* find(const std::string& uri, args_t& args_out);
        
        virtual ~RouteTree();
        
    protected:
        std::vector<CharNode*> nodes;
        std::map<std::string, std::regex*> regexs;
        
        std::mutex appendLock;
        
        static void parseRoute(const std::string& fullRoute, std::string& route, std::string& regex);
        std::regex*& getRegexObject(const std::string& pattern);
        
    private:
        RouteTree(const RouteTree& orig) = delete;
    
    };
}

#endif	/* ROUTETREE_HPP */

