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
	
	public synchronized static String getCcid(){
		if(ccid == null){
			try {
				File file = new File("/home/pi/ccid.txt");
				BufferedReader br = new BufferedReader(new FileReader(file));
				//Void the first line.
				br.readLine();
				ccid = br.readLine();
				br.close();
				
			} catch (IOException e) {
				//Return stub if failed to read file.
				return "0000000000000000000f";
			}
		}
		return ccid;
	}
}
