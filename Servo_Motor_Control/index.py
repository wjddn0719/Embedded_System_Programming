from flask import Flask, request, render_template, jsonify
import RPi.GPIO as GPIO
from time import sleep
import pymysql
from servo import setAngle


app = Flask(__name__)

db = pymysql.connect(
	host = "localhost",
	user = "root",
	password = "1234",
	database = "study_db",
	charset = "utf8mb4",
	cursorclass=pymysql.cursors.DictCursor
)

@app.route('/')
def index():
    return render_template('index.html')  

@app.route('/api/angle', methods=['POST'])

def control_servo():
    print("/api/angle 요청 들어옴")
    data = request.get_json()
    print("요청데이터: ", data)
    angle = data.get('angle')  # POST 요청에서 'angle' 값을 가져옴

    if angle is not None:
        cursor = db.cursor()
        sql = "INSERT INTO record_angle (angle) VALUES (%s)"
        cursor.execute(sql, (angle,))
        db.commit()

        setAngle(int(angle))  # 서보 모터 각도 설정
        return jsonify({'message': 'angle set to {}'.format(angle)})
    else:
        return jsonify({'message': 'fail! check your parameter'})


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
