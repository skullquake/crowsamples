#include"./config.hpp"
#include<iostream>
#include<fstream>
#include<sstream>
#include"crow/json.h"
App::Config::Config::Config():
	host(App::Config::default_host),
	port(App::Config::default_port),
	loglevel(App::Config::default_loglevel),
	concurrency(App::Config::default_concurrency)
{
	try{
		std::ifstream ifs("./res/config.json");
		std::ostringstream oss;
		oss<<ifs.rdbuf();
		try{
			crow::json::rvalue j=crow::json::load(oss.str().c_str(),oss.str().length());
			if(j.has("host"))this->host=j["host"].s();
			if(j.has("port"))this->port=j["port"].i();
			if(j.has("loglevel")){
				std::string sloglevel=j["loglevel"].s();
				      if(sloglevel=="Debug"){
					this->loglevel=crow::LogLevel::Debug;
				}else if(sloglevel=="Info"){
					this->loglevel=crow::LogLevel::Info;
				}else if(sloglevel=="Warning"){
					this->loglevel=crow::LogLevel::Warning;
				}else if(sloglevel=="Error"){
					this->loglevel=crow::LogLevel::Error;
				}else if(sloglevel=="Critical"){
					this->loglevel=crow::LogLevel::Critical;
				}else{
					this->loglevel=crow::LogLevel::Info;
				}
			}
			if(j.has("concurrency"))this->concurrency=j["concurrency"].i();
		}catch(const std::exception&e){
			std::cout<<"App::Config::Config::Config:error:failed to parse json"<<std::endl;
		}
	}catch(const std::exception&e){
		std::cout<<"App::Config::Config::Config:error:"<<e.what()<<std::endl;
	}
}
App::Config::Config::~Config(){
}
std::string App::Config::Config::get_host(){
	return this->host;
}
int App::Config::Config::get_port(){
	return this->port;
}
crow::LogLevel App::Config::Config::get_loglevel(){
	return this->loglevel;
}
int App::Config::Config::get_concurrency(){
	return this->concurrency;
}
namespace App{
	namespace Config{
		Config config;
	}
}
