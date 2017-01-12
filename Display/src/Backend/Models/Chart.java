package Backend.Models;

import java.awt.List;
import java.util.ArrayList;
import java.util.HashMap;
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

 public void getChartDataHours(){
	 JSONArray dataArray = null;
	 ChartAPI.retrieveHours().toJSONArray(dataArray);
	 
	   steps.clear();

	   for(int i = 0;i<dataArray.length();i++){
		   steps.add((Integer) dataArray.get(i));
	   }
 }
 public void getChartDataWeek(){
	   data = ChartAPI.retrieveWeek();

	    JSONArray stepstemp = null;
	    JSONArray goalstemp = null;
	    try{
	    	stepstemp = data.getJSONArray("steps");
	    	goalstemp = data.getJSONArray("goals");
	    } catch(Exception e){
	    	e.printStackTrace();
	    }

	   steps.clear();
	   goals.clear();

	   for(int i = 0;i<stepstemp.length();i++){
		   steps.add((Integer) stepstemp.get(i));
	   }
	   for(int i = 0;i<goalstemp.length();i++){
		   goals.add((Integer) goalstemp.get(i));
	   }
		
	 }
 public void getChartDataMonth(){
	   data = ChartAPI.retrieveMonth();

	    JSONArray stepstemp = null;
	    JSONArray goalstemp = null;
	    try{
	    	stepstemp = data.getJSONArray("steps");
	    	goalstemp = data.getJSONArray("goals");
	    } catch(Exception e){
	    	e.printStackTrace();
	    }

	   steps.clear();
	   goals.clear();

	   for(int i = 0;i<stepstemp.length();i++){
		   steps.add((Integer) stepstemp.get(i));
	   }
	   for(int i = 0;i<goalstemp.length();i++){
		   goals.add((Integer) goalstemp.get(i));
	   }
		
	 }
 public void getChartDataYear(){
	   data = ChartAPI.retrieveYear();
	   System.out.println(data);
	    JSONArray stepstemp = null;
	    JSONArray goalstemp = null;
	    try{
	    	data.getJSONObject("steps").toJSONArray(stepstemp);
	    	data.getJSONObject("goals").toJSONArray(goalstemp);
	    } catch(Exception e){
	    	e.printStackTrace();
	    }

	   steps.clear();
	   goals.clear();

	   for(int i = 0;i<stepstemp.length();i++){
		   steps.add((Integer) stepstemp.get(i));
	   }
	   for(int i = 0;i<goalstemp.length();i++){
		   goals.add((Integer) goalstemp.get(i));
	   }
	   System.out.println(steps);
	   System.out.println(goals);
		
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
