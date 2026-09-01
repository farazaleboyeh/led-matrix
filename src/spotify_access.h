#pragma once

#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <base64.h>
#include <TJpg_Decoder.h>

#include "secrets.h"

void refresh_access_token();
int skip_song(); 
int retrieve_artwork_url();
void download_artwork(const char* image_url);