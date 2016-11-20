#include "Debug.h"


// Set up output serial port(could be a SoftwareSerial
// if really wanted).

Stream& ehConsolePort(Serial);

// Wired to the blue LED on an ESP-01 board, active LOW.
//
// Cannot be used simultaneously with Serial.print/println()
// calls, as TX is wired to the same pin.
//
// UNLESS: You swap the TX pin using the alternate pinout.
const uint8_t LED_PIN = 1;

const char * const RST_REASONS[] =
{
	"REASON_DEFAULT_RST",
	"REASON_WDT_RST",
	"REASON_EXCEPTION_RST",
	"REASON_SOFT_WDT_RST",
	"REASON_SOFT_RESTART",
	"REASON_DEEP_SLEEP_AWAKE",
	"REASON_EXT_SYS_RST"
};

const char * const FLASH_SIZE_MAP_NAMES[] =
{
	"FLASH_SIZE_4M_MAP_256_256",
	"FLASH_SIZE_2M",
	"FLASH_SIZE_8M_MAP_512_512",
	"FLASH_SIZE_16M_MAP_512_512",
	"FLASH_SIZE_32M_MAP_512_512",
	"FLASH_SIZE_16M_MAP_1024_1024",
	"FLASH_SIZE_32M_MAP_1024_1024"
};

const char * const OP_MODE_NAMES[]
{
	"NULL_MODE",
	"STATION_MODE",
	"SOFTAP_MODE",
	"STATIONAP_MODE"
};

const char * const AUTH_MODE_NAMES[]
{
	"AUTH_OPEN",
	"AUTH_WEP",
	"AUTH_WPA_PSK",
	"AUTH_WPA2_PSK",
	"AUTH_WPA_WPA2_PSK",
	"AUTH_MAX"
};

const char * const PHY_MODE_NAMES[]
{
	"",
	"PHY_MODE_11B",
	"PHY_MODE_11G",
	"PHY_MODE_11N"
};

//old walues from example
//const char * const EVENT_NAMES[]
//{
//    "EVENT_STAMODE_CONNECTED",
//    "EVENT_STAMODE_DISCONNECTED",
//    "EVENT_STAMODE_AUTHMODE_CHANGE",
//    "EVENT_STAMODE_GOT_IP",
//    "EVENT_SOFTAPMODE_STACONNECTED",
//    "EVENT_SOFTAPMODE_STADISCONNECTED",
//    "EVENT_MAX"
//};

const char * const EVENT_NAMES[]
{
	"EVENT_STAMODE_CONNECTED",
	"EVENT_STAMODE_DISCONNECTED",
	"EVENT_STAMODE_AUTHMODE_CHANGE",
	"EVENT_STAMODE_GOT_IP",
	"EVENT_STAMODE_DHCP_TIMEOUT",
	"EVENT_SOFTAPMODE_STACONNECTED",
	"EVENT_SOFTAPMODE_STADISCONNECTED",
	"EVENT_SOFTAPMODE_PROBEREQRECVED",
	"EVENT_MAX"
};


const char * const EVENT_REASONS[]
{
	"",
	"REASON_UNSPECIFIED",
	"REASON_AUTH_EXPIRE",
	"REASON_AUTH_LEAVE",
	"REASON_ASSOC_EXPIRE",
	"REASON_ASSOC_TOOMANY",
	"REASON_NOT_AUTHED",
	"REASON_NOT_ASSOCED",
	"REASON_ASSOC_LEAVE",
	"REASON_ASSOC_NOT_AUTHED",
	"REASON_DISASSOC_PWRCAP_BAD",
	"REASON_DISASSOC_SUPCHAN_BAD",
	"REASON_IE_INVALID",
	"REASON_MIC_FAILURE",
	"REASON_4WAY_HANDSHAKE_TIMEOUT",
	"REASON_GROUP_KEY_UPDATE_TIMEOUT",
	"REASON_IE_IN_4WAY_DIFFERS",
	"REASON_GROUP_CIPHER_INVALID",
	"REASON_PAIRWISE_CIPHER_INVALID",
	"REASON_AKMP_INVALID",
	"REASON_UNSUPP_RSN_IE_VERSION",
	"REASON_INVALID_RSN_IE_CAP",
	"REASON_802_1X_AUTH_FAILED",
	"REASON_CIPHER_SUITE_REJECTED",
};

const char * const EVENT_REASONS_200[]
{
	"REASON_BEACON_TIMEOUT",
	"REASON_NO_AP_FOUND"
};

