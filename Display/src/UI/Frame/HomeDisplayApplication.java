package UI.Frame;


import java.util.ArrayList;
import java.util.List;

import UI.Frame.Charts.WeekChartPane;
import UI.Frame.User.UserInfoPane;

import javafx.animation.KeyFrame;
import javafx.animation.KeyValue;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.StackPane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import javafx.stage.WindowEvent;
import javafx.util.Duration;

public class HomeDisplayApplication extends Application {

	private StackPane root;
	private List<Node> nodes;
	private int currentNode;

	private boolean animationIsPlaying;
	private double startDragX;

	public void start(final Stage stage) {
		//Raspberry pi screen dimensions
		stage.setWidth(800);
		stage.setHeight(480);
		stage.setMaximized(true);
		
		//Root
		root = new StackPane();
		nodes = new ArrayList<>();
		currentNode = 0;
		animationIsPlaying = false;

		//Step Chart
		WeekChartPane stepPane = new WeekChartPane();
		nodes.add(stepPane);

		//User info
		UserInfoPane userInfoPane = new UserInfoPane();
		userInfoPane.setAlignment(Pos.TOP_CENTER);
		nodes.add(userInfoPane);

		//Blue square
        Group view2 = new Group();
        Rectangle rectangle2 = new Rectangle(300, 250);
        rectangle2.setFill(Color.BLUE);
        view2.getChildren().add(rectangle2);
        nodes.add(view2);

        //Display first node in list
        root.getChildren().add(nodes.get(0));
       
        //Start drag location
        root.setOnMousePressed(new javafx.event.EventHandler<MouseEvent>() {
			@Override
			public void handle(MouseEvent event) {
				startDragX = event.getX();
			}
		});
        
        //Drag motion
        root.setOnMouseDragged(new javafx.event.EventHandler<MouseEvent>() {
			@Override
			public void handle(MouseEvent event) {
				double dX = event.getX() - startDragX;
				if(dX >= 200){
					transits(false);
					startDragX = event.getX();
				}
				else if (dX <= -200){
					transits(true);
					startDragX = event.getX();
				}
			}
         });
        //Exit on 5 mouse clicks
        root.setOnMouseClicked(new javafx.event.EventHandler<MouseEvent>() {

			@Override
			public void handle(MouseEvent event) {
				if(event.getClickCount() >= 5){
					Platform.exit();
				}
			}
		});
        
        //Stop task scheduling on application window close
        stage.setOnCloseRequest(new javafx.event.EventHandler<WindowEvent>() {
			
			@Override
			public void handle(WindowEvent event) {
				RepetitiveUpdateTask.scheduler.shutdownNow();
			}
		});

        //Create scene and layout
        Scene scene = new Scene(root);
        scene.getStylesheets().add("color.css");
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

		int nextNode = currentNode + (left ? 1 : -1);
		Node to = null;
		if(nextNode < nodes.size() && nextNode >= 0){
			//Next node in list.
			to = nodes.get(nextNode);
			currentNode = nextNode;
		}
		else if(nextNode >= nodes.size() && nodes.size() > 0){
			//Last node in list reached. Loop to first.
			to = nodes.get(0);
			currentNode = 0;
		}
		else if(nextNode < 0 && nodes.size() > 0){
			//First node in list reached. Loop to last.
			to = nodes.get(nodes.size()-1);
			currentNode = nodes.size() - 1;
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
		launch(args);
	}
}
