/* 
 * File:   QueryArg.cpp
 * Author: Viktor Machnik
 * 
 * Created on 11. Juni 2015, 17:17
 */

#include "QueryArg.h"
#include "Request.h"
#include "RouteTree.h"

using namespace wpp;

QueryArg::QueryArg() :
    value()
{
    this->type = QueryArgType::None;
    this->fileInfo = nullptr;
}

QueryArg::QueryArg(const std::string& name, const std::string& value, const QueryArgType& type){
    this->name = name;
    this->type = type;
    this->value = value;
    
    this->fileInfo = nullptr;
}

QueryArg::QueryArg(const std::string& name, const std::string& fileName, const std::string& filePath, const std::string& fileType, const std::size_t& fileSize) : 
    value()
{
    this->name = name;
    this->type = QueryArgType::Post;
    
    this->fileInfo = new FileInfo();
    this->fileInfo->name = fileName;
    this->fileInfo->path = filePath;
    this->fileInfo->size = fileSize;
    this->fileInfo->mimeType = fileType;
}

QueryArg::QueryArg(const QueryArg& orig) {
    this->name = orig.name;
    this->value = orig.value;
    this->type = orig.type;
    
    if(orig.fileInfo != nullptr){
        this->fileInfo = new FileInfo();
        this->fileInfo->name = orig.fileInfo->name;
        this->fileInfo->path = orig.fileInfo->path;
        this->fileInfo->size = orig.fileInfo->size;
        this->fileInfo->mimeType = orig.fileInfo->mimeType;
    }
}

bool QueryArg::isPost(){
    return (this->type & QueryArgType::Post) == QueryArgType::Post;
}

bool QueryArg::isGet(){
    return (this->type & QueryArgType::Get) == QueryArgType::Get;
}

bool QueryArg::isFile(){
    return (this->fileInfo != nullptr);
}

const std::string& QueryArg::getName(){
    return this->name;
}

const std::string& QueryArg::getValue(){
    return this->value; 
}

const QueryArgType& QueryArg::getType(){
    return this->type;
}

const QueryArg::FileInfo& QueryArg::getFileInfo(){
    return *(this->fileInfo);
}

void QueryArg::setValue(const std::string& value){
    this->value = value;
}

void QueryArg::setType(const QueryArgType& type){
    this->type = type;
}

QueryArg::~QueryArg() {
    delete this->fileInfo;
}

