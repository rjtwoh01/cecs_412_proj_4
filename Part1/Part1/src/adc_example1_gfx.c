#include <asf.h>
#include <stdio.h>
#include <conf_example.h>

//! The latest, computed temperature.
static volatile int16_t last_temperature;

/**
* \brief Callback function for ADC interrupts
*
* \param adc Pointer to ADC module.
* \param ch_mask ADC channel mask.
* \param result Conversion result from ADC channel.
*/
static void adc_handler(ADC_t *adc, uint8_t ch_mask, adc_result_t result)
{
	#ifdef CONF_BOARD_OLED_UG_2832HSWEG04
	gfx_mono_draw_filled_rect(0,0,128,32,GFX_PIXEL_CLR);
	#endif
	int32_t temperature;
	char out_str[OUTPUT_STR_SIZE];
	float voltage;
	float last_voltage;
	
	/* Compute current temperature in Celsius, based on linearization
	* of the temperature sensor adc data.
	*/

	voltage = ((0.0157*result) + 0.0108); //calculated by the linear fit on excel
	last_voltage = (uint8_t)voltage > 2 ? 0 : voltage;
	//last_temperature = temperature;

	//Before the shift, the result was the voltage value
	//After the shift took place, we calculated the new voltage above.
	//But we still use result for temperature
	//So we're setting result = voltage again
	//result = voltage;
	
	if (result > 697) {
		temperature = (int8_t)((-0.0295 * result) + 40.5);
		} if (result > 420) {
		temperature = (int8_t)((-0.0474 * result) + 53.3);
		} else {
		temperature = (int8_t)((-0.0777 * result) + 65.1);
	}

	last_temperature = temperature;

	// Write temperature to display
	snprintf(out_str, OUTPUT_STR_SIZE, "Temperature: %4d C", last_temperature);
	gfx_mono_draw_string(out_str, 0, 0, &sysfont);
	// Write voltage to display
	snprintf(out_str, OUTPUT_STR_SIZE, "Voltage: %4d.%02d VDC", (uint8_t)last_voltage, (uint8_t)(last_voltage * 100)%100);
	gfx_mono_draw_string(out_str, 0, 8, &sysfont);
	
	// Start next conversion.
	adc_start_conversion(adc, ch_mask);
}

int main(void)
{
	struct adc_config         adc_conf;
	struct adc_channel_config adcch_conf;
	
	board_init();
	sysclk_init();
	sleepmgr_init();
	irq_initialize_vectors();
	cpu_irq_enable();
	gfx_mono_init();
	
	// Enable backlight if display type is not OLED
	#ifndef CONF_BOARD_OLED_UG_2832HSWEG04
	ioport_set_pin_high(LCD_BACKLIGHT_ENABLE_PIN);
	#endif
	
	// Initialize configuration structures.
	adc_read_configuration(&ADCB, &adc_conf);
	adcch_read_configuration(&ADCB, ADC_CH0, &adcch_conf);
	
	/* Configure the ADC module:
	* - unsigned, 12-bit results
	* - VCC voltage reference
	* - 200 kHz maximum clock rate
	* - manual conversion triggering
	* - temperature sensor enabled
	* - callback function
	*/
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_ON, ADC_RES_8,
	ADC_REF_VCC);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_enable_internal_input(&adc_conf, ADC_INT_TEMPSENSE);
	
	adc_write_configuration(&ADCB, &adc_conf);
	adc_set_callback(&ADCB, &adc_handler);
	
	/* Configure ADC channel 0:
	* - single-ended measurement from temperature sensor
	* - interrupt flag set on completed conversion
	* - interrupts disabled
	*/
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN1, ADCCH_NEG_NONE,
	1);
	adcch_set_interrupt_mode(&adcch_conf, ADCCH_MODE_COMPLETE);
	adcch_enable_interrupt(&adcch_conf);
	
	adcch_write_configuration(&ADCB, ADC_CH0, &adcch_conf);
	
	// Enable the ADC and start the first conversion.
	adc_enable(&ADCB);
	adc_start_conversion(&ADCB, ADC_CH0);
	
	do {
		// Sleep until ADC interrupt triggers.
		sleepmgr_enter_sleep();
	} while (1);
}