package UI.Frame;

import java.util.ArrayList;
import java.util.List;

import javafx.scene.chart.XYChart;

public class WeekChartDataSet {
	public static String[] DAYS = {"Maandag", "Dinsdag", "Woensdag" , "Vrijdag", "Zaterdag", "Zondag"};
	
	public List<XYChart.Data<String,Number>> steps = new ArrayList<>();
	public List<XYChart.Data<String,Number>> goals = new ArrayList<>();
	
	public WeekChartDataSet(){
		for(String day : DAYS){
			steps.add(new XYChart.Data<String,Number>(day, 0));
			goals.add(new XYChart.Data<String,Number>(day, 0));
			
			

		}
	}
	
	public void setStep(int index, int value){
		steps.get(index).setYValue(value);
	}
	
	public void setGoal(int index, int value){
		goals.get(index).setYValue(value);
	}
	
	public List<XYChart.Data<String, Number>> getSteps() {
		return steps;
	}
	
	public List<XYChart.Data<String, Number>> getGoals() {
		return goals;
	}
}
