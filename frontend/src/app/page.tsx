"use client";

import { Button } from "@/components/Button";
import axios from "axios";

const onLED = async () => {
  axios.get("http://localhost:8000/onRed");
};

const offLED = async () => {
  axios.get("http://localhost:8000/offRed");
};

export default function Home() {
  return (
    <main className="flex min-h-screen flex-col items-center justify-between p-24">
      <div className="flex flex-col gap-12">
        <Button
          className="w-20 h-20 bg-white text-black rounded-full"
          onClick={() => onLED()}
        >
          onLED
        </Button>
        <Button
          className="w-20 h-20 bg-white text-black rounded-full"
          onClick={() => offLED()}
        >
          offLED
        </Button>
      </div>
    </main>
  );
}
