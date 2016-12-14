package UI.Frame;

import java.beans.EventHandler;
import java.util.ArrayList;
import java.util.List;

import Backend.API.PatientAPI;
import Backend.Models.Patient;
import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.application.Platform;
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
	private List<Node> nodes;
	private int currentNode;
	
	private UserInfoPane userInfoPane; 
	
	private boolean animationIsPlaying;
	
	public void start(final Stage stage) {
		stage.setWidth(1380);
		stage.setHeight(720);
		stage.setMaximized(true);
		
		root = new BorderPane(userInfoPane);
		root.setBackground(new Background(new BackgroundFill(Color.WHITE, CornerRadii.EMPTY, Insets.EMPTY )));
		
		nodes = new ArrayList<>();
		currentNode = 0;
		animationIsPlaying = false;
		
		userInfoPane = new UserInfoPane();
		userInfoPane.setAlignment(Pos.TOP_CENTER);
		root.setCenter(userInfoPane);
		nodes.add(userInfoPane);
		
        Group view2 = new Group();
        Rectangle rectangle2 = new Rectangle(300, 250);
        rectangle2.setFill(Color.BLUE);
        view2.getChildren().add(rectangle2);
        nodes.add(view2);
      
        root.setOnDragDetected(new javafx.event.EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent event) {
				//System.out.println("Dragging");
				transits(false);
				System.out.println(event.getX());
				
			}
		});
        
        root.setOnMouseClicked(new javafx.event.EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent event) {
				if(event.getClickCount() >= 5){
					Platform.exit();
				}
				//931440301765220288f

			}
		});
        
        //root.setOnDragOver(new javafx.event.EventHandler<MouseEvent>() {

		//	@Override
		//	public void handle(MouseEvent event) {
				//System.out.println("Dragging");
				//transits(false);
		//		System.out.println(event.getX());
				
		//	}
		//});
 
        Scene scene = new Scene(root);
		// Add Scene
		//Scene scene = new Scene(new Group());
		stage.setScene(scene);
		stage.show();
	}
	
	private void transits(boolean left){
		if(animationIsPlaying) return;
		
		final Node from;
		if(currentNode < nodes.size()){
			//Get current node from list.
			from = nodes.get(currentNode);
		}
		else{
			//No current node we can do nothing.
			return;
		}
		
		int nextNode = currentNode + 1;
		Node to = null;
		if(nextNode < nodes.size()){
			//Next node in list.
			to = nodes.get(nextNode);
			currentNode = nextNode;
		}
		else if(nodes.size() > 0){
			//Last node in list reached. Loop to first.
			to = nodes.get(0);
			currentNode = 0;
		}
		else{
			//Empty list we can do nothing.
			return;
		}
		
		root.getChildren().add(to);
        double width = root.getWidth();
        if(!left) width *= -1;
        
        KeyFrame start = new KeyFrame(Duration.ZERO,
                new KeyValue(to.translateXProperty(), width),
                new KeyValue(from.translateXProperty(), 0));
        KeyFrame end = new KeyFrame(Duration.seconds(1),
                new KeyValue(to.translateXProperty(), 0),
                new KeyValue(from.translateXProperty(), -width));
        Timeline slide = new Timeline(start, end);
        slide.setOnFinished(new javafx.event.EventHandler<ActionEvent>() {
			
			@Override
			public void handle(ActionEvent event) {
				root.getChildren().remove(from); 
				animationIsPlaying = false;
				
			}
		});
        animationIsPlaying = true;
        slide.play();
	}

	public static void main(String[] args) {
		
		try{
		//PatientAPI pat = new PatientAPI();
		//Patient p = pat.retrieve("1");
		//System.out.println(p.getName());
		}
		catch(Exception e){
			
		}
		launch(args);
	}
}
