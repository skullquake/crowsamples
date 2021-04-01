#include"./cfg/cfg.hpp"
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
#include"crow/crow/mustache.h"
#include"middleware/test0.hpp"
#include"middleware/test1.hpp"
#include"returnable/test.hpp"
#include"test/class/test0.hpp"
int main(int argc,char*argv[]){
	crow::App<App::Middleware::MWTest0,App::Middleware::MWTest1>app;
	//--------------------------------------------------------------------------------
	//Example handler
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/")([](){
		return "test";
	});
	//--------------------------------------------------------------------------------
	//Example handler: <int>
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/add/<int>/<int>")([](int a,int b){
		std::ostringstream oss;
		oss<<(a+b);
		return oss.str();
	});
	//--------------------------------------------------------------------------------
	//Example handler: <path>
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/path/<path>")([](std::string path){
		std::ostringstream oss;
		oss<<path;
		return oss.str();
	});
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
	//--------------------------------------------------------------------------------
	//Handle response example
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/response")([](crow::response&res){
		res.add_header("Content-Type","text/plain");
		res.write(R"(test)");
		res.end();
	});
	//--------------------------------------------------------------------------------
	//Handle response example
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/response2")([](){
		crow::response res;
		res.add_header("Content-Type","text/plain");
		res.write(R"(test)");
		return res;
	});
	//--------------------------------------------------------------------------------
	//Handle request example
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/request")([](const crow::request&req,crow::response&res){
		res.add_header("Content-Type","text/plain");
		std::string p0=req.url_params.get("p0")==nullptr?"null":req.url_params.get("p0");
		std::ostringstream oss;
		oss<<p0;
		res.write(oss.str());
		res.end();
	});
	//--------------------------------------------------------------------------------
	//Return class instance example
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/returnable")([](){
		App::Returnable::Test t;
		return t;
	});
	//--------------------------------------------------------------------------------
	//Write json example
	//--------------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------------
	//Read json example
	//--------------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------------
	//Serve static file example
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/file")([](crow::response&res){
		res.set_static_file_info("public/index.html");
		res.end();
	});
	//--------------------------------------------------------------------------------
	//Mustache example
	//--------------------------------------------------------------------------------
	CROW_ROUTE(app,"/mustache")([](){
		crow::mustache::context ctx;
		//crow::json::wvalue ctx;
		ctx["title"]="test title";
		ctx["heading"]="test heading";
		ctx["message"]="test message";
		return crow::mustache::load("./test.html").render(ctx);
	});
	//--------------------------------------------------------------------------------
	//Multipart example
	//--------------------------------------------------------------------------------
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
	//--------------------------------------------------------------------------------
	//WebSocket example
	//--------------------------------------------------------------------------------
	std::random_device rd;
	std::mt19937::result_type seed=rd();
	std::mt19937 gen(seed);
	CROW_ROUTE(app,"/ws")
		.websocket()
		.onopen([&](crow::websocket::connection&conn){
			CROW_LOG_DEBUG<<"main::ws::onopen:"<<std::this_thread::get_id()<<":start";
			conn.send_text("onopen");
			conn.userdata(new App::Test::Class::Test0());
			CROW_LOG_DEBUG<<"main::ws::onopen:"<<std::this_thread::get_id()<<":end";
		})
		.onmessage([&](crow::websocket::connection&conn,const std::string&data,bool is_binary){
			CROW_LOG_DEBUG<<"main::ws::onmessage:"<<std::this_thread::get_id()<<":start";
			std::ostringstream oss;
			oss<<gen();
			conn.send_text(oss.str());
			CROW_LOG_DEBUG<<"main::ws::onmessage:"<<std::this_thread::get_id()<<":end";
		})
		.onclose([&](crow::websocket::connection&conn,const std::string&reason){
			CROW_LOG_DEBUG<<"main::ws::onclose:"<<std::this_thread::get_id()<<":start";
			conn.send_text("onclose");
			delete static_cast<App::Test::Class::Test0*>(conn.userdata());
			CROW_LOG_DEBUG<<"main::ws::onclose:"<<std::this_thread::get_id()<<":end";
		})
	;
	//--------------------------------------------------------------------------------
	//WebSocket background thread example
	//--------------------------------------------------------------------------------
	class WsThreadUserData{
		public:
			WsThreadUserData(crow::websocket::connection*conn):conn(conn){
				CROW_LOG_DEBUG<<"WsThreadUserData::WsThreadUserData:"<<std::this_thread::get_id()<<":start";
				t=new std::thread([this](){
					CROW_LOG_DEBUG<<"WsThreadUserData::WsThreadUserData:"<<std::this_thread::get_id()<<":start";
					std::random_device rd;
					std::mt19937::result_type seed=rd();
					std::mt19937 gen(seed);
					while(!aend.load()){
						CROW_LOG_DEBUG<<"WsThreadUserData::WsThreadUserData:"<<std::this_thread::get_id()<<":write";
						{
							std::lock_guard<std::mutex>l(mwrite);
							std::ostringstream oss;
							oss<<gen();
							this->conn->send_text(oss.str());
						}
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					}
					CROW_LOG_DEBUG<<"WsThreadUserData::WsThreadUserData:"<<std::this_thread::get_id()<<":end";
				});
				CROW_LOG_DEBUG<<"WsThreadUserData::WsThreadUserData:"<<std::this_thread::get_id()<<":end";
			};
			~WsThreadUserData(){
				CROW_LOG_DEBUG<<"WsThreadUserData::~WsThreadUserData:"<<std::this_thread::get_id()<<":start";
				end();
				CROW_LOG_DEBUG<<"WsThreadUserData::~WsThreadUserData:"<<std::this_thread::get_id()<<":end";
			};
			void end(){
				CROW_LOG_DEBUG<<"WsThreadUserData::end:"<<std::this_thread::get_id()<<":start";
				std::lock_guard<std::mutex>l(mwrite);
				aend=true;
				CROW_LOG_DEBUG<<"WsThreadUserData::end:"<<std::this_thread::get_id()<<":end";
			}
		private:
			crow::websocket::connection*conn;
			std::thread*t;
			std::atomic<bool>aend{false};
			std::mutex mwrite;
			
	};
	CROW_ROUTE(app,"/wsthread")
		.websocket()
		.onopen([&](crow::websocket::connection&conn){
			CROW_LOG_DEBUG<<"main::wsthread::onopen:"<<std::this_thread::get_id()<<":start";
			conn.send_text("onopen");
			conn.userdata(new WsThreadUserData(&conn));
			CROW_LOG_DEBUG<<"main::wsthread::onopen:"<<std::this_thread::get_id()<<":end";
		})
		.onmessage([&](crow::websocket::connection&conn,const std::string&data,bool is_binary){
			CROW_LOG_DEBUG<<"main::wsthread::onmessage:"<<std::this_thread::get_id()<<":start";
			CROW_LOG_DEBUG<<"main::wsthread::onmessage:"<<std::this_thread::get_id()<<":end";
		})
		.onclose([&](crow::websocket::connection&conn,const std::string&reason){
			CROW_LOG_DEBUG<<"main::wsthread::onclose:"<<std::this_thread::get_id()<<":start";
			delete static_cast<WsThreadUserData*>(conn.userdata());
			CROW_LOG_DEBUG<<"main::wsthread::onclose:"<<std::this_thread::get_id()<<":end";
		})
	;
	app.loglevel(crow::LogLevel::Warning);
	app.port(8080).concurrency(8).run();
}
