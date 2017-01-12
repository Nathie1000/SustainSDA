package UI.Frame.Charts;

import java.util.concurrent.TimeUnit;
import Backend.Models.Chart;
import UI.Frame.RepetitiveUpdateTask;


public class DayChartUpdateTask extends RepetitiveUpdateTask<Chart>{

	public DayChartUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
	}

	@Override
	protected Chart call() throws Exception {
		System.out.println("calling call");
		Chart chart = new Chart();
		chart.getChartDataHours();
		return chart;
	}
}
