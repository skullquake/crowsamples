#ifndef hc7bf9b66919811ebb5252f156a594951
#define hc7bf9b66919811ebb5252f156a594951
#include"crow/http_request.h"
#include"crow/http_response.h"
namespace App{
	namespace Middleware{
		struct MWTest{
			struct context{};
			void before_handle(crow::request&req,crow::response&res,context&ctx);
			void after_handle(crow::request&req,crow::response&res,context&ctx);
		};
	}
}
#endif
