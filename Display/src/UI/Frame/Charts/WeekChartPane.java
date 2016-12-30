package UI.Frame.Charts;

import org.json.JSONObject;

import Backend.Models.Chart;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.concurrent.WorkerStateEvent;
import javafx.event.EventHandler;
import javafx.geometry.Bounds;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.chart.BarChart;
import javafx.scene.chart.CategoryAxis;
import javafx.scene.chart.NumberAxis;
import javafx.scene.chart.XYChart;
import javafx.scene.chart.XYChart.Data;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;
import javafx.scene.text.FontWeight;
import javafx.scene.text.Text;

public class WeekChartPane extends GridPane {

	private WeekChartDataSet dataSet;

	int x = 1;

	public WeekChartPane() {
		dataSet = new WeekChartDataSet();
		
		final WeekChartUpdateTask task = new WeekChartUpdateTask();
		task.setOnSucceeded(new EventHandler<WorkerStateEvent>() {
			
			@Override
			public void handle(WorkerStateEvent event) {
				//TODO: bind chart to data set
				Chart chart = task.getValue();
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
		bc.setTitle("Aant Stappen per week");
		xAxis.setLabel("Dag");
		yAxis.setLabel("Stappen");

		//Steps bar
		XYChart.Series<String, Number> steps = new XYChart.Series<>();
		steps.setName("Stappen");
		steps.getData().addAll(dataSet.getSteps());
		bc.getData().add(steps);

		//Goal bar
		XYChart.Series<String, Number> goals = new XYChart.Series<>();
		goals.setName("Doel");
		goals.getData().addAll(dataSet.getGoals());
		bc.getData().add(goals);

		//Set a custom display label for each bar.
		for (Data<String, Number> data : steps.getData()) {
			displayLabelForData(data);
		}
		for (Data<String, Number> data : goals.getData()) {
			displayLabelForData(data);
		}

		//TODO: remove this shit
		setOnMousePressed(new EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent event) {
				dataSet.setStep(4, x+= 1);
				dataSet.setGoal(4, x * 2);

				dataSet.setStep(0, x++);
				dataSet.setGoal(0, x * 2);

			}
		});

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