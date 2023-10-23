import OffButton from "@/components/OffButton";
import SpeedSlider from "@/components/SpeedSlider";

export default function Home() {
  return (
    <main className="h-screen flex items-center justify-center overflow-hidden gap-16">
      {/* <LedToggle /> */}
      <OffButton />
      <div className="flex items-center justify-center">
        {/* <Joystick /> */}
        <SpeedSlider />
      </div>
    </main>
  );
}
