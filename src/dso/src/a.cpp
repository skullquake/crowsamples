#include"./config/config.hpp"
#include<iostream>
#include<sstream>
#include<vector>
#include<chrono>
#include<random>
#include<thread>
#include<chrono>
#include<atomic>
#include<mutex>
#include"crow/crow.h"
#include"middleware/dso.hpp"
#include"dso/dso.hpp"
std::string stest="lorem";
int itest=42;
float ftest=4.2;
int main(int argc,char*argv[]){
	crow::App<App::Middleware::MWDso>app;
	//--------------------------------------------------------------------------------
	//Obtain logging level
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/loglevel")([&app](){
		//curl http://localhost:8080/loglevel
		crow::json::wvalue j;
		switch(crow::logger::get_current_log_level()){
			case crow::LogLevel::Debug:
				j["loglevel"]="Debug";
				break;
			case crow::LogLevel::Info:
				j["loglevel"]="Info";
				break;
			case crow::LogLevel::Warning:
				j["loglevel"]="Warning";
				break;
			case crow::LogLevel::Error:
				j["loglevel"]="Error";
				break;
			case crow::LogLevel::Critical:
				j["loglevel"]="Critical";
				break;
			default:
				j["loglevel"]="Invalid";
				break;
		}
		return j;
	});
	//--------------------------------------------------------------------------------
	//Set logging level
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/loglevel/<string>")([&app](std::string level){
		//curl http://localhost:8080/loglevel/Debug
		//curl http://localhost:8080/loglevel/Info
		//curl http://localhost:8080/loglevel/Warning
		//curl http://localhost:8080/loglevel/Error
		//curl http://localhost:8080/loglevel/Critical
		crow::json::wvalue j;
		j["message"]="loglevel changed";
		      if(level=="Debug"){
		      app.loglevel(crow::LogLevel::Debug);
		}else if(level=="Info"){
		      app.loglevel(crow::LogLevel::Info);
		}else if(level=="Warning"){
		      app.loglevel(crow::LogLevel::Warning);
		}else if(level=="Error"){
		      app.loglevel(crow::LogLevel::Error);
		}else if(level=="Critical"){
		      app.loglevel(crow::LogLevel::Critical);
		}else{
			j["message"]="invalid loglevel";
		}
		return j;
	});

	app.loglevel(App::Config::config.get_loglevel());
	app.port(App::Config::config.get_port()).concurrency(App::Config::config.get_concurrency()).run();
}
