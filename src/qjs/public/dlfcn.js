http.add_header("Content-Type","text/html")
http.write(`<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8"/>
		<title>Test - ${new Date().getTime()}</title>
	</head>
	<body>
		<h3>DLFCN</h3>
		<p>${Object.keys(dlfcn).join("<br/>")}</p>
		<p>${(function(){
			var ret=[];
			var path="./qjsmod/a.so";
			var mod=dlfcn.dlopen(
				path,
				dlfcn.RTLD_LAZY
			);
			if(mod!=null&&mod!=0){
				ret.push("Module "+path+" loaded at "+mod);
				var symnam="entry";
				var sym=dlfcn.dlsym(mod,symnam);
				if(sym!=null&&sym!=0){
					ret.push("Symbol '"+symnam+"' loaded at "+sym);
					dlfcn.dlinvoke(sym);
					ret.push("module mod0 loaded:");
					ret.push("mod0 keys:");
					ret.push(Object.keys(mod0))
					try{
						var hdl={};
						mod0.fn0();
						mod0.fn1();
						mod0.fn2();
						mod0.fn3();
						ret.push(mod0.m0);
						ret.push(mod0.m1);
						ret.push(mod0.m2);
						ret.push(mod0.m3);
						hdl.a=new mod0.MyClass();
						hdl.b=new mod0.MyClass([1,2,3]);
						ret.push(Object.keys(hdl.a));
						ret.push(Object.keys(hdl.b));
						delete hdl.a;
						delete hdl.b;
						//delete hdl.b;
					}catch(e){
						ret.push("Error: "+e.toString());
					}
				}else{
					ret.push(dlfcn.dlerror());
				}
				dlfcn.dlclose(mod);
				ret.push("Module "+path+" closed");
			}else{
				ret.push(dlfcn.dlerror());
			}
			return ret.join("<br/>");
		})()}</p>
	</body>
</html>`);
http.end();
