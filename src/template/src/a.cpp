#include"./cfg/cfg.hpp"
#include<iostream>
#include<sstream>
#include<vector>
#include<chrono>
#include<random>
#include"crow/crow.h"
#include"crow/crow/mustache.h"
#include"middleware/test0.hpp"
#include"middleware/test1.hpp"
#include"returnable/test.hpp"
#include"test/class/test0.hpp"
int main(int argc,char*argv[]){
	crow::App<App::Middleware::MWTest0,App::Middleware::MWTest1>app;
	CROW_ROUTE(app,"/")([](){
		return "test";
	});
	CROW_ROUTE(app,"/add/<int>/<int>")([](int a,int b){
		std::ostringstream oss;
		oss<<(a+b);
		return oss.str();
	});
	CROW_ROUTE(app,"/path/<path>")([](std::string path){
		std::ostringstream oss;
		oss<<path;
		return oss.str();
	});
	CROW_ROUTE(app,"/loglevel")([&app](){
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

	CROW_ROUTE(app,"/loglevel/<string>")([&app](std::string level){
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
	CROW_ROUTE(app,"/response")([](crow::response&res){
		res.add_header("Content-Type","text/plain");
		res.write(R"(test)");
		res.end();
	});
	CROW_ROUTE(app,"/response2")([](){
		crow::response res;
		res.add_header("Content-Type","text/plain");
		res.write(R"(test)");
		return res;
	});
	CROW_ROUTE(app,"/request")([](const crow::request&req,crow::response&res){
		res.add_header("Content-Type","text/plain");
		std::string p0=req.url_params.get("p0")==nullptr?"null":req.url_params.get("p0");
		std::ostringstream oss;
		oss<<p0;
		res.write(oss.str());
		res.end();
	});
	CROW_ROUTE(app,"/returnable")([](){
		App::Returnable::Test t;
		return t;
	});
	CROW_ROUTE(app,"/json_write")([](){
		crow::json::wvalue j;
		j["k0"]=42;
		{
			crow::json::wvalue v;
			v["k0"]="lorem";
			v["k1"]="ipsum";
			j["k1"]=std::move(v);
		}
		{
			std::vector<int>v{0,1,2,3};
			j["k2"]=std::move(v);
		}
		j["k3"][0]=0xdead;
		j["k3"][1]=0xbeef;
		j["k4"]["bar"]="beep";
		j["k4"]["baz"]="boop";
		return j;
	});
	CROW_ROUTE(app,"/json_read").methods(crow::HTTPMethod::POST)([](const crow::request&req){
		//curl -X POST -H "Content-Type: application/json" --data "{}" http://localhost:8080/json_read
		//curl -X POST -H "Content-Type: application/json" --data "{junk}" http://localhost:8080/json_read
		if(req.get_header_value("Content-Type")=="application/json"){
			try{
				crow::json::rvalue j=crow::json::load(req.body.c_str(),req.body.length());
				return crow::json::wvalue(j);
			}catch(const std::exception&e){
				crow::json::wvalue j;
				j["error"]="failed to parse json";
				return j;
			}
		}else{
			crow::json::wvalue j;
			j["error"]="invalid content type";
			return j;
		}
	});
	CROW_ROUTE(app,"/file")([](crow::response&res){
		res.set_static_file_info("public/index.html");
		res.end();
	});
	CROW_ROUTE(app,"/mustache")([](){
		crow::mustache::context ctx;
		//crow::json::wvalue ctx;
		ctx["title"]="test title";
		ctx["heading"]="test heading";
		ctx["message"]="test message";
		return crow::mustache::load("./test.html").render(ctx);
	});
	CROW_ROUTE(app,"/multipart").methods(crow::HTTPMethod::POST)([](const crow::multipart::message&req){
		//curl -v -F upload=@file0.txt -F upload=@file1.txt http://localhost:8080/multipart
		crow::json::wvalue j;
		std::vector<std::string>vcontents;
		for(const auto&part:req.parts){
			vcontents.push_back(part.body);
		}
		j["contents"]=vcontents;
		return j;
	});
	std::random_device rd;
	std::mt19937::result_type seed=rd();
	std::mt19937 gen(seed);
	CROW_ROUTE(app,"/ws")
		.websocket()
		.onopen([&](crow::websocket::connection&conn){
			conn.send_text("onopen");
			conn.userdata(new App::Test::Class::Test0());
		})
		.onmessage([&](crow::websocket::connection&conn,const std::string&data,bool is_binary){
			std::ostringstream oss;
			oss<<gen();
			conn.send_text(oss.str());
		})
		.onclose([&](crow::websocket::connection&conn,const std::string&reason){
			conn.send_text("onclose");
			delete static_cast<App::Test::Class::Test0*>(conn.userdata());
		})
	;
	app.loglevel(crow::LogLevel::Warning);
	app.port(8080).concurrency(8).run();
}
