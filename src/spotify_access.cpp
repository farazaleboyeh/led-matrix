#include "master.h"

extern MatrixDriver display;

const char *clientId = SPOTIFY_CLIENT_ID;
const char *clientSecret = SPOTIFY_CLIENT_SECRET;
const char *refreshToken = SPOTIFY_REFRESH_TOKEN;

String accessToken = "";


void refresh_access_token()
{
    HTTPClient http;
    
    WiFiClientSecure client;
    client.setInsecure();

    http.begin("https://accounts.spotify.com/api/token");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String auth = String(clientId) + ":" + String(clientSecret);
    String authEncoded = base64::encode(auth);
    http.addHeader("Authorization", "Basic " + authEncoded);

    String httpRequestData = "grant_type=refresh_token&refresh_token=" + String(refreshToken);

    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode == 200)
    {
        String payload = http.getString();

        StaticJsonDocument<1024> doc;

        deserializeJson(doc, payload);

        accessToken = doc["access_token"].as<String>();
        Serial.println("New Access Token: " + accessToken);
    }
    else
    {
        Serial.printf("Error refreshing token: %d\n", httpResponseCode);
        Serial.println(http.getString());
    }
    http.end();
}

int skip_song()
{
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    int httpCode = -1;

    http.begin("https://api.spotify.com/v1/me/player/next");
    http.addHeader("Authorization", "Bearer " + accessToken + "");
    http.addHeader("Content-Length", "0");
    httpCode = http.POST("");
    Serial.print("Skip HTTP Response Code: ");
    Serial.println(httpCode); 
    http.end();
    return httpCode;
}

int retrieve_artwork_url()
{
    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    int httpCode = -1;

    http.begin("https://api.spotify.com/v1/me/player/currently-playing");
    http.addHeader("Authorization", "Bearer " + accessToken + "");

    httpCode = http.GET();
    Serial.print("Artwork HTTP Response Code: ");
    Serial.println(httpCode); 

    if (httpCode == 200)
    {
        String payload = http.getString();
        DynamicJsonDocument doc(4096);
        deserializeJson(doc, payload);
        
        Serial.println(doc["is_playing"].as<bool>());
        Serial.println(doc["item"]["album"]["images"].as<bool>());

        if (doc["is_playing"] && doc["item"]["album"]["images"])
        {
            
            const char *image_url = doc["item"]["album"]["images"][2]["url"];
            Serial.print("hey");
            Serial.println(image_url);
            download_artwork(image_url);
        }
    }
    http.end();
    return httpCode;
}

void download_artwork(const char *image_url)
{
    if (!image_url) return;

    WiFiClientSecure client;
    client.setInsecure();

    HTTPClient http;
    if (http.begin(client, image_url))
    {
        int httpCode = http.GET();
        if (httpCode == HTTP_CODE_OK)
        {
            int total_size = http.getSize();
            WiFiClient *stream = http.getStreamPtr();
            uint8_t* jpg_buffer = (uint8_t *)malloc(total_size);
            if (jpg_buffer)
            {
                int bytes_read = 0;
                while (http.connected() && (bytes_read < total_size)){
                    size_t available = stream->available();
                    if (available)
                    {
                        bytes_read += stream->readBytes(jpg_buffer + bytes_read, available);
                    }
                }
                delay(1);
            }
            display.clear();
            TJpgDec.drawJpg(0, 0, jpg_buffer, total_size);
            display.swap();
            free(jpg_buffer);
        }
    }
}
