// server.js – UDP → WebSocket bridge
const dgram = require('dgram');
const WebSocket = require('ws');

const UDP_PORT = 5000;
const WS_PORT = 8080;

const udp = dgram.createSocket('udp4');
const wss = new WebSocket.Server({ port: WS_PORT });

console.log(`WebSocket server on ws://localhost:${WS_PORT}`);

wss.on('connection', ws => {
    console.log('Client connected');
    ws.on('close', () => console.log('Client disconnected'));
});

udp.bind(UDP_PORT, () => {
    console.log(`Listening UDP :${UDP_PORT}`);
});

udp.on('message', (msg, rinfo) => {
    wss.clients.forEach(client => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(msg, { binary: true });
        }
    });
});
