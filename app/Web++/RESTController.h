/* 
 * File:   RESTController.h
 * Author: Viktor
 *
 * Created on 5. Juli 2015, 10:05
 */

#ifndef RESTCONTROLLER_H
#define	RESTCONTROLLER_H

#include "CoreController.h"

namespace wpp{
    class RESTController : public CoreController {
        friend class MainApp;
        template<class T> friend class RESTManager;
        
    public:
        RESTController(Request *rq, Response *rp);
        
        virtual int GET(const args_t& args);
        virtual int PUT(const args_t& args);
        virtual int POST(const args_t& args);
        virtual int DELETE(const args_t& args);
        
        virtual ~RESTController();
        
    protected:
        
    private:
        RESTController(const RESTController& orig) = delete;
    };
}

#endif	/* RESTCONTROLLER_H */

