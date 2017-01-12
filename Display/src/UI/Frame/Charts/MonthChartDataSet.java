package UI.Frame.Charts;

import java.time.DayOfWeek;
import java.time.LocalDate;
import java.time.temporal.TemporalAdjusters;
import java.util.ArrayList;
import java.util.List;

import javafx.scene.chart.XYChart;

public class MonthChartDataSet  extends DataSet{

	
	public String[] getLabels(){
		LocalDate today = LocalDate.now();
        LocalDate firstDayOfTheMonth = today.with(TemporalAdjusters.firstDayOfMonth());
        LocalDate lastDayOfTheMonth = today.with(TemporalAdjusters.lastDayOfMonth());
        LocalDate squareCalendarMonthDayStart = firstDayOfTheMonth
                .with(TemporalAdjusters.previousOrSame(DayOfWeek.SUNDAY));
        LocalDate squareCalendarMonthDayEnd = lastDayOfTheMonth
                .with(TemporalAdjusters.nextOrSame(DayOfWeek.SATURDAY));
        List<LocalDate> totalDates = new ArrayList<>();
        while (!squareCalendarMonthDayStart.isAfter(squareCalendarMonthDayEnd)) {
            totalDates.add(squareCalendarMonthDayStart);
            squareCalendarMonthDayStart = squareCalendarMonthDayStart.plusDays(1);
        }
        System.out.println(totalDates);
        List<String> temp = new ArrayList<String>();
        String[] arr = new String[totalDates.size()];
        for(LocalDate ld : totalDates){
        	temp.add(ld.toString());
        }
        temp.toArray(arr);
		return arr;
	}

	public MonthChartDataSet(){
		super();
	}


}
