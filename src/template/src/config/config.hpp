#ifndef h6f587de492dd11eb8c76e7a5ea34b743
#define h6f587de492dd11eb8c76e7a5ea34b743
#include"./config.hpp"
#include<string>
#include"crow/logging.h"
namespace App{
	namespace Config{
		const std::string default_host{"0.0.0.0"};
		const int default_port{8080};
		const crow::LogLevel default_loglevel{crow::LogLevel::Info};
		const int default_concurrency{8};
		class Config{
			public:
				Config();
				~Config();
				std::string get_host();
				int get_port();
				int get_concurrency();
				crow::LogLevel get_loglevel();
			private:
				std::string host;
				int port;
				crow::LogLevel loglevel;
				int concurrency;
		};
		extern Config config;
	}
}
#endif
