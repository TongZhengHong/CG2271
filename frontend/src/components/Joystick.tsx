"use client";

import { Joystick as ReactJoystick } from "react-joystick-component";
import { useState, useEffect } from "react";
import debounce from "lodash.debounce";

const SENDING_WINDOW_SECONDS = 0.2;

export default function Joystick() {
  const [position, setPosition] = useState({ x: 0, y: 0 });
  const [lastSentPosition, setLastSentPosition] = useState({ x: 0, y: 0 });

  const [isNotNextSendingWindow, setIsNotNextSendingWindow] = useState(false);

  // delayed function to allow for next sending
  // const setTransitionNextSendingWindow = debounce(() => {
  //   setIsNotNextSendingWindow(false);
  // }, SENDING_WINDOW_SECONDS * 1000);

  useEffect(() => {
    // setTransitionNextSendingWindow();
    const interval = setInterval(() => {
      if (
        lastSentPosition.x === 0 &&
        lastSentPosition.y === 0 &&
        position.x === 0 &&
        position.y === 0
      ) {
        return;
      }
      console.log("sent");
      console.log(position);
      console.log("prev");
      console.log(lastSentPosition);
      setLastSentPosition(position);
    }, SENDING_WINDOW_SECONDS * 1000);
    return () => clearInterval(interval);
  }, [position, lastSentPosition]);

  return (
    <ReactJoystick
      move={(event) => setPosition({ x: event.x ?? 0, y: event.y ?? 0 })}
      stop={() => setPosition({ x: 0, y: 0 })}
    />
  );
}
