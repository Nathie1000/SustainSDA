package UI.Frame.Charts;

import java.util.ArrayList;
import java.util.List;

import javafx.scene.chart.XYChart;

public class DataSet {

	public List<XYChart.Data<String,Number>> steps = new ArrayList<>();
	public List<XYChart.Data<String,Number>> goals = new ArrayList<>();

	protected String[] getLabels(){
		return null;
	}
	
	public DataSet(){
		System.out.println("in dataset constr");
		String[] labels = getLabels();
		for(String day : labels){
			steps.add(new XYChart.Data<String,Number>(day, 0));
			goals.add(new XYChart.Data<String,Number>(day, 0));
		}
	}

	public void setData(ArrayList steps, ArrayList goals){
		for(int i = 0;i<steps.size();i++){
			setStep(i,(Integer)steps.get(i));
		}
		for(int i = 0;i<goals.size();i++){
			setGoal(i,(Integer)goals.get(i));
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
