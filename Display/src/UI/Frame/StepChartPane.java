package UI.Frame;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
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

public class StepChartPane extends GridPane {

	private WeekChartDataSet dataSet;

	int x = 1;

	public StepChartPane() {
		dataSet = new WeekChartDataSet();

		final CategoryAxis xAxis = new CategoryAxis();
		final NumberAxis yAxis = new NumberAxis();
		yAxis.setMinorTickVisible(false);
		yAxis.setTickLabelsVisible(false);

		final BarChart<String, Number> bc = new BarChart<>(xAxis, yAxis);
		bc.setTitle("Aant Stappen per week");
		xAxis.setLabel("Dag");
		yAxis.setLabel("Stappen");

		XYChart.Series<String, Number> steps = new XYChart.Series<>();
		steps.setName("Stappen");
		steps.getData().addAll(dataSet.getSteps());

		XYChart.Series<String, Number> goals = new XYChart.Series<>();
		goals.setName("Doel");
		goals.getData().addAll(dataSet.getGoals());

		bc.getData().add(steps);
		bc.getData().add(goals);

		for (Data<String, Number> data : steps.getData()) {
			displayLabelForData(data);
		}
		for (Data<String, Number> data : goals.getData()) {
			displayLabelForData(data);
		}

		setOnMousePressed(new EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent event) {
				dataSet.setStep(4, x+= 100);
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
		
		node.parentProperty().addListener(new ChangeListener<Parent>() {
			@Override
			public void changed(ObservableValue<? extends Parent> ov, Parent oldParent, Parent parent) {
				System.out.println("parante changed");
				Group parentGroup = (Group) parent;
				parentGroup.getChildren().add(dataText);
			}
		});
		
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
			}
		});
	}
	
	public WeekChartDataSet getDataSet() {
		return dataSet;
	}
}