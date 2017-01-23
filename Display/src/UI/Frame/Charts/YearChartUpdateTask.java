package UI.Frame.Charts;

import java.util.concurrent.TimeUnit;
import Backend.Models.Chart;
import Backend.Models.Patient;
import UI.Frame.RepetitiveUpdateTask;


public class YearChartUpdateTask extends RepetitiveUpdateTask<Chart>{

	public YearChartUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
	}

	@Override
	protected Chart call() throws Exception {
		Patient patient = Patient.getPatient();
		Chart chart = new Chart();
		
		if(patient != null){
			chart.fetchChartDataYear(patient);
		}

		return chart;
	}
}
