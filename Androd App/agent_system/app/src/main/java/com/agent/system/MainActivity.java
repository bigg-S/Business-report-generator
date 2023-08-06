
/*******************************************************************************
This document has been modified for particular integration with other systems
 *by Wambugu Godfrey Kariuki and Stephen Oketch Okumu 2022
 *******************************************************************************/
 
package com.agent.system;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Toast;

import android.view.View;
import android.view.View.OnTouchListener;

import android.util.Log;

import android.widget.TextView;
import android.widget.EditText;
import android.widget.Button;
import android.widget.ListView;

import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;

import android.content.Intent;
import android.content.IntentFilter;

import java.io.IOException;
import java.io.InputStream;
import java.io.FileOutputStream;
import java.io.OutputStream;

import java.io.BufferedReader;
import java.net.UnknownHostException;
import java.io.InputStreamReader;
import java.io.ByteArrayOutputStream;
import java.io.PrintWriter;
import java.net.Socket;

import java.util.List;
import java.util.ArrayList;


import android.app.Dialog;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnClickListener;

import org.json.JSONException;  
import org.json.JSONObject;
import org.json.JSONArray;

public class MainActivity extends Activity 
{
	
	private static String TAG = "QAgency";
	
	Thread ConnectToServerThread = null;
	
	EditText etIP, etPort;
	TextView tvMessages;
	EditText id;
		
	Button btnlogin;
	Button btnConnect;
	
	String SERVER_IP;
	int SERVER_PORT;
	
	AlertDialog notice;
	
	String ids = "";
	String AgentStatus = "";
	
	String agent_id = "";
	
	ArrayList<String> products;
	ArrayList<ArrayList<String>> products_brands;
	ArrayList<ArrayList<ArrayList<String>>> products_brands_quantities;
	ArrayList<String> seasons;
	ArrayList<String> age_groups;
	ArrayList<String> genders;
	ArrayList<String> locations;
	
