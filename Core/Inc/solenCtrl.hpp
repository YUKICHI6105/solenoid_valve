/*
 *  SolenCtrl.hpp
 *
 *  Created on: 2023/04/13
 *      Author: ykc
 */

enum class mode {
	disable,
	enable,
};

struct solenoid{
	uint32_t BID;
	uint16_t valve_Pin;            //GPIOPin
	GPIO_TypeDef* GPIO = GPIOB;    //GPIOグループ
	uint32_t msk;                  //pinの状態の確認
	mode valve_Mode = mode::enable;//動作許可(今回はデフォルトで許可)
	HAL_StatusTypeDef value_update(uint8_t Rdata);
	void safty_OK();
	void safty_ERROR();
	void EMS_stop();
};

inline HAL_StatusTypeDef solenoid::value_update(uint8_t Rdata){
	if (valve_Mode == mode::enable){
		if (Rdata == 0x1){
			HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_SET);
			if(!(GPIO==GPIOA)){
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
			}
			return HAL_OK;
		}
		if (Rdata == 0x0){
			HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
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
	if(valve_Mode == mode::disable){
//		if(GPIO->ODR & msk){
		HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
//		}
	}
}

inline void solenoid::safty_ERROR(){
//	    if(!(GPIO->ODR & msk)){}
//		else{
		    HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
//		}
		if(valve_Mode == mode::disable){}
		else{
//		    valve_Mode = disable;
		}
}

inline void solenoid::EMS_stop(){
	HAL_GPIO_WritePin(GPIO,Valve_Pin,GPIO_PIN_RESET);
	if(valve_Mode == mode::enable){
//	   valve_Mode = disable;
	}
}


class SolenCtrl{
private:
	mode pre_EMS = mode::disable;//EMSの管理
	solenoid Valve[7];
public:
	void init();
	HAL_StatusTypeDef update(uint32_t RID,uint8_t rxData[8]);
	mode getPreEMS();
	HAL_StatusTypeDef set_pre_EMS(mode ems);
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
	Valve[6].GPIO = Valve6_GPIO_Port;
	Valve[0].msk=GPIO_ODR_ODR10;
	Valve[1].msk=GPIO_ODR_ODR11;
	Valve[2].msk=GPIO_ODR_ODR12;
	Valve[3].msk=GPIO_ODR_ODR13;
	Valve[4].msk=GPIO_ODR_ODR14;
	Valve[5].msk=GPIO_ODR_ODR15;
	Valve[6].msk=GPIO_ODR_ODR8;
}

inline HAL_StatusTypeDef SolenCtrl::update(uint32_t RID,uint8_t rxData[8]){
	uint8_t data = rxData[0];
	for(int i=0;i<7;i++){
		if(RID == Valve[i].BID){
			if(Valve[i].value_update(data)==HAL_OK){
				return HAL_OK;
			}
		}
	}
	return HAL_ERROR;
}

inline mode SolenCtrl::getPreEMS(){
	return pre_EMS;
}

inline HAL_StatusTypeDef SolenCtrl::set_pre_EMS(mode ems){
	pre_EMS = ems;
	return HAL_OK;
}

inline HAL_StatusTypeDef SolenCtrl::EMS_down(){
	for(int i=0;i<7;i++){
		Valve[i].EMS_stop();
	}
	pre_EMS = mode::disable;
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
	return HAL_OK;
}

inline void SolenCtrl::check_Safty_OK(){
	for (int i=0;i<7;i++){
		Valve[i].safty_OK();
	}
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
}

inline void SolenCtrl::check_Safty_ERROR(){
	for (int i=0;i<7;i++){
		Valve[i].safty_ERROR();
	}
}
