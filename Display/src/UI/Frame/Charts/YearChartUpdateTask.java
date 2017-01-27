package UI.Frame.Charts;

import java.util.concurrent.TimeUnit;
import Backend.Models.Chart;
import Backend.Models.Patient;
import UI.Frame.RepetitiveUpdateTask;


public class YearChartUpdateTask extends RepetitiveUpdateTask<Chart>{

	private Chart chart;
	
	public YearChartUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
	}

	@Override
	public Chart call() {
		Patient patient = Patient.getPatient();
		if(chart == null){
			chart = new Chart();
		}
		
		if(patient != null){
			chart.fetchChartDataYear(patient);
		}
		return chart;
	}
}
