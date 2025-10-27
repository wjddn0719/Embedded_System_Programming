from flask import Flask, render_template, jsonify
from db_model import MySQL
import sensor_dht

app = Flask(__name__)
db = MySQL()  # ✅ 클래스 인스턴스 생성

@app.route("/")
def home():
    return render_template("index.html")

@app.route("/api/now")
def insertdb():
    temp, hum = sensor_dht.get_now()
    db.add(temp, hum)
    return jsonify({"temp": temp, "hum": hum})  # ✅ JSON으로 응답

@app.route("/api/record")
def selectAll():
    result = db.selectAll()
    return jsonify(result)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
