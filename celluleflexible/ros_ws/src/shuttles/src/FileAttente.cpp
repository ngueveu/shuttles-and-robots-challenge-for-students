#include "FileAttente.h"
#include <rclcpp/rclcpp.hpp> // ROS 2 Include
#include <unistd.h>

using namespace std;

FileAttente::FileAttente(int id_aiguillage, int successeur_droite, int successeur_gauche, queue<int> spec_queue)
{
	id_aiguillage_=id_aiguillage;
	successeur_droite_=successeur_droite;
	successeur_gauche_=successeur_gauche;
	queue_=spec_queue;
}

FileAttente::~FileAttente()
{
}

int FileAttente::maj(int DD, int DG)
{
	// On crée un logger temporaire
	auto logger = rclcpp::get_logger("file_attente");

	//on est là suite au front descendant du capteur concerné
	if (!queue_.empty())
	{       
		if (id_aiguillage_==0){return successeur_droite_;}
		//appeler service, pour savoir comment est l'aiguillage
		if(DD)//si l'aiguillage est a droite
		{
			return successeur_droite_;
		}
		else if (DG)//si l'aiguillage est a gauche
		{
			return successeur_gauche_;
		}
		else
		{
			RCLCPP_ERROR(logger, "J'ai perdu une navette, a partir de maintenant, les deposes de pièces vont faire n'importe quoi");
			RCLCPP_ERROR(logger, "Vous feriez mieux de relancer la simulation. J'ai perdu une navette car un aiguillage n'était pas en butee droite ou gauche au moment ou une navette c'est engage");
			rclcpp::sleep_for(std::chrono::seconds(3)); // ROS 2 sleep
			return -1;
		}
	}
	else
	{
		return 0;
	}
}

int FileAttente::get_first_navette()
{
	return queue_.front();
}

void FileAttente::add_navette_in_queue(int navette)
{
	queue_.push(navette);
}

void FileAttente::delete_navette_in_queue()
{
	auto logger = rclcpp::get_logger("file_attente");
	if (queue_.empty())
	{
		// Si on est la ça veut dire que le shuttlemanager s'est perdu
		RCLCPP_WARN(logger, "Attention, ça va segfault dans 10 secondes, désolé");
		//rclcpp::sleep_for(std::chrono::seconds(10));
	}
	queue_.pop();
}

int FileAttente::get_id_aiguillage()
{
	return id_aiguillage_;
}

queue<int> FileAttente::get_queue()
{
	return queue_;
}
