#include"crow/http_request.h"
#include"crow/http_response.h"
#include"crow/logging.h"
#include"util/util.hpp"
extern "C"{
	__attribute__((visibility("default")))int entry(void*vreq,void*vres){
		if(vreq==nullptr||vres==nullptr)return 1;
		auto req{static_cast<crow::request*>(vreq)};
		auto res{static_cast<crow::response*>(vres)};
		res->add_header("Content-Type","text/plain");
		res->write(modpath());
		res->end();
		CROW_LOG_DEBUG<<"liba:entry:end";
		return 0;
	}
}
