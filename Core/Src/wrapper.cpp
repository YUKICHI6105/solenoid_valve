/*
 *  wrapper.hpp for solenoid_valve
 *
 *  Created on: 2023/04/13
 *      Author: ykc
 */
#include <main.h>
#include <CanCtrl.hpp>
#include <solenCtrl.hpp>

CanCtrl Can;
SolenCtrl Solen;

void HAL_CAN_RxFifo0MsgPendingCallback(HAL_StatusTypeDef &hcan){
	uint32_t RID = 0x110;
	uint8_t RData[8];
	if (Can.Canrx(RID,RData) == HAL_OK){
		if(Solen.get_pre_EMS()==HAL_OK){
			if (Solen.update(RID,RData) == HAL_OK){

			}
		}
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(Solen.EMS_down()==HAL_OK){
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
	}
	else{
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);;
	}
}

void main_cpp(){
	Can.init();
	Solen.init();

	for(uint8_t i=0;i<3;i++){
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
	}
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	while(true){//safty_roop
    	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_RESET);
    	if(Solen.get_pre_EMS() == HAL_OK){
    		if((GPIOC->IDR & !(GPIO_IDR_IDR13))){
    		  	Solen.EMS_down();
    		}
    		if((GPIOC->IDR & GPIO_IDR_IDR13)){
    			Solen.check_safty_O();
    		}

    	}
    	if(Solen.get_pre_EMS() == HAL_ERROR){
    		if((GPIOC->IDR & !(GPIO_IDR_IDR13))){
    			Solen.check_safty_E();
    		}
    		if((GPIOC->IDR & GPIO_IDR_IDR13)){
    			Solen.check_safty_E();
    			Solen.set_pre_EMS(HAL_OK);
    		}
    	}
    	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
    }
}
