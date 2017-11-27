//Escucha los mensajes

#include "ros/ros.h"
#include "interaccion/inf_personal_usuario.h"
#include "interaccion/pos_usuario.h"
#include "std_msgs/String.h"
#include "interaccion/usuario.h"
#include "interaccion/multiplicador.h"
#include <iostream>

using namespace std;
int edad;
bool servicio = false;
string text2;


string numberToString(int number)
{
	ostringstream convert;
	convert << number;
	return convert.str();
}

void funcionCallback(const interaccion::usuario::ConstPtr& msg)
{

	cout<<"Nombre: "<<msg->infPersonal.nombre<<endl;
	text2 = "Nombre " + msg->infPersonal.nombre + ", ";

	cout<<"Edad: "<<msg->infPersonal.edad<<endl;
	edad = msg->infPersonal.edad;	//asignar edad para enviar al srv  
	text2 = text2 + "edad " + numberToString(edad) +", ";

	cout<<"Idiomas: "<<msg->infPersonal.idiomas<<endl;
	text2 = text2 + "Idiomas " + msg->infPersonal.idiomas + ", ";

	cout<<"Posicion: "<<endl;
	cout<<"\tx: "<<msg->posicion.x<<endl;
	text2 = text2 + "x " + numberToString(msg->posicion.x) +", ";
	cout<<"\ty: "<<msg->posicion.y<<endl;
	text2 = text2 + "y " + numberToString(msg->posicion.y) +", ";
	cout<<"\tz: "<<msg->posicion.z<<endl;
	text2 = text2 + "z " + numberToString(msg->posicion.z) +", ";

	cout<<"Emociones: "<<msg->emocion<<endl;
	text2 = text2 + "Emociones " + msg->emocion + ", ";

	servicio = true;

}


int main(int arcg, char **argv)
{
	ros::init(arcg,argv,"dialogo_nodo");
	ros::NodeHandle dialogo_nodo;

	ROS_INFO("dialogo_nodo creado y registrado");


	ros::Subscriber subscriptor_user = dialogo_nodo.subscribe("user_topic", 0, funcionCallback);
	
	ros::ServiceClient client = dialogo_nodo.serviceClient<interaccion::multiplicador>("nodo_servidor");
	ros::Rate rate(0.75);

	//inicio de bucle
	while(ros::ok())
	{
	//se recibe el mensaje completo
	if(servicio)
	{
		interaccion::multiplicador srv;
		srv.request.entrada = edad;
		if(client.call(srv))
		{	
			ROS_INFO("Respuesta: %d", (int)srv.response.resultado); 
			text2 = text2 + "Respuesta " + numberToString((int)srv.response.resultado);

			//sintesis de voz
			string text = "Este es el texto a sintetizar";
			string command = "espeak -v es \"" + text + "\"";
			system(command.c_str());
			command = "espeak -v es \"" + text2 + "\"";
			system(command.c_str());
		}
		else
		{
			ROS_INFO("Error al llamar al servicio");
		}
		servicio = false;
	}
	ros::spinOnce();
	rate.sleep();
	}

	return 0;

}