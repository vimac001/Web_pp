/* 
 * File:   RESTController.cpp
 * Author: Viktor
 * 
 * Created on 5. Juli 2015, 10:05
 */

#include "RESTController.h"
#include "RESTManager.hpp"

using namespace wpp;

RESTController::RESTController(Request *rq, Response *rp) : CoreController(rq, rp){
    
}

int RESTController::GET(const args_t& args){
    //TODO: Throw exception
    return 0;
}

int RESTController::POST(const args_t& args){
    //TODO: Throw exception
    return 0;
}

int RESTController::PUT(const args_t& args){
    //TODO: Throw exception
    return 0;
}

int RESTController::DELETE(const args_t& args){
    //TODO: Throw exception
    return 0;
}

RESTController::~RESTController(){
    
}

