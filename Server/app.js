net = require('net');

var clients = [];

net.createServer(function (socket) {

	socket.name = socket.remoteAddress + ":" + socket.remotePort

	// Put this new client in the list
	// clients.push(socket);
	// console.log(clients);
	// console.log("-------------------------------");
	// console.log(socket);
	console.log("new user connected to server");

	// Handle incoming messages from clients.
	socket.on('data', function (data) {
		//console.log(data);
		var hostPatt = new RegExp("host");
		var clientPatt = new RegExp("client");
		var dataString = data.toString().trim();
		if (hostPatt.test(dataString)) {
			var hostName = dataString.split(",")[1];
			console.log("host connected, name='" + hostName + "', ip='" + socket.remoteAddress + "'");
			//console.log(socket);
		} else if (clientPatt.test(dataString)) {
			console.log("client connected");
		} else {
			console.log("error, unknown type connected");
		}
		//broadcast(socket.name + "> " + data, socket);
	});

	// Remove the client from the list when it leaves
	socket.on('end', function () {
		console.log("user disconnected");
		clients.splice(clients.indexOf(socket), 1);
	});

	// Send a message to all clients
	function broadcast(message, sender) {
		clients.forEach(function (client) {
			// Don't want to send it to sender
			if (client === sender) return;
			client.write(message);
		});
		// Log it to the server output too
		process.stdout.write(message)
	}

}).listen(5000);

// Put a friendly message on the terminal of the server.
console.log("Chat server running at port 5000\n");