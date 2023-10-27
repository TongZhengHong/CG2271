"use client";

import { Button } from "@/components/Button";
import axios from "axios";
import {
  LucideMoveLeft,
  LucideMoveRight,
  LucideMoveUpLeft,
  LucideMoveUpRight,
  LucideCornerUpLeft,
  LucideCornerUpRight,
} from "lucide-react";

const TURN_DURATION_SECONDS = 0.2;

export default function MovementButtonsVariableSpeed({
  speed,
}: {
  speed: number;
}) {
  function right() {
    axios.post("/api/motor", {
      motorX: speed,
      motorY: -speed,
    });
  }

  function turnRight() {
    axios.post("/api/motor", {
      motorX: 8,
      motorY: 2,
    });
  }

  async function curveRight() {
    await axios.post("/api/motor", {
      motorX: 8,
      motorY: 5,
    });
  }

  function left() {
    axios.post("/api/motor", {
      motorX: -speed,
      motorY: speed,
    });
  }

  function turnLeft() {
    axios.post("/api/motor", {
      motorX: 2,
      motorY: 8,
    });
  }

  async function curveLeft() {
    await axios.post("/api/motor", {
      motorX: 5,
      motorY: 8,
    });
  }

  function stop() {
    axios.post("/api/motor", {
      motorX: 0,
      motorY: 0,
    });
  }
  return (
    <div className="flex gap-4">
      <div className="flex flex-col gap-4">
        <Button className="" onTouchStart={curveLeft} onTouchEnd={stop}>
          <LucideMoveUpLeft />
        </Button>
        <Button
          className=" cursor-pointer"
          onTouchStart={left}
          onTouchEnd={stop}
        >
          <LucideMoveLeft />
        </Button>
        <Button className=" text-sm" onTouchStart={turnLeft} onTouchEnd={stop}>
          <LucideCornerUpLeft />
        </Button>
      </div>
      <div className="flex flex-col gap-4">
        <Button className="" onTouchStart={curveRight} onTouchEnd={stop}>
          <LucideMoveUpRight />
        </Button>
        <Button className="" onTouchStart={right} onTouchEnd={stop}>
          <LucideMoveRight />
        </Button>
        <Button className=" text-sm" onTouchStart={turnRight} onTouchEnd={stop}>
          <LucideCornerUpRight />
        </Button>
      </div>
    </div>
  );
}
