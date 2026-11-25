/**** Bruno DATO, Abdellah ELGOURAIN, Evgeny SHULGA M1 EEA ISTR Université Paul Sabatier Toulouse III 2016 ****/
#ifndef ACT
#define ACT

#include <commande_locale/msg/msg_stop_control.hpp>
#include "commande_locale/msg/msg_switch_control.hpp"
#include "commande_locale/msg/msg_pin_control.hpp"
#include "commande_locale/msg/actionneurs.hpp"
#include <rclcpp/rclcpp.hpp>


class Actionneurs
{
private:

    rclcpp::Publisher<commande_locale::msg::Actionneurs>::SharedPtr pub_actionneurs_ligne;
    rclcpp::Publisher<commande_locale::msg::MsgSwitchControl>::SharedPtr pub_actionneurs_simu_aguillages;
    rclcpp::Publisher<commande_locale::msg::MsgStopControl>::SharedPtr pub_actionneurs_simu_stops;
    rclcpp::Publisher<commande_locale::msg::MsgPinControl>::SharedPtr pub_actionneurs_simu_pins;

public:
	
	long int Actionneurs_ligne;
	commande_locale::msg::MsgStopControl actionneurs_simulation_Stop;
	commande_locale::msg::MsgSwitchControl actionneurs_simulation_Aguillages;
	commande_locale::msg::MsgPinControl actionneurs_simulation_Pin;

	Actionneurs(rclcpp::Node::SharedPtr node);
	~Actionneurs();

	void Envoyer(bool STx[],bool RxD[],bool RxG[],bool Vx[],bool Dx[],bool PIx[]);

	void publish_actionneurs_ligne();
	void publish_actionneurs_simulation();

	void Ecrire_ligne_STx(bool STx[]);
	void Ecrire_ligne_RxD(bool RxD[]);
	void Ecrire_ligne_RxG(bool RxG[]);
	void Ecrire_ligne_PIx(bool PIx[]);
	void Ecrire_ligne_Vx(bool Vx[]);
	void Ecrire_ligne_Dx(bool Dx[]);

};

// Fonction globale
void WRITE(long int *registre,bool donnee,int numero_bit);

#endif
