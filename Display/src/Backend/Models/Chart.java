package Backend.Models;

import java.util.HashMap;
import java.util.Map;

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

 public Map<String,Object> getChartDataHours(){
   return ChartAPI.retrieveHours();
 }
 public Map<String,Object> getChartDataWeek(){
   return ChartAPI.retrieveWeek();
 }
 public Map<String,Object> getChartDataMonth(){
   return ChartAPI.retrieveMonth();
 }
 public Map<String,Object> getChartDataYear(){
   return ChartAPI.retrieveYear();
 }

}
