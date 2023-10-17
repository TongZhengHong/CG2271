"use client";

import { Joystick as ReactJoystick } from "react-joystick-component";
import { useState, useEffect } from "react";
import axios from "axios";

const SENDING_WINDOW_SECONDS = 0.1;

export default function Joystick() {
  const [position, setPosition] = useState({ x: 0, y: 0 });
  const [lastSentPosition, setLastSentPosition] = useState({ x: 0, y: 0 });

  // const [isNotNextSendingWindow, setIsNotNextSendingWindow] = useState(false);

  // delayed function to allow for next sending
  // const setTransitionNextSendingWindow = debounce(() => {
  //   setIsNotNextSendingWindow(false);
  // }, SENDING_WINDOW_SECONDS * 1000);

  useEffect(() => {
    // setTransitionNextSendingWindow();
    const interval = setInterval(() => {
      if (
        lastSentPosition.x === position.x &&
        lastSentPosition.y === position.y
      ) {
        return;
      }
      const x = position.x;
      const y = position.y;
      const motorLeft = Math.floor(
        (y - Math.abs(x / 2) * y + (x - Math.abs(y / 2) * x)) * 8
      );
      const motorRight = Math.floor(
        (y - Math.abs(x / 2) * y - (x - Math.abs(y / 2) * x)) * 8
      );
      axios.post("/api/motor", {
        motorX: motorLeft,
        motorY: motorRight,
      });
    }, SENDING_WINDOW_SECONDS * 1000);
    return () => clearInterval(interval);
  }, [position, lastSentPosition]);

  return (
    <ReactJoystick
      move={(event) => setPosition({ x: event.x ?? 0, y: event.y ?? 0 })}
      stop={() => {
        setPosition({ x: 0, y: 0 });
        axios.post("/api/motor", {
          motorX: 0,
          motorY: 0,
        });
      }}
    />
  );
}
