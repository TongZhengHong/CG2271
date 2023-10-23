"use client";

import { Joystick as ReactJoystick } from "react-joystick-component";
import { useState, useEffect } from "react";
import axios from "axios";

const SENDING_WINDOW_SECONDS = 1;

const fourBitMapper = new Map();

fourBitMapper.set(0, "0000");
fourBitMapper.set(1, "0001");
fourBitMapper.set(2, "0010");
fourBitMapper.set(3, "0011");
fourBitMapper.set(4, "0100");
fourBitMapper.set(5, "0101");
fourBitMapper.set(6, "0110");
fourBitMapper.set(7, "0111");
fourBitMapper.set(8, "0111");
fourBitMapper.set(-1, "1001");
fourBitMapper.set(-2, "1010");
fourBitMapper.set(-3, "1011");
fourBitMapper.set(-4, "1100");
fourBitMapper.set(-5, "1101");
fourBitMapper.set(-6, "1110");
fourBitMapper.set(-7, "1111");
fourBitMapper.set(-8, "1111");

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
      console.log("motor move");
      axios.get(
        `http://${
          process.env.NEXT_PUBLIC_HOTSPOT_URL
        }/motor=${fourBitMapper.get(motorLeft)}${fourBitMapper.get(motorRight)}`
      );
      // axios.post("/api/motor", {
      //   motorX: motorLeft,
      //   motorY: motorRight,
      // });
    }, SENDING_WINDOW_SECONDS * 1000);
    return () => clearInterval(interval);
  }, [position, lastSentPosition]);

  return (
    <ReactJoystick
      move={(event) => setPosition({ x: event.x ?? 0, y: event.y ?? 0 })}
      stop={() => {
        setPosition({ x: 0, y: 0 });
        axios.get(
          `http://${process.env.NEXT_PUBLIC_HOTSPOT_URL}/motor=00000000`
        );
      }}
    />
  );
}
