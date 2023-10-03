import "dotenv/config";

import express from "express";
import axios from "axios";

var cors = require("cors");
const app = express();

app.use(express.json());
app.use(cors());

let HOTSPOT_URL: string;
if (process.env.HOTSPOT_URL) {
  HOTSPOT_URL = process.env.HOTSPOT_URL;
} else {
  throw new Error("HOTSPOT_URL is not defined");
}

app.get("/onRed", (req, res) => {
  const ESPResponse = axios.get(HOTSPOT_URL + "/onRed");
  res.send(ESPResponse);
});

app.listen(3000, () => {
  console.log("Backend server is running on port 3000");
});
