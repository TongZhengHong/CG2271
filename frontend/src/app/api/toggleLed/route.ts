import { isValidBody } from "@/lib/utils";
import axios from "axios";
import z from "zod";

const toggleLedSchema = z.object({
  ledState: z.boolean(),
});

export async function POST(req: Request) {
  const body = await req.json();
  if (!isValidBody(body, toggleLedSchema)) {
    return Response.json({ message: "Invalid body" });
  }
  if (body.ledState) {
    axios.get("http://" + process.env.NEXT_PUBLIC_HOTSPOT_URL + "/onRed");
    return Response.json({ message: "Red LED on" });
  } else {
    axios.get("http://" + process.env.NEXT_PUBLIC_HOTSPOT_URL + "/offRed");
    return Response.json({ message: "Red LED off" });
  }
}
