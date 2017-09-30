/* 
 * File:   SiteController.cpp
 * Author: Viktor
 * 
 * Created on 5. Juli 2015, 10:54
 */

#include "SiteController.h"

SiteController::SiteController(Request *rq, Response *rp) 
  : RESTController(rq, rp) {
}

int SiteController::GET(const args_t& args){
    //rq() ist equivalent zu this->request
    if(!rq()->hasCookie("test")){
        rp()->setCookie("test", "cookieSet");
    }
    
    out() << "Working with cookies example.";
    
    return 0;
}

int SiteController::POST(const args_t& args){
    rp().setStatus(ResponseStatus.MethodNotAllowed);
    out() << "Hi!";
    
    return 0;
}

SiteController::~SiteController(){}

