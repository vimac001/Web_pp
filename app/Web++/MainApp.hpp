/* 
 * File:   MainApp.hpp
 * Author: Viktor
 *
 * Created on 5. Juli 2015, 09:57
 */

#ifndef MAINAPP_HPP
#define	MAINAPP_HPP

#include <type_traits>
#include <memory>

#include "CoreApp.h"
#include "RESTController.h"
#include "RESTManager.hpp"
#include "MethodManager.hpp"

#include "RouteTree.h"

#include <iostream>

namespace wpp{
    class MainApp : public CoreApp{
        
    public:
        MainApp();
        MainApp(int sock, int flags);
        MainApp(const char *path, int backlog, int flags = 0);
        
        template<class Controller>
        void beginAddingRoutes(){
            static_assert(std::is_base_of<MethodController, Controller>::value, "Type Controller is not a child of MethodController!");
            
            this->sharedController.push_back(new Controller(nullptr, nullptr));
            this->tmpController = &(this->sharedController.back());
            
            this->addingRoutes = true;
        }
        
        template<class Controller>
        void addRoute(const std::string& route, int(Controller::*method)(const args_t&)){
            MethodManager<Controller> *mgr;
            
            if(this->addingRoutes){
                mgr = new MethodManager<Controller>(method, this->tmpController);
            }else{
                mgr = new MethodManager<Controller>(method, false);
            }
            
            this->routes.add(route, (CoreManager<CoreController>*)mgr);
        }
        
        void endAddingRoutes(){
            this->addingRoutes = false;
            this->tmpController = nullptr;
        }
        
        template<class Controller>
        void addRoute(const std::string& route, const bool& persistent){
            RESTManager<Controller> *mgr = new RESTManager<Controller>(persistent);
            this->routes.add(route, (CoreManager<CoreController>*)mgr);
        }
        
        template<class Controller>
        void addRoute(const std::string& route){
            RESTManager<Controller> *mgr = new RESTManager<Controller>(true);
            this->routes.add(route, (CoreManager<CoreController>*)mgr);
        }
        
        template<class Controller>
        void addRoute(const std::string& route, int(Controller::*method)(const args_t&), const bool& persistent){
            MethodManager<Controller> *mgr = new MethodManager<Controller>(method, persistent);
            this->routes.add(route, (CoreManager<CoreController>*)mgr);
        }
        
        
        void addRouteAlias(const std::string& route, const std::string& alias);
        
        virtual ~MainApp();
        
    protected:
        bool addingRoutes = false;
        MethodController **tmpController = nullptr;
        
        std::list<MethodController*> sharedController;
        
        RouteTree routes;
        
        virtual void onNewRequest(Request& rq, Response& rp);
        
    private:
        MainApp(const MainApp& orig) = delete;
    };
}

#endif	/* MAINAPP_HPP */

