#ifndef h3245afcc92dc11ebbed0132767120701
#define h3245afcc92dc11ebbed0132767120701
#include<map>
namespace App{
	namespace DSO{
		class DSOMgr{
			public:
				DSOMgr();
				~DSOMgr();
			private:
				std::map<std::string,void*>mdso;
		};
	}
}
#endif
