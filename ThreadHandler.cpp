#include "ThreadHandler.h"

void ScrapeThread::run ( MainEngine::ThreadData * td , u_int32t _id ) {
	
	
}

void VerificationThread::run ( MainEngine::ThreadData * td , u_int32t _id ) {
	
	using std::chrono::system_clock;
	std::this_thread::sleep_until (system_clock::from_time_t (mktime(ptm)));
	
	
	
	
}