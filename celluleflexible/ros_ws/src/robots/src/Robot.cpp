/**** Projet long N7 2020 ****/
#include "Robot.h"
#include <chrono>

using namespace std;

//Constructeur
Robot::Robot(int num_du_robot): Node("robot_" + std::to_string(num_du_robot))
{
	//Valeur de pi
	pi=3.14159265359;
	num_robot=num_du_robot;
	loop_rate = new rclcpp::Rate(25);
	loop_ok = new rclcpp::Rate(2);
	retour.num_robot=num_robot;
	RCLCPP_INFO(this->get_logger(),"Le numero du robot est %d", num_robot);
	for (int i=0;i<7;i++)
	{
		//ATTENTION : Ne pas changer la valeur d'initialisation
		//l'ensemble des setModes doit être à 1 pour que les joints des robots soient commandés
		mymodes[i]=1;
	}

	for(int i=0; i<NB_CUBE; i++)
		couleur_transportee[i]=0;

	repSim_getObjectHandle=false;
	repSim_setJointState=false;
	repSim_getJointState=false;
	repSim_getTime=false;
	repSim_changeColor=false;
	repSim_changeShuttleColor=false;
	repSim_getColor=false;
	repSim_getColorUpdate=false;

	msgSim_setJointState.layout.dim.push_back(std_msgs::msg::MultiArrayDimension());
	msgSim_setJointState.layout.dim[0].label="handles";
	msgSim_setJointState.layout.dim[0].size=7; // On utilise que cette valeur
	msgSim_setJointState.layout.dim.push_back(std_msgs::msg::MultiArrayDimension());
	msgSim_setJointState.layout.dim[0].label="values";
	msgSim_setJointState.layout.dim[0].size=7;
}

//Destructeur
Robot::~Robot()
{
	delete loop_rate;
	delete loop_ok;
}

/** Pour atteindre une position prédéfinie **/
//Fonction permettant d'envoyer le robot dans une position prédéfinie
void Robot::EnvoyerRobot(int numposition)
{
	robotPosition.data=numposition;
	//Choix de la position en fonction du numéro reçu
	switch(numposition)
	{
		case 1:
			Rpos[0]=128*pi/180;       // attributs correspondant au position (7 axes du robot)
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-40*pi/180;
			break;

		case 2:
			Rpos[0]=60*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-20*pi/180;
			break;

		case 3:
			Rpos[0]=15*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-65*pi/180;
			break;

		case 4:
			Rpos[0]=-53*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-40*pi/180;
			break;
	}


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(static_cast<float>(Rints[i]));
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState->publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&rclcpp::ok())
	{
		loop_rate->sleep();
	}
	repSim_setJointState=false;


	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé :
	//on considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState->publish(msgSim_getJointState);
		while(!repSim_getJointState&&rclcpp::ok())
		{
			loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(std::abs(Position-Rpos[i])>=0.001&&rclcpp::ok())
		{
			pubSim_getJointState->publish(msgSim_getJointState);
			while(!repSim_getJointState&&rclcpp::ok())
			{
				loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;

			loop_rate->sleep();
		}
	}

	RCLCPP_INFO(this->get_logger(), "Position atteinte robot %d", num_robot);

	//Retour vers la commande
	retour.data = 2;
	pub_retourCommande->publish(retour);

	pub_robotPosition->publish(robotPosition);
}

/** Pour atteindre une position définie manuellement **/
//Fonction permettant d'envoyer le robot dans une position définie manuellement
void Robot::EnvoyerJoints(int joint1, int joint2, int joint3, int joint4, int joint5, int joint6, int joint7)
{
	//Récupération des angles choisis par l'utilisateur
	Rpos[0]=(joint1)*pi/180;
	Rpos[1]=(joint2)*pi/180;
	Rpos[2]=(joint3)*pi/180;
	Rpos[3]=(joint4)*pi/180;
	Rpos[4]=(joint5)*pi/180;
	Rpos[5]=(joint6)*pi/180;
	Rpos[6]=(joint7)*pi/180;


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(static_cast<float>(Rints[i]));
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState->publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&rclcpp::ok())
	{
        loop_rate->sleep();
	}
	repSim_setJointState=false;


	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé
	//On considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState->publish(msgSim_getJointState);
		while(!repSim_getJointState&&rclcpp::ok())
		{
        	loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(std::abs(Position-Rpos[i])>=0.001&&rclcpp::ok())
		{
			pubSim_getJointState->publish(msgSim_getJointState);
			while(!repSim_getJointState&&rclcpp::ok())
			{
        		loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;

			loop_rate->sleep();

		}
	}

    RCLCPP_INFO(this->get_logger(), "Position atteinte %d", num_robot);

	//Retour vers la commande
	retour.data = 2;
	pub_retourCommande->publish(retour);
}

/** Pour descendre ou monter le bras **/
//Fonction permettant de mettre le bras en position basse
void Robot::DescendreBras()
{
	//Récupération et modification de la position actuelle
	Rpos[0]=Rpos[0]+2*pi/180;
	Rpos[1]=Rpos[1]+10*pi/180;
	Rpos[2]=Rpos[2];
	Rpos[3]=Rpos[3];
	Rpos[4]=Rpos[4]+10*pi/180;
	Rpos[5]=Rpos[5]+3*pi/180;
	Rpos[6]=Rpos[6]+6*pi/180;


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(static_cast<float>(Rints[i]));
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState->publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&rclcpp::ok())
	{
        loop_rate->sleep();
	}
	repSim_setJointState=false;

	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé
	//On considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState->publish(msgSim_getJointState);
		while(!repSim_getJointState&&rclcpp::ok())
		{
        	loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(std::abs(Position-Rpos[i])>=0.001&&rclcpp::ok())
		{
			pubSim_getJointState->publish(msgSim_getJointState);
			while(!repSim_getJointState&&rclcpp::ok())
			{
        		loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;
			loop_rate->sleep();

		}
	}


	RCLCPP_INFO(this->get_logger(), "Bras descendu pour le robot %d", num_robot);

	//Retour vers la commande
	retour.data = 4;
	pub_retourCommande->publish(retour);

	//Retour de l'état actuel du bras
	robotBras.data = 0;
	pub_robotBras->publish(robotBras);
}

