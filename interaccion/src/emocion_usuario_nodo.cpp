//Nodo que envia emocion

#include "ros/ros.h"
#include "std_msgs/String.h"
#include <iostream>

using namespace std;

//funcion principal
int main(int arcg, char **argv)
{
	//Iniciar nodo
	ros::init(arcg, argv, "emocion_usuario_nodo");
	ros::NodeHandle nodo;
	ROS_INFO("emocion_usuario_nodo creado y registrado");

	//Inicializar Publicador
	ros::Publisher publicadorMensajes = nodo.advertise<std_msgs::String>("emocion_topic", 0);

	//Variable temporal
	ros::Duration seconds_sleep(1);

	//Si ROS está iniciado
	while(ros::ok())
	{
		//variables locales
		std_msgs::String msg;
		string emocion;
		string aux;

		ROS_INFO("Introduce tus emociones (escribe 'stop' para terminar:)");
		int cont = 0;
		//bucle hasta 'stop'
		do{
			cin>>aux;
			if(aux.compare("stop")!=0)
			{
				if (cont==0)
				{
					emocion = aux;
				}
				else
				{
					emocion = emocion + "," + aux;
				}
			}
			cont++;
			msg.data = emocion;
		}while(aux.compare("stop")!=0);

		//envia datos recibidos
		publicadorMensajes.publish(msg);

		//bucle para mantener petición
		ros::spinOnce();
		seconds_sleep.sleep();
	}

}