#include "master.h"

const char* clientId = SPOTIFY_CLIENT_ID;
const char* clientSecret = SPOTIFY_CLIENT_SECRET;
const char* refreshToken = SPOTIFY_REFRESH_TOKEN;

String accessToken = "";
HTTPClient http;

void refresh_access_token(){
    http.begin("https://accounts.spotify.com/api/token");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String auth = String(clientId) + ":" + String(clientSecret);
    String authEncoded = base64::encode(auth);
    http.addHeader("Authorization", "Basic " + authEncoded);

    String httpRequestData = "grant_type=refresh_token&refresh_token=" + String(refreshToken);
    
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode == 200) {
        String payload = http.getString();

        StaticJsonDocument<1024> doc;

        deserializeJson(doc, payload);
        
        accessToken = doc["access_token"].as<String>();
        Serial.println("New Access Token: " + accessToken);
    } else {
        Serial.printf("Error refreshing token: %d\n", httpResponseCode);
        Serial.println(http.getString());
    }
    http.end();
}

int skip_song(){
    http.begin("https://api.spotify.com/v1/me/player/next");
    http.addHeader("Authorization", "Bearer "+accessToken+"");
    http.addHeader("Content-Length", "0");
    int httpCode = http.POST("");
    Serial.print("HTTP Response Code: ");
    Serial.println(httpCode); // expect 204
    http.end();
}

