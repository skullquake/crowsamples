#ifndef hc67c0e82961c11eb959b376e99eacfc0
#define hc67c0e82961c11eb959b376e99eacfc0
#include<sstream>
#include<string>
#include<dlfcn.h>
#include"crow/http_request.h"
#include"crow/http_response.h"
#include"crow/logging.h"
static void __attribute__((constructor))startup(void){}
static void __attribute__((destructor))shutdown(void){}
std::string modpath(){
	std::ostringstream oss;
	Dl_info info;
	if(dladdr((void*)__func__,&info)){
		oss<<std::string(info.dli_fname);
	}else{
		oss<<"?";
	}
	oss<<":start";
	return oss.str();
}
#endif
