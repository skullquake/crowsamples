#include"./test.hpp"
#include<iostream>
void App::Middleware::MWTest::before_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWTest::before_handle:start";
	CROW_LOG_DEBUG<<"App::Middlware::MWTest::before_handle:end";
}
void App::Middleware::MWTest::after_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWTest::after_handle:start";
	CROW_LOG_DEBUG<<"App::Middlware::MWTest::after_handle:end";
};
