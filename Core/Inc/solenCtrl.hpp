/*
 *  SolenCtrl.hpp
 *
 *  Created on: 2023/05/07
 *      Author: ykc
 */

enum class mode {
	disable,
	enable,
};

struct Solenoid{
	uint32_t canID = 0x10e;
	uint16_t valvePin = 0;         //GPIOPin
	GPIO_TypeDef* gpio = GPIOB;    //GPIOグループ
	uint32_t msk;                  //pinの状態の確認
	mode valveMode = mode::disable; //動作許可(今回はデフォルトで許可)
	bool valueUpdate(uint8_t receiveData);
};

inline bool Solenoid::valueUpdate(uint8_t receiveData){
	if (valveMode == mode::enable){
		if (receiveData == 0x1){
			HAL_GPIO_WritePin(gpio,valvePin,GPIO_PIN_SET);
			if(gpio->ODR & msk){
			HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
			}
			return true;
		}
		if (receiveData == 0x0){
			if(gpio->ODR & msk){
			HAL_GPIO_WritePin(gpio,valvePin,GPIO_PIN_RESET);
			}
			return true;
		}
		else{
			return false;
		}
	}
	else{
		return false;
	}
}

class SolenoidCtrl{
private:
	mode preEMS = mode::disable;//EMSの管理
	Solenoid valve[7];
public:
	void init();
	bool modeSet(uint32_t receiveID,uint8_t receiveData[8]);
	bool update(uint32_t RID,uint8_t rxData[8]);
	mode getPreEMS();
	bool setPreEMS(mode ems);
	bool downEMS();
	void allValveCheck(mode getPreEMS);
};

inline void SolenoidCtrl::init(){
	for (uint32_t i=0;i<0x7;i++){
		valve[i].canID = (0x101+i);
	}
	valve[0].valvePin = Valve0_Pin;
	valve[1].valvePin = Valve1_Pin;
	valve[2].valvePin = Valve2_Pin;
	valve[3].valvePin = Valve3_Pin;
	valve[4].valvePin = Valve4_Pin;
	valve[5].valvePin = Valve5_Pin;
	valve[6].valvePin = Valve6_Pin;
	valve[6].gpio = Valve6_GPIO_Port;
	valve[0].msk=GPIO_ODR_ODR10;
	valve[1].msk=GPIO_ODR_ODR11;
	valve[2].msk=GPIO_ODR_ODR12;
	valve[3].msk=GPIO_ODR_ODR13;
	valve[4].msk=GPIO_ODR_ODR14;
	valve[5].msk=GPIO_ODR_ODR15;
	valve[6].msk=GPIO_ODR_ODR8;
}

inline bool SolenoidCtrl::modeSet(uint32_t receiveID,uint8_t receiveData[8]){
	if(receiveID == 0x100){
		for(int i=0;i<7;i++){
			if(receiveData[i] == 0x1){
				valve[i].valveMode = mode::enable;
			}
			if(receiveData[i] == 0x0){
				valve[i].valveMode = mode::disable;
			}

		}
		return true;
	}
	else{
		return false;
	}
}

inline bool SolenoidCtrl::update(uint32_t RID,uint8_t rxData[8]){
	uint8_t data = rxData[0];
	for(int i=0;i<7;i++){
		if(RID == valve[i].canID){
			if(valve[i].valueUpdate(data) == true){
				return true;
			}
		}
	}
	return false;
}

inline mode SolenoidCtrl::getPreEMS(){
	return preEMS;
}

inline bool SolenoidCtrl::setPreEMS(mode ems){
	preEMS = ems;
	return true;
}

inline bool SolenoidCtrl::downEMS(){
	for(int i=0;i<7;i++){
		HAL_GPIO_WritePin(valve[i].gpio,valve[i].valvePin,GPIO_PIN_RESET);
			if(valve[i].valveMode == mode::enable){
			   valve[i].valveMode = mode::disable;
			}
	}
	preEMS = mode::disable;
	HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
	return true;
}

inline void SolenoidCtrl::allValveCheck(mode getPreEMS){
	for (int i=0;i<7;i++){
		if(getPreEMS == mode::enable){
			if(valve[i].valveMode == mode::disable){
				if(valve[i].gpio->ODR & valve[i].msk){
					HAL_GPIO_WritePin(valve[i].gpio,valve[i].valvePin,GPIO_PIN_RESET);
				}
			}
		}
		if(getPreEMS == mode::disable){
			if(!(valve[i].gpio->ODR & valve[i].msk)){
				HAL_GPIO_WritePin(valve[i].gpio,valve[i].valvePin,GPIO_PIN_RESET);
			}
			if(valve[i].valveMode == mode::disable){}
			else{
			valve[i].valveMode = mode::disable;
			}
		}
	}
}
