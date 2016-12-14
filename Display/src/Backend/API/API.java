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

    String inputLine;

    // while ((inputLine = in.readLine()) != null)
    // {
    //   System.out.println(inputLine);
    // }
    JSONObject obj = new JSONObject(in.readLine());
    in.close();
    return obj;
  }

}