//Fonction permettant de mettre le bras en position haute
void Robot::MonterBras()
{
	//Récupération et modification de la position actuelle
	Rpos[0]=Rpos[0]-2*pi/180;
	Rpos[1]=Rpos[1]-10*pi/180;
	Rpos[2]=Rpos[2];
	Rpos[3]=Rpos[3];
	Rpos[4]=Rpos[4]-10*pi/180;
	Rpos[5]=Rpos[5]-3*pi/180;
	Rpos[6]=Rpos[6]-6*pi/180;


	// Utilisation du topic SetJointState pour envoyer le robot dans la position souhaitée
	msgSim_setJointState.data.clear();
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(static_cast<float>(Rints[i]));
	for(int i=0; i<7; i++)
		msgSim_setJointState.data.push_back(Rpos[i]);

	pubSim_setJointState->publish(msgSim_setJointState);

	//Attente de la réponse
	while(!repSim_setJointState&&rclcpp::ok())
	{
        loop_rate->sleep();
	}
	repSim_setJointState=false;

	//Utilisation du topic GetJointState pour savoir si le mouvement du robot est terminé
	//On considère que la position est atteinte si la position est à 0.001 de sa consigne
	float Position;
	for(int i=0;i<7;i++)
	{
		Position=0;

		//Appel du topic pour connaître la position du robot
		msgSim_getJointState.data = Rints[i];
		pubSim_getJointState->publish(msgSim_getJointState);
		while(!repSim_getJointState&&rclcpp::ok())
		{
        	loop_rate->sleep();
		}
		repSim_getJointState = false;
		Position = valueSim_getJointState;

		//Attente jusqu'à ce que la position soit atteinte
		while(std::abs(Position-Rpos[i])>=0.001&&rclcpp::ok())
		{
			pubSim_getJointState->publish(msgSim_getJointState);
			while(!repSim_getJointState&&rclcpp::ok())
			{
                loop_rate->sleep();
			}
			repSim_getJointState=false;
			Position = valueSim_getJointState;
			loop_rate->sleep();

		}
	}


	RCLCPP_INFO(this->get_logger(), "Bras monte pour le robot %d", num_robot);

	//Retour vers la commande
	retour.data = 5;
	pub_retourCommande->publish(retour);

	//Retour de l'état actuel du bras
	robotBras.data = 1;
	pub_robotBras->publish(robotBras);
}

/** Pour fermer ou ouvrir la pince **/
//Fonction permettant de fermer la pince du robot en envoyant une commande sur le topic correspondant
void Robot::FermerPince()
{
	//Message de commande de fermeture de la pince (1 pour fermer, 0 pour ouvrir)
	std_msgs::msg::Int32 cmd;
	cmd.data = 1;

	pub_pince->publish(cmd);

	//Si la pince n'est pas déjà fermée, un retour est envoyé à la commande
	if(retour.data != 6)
	{
		//Attente pour que la pince se ferme
		double t0 = this->now().seconds();
		double time = t0;
        
		while(time - t0 < 0.5 && rclcpp::ok())
		{
			time = this->now().seconds();
			loop_rate->sleep(); // On utilise loop_rate (25Hz) pour plus de fluidité

		}

		//Retour vers la commande
		retour.data = 6;
		pub_retourCommande->publish(retour);
	}

	//Retour de l'état de la pince
	robotPince.data=1;
	pub_robotPince->publish(robotPince);
}

//Fonction permettant d'ouvrir la pince du robot en envoyant une commande sur le topic correspondant
void Robot::OuvrirPince()
{
	//Message de commande d'ouverture de la pince (1 pour fermer, 0 pour ouvrir)
	std_msgs::msg::Int32 cmd;
	cmd.data = 0;

	pub_pince->publish(cmd);

	//Si la pince n'est pas déjà ouverte, un retour est envoyé à la commande
	if(retour.data != 7)
	{
		//Attente pour que la pince s'ouvre
		double t0 = this->now().seconds();
		double time = t0;
        
		while(time - t0 < 0.5 && rclcpp::ok())
		{
			time = this->now().seconds();
			loop_rate->sleep(); 


		}

		//Retour vers la commande
		retour.data = 7;
		pub_retourCommande->publish(retour);
	}

	//Retour de l'état de la pince
	robotPince.data=0;
	pub_robotPince->publish(robotPince);
}

/*** Fonctions permettant de controler le robot avec des ordres du noeud commande ***/
/** Envoyer le robot automatiquement **/
//Fonction Callback permettant d'envoyer le robot dans une position prédéfinie à la réception du message de Commande
void Robot::SendPositionCallback(const robots::msg::MsgNumRobot::SharedPtr msg)
{
	if(num_robot==msg->num_robot)
	{
		//Récupération des données du message : numéro de la position prédéfinie
		int pos;
		pos = msg->data;

		//Envoi du robot dans la position choisie
		EnvoyerRobot(pos);
	}
}

/** Envoyer le robot manuellement **/
//Fonction Callback permettant d'envoyer le robot dans une position choisie par l'utilisateur à la réception du message de Commande
void Robot::SendJointsCallback(const commande_locale::msg::RobotJoints::SharedPtr msg)
{
	if(num_robot==msg->num_robot)
	{
		EnvoyerJoints(msg->joint1, msg->joint2, msg->joint3, msg->joint4, msg->joint5, msg->joint6, msg->joint7);
	}
}

/** Fermer la pince **/
//Fonction Callback permettant de fermer la pince du robot à la réception du message de Commande
void Robot::FermerPinceCallback(const robots::msg::MsgNumRobot::SharedPtr msg)
{
	if(num_robot==msg->num_robot)
	{
		FermerPince();
	}
}
/** Ouvrir la pince **/
//Fonction Callback permettant d'ouvrir la pince du robot à la réception du message de Commande
void Robot::OuvrirPinceCallback(const robots::msg::MsgNumRobot::SharedPtr msg)
{
	if(num_robot==msg->num_robot)
	{
		OuvrirPince();
	}
}

