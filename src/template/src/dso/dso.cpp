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
namespace App{
	namespace DSO{
		static DSOMgr dsomgr;
	}
}
