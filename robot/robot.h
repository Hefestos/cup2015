#ifndef ROBOT_H
#define ROBOT_H

#include<vector>

struct circle {
	float x;
	float y;
};

class robot{
	public:

		robot(void);
		robot(float, float, int);
		robot(float, float, int, float);
		robot(float, float);
		robot(float, float, float);
		robot(float, float, float, int, int);
		robot& operator=(const robot&);

		void setPos(robot);
		void setPos(float, float);
		void setSec1(int, int);
		void setSec2(int, int);
		void setSec1(robot);
		void setSec2(robot);
		void setGoal(robot);
		void setGoal(float, float);
		void setObjective(robot);
		void setObjective(float, float);

		void setComportamento(int);				//Seta o comportamento do carro
		void setPosse(bool, int);				//Seta se o carro tem ou nao a posse de bola
		void setDistancia_bola(float);			//Seta a distancia do carro a bola
		void setDistancia_gol(float);			//Seta a distancia do carro ao gol


		/*------------------------------------------- Variaveis ----------------------------------------------------*/
		int g_value;
		float h_value;
		float x;				  //Coordenada x do carro
		float y;				  //Coordenada y do carro
		float theta;			  //Orientacao do carro
		int comportamento;        //1-->Goleiro ; 2-->Atacante ; 3-->Lateral
		bool posse;               //Posse = 1 --> tem bola, posse = 0 --> nao tem bola
		float distancia_bola;     //Distancia do carro a bola
		float distancia_gol;      //Distancia do carro ao gol

		float goal_x;
		float goal_y;

		int flag_pos_bola;        //Diz se a posse da bola é na frente (1) ou atras (0)

		float refaux_acumulated;
		float refaux_old;
		float refaux;
		float theta_acumulated;
		float theta_old;

		float objective_x;
		float objective_y;

		int sec1_x;					//Ponto de segurança 1
		int sec1_y;					//Ponto de segurança 1
		int sec2_x;					//Ponto de segurança 2
		int sec2_y;					//Ponto de segurança 2


		bool stop;

		std::vector<float> group_x;
		std::vector<float> group_y;
		std::vector<float> group_theta;
		float gain[3];                          // 0 = PROPORCIONAL, 1 = INTEGRAL, 2 = DERIVATIVO
		float old_gain[3];					// ganhos anteriores -> usado no calculo dos ganhos
		float limit_gain[6];                    // pares -> inferiores, impares -> superiores
		float condition_x[6];				// 0-ts; 1-over; 2-uper; 3-prev_ts; 4-prev_over; 5-prev_uper
		float condition_y[6];
		float condition_theta[6];
};

#endif
