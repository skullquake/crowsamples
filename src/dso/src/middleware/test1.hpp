#ifndef h49fdc498921d11eba826bbd2795b2217
#define h49fdc498921d11eba826bbd2795b2217
#include"crow/http_request.h"
#include"crow/http_response.h"
namespace App{
	namespace Middleware{
		struct MWTest1{
			struct context{};
			void before_handle(crow::request&req,crow::response&res,context&ctx);
			void after_handle(crow::request&req,crow::response&res,context&ctx);
		};
	}
}
#endif
