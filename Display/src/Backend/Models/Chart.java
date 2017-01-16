package Backend.Models;

import java.util.ArrayList;
import org.json.JSONArray;
import org.json.JSONObject;

import Backend.API.ChartAPI;

public class Chart extends Model {

	private JSONObject data = new JSONObject();

	public JSONObject getData() {
		return data;
	}

	public void setData(JSONObject data) {
		this.data = data;
	}

	private ArrayList<Integer> steps = new ArrayList<Integer>();
	private ArrayList<Integer> goals = new ArrayList<Integer>();

	/**
	 * Default empty Chart constructor
	 */
	public Chart() {
		super();
	}

	public void getChartDataHours() {
		JSONArray dataArray = new JSONArray();
		ChartAPI.retrieveHours().toJSONArray(dataArray);

		steps.clear();

		for (int i = 0; i < dataArray.length(); i++) {
			steps.add((Integer) dataArray.get(i));
		}
	}

	public void getChartDataWeek() {
		data = ChartAPI.retrieveWeek();
		//If no connection data will be null. This is a safty check to avaoid null poirters later on.
		if(data == null) return;

		//Do not go and declare null out of scoop.
		//If a exception happens stepstemp and goalstemp will be null and can not be
		//referenced later the get the .lenght() in the for loop.
		try {
			JSONArray stepstemp = data.getJSONArray("steps");
			JSONArray goalstemp = data.getJSONArray("goals");

			steps.clear();
			goals.clear();

			for (int i = 0; i < stepstemp.length(); i++) {
				steps.add((Integer) stepstemp.get(i));
			}
			for (int i = 0; i < goalstemp.length(); i++) {
				goals.add((Integer) goalstemp.get(i));
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getChartDataMonth() {
		data = ChartAPI.retrieveMonth();
		if(data == null) return;

		try {
			JSONArray stepstemp = data.getJSONArray("steps");
			JSONArray goalstemp = data.getJSONArray("goals");
			
			steps.clear();
			goals.clear();
			
			for (int i = 0; i < stepstemp.length(); i++) {
				steps.add((Integer) stepstemp.get(i));
			}
			for (int i = 0; i < goalstemp.length(); i++) {
				goals.add((Integer) goalstemp.get(i));
			}
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void getChartDataYear() {
		data = ChartAPI.retrieveYear();
		System.out.println(data);
		if(data == null) return;
		
		try {
			//You forgot to assign the result from data to stepstemp and goalstemp. 
			//Thas's why it wasn't working.
			JSONArray stepstemp = new JSONArray();
			stepstemp = data.getJSONObject("steps").toJSONArray(stepstemp);
			JSONArray goalstemp = new JSONArray();
			goalstemp = data.getJSONObject("goals").toJSONArray(goalstemp);
			
			steps.clear();
			goals.clear();

			for (int i = 0; i < stepstemp.length(); i++) {
				steps.add((Integer) stepstemp.get(i));
			}
			for (int i = 0; i < goalstemp.length(); i++) {
				goals.add((Integer) goalstemp.get(i));
			}
			System.out.println(steps);
			System.out.println(goals);
			
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public ArrayList<Integer> getSteps() {
		return steps;
	}

	public void setSteps(ArrayList<Integer> steps) {
		this.steps = steps;
	}

	public ArrayList<Integer> getGoals() {
		return goals;
	}

	public void setGoals(ArrayList<Integer> goals) {
		this.goals = goals;
	}

}