/** Descendre le bras **/
//Fonction Callback permettant de mettre le bras en position basse à la réception du message de Commande
void Robot::DescendreBrasCallback(const robots::msg::MsgNumRobot::SharedPtr msg)
{
	if(num_robot==msg->num_robot)
	{
		DescendreBras();
	}
}

/** Monter le bras **/
//Fonction Callback permettant de mettre le bras en position haute à la réception du message de Commande
void Robot::MonterBrasCallback(const robots::msg::MsgNumRobot::SharedPtr msg)
{
	if(num_robot==msg->num_robot)
	{
		MonterBras();
	}
}

/** Contrôler le robot entièrement **/
//Fonction Callback permettant de contrôler l'ensemble des mouvements du robot à la réception du message de Commande
void Robot::ControlerRobotCallback(const robots::msg::MoveRobot::SharedPtr msg)
{
	if(num_robot==msg->num_robot)
	{
		//Envoi du robot dans la position souhaitée
		EnvoyerRobot(msg->position);

		//Envoi du bras dans l'état souhaité
		switch(msg->bras)
		{
			case -1:
				DescendreBras();
				break;

			case 1:
				MonterBras();
				break;
		}

		//Envoi de la pince dans l'état souhaité
		switch(msg->pince)
		{
			case -1:
				OuvrirPince();
				break;

			case 1:
				FermerPince();
				break;
		}
	}
}

int Robot::computeTableId(int position)
{
	int id=-1;

	switch(num_robot)
	{
		case 1:
			if(position==1)
				id=1;
			else if(position==4)
				id=0;
			break;
		case 2:
			if(position==1)
				id=3;
			else if(position==4)
				id=4;
			break;
		case 3:
			if(position==1)
				id=7;
			else if(position==4)
				id=6;
			break;
		case 4:
			if(position==1)
				id=9;
			else if(position==4)
				id=10;
			break;
	}

	return id;
}

void Robot::Colorer(int position, int type)//attention c'est forcement quand on transporte !!
{
	std::lock_guard<std::mutex> lock(color_mutex_);
	
	// type==0 <=> prise /  =1 <=> pose
	int idNavette=-1;
	if(position==2 || position==3) // Si navette
	{
		auto request = std::make_shared<shuttles::srv::ShuttleId::Request>();
		request->robot = num_robot;
		request->position = position;

		// On s'assure juste que le service existe avant de tirer dans le vide
		client->wait_for_service(std::chrono::seconds(1)); 

		auto result_future = client->async_send_request(request);

		if (result_future.wait_for(std::chrono::seconds(3)) != std::future_status::ready)
		{
			RCLCPP_ERROR(this->get_logger(), "Erreur lors de l'appel du service");
			return;
		}

		idNavette = result_future.get()->id_shuttle;
		RCLCPP_INFO(this->get_logger(), "Navette %d", idNavette);
	}
	
	if(idNavette==66) // 66=Erreur
	{
		RCLCPP_ERROR(this->get_logger(),"ERREUR : Pas de navette a la position demandee");	
		for(int i=0; i<NB_CUBE; i++)
			couleur_transportee[i]=0;
		transport(false);
	}
	else
	{
		// regarde la couleur de ce qu'on veut prendre
		bool couleur_vide=true;
		int couleur[NB_CUBE];
		char c[2];
		if (idNavette == 0)
            c[0] = static_cast<char>(idNavette + 90);
        else
            c[0] = static_cast<char>(idNavette + 64);
        c[1] = '\0';
		
		std::string signal;
		if(position==2 || position==3) // Si navette
			signal="Shuttle"+std::string(c);
		else if(position==1 || position==4) // Si poste
		{
			if(position==1)
			{
				signal=poste_pos_1.get_nom();
				if(poste_pos_1.isTaskEnCours())
				{
					// Log
					RCLCPP_ERROR(this->get_logger(),"Manipulation d'une piece en cours de traitement ! [poste=%d]", poste_pos_1.get_numero());
					commande_locale::msg::MsgErreur msgErreur;
					msgErreur.code=3;
					msgErreur.n_poste=poste_pos_1.get_numero();
					pub_erreur_log->publish(msgErreur);

					// Interrompt tache
					poste_pos_1.stopTask();
					retour.data=8;
					pub_retourCommande->publish(retour);
				}
			}
			else
			{
				signal=poste_pos_4.get_nom();
				if(poste_pos_4.isTaskEnCours())
				{
					// Log
					RCLCPP_ERROR(this->get_logger(),"Manipulation d'une piece en cours de traitement ! [poste=%d]", poste_pos_4.get_numero());
					commande_locale::msg::MsgErreur msgErreur;
					msgErreur.code=3;
					msgErreur.n_poste=poste_pos_4.get_numero();
					pub_erreur_log->publish(msgErreur);

					// Interrompt tache
					poste_pos_4.stopTask();
					retour.data=9;
					pub_retourCommande->publish(retour);
				}
			}
		}

		std::string fin;

		for(int i=0; i<NB_CUBE; i++)
		{
			fin.clear();
			fin.append(signal);
			fin.append("#");
			fin.append(std::to_string(i));
			fin.append("_color");
			msgSim_getColor.data=fin;

			pubSim_getColor->publish(msgSim_getColor);
			while(!repSim_getColor&&rclcpp::ok())
			{
				loop_rate->sleep();
			}
			repSim_getColor=false;
			couleur[i]=valueSim_getColor;
		}
		for(int i=0; i<NB_CUBE; i++)
		{
			if(couleur[i]!=0)
				couleur_vide=false;
		}

		// colore le poste ou navette en pos 1 avec couleur en mémoire
		if(position==2 || position==3) // Si navette
		{
			msgSim_changeShuttleColor.data.clear();
			msgSim_changeShuttleColor.data.push_back(idNavette);
			for(int i=0; i<NB_CUBE; i++)
				msgSim_changeShuttleColor.data.push_back(couleur_transportee[i]);
			pubSim_changeShuttleColor->publish(msgSim_changeShuttleColor);

			while(!repSim_changeShuttleColor&&rclcpp::ok())
			{
                loop_rate->sleep();
			}
			repSim_changeShuttleColor=false;
		}
		else if(position==1 || position==4)
		{
			msgSim_changeColor.data.clear();
			msgSim_changeColor.data.push_back(computeTableId(position));
			for(int i=0; i<NB_CUBE; i++)
				msgSim_changeColor.data.push_back(couleur_transportee[i]);
			pubSim_changeColor->publish(msgSim_changeColor);

			while(!repSim_changeColor&&rclcpp::ok())
			{
				loop_rate->sleep();
			}
			repSim_changeColor=false;
		}

		// on met a jour la couleur en mémoire (qu'on transporte) seulement si prise
		for(int i=0; i<NB_CUBE; i++)
		{
			if(type==0)
				couleur_transportee[i]=couleur[i];
			else
				couleur_transportee[i]=0;
			std::cout << "couleur_trasportee[" << i << "]=" << couleur_transportee[i] << std::endl;
		}

		// Mise jour modele pince (si tiens quelque chose, non vide)
		if(type==0 && !couleur_vide)
			transport(true);
		else
			transport(false);

		// Detecte si on a écrasé un produit
		if(type==1 && !couleur_vide)
		{
			RCLCPP_ERROR(this->get_logger(), "ON A ÉCRASÉ UN PRODUIT !!!");
			msg_erreur.code=66; //66=code ecrasement produit
			msg_erreur.n_poste=num_robot;
			pub_erreur_log->publish(msg_erreur);
		}
	}
}

