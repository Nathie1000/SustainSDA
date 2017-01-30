package UI.Frame;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import javafx.application.Platform;

public abstract class RepetitiveUpdateTask<T> implements Runnable {
	public static final int DELAY = 0;
	public static final int PERIOD = 10;
	public static final TimeUnit UNIT = TimeUnit.SECONDS;
	
	public static final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();
	
	private T value;
	private RepetitiveTaskHandler<T> eventHandler;
	
	public RepetitiveUpdateTask(int delay, int period, TimeUnit unit) {
		scheduler.scheduleAtFixedRate(this, delay, period, unit);
	}
	
	@Override
	public void run() {
		value = call();
		if(eventHandler != null){
			Platform.runLater(new Runnable() {
				
				@Override
				public void run() {
					eventHandler.handle(value);
					
				}
			});
		}
	}
	
	public abstract T call();
	
	public void setOnSucceeded( RepetitiveTaskHandler<T> eventHandler){
		this.eventHandler = eventHandler;
	}
}
