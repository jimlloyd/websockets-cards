const express = require('express');
const http_ = require('http');
const socketio = require('socket.io');

const app = express();
const http = http_.createServer(app);
const io = socketio(http);

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/hearts.html');
});

app.use(express.static('public'));

io.on('connection', function(socket){
    console.log('a user connected');
  });

http.listen(3000, () => {
    console.log('listening on *:3000');
});
