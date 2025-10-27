# test.py

import adafruit_dht
import board

sensor = adafruit_dht.DHT11(board.D14) # GPIO 14를 뜻함

# 자신의 파이썬 버전이 3.13이면 아래 코드로 대체
# sensor = adafruit_dht.DHT11(board.D14, use_pulseio=False)


temp = sensor.temperature
hum = sensor.humidity

print([temp, hum])
