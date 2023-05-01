/*
 *  wrapper.hpp for solenoid_valve
 *
 *  Created on: 2023/04/13
 *      Author: ykc
 */
#include <main.h>
#include <CanCtrl.hpp>
#include <SolenCtrl.hpp>
#include <stm32f1xx_hal_uart.h>

CanCtrl Can;
SolenCtrl Solen;

extern"C"{
	extern UART_HandleTypeDef huart2;
	void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
		uint32_t RID = 0x110;
		uint8_t RData[8];
		if (Can.Canrx(RID,RData) == HAL_OK)return;
		if(Solen.get_pre_EMS()== mode::enable)return;
		if (Solen.update(RID,RData) == HAL_OK)return;
	}

	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
		if(GPIO_Pin==EMS_Pin)return;
		if(Solen.EMS_down()==HAL_OK){
			HAL_GPIO_WritePin(LED_RED_GPIO_Port,LED_RED_Pin,GPIO_PIN_SET);
		}
		else{
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);;
		}
	}
}

void main_cpp(){
	Can.init();
	Solen.init();
	Solen.EMS_down();
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
    	if(Solen.get_pre_EMS() == mode::enable){
    		if((GPIOC->IDR & GPIO_IDR_IDR13)){//modeの状態違反の確認
    			Solen.check_Safty_OK();
    		}
    		else{//予備の緊急停止
    			Solen.EMS_down();//コールバックだけだと一定の確率でバグる。意味不明。<-割り込み使用時に注意点があるらしい。
    		}

    	}
    	if(Solen.get_pre_EMS() == mode::disable){
    		if((GPIOC->IDR & GPIO_IDR_IDR13)){//EMS解除時
    			Solen.check_Safty_ERROR();
    			Solen.set_pre_EMS(mode::enable);
    			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);
    		}
    		else{//EMS継続
    			Solen.check_Safty_ERROR();
    			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
    		}
    	}
    	uint16_t tick = HAL_GetTick()%2000;
    	if(tick < 100) HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
    	else HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
    }
}
