package UI.Frame.User;

import Backend.Models.Patient;
import UI.Frame.RepetitiveTaskHandler;
import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.control.Separator;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderStroke;
import javafx.scene.layout.BorderStrokeStyle;
import javafx.scene.layout.BorderWidths;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;
import javafx.scene.text.Font;

public class UserInfoPane extends GridPane {
	
	private final Label userName;
	private final Label setpsTodayNumber;
	private final Label goalTodayNumber;
	private final Label goalsReachedNumber;
	private final Label totalStepsNumber;
	
	public UserInfoPane(){
		setPadding(new Insets(10, 10, 10, 10));
		setVgap(15);
		setHgap(15);
		
		//Layout
		final Font userNameFont = new Font(30);
		final Font statusFont = new Font(26);
		final Border valueBorer = new Border(new BorderStroke(Color.BLACK, BorderStrokeStyle.SOLID, new CornerRadii(3), new BorderWidths(2)));
		final Insets valuePadding = new Insets(5, 15, 5, 15);
		final int valueLabelSize = 120;
		
		//User Name
		userName = new Label("User Name");
		userName.setFont(userNameFont);
		GridPane.setConstraints(userName, 0, 0, 2, 1);
		getChildren().add(userName);
		GridPane.setHalignment(userName, HPos.CENTER);
		GridPane.setHgrow(userName, Priority.ALWAYS);
		
		//Separator
		final Separator separator = new Separator();
		separator.setBackground(new Background(new BackgroundFill(Color.BLUE, CornerRadii.EMPTY, Insets.EMPTY )));
		GridPane.setConstraints(separator, 0, 1, 2, 1);
		getChildren().add(separator);
		
		//Steps Today
		final Label stepsToday = new Label("Aantal stappen vandaag");
		setHalignment(stepsToday, HPos.CENTER);
		stepsToday.setFont(statusFont);
		setpsTodayNumber = new Label("0");
		setpsTodayNumber.setFont(statusFont);
		setpsTodayNumber.setBorder(valueBorer);
		setpsTodayNumber.setPadding(valuePadding);
		setpsTodayNumber.setPrefWidth(valueLabelSize);
		setpsTodayNumber.setAlignment(Pos.CENTER);
		GridPane.setConstraints(stepsToday, 0, 2);
		getChildren().add(stepsToday);
		GridPane.setConstraints(setpsTodayNumber, 1, 2);
		getChildren().add(setpsTodayNumber);
		
		//Goal Today
		final Label goalToday = new Label("Doelstelling vandaag");
		setHalignment(goalToday, HPos.CENTER);
		goalToday.setFont(statusFont);
		goalTodayNumber = new Label("0");
		goalTodayNumber.setFont(statusFont);
		goalTodayNumber.setBorder(valueBorer);
		goalTodayNumber.setPadding(valuePadding);
		goalTodayNumber.setPrefWidth(valueLabelSize);
		goalTodayNumber.setAlignment(Pos.CENTER);
		GridPane.setConstraints(goalToday, 0, 3);
		getChildren().add(goalToday);
		GridPane.setConstraints(goalTodayNumber, 1, 3);
		getChildren().add(goalTodayNumber);
		
		//Goals Reached
		final Label goalsReached = new Label("Aantal doelstellingen gehaald");
		setHalignment(goalsReached, HPos.CENTER);
		goalsReached.setFont(statusFont);
		goalsReachedNumber = new Label("0");
		goalsReachedNumber.setFont(statusFont);
		goalsReachedNumber.setBorder(valueBorer);
		goalsReachedNumber.setPadding(valuePadding);
		goalsReachedNumber.setPrefWidth(valueLabelSize);
		goalsReachedNumber.setAlignment(Pos.CENTER);
		GridPane.setConstraints(goalsReached, 0, 4);
		getChildren().add(goalsReached);
		GridPane.setConstraints(goalsReachedNumber, 1, 4);
		getChildren().add(goalsReachedNumber);
		
		//Total amount of steps
		final Label totalSteps = new Label("Totaal aantal stappen");
		setHalignment(totalSteps, HPos.CENTER);
		totalSteps.setFont(statusFont);
		totalStepsNumber = new Label("0");
		totalStepsNumber.setFont(statusFont);
		totalStepsNumber.setBorder(valueBorer);
		totalStepsNumber.setPadding(valuePadding);
		totalStepsNumber.setPrefWidth(valueLabelSize);
		totalStepsNumber.setAlignment(Pos.CENTER);
		GridPane.setConstraints(totalSteps, 0, 5);
		getChildren().add(totalSteps);
		GridPane.setConstraints(totalStepsNumber, 1, 5);
		getChildren().add(totalStepsNumber);
		
		//Updating task
		final UserInfoUpdateTask task = new UserInfoUpdateTask();
		task.setOnSucceeded(new RepetitiveTaskHandler<Patient>() {
			
			@Override
			public void handle(Patient result) {
				Patient patient = result;
				userName.setText(patient.getFirstName() + " " + patient.getLastName());
				setpsTodayNumber.setText(""+patient.getStepsToday());
				goalTodayNumber.setText(""+patient.getGoalToday());
				goalsReachedNumber.setText(""+patient.getGoalsCompleted());
				totalStepsNumber.setText(""+patient.getTotalSteps());
				
			}
		}); 
	}
}