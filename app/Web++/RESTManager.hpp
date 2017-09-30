/* 
 * File:   RESTManager.hpp
 * Author: Viktor
 *
 * Created on 5. Juli 2015, 11:58
 */

#ifndef RESTMANAGER_HPP
#define	RESTMANAGER_HPP

#include <type_traits>

#include "Request.h"
#include "Response.h"

#include "CoreManager.hpp"
#include "RESTController.h"

namespace wpp
{
    template<class T>
    class RESTManager : public CoreManager<T> {
        static_assert(std::is_base_of<RESTController, T>::value, "Type T is not a child of RESTController!");
        
    public:
        RESTManager(const bool& persistent = false) : CoreManager<T>(persistent){
        }
        
        RESTManager(const RESTManager&& orig) : CoreManager<T>((CoreManager<T>&&)orig) {
            
        }
        
    protected:
        virtual int doRequest(Request& rq, Response& rp, const std::vector<std::string>& args){
            T *c;
            if(this->controller == NULL || !this->persistent){
                c = this->createController(&rq, &rp);
            }else{
                c = this->getController();
                ((RESTController*)c)->updateRequestResponsePointers(&rq, &rp);
            }
            
            switch(rq.getMethod()){
                case RequestMethod::GET:
                    return ((RESTController*)c)->GET(args);
                
                case RequestMethod::PUT:
                    return ((RESTController*)c)->PUT(args);
                    
                case RequestMethod::POST:
                    return ((RESTController*)c)->POST(args);
                    
                case RequestMethod::DELETE:
                    return ((RESTController*)c)->DELETE(args);
                    
                default:
                    //TODO: Throw exception
                    rp.out << "Undefined request method!";
                    return 1;
            }
        }
        
    private:
        RESTManager(const RESTManager& orig) = delete;
    };
}

#endif	/* RESTMANAGER_HPP */

