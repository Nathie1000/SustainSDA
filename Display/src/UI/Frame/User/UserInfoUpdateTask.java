package UI.Frame.User;

import java.util.concurrent.TimeUnit;
import Backend.Models.Patient;
import IO.Sim;
import UI.Frame.RepetitiveUpdateTask;


public class UserInfoUpdateTask extends RepetitiveUpdateTask<Patient>{
	
	private Patient patient;
	
	public UserInfoUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
	}

	@Override
	protected Patient call() throws Exception {
		if(patient == null){
			patient = Patient.getPatient(Sim.getCCID());
		}

		patient.getProgress();

		return patient;
	}
} 
