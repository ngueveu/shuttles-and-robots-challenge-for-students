/**** Bruno DATO, Abdellah ELGOURAIN, Evgeny SHULGA M1 EEA ISTR Université Paul Sabatier Toulouse III 2016 ****/
#ifndef CAPT
#define CAPT

#include <aiguillages/msg/msg_sensor_state.hpp>
#include <rclcpp/rclcpp.hpp>

class Capteurs
{
	private:
		rclcpp::Subscription<aiguillages::msg::MsgSensorState>::SharedPtr sub_capteurs_simu;

		bool PSx[25],DxD[13],DxG[13],CPx[11],CPIx[9];

	public:
		Capteurs(rclcpp::Node::SharedPtr node);
		~Capteurs();

		void Actualiser(bool PS[],bool DD[],bool DG[],bool CP[],bool CPI[]);

		void Callback_capteurs_simulation(aiguillages::msg::MsgSensorState::SharedPtr msg);

		bool get_CP(int num_CP);
		bool get_CPI(int num_CPI);
		bool get_DG(int num_DG);
		bool get_DD(int num_DD);
		bool get_PS(int num_PS);

};

bool MASK(long int registre,int numero_bit);

#endif
