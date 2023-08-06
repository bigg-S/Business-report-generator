
/*
* This Class allows as to use the same socket for the two other classes
* by setting on intiallization and getting for use in those classes
*/

package com.agent.system;

import java.net.Socket;

public class SocketSingleton
{
	//This can then be accessed from othe activites after initializaton
	private static Socket socket;
	
	public static void setSocket(Socket socketpass)
	{
		SocketSingleton.socket = socketpass;
	}
	
	public static Socket getSocket()
	{
		return SocketSingleton.socket;
	}
}