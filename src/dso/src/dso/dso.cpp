#include"./dso.hpp"
#include"crow/logging.h"
App::DSO::DSOMgr::DSOMgr(){
	CROW_LOG_DEBUG<<"App::DSO::DSOMgr::DSOMgr:start";
	CROW_LOG_DEBUG<<"App::DSO::DSOMgr::DSOMgr:end";
}
App::DSO::DSOMgr::~DSOMgr(){
	CROW_LOG_DEBUG<<"App::DSO::DSOMgr::~DSOMgr:start";
	CROW_LOG_DEBUG<<"App::DSO::DSOMgr::~DSOMgr:end";
}
bool App::DSO::DSOMgr::load(std::string path){
	CROW_LOG_DEBUG<<"App::DSO::DSOMgr::load:start";
	CROW_LOG_DEBUG<<"App::DSO::DSOMgr::load:loading "<<path;
	CROW_LOG_DEBUG<<"App::DSO::DSOMgr::end:end";
}
namespace App{
	namespace DSO{
		DSOMgr dsomgr;
	}
}
