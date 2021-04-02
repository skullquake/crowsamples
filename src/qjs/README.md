### Crow DSO Middleware ###

`./src/middleware/dso` provides `App::Middleware::MWDso`, which uses `dlfcn` to load in `.so` files and call `entry` with `crow::http_request` and `crow::http_response`
