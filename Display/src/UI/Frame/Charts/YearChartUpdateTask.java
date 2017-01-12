package UI.Frame.Charts;

import java.util.concurrent.TimeUnit;
import Backend.Models.Chart;
import UI.Frame.RepetitiveUpdateTask;


public class YearChartUpdateTask extends RepetitiveUpdateTask<Chart>{

	public YearChartUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
	}

	@Override
	protected Chart call() throws Exception {
		Chart chart = new Chart();
		chart.getChartDataYear();
		return chart;
	}
}
