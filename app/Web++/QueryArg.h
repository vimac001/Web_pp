/* 
 * File:   QueryArg.h
 * Author: vikto
 *
 * Created on 11. Juni 2015, 17:17
 */

#ifndef QUERYARG_H
#define	QUERYARG_H

#include "string.h"

namespace wpp{

    enum QueryArgType : unsigned char {
        Get = 2,
        Post = 8,
        Any = 10,
        None = 0
    };
    
    class QueryArg {
        friend class Request;
        
        friend class Request_old; //TODO: rem me
        
    public:
        struct FileInfo {
        public:
            std::string name;
            std::string path;
            std::size_t size;
            std::string mimeType;
        };
        
        QueryArg();
        QueryArg(const std::string& name, const std::string& value, const QueryArgType& type);
        QueryArg(const std::string& name, const std::string& fileName, const std::string& filePath, const std::string& fileType, const std::size_t& fileSize);
        QueryArg(const QueryArg& orig);
        
        const std::string& getName();
        const std::string& getValue();
        const QueryArgType& getType();
        
        bool isPost();
        bool isGet();
        
        bool isFile();
        const FileInfo& getFileInfo();
        
        virtual ~QueryArg();
        
    protected:
        void setValue(const std::string& value);
        void setType(const QueryArgType& type);
        
    private:
        std::string name;
        std::string value;
        QueryArgType type;
        
        FileInfo *fileInfo;
    };
}

#endif	/* QUERYARG_H */

