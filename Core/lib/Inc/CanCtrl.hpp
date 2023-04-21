/*
 *  CanCtrl.hpp
 *
 *  Created on: 2023/04/13
 *      Author: ykc
 */

#include <main.h>
#include <stm32f1xx_hal_can.h>
#include <stm32f1xx_hal_gpio.h>

extern "C"{
	extern CAN_HandleTypeDef hcan;
}

class CanCtrl {
private:
	uint32_t test;
	uint8_t d;
	CAN_RxHeaderTypeDef RxHeader;//受信用フレーム設定
	CAN_FilterTypeDef filter;//受信時に中身を仕分けるためのパラメーター設定
public:
    void init();
	HAL_StatusTypeDef Canrx(uint32_t& RID,uint8_t data[8]);//受信関数(エラー判定のみ)内容は引数に入れ込む。
};

void CanCtrl::init(){
	filter.FilterIdHigh         = 0x100 << 5;                 // フィルターIDの上位16ビット
	filter.FilterIdLow          = 0;                        // フィルターIDの下位16ビット
	filter.FilterMaskIdHigh     = 0x7f8 << 5;               // フィルターマスクの上位16ビット
	filter.FilterMaskIdLow      = 0b110;                    // フィルターマスクの下位16ビット
	//フィルターマスクは1が確定させたいところで0が無視したいところ
	filter.FilterScale          = CAN_FILTERSCALE_32BIT;    // フィルタースケール
	filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;         // フィルターに割り当てるFIFO
	filter.FilterBank           = 0;                        // フィルターバンクNo
	filter.FilterMode           = CAN_FILTERMODE_IDMASK;    // フィルターモード
	filter.SlaveStartFilterBank = 14;                       // スレーブCANの開始フィルターバンクNo
	filter.FilterActivation     = ENABLE;                   // フィルター無効／有効
	HAL_CAN_ConfigFilter(&hcan, &filter);
}

HAL_StatusTypeDef CanCtrl::Canrx(uint32_t& RID,uint8_t data[8]){
	if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &RxHeader, data) == HAL_OK){
		RID = RxHeader.StdId;
		test = RID;
		d=data[0];
		HAL_GPIO_WritePin(GPIOB,CAN_LED_Pin,GPIO_PIN_SET);
//		HAL_Delay(80);
		HAL_GPIO_WritePin(GPIOB,CAN_LED_Pin,GPIO_PIN_RESET);
		return HAL_OK;
	}
	else{
		return HAL_ERROR;
	}
}

