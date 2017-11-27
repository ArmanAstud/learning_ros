//nodo que envia info

#include "ros/ros.h"
#include "interaccion/inf_personal_usuario.h"
#include <iostream>

using namespace std;


int main(int arcg, char **argv)
{
	ros::init(arcg, argv, "informacion_personal_nodo");
	
	ros::NodeHandle nodo;
	ROS_INFO("informacion_personal_nodo creado y registrado");

	ros::Publisher publicadorMensajes = nodo.advertise<interaccion::inf_personal_usuario>("inf_pers_topic", 0);

	ros::Duration seconds_sleep(1);

	string nombre;
	int edad;
	string idioma;
	string aux;
	bool idioma_completed = false;
	int indx = 0;

	while(ros::ok())
	{
		interaccion::inf_personal_usuario mensajeAEnviar;

		cout<<"Introduce nombre: ";
		cin>>mensajeAEnviar.nombre;
		cout<<"Introduce edad: ";
		cin>>mensajeAEnviar.edad;
		cout<<"Introduce idiomas (escribe 'stop' para terminar)"<<endl;
		int cont = 0;
		do{
			cin>>aux;
			if(aux.compare("stop")!=0)
			{
				if (cont==0)
				{
					idioma = aux;
				}
				else
				{
					idioma = idioma + "," + aux;
				}
			}
			cont++;
			mensajeAEnviar.idiomas = idioma;
		}while(aux.compare("stop")!=0);
		cout<<endl;

		publicadorMensajes.publish(mensajeAEnviar);

		//bucle para mantener petición
		ros::spinOnce();
		//ROS_DEBUG("Se duerme el nodo emisor durante un segundo");
		seconds_sleep.sleep();
	}

}