package UI.Frame.User;

import java.util.concurrent.TimeUnit;
import Backend.Models.Patient;
import UI.Frame.RepetitiveUpdateTask;


public class UserInfoUpdateTask extends RepetitiveUpdateTask<Patient>{
	
	private Patient patient;
	
	public UserInfoUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
		patient = new Patient();
	}

	@Override
	protected Patient call() throws Exception {
		patient.getProgress();
		return patient;
	}
}
