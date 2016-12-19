package Backend.API;

import java.util.Map;

import org.json.*;

import Backend.Models.Patient;
public class PatientAPI extends API {

	public static Patient retrieve(String shdNumber) {
		System.out.println("balalalala");
		try{
			JSONObject json = apiCall("http://localhost:3000/api/shd/patient/"+shdNumber);
			Patient p = new Patient(json.getString("firstName"),json.getString("lastName"),json.getInt("battery"), 0);
			return p;
		} catch(Exception e){
			System.out.println("Exception: ");
			System.out.println(e);
		}
		return null;

	}

	public static JSONObject retrieveProgress(int id) {
		JSONObject json = retrieveByUrl("http://localhost:3000/api/shd/getProgressData/1");
		System.out.println(json);
		return json;
	}

}
