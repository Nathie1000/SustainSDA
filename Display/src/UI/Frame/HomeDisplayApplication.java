package UI.Frame;

import java.beans.EventHandler;

import Backend.API.PatientAPI;
import Backend.Models.Chart;
import Backend.Models.Patient;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.TabPane.TabClosingPolicy;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.Background;
import javafx.scene.layout.BackgroundFill;
import javafx.scene.layout.Border;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.CornerRadii;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.util.Duration;

public class HomeDisplayApplication extends Application {

	private BorderPane root;
	private UserInfoPane userInfoPane; 
	
	public void start(final Stage stage) {
		stage.setWidth(1380);
		stage.setHeight(720);
		stage.setMaximized(true);
		
		root = new BorderPane(userInfoPane);
		root.setBackground(new Background(new BackgroundFill(Color.ALICEBLUE, CornerRadii.EMPTY, Insets.EMPTY )));
		
		userInfoPane = new UserInfoPane();
		userInfoPane.setAlignment(Pos.TOP_CENTER);
		root.setCenter(userInfoPane);
		
		
	
        Group view2 = new Group();
        Rectangle rectangle2 = new Rectangle(300, 250);
        rectangle2.setFill(Color.BLUE);
        view2.getChildren().add(rectangle2);

      
        root.setOnDragDetected(new javafx.event.EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent event) {
				System.out.println("Dragging");
				transist(userInfoPane, view2);
				
			}
		});
       
       // nextView.setOnAction(event -> {
       // 	transist(view1, view2);
      //  });

        Scene scene = new Scene(root);
		// Add Scene
		//Scene scene = new Scene(new Group());
		stage.setScene(scene);
		stage.show();
	}
	
	private void transist(Node form, Node to){
		root.getChildren().add(to);
        double width = root.getWidth();
        KeyFrame start = new KeyFrame(Duration.ZERO,
                new KeyValue(to.translateXProperty(), width),
                new KeyValue(form.translateXProperty(), 0));
        KeyFrame end = new KeyFrame(Duration.seconds(1),
                new KeyValue(to.translateXProperty(), 0),
                new KeyValue(form.translateXProperty(), -width));
        Timeline slide = new Timeline(start, end);
        slide.setOnFinished(e -> root.getChildren().remove(form));
        slide.play();
	}

	public static void main(String[] args) {
		
		try{
		PatientAPI pat = new PatientAPI();
		Patient p = pat.retrieve("1");
		Chart.getChartDataHours();
		System.out.println(p.getName());
		}
		catch(Exception e){
			
		}

		System.out.println("Hallo wo1rld, Classic nathan");
		
		
		launch(args);
	}
}
