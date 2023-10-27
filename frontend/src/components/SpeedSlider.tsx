"use client";

import { useState } from "react";
import MovementButtonsVariableSpeed from "@/components/LeftRight";
import { Button } from "@/components/Button";
import OffButton from "@/components/OffButton";
import ForwardBack from "@/components/ForwardBack";
import LeftRight from "@/components/LeftRight";

const speedArrays = [5, 6, 7];

export default function SpeedSlider() {
  const [speed, setSpeed] = useState(7);

  return (
    <div className="flex w-full items-center justify-around">
      <ForwardBack speed={speed} />
      {/* <div className="flex flex-col gap-4">
        {speedArrays.map((buttonSpeed) => (
          <Button
            key={buttonSpeed}
            className={
              `text-2xl font-bold text-slate-800 ` +
              (speed === buttonSpeed ? "bg-green-600" : "text")
            }
            onClick={() => setSpeed(buttonSpeed)}
          >
            {buttonSpeed}
          </Button>
        ))}
      </div> */}
      <OffButton />
      <LeftRight speed={speed} />
    </div>
  );
}
