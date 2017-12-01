//Empaqueta los mensajes

#include "ros/ros.h"
#include "interaccion/inf_personal_usuario.h"
#include "interaccion/pos_usuario.h"
#include "std_msgs/String.h"
#include "interaccion/usuario.h"
#include <iostream>

using namespace std;

//variables globales
bool _info = false;
string nombre;
int edad;
string idioma;

bool _pos = false;
int position[3] = {0};

bool _emo = false;
string emocion;

ros::Publisher pub_usuario;

//funcion publicar toda la informacion
void mostrar_todo(bool info,bool pos, bool emo)
{
	if(info == true && pos==true && emo==true)
	{
		cout<<"datos recibidos"<<endl;
		interaccion::usuario mensajeAEnviar;
		mensajeAEnviar.emocion = emocion;
		mensajeAEnviar.posicion.x = position[0];
		mensajeAEnviar.posicion.y = position[1];
		mensajeAEnviar.posicion.z = position[2];
		mensajeAEnviar.infPersonal.nombre = nombre;
		mensajeAEnviar.infPersonal.edad = edad;
		mensajeAEnviar.infPersonal.idiomas = idioma;

		pub_usuario.publish(mensajeAEnviar);
		_pos = false;
		_info = false;
		_emo = false;

	}
}

//funcion que guarda el topic 'inf_pers_topic'
void funcionCallback_info(const interaccion::inf_personal_usuario::ConstPtr& msg)
{

	nombre = msg->nombre;
	edad = msg->edad;
	idioma = msg->idiomas;
	_info = true;
	mostrar_todo(_info, _pos, _emo);
}

//funcion que guarda el topic 'pos_usuario'	
void funcionCallback_pos(const interaccion::pos_usuario::ConstPtr& msg)
{

	position[0] = msg->x;
	position[1] = msg->y;
	position[2] = msg->z;
	_pos = true;
	mostrar_todo(_info, _pos, _emo);
}

//funcion que guarda el topic 'emocion_topic'
void funcionCallback_emocion(const std_msgs::String::ConstPtr& msg)
{

	emocion = msg->data.c_str();
	_emo = true;
	mostrar_todo(_info, _pos, _emo);
}


//funcion principal
int main(int arcg, char **argv)
{
	//iniciar ROS
	ros::init(arcg,argv,"empaquetador_nodo");
	ros::NodeHandle empaquetador_nodo;
	ROS_INFO("empaquetador_nodo creado y registrado");

	//inicializar Subscriptores y publicadores
	ros::Subscriber subscriptor_info = empaquetador_nodo.subscribe("inf_pers_topic", 0, funcionCallback_info);
	ros::Subscriber subscriptor_pos = empaquetador_nodo.subscribe("pos_usuario_topic", 0, funcionCallback_pos);
	ros::Subscriber subscriptor_emo = empaquetador_nodo.subscribe("emocion_topic", 0, funcionCallback_emocion);
	pub_usuario = empaquetador_nodo.advertise<interaccion::usuario>("user_topic", 0);

	ros::spin();

	return 0;

}