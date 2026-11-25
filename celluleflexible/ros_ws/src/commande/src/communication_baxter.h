/**** Bruno DATO M1 EEA ISTR Université Paul Sabatier Toulouse III 2016 ****/
#ifndef BAXT
#define BAXT

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/bool.hpp>

class Communication_Baxter
{
private:

    rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_prise_demandee_bras_droit;
	rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr pub_prise_demandee_bras_gauche;

	rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_prise_effectuee_bras_droit;
	rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_prise_effectuee_bras_gauche;
	rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_attente_prise_bras_droit;
	rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr sub_attente_prise_bras_gauche;

public:

	std_msgs::msg::Bool msg_prise_demandee_bras_droit;
	std_msgs::msg::Bool msg_prise_demandee_bras_gauche;
	std_msgs::msg::Bool msg_prise_effectuee_bras_droit, msg_prise_effectuee_bras_gauche ;
	std_msgs::msg::Bool msg_attente_prise_bras_droit, msg_attente_prise_bras_gauche ;

	Communication_Baxter(rclcpp::Node::SharedPtr node);
	~Communication_Baxter();

	void Callback_prise_effectuee_bras_droit(const std_msgs::msg::Bool::SharedPtr msg);
	void Callback_prise_effectuee_bras_gauche(const std_msgs::msg::Bool::SharedPtr msg);
	void Callback_attente_prise_bras_gauche(const std_msgs::msg::Bool::SharedPtr msg);
	void Callback_attente_prise_bras_droit(const std_msgs::msg::Bool::SharedPtr msg);

	bool Prise_effectuee_bras_droit();
	bool Prise_effectuee_bras_gauche();

	bool Attente_prise_bras_droit();
	bool Attente_prise_bras_gauche();

	void Demander_prise_bras_droit();
	void Demander_prise_bras_gauche();

	void Afficher_Communication_Baxter();

	void Update();
};
#endif
