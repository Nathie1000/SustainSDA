package UI.Frame.User;

import Backend.Models.Patient;
import UI.Frame.RepetitiveUpdateTask;


public class UserInfoUpdateTask extends RepetitiveUpdateTask<Patient>{
	
	public UserInfoUpdateTask() {
		super(DELAY, PERIOD, UNIT);
	}

	@Override
	public Patient call() {
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
