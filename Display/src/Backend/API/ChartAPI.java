package Backend.API;

import java.util.HashMap;
import java.util.Map;
import org.json.*;

public class ChartAPI extends API {

	public static JSONObject retrieveChart(int index) {
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


	public static JSONObject retrieveHours() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataHours/1");
	}
	public static JSONObject retrieveWeek() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataWeek/1");
	}
	public static JSONObject retrieveMonth() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataMonth/1");
	}
	public static JSONObject retrieveYear() {
		return retrieveByUrl("http://localhost:3000/api/shd/getChartDataYear/1");
	}




}
