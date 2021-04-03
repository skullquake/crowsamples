http.add_header("Content-Type","text/html")
http.write(`<!DOCTYPE html>
<html lang="en">
	<head>
		<meta charset="utf-8"/>
		<title>Test - ${new Date().getTime()}</title>
	</head>
	<body>
`);
http.write(`<table>`)
for(var row=0;row<8;row++){
	http.write(`<tr>`);
	for(var col=0;col<8;col++){
		http.write(`<td>${Math.floor(Math.random()*10)}</td>`);
	}
	http.write(`</tr>`);
}
http.write(`<table/>`)
http.write(`
	</body>
</html>`);
http.end();
