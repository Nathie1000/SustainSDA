package UI.Frame;

import javafx.concurrent.Task;


public class UpdateTask extends Task<Void>{ //<--Void has to set to type of model.
	
	public UpdateTask(){
		//Maybe do some initialization, dunno depends on model.
	}

	@Override
	protected Void call() throws Exception {
		//Execute connection and fetch model here.
		
		return null; //<--- Return the type of model to laster us in UI.
		
		
	}

}
