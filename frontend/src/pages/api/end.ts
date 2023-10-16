import { NextApiRequest, NextApiResponse } from "next";
import axios from "axios";

export default async function end(req: NextApiRequest, res: NextApiResponse) {
  if (req.method !== "POST") {
    return res.status(405).json({ message: "Method not allowed" });
  }
  axios.get(`http://${process.env.NEXT_PUBLIC_HOTSPOT_URL}/off`);
  return res.status(200).json({ message: "Off sent" });
}
