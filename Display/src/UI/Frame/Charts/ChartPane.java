package UI.Frame.Charts;

import Backend.Models.Chart;
import UI.Frame.RepetitiveUpdateTask;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;
import javafx.geometry.Bounds;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.chart.XYChart.Data;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;

public class ChartPane extends GridPane {

	private DataSet dataSet;

	public ChartPane(DataSet dataset, RepetitiveUpdateTask<Chart> rtask, String title, String xlabel){
		this(dataset,rtask,title,xlabel,true);
	}

	public ChartPane(DataSet dataset, RepetitiveUpdateTask<Chart> rtask, String title, String xlabel, boolean goal) {
		dataSet = dataset;

		final RepetitiveUpdateTask<Chart> task = rtask;
		task.setOnSucceeded(new EventHandler<WorkerStateEvent>() {

			@Override
			public void handle(WorkerStateEvent event) {
				System.out.println("handling");
				//TODO: bind chart to data set
				Chart chart = task.getValue();
				System.out.println(chart.getSteps());
				System.out.println(chart.getGoals());
				dataSet.setData(chart.getSteps(),chart.getGoals());
			}
		});

		//Chart
		//Chart Axis
		final CategoryAxis xAxis = new CategoryAxis();
		final NumberAxis yAxis = new NumberAxis();
		yAxis.setMinorTickVisible(false);
		yAxis.setTickLabelsVisible(false);

		//Bar chart
		final BarChart<String, Number> bc = new BarChart<>(xAxis, yAxis);
		bc.setTitle(title);
		xAxis.setLabel(xlabel);
		yAxis.setLabel("Stappen");

		//Steps bar
		XYChart.Series<String, Number> steps = new XYChart.Series<>();
		steps.setName("Stappen");
		steps.getData().addAll(dataSet.getSteps());
		bc.getData().add(steps);
		XYChart.Series<String, Number> goals = null;
		if(goal == true){
			//Goal bar
			goals = new XYChart.Series<>();
			goals.setName("Doel");
			goals.getData().addAll(dataSet.getGoals());
			bc.getData().add(goals);
		}

		//Set a custom display label for each bar.
		for (Data<String, Number> data : steps.getData()) {
			displayLabelForData(data);
		}
		if(goal == true){
			for (Data<String, Number> data : goals.getData()) {
				displayLabelForData(data);
			}
		}
	 	GridPane.setHgrow(bc, Priority.ALWAYS);
	 	GridPane.setVgrow(bc, Priority.ALWAYS);
	 	getChildren().add(bc);
	 }

	private void displayLabelForData(XYChart.Data<String, Number> data) {
		final Node node = data.getNode();
		final Text dataText = new Text(data.getYValue() + "");
		dataText.setFill(Color.WHITE);
		dataText.setFont(Font.font(null, FontWeight.BOLD, 20));

		Group parentGroup = (Group)node.getParent();
		parentGroup.getChildren().add(dataText);

		data.YValueProperty().addListener(new ChangeListener<Number>() {

			@Override
			public void changed(ObservableValue<? extends Number> observable, Number oldValue, Number newValue) {
				dataText.setText(""+newValue);
			}
		});

		node.boundsInParentProperty().addListener(new ChangeListener<Bounds>() {
			@Override
			public void changed(ObservableValue<? extends Bounds> ov, Bounds oldBounds, Bounds bounds) {
				dataText.setLayoutX(Math.round(bounds.getMinX() + bounds.getWidth() / 2 - dataText.prefWidth(-1) / 2));
				dataText.setLayoutY(Math.round(bounds.getMinY() - dataText.prefHeight(-1) * 0.5) + 34);

				double scale = (bounds.getWidth() - 10) / dataText.getLayoutBounds().getWidth();
				if(scale > 1.0) scale = 1.0;
				dataText.setScaleX(scale);
				dataText.setScaleY(scale);
			}
		});
	}
}
