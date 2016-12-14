package UI.Frame;

import javafx.event.EventHandler;
import javafx.scene.Node;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;

public class StepChartPane extends GridPane{
	
	private WeekChartDataSet dataSet;
	
	 int x = 1;
	
	public StepChartPane() {
		dataSet = new WeekChartDataSet();
		
		final CategoryAxis xAxis = new CategoryAxis();
        final NumberAxis yAxis = new NumberAxis();
        
      
        final BarChart<String,Number> bc = new BarChart<>(xAxis,yAxis);
        bc.setTitle("Aant Stappen per week");
        xAxis.setLabel("Dag");       
        yAxis.setLabel("Stappen");
	 
        XYChart.Series<String, Number> steps = new XYChart.Series<>();
        steps.setName("Stappen");       
        steps.getData().addAll(dataSet.getSteps());
       
        XYChart.Series<String, Number> goals = new XYChart.Series<>();
        goals.setName("Doel");
        goals.getData().addAll(dataSet.getGoals()) ;
        
        bc.getData().addAll(steps, goals);
        
        
        setOnMousePressed(new EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent event) {
				dataSet.setStep(4, x++);
				
			}
		});
        
        GridPane.setHgrow(bc, Priority.ALWAYS);
        GridPane.setVgrow(bc, Priority.ALWAYS);
        getChildren().add(bc);
	}
}