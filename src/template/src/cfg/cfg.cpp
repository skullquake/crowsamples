#include"./cfg.hpp"
void printcfg(){
	std::cout<<__PRETTY_FUNCTION__<<":GCCVERSION:......... "<<GCCVERSION<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<":PREFIX:............. "<<PREFIX<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<":POSTFIX:............ "<<POSTFIX<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<":LIBDIR:............. "<<LIBDIR<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<":LIBPREFIX:.......... "<<LIBPREFIX<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<":LIBPOSTFIX:......... "<<LIBPOSTFIX<<std::endl;
	std::cout<<__PRETTY_FUNCTION__<<":DSOEXT:............. "<<DSOEXT<<std::endl;
}
