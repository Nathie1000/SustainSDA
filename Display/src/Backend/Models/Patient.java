package Backend.Models;

import Backend.API.PatientAPI;
import java.util.Map;

public class Patient extends Model {

  private String firstName,lastName;
  private int battery;
  private int id;
  private int stepsToday,goalToday,totalSteps;
  private String goalsAchieved;


	/**
	* Returns value of firstName
	* @return
	*/
	public String getFirstName() {
		return firstName;
	}

	/**
	* Sets new value of firstName
	* @param
	*/
	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}

	/**
	* Returns value of lastName
	* @return
	*/
	public String getLastName() {
		return lastName;
	}

	/**
	* Sets new value of lastName
	* @param
	*/
	public void setLastName(String lastName) {
		this.lastName = lastName;
	}

	/**
	* Returns value of battery
	* @return
	*/
	public int getBattery() {
		return battery;
	}

	/**
	* Sets new value of battery
	* @param
	*/
	public void setBattery(int battery) {
		this.battery = battery;
	}

  public String getName(){
    return this.firstName + " " + this.lastName;
  }

	/**
	* Create string representation of Patient for printing
	* @return
	*/
	@Override
	public String toString() {
		return "Patient [firstName=" + firstName + ", lastName=" + lastName + ", battery=" + battery +"]";
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
    this.setId(id);
	}

  public static Patient getPatient(String shdNumber){
    return PatientAPI.retrieve(shdNumber);
  }

  public Map<String,Object> getProgress(){
    //PatientAPI.getProgress(this.id);
    Map<String,Object> map = PatientAPI.retrieveProgress(this.id);
    this.stepsToday = Integer.parseInt((String)map.get("stepsToday"));
    this.totalSteps = Integer.parseInt((String)map.get("totalSteps"));
    this.goalToday = Integer.parseInt((String)map.get("stepGoal"));
    this.goalsAchieved = (String)map.get("goalsCompleted") + "/" + (String)map.get("totalGoals");
    return map;
  }

	public int getStepsToday() {
		return stepsToday;
	}

	public void setStepsToday(int stepsToday) {
		this.stepsToday = stepsToday;
	}

	public int getGoalToday() {
		return goalToday;
	}

	public void setGoalToday(int goalToday) {
		this.goalToday = goalToday;
	}

	public String getGoalsAchieved() {
		return goalsAchieved;
	}

	public void setGoalsAchieved(String goalsAchieved) {
		this.goalsAchieved = goalsAchieved;
	}

	public int getTotalSteps() {
		return totalSteps;
	}

	public void setTotalSteps(int totalSteps) {
		this.totalSteps = totalSteps;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}
}
