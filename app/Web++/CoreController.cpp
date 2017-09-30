/* 
 * File:   BasicController.cpp
 * Author: Viktor
 * 
 * Created on 7. Juli 2015, 21:34
 */

#include "CoreController.h"
#include "CoreApp.h"

using namespace wpp;

CoreController::CoreController(Request *rq, Response *rp){
    this->request = rq;
    this->response = rp;
}

std::ostringstream& CoreController::out(std::string data){
    this->response->out << data;
    return this->response->out;
}

std::ostringstream& CoreController::err(std::string data){
    this->response->err << data;
    return this->response->err;
}

std::ostringstream& CoreController::out(){
    return this->response->out;
}

std::ostringstream& CoreController::err(){
    return this->response->err;
}

void CoreController::updateRequestResponsePointers(Request* rq, Response* rp){
    this->request = rq;
    this->response = rp;
}

Request& CoreController::rq(){
    return *(this->request);
}

Response& CoreController::rp(){
    return *(this->response);
}

CoreController::~CoreController(){}

