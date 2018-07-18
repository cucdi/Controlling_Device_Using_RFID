//Test of cheap 13.56 Mhz RFID-RC522 module from eBay
//This code is based on Martin Olejar's MFRC522 library. Minimal changes
//Adapted for Nucleo STM32 F401RE. Should work on other Nucleos too

//Connect as follows:
//RFID pins        ->  Nucleo header CN5 (Arduino-compatible header)
//----------------------------------------
//RFID IRQ=pin5    ->   Not used. Leave open
//RFID MISO=pin4   ->   Nucleo SPI_MISO=PA_6=D12
//RFID MOSI=pin3   ->   Nucleo SPI_MOSI=PA_7=D11
//RFID SCK=pin2    ->   Nucleo SPI_SCK =PA_5=D13
//RFID SDA=pin1    ->   Nucleo SPI_CS  =PB_6=D10
//RFID RST=pin7    ->   Nucleo         =PA_9=D8
//3.3V and Gnd to the respective pins

#include "mbed.h"
#include "MFRC522.h"
#include <sstream>
#include <string>

// CARD number
uint8_t CARD_1 = 0x10;
uint8_t CARD_2 = 0xA0;
uint8_t CARD_3 = 0x26;
uint8_t CARD_4 = 0x83;

uint8_t CARD_5 = 0x50;
uint8_t CARD_6 = 0x4C;
uint8_t CARD_7 = 0x33;
uint8_t CARD_8 = 0x83;

uint8_t CARD_9 = 0x10;
uint8_t CARD_10 = 0x70;
uint8_t CARD_11 = 0xFD;
uint8_t CARD_12 = 0x73;

#define SPI_MOSI    D11
#define SPI_MISO    D12
#define SPI_SCLK    D13
#define SPI_CS      D10

// WIZWiki-W7500 Pin for MFRC522 reset(pick another D pin if you need D8)
#define MF_RESET    D9
#define VOLUME 0.08


Serial pc(USBTX, USBRX);

MFRC522    RfChip (SPI_MOSI, SPI_MISO, SPI_SCLK, SPI_CS, MF_RESET);        // Init MFRC522 card

DigitalOut LedGreen(D7);
DigitalOut LedYellow(D6);
PwmOut Buzzer(D5);
float beat_duration;

// Plays a sound with the defined frequency, duration, and volume
void playNote(float frequency, float volume)
{
    Buzzer.period(1.0/(double)frequency);
    Buzzer = ((double)volume/2.0);
}
char* store_buf;
char data2[8];

int main(void)
{
    printf("Welcome to My Smart Home.... ");
    wait_ms(50);
    printf("\n\r");
    printf("Please swipe card.. Here ");
    wait_ms(50);
    printf("\n\r");
    // Init. RC522 Chip
    RfChip.PCD_Init();

    while (true) {
        LedGreen = 1;
        // Look for new cards
        if ( ! RfChip.PICC_IsNewCardPresent()) {
            wait_ms(50);
            continue;
        }
        LedGreen = 0;
        wait_ms(50);
        // Select one of the cards
        if ( ! RfChip.PICC_ReadCardSerial()) {
            wait_ms(50);
            continue;
        }
        LedGreen = 1;
        wait_ms(50);

        // Print Card UID
        for (uint8_t i = 0; i < RfChip.uid.size; i++) {
            store_buf+= sprintf(store_buf,"%X",RfChip.uid.uidByte[i]);
        }
        printf("\n\r");
        // Print Card type
        uint8_t piccType = RfChip.PICC_GetType(RfChip.uid.sak);
        LedGreen = 0;
        wait_ms(50);

        // store_buf=data2;
        if((RfChip.uid.uidByte[0] == CARD_1) && (RfChip.uid.uidByte[1] == CARD_2) && (RfChip.uid.uidByte[2] == CARD_3) && (RfChip.uid.uidByte[3] == CARD_4)) {
            LedYellow=1;
            //  wait_ms(1000);
            pc.printf("Yello Led is On.....");
            printf("\n\r");

        } else if((RfChip.uid.uidByte[0] == CARD_5) && (RfChip.uid.uidByte[1] == CARD_6) && (RfChip.uid.uidByte[2] == CARD_7) && (RfChip.uid.uidByte[3] == CARD_8)) {
            playNote(999.999, VOLUME);
            pc.printf("Buzzer is On.....");
            printf("\n\r");

        }  else if((RfChip.uid.uidByte[0] == CARD_9) && (RfChip.uid.uidByte[1] == CARD_10) && (RfChip.uid.uidByte[2] == CARD_11) && (RfChip.uid.uidByte[3] == CARD_12)) {
            pc.printf("This is a master Card: ");
            printf("\n\r");
            LedYellow=0;
            Buzzer=0;
            pc.printf("ALL Device off.....");
            printf("\n\r");
            pc.printf("Thank you.....");
        }
        printf("\n\r");



    }
}