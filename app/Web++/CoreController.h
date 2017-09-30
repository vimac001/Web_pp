/* 
 * File:   BasicController.h
 * Author: Viktor
 *
 * Created on 5. Juli 2015, 10:05
 */

#ifndef CORECONTROLLER_H
#define	CORECONTROLLER_H

#include "Request.h"
#include "Response.h"

namespace wpp{
    class CoreController{
        friend class MainApp;
        template<class T> friend class CoreManager;
        
    public:
        CoreController(Request *rq, Response *rp);
        
        
        
        virtual ~CoreController();
        
    protected:
        Request *request;
        Response *response;
        
        Request& getRequest();
        Response& getResponse();
        
        std::ostringstream& out(std::string data);
        std::ostringstream& err(std::string data);
        
        std::ostringstream& out();
        std::ostringstream& err();
        
        Request& rq();
        Response& rp();
        
        virtual void updateRequestResponsePointers(Request *rq, Response *rp);
        
    private:
        CoreController(const CoreController& orig) = delete;
    };
}

#endif	/* CORECONTROLLER_H */

