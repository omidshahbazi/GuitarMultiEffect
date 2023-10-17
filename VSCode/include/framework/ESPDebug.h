#pragma once
#ifndef ESP_DEBUG_H
#define ESP_DEBUG_H

#include "Debug.h"

const char *TAG = "ES8388_DRIVER";

#define ESP_CHECK_CALL(Expression) ASSERT((Expression) == ESP_OK, "Call failed %s", #Expression);

#endif