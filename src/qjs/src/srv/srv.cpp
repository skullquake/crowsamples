#include"./srv.hpp"
#include<iostream>
#include<sstream>
#include<vector>
#include<chrono>
#include<random>
#include<thread>
#include<chrono>
#include<atomic>
#include<mutex>
#include<boost/filesystem.hpp>
#include"srv/config/config.hpp"
#include"srv/dso/dso.hpp"
#include"quickjspp.hpp"
#include"quickjs/quickjs-libc.h"
crow::App<Srv::Middleware::MWDso>Srv::init(){
	crow::App<Srv::Middleware::MWDso>app;
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
	//--------------------------------------------------------------------------------
	//quickjs
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/sjs/<path>")([&app](const crow::request&req,crow::response&res,const std::string&pathP){
		bool responded=false;
		std::string src;
		std::string path=boost::filesystem::path(std::string(CROW_STATIC_DIRECTORY)+pathP).lexically_normal().string();
		std::ifstream ifs(path);
		if(!ifs.is_open()){
			CROW_LOG_ERROR<<"sjs/:Failed to open "<<path;
		}else{
			{
				std::ostringstream oss;
				oss<<ifs.rdbuf();
				src=oss.str();
			}
			qjs::Runtime runtime;
			qjs::Context context(runtime);
			JSContext*ctx=context.ctx;
			JSRuntime*rt=runtime.rt;
			js_std_init_handlers(rt);
			JS_SetModuleLoaderFunc(rt,nullptr,js_module_loader,nullptr);
			js_std_add_helpers(ctx,0,nullptr);
			js_init_module_std(ctx,"std");
			js_init_module_os(ctx,"os");
			context.eval(R"(
				import * as std from 'std';
				import * as os from 'os';
				globalThis.std=std;
				globalThis.os=os;
			)","<input>",JS_EVAL_TYPE_MODULE);
			auto& module=context.addModule("HttpResponse");
			module.function("add_header",[&res](std::string k,std::string v){res.add_header(k,v);});
			module.function("write",[&res,&responded](std::string val){res.write(val);responded=true;});
			module.function("end",[&res](){res.end();});
			context.eval(R"(
				import * as http from 'HttpResponse';
				globalThis.http=http;
			)","<input>",JS_EVAL_TYPE_MODULE);
			try{
				context.eval(src,"<eval>",JS_EVAL_TYPE_MODULE);
			}catch(qjs::exception){
				auto exc=context.getException();
				CROW_LOG_ERROR<<"sjs/"<<(std::string)exc;
			}
		}
		if(!responded){//empty
			res.add_header("Content-Type","text/plain");
			res.end();
		}
	});
	app.loglevel(Srv::Config::config.get_loglevel());
	app.port(Srv::Config::config.get_port()).concurrency(Srv::Config::config.get_concurrency()).run();
}