int Robot::colorerPosteDebutTask(int positionPoste)
{
	std::lock_guard<std::mutex> lock(color_mutex_);

	std::string signal;
	std::string fin;
	int couleur[NB_CUBE];
	
	for(int i=0; i<NB_CUBE; couleur[i++]=0){}
	int couleur_last(0);
	int retour=-1;
	int n_poste;
	int couleur_a_ajouter;

	if(positionPoste==1)
	{
		signal=poste_pos_1.get_nom();
		n_poste=poste_pos_1.get_numero();
		couleur_a_ajouter=poste_pos_1.get_color()-1; // -1 pour opacité 50%
	}
	if(positionPoste==4)
	{
		signal=poste_pos_4.get_nom();
		n_poste=poste_pos_4.get_numero();
		couleur_a_ajouter=poste_pos_4.get_color()-1;
	}

	// On cherche la 1ere case vide
	int i=0;
	do
	{
		fin.clear();
		fin.append(signal);
		fin.append("#");
		fin.append(std::to_string(i));
		fin.append("_color");
		msgSim_getColor.data=fin;

		pubSim_getColor->publish(msgSim_getColor);
		while(!repSim_getColor&&rclcpp::ok())
		{
			loop_rate->sleep();
		}
		repSim_getColor=false;
		couleur[i]=valueSim_getColor;
		couleur_last=couleur[i];

		i++;
	} while(i<NB_CUBE && couleur_last!=0 && rclcpp::ok());


	if(i==1)
	{
		RCLCPP_ERROR(this->get_logger(), "TACHE SUR AUCUN PRODUIT !!!");
		msg_erreur.code=1;
		msg_erreur.n_poste=n_poste;
		pub_erreur_log->publish(msg_erreur);
	}

	else if(i==NB_CUBE && couleur_last!=0)
	{
		RCLCPP_ERROR(this->get_logger(), "PRODUIT PLEIN !!!");
		msg_erreur.code=2;
		msg_erreur.n_poste=n_poste;
		pub_erreur_log->publish(msg_erreur);
	}
	else
	{
		// mettre couleur sur signal/case i-1
		std::string idStr= signal.substr(6);
		int idPoste = atoi(idStr.c_str());
		msgSim_changeColor.data.clear();
		msgSim_changeColor.data.push_back(idPoste);

		couleur[i-1]=couleur_a_ajouter;
		std::cout << "couleur_a_ajouter=" << couleur_a_ajouter << std::endl;

		for(int j=0; j<NB_CUBE; j++)
			msgSim_changeColor.data.push_back(couleur[j]);

		pubSim_changeColor->publish(msgSim_changeColor);
		while(!repSim_changeColor&&rclcpp::ok())
		{
			loop_rate->sleep();
		}
		repSim_changeColor=false;

		retour = i-1;
	}

	return retour;
}

