/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * lcd/extui/anycubic_dgus_lcd.cpp
 *
 * Anycubic Dgus TFT support for Marlin
 */

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(ANYCUBIC_LCD_DGUS)

#include "../ui_api.h"
#include "dgus_tft.h"

using namespace Anycubic;

namespace ExtUI {

  void onStartup() { Dgus.Startup(); }

  void onParamInit() { Dgus.ParamInit(); }

  void onIdle() { Dgus.IdleLoop(); }

  void onPrinterKilled(PGM_P const error, PGM_P const component) {
    Dgus.PrinterKilled(error,component);
  }

  void onMediaInserted() { Dgus.MediaEvent(AC_media_inserted); }
  void onMediaError()    { Dgus.MediaEvent(AC_media_error);    }
  void onMediaRemoved()  { Dgus.MediaEvent(AC_media_removed);  }

  void onPlayTone(const uint16_t frequency, const uint16_t duration) {
    #if ENABLED(SPEAKER)
      ::tone(BEEPER_PIN, frequency, duration);
    #endif
  }

  void onPrintTimerStarted() { Dgus.TimerEvent(AC_timer_started); }
  void onPrintTimerPaused()  { Dgus.TimerEvent(AC_timer_paused);  }
  void onPrintTimerStopped()                         { Dgus.TimerEvent(AC_timer_stopped); }
  void onFilamentRunout(const extruder_t)            { Dgus.FilamentRunout();             }
  void onUserConfirmRequired(const char * const msg) { Dgus.ConfirmationRequest(msg);     }
  void onStatusChanged(const char * const msg)       { Dgus.StatusChange(msg);            }

  void onHomingStart() {}
  void onHomingComplete() {}
  void onPrintFinished() {}

  void onFactoryReset() {
    Dgus.page_index_now = 121;
    Dgus.lcd_info.language = ExtUI::ENG;
    Dgus.lcd_info.audio = ExtUI::ON;
  }

  void onStoreSettings(char *buff) {
    // Called when saving to EEPROM (i.e. M500). If the ExtUI needs
    // permanent data to be stored, it can write up to eeprom_data_size bytes
    // into buff.

    // Example:
    static_assert(sizeof(Dgus.lcd_info) <= ExtUI::eeprom_data_size);
    memcpy(buff, &Dgus.lcd_info, sizeof(Dgus.lcd_info));
  }

  void onLoadSettings(const char *buff) {
    // Called while loading settings from EEPROM. If the ExtUI
    // needs to retrieve data, it should copy up to eeprom_data_size bytes
    // from buff

    // Example:
    static_assert(sizeof(Dgus.lcd_info) <= ExtUI::eeprom_data_size);
    memcpy(&Dgus.lcd_info, buff, sizeof(Dgus.lcd_info));
    memcpy(&Dgus.lcd_info_back, buff, sizeof(Dgus.lcd_info_back));
  }

  void onPostprocessSettings() {
    // Called after loading or resetting stored settings
  }

  void onConfigurationStoreWritten(bool success) {
    // Called after the entire EEPROM has been written,
    // whether successful or not.
  }

  void onConfigurationStoreRead(bool success) {
    // Called after the entire EEPROM has been read,
    // whether successful or not.
  }

  #if HAS_MESH
    void onMeshLevelingStart() {}

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const float zval) {
      // Called when any mesh points are updated
      //SERIAL_ECHOLNPAIR("onMeshUpdate() x:", xpos, " y:", ypos, " z:", zval);
    }

    void onMeshUpdate(const int8_t xpos, const int8_t ypos, const ExtUI::probe_state_t state) {
      // Called to indicate a special condition
      //SERIAL_ECHOLNPAIR("onMeshUpdate() x:", xpos, " y:", ypos, " state:", state);
    }
  #endif

  #if ENABLED(POWER_LOSS_RECOVERY)
    // Called on resume from power-loss
    void onPowerLoss() { Dgus.PowerLoss(); }
    void onPowerLossResume() { Dgus.PowerLossRecovery(); }
  #endif

  #if HAS_PID_HEATING
    void onPidTuning(const result_t rst) {
      // Called for temperature PID tuning result
    }
  #endif

  void onSteppersDisabled() {}
  void onSteppersEnabled()  {}
}

#endif // ANYCUBIC_LCD_DGUS
