#include"./dso.hpp"
#include"crow/logging.h"
Srv::DSO::DSOMgr::DSOMgr(){
	CROW_LOG_DEBUG<<"Srv::DSO::DSOMgr::DSOMgr:start";
	CROW_LOG_DEBUG<<"Srv::DSO::DSOMgr::DSOMgr:end";
}
Srv::DSO::DSOMgr::~DSOMgr(){
	CROW_LOG_DEBUG<<"Srv::DSO::DSOMgr::~DSOMgr:start";
	CROW_LOG_DEBUG<<"Srv::DSO::DSOMgr::~DSOMgr:end";
}
bool Srv::DSO::DSOMgr::load(std::string path){
	CROW_LOG_DEBUG<<"Srv::DSO::DSOMgr::load:start";
	CROW_LOG_DEBUG<<"Srv::DSO::DSOMgr::load:loading "<<path;
	CROW_LOG_DEBUG<<"Srv::DSO::DSOMgr::end:end";
}
namespace Srv{
	namespace DSO{
		DSOMgr dsomgr;
	}
}
