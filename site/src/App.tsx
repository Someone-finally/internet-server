import { useState, useEffect } from "react";
import reactLogo from "./assets/react.svg";
import viteLogo from "/vite.svg";
import "./App.css";


function App() {
  const [btnState, setBtnState ] = useState(true);
  const [ledOn, setLedOn] = useState(false);

  useEffect(() => {
    webSocket();
  }, []);


  const getWebSocketUrl = (suffix: string) => {
    const l = window.location;
    return (
      (l.protocol == "https:" ? "wss://" : "ws://") +
      l.host +
      l.pathname +
      suffix
    );
  };

  const webSocket = () => {
    const socket = new WebSocket(getWebSocketUrl("ws/"));
    socket.onopen = () => {
      socket.send("Hello ESP32");
    };
    socket.onmessage = (event) => {
      console.log(event.data);
      try {
        const AttemptedBtnState = JSON.parse(event.data);
        setBtnState(AttemptedBtnState.btn_state);
      } finally {
      }
    };
    socket.onerror = (err) => console.error(err);
    socket.onclose = (event) => console.log(event);
  };

  const switchLed = async (is_on: boolean) => {
    const payload = { is_on };
    const webResult = await fetch("/api/toggle-led/", {
      method: "POST",
      body: JSON.stringify(payload),
    });
    if (!webResult.ok) {
      console.error(webResult.statusText);
      return;
    }
    setLedOn(is_on);
  };
  
  const getLEDText = () => (ledOn ? "LED is on" : "LED is off");


   return (
    <>
      <div>
        <a href="https://vitejs.dev" target="_blank">
          <img src={viteLogo} className="logo" alt="Vite logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <h1>Controlling esp32 via website</h1>
      <div className="card">
        <button
          style={{ background: ledOn ? "lightBlue" : "" }}
          onClick={() => switchLed(!ledOn)}
        >
          {getLEDText()}
        </button>
        <div
          style={{
            width: "70px",
            height: "70px",
            border: "2px solid black",
            margin: "auto",
            borderRadius: "10px",
            marginTop: "16px",
            background: btnState ? "purple" : "red",
          }}
        ></div>
      </div>
    </>
  );
}

export default App;