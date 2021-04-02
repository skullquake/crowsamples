#ifndef hacd72f78939e11ebbb6a271fc29400bb
#define hacd72f78939e11ebbb6a271fc29400bb
#include"crow/crow.h"
#include"app/config/config.hpp"
#include"app/middleware/dso.hpp"
#include"dso/dso.hpp"
namespace App{
	crow::App<App::Middleware::MWDso>mkapp();
}
#endif
