
/****************************************************************************
This document has been modified for particular integration with other systems
by Wambugu Godfrey Kariuki and Stephen Oketch Okumu 2022
 ***************************************************************************/
 
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


public class RegisterSaleActivity extends Activity implements  
        AdapterView.OnItemSelectedListener
{
	private static String TAG = "QAgency";
	
		
	Socket socket;
	private PrintWriter output;
	
	String agent_id = "";
	
		
	ArrayList<String> products;
	ArrayList<ArrayList<String>> products_brands;
	ArrayList<ArrayList<ArrayList<String>>> products_brands_quantities;
	ArrayList<String> seasons;
	ArrayList<String> age_groups;
	ArrayList<String> genders;
	ArrayList<String> locations;
	
	AlertDialog notice;	
		
	//Product name
	 Spinner name;
	 int selected_product_index = 0;
	 
	 boolean initialized_spinners = false;
	 
	//Customer's Gender
	 Spinner gender;
	//Customer's age group
	 Spinner age_group;
	//Customer's location
	 Spinner location;
	//Season
	 Spinner season;
	
	//Brand
	 Spinner brand;
	 List<String> categories6;
	 ArrayAdapter<String> dataAdapter6;
	 	 
	 //Quantities
	 Spinner quantity;
	 List<String> categories7;
	 ArrayAdapter<String> dataAdapter7;
	 
	//Number of Items sold
	 EditText items_count;
	 	 
	 Button add_record;
	 Button upload;
	 
	//Holds the data of all the records added 
	String products_data = "";
	//Holds the number of records added
	int number_of_products = 1;
	//A list of the products added(will be used to avoid duplication)
	List<String> present_products = new ArrayList<String>();
	
    @Override
    public void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);
  
        setContentView(R.layout.activity_register_sale);
		
		//these two hold the values for the brands and quantities respectively
		//they are global to ensure that if a product or brand is changed, the Spinners are reset accordingly
		categories6 = new ArrayList<String>();
		categories7 = new ArrayList<String>();
        
		/********************************Extact the data that was forwarded from the MainActivity*/
		
		
		
		
		Intent intent = getIntent();
				
		agent_id = intent.getStringExtra("agent_id");
		
		products = (ArrayList<String>)intent.getSerializableExtra("products");
		
		products_brands = (ArrayList<ArrayList<String>>) intent.getSerializableExtra("products_brands");
		products_brands_quantities = (ArrayList<ArrayList<ArrayList<String>>>) intent.getSerializableExtra("products_brands_quantities");
		seasons = (ArrayList<String>) intent.getSerializableExtra("seasons");
		age_groups = (ArrayList<String>) intent.getSerializableExtra("age_groups");
		genders = (ArrayList<String>) intent.getSerializableExtra("genders");
		locations = (ArrayList<String>) intent.getSerializableExtra("locations");
		
						
		//initialize the Spinners(drop downs) if we had not initialized it;
		setupProductDetailsForm();				
		
		//get the already existing Socket initialized in the MainActivity
		socket = SocketSingleton.getSocket();
		
		try
		{
			output = new PrintWriter(socket.getOutputStream());
			
		}
		catch (IOException e) 
		{
			e.printStackTrace();
		}	
		
		
  }	 	
	
	
	
	public AlertDialog my_notice(String notice) {
        // Use the Builder class for convenient dialog construction
        AlertDialog.Builder builder = new AlertDialog.Builder(RegisterSaleActivity.this);
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
	
	public void setupProductDetailsForm()
	{
	/*----------------Product Name--------------*/	
		// Spinner element
        name = (Spinner)findViewById(R.id.name);
		 // Spinner click listener
        name.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
		{
			@Override
			public void onItemSelected(AdapterView<?> parent,View view, int position, long id)
			{
				
				dataAdapter6.clear();
				
				if(position == 0)
				{					
					dataAdapter6.add("Select");					
					dataAdapter6.notifyDataSetChanged();
					brand.setSelection(0);
					quantity.setSelection(0);						
					return;
				}
					
				else
					selected_product_index = position - 1;
				
				
				for(int brand_index = 0; brand_index < products_brands.get(selected_product_index).size(); brand_index++)
				{
					
					if(brand_index == 0)
						dataAdapter6.add("Select");
					Log.i(TAG, "brands: " + products_brands.get(selected_product_index).get(brand_index).toString());
					dataAdapter6.add(products_brands.get(selected_product_index).get(brand_index).toString());
											
					
				}
				dataAdapter6.notifyDataSetChanged();
				brand.setSelection(0);				
				quantity.setSelection(0);
				
			}
			
			@Override  
			public void onNothingSelected(AdapterView<?> arg0) 
			{  
				//use the default function
			}  
			
			
			
		});		
		// Spinner Drop down elements
        List<String> categories1 = new ArrayList<String>();
		
		for(int product_index = 0; product_index < products.size(); product_index++)
		{
			if(product_index == 0)
				categories1.add("Select");
			categories1.add(products.get(product_index).toString());
		}
       		
		// Creating adapter for spinner
        ArrayAdapter<String> dataAdapter1 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, categories1);	
		dataAdapter1.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // attaching data adapter to spinner
        name.setAdapter(dataAdapter1);
				
		
		
		/*----------------Gender--------------*/	
		// Spinner element
        gender = (Spinner)findViewById(R.id.gender);
		 // Spinner click listener
        gender.setOnItemSelectedListener(this);		
		// Spinner Drop down elements
        List<String> categories2 = new ArrayList<String>();
		
		for(int gender_index = 0; gender_index < genders.size(); gender_index++)
		{
			if(gender_index == 0)
				categories2.add("Select");
			
			categories2.add(genders.get(gender_index).toString());
		}
		   

		
		// Creating adapter for spinner
        ArrayAdapter<String> dataAdapter2 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, categories2);	
		dataAdapter2.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // attaching data adapter to spinner
        gender.setAdapter(dataAdapter2);
		
		/*----------------Age group--------------*/	
		// Spinner element
        age_group = (Spinner)findViewById(R.id.age_group);
		 // Spinner click listener
        age_group.setOnItemSelectedListener(this);		
		// Spinner Drop down elements
        List<String> categories3 = new ArrayList<String>();
		
		for(int age_group_index = 0; age_group_index < age_groups.size(); age_group_index++)
		{
			if(age_group_index == 0)
				categories3.add("Select");
			categories3.add(age_groups.get(age_group_index).toString());
		}	
        
        		
		// Creating adapter for spinner
        ArrayAdapter<String> dataAdapter3 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, categories3);	
		dataAdapter3.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // attaching data adapter to spinner
       age_group.setAdapter(dataAdapter3);
		
		/*----------------Region--------------*/	
		// Spinner element
        location = (Spinner)findViewById(R.id.location);
		 // Spinner click listener
        location.setOnItemSelectedListener(this);		
		// Spinner Drop down elements
        List<String> categories4 = new ArrayList<String>();
		
		for(int location_index = 0; location_index < locations.size(); location_index++)
		{
			if(location_index == 0)
				categories4.add("Select");
			categories4.add(locations.get(location_index).toString());
		}	
                
        		
		// Creating adapter for spinner
        ArrayAdapter<String> dataAdapter4 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, categories4);	
		dataAdapter4.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // attaching data adapter to spinner
        location.setAdapter(dataAdapter4);
		
		/*----------------Season--------------*/	
		// Spinner element
        season = (Spinner)findViewById(R.id.season);
		 // Spinner click listener
        season.setOnItemSelectedListener(this);		
		// Spinner Drop down elements
        List<String> categories5 = new ArrayList<String>();
		
		for(int season_index = 0; season_index < seasons.size(); season_index++)
		{
			if(season_index == 0)
				categories5.add("Select");
			categories5.add(seasons.get(season_index).toString());
		}	
		
        		
		// Creating adapter for spinner
        ArrayAdapter<String> dataAdapter5 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, categories5);	
		dataAdapter5.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // attaching data adapter to spinner
        season.setAdapter(dataAdapter5);
		
		/*----------------Brand--------------*/	
		// Spinner element
        brand = (Spinner)findViewById(R.id.brand);
		 // Spinner click listener
        brand.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener()
		{
			@Override
			public void onItemSelected(AdapterView<?> parent,View view, int position, long id)
			{				
				onBrandChanged(position);
			}
			@Override  
			public void onNothingSelected(AdapterView<?> arg0) 
			{  
				//use the default function
			}  
			 
		});		
		// Spinner Drop down elements
        
        
        		
		// Creating adapter for spinner
		dataAdapter6 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, categories6);	
		dataAdapter6.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // attaching data adapter to spinner
        brand.setAdapter(dataAdapter6);
		
		/*----------------Quantity--------------*/	
		// Spinner element
        quantity = (Spinner)findViewById(R.id.quantity);
		 // Spinner click listener
        quantity.setOnItemSelectedListener(this);		
		// Spinner Drop down elements
        categories7 = new ArrayList<String>();
        
        		
		// Creating adapter for spinner
        dataAdapter7 = new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item, categories7);	
		dataAdapter7.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        // attaching data adapter to spinner
        quantity.setAdapter(dataAdapter7);
		
		items_count = (EditText)findViewById(R.id.items_count);
		add_record = (Button)findViewById(R.id.add_record);
		upload = (Button)findViewById(R.id.upload);
		
		items_count.clearFocus();
		
		//When the add item button is clicked
		add_record.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{
				///What to do when the user clicks add_record
				append_record();
				number_of_products += 1;
				
			}
			
		});
		
		
		//When the upload button is clicked
		upload.setOnClickListener(new View.OnClickListener()
		{
			@Override
			public void onClick(View v)
			{	
				
				if(items_count.getText().toString().equals("0") || items_count.getText().toString().equals(""))
				{
					notice = my_notice("Please Fill All Details Correctly");
					notice.show();
				}
				else
				{		
					prepare_and_send_data();
					
					new Thread(new SendThread(products_data)).start();
					
					notice = my_notice("Sale(s) Uploaded Successfully.");
					notice.show();
					
					products_data = "";
				}
				
			}
			
		});
		
				
	}
	
	
	//Adds a new sale in case the agent want to register more than one sale before uploading the data
	public void append_record()
	{
		String nameholder = name.getSelectedItem().toString();
		
		
		//Do not add an item that already exists in the records to be upladed
		if(present_products.indexOf(nameholder) != -1)
			{
				notice = my_notice("There is an item named " + name.getSelectedItem().toString() + " please upload the details first before continuing");
				notice.show();
			}
			//Do not upload if there are unfilled fields
		else if(items_count.getText().equals("0") || items_count.getText().toString().contains("-") || name.getSelectedItem().toString().equals("Select") || name.getSelectedItem().toString().equals("Select") || brand.getSelectedItem().toString().equals("Select")
				|| quantity.getSelectedItem().toString().equals("Select") || season.getSelectedItem().toString().equals("Select") || age_group.getSelectedItem().toString().equals("Select")
				|| gender.getSelectedItem().toString().equals("Select") || location.getSelectedItem().toString().equals("Select") || name.getSelectedItem().toString().equals("Select"))
		{			
			notice = my_notice("Please Fill the Details Correctly");
			notice.show();
		}
		else		
		{
			products_data += agent_id;
			products_data += " " + handle_spaces(name.getSelectedItem().toString());
			products_data += " " + handle_spaces(brand.getSelectedItem().toString());
			products_data += " " + handle_spaces(quantity.getSelectedItem().toString());
			products_data += " " + handle_spaces(season.getSelectedItem().toString());
			products_data += " " + handle_spaces(age_group.getSelectedItem().toString());			
			products_data += " " + handle_spaces(gender.getSelectedItem().toString());
			products_data += " " + handle_spaces(location.getSelectedItem().toString());
			products_data += " " + items_count.getText() + "\n";			
			
			present_products.add(name.getSelectedItem().toString());
			
			reset_form();
		}
		
		
	}
	
	
	//Replace the spaces with underscores in names that as two parts eg. "Cooking Oil".
	//This makes it easier to parse the sales
	public String handle_spaces(String word)
	{
		String mystring = word;
		
		mystring = mystring.replace(" ","_");
				
		return mystring;
	}
	
	//In case a user wants to add a new record, reset the form
	public void reset_form()
	{
		name.setSelection(0);
		items_count.setText("0");
		gender.setSelection(0);
		age_group.setSelection(0);
		brand.setSelection(0);
		quantity.setSelection(0);
		location.setSelection(0);
		season.setSelection(0);
	}
	
	
	//In case the agent wants to upload a single sale
	public void prepare_and_send_data()
	{	
		if(number_of_products == 1)
		{
			if(items_count.getText().equals("0") || items_count.getText().toString().contains("-") || name.getSelectedItem().toString().equals("Select") || name.getSelectedItem().toString().equals("Select") || brand.getSelectedItem().toString().equals("Select")
				|| quantity.getSelectedItem().toString().equals("Select") || season.getSelectedItem().toString().equals("Select") || age_group.getSelectedItem().toString().equals("Select")
				|| gender.getSelectedItem().toString().equals("Select") || location.getSelectedItem().toString().equals("Select") || name.getSelectedItem().toString().equals("Select"))
			{
				
				notice = my_notice("Please Fill the Details Correctly");
				notice.show();	
				
			}
			else
			{
				products_data += agent_id;
				products_data += " " + handle_spaces(name.getSelectedItem().toString());
				products_data += " " + handle_spaces(brand.getSelectedItem().toString());
				products_data += " " + handle_spaces(quantity.getSelectedItem().toString());
				products_data += " " + handle_spaces(season.getSelectedItem().toString());
				products_data += " " + handle_spaces(age_group.getSelectedItem().toString());
				products_data += " " + handle_spaces(gender.getSelectedItem().toString());
				products_data += " " + handle_spaces(location.getSelectedItem().toString());
				products_data += " " + items_count.getText() + "\n";	
			}	
				
		}	
				
	}

	
	//This thread sends the sale data to the server
	class SendThread implements Runnable 
	{
		private String sale_data;
		SendThread(String sale_data) 
		{
			this.sale_data = sale_data;
		}
		@Override
		public void run() 
		{
			output.write(sale_data);
			output.flush();
		}
	}
	
	public void onBrandChanged(int brand_index)
	{
		dataAdapter7.clear();
		if(brand_index == 0)
		{
			dataAdapter7.add("Select");
			dataAdapter7.notifyDataSetChanged();
			
			quantity.setSelection(0);
			return;
		}
		else
			brand_index = brand_index - 1;
		
		
		for(int quantity_index = 0; quantity_index < products_brands_quantities.get(selected_product_index).get(brand_index).size(); quantity_index++)
		{
			if(quantity_index == 0)
				dataAdapter7.add("Select");
			dataAdapter7.add(products_brands_quantities.get(selected_product_index).get(brand_index).get(quantity_index).toString());
			Log.i(TAG, "QAgncy quantity: " + products_brands_quantities.get(selected_product_index).get(brand_index).get(quantity_index).toString());
				
		}
		dataAdapter7.notifyDataSetChanged();
		quantity.setSelection(0);
		
		
	}
	
	 //Performing action onItemSelected and onNothing selected  on the dropdowns(Spinners)
    @Override  
    public void onItemSelected(AdapterView<?> arg0, View arg1, int position, long id) 
	{  
          //use the default function
    }  
    @Override  
    public void onNothingSelected(AdapterView<?> arg0) 
	{  
          //use the default function
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

