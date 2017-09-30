/* 
 * File:   ObjectDispatcher.cpp
 * Author: Viktor
 * 
 * Created on 5. Juli 2015, 09:57
 */

#include "MainApp.hpp"

using namespace wpp;

MainApp::MainApp() : CoreApp(), routes() {
}

MainApp::MainApp(int sock, int flags) : CoreApp(sock, flags), routes() {
}

MainApp::MainApp(const char *path, int backlog, int flags) : CoreApp(path, backlog, flags), routes() {
}

void MainApp::addRouteAlias(const std::string& route, const std::string& alias){
    this->routes.addAlias(route, alias);
}

void MainApp::onNewRequest(Request& rq, Response& rp){
    args_t args;
    
    try{
        CoreManager<CoreController> *mgr = this->routes.find(rq.getDocumentUri(), args);

        if(mgr == NULL){
            //rp.setStatus(ResponseStatus::NotFound);
            rp.setContentType("text/plain");
            rp.out << "Error: Bad DOCUMENT_URI!\n" << std::endl;
			rp.out << "Thread-Id: " << std::this_thread::get_id() << std::endl;
        }else{
            mgr->proceedRequest(rq, rp, args);
        }
    }catch(const std::string& ex){
        rp.setStatus(ResponseStatus::InternalServerError);
        rp.setContentType("text/plain");
        rp.out.str("");
        rp.out << "500 Internal Server Error >: " << ex << std::endl;
    }
}

MainApp::~MainApp(){
    for(std::list<MethodController*>::iterator i = this->sharedController.begin(); i != this->sharedController.end(); i++){
        delete (*i);
    }
}

