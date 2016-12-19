package Backend.Models;

import java.util.HashMap;
import java.util.Map;

import org.json.JSONObject;

import Backend.API.ChartAPI;

public class Chart extends Model {

  private Map<String,Integer> steps = new HashMap<>();

	/**
	* Default empty Chart constructor
	*/
	public Chart() {
		super();
	}

	/**
	* Default Chart constructor
	*/
	public Chart(Map<String,Integer> stepsMap) {
		super();
		this.steps = stepsMap;
	}

 public JSONObject getChartDataHours(){
   return ChartAPI.retrieveHours();
 }
 public JSONObject getChartDataWeek(){
   return ChartAPI.retrieveWeek();
 }
 public JSONObject getChartDataMonth(){
   return ChartAPI.retrieveMonth();
 }
 public JSONObject getChartDataYear(){
   return ChartAPI.retrieveYear();
 }

}
