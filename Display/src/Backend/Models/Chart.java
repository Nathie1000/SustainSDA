package Backend.Models;

import java.util.ArrayList;
import org.json.JSONArray;
import org.json.JSONObject;

import Backend.API.API;

public class Chart{

	private ArrayList<Integer> steps = new ArrayList<Integer>();
	private ArrayList<Integer> goals = new ArrayList<Integer>();

	public void fetchChartDataHours(Patient patient) {
		JSONObject data = API.retrieveChartHours(patient); //API.retrieveChartHours(patient).toJSONArray(dataArray);
		if(data.has("steps")){
			steps.clear();
			goals.clear();
			JSONArray dataArray = data.getJSONArray("steps");
			for (int i = 0; i < dataArray.length(); i++) {
				steps.add(dataArray.getInt(i));
			}
		}
	}

	public void fetchChartDataWeek(Patient patient) {
		JSONObject data = API.retrieveChartWeek(patient);
		if(data.has("steps") && data.has("goals")){
			JSONArray stepstemp = data.getJSONArray("steps");
			JSONArray goalstemp = data.getJSONArray("goals");
			
			steps.clear();
			goals.clear();
	
			for (int i = 0; i < stepstemp.length(); i++) {
				steps.add(stepstemp.getInt(i));
			}
			for (int i = 0; i < goalstemp.length(); i++) {
				goals.add(goalstemp.getInt(i));
			}
		}
	}

	public void fetchChartDataMonth(Patient patient) {
		JSONObject data = API.retrieveChartMonth(patient);
		
		if(data.has("steps") && data.has("goals")){
			JSONArray stepstemp = data.getJSONArray("steps");
			JSONArray goalstemp = data.getJSONArray("goals");
			steps.clear();
			goals.clear();
			
			for (int i = 0; i < stepstemp.length(); i++) {
				steps.add(stepstemp.getInt(i));
			}
			for (int i = 0; i < goalstemp.length(); i++) {
				goals.add(goalstemp.getInt(i));
			}
		}
	}

	public void fetchChartDataYear(Patient patient) {
		JSONObject data = API.retrieveChartYear(patient);
		
		if(data.has("steps") && data.has("goals")){
			JSONArray stepstemp = data.getJSONArray("steps");
			JSONArray goalstemp = data.getJSONArray("goals");
			
			steps.clear();
			goals.clear();
	
			for (int i = 0; i < stepstemp.length(); i++) {
				steps.add(stepstemp.getInt(i));
			}
			for (int i = 0; i < goalstemp.length(); i++) {
				goals.add(goalstemp.getInt(i));
			}
		}
	}

	public ArrayList<Integer> getSteps() {
		return steps;
	}

	public ArrayList<Integer> getGoals() {
		return goals;
	}
}
