package IO;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

/**
 * @author Nathan Schaaphuizen
 *
 */

/**
 * @class Sim
 * Static class to retrieve SIM800C device information.
 *
 */
public class Sim {
	private static String ccid;
	
	public synchronized static String getCCID(){
		if(ccid == null){
			try {
				File file = new File("/home/pi/ccid.txt");
				FileReader fr = new FileReader(file);
				BufferedReader br = new BufferedReader(fr);
				br.readLine();
				ccid = br.readLine();
				br.close();
				
			} catch (IOException e) {
				return "02358494185083431f";
			}
		}
	
		return ccid;
	}
}
