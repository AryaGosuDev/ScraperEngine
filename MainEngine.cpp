#include "MainEngine.h"

using namespace rapidjson ;
using namespace std ;

//host=localhost port=5432 dbname=mydb connect_timeout=10
DB_Hook::connectToDB (string _host, u_int32t _port , string _dbname, string _pw, string _dbusername ) {
	
	try {
		pqxx::connection tempc;
		const std::string connectionString = "host=" + _host + " port=" + to_string ( _port ) + " dbname=" + _dbname + " connect_timeout=10" ) ;
		
		c = tempc ( connectionString.c_str ()  ) ;
		
		if ( !c.is_open() ) throw "Can't open database." ;
	}
	catch ( ... ) {
		std::cerr << "Error in DB_Hook::connectToDB " << endl ;
	}	
}

DB_Hook::disconnetToDB () {
	try {
		c.diconnect() ;
	}
	catch (...) {
		std::cerr << "Error in DB_Hook::disconnetToDB " << endl ;
	}
}

bool DB_Hook::chooseOpenProxy(u_int32t & _id) {
	try {
		lock_guard<mutex> lk ( mutDB );
		pqxx::work tx{c};
		CURL *curl = curl_easy_init();
		for (auto [id, ip, port, used] : tx.query<int, std::string, int, bool>(
                "SELECT id, IP, port, used FROM Proxy_List where used = False")) {
					_id = id ; break ;	
        }
		return true ;
	}
	catch ( ... ) {
		std::cerr << "Error in DB_Hook::chooseOpenProxy " << endl ;
	}
}

bool DB_Hook::checkDBEntries() {
	
	try { 
	
		lock_guard<mutex> lk ( mutDB );
		
		CURL *curl;
		CURLcode res;
		curl_global_init(CURL_GLOBAL_DEFAULT);	
		curl = curl_easy_init();
		if(!curl)
			throw "CURL ERROR in DB_Hook::checkDBEntries()" ;	
		
		pqxx::work tx{c};
		
		std::vector<std::string> sitesToVerify ;
		std::vecotr<u_int32t>    proxiesToErase ;
		for (auto [site] : tx.query<std::string>(
                "SELECT site FROM Sitelist"))
					sitesToVerify.push_back ( site ) ;	
        
		for (auto [id, ip, port, used] : tx.query<int, std::string, int, bool>(
                "SELECT id, IP, port, used FROM Proxy_List where used = False")) {
					for ( auto & sites : sitesToVerify ) {
						curl_easy_setopt(curl, CURLOPT_URL, sites);
						curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
						res = curl_easy_perform(curl);
						
						if(res != CURLE_OK) {
							cerr << "curl_easy_perform() failed : " <<  curl_easy_strerror(res) << endl ;	
							proxiesToErase.push_back (id ) ;
						}
					}		
        }
		
		tx.query ( "DELETE FROM Proxy_List WHERE id IN ( " + 
					[&]() { std::string idList ; 
					        for ( auto & idVal : proxiesToErase) idList += idVal + "," ;
							return idList ; } + ")" ) ;
		
		curl_easy_cleanup(curl); curl_global_cleanup();
	}
	
	catch ( ... ) {
	}	
}
	
MainEngine::MainEngine() {
	
	try {
		
		ifstream ifs ( "config.json" );
		IStreamWrapper isw(ifs);
		
		Document d ;
		d.ParseStream(isw) ;
		
		td = new ThreadData () ;
		
		numberOfScrapeThreads = std::thread::hardware_concurrency() ;
		numberOfVerificationThreads = 1 ;
		
		td->DB_USERNAME = d["DBUSERNAME"];
		td->DB_PW       = d["DB_PW"];
		td->DB_LOCATION = d["DB_ADDRESS"];
		td->DP_PORT	   = d["DB_PORT"];
		td->DB_DBNAME   = d["DB_DBNAME"];
		
		td->domainToScrape = d["WebSiteToScrape"];
		td->threadWaitTime = d["WaitTimePerThread"];
		
		td=>dbHook = new DB_Hook() ;
		td->dbHook->connectToDB ( td.DB_LOCATION,td.DP_PORT, td.DB_DBNAME , td.DB_PW , td.DB_USERNAME ) ;
		
		td->verificationStart = false ;
		td->threadVerificationWaits.resize ( td.numberOfScrapeThreads ) ;
	}
	catch ( ... ) {
		std::cerr << "Error in MainEngine::MainEngine() " << endl ;
	}
}

void MainEngine::runEngine() {
	try {
		
		for ( size_t i = 0 ; i < td.numberOfScrapeThreads ; ++ i ) {
			ScrapeThread tempScraper ( td, i ) ;
			scrapers.push_back ( std::move ( tempScraper )) ;
			
			std::thread tempTd ( scrapers.back().run() ) ;
			scrapingThreads.push_back ( std::move ( tempTd ) ) ;	
		}
		
		for ( size_t i = 0 ; i < td.numberOfVerificationThreads ; ++ i ) {
			VerificationThread tempVerify ( td, i ) ;
			verifiers.push_back ( std::move ( tempVerify )) ;
			
			std::thread tempTd ( verifiers.back().run() ) ;
			verificationThreads.push_back ( std::move ( tempTd ) ) ;	
		}
		
		for ( size_t i = 0 ; i <  td.numberOfScrapeThreads ; ++ i )  
			scrapingThreads[i].join () ;
		
		
		for ( size_t i = 0 ; i <  td.numberOfVerificationThreads ; ++ i )  
			verificationThreads[i].join () ;
		
		std::cout <<  " Successfully ended process " << std::endl ;
		
	}
	catch ( ... ) {
		std::cerr << "Error in MainEngine::runEngine" << std::endl ;
		
	}
}