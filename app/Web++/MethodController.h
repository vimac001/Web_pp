/* 
 * File:   MethodController.h
 * Author: Viktor
 *
 * Created on 7. Juli 2015, 22:30
 */

#ifndef METHODCONTROLLER_H
#define	METHODCONTROLLER_H

#include "CoreController.h"

namespace wpp{
    class MethodController : public CoreController {
        friend class MainApp;
        template<class T> friend class MethodManager;
        
    public:
        MethodController(Request *rq, Response *rp);
        
        
        virtual ~MethodController();
        
    protected:
        
        
    private:
        MethodController(const MethodController& orig) = delete;
    };
}

#endif	/* METHODCONTROLLER_H */

