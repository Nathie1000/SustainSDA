package UI.Frame.Charts;

import java.util.ArrayList;
import java.util.List;

import javafx.scene.chart.XYChart;

public class WeekChartDataSet  extends DataSet{

	public String[] getLabels(){
		return new String[] {"Maandag", "Dinsdag", "Woensdag", "Donderdag", "Vrijdag", "Zaterdag", "Zondag"};
	}
	
	

	public WeekChartDataSet(){
		super();
	}
	
	
}
