/* 
 * File:   SiteController.h
 * Author: Viktor
 *
 * Created on 5. Juli 2015, 10:54
 */

#ifndef SITECONTROLLER_H
#define	SITECONTROLLER_H

#include "Web++/RESTController.h"

using namespace wpp;

class SiteController : public RESTController{
public:
    SiteController(Request *rq, Response *rp);
    
    virtual int GET(const args_t& args);
    virtual int POST(const args_t& args);
    
    virtual ~SiteController();
    
protected:
    int c;
    
private:
    SiteController(const SiteController& orig) = delete;
};

#endif	/* SITECONTROLLER_H */

