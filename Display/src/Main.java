import Backend.API.PatientAPI;
import Backend.Models.Patient;
public class Main {
	public static void main(String[] args){
		PatientAPI pat = new PatientAPI();
		Patient p = pat.retrieve("1");
		System.out.println(p.getName());


		System.out.println("Hallo wo1rld, Classic nathan");
	}


}
