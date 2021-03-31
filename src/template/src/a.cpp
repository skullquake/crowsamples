#include"./cfg/cfg.hpp"
#include<iostream>
#include<sstream>
#include<vector>
#include"crow/crow.h"
#include"middleware/test.hpp"
#include"returnable/test.hpp"
int main(int argc,char*argv[]){
	crow::App<App::Middleware::MWTest>app;
	CROW_ROUTE(app,"/")([](){
		return "test";
	});
	CROW_ROUTE(app,"/add/<int>/<int>")([](int a,int b){
		std::ostringstream oss;
		oss<<(a+b);
		return oss.str();
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
	app.loglevel(crow::LogLevel::Warning);
	app.port(8080).concurrency(8).run();


}
