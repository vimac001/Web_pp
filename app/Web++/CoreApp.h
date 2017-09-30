/* 
 * File:   CoreApp.h
 * Author: Viktor
 *
 * Created on 21. Juni 2015, 17:47
 */

#ifndef COREAPP_H
#define	COREAPP_H

#include "Request.h"
#include "Response.h"

namespace wpp{
    class CoreApp{
    public:
        CoreApp();
        CoreApp(int sock, int flags);
        CoreApp(const char *path, int backlog, int flags = 0);
        
        const unsigned int DefaultThreadSleepDuration = 128; //ms
        
        /**
         * Runs the FastCGI app.
         * @param threads_count Ammount of running threads. (Default: 0 = As many threads as cores on running machine.)
         */
        void run(const unsigned int& threads_count = 0);
        
        /**
         * Stop all running threads.
         * (Bug: No non-blocking FCGX_Accept_r!)
         * @param block Wait until all threads are stopped.
         */
        void stop();
        
        const bool& isRunning();
        
        const unsigned int& getThreadsAmmount();
        
        virtual ~CoreApp();
        
    protected:
        int sock;
        int flags;
        int backlog;
        const char *path;
        unsigned int tcount;
        bool running;
        std::mutex acceptRequestMutex;
        
        std::vector<std::thread> threads;
        
        virtual void onNewRequest(Request& rq, Response& rp) = 0;
        
        void t_run();
        
    private:
        CoreApp(const CoreApp& orig) = delete;
        CoreApp(const CoreApp&& orig) = delete;
    };
}

#endif	/* COREAPP_H */

