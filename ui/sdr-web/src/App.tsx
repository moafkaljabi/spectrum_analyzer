import { useEffect, useRef, useState } from 'react';
import { Line } from 'react-chartjs-2';
import { Chart as ChartJS, CategoryScale, LinearScale, PointElement, LineElement } from 'chart.js';
ChartJS.register(CategoryScale, LinearScale, PointElement, LineElement);

function App() {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const [ws, setWs] = useState<WebSocket | null>(null);
  const [freq, setFreq] = useState(100000000);
  const [gain, setGain] = useState(40);
  const [psd, setPsd] = useState<Float32Array>(new Float32Array(4096));

  // Waterfall buffer
  const waterfall = useRef<ImageData | null>(null);
  const history = useRef<number[]>([]);

  useEffect(() => {
    const socket = new WebSocket('ws://localhost:8080');
    socket.binaryType = 'arraybuffer';

    socket.onmessage = (event) => {
      const data = new Uint8Array(event.data);
      if (data.length !== 8192) return;

      // Convert I/Q to float and compute PSD
      const complex = new Float32Array(8192);
      for (let i = 0; i < 8192; i++) {
        complex[i] = (data[i] - 127.5) / 128.0;
      }

      // Simple magnitude squared (no FFT for speed demo)
      const mag2 = new Float32Array(4096);
      for (let i = 0; i < 4096; i++) {
        const I = complex[i*2];
        const Q = complex[i*2+1];
        mag2[i] = I*I + Q*Q;
      }

      // dB + shift
      const psdDb = mag2.map(m => 10 * Math.log10(m + 1e-12) + 50);
      setPsd(psdDb);

      // Waterfall update
      const ctx = canvasRef.current?.getContext('2d');
      if (!ctx) return;

      if (!waterfall.current) {
        waterfall.current = ctx.createImageData(4096, 600);
      }
      const img = waterfall.current;
      // Shift up
      img.data.set(img.data.slice(4096*4));
      // New line
      for (let i = 0; i < 4096; i++) {
        const v = Math.min(255, Math.max(0, psdDb[i] * 3));
        const idx = 4096*4*599 + i*4;
        img.data[idx+0] = v;      // R
        img.data[idx+1] = 255-v;  // G
        img.data[idx+2] = 128;    // B
        img.data[idx+3] = 255;
      }
      ctx.putImageData(img, 0, 0);
    };

    setWs(socket);
    return () => socket.close();
  }, []);

  const sendCommand = (cmd: string) => {
    if (ws) ws.send(cmd);
  };

  return (
    <div style={{fontFamily: 'sans-serif', padding: 20}}>
      <h1>RTL-SDR Live Spectrum Analyzer</h1>
      <div style={{display: 'grid', gridTemplateColumns: '1fr 300px', gap: 20}}>
        <div>
          <h2>Waterfall</h2>
          <canvas ref={canvasRef} width={4096} height={600} style={{width: '100%', imageRendering: 'pixelated'}} />
          <h2>Spectrum</h2>
          <Line data={{
            labels: Array(4096).fill(''),
            datasets: [{ data: Array.from(psd), borderColor: '#00ff00', tension: 0.1 }]
          }} options={{ animation: false }} />
        </div>
        <div>
          <h2>Controls</h2>
          <label>Frequency: {(freq/1e6).toFixed(1)} MHz<br/>
            <input type="range" min="24000000" max="1700000000" step="100000" value={freq}
                   onChange={e => { setFreq(+e.target.value); sendCommand(`FREQ:${+e.target.value}`); }} style={{width: '100%'}}/>
          </label><br/><br/>
          <label>Gain: {gain}<br/>
            <input type="range" min="0" max="496" step="1" value={gain}
                   onChange={e => { setGain(+e.target.value); sendCommand(`GAIN:${+e.target.value}`); }} style={{width: '100%'}}/>
          </label>
        </div>
      </div>
    </div>
  );
}

export default App;
