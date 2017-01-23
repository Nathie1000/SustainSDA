package UI.Frame.User;

import java.util.concurrent.TimeUnit;
import Backend.Models.Patient;
import UI.Frame.RepetitiveUpdateTask;


public class UserInfoUpdateTask extends RepetitiveUpdateTask<Patient>{
	
	public UserInfoUpdateTask() {
		super(0, 5, TimeUnit.MINUTES);
	}

	@Override
	protected Patient call() throws Exception {
		Patient patient = Patient.getPatient();
		
		if(patient != null){
			patient.fetchProgress();
		}
		else{
			patient = new Patient(null, -1, "Geen", "verbinding", -1);
		}
		
		return patient;
	}
} 
