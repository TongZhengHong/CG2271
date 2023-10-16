"use client";

import { Button } from "@/components/Button";
import axios from "axios";

const offRobot = async () => {
  const res = await axios.post("/api/off", { message: "test" });
  console.log(res);
};

export default function LedToggle() {
  return (
    <div className="flex flex-col gap-12">
      <Button
        className="w-20 h-20 bg-white text-black rounded-full"
        onClick={() => offRobot()}
      >
        OFF BOT
      </Button>
    </div>
  );
}
