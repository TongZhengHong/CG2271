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

const TURN_DURATION_SECONDS = 0.25;

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

  function right() {
    axios.post("/api/motor", {
      motorX: speed,
      motorY: -speed,
    });
  }

  async function shortRight() {
    axios.post("/api/motor", {
      motorX: 8,
      motorY: -8,
    });
    await new Promise((resolve) =>
      setTimeout(resolve, TURN_DURATION_SECONDS * 1000)
    );
    axios.post("/api/motor", {
      motorX: 0,
      motorY: 0,
    });
  }

  function left() {
    axios.post("/api/motor", {
      motorX: -speed,
      motorY: speed,
    });
  }

  async function shortLeft() {
    axios.post("/api/motor", {
      motorX: -8,
      motorY: 8,
    });
    await new Promise((resolve) =>
      setTimeout(resolve, TURN_DURATION_SECONDS * 1000)
    );
    axios.post("/api/motor", {
      motorX: 0,
      motorY: 0,
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
        <Button className="rounded-full" onClick={shortLeft}>
          <LucideMoveUpLeft />
        </Button>
        <Button className="rounded-full" onClick={left}>
          <LucideMoveLeft />
        </Button>
        <Button className="rounded-full"></Button>
      </div>
      <div className="flex flex-col gap-4">
        <Button className="rounded-full" onClick={forward}>
          <LucideMoveUp />
        </Button>
        <Button className="rounded-full" onClick={stop}>
          <LucideBan />
        </Button>
        <Button className="rounded-full" onClick={reverse}>
          <LucideMoveDown />
        </Button>
      </div>
      <div className="flex flex-col gap-4">
        <Button className="rounded-full" onClick={shortRight}>
          <LucideMoveUpRight />
        </Button>
        <Button className="rounded-full" onClick={right}>
          <LucideMoveRight />
        </Button>
        <Button className="rounded-full"></Button>
      </div>
    </div>
  );
}
