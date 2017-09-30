/* 
 * File:   MethodManager.hpp
 * Author: Viktor
 *
 * Created on 7. Juli 2015, 22:28
 */

#ifndef METHODMANAGER_HPP
#define	METHODMANAGER_HPP

#include <type_traits>

#include "Request.h"
#include "Response.h"

#include "CoreManager.hpp"
#include "MethodController.h"

namespace wpp
{
    template<class T>
    class MethodManager : public CoreManager<T> {
        static_assert(std::is_base_of<MethodController, T>::value, "Type T is not a child of MethodController!");
        
        friend class MainApp;
        
    public:
        MethodManager(int(T::*method)(const args_t&), const bool& persistent) : CoreManager<T>(persistent){
            this->method = method;
            this->shared = false;
            this->sharedController = nullptr;
        }
        
        MethodManager(int(T::*method)(const args_t&), MethodController**& sharedController) : CoreManager<T>(true){
            this->method = method;
            this->shared = true;
            
            this->sharedController = sharedController;
        }
        
        MethodManager(const MethodManager&& orig) : CoreManager<T>((CoreManager<T>&&)orig) {
            this->shared = orig.shared;
            this->sharedController = orig.sharedController;
        }
        
        const bool& isShared(){
            return this->shared;
        }
        
    protected:
        int(T::*method)(const args_t&);

        bool shared;
        MethodController **sharedController;
        
        virtual int doRequest(Request& rq, Response& rp, const std::vector<std::string>& args){
            T *c;
            if(this->shared){
                c = this->getSharedController(rq, rp);
            }else if(this->controller == nullptr || !this->persistent){
                c = this->createController(&rq, &rp);
            }else{
                c = this->getController();
                ((MethodController*)c)->updateRequestResponsePointers(&rq, &rp);
            }
            
            return ((*c).*(this->method))(args);
        }
        
        T* getSharedController(Request& rq, Response& rp){
            MethodController* c = *(this->sharedController);
            if(c == nullptr){
                c = (MethodController*)this->createController(&rq, &rp);
                *(this->sharedController) = c;
            }else{
                ((MethodController*)c)->updateRequestResponsePointers(&rq, &rp);
            }
            
            return (T*)c;
        }
    private:
        MethodManager(const MethodManager& orig) = delete;
    };
}

#endif	/* METHODMANAGER_HPP */

