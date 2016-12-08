package Backend.API;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import org.json.*;

import Backend.Models.Patient;
public class PatientAPI extends API {

	public Patient retrieve(String shdNumber) {
		System.out.println("balalalala");
		try{
			JSONObject json = apiCall("http://localhost:3000/api/shd/patient/"+shdNumber);
			Patient p = new Patient(json.getString("firstName"),json.getString("lastName"),json.getInt("battery"));
			return p;
		} catch(Exception e){
			System.out.println("Exception: ");
			System.out.println(e);
		}
		return null;

	}

}
