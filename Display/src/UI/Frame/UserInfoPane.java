package UI.Frame;

import com.sun.org.apache.xml.internal.serializer.ToHTMLSAXHandler;

import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.control.Label;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderStroke;
import javafx.scene.layout.BorderStrokeStyle;
import javafx.scene.layout.BorderWidths;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.GridPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.text.Font;
import javafx.scene.text.TextAlignment;

public class UserInfoPane extends GridPane {
	
	private Label userName;
	private Label setpsTodayNumber;
	private Label goalTodayNumber;
	private Label goalsReachedNumber;
	private Label totalStepsNumber;
	
	public UserInfoPane(){
		setPadding(new Insets(10, 10, 10, 10));
		setVgap(15);
		setHgap(5);
		setBackground(new Background(new BackgroundFill(Color.RED, CornerRadii.EMPTY, Insets.EMPTY )));
		
		Font userNameFont = new Font(30);
		Font statusFont = new Font(26);
		Border valueBorer = new Border(new BorderStroke(Color.BLACK, BorderStrokeStyle.SOLID, new CornerRadii(3), new BorderWidths(2)));
		Insets valuePadding = new Insets(5, 20, 5, 20);
		int valueLabelSize = 100;
				
		userName = new Label("User Name");
		userName.setFont(userNameFont);
		GridPane.setConstraints(userName, 0, 0, 2, 1);
		getChildren().add(userName);
		GridPane.setHalignment(userName, HPos.CENTER);
		
		Line line = new Line();
		line.setStroke(Color.BLUE);
		GridPane.setConstraints(line, 0, 1, 2, 1);
		//line.endXProperty().bind(widthProperty());
		getChildren().add(line);
		
		Label stepsToday = new Label("xxxxxxxxx");
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
		
		Label goalToday = new Label("Goal:");
		goalToday.setFont(statusFont);
		goalTodayNumber = new Label("00");
		goalTodayNumber.setFont(statusFont);
		goalTodayNumber.setBorder(valueBorer);
		goalTodayNumber.setPadding(valuePadding);
		goalTodayNumber.setPrefWidth(valueLabelSize);
		goalTodayNumber.setAlignment(Pos.CENTER);
		GridPane.setConstraints(goalToday, 0, 3);
		getChildren().add(goalToday);
		GridPane.setConstraints(goalTodayNumber, 1, 3);
		getChildren().add(goalTodayNumber);
		
		Label goalsReached = new Label("Totaal:");
		goalsReached.setFont(statusFont);
		goalsReachedNumber = new Label("999");
		goalsReachedNumber.setFont(statusFont);
		goalsReachedNumber.setFont(statusFont);
		goalsReachedNumber.setBorder(valueBorer);
		goalsReachedNumber.setPadding(valuePadding);
		goalsReachedNumber.setPrefWidth(valueLabelSize);
		goalsReachedNumber.setAlignment(Pos.CENTER);
		GridPane.setConstraints(goalsReached, 0, 4);
		getChildren().add(goalsReached);
		GridPane.setConstraints(goalsReachedNumber, 1, 4);
		getChildren().add(goalsReachedNumber);
		
		Label totalSteps = new Label("Totaal aantal stapeen");
		totalSteps.setFont(statusFont);
		totalStepsNumber = new Label("0");
		totalStepsNumber.setFont(statusFont);
		GridPane.setConstraints(totalSteps, 0, 5);
		getChildren().add(totalSteps);
		GridPane.setConstraints(totalStepsNumber, 1, 5);
		getChildren().add(totalStepsNumber);
		
		
		
		
	}
	
	
}
