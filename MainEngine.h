#include <thread>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ThreadHandler.h"
#include <vector>

#define JSON_CONFIG_PATH "config.cfg"

struct MainEngine {

	MainEngine() {}
	bool setUpEngine () ;
	void runEngine () ;

	std::string URLToScrape ;
	std::vector<ScrapeThread> scrapeThreads ;
	std::vector<VerificationThread> verificationThreads ;
};

