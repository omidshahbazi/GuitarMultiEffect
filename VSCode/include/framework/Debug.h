#pragma once
#ifndef DEBUG_H
#define DEBUG_H

#include <esp_log.h>

#define LOG_INFO(Tag, Message, ...)                                 \
	ESP_LOG_LEVEL_LOCAL(ESP_LOG_INFO, Tag, Message, ##__VA_ARGS__); \
	ESP_LOG_LEVEL_LOCAL(ESP_LOG_INFO, Tag, "\n");

#define LOG_WARNING(Tag, Message, ...)                              \
	ESP_LOG_LEVEL_LOCAL(ESP_LOG_WARN, Tag, Message, ##__VA_ARGS__); \
	ESP_LOG_LEVEL_LOCAL(ESP_LOG_WARN, Tag, "\n");

#define LOG_ERROR(Tag, Message, ...)                                 \
	ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, Tag, Message, ##__VA_ARGS__); \
	ESP_LOG_LEVEL_LOCAL(ESP_LOG_ERROR, Tag, "\n");

#define ASSERT(Expression, Message, ...) \
	if (!(Expression))                   \
	{                                    \
		LOG_ERROR(Message, __VA_ARGS__); \
		esp_restart();                   \
	}

#define CHECK_CALL(Expression) ASSERT(Expression, "Call failed %s", #Expression);

#endif