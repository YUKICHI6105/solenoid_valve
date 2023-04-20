/*
 *  SolenCtrl.hpp
 *
 *  Created on: 2023/04/13
 *      Author: ykc
 */

struct solenoid{
	uint32_t BID;
	uint16_t V_Pin;                 //GPIOPin
	GPIO_TypeDef* GPIO = GPIOB;     //GPIOグループ
	HAL_StatusTypeDef mode = HAL_OK;//動作許可(今回はデフォルトで許可)
	HAL_StatusTypeDef value_update(uint8_t Rdata);
	void safty_O();
	void safty();
	void EMS();
};

inline HAL_StatusTypeDef solenoid::value_update(uint8_t Rdata){
	if (mode == HAL_OK){
		if (Rdata == 0x01){
			HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_SET);
			return HAL_OK;
		}
		if (Rdata == 0x00){
			HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_RESET);
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

inline void solenoid::safty_O(){
	if(mode == HAL_ERROR){
//		if(Pin == 1){
		HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_RESET);
//		}
	}
}

inline void solenoid::safty(){
//		if(Pin == 0){}
//		else{
		    HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_RESET);
//		}
//		if(mode == HAL_ERROR){}
//		else{
//		    mode = HAL_ERROR;
//		}
}

inline void solenoid::EMS(){
	HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_RESET);
	if(mode == HAL_OK){
	   mode = HAL_ERROR;
	}
}

class SolenCtrl{
private:
	HAL_StatusTypeDef pre_EMS = HAL_OK;//EMSの管理
	solenoid Valve[7];
public:
	void init();
	HAL_StatusTypeDef update(uint32_t RID,uint8_t rxData[8]);
	HAL_StatusTypeDef get_pre_EMS();
	HAL_StatusTypeDef set_pre_EMS(HAL_StatusTypeDef ad);
	HAL_StatusTypeDef EMS_down();
	void check_safty_O();
	void check_safty_E();
};

inline void SolenCtrl::init(){
	for (uint32_t i=0;i<0x7;i++){
		Valve[i].BID = (0x101+i);
	}
	Valve[0].V_Pin = V0_Pin;
	Valve[1].V_Pin = V1_Pin;
	Valve[2].V_Pin = V2_Pin;
	Valve[3].V_Pin = V3_Pin;
	Valve[4].V_Pin = V4_Pin;
	Valve[5].V_Pin = V5_Pin;
	Valve[6].V_Pin = V6_Pin;
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
	return HAL_OK;
}

inline void SolenCtrl::check_safty_O(){
	for (int i=0;i<7;i++){
		Valve[i].safty_O();
	}
}

inline void SolenCtrl::check_safty_E(){
	for (int i=0;i<7;i++){
		Valve[i].safty_O();
	}
}
