#ifndef hacd72f78939e11ebbb6a271fc29400bb
#define hacd72f78939e11ebbb6a271fc29400bb
#include"crow/crow.h"
#include"srv/middleware/dso.hpp"
#include"dso/dso.hpp"
namespace Srv{
	crow::App<Srv::Middleware::MWDso>init();
}
#endif
