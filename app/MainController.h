/* 
 * File:   MainController.h
 * Author: Viktor
 *
 * Created on 7. Juli 2015, 22:15
 */

#ifndef MAINCONTROLLER_H
#define	MAINCONTROLLER_H

#include "Web++/MethodController.h"

using namespace wpp;

class MainController : public MethodController {
public:
    MainController(Request *rq, Response *rp);
    
    static std::ostringstream log;
    
    int test(const args_t& args);
    int test2(const args_t& args);
    
    virtual ~MainController();
private:
    MainController(const MainController& orig) = delete;
    
    unsigned int c;

};

#endif	/* MAINCONTROLLER_H */

