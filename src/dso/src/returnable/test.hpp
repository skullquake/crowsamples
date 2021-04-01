#ifndef h4b8485a091a411eba97177b50c8df1df
#define h4b8485a091a411eba97177b50c8df1df
#include"crow/returnable.h"
namespace App{
	namespace Returnable{
		class Test:public crow::returnable{
			public:
				Test();
				std::string as_string();
				std::string dump()override;
		};
	}
}
#endif
