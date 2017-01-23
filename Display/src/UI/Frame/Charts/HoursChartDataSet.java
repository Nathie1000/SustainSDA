package UI.Frame.Charts;

public class HoursChartDataSet extends DataSet{

	public String[] getLabels(){
		String[] hours = new String[24];
		for(int i = 0; i < hours.length; i++){
			hours[i] = i+1 + " uur";
		}	
		return hours;
	}
}
