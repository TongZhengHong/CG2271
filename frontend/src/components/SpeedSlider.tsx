"use client";

import { useState } from "react";
import MovementButtonsVariableSpeed from "@/components/MovementButtonsVariableSpeed";
import { Button } from "@/components/Button";

const speedArrays = [5, 6, 7];

export default function SpeedSlider() {
  const [speed, setSpeed] = useState(7);

  return (
    <div className="flex gap-10">
      <div className="flex flex-col gap-4">
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
      </div>
      <MovementButtonsVariableSpeed speed={speed} />
    </div>
  );
}