void wifi_event_handler_cb(System_Event_t * event)
{
	ehConsolePort.print(" (");
	ehConsolePort.print(EVENT_NAMES[event->event] + event->event);

	switch (event->event)
	{
	case EVENT_STAMODE_CONNECTED:
		break;
	case EVENT_STAMODE_DISCONNECTED:
		break;
	case EVENT_STAMODE_AUTHMODE_CHANGE:
		break;
	case EVENT_STAMODE_GOT_IP:
		break;
	case EVENT_SOFTAPMODE_STACONNECTED:
	case EVENT_SOFTAPMODE_STADISCONNECTED:
	{
		char mac[32] = { 0 };
		snprintf(mac, 32, MACSTR ", aid: %d", MAC2STR(event->event_info.sta_connected.mac), event->event_info.sta_connected.aid);

		ehConsolePort.print(mac);
	}
	break;
	}

	ehConsolePort.println(")");
}

void print_softap_config(Stream & consolePort, softap_config const& config)
{
	consolePort.println();
	consolePort.println(F("SoftAP Configuration"));
	consolePort.println(F("--------------------"));

	consolePort.print(F("ssid:            "));
	consolePort.println((char *)config.ssid);

	consolePort.print(F("password:        "));
	consolePort.println((char *)config.password);

	consolePort.print(F("ssid_len:        "));
	consolePort.println(config.ssid_len);

	consolePort.print(F("channel:         "));
	consolePort.println(config.channel);

	consolePort.print(F("authmode:        "));
	consolePort.println(AUTH_MODE_NAMES[config.authmode]);

	consolePort.print(F("ssid_hidden:     "));
	consolePort.println(config.ssid_hidden);

	consolePort.print(F("max_connection:  "));
	consolePort.println(config.max_connection);

	consolePort.print(F("beacon_interval: "));
	consolePort.print(config.beacon_interval);
	consolePort.println("ms");

	consolePort.println(F("--------------------"));
	consolePort.println();
}

void print_system_info(Stream & consolePort)
{
	const rst_info * resetInfo = system_get_rst_info();
	consolePort.print(F("system_get_rst_info() reset reason: "));
	consolePort.println(RST_REASONS[resetInfo->reason]);

	consolePort.print(F("system_get_free_heap_size(): "));
	consolePort.println(system_get_free_heap_size());

	consolePort.print(F("system_get_os_print(): "));
	consolePort.println(system_get_os_print());
	system_set_os_print(1);
	consolePort.print(F("system_get_os_print(): "));
	consolePort.println(system_get_os_print());

	system_print_meminfo();

	consolePort.print(F("system_get_chip_id(): 0x"));
	consolePort.println(system_get_chip_id(), HEX);

	consolePort.print(F("system_get_sdk_version(): "));
	consolePort.println(system_get_sdk_version());

	consolePort.print(F("system_get_boot_version(): "));
	consolePort.println(system_get_boot_version());

	consolePort.print(F("system_get_userbin_addr(): 0x"));
	consolePort.println(system_get_userbin_addr(), HEX);

	consolePort.print(F("system_get_boot_mode(): "));
	consolePort.println(system_get_boot_mode() == 0 ? F("SYS_BOOT_ENHANCE_MODE") : F("SYS_BOOT_NORMAL_MODE"));

	consolePort.print(F("system_get_cpu_freq(): "));
	consolePort.println(system_get_cpu_freq());

	consolePort.print(F("system_get_flash_size_map(): "));
	consolePort.println(FLASH_SIZE_MAP_NAMES[system_get_flash_size_map()]);
}

void print_wifi_general(Stream & consolePort)
{
	consolePort.print(F("wifi_get_channel(): "));
	consolePort.println(wifi_get_channel());

	consolePort.print(F("wifi_get_phy_mode(): "));
	consolePort.println(PHY_MODE_NAMES[wifi_get_phy_mode()]);
}

void printChipDetails() {
	Serial.println("--------Module details--------");
	Serial.print(F("system_get_time(): "));
	Serial.println(system_get_time());

	uint32_t realSize = ESP.getFlashChipRealSize();
	uint32_t ideSize = ESP.getFlashChipSize();
	FlashMode_t ideMode = ESP.getFlashChipMode();

	Serial.printf("Flash real id:   %08X\n", ESP.getFlashChipId());
	Serial.printf("Flash real size: %u\n\n", realSize);

	Serial.printf("Flash ide  size: %u\n", ideSize);
	Serial.printf("Flash ide speed: %u\n", ESP.getFlashChipSpeed());
	Serial.printf("Flash ide mode:  %s\n", (ideMode == FM_QIO ? "QIO" : ideMode == FM_QOUT ? "QOUT" : ideMode == FM_DIO ? "DIO" : ideMode == FM_DOUT ? "DOUT" : "UNKNOWN"));

	if (ideSize != realSize) {
		Serial.println("Flash Chip configuration wrong!\n");
	}
	else {
		Serial.println("Flash Chip configuration ok.\n");
	}
	Serial.println("-------------------------------");
	print_system_info(Serial);
}