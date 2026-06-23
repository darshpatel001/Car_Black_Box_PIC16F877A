/* 
 * File:   EEprom.h
 * Author: darsh
 *
 * Created on May 28, 2026, 12:36 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H


#define SLAVE_READ_EEPROM      0xA1
#define SLAVE_WRITE_EEPROM     0xA0


unsigned char ext_eeprom_24C02_read(unsigned char addr);
void ext_eeprom_24C02_byte_write(unsigned char addr, char data);
void ext_eeprom_24C02_str_write(unsigned char addr, char *data);


#endif	/* EEPROM_H */

