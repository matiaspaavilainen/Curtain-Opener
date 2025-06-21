from fastapi import FastAPI, Request
from datetime import datetime
from random import choice
from pydantic import BaseModel

app = FastAPI()

alarm_status = True
tmps = [19.6, 21.4, 22.5, 27.8, 23.0]
hums = [50.2, 61.4, 5.5, 16.8, 34.0]

opening_times = ["10:00", "07:30", "07:30", "07:30", "07:30", "07:30", "10:00"]


class OpenTimes(BaseModel):
    openingTimes: list


@app.get("/")
async def root():
    return {"message": "arduino ready"}


@app.get("/status")
async def status():
    current_timestamp = int(datetime.now().timestamp())
    stats = {
        "alarmStatus": alarm_status,
        "time": current_timestamp,
        "temperature": choice(tmps),
        "humidity": choice(hums),
        "openingTimes": opening_times,
    }
    return stats


@app.get("/alarm")
async def alarm():
    global alarm_status
    alarm_status = not alarm_status
    return alarm_status


@app.post("/set/time")
async def setTime(request: Request):
    body = await request.body()
    print(body)
    timestamp = int(body.decode("utf-8"))
    print(timestamp)
    print(datetime.fromtimestamp(timestamp))


@app.post("/set/open")
async def setOpen(open: OpenTimes):
    global opening_times
    print(open)
    opening_times = open
    return "Times set!"


@app.post("/move")
async def move_manually(request: Request):
    body = await request.body()
    input_str = body.decode("utf-8")

    # Print debug info
    print(f"Manual move: {input_str}")

    if input_str.startswith("-"):
        # Negative value means "close"
        return "-1"
    else:
        # Positive value means "open"
        return "1"
