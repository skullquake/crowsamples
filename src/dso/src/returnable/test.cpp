#include"./test.hpp"
#include"crow/returnable.h"
App::Returnable::Test::Test():crow::returnable("text/plain"){}
std::string App::Returnable::Test::as_string(){
	return "test";
}
std::string App::Returnable::Test::dump(){
	return this->as_string();
}
