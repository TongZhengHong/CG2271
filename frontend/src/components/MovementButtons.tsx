"use client";

import { Button } from "@/components/Button";
import axios from "axios";

export default function MovementButtons() {
  function forward() {
    axios.post("/api/motor", {
      motorX: 7,
      motorY: 7,
    });
  }

  function reverse() {
    axios.post("/api/motor", {
      motorX: -6,
      motorY: -6,
    });
  }

  function right() {
    axios.post("/api/motor", {
      motorX: 6,
      motorY: -6,
    });
  }

  function left() {
    axios.post("/api/motor", {
      motorX: -6,
      motorY: 6,
    });
  }

  function smallRight() {
    axios.post("/api/motor", {
      motorX: 5,
      motorY: -5,
    });
  }

  function smallLeft() {
    axios.post("/api/motor", {
      motorX: -5,
      motorY: 5,
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
        <Button className="rounded-full" onClick={smallLeft}>
          Hi
        </Button>
        <Button className="rounded-full" onClick={left}>
          Hi
        </Button>
        <Button className="rounded-full">Hi</Button>
      </div>
      <div className="flex flex-col gap-4">
        <Button className="rounded-full" onClick={forward}>
          Hi
        </Button>
        <Button className="rounded-full" onClick={stop}>
          Hi
        </Button>
        <Button className="rounded-full" onClick={reverse}>
          Hi
        </Button>
      </div>
      <div className="flex flex-col gap-4">
        <Button className="rounded-full" onClick={smallRight}>
          Hi
        </Button>
        <Button className="rounded-full" onClick={right}>
          Hi
        </Button>
        <Button className="rounded-full">Hi</Button>
      </div>
    </div>
  );
}
