package UI.Frame.Charts;

import Backend.Models.Chart;
import Backend.Models.Patient;
import UI.Frame.RepetitiveUpdateTask;


public class HoursChartUpdateTask extends RepetitiveUpdateTask<Chart>{

	private Chart chart;
	
	public HoursChartUpdateTask() {
		super(DELAY, PERIOD, UNIT);
	}

	@Override
	public Chart call() {
		Patient patient = Patient.getPatient();
		if(chart == null){
			chart = new Chart();
		}
		if(patient != null){
			chart.fetchChartDataHours(patient);
		}
		return chart;
	}
}
