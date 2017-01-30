package UI.Frame.Charts;

import java.util.ArrayList;
import java.util.List;

import javafx.scene.chart.XYChart;

public abstract class DataSet {
	
	private List<XYChart.Data<String,Number>> steps = new ArrayList<>();
	private List<XYChart.Data<String,Number>> goals = new ArrayList<>();

	public DataSet(){
		String[] labels = getLabels();
		for(String day : labels){
			steps.add(new XYChart.Data<String,Number>(day, 0));
			goals.add(new XYChart.Data<String,Number>(day, 0));
		}
	}
	
	public abstract String[] getLabels();

	public void setData(ArrayList<Integer> steps, ArrayList<Integer> goals){
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
