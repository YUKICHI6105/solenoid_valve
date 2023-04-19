/*
 *  solenCtrl.hpp
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

HAL_StatusTypeDef solenoid::value_update(uint8_t Rdata){
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

void solenoid::safty_O(){
	if(mode == HAL_ERROR){
//		if(Pin == 1){
		HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_RESET);
//		}
	}
}

void solenoid::safty(){
//		if(Pin == 0){}
//		else{
		    HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_RESET);
//		}
//		if(mode == HAL_ERROR){}
//		else{
//		    mode = HAL_ERROR;
//		}
}

void solenoid::EMS(){
	HAL_GPIO_WritePin(GPIO,V_Pin,GPIO_PIN_RESET);
	if(mode == HAL_OK){
	   mode = HAL_ERROR;
	}
}

class solenCtrl{
private:
	HAL_StatusTypeDef administrater = HAL_OK;//EMSの管理
	solenoid V0;
	solenoid V1;
	solenoid V2;
	solenoid V3;
	solenoid V4;
	solenoid V5;
	solenoid V6;
public:
	void init();
	HAL_StatusTypeDef update(uint32_t RID,uint8_t RData[8]);
	HAL_StatusTypeDef get_ad();
	HAL_StatusTypeDef set_ad(HAL_StatusTypeDef ad);
	HAL_StatusTypeDef EMS_down();
	void check_safty_O();
	void check_safty_E();
};

void solenCtrl::init(){
	V0.BID = 0x101;
	V1.BID = 0x102;
	V2.BID = 0x103;
	V3.BID = 0x104;
	V4.BID = 0x105;
	V5.BID = 0x106;
	V6.BID = 0x107;
	V0.V_Pin = V0_Pin;
	V1.V_Pin = V1_Pin;
	V2.V_Pin = V2_Pin;
	V3.V_Pin = V3_Pin;
	V4.V_Pin = V4_Pin;
	V5.V_Pin = V5_Pin;
	V6.V_Pin = V6_Pin;
	V6.GPIO = GPIOC;
}

HAL_StatusTypeDef solenCtrl::update(uint32_t RID,uint8_t RData[8]){
	uint8_t data = RData[1];
	if (RID == V0.BID){
		if(V0.value_update(data)){
			return HAL_OK;
		}
		else{
			return HAL_ERROR;
		}
	}
	if (RID == V1.BID){
		if(V1.value_update(data)){
				return HAL_OK;
			}
			else{
				return HAL_ERROR;
			}
	}
	if (RID == V2.BID){
		if(V2.value_update(data)){
			return HAL_OK;
		}
		else{
			return HAL_ERROR;
		}
	}
	if (RID == V3.BID){
		if(V3.value_update(data)){
			return HAL_OK;
		}
		else{
			return HAL_ERROR;
		}
	}
	if (RID == V4.BID){
		if(V0.value_update(data)){
			return HAL_OK;
		}
		else{
			return HAL_ERROR;
		}
	}
	if (RID == V5.BID){
		if(V0.value_update(data)){
			return HAL_OK;
		}
		else{
			return HAL_ERROR;
		}
	}
	if (RID == V6.BID){
		if(V0.value_update(data)){
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

HAL_StatusTypeDef solenCtrl::get_ad(){
	return administrater;
}

HAL_StatusTypeDef solenCtrl::set_ad(HAL_StatusTypeDef ad){
	administrater = ad;
	return HAL_OK;
}

HAL_StatusTypeDef solenCtrl::EMS_down(){
	V0.EMS();
	V1.EMS();
	V2.EMS();
	V3.EMS();
	V4.EMS();
	V5.EMS();
	V6.EMS();
	administrater = HAL_ERROR;
	return HAL_OK;
}

void solenCtrl::check_safty_O(){
	V0.safty_O();
	V1.safty_O();
	V2.safty_O();
	V3.safty_O();
	V4.safty_O();
	V5.safty_O();
	V6.safty_O();
}

void solenCtrl::check_safty_E(){
	V0.safty();
	V1.safty();
	V2.safty();
	V3.safty();
	V4.safty();
	V5.safty();
	V6.safty();
}
