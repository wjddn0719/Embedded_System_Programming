import RPi.GPIO as GPIO
from time import sleep

pin = 16

GPIO.setmode(GPIO.BOARD)
GPIO.setup(pin, GPIO.OUT)
servo = GPIO.PWM(pin, 50)

servo.start(0)

def setAngle(angle):
    duty = 2.5 + 10 * angle / 180
    print(f"degree : {angle} to {duty}(duty)")
    servo.ChangeDutyCycle(duty)
