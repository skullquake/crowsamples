#include<iostream>
#include<cstdio>
#include"./a.hpp"
#ifndef LIBNAM
#define LIBNAM "unnamed_"
#endif
#ifndef DSOENTRY
#define DSOENTRY entry
#endif
	static void __attribute__((constructor))startup(void){
#if defined(_WIN32)||defined(__DJGPP__)
		std::cout<<"int __attribute__((constructor))startup(void)"<<std::endl;
#else
		//crashes on some system, iostream not initialized properly at this point
		printf("int __attribute__((constructor))startup(void)\n");
#endif
	}
	static void __attribute__((destructor))shutdown(void){
#if defined(_WIN32)||defined(__DJGPP__)
		std::cout<<"int __attribute__((destructor))shutdown(void)"<<std::endl;
#else
		printf("int __attribute__((destructor))shutdown(void)\n");
#endif
	}
extern "C"{
	__attribute__((visibility("default")))int DSOENTRY(int argc,char*argv[]){
		std::cout<<LIBNAM<<":main:start"<<std::endl;
		std::cout<<LIBNAM<<":main:end"<<std::endl;
		return 0;
	}
	__attribute__((visibility("default")))int fn_a(int argc,char*argv[]){
		std::cout<<LIBNAM<<":main:start"<<std::endl;
		std::cout<<LIBNAM<<":main:end"<<std::endl;
		return 0;
	}
}

