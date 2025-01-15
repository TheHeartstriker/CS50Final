import { useState, useEffect, useRef } from "react";
import SDLModule from "./WebAsm/sdl_app.mjs?init";
import { use } from "react";

//Calls the module and passes the canvas element
const loadWasm = async (canvas) => {
  const wasm = await SDLModule({
    canvas,
    wasi_snapshot_preview1: {
      proc_exit: (code) => {
        console.log(`WASM exited with code ${code}`);
      },
    },
  });

  //wasm._setArguments(200, 200);

  return wasm;
};
function Ball() {
  const [wasm, setWasm] = useState(null);
  const canvasRef = useRef(null);
  const isLoaded = useRef(false);

  useEffect(() => {
    const fetchWasm = async () => {
      if (canvasRef.current && !wasm && !isLoaded.current) {
        const wasmModule = await loadWasm(canvasRef.current);
        setWasm(wasmModule);
        isLoaded.current = true;
      }
    };
    fetchWasm();
  }, [wasm]);

  return (
    <div>
      <canvas ref={canvasRef} id="canvas"></canvas>
    </div>
  );
}

export default Ball;
