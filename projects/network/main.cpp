/**
 * This file is part of RT2D, a 2D OpenGL framework.
 *
 * - Copyright 2015 Rik Teerling <rik@onandoffables.com>
 */
#include <rt2d/core.h>
#include "scene01.h"

#include "NetworkClient.h"
#include "NetworkServer.h"


//typedef int SOCKET;

int main(void)
{

	/* 
	NetworkClient* client = new NetworkClient();

	//if (client->Connect("216.58.211.99", 80)) {
	if (client->Connect("google.nl", 80)) {

		client->Send("GET / HTTP/1.1\r\nHost: google.nl\r\n\r\n");
		
		std::string Message;
		do {
			Message = client->Receive();
		} while (Message.size() == 0);

		client->Disconnect();
	} */



	NetworkServer* server = new NetworkServer();
	server->Start(1337);
	server->ListenASync();

	//while (true) {
		//server->Listen();
		//server->HandleClients();
	//}


	


	int x;
	int *ptr_p;

	x = 5;
	ptr_p = &x;

	std::cout << "Address of: " << &x << std::endl;
	std::cout << "Value of: " << x << std::endl;
	
	std::cout << std::endl;

	std::cout << "Address of: " << &ptr_p << std::endl;
	std::cout << "Value of: " << ptr_p << std::endl;

	std::cout << "Value pointed by the pointer: " << *ptr_p << std::endl;

	std::cout << std::endl << std::endl << std::endl;


	// Core instance
	Core core;
	//core = new Core();
	//extern Core core;
	//extern const Core core;

	// scene handle
	Scene* scene = NULL;

	scene = new Scene01();		// create Scene on the heap
	while(scene->isRunning()) {	// check status of Scene every frame
		core.run(scene);		// update and render the current scene
		core.showFrameRate(5);	// show framerate in output every 5 seconds

		server->HandleClients();
	}
	//core.cleanup();				// cleanup ResourceManager (Textures + Meshes, but not Shaders)
	delete scene;				// delete Scene from heap to make space for next one


	

	return 0;
}
