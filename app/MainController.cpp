/* 
 * File:   MainController.cpp
 * Author: Viktor
 * 
 * Created on 7. Juli 2015, 22:15
 */

#include "MainController.h"

std::ostringstream MainController::log;

MainController::MainController(Request *rq, Response *rp) : MethodController(rq, rp){
    this->c = 0;
}

int MainController::test(const args_t& args){
    out() << "<h1>Hello world!</h1>\n";
    out() << std::to_string(this->c++);

	out() << "<br>";

	out() << rq().getQueryArg("b") << "<br><br><hr><br><br>";
	
	const skvmap queryArgs = rq().getQueryArgs();

	for (skvmap::const_iterator i = queryArgs.cbegin(); i != queryArgs.cend(); i++)
	{
		out() << i->first << "=" << i->second << "<br>";
	}
	
    return 0;
}

int MainController::test2(const args_t& args){
    rp().setContentType("text/plain");
    out() << "Hello world 2!\n";
    out() << std::to_string(this->c++);
    
//    out() << "\n\n\n";
//    
//    for(args_t::const_iterator i = args.begin(); i != args.end(); i++){
//        out() << "    " << (*i) << "\n";
//    }
//    
////    out() << "\n\n\nLog:\n" << MainController::log.str();
//    
//    out() << "\n" << rq().getServerProtocol();
    
    return 0;
}

MainController::~MainController(){ }

