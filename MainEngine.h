#include <thread>
#include <condition_variable>
#include <mutex>
#include <memory>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ThreadHandler.h"
#include <vector>
#include <pqxx/pqxx>
#include "ConcurrentQueue.h"
#include <curl/curl.h>

#define JSON_CONFIG_PATH "config.json"

class DB_Hook {
	
	private :
		pqxx::connection c;
		mutable std::mutex mutDB ;
	public :
		DB_Hook () = default ;
		
		bool connectToDB (std::string, u_int32t, std::string, std::string, std::string ) ;
		bool disconnetToDB () ;
		bool chooseOpenProxy(u_int32t &) ;
		bool checkDBEntries() ;
};

class MainEngine {
	
	struct ThreadData  {
		
		DB_Hook * dbHook ;
		ThreadSafeConcurrentQueue * ThreadSqueue ;
		std::string domainToScrape ;
		u_int32t threadWaitTime ;
		std::string DB_USERNAME ;
		std::string DB_PW ;
		std::string DB_LOCATION ;
		std::string DB_PORT ;
		std::string DB_DBNAME ;
		bool verificationEnd ;
		std::atomic<bool> verificationStart ;
		std::condition_variable verificationCond;
		std::vector<std::atomic<bool>> threadVerificationWaits = {} ;
		std::mutex verificationEndMutex ;
	};

	public :
		MainEngine() {}
		~MainEngine() ;
		void runEngine () ;
		
	private :
		std::string URLToScrape ;
		
		std::vector<std::Thread> scrapingThreads = {} ;
		std::vector<ScrapeThread> scrapers = {} ;
		
		std::vector<std::Thread> verificationThreads = {} ;
		std::vector<VerificationThread> verifiers = {} ;
		
		int numberOfScrapeThreads ;
		int numberOfVerificationThreads ;
		
		ThreadData * td ;
};