int Robot::colorerPosteFinTask(int positionPoste, int duree)
{
	std::lock_guard<std::mutex> lock(color_mutex_);

	std::string signal;
	std::string fin;
	int couleur[NB_CUBE];
	for(int i=0; i<NB_CUBE; couleur[i++]=0){}
	int couleur_last(0);
	int retour=-1;
	int n_poste;
	int couleur_a_ajouter;

	if(positionPoste==1)
	{
		signal=poste_pos_1.get_nom();
		n_poste=poste_pos_1.get_numero();
		couleur_a_ajouter=poste_pos_1.get_color();
	}
	if(positionPoste==4)
	{
		signal=poste_pos_4.get_nom();
		n_poste=poste_pos_4.get_numero();
		couleur_a_ajouter=poste_pos_4.get_color();
	}

	// On cherche 1ere case vide
	int i=0;
	do
	{
		fin.clear();
		fin.append(signal);
		fin.append("#");
		fin.append(std::to_string(i));
		fin.append("_color");
		msgSim_getColor.data=fin;

		pubSim_getColor->publish(msgSim_getColor);
		while(!repSim_getColor&&rclcpp::ok())
		{
			loop_rate->sleep();
		}
		repSim_getColor=false;
		couleur[i]=valueSim_getColor;
		couleur_last=couleur[i];

		i++;

	}while(i<NB_CUBE && couleur_last!=0&&rclcpp::ok());

	// mettre couleur sur signal i-1
	std::string idStr= signal.substr(6);
	int idPoste = atoi(idStr.c_str());
	msgSim_changeColor.data.clear();
	msgSim_changeColor.data.push_back(idPoste);

	if(i==NB_CUBE && couleur_last!=0)
	{
		couleur[i-1]=couleur_a_ajouter;
		retour = i-1;
	}
	else
	{
		couleur[i-2]=couleur_a_ajouter;
		retour = i-2;
	}
	std::cout << "couleur_a_ajouter=" << couleur_a_ajouter << std::endl;

	for(int j=0; j<NB_CUBE; j++)
		msgSim_changeColor.data.push_back(couleur[j]);
	pubSim_changeColor->publish(msgSim_changeColor);
	while(!repSim_changeColor&&rclcpp::ok())
	{
        loop_rate->sleep();
	}
	repSim_changeColor=false;

	// pour le log
	RCLCPP_INFO(this->get_logger(), "Task Po:%d, Pr:%d, Du%d",n_poste,couleur[0],duree);
	msg_tache_finie.num_poste=n_poste;
	msg_tache_finie.num_produit=couleur[0];
	msg_tache_finie.duree=duree;
	pub_tache_finie->publish(msg_tache_finie);

	return retour;
}

void Robot::faireTacheCallback(const robots::msg::FaireTacheMsg::SharedPtr msg)
{
	if((msg->num_robot==num_robot)
			&& (msg->position==1||msg->position==4)) // pas sur une navette
	{
		rclcpp::sleep_for(std::chrono::milliseconds(2500));
		
		RCLCPP_INFO(this->get_logger(), "Debut tache pos=%d", msg->position);
		int retourDebTask = colorerPosteDebutTask(msg->position);
		double time = this->now().seconds();

		if(msg->position==1)
		{
			if(poste_pos_1.isTaskEnCours())
				RCLCPP_ERROR(this->get_logger(),"ERREUR : Nouvelle tache pendant une tache en cours !");
			//  commence la tache seulement si retour ok
			if(retourDebTask!=-1)
				poste_pos_1.debutTask(time,msg->duree);
			else
			{
				retour.data=8;
				pub_retourCommande->publish(retour);
			}
		}
		else
		{
			if(poste_pos_4.isTaskEnCours())
				RCLCPP_ERROR(this->get_logger(),"ERREUR : Nouvelle tache pendant une tache en cours !");
			//  commence la tache seulement si retour ok
			if(retourDebTask!=-1)
				poste_pos_4.debutTask(time,msg->duree);
			else
			{
				retour.data=9;
				pub_retourCommande->publish(retour);
			}
		}
	}
}

//Fonction principale de Robot
void Robot::update()
{
	if(is_paused_) return;
	
	// Si une tache est en cours
	if(poste_pos_1.isTaskEnCours() || poste_pos_4.isTaskEnCours())
	{
		
		// On demande le temps à VREP
		double time = this->now().seconds();

		
		std::cout << std::endl;

		// Si la tache poste pos 1 en cours, on update
		if(poste_pos_1.isTaskEnCours())
		{
			// On update et verifie si la tache poste pos 1 est finie
			if(poste_pos_1.updateTask(time)) // si tache finie
			{
				int indice=colorerPosteFinTask(1,poste_pos_1.get_duree());
				if(indice==-1)
					RCLCPP_ERROR(this->get_logger(),"ColorerPosteTask Probleme !!");
				std::string signal=poste_pos_1.get_nom();
				std::string fin;
				fin.append(signal);
				fin.append("#");
				fin.append(std::to_string(indice));
				fin.append("_color");
				msgSim_getColor.data=fin;
				int couleur;

				do
				{
					pubSim_getColor->publish(msgSim_getColor);
					while(!repSim_getColor&&rclcpp::ok())
					{
						loop_rate->sleep();
					}
					repSim_getColor=false;
					couleur=valueSim_getColor;

					loop_rate->sleep();
				}while(couleur!=poste_pos_1.get_color()&&rclcpp::ok());

				retour.data=8;
				pub_retourCommande->publish(retour);
			}
		}

		// Si la tache poste pos 4 est en cours, on update
		if(poste_pos_4.isTaskEnCours())
		{
			// On update et verifie si la tache poste pos 4 est finie
			if(poste_pos_4.updateTask(time)) // si tache finie
			{
				int indice=colorerPosteFinTask(4,poste_pos_4.get_duree());
				if(indice==-1)
					RCLCPP_ERROR(this->get_logger(),"ColorerPosteTask Probleme !!");
				std::string signal=poste_pos_4.get_nom();
				std::string fin;
				fin.append(signal);
				fin.append("#");
				fin.append(std::to_string(indice));
				fin.append("_color");
				msgSim_getColor.data=fin;
				int couleur;

				do
				{
					pubSim_getColor->publish(msgSim_getColor);
					while(!repSim_getColor&&rclcpp::ok())
					{
						loop_rate->sleep();
					}
					repSim_getColor=false;
					couleur=valueSim_getColor;

					loop_rate->sleep();
				}while(couleur!=poste_pos_4.get_color()&&rclcpp::ok());
				retour.data=9;
				pub_retourCommande->publish(retour);
			}
		}
	}
}

void Robot::transport(bool valeur)
{
	std_msgs::msg::Bool msg;
	msg.data=valeur;
	pub_robot_transport->publish(msg);
}

