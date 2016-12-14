package Backend.API;

import Backend.Models.Patient;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.HashMap;
import java.util.Map;
import org.json.*;

public class ChartAPI extends API {

	public static Map<String, Integer> retrieveChart(int index) {
		switch(index){
			case 1:
				return retrieveHours();
			case 2:
				return retrieveWeek();
			case 3:
				return retrieveMonth();
			case 4:
				return retrieveYear();
			default:
				return null;
		}
	}


	public static Map<String, Integer> retrieveHours() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataHours/1");
	}
	public static Map<String, Integer> retrieveWeek() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataWeek/1");
	}
	public static Map<String, Integer> retrieveMonth() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataMonth/1");
	}
	public static Map<String, Integer> retrieveYear() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataYear/1");
	}

	private static Map<String,Integer> retrieveByUrl(String url){
		try{
			JSONObject json = apiCall(url);//+shdNumber);
			return JSONToMap(json);
		} catch(Exception e){
			System.out.println("Exception: ");
			System.out.println(e);
			e.printStackTrace();
//			System.out.println();
		}
		return null;
	}

	private static Map<String,Integer> JSONToMap(JSONObject json){
		Map<String,Integer> map = new HashMap<>();
		for (String key : json.keySet()) {
			Integer value = (Integer) json.get(key);
			map.put(key,value);
		 }
		 return map;
	}

}
