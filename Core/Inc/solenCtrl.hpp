/*
 *  SolenCtrl.hpp
 *
 *  Created on: 2023/04/13
 *      Author: ykc
 */

struct solenoid{
	uint32_t BID;
	uint16_t Valve_Pin;             //GPIOPin
	GPIO_TypeDef* GPIO = GPIOB;     //GPIOグループ
	HAL_StatusTypeDef mode = HAL_OK;//動作許可(今回はデフォルトで許可)
	HAL_StatusTypeDef value_update(uint8_t Rdata);
	void safty_OK();
	void safty_ERROR();
	void EMS();
};

inline HAL_StatusTypeDef solenoid::value_update(uint8_t Rdata){
	if (mode == HAL_OK){
		if (Rdata == 0x01){
			HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_SET);
			return HAL_OK;
		}
		if (Rdata == 0x00){
			HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
			return HAL_OK;
		}
		else{
			return HAL_ERROR;
		}
	}
	else{
		return HAL_ERROR;
	}
}

inline void solenoid::safty_OK(){
	if(mode == HAL_ERROR){
//		if(Pin == 1){
		HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
//		}
	}
}

inline void solenoid::safty_ERROR(){
//		if(Pin == 0){}
//		else{
		    HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
//		}
//		if(mode == HAL_ERROR){}
//		else{
//		    mode = HAL_ERROR;
//		}
}

inline void solenoid::EMS(){
	HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
	if(mode == HAL_OK){
//	   mode = HAL_ERROR;
	}
}

class SolenCtrl{
private:
	HAL_StatusTypeDef pre_EMS = HAL_ERROR;//EMSの管理
	solenoid Valve[7];
public:
	void init();
	HAL_StatusTypeDef update(uint32_t RID,uint8_t rxData[8]);
	HAL_StatusTypeDef get_pre_EMS();
	HAL_StatusTypeDef set_pre_EMS(HAL_StatusTypeDef ad);
	HAL_StatusTypeDef EMS_down();
	void check_Safty_OK();
	void check_Safty_ERROR();
};

inline void SolenCtrl::init(){
	for (uint32_t i=0;i<0x7;i++){
		Valve[i].BID = (0x101+i);
	}
	Valve[0].Valve_Pin = Valve0_Pin;
	Valve[1].Valve_Pin = Valve1_Pin;
	Valve[2].Valve_Pin = Valve2_Pin;
	Valve[3].Valve_Pin = Valve3_Pin;
	Valve[4].Valve_Pin = Valve4_Pin;
	Valve[5].Valve_Pin = Valve5_Pin;
	Valve[6].Valve_Pin = Valve6_Pin;
	Valve[6].GPIO = GPIOC;
}

inline HAL_StatusTypeDef SolenCtrl::update(uint32_t RID,uint8_t rxData[8]){
	uint8_t data = rxData[0];
	for(int i=0;i<7;i++){
		if(RID == Valve[i].BID){
			if(Valve[i].value_update(data)){
				return HAL_OK;
			}
			else {
				return HAL_ERROR;
			}
		}
	}
	return HAL_ERROR;
}

inline HAL_StatusTypeDef SolenCtrl::get_pre_EMS(){
	return pre_EMS;
}

inline HAL_StatusTypeDef SolenCtrl::set_pre_EMS(HAL_StatusTypeDef ad){
	pre_EMS = ad;
	return HAL_OK;
}

inline HAL_StatusTypeDef SolenCtrl::EMS_down(){
	for(int i=0;i<7;i++){
		Valve[i].EMS();
	}
	pre_EMS = HAL_ERROR;
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	return HAL_OK;
}

inline void SolenCtrl::check_Safty_OK(){
	for (int i=0;i<7;i++){
		Valve[i].safty_OK();
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
	}
}

inline void SolenCtrl::check_Safty_ERROR(){
	for (int i=0;i<7;i++){
		Valve[i].safty_ERROR();
	}
}