// On definit qui evacue et comment
void Robot::Evacuer(const robots::msg::EvacuerMsg::SharedPtr msg)
{
	std::lock_guard<std::mutex> lock(color_mutex_);   // protege l'acces couleur (multithread)

	int numero_poste = msg->numero_poste;   // poste demande par l'etudiant

	int robot_cible = 0;   // robot responsable de ce poste
	int position    = 0;   // position du poste chez ce robot (1 ou 4)

	if(numero_poste==1) { robot_cible=1; position=4; }   // poste 1 -> robot 1
	if(numero_poste==2) { robot_cible=1; position=1; }   // poste 2 -> robot 1
	if(numero_poste==3) { robot_cible=2; position=1; }   // poste 3 -> robot 2
	if(numero_poste==4) { robot_cible=2; position=4; }   // poste 4 -> robot 2
	if(numero_poste==5) { robot_cible=3; position=1; }   // poste 5 -> robot 3
	if(numero_poste==6) { robot_cible=3; position=4; }   // poste 6 -> robot 3
	if(numero_poste==7) { robot_cible=4; position=1; }   // poste 7 -> robot 4
	if(numero_poste==8) { robot_cible=4; position=4; }   // poste 8 -> robot 4

	if(num_robot==robot_cible)   // seul le robot concerne agit
	{
		int couleur[NB_CUBE];

		std::string signal;
		if(position==1)
			signal=poste_pos_1.get_nom();   // nom du poste en position 1
		else
			signal=poste_pos_4.get_nom();   // nom du poste en position 4

		msg_log_couleur.data.clear();
		msg_log_couleur.data.push_back(numero_poste);   // le poste evacue, en 1ere position du tableau
		std::string fin;

		for(int i=0; i<NB_CUBE; i++)   // lecture de la couleure de chaque cube
		{
			fin.clear();
			fin.append(signal);
			fin.append("#");
			fin.append(std::to_string(i));
			fin.append("_color");
			msgSim_getColor.data=fin;   // nom du signal couleur du cube i

			pubSim_getColor->publish(msgSim_getColor);   // demande la couleur a Coppelia
			int tentatives_get = 0;

			while(!repSim_getColor&&rclcpp::ok())
			{
				if(tentatives_get++ > 25) {   // pas de reponse -> on redemande
					pubSim_getColor->publish(msgSim_getColor);
					tentatives_get = 0;
				}
				rclcpp::sleep_for(std::chrono::milliseconds(40));
			}
			repSim_getColor=false;   // reset pour le cube suivant
			couleur[i]=valueSim_getColor;
			msg_log_couleur.data.push_back(couleur[i]);
			std::cout << "couleur[" << i << "]=" << couleur[i] << std::endl;
		}

		pub_produitEvac->publish(msg_log_couleur);   // envoie les couleurs au log

		msgSim_changeColor.data.clear();
		msgSim_changeColor.data.push_back(computeTableId(position));   // identifiant table Coppelia
		for(int i=0; i<NB_CUBE; i++)
			msgSim_changeColor.data.push_back(0);   // couleur 0 = disparition
		pubSim_changeColor->publish(msgSim_changeColor);

		int tentatives_change = 0;

		while(!repSim_changeColor && rclcpp::ok())
		{
			if(tentatives_change++ > 25) {   // pas de reponse -> on redemande
				pubSim_changeColor->publish(msgSim_changeColor);
				tentatives_change = 0;
			}
			rclcpp::sleep_for(std::chrono::milliseconds(40));
		}
		repSim_changeColor=false;   // reset pour la prochaine evacuation
	}
}

void Robot::stopTacheCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	if(msg->data==1) //pos
	{
		poste_pos_1.stopTask();
		retour.data=8;
		pub_retourCommande->publish(retour);
	}
	else if(msg->data==4)
	{
		poste_pos_4.stopTask();
		retour.data=9;
		pub_retourCommande->publish(retour);
	}
}

void Robot::DeplacerPieceCallback(const commande_locale::msg::DeplacerPieceMsg::SharedPtr msg)
{
	if (num_robot==msg->num_robot)
	{
		EnvoyerRobot(msg->positiona);
		DescendreBras();

		// Prise de pièce
		//le robot a rien en mémoire, il décolore
		Colorer(msg->positiona,0);
		FermerPince();

		MonterBras();
		EnvoyerRobot(msg->positionb);
		DescendreBras();

		// Pose de pièce
		//le robot a la couleur du produit en memoire, il colore
		Colorer(msg->positionb,1);
		OuvrirPince();

		MonterBras();
		retour.data = 10;
		pub_retourCommande->publish(retour);
	}
}

