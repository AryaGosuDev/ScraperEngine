#include "ThreadHandler.h"

using namespace std ;

void ScrapeThread::run () {
	
	size_t DBRow = 0 ;
	td->dbHook->chooseOpenProxy ( DBRow ) ;
	
	try {
		
		while ( 1 ) {
			
			if ( td->verificationStart ) {
				td->threadVerificationWaits[ID]->true ;
				
				std::unique_lock<std::mutex> lk ( td->verificationEndMutex ) ;
				verificationCond.wait ( lk , [] { return  verificationEnd ; } ;
				td->dbHook->chooseOpenProxy ( DBRow ) ;
				lk.unlock() ;
				continue ;
			}
			
			string URLToScrape ;
			if ( td->ThreadSqueue->try_pop ( URLToScrape ) ) {
				
				CURL *curl;
				CURLcode res;
				curl_global_init(CURL_GLOBAL_DEFAULT);	
				curl = curl_easy_init();
				
				if(!curl)
					throw "CURL ERROR in DB_Hook::checkDBEntries()" ;

				curl_easy_setopt(curl, CURLOPT_URL, sites);
				curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
				res = curl_easy_perform(curl);
						
				if(res != CURLE_OK) {
					cerr << "curl_easy_perform() failed : " <<  curl_easy_strerror(res) << endl ;	
					proxiesToErase.push_back (id ) ;
				}
				else {
					struct MyData thisData;
					
					curl_easy_setopt(curl, CURLOPT_READDATA, &this);
 
					curl_easy_perform(curl);
				}
			}
		}
	}
	catch ( ... ) {
		std::cerr << "Error in ScrapeThread::run " << end ;	
	}
}

void VerificationThread::run () {
	try {
		
		while ( 1 )  {
			
			using std::chrono::system_clock;
			std::this_thread::sleep_until (system_clock::from_time_t (mktime(ptm)));
			
			td->verificationStart = true ;
			
			for ( int i = 0 ; i < td.threadVerificationWaits.size() ; ++ i )  {
				while ( !td->threadVerificationWaits[i] ) ;
			}
			
			td->dbHook->checkDBEntries() ;
			
			std::unique_lock < std::mutex > lk ( td->verificationEndMutex ) ;
			verificationEnd = true ;
			td->verificationStart = false ;
			verificationCond.notify_all () ;
		}
	}
	catch ( ... ) {
		std::cerr << "Error in VerificationThread::run " << end ;
	}
}