#include<sstream>
#include"crow/http_request.h"
#include"crow/http_response.h"
#include"crow/logging.h"
static void __attribute__((constructor))startup(void){}
static void __attribute__((destructor))shutdown(void){}
extern "C"{
	__attribute__((visibility("default")))int entry(void*vreq,void*vres){
	}
}
