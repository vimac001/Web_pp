#include "app/Web++/MainApp.hpp"
#include "app/SiteController.h"
#include "app/MainController.h"

using namespace std;

int main(int argc, char *argv[], char **envp){
	//Hier wird der Service (FastCGI) an eine Addresse/Port gebunden.
#ifdef _WIN32
	wpp::MainApp app("localhost:1337", 500);
#else
	wpp::MainApp app("127.0.0.1:1337", 500);
#endif

	//Hier werden die Routen hinzugefügt und mit den aufzurufenden Methoden verknöpft
	//Method Controller (Method Dispatcher), der Method Dispatcher ist eine direkte Zuweisung von Route zu Methode (Wird bei jeder REQUEST_METHOD durchgeleitet)
	app.beginAddingRoutes<MainController>();
	app.addRoute<MainController>("/", &MainController::test);
	app.addRoute<MainController>("/{a/.+}", &MainController::test2);
	app.endAddingRoutes();


	//REST Controller (Object Dispatcher), der Object Dispatcher arbeitet nach dem REST Prinzip. Die Route ist mit dem Objekt verknöpft und je nach REQUEST_METHOD wird eine andere Methode des Objekts ausgeführt.
	app.addRoute<SiteController>("/site", true); //Das true steht für Persistent, das bedeutet, dass das Objekt ein Mal erzeugt wird und bei jedem weiteren Aufrum wiederverwendet wird. (Alle Zustände bleiben erhalten)

	//Alias Routes
	app.addRouteAlias("/site", "/test/site"); //Ein Alias für Routen
	//    app.addRouteAlias("/{.+}", "/main{.+}");

	//Ab dieser Stelle wird der Service gestartet, wenn keine Parameter an run(), dann werden so viele Threads erstellt wie Kerne in der CPU
	app.run(); //Ist Non-Blocking
        
	//Sonstigere Code, z.B. CLI-Kommandos wie in dem Fall exit
    cout << "App(" << app.getThreadsAmmount() << ") is running..." << endl;
        
    string str;
    while(app.isRunning()){
        cout << "Cmd: ";
        cin >> str;
            
        if(str == "exit"){
            cout << "Closing app ..." << endl;
            app.stop();
        }else{
            cout << "Unknown command " << str << ", use 'exit' to close the app." << endl;
        }
    }
        
    cout << "App closed." << endl;
        
	return 0;
}