package Backend.API;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import org.json.JSONException;
import org.json.JSONObject;

import Backend.Models.Patient;
import IO.Sim;

public class API {
	private static final String HOST 		= "http://82.217.103.246/";
	private static final String PATIENT		= "api/shd/patient/";
	private static final String PROGRESS	= "api/shd/getProgressData/";
	private static final String CHART_HOURS = "api/shd/getChartDataHours/";
	private static final String CHART_WEEK 	= "api/shd/getChartDataWeek/";
	private static final String CHART_MONTH = "api/shd/getChartDataMonth/";
	private static final String CHART_YEAR 	= "api/shd/getChartDataYear/";

	public static JSONObject apiCall(String url) {
		JSONObject obj = new JSONObject();
		try {
			URL myurl = new URL(url);
			HttpURLConnection con = (HttpURLConnection) myurl.openConnection();
			
			//Read
			BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
			obj = new JSONObject(in.readLine());
			
			in.close();
			return obj;
			
		} catch (JSONException | IOException e) {
			System.out.println("Connection error: " + e.getMessage());
		}
		
		return obj;
	}
	
	
	public static Patient retrievePatient() {
		try{
			String ccid = Sim.getCcid();
			JSONObject json = apiCall(HOST + PATIENT + ccid);
			int id = json.getInt("id");
			String firstName = json.getString("firstName");
			String lastName = json.getString("lastName");
			int battery = json.getInt("battery");
			return new Patient(ccid, id, firstName, lastName, battery);
		} catch(JSONException e){
			System.out.println("Exception: " + e.getMessage());
		}
		return null;
	}

	public static JSONObject retrievePatientProgress(Patient patient) {
		return API.apiCall(HOST + PROGRESS + patient.getId());
	}
		
	public static JSONObject retrieveChartHours(Patient patient) {
		return API.apiCall(HOST + CHART_HOURS + patient.getId());
	}
	
	public static JSONObject retrieveChartWeek(Patient patient) {
		return API.apiCall(HOST + CHART_WEEK + patient.getId());
	}
	
	public static JSONObject retrieveChartMonth(Patient patient) {
		return API.apiCall(HOST + CHART_MONTH + patient.getId());
	}
	
	public static JSONObject retrieveChartYear(Patient patient) {
		return API.apiCall(HOST + CHART_YEAR + patient.getId());
	}
}
