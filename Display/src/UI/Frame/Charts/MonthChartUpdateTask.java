package UI.Frame.Charts;

import java.util.concurrent.TimeUnit;
import Backend.Models.Chart;
import UI.Frame.RepetitiveUpdateTask;


public class MonthChartUpdateTask extends RepetitiveUpdateTask<Chart>{

	public MonthChartUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
	}

	@Override
	protected Chart call() throws Exception {
		Chart chart = new Chart();
		chart.getChartDataMonth();
		return chart;
	}
}
