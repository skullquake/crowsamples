#include"./test0.hpp"
#include<iostream>
#include"crow/logging.h"
App::Test::Class::Test0::Test0(){
	CROW_LOG_DEBUG<<"App::Test::Class::Test0::Test0():start";
	CROW_LOG_DEBUG<<"App::Test::Class::Test0::Test0():end";
}
App::Test::Class::Test0::~Test0(){
	CROW_LOG_DEBUG<<"App::Test::Class::Test0::~Test0():start";
	CROW_LOG_DEBUG<<"App::Test::Class::Test0::~Test0():end";
}

