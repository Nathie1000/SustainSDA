import requests
import json

class Api:
    url = "http://145.89.152.226:3000"
    endPointPatientInfo = "/api/shd/getPatientInfo"
    endPointChartData = "/api/shd/getChartData"
    phoneNumber = "/";
    
    def __init__(self, phoneNumber):
        self.phoneNumber += phoneNumber
    
    #Get the patient information.
    #Returns a dictionary containing info.
    def getPatientInfo(self):
        r = requests.get(self.url + self.endPointPatientInfo + self.phoneNumber)
        return json.loads(r.json())
    
    #Get the patient chart data.
    #Returns a dictionary containing info.     
    def getChartData(self):
        r = requests.get(self.url + self.endPointChartData + self.phoneNumber)
        return json.loads(r.json())

    #Test function, needs to be removed later.
    def testSendPatientData(self):
        data = {}
        data["steps"] = 5;
        data["beltNumber"] = "0612121212";
        data["battery"] = 100;
        
        r = requests.post(self.url + "/api/belt/sendPatientData", json=data)
        print(r.status_code)
        print(r.text)

api = Api("0612121212")
api.testSendPatientData()


