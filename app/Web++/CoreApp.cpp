/* 
 * File:   CoreApp.cpp
 * Author: Viktor
 * 
 * Created on 21. Juni 2015, 17:47
 */

#include "CoreApp.h"

using namespace wpp;

CoreApp::CoreApp() : acceptRequestMutex(), threads() {
	FCGX_Init();
    this->sock = 0;
    this->flags = 0;
    this->backlog = 0;
    this->path = nullptr;
    
    this->running = false;
    this->tcount = 1;
}

CoreApp::CoreApp(int sock, int flags) : acceptRequestMutex(), threads() {
	FCGX_Init();
    this->sock = sock;
    this->flags = flags;
    this->backlog = 0;
    this->path = nullptr;
    
    this->running = false;
    this->tcount = 1;
}

CoreApp::CoreApp(const char *path, int backlog, int flags) : acceptRequestMutex(), threads() {
	FCGX_Init();
    this->sock = FCGX_OpenSocket(path, backlog);
    this->flags = flags;
    this->backlog = backlog;
    this->path = path;
    
    this->running = false;
    this->tcount = 1;
}


void CoreApp::t_run(){
    FCGX_Request request;
    FCGX_InitRequest(&request, this->sock, this->flags);

    std::chrono::milliseconds slp(CoreApp::DefaultThreadSleepDuration);

    int rc;

    while(this->isRunning()){
		{
			std::lock_guard<std::mutex> lg(this->acceptRequestMutex);
			rc = FCGX_Accept_r(&request);
		}

        if(rc >= 0){
            Request rq(request);
            Response rp(request);

            rp.hostPointer = rq.hostPointer;

            rp.request = &rq;
            rq.response = &rp;

            this->onNewRequest(rq, rp);

			rq.f_envDataReady.wait();

            FCGX_FPrintF(request.out, rp.getRawResponse().c_str());
            FCGX_FPrintF(request.err, rp.err.str().c_str());

			FCGX_Finish_r(&request);
        }else std::this_thread::sleep_for(slp);


    }
}

void CoreApp::run(const unsigned int& threads_count){
    unsigned int tc = (threads_count == 0) ? std::thread::hardware_concurrency() : threads_count;
    if(tc == 0) tc = 1;
    this->tcount = tc;

    FCGX_Init();

    running = true;

    for(; tc > 0; tc--){
        this->threads.push_back(std::thread(&CoreApp::t_run, this));
    }
}

void CoreApp::stop(){
    running = false;

    for(std::vector<std::thread>::iterator i = this->threads.begin(); i != this->threads.end(); i++){
        i->detach();
        if(i->joinable()){
            i->join();
        }
    }
//    
//    if(block){
//        for(std::vector<std::thread>::iterator i = this->threads.begin(); i != this->threads.end(); i++){
//            if(i->joinable())
//                i->join();
//        }
//    }
}

const bool& CoreApp::isRunning(){
    return this->running;
}

const unsigned int& CoreApp::getThreadsAmmount(){
    return this->tcount;
}

CoreApp::~CoreApp() {
    
}