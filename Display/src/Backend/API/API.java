package Backend.API;

import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;

import javax.net.ssl.HttpsURLConnection;
import org.json.*;



public abstract class API {

  public static JSONObject apiCall(String url) throws Exception
  {
    String httpsURL = url;
    URL myurl = new URL(httpsURL);
    HttpURLConnection con = (HttpURLConnection)myurl.openConnection();
    InputStream ins = con.getInputStream();
    InputStreamReader isr = new InputStreamReader(ins);
    BufferedReader in = new BufferedReader(isr);

    JSONObject obj = new JSONObject(in.readLine());
    in.close();
    return obj;
  }



  protected static JSONObject retrieveByUrl(String url){
		try{
			JSONObject json = apiCall(url);//+shdNumber);
			return json;
		} catch(Exception e){
			System.out.println("Exception: ");
			System.out.println(e);
			e.printStackTrace();
		}
		return null;
	}

}
