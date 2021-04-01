#include"./test0.hpp"
#include<iostream>
void App::Middleware::MWTest0::before_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWTest0::before_handle:start";
	CROW_LOG_DEBUG<<"App::Middlware::MWTest0::before_handle:end";
}
void App::Middleware::MWTest0::after_handle(crow::request&req,crow::response&res,context&ctx){
	CROW_LOG_DEBUG<<"App::Middlware::MWTest0::after_handle:start";
	CROW_LOG_DEBUG<<"App::Middlware::MWTest0::after_handle:end";
};
