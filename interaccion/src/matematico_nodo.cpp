//nodo de servicios

#include "ros/ros.h"
#include "interaccion/multiplicador.h"
#include <iostream>

using namespace std;

bool servicio(interaccion::multiplicador::Request &req, interaccion::multiplicador::Response &res)
{
	res.resultado = req.entrada * 2;
	return true;
}

int main(int arcg, char **argv)
{
	//registra nodo
	ros::init(arcg, argv, "nodo_servidor");
	ros::NodeHandle nodo_servidor;

	//registra servicio
	ros::ServiceServer service = nodo_servidor.advertiseService("nodo_servidor", servicio);
	ROS_INFO("Servicio registrado.");

	ros::spin();


	return 0;
}
