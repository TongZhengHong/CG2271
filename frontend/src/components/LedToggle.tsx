"use client";

import { Button } from "@/components/Button";
import axios from "axios";

const LED = async (state: boolean) => {
  const res = await axios.post("/api/toggleLed", { ledState: state });
  console.log(res);
};

export default function LedToggle() {
  return (
    <div className="flex flex-col gap-12">
      <Button
        className="w-20 h-20 bg-white text-black rounded-full"
        onClick={() => LED(true)}
      >
        ON
      </Button>
      <Button
        className="w-20 h-20 bg-white text-black rounded-full"
        onClick={() => LED(false)}
      >
        OFF
      </Button>
    </div>
  );
}
