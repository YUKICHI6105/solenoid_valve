/*
 *  wrapper.hpp for solenoid_valve
 *
 *  Created on: 2023/05/07
 *      Author: ykc
 */
#include <main.h>
#include <CanCtrl.hpp>
#include <stm32f1xx_hal_uart.h>
#include <SolenCtrl.hpp>

CanCtrl can;
SolenoidCtrl solenoid;

extern"C"{
	extern UART_HandleTypeDef huart2;
	void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
		uint32_t receiveID = 0x110;
		uint8_t receiveData[8];
		if (can.receive(receiveID,receiveData) == false)return;
		if (solenoid.getPreEMS() == mode::disable)return;
		if (solenoid.modeSet(receiveID,receiveData) == true)return;
		if (solenoid.update(receiveID,receiveData) == false)return;
	}

	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
		if (!(GPIO_Pin == EMS_Pin))return;
		if (solenoid.downEMS() == true){
			HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		}
	}
}

void main_cpp(){
	can.init();
	solenoid.init();
	solenoid.downEMS();

	for(uint8_t i=0;i<3;i++){
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
		HAL_Delay(100);
	}
	HAL_CAN_Start(&hcan);
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

	HAL_GPIO_WritePin(LED_GREEN_GPIO_Port,LED_GREEN_Pin,GPIO_PIN_SET);
//	unsigned char usart_data = 0;
//	uint16_t i = 0b1;
	while(true){//safty_roop
//		HAL_UART_Receive(&huart2, usart_data, i, 100);//(レジスタモジュール,データ,文字数,タイムアウト(つまりその時間経過したらエラー終了))
    	if(solenoid.getPreEMS() == mode::enable){
    		if((GPIOC->IDR & GPIO_IDR_IDR13)){//modeの状態違反の確認
    			solenoid.allValveCheck(solenoid.getPreEMS());
    		}
    		else{//予備の緊急停止
    			solenoid.downEMS();//コールバックだけだと一定の確率でバグる。意味不明。<-割り込み使用時に注意点があるらしい。
    		}
    	}
    	if (solenoid.getPreEMS() == mode::disable){
    		if(GPIOC->IDR & GPIO_IDR_IDR13){//EMS解除時
    			solenoid.allValveCheck(solenoid.getPreEMS());
    			solenoid.setPreEMS(mode::enable);
    			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
    		}
    		else{//EMS継続
    			solenoid.allValveCheck(solenoid.getPreEMS());
    			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    		}
    	}
    	can.ledCan();
    	uint16_t tick = HAL_GetTick()%2000;
    	if(tick < 100) HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    	else HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    }
}
