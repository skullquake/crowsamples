#include"./app.hpp"
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
#include"app/config/config.hpp"
#include"app/middleware/dso.hpp"
#include"dso/dso.hpp"
crow::App<App::Middleware::MWDso>App::mkapp(){
	crow::App<App::Middleware::MWDso>app;
	//--------------------------------------------------------------------------------
	//Obtain logging level
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/api/loglevel")([&app](){
		//curl http://localhost:8080/api/loglevel
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
	CROW_ROUTE(app,"/api/loglevel/<string>")([&app](std::string level){
		//curl http://localhost:8080/api/loglevel/Debug
		//curl http://localhost:8080/api/loglevel/Info
		//curl http://localhost:8080/api/loglevel/Warning
		//curl http://localhost:8080/api/loglevel/Error
		//curl http://localhost:8080/api/loglevel/Critical
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
	
}