/*** Initialisation ***/
//Initialisation des services, des publishers et des subscribers + Récupération des handles des robots
void Robot::init()
{
	std::string nom;
	int numero_poste;

	callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::Reentrant);
	rclcpp::SubscriptionOptions options;
	options.callback_group = callback_group_;


	//En fonction du numéro attribué au robot lors du call de l'exécutable, on initialise différents postes.
	switch(num_robot){
		//Par exemple, pour le robot 1, nous initialisons les postes 1 et 2
		case 1:
			nom="Table#1";
			numero_poste=2;
			poste_pos_1.init(nom,numero_poste);		//Appelle la fonction init du fichier Poste.cpp
			nom="Table#0";
			numero_poste=1;
			poste_pos_4.init(nom,numero_poste);		//Appelle la fonction init du fichier Poste.cpp
			break;
		case 2:
			nom="Table#3";
			numero_poste=3;
			poste_pos_1.init(nom,numero_poste);
			nom="Table#4";
			numero_poste=4;
			poste_pos_4.init(nom,numero_poste);
			break;
		case 3:
			nom="Table#7";
			numero_poste=6;
			poste_pos_1.init(nom,numero_poste);
			nom="Table#6";
			numero_poste=5;
			poste_pos_4.init(nom,numero_poste);
			break;
		case 4:
			nom="Table#9";
			numero_poste=7;
			poste_pos_1.init(nom,numero_poste);
			nom="Table#10";
			numero_poste=8;
			poste_pos_4.init(nom,numero_poste);
			break;
		default:
			RCLCPP_INFO(this->get_logger(),"CHOIX ROBOT INCORRECT");		//Met un message d'erreur si le numero_poste n'est pas compris entre 1 et 5
			break;
	}

	// Topic pour V-Rep; s'abonne aux topics en publisher et subscriber
	// Tant q'uil n'y a pas de messages publiés sur les topics sur lesquels on s'abonne, on n'exécute rien
	pubSim_getObjectHandle = this->create_publisher<std_msgs::msg::String>("/sim_ros_interface/services/robot"+std::to_string(num_robot)+"/GetObjectHandle",100);
	subSim_getObjectHandle = this->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/services/response/robot"+std::to_string(num_robot)+"/GetObjectHandle",100,std::bind(&Robot::simGetObjectHandleCallback, this, std::placeholders::_1),options);
	
	pubSim_setJointState = this->create_publisher<std_msgs::msg::Float32MultiArray>("/sim_ros_interface/services/robot"+std::to_string(num_robot)+"/SetJointState",100);
	subSim_setJointState = this->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/response/robot"+std::to_string(num_robot)+"/SetJointState",100,std::bind(&Robot::simSetJointStateCallback, this, std::placeholders::_1),options);

	pubSim_getJointState = this->create_publisher<std_msgs::msg::Int32>("/sim_ros_interface/services/robot" + std::to_string(num_robot) + "/GetJointState", 100);
	subSim_getJointState = this->create_subscription<sensor_msgs::msg::JointState>("/sim_ros_interface/services/response/robot" + std::to_string(num_robot) + "/GetJointState", 100, std::bind(&Robot::simGetJointStateCallback, this, std::placeholders::_1),options);

	pubSim_getTime = this->create_publisher<std_msgs::msg::Byte>("/sim_ros_interface/services/robot" + std::to_string(num_robot) + "/GetTime", 100);
	subSim_getTime = this->create_subscription<std_msgs::msg::Float32>("/sim_ros_interface/services/response/robot" + std::to_string(num_robot) + "/GetTime", 100, std::bind(&Robot::simGetTimeCallback, this, std::placeholders::_1),options);

	pubSim_getTimeUpdate = this->create_publisher<std_msgs::msg::Byte>("/sim_ros_interface/services/robot" + std::to_string(num_robot) + "Update/GetTime", 100);
	subSim_getTimeUpdate = this->create_subscription<std_msgs::msg::Float32>("/sim_ros_interface/services/response/robot" + std::to_string(num_robot) + "Update/GetTime", 100, std::bind(&Robot::simGetTimeUpdateCallback, this, std::placeholders::_1),options);

	pubSim_changeColor = this->create_publisher<std_msgs::msg::Int32MultiArray>("/sim_ros_interface/services/robot" + std::to_string(num_robot) + "/ChangeColor", 100);
	subSim_changeColor = this->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/response/robot" + std::to_string(num_robot) + "/ChangeColor", 100, std::bind(&Robot::simChangeColorCallback, this, std::placeholders::_1),options);

	pubSim_changeShuttleColor = this->create_publisher<std_msgs::msg::Int32MultiArray>("/sim_ros_interface/services/robot" + std::to_string(num_robot) + "/ChangeShuttleColor", 100);
	subSim_changeShuttleColor = this->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/response/robot" + std::to_string(num_robot) + "/ChangeShuttleColor", 100, std::bind(&Robot::simChangeShuttleColorCallback, this, std::placeholders::_1),options);

	pubSim_getColor = this->create_publisher<std_msgs::msg::String>("/sim_ros_interface/services/robot" + std::to_string(num_robot) + "/GetColor", 100);
	subSim_getColor = this->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/services/response/robot" + std::to_string(num_robot) + "/GetColor", 100, std::bind(&Robot::simGetColorCallback, this, std::placeholders::_1),options);

	pubSim_getColorUpdate = this->create_publisher<std_msgs::msg::String>("/sim_ros_interface/services/robot" + std::to_string(num_robot) + "/GetColorUpdate", 100);
	subSim_getColorUpdate = this->create_subscription<std_msgs::msg::Int32>("/sim_ros_interface/services/response/robot" + std::to_string(num_robot) + "/GetColorUpdate", 100, std::bind(&Robot::simGetColorUpdateCallback, this, std::placeholders::_1),options);

	pub_robot_transport = this->create_publisher<std_msgs::msg::Bool>("/commande/Simulation/TransportBras" + std::to_string(num_robot), 10);
	pub_tache_finie = this->create_publisher<commande_locale::msg::TacheFinieMsg>("/commande/Simulation/TacheFinie",10);

	
	//Subscribers
	planifSendPosition = this->create_subscription<robots::msg::MsgNumRobot>("/commande/Simulation/SendPositionRobot",10,std::bind(&Robot::SendPositionCallback, this, std::placeholders::_1),options);
	planifSendJoints = this->create_subscription<commande_locale::msg::RobotJoints>("/commande/Simulation/SendJointsRobot",10,std::bind(&Robot::SendJointsCallback, this, std::placeholders::_1),options);
	planifFermerPince = this->create_subscription<robots::msg::MsgNumRobot>("/commande/Simulation/FermerPinceRobot",10,std::bind(&Robot::FermerPinceCallback, this, std::placeholders::_1),options);
	planifOuvrirPince = this->create_subscription<robots::msg::MsgNumRobot>("/commande/Simulation/OuvrirPinceRobot",10,std::bind(&Robot::OuvrirPinceCallback, this, std::placeholders::_1),options);
	planifDescendreBras = this->create_subscription<robots::msg::MsgNumRobot>("/commande/Simulation/DescendreBras",10,std::bind(&Robot::DescendreBrasCallback, this, std::placeholders::_1),options);
	planifMonterBras = this->create_subscription<robots::msg::MsgNumRobot>("/commande/Simulation/MonterBras",10,std::bind(&Robot::MonterBrasCallback, this, std::placeholders::_1),options);
	planifControlerRobot = this->create_subscription<robots::msg::MoveRobot>("/commande/Simulation/ControlerBras",10,std::bind(&Robot::ControlerRobotCallback, this, std::placeholders::_1),options);
	sub_faireTache = this->create_subscription<robots::msg::FaireTacheMsg>("/commande/Simulation/faireTache",10,std::bind(&Robot::faireTacheCallback, this, std::placeholders::_1),options);
	//sub_evacuer= this->create_subscription<std_msgs::msg::Byte>("/commande/Simulation/Evacuer",10,std::bind(&Robot::Evacuer, this, std::placeholders::_1),options);
	sub_evacuer= this->create_subscription<robots::msg::EvacuerMsg>("/commande/Simulation/Evacuer",10,std::bind(&Robot::Evacuer, this, std::placeholders::_1),options);
	subStopTache= this->create_subscription<std_msgs::msg::Int32>("/commande/Simulation/Robot"+std::to_string(num_robot)+"/StopTache",10,std::bind(&Robot::stopTacheCallback, this, std::placeholders::_1),options);
	subDeplacerPiece= this->create_subscription<commande_locale::msg::DeplacerPieceMsg>("/commande/Simulation/DeplacerPiece",10,std::bind(&Robot::DeplacerPieceCallback, this, std::placeholders::_1),options);
	sub_pause = this->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/vrep_controller/PauseSimulation", 10, std::bind(&Robot::pauseCallback, this, std::placeholders::_1), options);
	sub_play = this->create_subscription<std_msgs::msg::Byte>("/sim_ros_interface/services/vrep_controller/StartSimulation", 10, std::bind(&Robot::playCallback, this, std::placeholders::_1), options);

	//Publishers
	pub_pince = this->create_publisher<std_msgs::msg::Int32>("/robot/cmdPinceRobot"+std::to_string(num_robot), 10);
	pub_robotPosition = this->create_publisher<std_msgs::msg::Int32>("/robot/PositionRobot"+std::to_string(num_robot),10);
	pub_robotBras = this->create_publisher<std_msgs::msg::Int32>("/robot/BrasRobot"+std::to_string(num_robot),10);
	pub_robotPince = this->create_publisher<std_msgs::msg::Int32>("/robot/PinceRobot"+std::to_string(num_robot),10);
	pub_retourCommande = this->create_publisher<robots::msg::MsgNumRobot>("/commande/Simulation/retourCommande", 10);
	pub_produitEvac = this->create_publisher<std_msgs::msg::Int32MultiArray>("/commande/Simulation/produitEvac", 10);
	pub_erreur_log = this->create_publisher<commande_locale::msg::MsgErreur>("/commande/Simulation/Erreur_log",10);

	//Client (pour les services)
	client = this->create_client<shuttles::srv::ShuttleId>("get_id_shuttle_at_poste", rclcpp::ServicesQoS(), callback_group_);
	
	rclcpp::sleep_for(std::chrono::seconds(2));

	//Utilisation du service simRosGetObjectHandle pour obtenir les handles du robot
	for (int i=1;i<8;i++)
	{
		std::stringstream sr;
		sr << i;
		switch(num_robot){
			case 1:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str();
				break;
			case 2:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str()+"#0";
				break;
			case 3:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str()+"#1";
				break;
			case 4:
				msgSim_getObjectHandle.data = "LBR_iiwa_14_R820_joint" + sr.str()+"#2";
				break;
		}

		// On publie une première fois
		pubSim_getObjectHandle->publish(msgSim_getObjectHandle);
		
		int tentatives = 0;
		while(!repSim_getObjectHandle && rclcpp::ok())
		{
			// Si on attend depuis plus de 1 seconde (25 boucles de 40ms)
			if (tentatives++ > 25) 
			{
				pubSim_getObjectHandle->publish(msgSim_getObjectHandle); // On répète la question !
				tentatives = 0;
			}
			loop_rate->sleep();
		}
		
		repSim_getObjectHandle = false;
		Rints[i-1]=valueSim_getObjectHandle;

		if(Rints[i-1]==-1)
		{
			RCLCPP_INFO(this->get_logger(),"Robot %d : Handle non obtenu pour joint %d",num_robot,i);
		}
		else
		{
			if(i==7)
			{
				RCLCPP_INFO(this->get_logger(),"Robot %d OK",num_robot);
				//Retour vers la commande
				retour.data = 0;
				pub_retourCommande->publish(retour);
			}
		}
	}

}

