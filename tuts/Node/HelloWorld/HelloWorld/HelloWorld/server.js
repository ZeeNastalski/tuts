﻿var http = require('http');
var port = process.env.port || 1337;

http.createServer(function (req, res) {
    res.writeHead(200, { 'Content-Type': 'text/plain' });
    res.write("Hello this is dog\n");
    res.end('Hello World\n');
}).listen(port);

console.log("listening on port ", port);