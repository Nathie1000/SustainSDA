package Backend.Models;

import Backend.API.PatientAPI;
import org.json.JSONObject;

public class Patient extends Model {

	private String firstName, lastName;
	private int battery;
	private int id;
	private int stepsToday, goalToday, totalSteps;
	private int goalsCompleted, totalGoals;
	
	
	public static Patient getPatient(String shdNumber) {
		return PatientAPI.retrieve(shdNumber);
	}
	
	
	/**
	 * Default empty Patient constructor
	 */
	public Patient() {
		super();
	}

	/**
	 * Default Patient constructor
	 */
	public Patient(String firstName, String lastName, int battery, int id) {
		super();
		this.firstName = firstName;
		this.lastName = lastName;
		this.battery = battery;
		this.id =id;
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

	public int getId() {
		return id;
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
	

	public void getProgress() {
		// PatientAPI.getProgress(this.id);
		JSONObject json = PatientAPI.retrieveProgress(this.id);
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
