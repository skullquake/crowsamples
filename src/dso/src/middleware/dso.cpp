#include"./dso.hpp"
#include<iostream>
#include<dlfcn.h>
#include<sys/stat.h>
#include"crow/json.h"
#if __cplusplus >= 201703L && __has_include(<filesystem>)
#include<filesystem>
namespace fs=std::filesystem;
#else
#include<experimental/filesystem>
namespace fs=std::experimental::filesystem;
#endif
static bool fileExists(std::string path){
	CROW_LOG_DEBUG<<"Checking "+path;
	struct stat sb;
	if(stat(path.c_str(),&sb)==-1){
		return false;
	}
	return true;
}
enum class Os{
        WIN32=0,
        WIN64,
        APPLE,
        LINUX,
        FREEBSD,
        UNIX,
        OTHER
};
static bool canLoadDSO(std::string path){
	std::string extension=fs::path(path).extension();
	std::string dsoextension=
#ifdef _WIN32
		".dll"
#elif _WIN64
		".dll"
#elif __APPLE__ || __MACH__
		".so"
#elif __linux__
		".so"
#elif __FreeBSD__
		".so"
#elif __unix || __unix__
		".so"
#else
		""
#endif
		;
	return extension==dsoextension;
}
Os getOs(){
    #ifdef _WIN32
    return Os::WIN32;//"Windows 32-bit";
    #elif _WIN64
    return Os::WIN64;//"Windows 64-bit";
    #elif __APPLE__ || __MACH__
    return Os::APPLE;//"Mac OSX";
    #elif __linux__
    return Os::LINUX;//"Linux";
    #elif __FreeBSD__
    return Os::FREEBSD;//"FreeBSD";
    #elif __unix || __unix__
    return Os::UNIX;//"Unix";
    #else
    return Os::OTHER;//"Other";
    #endif
}
void App::Middleware::MWDso::before_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWDso::before_handle:start";
	std::string path=std::string(CROW_STATIC_DIRECTORY)+req.url;
	if(canLoadDSO(path)&&fileExists(path)){
		CROW_LOG_DEBUG<<"App::Middlware::MWDso::before_handle:opening "<<path;
		void*mod=dlopen(path.c_str(),RTLD_LAZY);
		if(mod!=nullptr){
			CROW_LOG_DEBUG<<"App::Middlware::MWDso::before_handle:opened "<<path;
			dlclose(mod);
			CROW_LOG_DEBUG<<"App::Middlware::MWDso::before_handle:closed "<<path;
			res.add_header("Content-Type","application/json");
			crow::json::wvalue j;
			j["message"]="dso loaded";
			res.write(j.dump());
		}else{
			std::string error{dlerror()};
			CROW_LOG_DEBUG<<"App::Middlware::MWDso::before_handle:failed to open "<<path<<":"<<error;
			res.add_header("Content-Type","application/json");
			crow::json::wvalue j;
			j["error"]=error;
			res.write(j.dump());
		}
		res.end();
	}
	CROW_LOG_DEBUG<<"App::Middlware::MWDso::before_handle:end";
}
void App::Middleware::MWDso::after_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWDso::after_handle:start";
	CROW_LOG_DEBUG<<"App::Middlware::MWDso::after_handle:end";
};
