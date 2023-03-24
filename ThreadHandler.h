#include <string>
#include <memory>
#include <queue>
#include <thread>
#include "ConcurrentQueue.h"
#include "MainEngine.h"
#include <iostream>       
#include <iomanip>       
#include <thread>       
#include <chrono>       
#include <ctime>       

class ThreadHandler {
	public:
		ThreadHandler () = default ;
		virtual std::string Handle(std::string request) = 0;
		virtual void run(  ) = 0 ;
		
	private :
	
	protected :
		u_int32t ID;
		MainEngine::ThreadData * threadData ;
};

class AbstractThreadHandler : public ThreadHandler {
	private:
	public:
		AbstractHandler() {}
		virtual std::string Handle(std::string request) override {
			return {};
		}
		virtual void run () {}
};

class ScrapeThread final : public AbstractThreadHandler {
	private :
	public :
		ScrapeThread() = default ;
		ScrapeThread ( MainEngine::ThreadData * _threadData, u_int32t _ID  ) : threadData ( _threadData ), ID ( _ID )  {}
		virtual void run () override ;
};

class VerificationThread final: public AbstractThreadHandler {
	private :
	public :  
		VerificationThread () = default ;
		VerificationThread ( MainEngine::ThreadData * _threadData  ) threadData ( _threadData ) { ID = 0 ;}
		virtual void run () override;
};