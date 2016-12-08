package Backend.Models;

public class Patient extends Model {

  private String firstName,lastName;
  private int battery;

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
	public Patient(String firstName, String lastName, int battery) {
		super();
		this.firstName = firstName;
		this.lastName = lastName;
		this.battery = battery;
	}
}
