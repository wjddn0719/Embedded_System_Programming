import adafruit_dht
import board

def get_now():
    sensor = adafruit_dht.DHT11(board.D18)
    temp = sensor.temperature
    hum = sensor.humidity
    return [temp, hum]
