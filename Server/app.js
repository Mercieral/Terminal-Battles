net = require('net');

var hostSockets = [];
var hostInfo = [];

net.createServer(function (socket) {

	socket.name = socket.remoteAddress + ":" + socket.remotePort;

	// Handle incoming messages from clients.
	socket.on('data', function (data) {
		var hostPatt = new RegExp("host");
		var clientPatt = new RegExp("client");
		var getPatt = new RegExp("get");
		var dataString = data.toString().trim();

		if (hostPatt.test(dataString)) {
            //Host initiating connection
			var hostName = dataString.split(",")[1];
			// console.log("host connected, name='" + hostName + "', ip='" + socket.remoteAddress.replace(/^.*:/, '') + "'");
			hostSockets.push(socket);
			hostInfo.push(hostName+";"+ socket.remoteAddress.replace(/^.*:/, ''));
			//console.log(socket);
		} else if (clientPatt.test(dataString)) {
			//Client intiating connection
			// console.log("client connected");
		}

		if (getPatt.test(dataString)) {
			//Client looking for hosts
            // console.log("client called get, sending host info");
            // console.log(hostInfo.toString());
            socket.write(hostInfo.toString() + '\0');
		}
	});

	// Remove the client from the list when it leaves
	socket.on('end', function () {
		let index = hostSockets.indexOf(socket);
        if (index != -1) {
            // console.log("user disconnected, host index = " + index);
            hostSockets.splice(index, 1);
            hostInfo.splice(index, 1);
		}
		else {
            // console.log("user disconnected, client");
		}
	});


}).listen(process.env.PORT || 5000);

// Put a friendly message on the terminal of the server.
console.log("Matchmaking server running at port 5000\n");