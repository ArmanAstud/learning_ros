//Escucha los mensajes

#include "ros/ros.h"
#include "interaccion/inf_personal_usuario.h"
#include "interaccion/pos_usuario.h"
#include "std_msgs/String.h"
#include "std_msgs/Bool.h"
#include "interaccion/usuario.h"
#include "interaccion/multiplicador.h"
#include <iostream>

using namespace std;

//variables globales
int edad;
bool servicio = false;
bool start = true;
string text2;
ros::Publisher publicadorStart;
ros::Publisher publicadorReset;

//convierte int a string
string numberToString(int number)
{
	ostringstream convert;
	convert << number;
	return convert.str();
}

//funcion manejadora del topic 'user_topic'
void funcionCallback(const interaccion::usuario::ConstPtr& msg)
{
	//SUSTITUIR POR COUT<<msg
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

	//puedes llamar al servicio
	servicio = true;

}

//funcion manejadora del reloj
void aliveCallback(const std_msgs::Bool::ConstPtr& msg)
{
	ROS_INFO("ALIVE!!");
}

//funcion para publicar 'start_topic' o 'reset_topic'
void callClock()
{
	//variable local
	std_msgs::String msg;
	//si es la primera vez que llama al reloj
	if(start)
	{
		msg.data = "Start!";
		publicadorStart.publish(msg);
		start = false;
	}
	else
	{
		msg.data = "Reset!";
		publicadorReset.publish(msg);
	}
}

//funcion principal
int main(int arcg, char **argv)
{
	//iniciar nodo
	ros::init(arcg,argv,"dialogo_nodo");
	ros::NodeHandle dialogo_nodo;
	ROS_INFO("dialogo_nodo creado y registrado");

	//Inicializar Subscriptores y Publicadores
	ros::Subscriber subscriptor_user = dialogo_nodo.subscribe("user_topic", 0, funcionCallback);
	ros::Subscriber subscriptor_alive = dialogo_nodo.subscribe("still_alive", 0, aliveCallback);
	ros::ServiceClient client = dialogo_nodo.serviceClient<interaccion::multiplicador>("nodo_servidor");
	publicadorStart = dialogo_nodo.advertise<std_msgs::String>("start_topic", 0);
	publicadorReset = dialogo_nodo.advertise<std_msgs::String>("reset_topic", 0);

	//ratio para el servicio
	ros::Rate rate(0.75);

	//inicio de bucle
	while(ros::ok())
	{
	//si se recibe el mensaje completo
	if(servicio)
	{
		//llamada al servicio
		interaccion::multiplicador srv;
		srv.request.entrada = edad;
		if(client.call(srv))
		{	
			ROS_INFO("Respuesta: %d", (int)srv.response.resultado); 
			text2 = text2 + "Respuesta " + numberToString((int)srv.response.resultado);

			//llamada al reloj
			callClock();

			//sintesis de voz
			string text = "Este es el texto a sintetizar";
			string command = "espeak -v es \"" + text + "\"";
			system(command.c_str());
			command = "espeak -v es \"" + text2 + "\"";
			system(command.c_str());
		}
		//si no se establece conexion con el servidor
		else
		{
			ROS_INFO("Error al llamar al servicio");
		}
		//reinicia variable de llamada al servidor
		servicio = false;

	}
	ros::spinOnce();
	rate.sleep();
	}
	
	return 0;
}