/*
 * Copyright 2017-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Clocks v4.0
* BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/**
 * @file    clock_config.c
 * @brief   Board clocks initialization file.
 */
 
/* This is a template for board specific configuration created by MCUXpresso IDE Project Wizard.*/

/*****************************************************************************
* Copyright (C) 2019 by Jon Warriner
*
* Redistribution, modification or use of this software in source or binary
* forms is permitted as long as the files maintain this copyright. Users are
* permitted to modify this and use it to learn about the field of embedded
* software. Jon Warriner and the University of Colorado are not liable for
* any misuse of this material.
*
*****************************************************************************/
/**
* @file clock_config.c
* @brief Configure CPU clocks
*
* This source file was provided as a blank template from NXP.  All code was filled
* in by me.
*
* @author Jon Warriner
* @date March 28, 2019
* @version 1.0
*
*/

#include "MKL25Z4.h"
#include "clock_config.h"

/**
 * @brief Set up and initialize all required blocks and functions related to the board hardware.
 */
void BOARD_InitBootClocks(void) {
	/* The user initialization should be placed here */

	//OUTDIV1 = 0000 - Divide-by-1
	//OUTDIV4 = 001 - Divide-by-2
	SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV1(0) | SIM_CLKDIV1_OUTDIV4(1);

	//CLKS = 00 - Output of FLL or PLL is selected as source of MCGOUTCLK (depends on PLLS control bit).
	//FRDIV = 000 - FLL reference clock divided by 1
	//IREFS = 1 - select slow internal clock as source for FLL
	//IRCLKEN = 1 - enable internal reference clock (IRCSCLK) as MCGIRCLK
	//IREFSTEN = 0 - Internal reference clock is disabled in Stop mode
	MCG->C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;

	//LOCRE0 = 1 - Generate a reset request on a loss of OSC0 external reference clock.
	//RANGE0 = 00 - Low frequency range selected for the crystal oscillator.  (doesn't really matter, not using)
	//HGO0 = 0 - Configure crystal oscillator for low-power operation.
	//EREFS0 = 0 - External reference clock requested.
	//LP = 0 - FLL or PLL is not disabled in bypass modes.
	//IRCS = 1 - Fast internal reference clock selected as IRCSCLK.
	MCG->C2 = MCG_C2_LOCRE0_MASK | MCG_C2_IRCS_MASK;

	//DMX32 = 1 - reference frequency is 32.768 kHz
	//DRST_DRS = 01 - Encoding 1 — Mid range.
	//The above bits configure the FLL to multiply a 32.768kHz clock by 1464 for a 47,972,352 Hz output.
	//FCTRIM = XXXX - A value for FCTRIM is loaded during reset from a factory programmed location.
	//SCFTRIM = X - A value for SCFTRIM is loaded during reset from a factory programmed location.
	MCG->C4 |= (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(1));

	/* Read core clock setting. */
	SystemCoreClockUpdate();
}