	String ProductsDetails = "";
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_main);
		etIP = (EditText)findViewById(R.id.etIP);
		etPort = (EditText)findViewById(R.id.etPort);
		tvMessages = (TextView)findViewById(R.id.tvMessages);
		id = (EditText)findViewById(R.id.myid);
		btnlogin = (Button)findViewById(R.id.btnlogin);
		btnConnect = (Button)findViewById(R.id.btnConnect);
		
		//ininitialize the arraylists by assigning new ArrayLists to them
		initialize_arraylists();		
		
		//when the connect button is clicked
		btnConnect.setOnClickListener(new View.OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{
				if(etIP.getText().toString().length() != 0 && etPort.getText().toString().length() != 0)
				{
					tvMessages.setText("");
					SERVER_IP = etIP.getText().toString().trim();
					SERVER_PORT = Integer.parseInt(etPort.getText().toString().trim());
					
					//Start the connection thread
					ConnectToServerThread = new Thread(new ConnectToServerThread());
					ConnectToServerThread.start();
				}
				else
				{
					notice = my_notice("Please fill in the ip address and the port number to connect to the Server.");
					notice.show();
				}
			}
		});
		
		//when the login button is clicked
		btnlogin.setOnClickListener(new View.OnClickListener() 
		{
			@Override
			public void onClick(View v) 
			{
				//make sure id is not empty
				if(id.getText().toString().length() == 0)
				{
					notice = my_notice("Please fill your ID and make sure you are connected to the Server.");
					notice.show();
				}
				else
				{
					//check if the id exists in the ids string sent by the server ids are in the form _id_
					if(ids.contains("_" + id.getText().toString() + "_"))
					{
						AgentStatus = "Valid";
						agent_id = id.getText().toString();												
						
						//send the valid agent confirmation message to the server
						new Thread(new RespondToServerThread(AgentStatus)).start();
					}
					else
					{
						AgentStatus = "InValid";
						notice = my_notice("Invalid ID.\nMake Sure You Are Connected To The Server.");
						notice.show();
					}
				}
				
				
			}
		});
	}


	//start the RegisterSaleActivity and pass the necessary data to it
	public void login()
	{
		Intent intent = new Intent(MainActivity.this, RegisterSaleActivity.class);
		
		intent.putExtra("agent_id", agent_id);
		
		intent.putExtra("products", products);
		intent.putExtra("products_brands", products_brands);
		intent.putExtra("products_brands_quantities", products_brands_quantities);
		intent.putExtra("seasons", seasons);
		intent.putExtra("age_groups", age_groups);
		intent.putExtra("genders", genders);
		intent.putExtra("locations", locations);
		
		//start the activity
		startActivity(intent);
	}

	//set up the values of the spinners(drop downs) to the values that we received
	public void setUpProducDetails()
	{
		
		try
		{
			//if we had already initialized the arraylists, reset them to new ArrayLists
			if(products.size() != 0)
				initialize_arraylists();
			
			
			//Parse the String of the data that will be in the drop downs , this string is from the server
			
			JSONObject jObject = new JSONObject(ProductsDetails);

			JSONArray jProductsArray = jObject.getJSONArray("Products");
			
			JSONObject jBrandsObject = jObject.getJSONObject("Brands");
			JSONObject jQuantitiesObject = jObject.getJSONObject("Quantities");
			
			JSONArray jBrandsArray;
			JSONArray jQuantitiesArray;
			
			JSONArray jSeasonsArray = jObject.getJSONArray("Seasons");
			JSONArray jAgeGroupsArray = jObject.getJSONArray("Age Groups");
			JSONArray jGendersArray = jObject.getJSONArray("Genders");
			JSONArray jLocationsArray = jObject.getJSONArray("Locations");
			
			for(int product_index = 0; product_index < jProductsArray.length(); product_index++)
			{
								
				products.add(jProductsArray.getString(product_index));
				
				jBrandsArray = jBrandsObject.getJSONArray(jProductsArray.getString(product_index));
				
				ArrayList<String> brands = new ArrayList<String>();
				ArrayList<ArrayList<String>> quantities = new ArrayList<ArrayList<String>>();
				for(int brand_index = 0; brand_index < jBrandsArray.length(); brand_index++)
				{
					
					brands.add(jBrandsArray.getString(brand_index).substring(products.get(product_index).toString().length() + 1));
					
					jQuantitiesArray = jQuantitiesObject.getJSONArray(jBrandsArray.getString(brand_index));
					
					ArrayList<String> b_quantities = new ArrayList<String>();
					for(int quantity_index = 0; quantity_index < jQuantitiesArray.length(); quantity_index++)
					{
						b_quantities.add(jQuantitiesArray.getString(quantity_index));
					}
					
					quantities.add(b_quantities);
				}
				
				products_brands.add(brands);
				products_brands_quantities.add(quantities);
			}
			
			for(int season_index = 0; season_index < jSeasonsArray.length(); season_index++)
			{
				seasons.add(jSeasonsArray.getString(season_index));
			}
			
			for(int age_group_index = 0; age_group_index < jAgeGroupsArray.length(); age_group_index++)
			{
				age_groups.add(jAgeGroupsArray.getString(age_group_index));
			}
			
			for(int gender_index = 0; gender_index < jGendersArray.length(); gender_index++)
			{
				genders.add(jGendersArray.getString(gender_index));
			}
			
			for(int location_index = 0; location_index < jLocationsArray.length(); location_index++)
			{
				locations.add(jLocationsArray.getString(location_index));
			}
			
			
		}
		catch (JSONException e) 
		{
			e.printStackTrace();
		}
		
	}
	
	//set the values of the arraylists to new ArrayLists
	public void initialize_arraylists()
	{
		products = new ArrayList<String>();
		products_brands = new ArrayList<ArrayList<String>>();
		products_brands_quantities = new ArrayList<ArrayList<ArrayList<String>>>();
		seasons = new ArrayList<String>();
		age_groups = new ArrayList<String>();
		genders = new ArrayList<String>();
		locations = new ArrayList<String>();
	}
	
	//for showing a notice to the agent
	public AlertDialog my_notice(String notice) {
        // Use the Builder class for convenient dialog construction
        AlertDialog.Builder builder = new AlertDialog.Builder(MainActivity.this);
        builder.setMessage(notice)
			   .setTitle("Notice")			
               .setPositiveButton("OK", new DialogInterface.OnClickListener() 
			   {
                   public void onClick(DialogInterface dialog, int id) 
				   {
                       //Dialog closes
                   }
               });
        // Create the AlertDialog object and return it
        return builder.create();
    }
	
	//Thread thet handles connection to the server and also receives data sent by the server
	private PrintWriter output;
	class ConnectToServerThread implements Runnable 
	{
		@Override
		public void run() 
		{
			
			try 
			{
				//get the socket
				Socket socket = new Socket(SERVER_IP, SERVER_PORT);	

				//initialize the global sockect that will be used by the RegisterSaleActivity to send the sales data to the server
				SocketSingleton.setSocket(socket);
				//acquire the output stream
				output = new PrintWriter(socket.getOutputStream());
				
				runOnUiThread(new Runnable() 
				{
					@Override
					public void run() 
					{
						tvMessages.setText("Connected\n");
					}
				});
				
				ByteArrayOutputStream byteArrayOutputStream = 	new ByteArrayOutputStream(1024);				
				byte [] buffer = new byte[1024];
				int bytesRead;
				
				//Acquire the input stream
				InputStream inputstream = socket.getInputStream();
				
				//As long as there is no errors when reading the bytes
				while((bytesRead = inputstream.read(buffer)) != -1)
				{
					byteArrayOutputStream.write(buffer, 0, bytesRead);
					
					String received_data = byteArrayOutputStream.toString();
					
					if(bytesRead != 0)
					{
						runOnUiThread(new Runnable() 
						{
							@Override
							public void run() 
							{
								tvMessages.append("server: " + received_data + "\n");
								
								//if the data sent by the server is the ids
								if(received_data.substring(0,3).equals("ids"))
								{
									ids = received_data.substring(4);
								}
								//if the data sent from the server is the items to be used in the drop downs, this data is sent to the client when thei click the login button
								else if(received_data.substring(0,5).equals("items"))
								{
									//get the details string which is in JSON format
									ProductsDetails = received_data.substring(6);
									
									//Parse the string and initialize the arraylists
									setUpProducDetails();
									
									//login
									login();
								}							
							}
						});
						//clear this buffer for reuse				
						byteArrayOutputStream.reset();
					}					
					
				}			
				
			} 
			catch (IOException e) 
			{
				e.printStackTrace();
			}
		}
	}
	
	
	//sends data to the server
	class RespondToServerThread implements Runnable 
	{
		private String message;
		RespondToServerThread(String message) 
		{
			this.message = message;
		}
		@Override
		public void run() 
		{
			output.write(message);
			output.flush();
			runOnUiThread(new Runnable() 
			{
				@Override
				public void run() 
				{
					tvMessages.append("client: " + message + "\n");
				}
			});
		}
	}
	
	
	@Override
    protected void onStart() 
	{
        super.onStart();
		
        Log.i(TAG, "onStart()");
							
    }

    @Override
    protected void onResume() 
	{
        super.onResume();
        Log.i(TAG, "onResume()");
		
    }
    
    @Override
    protected void onPause() 
	{
        super.onPause();
        Log.i(TAG, "onPause()");
        
    }

    @Override
    protected void onStop() 
	{
        super.onStop();
        Log.i(TAG, "onStop()");
        
    }
	
	@Override
	protected void onDestroy() 
	{
		super.onDestroy();
       
	}
}
