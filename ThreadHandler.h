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
		virtual void run( MainEngine::ThreadData , u_int32t ) = 0 ;
		
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
		virtual void run ( MainEngine::ThreadData , u_int32t ) {}
};

class ScrapeThread final : public AbstractThreadHandler {
	private :
	public :
		virtual void run ( MainEngine::ThreadData , u_int32t ) override ;
};

class VerificationThread final: public AbstractThreadHandler {
	private :
	public :
		virtual void run ( MainEngine::ThreadData , u_int32t ) override;
};