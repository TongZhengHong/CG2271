import Joystick from "@/components/Joystick";
import LedToggle from "@/components/LedToggle";
import OffButton from "@/components/OffButton";

export default function Home() {
  return (
    <main className="h-screen flex flex-col items-center p-20">
      {/* <LedToggle /> */}
      <OffButton />
      <div className="flex items-center justify-center flex-1">
        <Joystick />
      </div>
    </main>
  );
}