/** Callbacks pour V-Rep **/
//Quand un message est publié sur un des topics vrep/Coppelia sur lesquels on est abonné, on exécute la fonction associée
void Robot::simGetObjectHandleCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	valueSim_getObjectHandle=msg->data;
	repSim_getObjectHandle=true;
}

void Robot::simSetJointStateCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	repSim_setJointState=true;
}

void Robot::simGetJointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg)
{
	valueSim_getJointState=msg->position[0];
	repSim_getJointState=true;
}

void Robot::simGetTimeCallback(const std_msgs::msg::Float32::SharedPtr msg)
{
	valueSim_getTime=msg->data;
	repSim_getTime=true;
}

void Robot::simGetTimeUpdateCallback(const std_msgs::msg::Float32::SharedPtr msg)
{
	valueSim_getTimeUpdate=msg->data;
	repSim_getTimeUpdate=true;
}

void Robot::simChangeColorCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	repSim_changeColor=true;
}

void Robot::simChangeShuttleColorCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	repSim_changeShuttleColor=true;
}

void Robot::simGetColorCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	valueSim_getColor=msg->data;
	repSim_getColor=true;
}

void Robot::simGetColorUpdateCallback(const std_msgs::msg::Int32::SharedPtr msg)
{
	valueSim_getColorUpdate=msg->data;
	repSim_getColorUpdate=true;
}

void Robot::pauseCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	if(!is_paused_) {
		is_paused_ = true;
		time_pause_started_ = this->now().seconds(); // On mémorise l'heure exacte de la pause
	}
}

void Robot::playCallback(const std_msgs::msg::Byte::SharedPtr msg)
{
	if(is_paused_) {
		// On calcule la durée de la pause dans le monde réel
		double duree_pause = this->now().seconds() - time_pause_started_;
		
		// On décale les tâches en cours pour compenser
		poste_pos_1.decalerTemps(duree_pause);
		poste_pos_4.decalerTemps(duree_pause);
		
		is_paused_ = false;
	}
}