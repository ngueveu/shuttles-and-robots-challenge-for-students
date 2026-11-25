/**** Bruno DATO, Abdellah ELGOURAIN, Evgeny SHULGA M1 EEA ISTR Université Paul Sabatier Toulouse III 2016 ****/
#ifndef CAPT
#define CAPT

// Include ROS 2 généré (snake_case)
#include "aiguillages/msg/msg_sensor_state.hpp"
#include <rclcpp/rclcpp.hpp>

class Capteurs
{
	private:
		// Subscriber ROS 2
		rclcpp::Subscription<aiguillages::msg::MsgSensorState>::SharedPtr sub_capteurs_simu;
		// On garde pas le subscriber ligne car pas utilisé dans le cpp fourni

		bool PSx[25],DxD[13],DxG[13],CPx[11],CPIx[9];

	public:
		// Constructeur prend un pointeur vers le Node
		Capteurs(std::shared_ptr<rclcpp::Node> noeud);
		~Capteurs();

		void Actualiser(bool PS[],bool DD[],bool DG[],bool CP[],bool CPI[]);

		// Callback avec SharedPtr
		void Callback_capteurs_simulation(const aiguillages::msg::MsgSensorState::SharedPtr msg);

		bool get_CP(int num_CP);
		bool get_CPI(int num_CPI);
		bool get_DG(int num_DG);
		bool get_DD(int num_DD);
		bool get_PS(int num_PS);
};

#endif
