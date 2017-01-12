package UI.Frame.Charts;

import java.util.ArrayList;
import java.util.List;

import javafx.scene.chart.XYChart;

public class YearChartDataSet  extends DataSet{

	public String[] getLabels(){
		return new String[] {"Januari", "Februari", "Maart", "April", "Mei", "Juni", "Juli","Augustus","September","Oktober","November","December"};
	}
	
	

	public YearChartDataSet(){
		super();
	}
	
	
}
