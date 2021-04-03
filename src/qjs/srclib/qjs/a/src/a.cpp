#include<iostream>
#include<sstream>
#include"quickjspp.hpp"
#ifdef __cplusplus
extern "C"{
#endif
class MyClass{
	public:
		MyClass(){
			//std::cout<<"MyClassqwer::MyClass::start"<<std::endl;
			//std::cout<<"MyClassqwer::MyClass::this::"<<this<<std::endl;
			//std::cout<<"MyClassqwer::MyClass::end"<<std::endl;
		}
		~MyClass(){
			//std::cout<<"MyClass::~MyClass::start"<<std::endl;
			//std::cout<<"MyClass::~MyClass::end"<<std::endl;
		}

		MyClass(std::vector<int>){}
		double member_variable=5.5;
		std::string member_function(const std::string&s){return"Hello,"+s;}
};
void fn0(){
	//std::cout<<"fn0::start"<<std::endl;
	//std::cout<<"fn0::end"<<std::endl;
}
int fn1(){
	//std::cout<<"fn1::start"<<std::endl;
	//std::cout<<"fn1::end"<<std::endl;
	return 42;
}
void fn2(int a){
	//std::cout<<"fn2::start"<<std::endl;
	//std::cout<<"fn2::a:"<<a<<std::endl;
	//std::cout<<"fn2::end"<<std::endl;
}
int fn3(int a){
	//std::cout<<"fn3::start"<<std::endl;
	//std::cout<<"fn3::a:"<<a<<std::endl;
	//std::cout<<"fn3::end"<<std::endl;
	return a*2;
}
//static Loader l;
__attribute__((visibility("default"))) int entry(void*data){
	//std::cout<<"dso_template:entry:start"<<std::endl;
	//std::cout<<"dso_template:entry:data:"<<data<<std::endl;
	if(data==nullptr){
		std::cerr<<"data null"<<std::endl<<std::flush;
		return 0;
	}
	qjs::Context*context=(qjs::Context*)data;
	{
		qjs::Context::Module&module=context->addModule("mod0");
		module.add("m0",true);
		module.add("m1",42);
		module.add("m2",4.2);
		module.add("m3","foo");
		for(int i=0;i<8;i++){
			std::ostringstream oss;
			oss<<"m"<<i;
			module.add(oss.str().c_str(),"bar");
		}
		module.function<&fn0>("fn0");
		module.function<&fn1>("fn1");
		module.function<&fn2>("fn2");
		module.function<&fn3>("fn3");
		module.class_<MyClass>("MyClass")
			.constructor<>()
			.constructor<std::vector<int>>("MyClassA")
			.fun<&MyClass::member_variable>("member_variable")
			.fun<&MyClass::member_function>("member_function")
		;
		/*
		context->eval(R"()","<eval>",JS_EVAL_TYPE_MODULE);
		*/
		context->eval(R"(
			import * as mod0 from 'mod0';
			globalThis.mod0=mod0;
		)",
		//"<eval>",JS_EVAL_TYPE_MODULE);
		"<input>",JS_EVAL_TYPE_MODULE);
	}
	//std::cout<<"dso_template:entry:end"<<std::endl;
	//std::cout<<std::flush;
	return 1;
}
#ifdef __cplusplus
}
#endif
