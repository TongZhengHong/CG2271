import { NextApiRequest, NextApiResponse } from "next";
import axios from "axios";
import { z } from "zod";
import { isValidBody } from "@/lib/utils";

const motorSpeedSchema = z.object({
  motorX: z.number().min(-8).max(8),
  motorY: z.number().min(-8).max(8),
});

const fourBitMapper = new Map();

fourBitMapper.set(0, "0000");
fourBitMapper.set(1, "0001");
fourBitMapper.set(2, "0010");
fourBitMapper.set(3, "0011");
fourBitMapper.set(4, "0100");
fourBitMapper.set(5, "0101");
fourBitMapper.set(6, "0110");
fourBitMapper.set(7, "0111");
fourBitMapper.set(8, "0111");
fourBitMapper.set(-1, "1001");
fourBitMapper.set(-2, "1010");
fourBitMapper.set(-3, "1011");
fourBitMapper.set(-4, "1100");
fourBitMapper.set(-5, "1101");
fourBitMapper.set(-6, "1110");
fourBitMapper.set(-7, "1111");
fourBitMapper.set(-8, "1111");

export default async function motor(req: NextApiRequest, res: NextApiResponse) {
  if (req.method !== "POST") {
    return res.status(405).json({ message: "Method not allowed" });
  }
  if (!isValidBody(req.body, motorSpeedSchema)) {
    return res.status(400).json({ message: "Invalid body" });
  }
  const { motorX, motorY } = req.body;
  console.log(req.body);
  console.log(
    `http://${process.env.NEXT_PUBLIC_HOTSPOT_URL}/motor=${fourBitMapper.get(
      motorX
    )}${fourBitMapper.get(motorY)}`
  );
  axios.get(
    `http://${process.env.NEXT_PUBLIC_HOTSPOT_URL}/motor=${fourBitMapper.get(
      motorX
    )}${fourBitMapper.get(motorY)}`
  );
  return res.status(200).json({ message: "Motor value sent" });
}
