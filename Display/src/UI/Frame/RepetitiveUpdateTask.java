package UI.Frame;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.concurrent.Task;

public abstract class RepetitiveUpdateTask<T> extends Task<T> {
	
	public static final ScheduledExecutorService scheduler = Executors.newSingleThreadScheduledExecutor();
	
	public RepetitiveUpdateTask(int delay, int period, TimeUnit unit) {
		scheduler.scheduleAtFixedRate(this, delay, period, unit);
		exceptionProperty().addListener(new ChangeListener<Throwable>() {

			@Override
			public void changed(ObservableValue<? extends Throwable> observable, Throwable oldValue, Throwable newValue) {
				newValue.printStackTrace();
			}
		});
	}
}
