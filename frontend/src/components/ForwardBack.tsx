"use client";

import { Button } from "@/components/Button";
import axios from "axios";
import {
  LucideMoveUp,
  LucideMoveLeft,
  LucideMoveRight,
  LucideMoveDown,
  LucideMoveUpLeft,
  LucideMoveUpRight,
  LucideBan,
} from "lucide-react";

const TURN_DURATION_SECONDS = 0.2;

export default function MovementButtonsVariableSpeed({
  speed,
}: {
  speed: number;
}) {
  function forward() {
    axios.post("/api/motor", {
      motorX: speed,
      motorY: speed,
    });
  }

  function reverse() {
    axios.post("/api/motor", {
      motorX: -speed,
      motorY: -speed,
    });
  }

  function stop() {
    axios.post("/api/motor", {
      motorX: 0,
      motorY: 0,
    });
  }
  return (
    <div className="flex flex-col gap-4">
      <Button className="h-32 w-32" onTouchStart={forward} onTouchEnd={stop}>
        <LucideMoveUp />
      </Button>

      <Button className="h-32 w-32" onTouchStart={reverse} onTouchEnd={stop}>
        <LucideMoveDown />
      </Button>
    </div>
  );
}
