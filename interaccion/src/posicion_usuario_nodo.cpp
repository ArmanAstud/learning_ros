//nodo que envia posicion

#include "ros/ros.h"
#include "interaccion/pos_usuario.h"
#include <iostream>

using namespace std;


int main(int arcg, char **argv)
{
	ros::init(arcg, argv, "posicion_usuario_nodo");
	
	ros::NodeHandle nodo;
	ROS_INFO("posicion_usuario_nodo creado y registrado");

	ros::Publisher publicadorMensajes = nodo.advertise<interaccion::pos_usuario>("pos_usuario_topic", 0);

	ros::Duration seconds_sleep(1);

	int x,y,z;

	while(ros::ok())
	{
		cout<<"Introduce x: ";
		cin>>x;
		cout<<"Introduce y: ";
		cin>>y;
		cout<<"Introduce z: ";
		cin>>z;

		cout<<endl;

		//envia datos recibidos
		interaccion::pos_usuario mensajeAEnviar;

		mensajeAEnviar.x = x;
		mensajeAEnviar.y = y;
		mensajeAEnviar.z = z;

		publicadorMensajes.publish(mensajeAEnviar);

		//bucle para mantener petición
		ros::spinOnce();
		//ROS_DEBUG("Se duerme el nodo emisor durante un segundo");
		seconds_sleep.sleep();
	}

}