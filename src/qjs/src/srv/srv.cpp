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
#include<malloc.h>
#include<boost/filesystem.hpp>
#include<dlfcn.h>
#include"srv/config/config.hpp"
#include"srv/dso/dso.hpp"
#include"quickjspp.hpp"
#include"quickjs/quickjs-libc.h"
#include"mon/mon.hpp"
crow::App<Srv::Middleware::MWDso>Srv::init(){
	crow::App<Srv::Middleware::MWDso>app;
	//--------------------------------------------------------------------------------
	//mon
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/api/mon")([&app](){
		crow::json::wvalue j;
		double vm,rss;
		Mon::mem_usage(vm,rss);
		j["vm"]=vm;
		j["rss"]=rss;
		return j.dump();
	});
	CROW_ROUTE(app,"/api/mem/trim")([&app](){
		malloc_trim(0);
		crow::json::wvalue j;
		j["message"]="malloc_trim(0) called";
		return j.dump();
	});


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
			{
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
			}
			{
				auto&module=context.addModule("HttpResponse");
				module.function("add_header",[&res](std::string k,std::string v){res.add_header(k,v);});
				module.function("write",[&res,&responded](std::string val){res.write(val);responded=true;});
				module.function("end",[&res](){res.end();});
				context.eval(R"(
					import * as http from 'HttpResponse';
					globalThis.http=http;
				)","<input>",JS_EVAL_TYPE_MODULE);
			}
			{
				auto&module=context.addModule("Dlfcn");
				module.add("RTLD_LAZY",RTLD_LAZY);
				module.add("RTLD_NOW",RTLD_NOW);
				module.function("dlopen",[](std::string path,int mode){
					std::ostringstream oss;
					void*addr=dlopen(path.c_str(),mode);
					oss<<addr;
					return oss.str();
				});
				module.function("dlclose",[](std::string saddr){
					std::istringstream iss(saddr);
					void*addr;
					iss>>addr;
					return dlclose((void*)addr);
				});
				module.function("dlsym",[](const char*saddr,const char*snam){
					std::istringstream iss(saddr);
					void*addr;
					iss>>addr;
					std::ostringstream oss;
					addr=dlsym(addr,snam);
					oss<<addr;
					return oss.str();
				});
				module.function("dlerror",[](int a){
					return std::string(dlerror());
				});
				module.function("dlinvoke",[&context](const char*saddr){
					std::istringstream iss(saddr);
					void*addr;
					iss>>addr;
					int(*cb)(void*){(int(*)(void*))addr};
					return cb((void*)&(context));
				});
				context.eval(R"(
					import * as dlfcn from 'Dlfcn';
					globalThis.dlfcn=dlfcn;
				)","<input>",JS_EVAL_TYPE_MODULE);
			}
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
