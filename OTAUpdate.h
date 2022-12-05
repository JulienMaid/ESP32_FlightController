/*
 * OTAUpdate.h
 *
 *  Created on: 21 août 2022
 *      Author: julien
 */

#include <Arduino.h>

#pragma once

void DemarrerServeurOTA(const String &pm_Texte);

void RedemarrerServeurOTA(void);

void StopperServeurOTA(void);
