import pymysql

class MySQL:
    def __init__(self):
        self.db = pymysql.connect(host='localhost', user='bssmMrchoi', password='q1w2e3', db='webapp')
        self.cur = self.db.cursor()
        print("connect ok")

    def add(self, temp, hum):
        sql = "insert into record_dht(temperature, humidity) values('{0}', '{1}')".format(temp, hum)
        self.cur.execute(sql)
        self.db.commit()
        return [temp, hum]

    def selectAll(self):
        sql = "select * from record_dht"
        self.cur.execute(sql)
        result = self.cur.fetchall()
        return result
