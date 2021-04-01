#ifndef h5f0d9e8292ed11eb94da436c5dd0e44d
#define h5f0d9e8292ed11eb94da436c5dd0e44d
#include"crow/http_request.h"
#include"crow/http_response.h"
namespace App{
	namespace Middleware{
		struct MWDso{
			struct context{};
			void before_handle(crow::request&req,crow::response&res,context&ctx);
			void after_handle(crow::request&req,crow::response&res,context&ctx);
		};
	}
}
#endif
