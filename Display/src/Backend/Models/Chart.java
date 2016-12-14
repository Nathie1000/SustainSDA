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

 public static Map<String,Integer> getChartDataHours(){
   return ChartAPI.retrieveYear();
 }

}
