#include<sstream>
#include<dlfcn.h>
#include"crow/http_request.h"
#include"crow/http_response.h"
#include"crow/logging.h"
static void __attribute__((constructor))startup(void){}
static void __attribute__((destructor))shutdown(void){}
extern "C"{
	__attribute__((visibility("default")))int entry(void*vreq,void*vres){
		if(vreq==nullptr||vres==nullptr)return 1;
		auto req{static_cast<crow::request*>(vreq)};
		auto res{static_cast<crow::response*>(vres)};
		res->add_header("Content-Type","text/plain");
		std::ostringstream oss;
		Dl_info info;
		if(dladdr((void*)entry,&info)){
			oss<<std::string(info.dli_fname);
		}else{
			oss<<"?";
		}
		oss<<":start";
		res->write(oss.str());
		res->end();
		CROW_LOG_DEBUG<<"liba:entry:end";
		return 0;
	}
}
