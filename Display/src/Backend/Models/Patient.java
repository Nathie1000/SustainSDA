package Backend.Models;


import org.json.JSONObject;

import Backend.API.API;

public class Patient {

	private int id;
	private String ccid;
	private String firstName, lastName;
	private int battery;
	private int stepsToday, goalToday, totalSteps;
	private int goalsCompleted, totalGoals;
	
	
	public static Patient patient;
	public static synchronized Patient getPatient(){
		if(patient == null){
			//retrivePatient() may return null.
			patient = API.retrievePatient();
		}
		return patient;
	}
	
	/**
	 * Default Patient constructor
	 */
	public Patient(String ccid, int id, String firstName, String lastName, int battery) {
		this.id = id;
		this.ccid = ccid;
		this.firstName = firstName;
		this.lastName = lastName;
		this.battery = battery;
	}
	
	public int getId() {
		return id;
	}

	/**
	 * Returns value of firstName
	 * 
	 * @return
	 */
	public String getFirstName() {
		return firstName;
	}

	/**
	 * Returns value of lastName
	 * 
	 * @return
	 */
	public String getLastName() {
		return lastName;
	}

	/**
	 * Returns value of battery
	 * 
	 * @return
	 */
	public int getBattery() {
		return battery;
	}

	public String getCcid() {
		return ccid;
	}
	
	public int getStepsToday() {
		return stepsToday;
	}

	public int getGoalToday() {
		return goalToday;
	}

	public int getGoalsCompleted() {
		return goalsCompleted;
	}
	
	public int getTotalGoals() {
		return totalGoals;
	}
	
	public int getTotalSteps() {
		return totalSteps;
	}

	
	/**
	 * Create string representation of Patient for printing
	 * 
	 * @return
	 */
	@Override
	public String toString() {
		return "Patient [firstName=" + firstName + ", lastName=" + lastName + ", battery=" + battery + "]";
	}
	
	public void fetchProgress() {
		JSONObject json = API.retrievePatientProgress(this);
		this.stepsToday = json.getInt("steps");
		this.totalSteps = json.getInt("totalSteps");
		if (json.get("stepGoal") instanceof String) {
			this.goalToday = 0;
		} else {
			this.goalToday = json.getInt("stepGoal");
		}
		this.goalsCompleted= json.getInt("goalsCompleted"); 
		this.totalGoals = json.getInt("totalGoals");
	}
}
