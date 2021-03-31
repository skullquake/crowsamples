#include"./test1.hpp"
#include<iostream>
void App::Middleware::MWTest1::before_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWTest1::before_handle:start";
	CROW_LOG_DEBUG<<"App::Middlware::MWTest1::before_handle:end";
}
void App::Middleware::MWTest1::after_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWTest1::after_handle:start";
	CROW_LOG_DEBUG<<"App::Middlware::MWTest1::after_handle:end";
};
