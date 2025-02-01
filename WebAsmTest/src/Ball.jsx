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
  return wasm;
};
function Ball() {
  const [wasm, setWasm] = useState(null);
  const canvasRef = useRef(null);
  const isLoaded = useRef(false);
  const [AniType, setAniType] = useState(1);
  const [windowSize, setWindowSize] = useState({
    width: window.innerWidth,
    height: window.innerHeight,
  });
  //load wasm module also loads main function
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
  //Window size event listener
  useEffect(() => {
    window.addEventListener("resize", () => {
      setWindowSize({ width: window.innerWidth, height: window.innerHeight });
    });

    return () => {
      window.removeEventListener("resize", () => {});
    };
  }, []);
  //Set arguments for the wasm module
  useEffect(() => {
    if (wasm) {
      wasm._setArguments(window.innerHeight, window.innerWidth, AniType);
    }
  }, [windowSize, wasm, AniType]);

  return (
    <div>
      <canvas ref={canvasRef} id="canvas"></canvas>
      <button className="btn" onClick={() => setAniType(1)}>
        Linear
      </button>
      <button className="btn" onClick={() => setAniType(2)}>
        Quadratic
      </button>
    </div>
  );
}

export default Ball;
