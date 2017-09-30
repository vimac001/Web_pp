/* 
 * File:   BasicManager.hpp
 * Author: Viktor
 *
 * Created on 7. Juli 2015, 21:57
 */

#ifndef COREMANAGER_HPP
#define	COREMANAGER_HPP

#include "CoreController.h"

namespace wpp{
    template<class T>
    class CoreManager{
        static_assert(std::is_base_of<CoreController, T>::value, "Type T is not a child of CoreController!");
        
    public:
        CoreManager(const bool& persistent = false) : mgrMutex(){
            this->controller = nullptr;
            this->persistent = persistent;
        }
        
        CoreManager(const CoreManager&& orig) : mgrMutex(){
            this->controller = orig.controller;
            this->persistent = orig.persistent;
        }
        
        int proceedRequest(Request& rq, Response& rp, const std::vector<std::string>& args)
		{
            //mgrMutex.lock();
            int status = this->doRequest(rq, rp, args);
            //mgrMutex.unlock();
            
            return status;
        }
        
        virtual ~CoreManager(){
            delete this->controller;
        }
        
        const bool& isPersistent(){
            return this->persistent;
        }
        
    protected:
        bool persistent;
        T *controller;
        
        std::mutex mgrMutex;
        
        virtual int doRequest(Request& rq, Response& rp, const std::vector<std::string>& args) = 0;
        
        virtual T *getController(){
            return this->controller;
        }
        
        virtual T *createController(Request *rq, Response *rp){
            delete this->controller;
            this->controller = new T(rq, rp);
            return this->controller;
        }
        
    private:
        CoreManager(const CoreManager& orig) = delete;
    };
}

#endif	/* COREMANAGER_HPP */

