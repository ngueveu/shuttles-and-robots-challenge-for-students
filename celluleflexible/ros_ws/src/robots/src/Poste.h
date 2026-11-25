/**** Projet long N7 2020 ****/

#ifndef POSTE
#define POSTE

#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/int32.hpp>
#include "commande_locale/msg/msg_add_product.hpp"

using namespace std;

class Poste
{
	private:
		string nom_poste_;
		int numero_poste_;
		int couleur_;
		int duree_task_;
		double temps_debut_task_;
		bool task_en_cours_;

	public:
		Poste();
		~Poste();

		void init(string nom, int numero);
		string get_nom();
		int get_numero();
		int get_color();
		int get_duree();
		void decalerTemps(double decalage);

		void debutTask(double VREPtime, int duree);
		bool updateTask(double time);
		bool isTaskEnCours();
		void stopTask();
};
#endif
