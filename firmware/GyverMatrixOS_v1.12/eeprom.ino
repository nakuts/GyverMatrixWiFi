#define EEPROM_OK 0xA5                     // Флаг, показывающий, что EEPROM инициализирована корректными данными 
#define EFFECT_EEPROM 150                  // начальная ячейка eeprom с параметрами эффектов
#define GAME_EEPROM 230                    // начальная ячейка eeprom с параметрами игр

void loadSettings() {

  // Адреса в EEPROM:
  //    0 - если EEPROM_OK - EEPROM инициализировано, если другое значение - нет 
  //    1 - максимальная яркость ленты 1-255
  //    2 - скорость прокрутки текста по умолчанию
  //    3 - скорость эффектов по умолчанию
  //    4 - скорость игр по умолчанию
  //    5 - разрешен оверлей часов для эффектов
  //    6 - автосмена режима в демо: вкл/выкл
  //    7 - время автосмены режимов
  //    8 - время бездействия до переключения в авторежим
  //    9 - использовать синхронизацию времени через NTP
  //10,11 - период синхронизации NTP (int16_t - 2 байта)
  //   12 - time zone
  //   13 - ориентация часов: 0 - горизонтально; 1 - вертикально
  //   14 - цветовая схема часов 0 - монохром; 1 - каждая цифра свой цвет плавно; 2 - часы, точки, минуты - свой цвет плавно; 3 - часы, точки, минуты - цвет пользователя
  //   15 - использовать эффекты бегущего текста в демо режиме
  //   16 - Отображать с часами текущую дату 
  //   17 - Кол-во секунд отображения даты
  //   18 - Кол-во секунд отображения часов
  //   19 - Будильник вкал/выкл 1 - вкл; 0 -выкл
  //   20 - Будильник, время: часы
  //   21 - Будильник, время: минуты
  //   22 - Будильник, дни недели
  //   23 - Будильник, продолжительность "рассвета"
  //   24 - Будильник, эффект "рассвета"
  //   25 - Будильник, использовать звук
  //   26 - Будильник, играть звук N минут после срабатывания
  //   27 - Будильник, Номер мелодии будильника (из папки 01 на SD карте)
  //   28 - Будильник, Номер мелодии рассвета (из папки 02 на SD карте) 
  //   29 - Будильник, Максимальная громкость будильника
  //   30 - Использовать режим точки доступа
  //   31 - Использовать авторегулировку яркости
  //   32 - Минимальное значение яркости при автоматической регулировке
  //   33 - Режим 1 по времени - часы
  //   34 - Режим 1 по времени - минуты
  //   35 - Режим 1 по времени - ID эффекта или -1 - выключено; 0 - случайный;
  //   36 - Режим 2 по времени - часы
  //   37 - Режим 2 по тайвременимеру - минуты
  //   38 - Режим 2 по времени - ID эффекта или -1 - выключено; 0 - случайный;
  //   39 - зарезервировано
  //  ... - зарезервировано  
  //  48-63   - имя точки доступа    - 16 байт
  //  64-79   - пароль точки доступа - 16 байт
  //  80-103  - имя сети  WiFi       - 24 байта
  //  104-119 - пароль сети  WiFi    - 16 байт
  //  120-149 - имя NTP сервера      - 30 байт
  //  ... - зарезервировано
  //  149 - зарезервировано
  //  150 - 150+(Nэфф*3)   - скорость эффекта
  //  151 - 150+(Nэфф*3)+1 - 1 - оверлей часов разрешен; 0 - нет оверлея часов
  //  152 - 150+(Nэфф*3)+2 - эффект в авторежиме: 1 - использовать; 0 - не использовать
  // ....
  //  230 - 100+(Nигр*2)   - скорость игры
  //  230 - 100+(Nигр*2)+1 - использовать игру в демо-режиме

  // Инициализировано ли EEPROM
  bool isInitialized = EEPROMread(0) == EEPROM_OK;  
    
  if (isInitialized) {    
    globalBrightness = getMaxBrightness();
    scrollSpeed = getScrollSpeed();
    effectSpeed = map(EEPROMread(3),0,255,D_EFFECT_SPEED_MIN,D_EFFECT_SPEED_MAX);
    gameSpeed = map(EEPROMread(4),0,255,D_GAME_SPEED_MIN,D_GAME_SPEED_MAX); 
    AUTOPLAY = getAutoplay();
    autoplayTime = getAutoplayTime();
    idleTime = getIdleTime();    
    overlayEnabled = getClockOverlayEnabled();
    useNtp = getUseNtp();
    SYNC_TIME_PERIOD = getNtpSyncTime();
    timeZoneOffset = getTimeZone();
    CLOCK_ORIENT = getClockOrientation();
    COLOR_MODE = getClockColorMode();
    showDateInClock = getShowDateInClock();  
    showDateDuration = getShowDateDuration();
    showDateInterval = getShowDateInterval();
    alarmOnOff = getAlarmOnOff(); 
    
    alarmHour = getAlarmHour();
    alarmMinute = getAlarmMinute();
    alarmWeekDay = getAlarmWeekDay();
    alarmEffect = getAlarmEffect();
    alarmDuration = getAlarmDuration();

    dawnDuration = getDawnDuration();
    useAlarmSound = getUseAlarmSound();    
    alarmSound = getAlarmSound();
    dawnSound = getDawnSound();
    maxAlarmVolume = getMaxAlarmVolume();

    useSoftAP = getUseSoftAP();
    getSoftAPName().toCharArray(apName, 17);        //  48-63   - имя точки доступа    (16 байта макс) + 1 байт '\0'
    getSoftAPPass().toCharArray(apPass, 17);        //  64-79   - пароль точки доступа (16 байт макс) + 1 байт '\0'
    getSsid().toCharArray(ssid, 25);                //  80-103  - имя сети  WiFi       (24 байта макс) + 1 байт '\0'
    getPass().toCharArray(pass, 17);                //  104-119 - пароль сети  WiFi    (16 байт макс) + 1 байт '\0'
    getNtpServer().toCharArray(ntpServerName, 31);  //  120-149 - имя NTP сервера      (30 байт макс) + 1 байт '\0'
    if (strlen(apName) == 0) strcpy(apName, DEFAULT_AP_NAME);
    if (strlen(apPass) == 0) strcpy(apPass, DEFAULT_AP_PASS);
    if (strlen(ntpServerName) == 0) strcpy(ntpServerName, DEFAULT_NTP_SERVER);

    useAutoBrightness = getUseAutoBrightness();
    autoBrightnessMin = getAutoBrightnessMin();

    AM1_hour = getAM1hour();
    AM1_minute = getAM1minute();
    AM1_effect_id = getAM1effect();
    AM2_hour = getAM2hour();
    AM2_minute = getAM2minute();
    AM2_effect_id = getAM2effect();

  } else {
    globalBrightness = BRIGHTNESS;
    scrollSpeed = D_TEXT_SPEED;
    effectSpeed = D_EFFECT_SPEED;
    gameSpeed = D_GAME_SPEED;
    AUTOPLAY = true;
    autoplayTime = ((long)AUTOPLAY_PERIOD * 1000L);     // секунды -> миллисек
    idleTime = ((long)IDLE_TIME * 60L * 1000L);         // минуты -> миллисек
    overlayEnabled = true;
    useNtp = true;
    SYNC_TIME_PERIOD = 60;
    timeZoneOffset = 7;
    CLOCK_ORIENT = 0;
    COLOR_MODE = 0;
    showDateInClock = true;  
    showDateDuration = 5;
    showDateInterval = 20;
    alarmOnOff = false;
    alarmHour = 0;
    alarmMinute = 0;
    alarmWeekDay = 0;
    dawnDuration = 20;
    alarmEffect = EFFECT_DAWN_ALARM;
    useSoftAP = false;
    useAlarmSound = false;
    alarmDuration = 1;
    alarmSound = 1;
    dawnSound = 1;
    maxAlarmVolume = 30;
    useAutoBrightness = false;
    autoBrightnessMin = 0;

    AM1_hour = 0;
    AM1_minute = 0;
    AM1_effect_id = -5;
    AM2_hour = 0;
    AM2_minute = 0;
    AM2_effect_id = -5;
    
    strcpy(apName, DEFAULT_AP_NAME);
    strcpy(apPass, DEFAULT_AP_PASS);
    strcpy(ssid, NETWORK_SSID);
    strcpy(pass, NETWORK_PASS);

    strcpy(ntpServerName, DEFAULT_NTP_SERVER);
  }

  scrollTimer.setInterval(scrollSpeed);
  effectTimer.setInterval(effectSpeed);
  gameTimer.setInterval(gameSpeed);
  idleTimer.setInterval(idleTime);  
  ntpSyncTimer.setInterval(1000 * 60 * SYNC_TIME_PERIOD);
  
  // После первой инициализации значений - сохранить их принудительно
  if (!isInitialized) {
    saveDefaults();
    saveSettings();
  }
}

void saveDefaults() {

  EEPROMwrite(1, globalBrightness);

  EEPROMwrite(2, D_TEXT_SPEED);
  EEPROMwrite(3, D_EFFECT_SPEED);
  EEPROMwrite(4, D_GAME_SPEED);

  EEPROMwrite(6, AUTOPLAY ? 1 : 0);
  EEPROMwrite(7, autoplayTime / 1000L);
  EEPROMwrite(8, constrain(idleTime / 60L / 1000L, 0, 255));

  EEPROMwrite(5, overlayEnabled);
  EEPROMwrite(9, useNtp ? 1 : 0);
  EEPROM_int_write(10, SYNC_TIME_PERIOD);
  EEPROMwrite(12, (byte)timeZoneOffset);
  EEPROMwrite(13, CLOCK_ORIENT == 1 ? 1 : 0);
  EEPROMwrite(14, COLOR_MODE);
  EEPROMwrite(16, showDateInClock ? 1 : 0);
  EEPROMwrite(17, showDateDuration);
  EEPROMwrite(18, showDateInterval);
  
  saveAlarmParams(alarmOnOff,alarmHour,alarmMinute,alarmWeekDay,dawnDuration,alarmEffect);

  EEPROMwrite(15, 1);    // Использовать бегущий текст в демо-режиме: 0 - нет; 1 - да
  EEPROMwrite(30, 0);    // Использовать режим точки доступа: 0 - нет; 1 - да

  EEPROMwrite(31, 0);    // Авторегулирование яркости отключено.
  EEPROMwrite(32, 1);    // Минимальная яркость: 1

  EEPROMwrite(33, AM1_hour);            // Режим 1 по времени - часы
  EEPROMwrite(34, AM1_minute);          // Режим 1 по времени - минуты
  EEPROMwrite(35, (byte)AM1_effect_id); // Режим 1 по времени - действие: -5 - выключено; -4 - выключить матрицу (черный экран); -3 - ночные часы, -2 - камин с часами, -1 - бегущая строка, 0 - случайныйб 1 и далее - эффект ALARM_LIST
  EEPROMwrite(36, AM2_hour);            // Режим 2 по времени - часы
  EEPROMwrite(37, AM2_minute);          // Режим 2 по времени - минуты
  EEPROMwrite(38, (byte)AM2_effect_id); // Режим 2 по времени - действие: -5 - выключено; -4 - выключить матрицу (черный экран); -3 - ночные часы, -2 - камин с часами, -1 - бегущая строка, 0 - случайныйб 1 и далее - эффект ALARM_LIST
  
  // Настройки по умолчанию для эффектов
  int addr = EFFECT_EEPROM;
  for (int i = 0; i < MAX_EFFECT; i++) {
    saveEffectParams(i, effectSpeed, false, true);
  }

  // Настройки по умолчанию для игр
  addr = GAME_EEPROM;
  for (int i = 0; i < MAX_GAME; i++) {
    saveGameParams(i, gameSpeed, true);
  }
    
  strcpy(apName, DEFAULT_AP_NAME);
  strcpy(apPass, DEFAULT_AP_PASS);
  setSoftAPName(String(apName));
  setSoftAPPass(String(apPass));
  
  strcpy(ntpServerName, DEFAULT_NTP_SERVER);
  setNtpServer(String(ntpServerName));
      
  eepromModified = true;
}

void saveSettings() {

  if (!eepromModified) return;
  
  // Поставить отметку, что EEPROM инициализировано параметрами эффектов
  EEPROMwrite(0, EEPROM_OK);
  
  EEPROM.commit();
  Serial.println(F("Настройки сохранены в EEPROM"));
  
  eepromModified = false;
}

void saveEffectParams(byte effect, int speed, boolean overlay, boolean use) {
  const int addr = EFFECT_EEPROM;  
  EEPROMwrite(addr + effect*3, constrain(map(speed, D_EFFECT_SPEED_MIN, D_EFFECT_SPEED_MAX, 0, 255), 0, 255));        // Скорость эффекта
  EEPROMwrite(addr + effect*3 + 1, overlay ? 1 : 0);             // По умолчанию оверлей часов для эффекта отключен  
  EEPROMwrite(addr + effect*3 + 2, use ? 1 : 0);                 // По умолчанию эффект доступен в демо-режиме  
  eepromModified = true;
}

void saveEffectSpeed(byte effect, int speed) {
  if (speed != getEffectSpeed(effect)) {
    const int addr = EFFECT_EEPROM;  
    EEPROMwrite(addr + effect*3, constrain(map(speed, D_EFFECT_SPEED_MIN, D_EFFECT_SPEED_MAX, 0, 255), 0, 255));        // Скорость эффекта
    eepromModified = true;
  }
}

int getEffectSpeed(byte effect) {
  const int addr = EFFECT_EEPROM;
  return map(EEPROMread(addr + effect*3),0,255,D_EFFECT_SPEED_MIN,D_EFFECT_SPEED_MAX);
}

void saveEffectUsage(byte effect, boolean use) {
  if (use != getEffectUsage(effect)) {
    const int addr = EFFECT_EEPROM;  
    EEPROMwrite(addr + effect*3 + 2, use ? 1 : 0);             // По умолчанию оверлей часов для эффекта отключен  
    eepromModified = true;
  }
}

boolean getEffectUsage(byte effect) {
  const int addr = EFFECT_EEPROM;
  return EEPROMread(addr + effect*3 + 2) == 1;
}

void saveGameParams(byte game, int speed, boolean use) {
  const int addr = GAME_EEPROM;  
  EEPROMwrite(addr + game*2, constrain(map(speed, D_GAME_SPEED_MIN, D_GAME_SPEED_MAX, 0, 255), 0, 255));         // Скорость эффекта
  EEPROMwrite(addr + game*2 + 1, use ? 1 : 0);                                                                   // Использовать игру в демо-режиме
  eepromModified = true;
}

void saveGameSpeed(byte game, int speed) {
  if (speed != getGameSpeed(game)) {
    const int addr = GAME_EEPROM;  
    EEPROMwrite(addr + game*2, constrain(map(speed, D_GAME_SPEED_MIN, D_GAME_SPEED_MAX, 0, 255), 0, 255));         // Скорость эффекта
    eepromModified = true;
  }
}

int getGameSpeed(byte game) {
  const int addr = GAME_EEPROM;  
  return map(EEPROMread(addr + game*2),0,255,D_GAME_SPEED_MIN,D_GAME_SPEED_MAX);
}

void saveGameUsage(byte game, boolean use) {
  if (use != getGameUsage(game)) {
    const int addr = GAME_EEPROM;  
    EEPROMwrite(addr + game*2 + 1, use ? 1 : 0);             // По умолчанию оверлей часов для эффекта отключен  
    eepromModified = true;
  }
}

boolean getGameUsage(byte game) {
  const int addr = GAME_EEPROM;
  return EEPROMread(addr + game*2 + 1) == 1;
}

void saveScrollSpeed(int speed) {
  if (speed != getScrollSpeed()) {
    EEPROMwrite(2, constrain(map(speed, D_TEXT_SPEED_MIN, D_TEXT_SPEED_MAX, 0, 255), 0, 255));
    eepromModified = true;
  }
}

int getScrollSpeed() {
  return map(EEPROMread(2),0,255,D_TEXT_SPEED_MIN,D_TEXT_SPEED_MAX);
}

byte getMaxBrightness() {
  return EEPROMread(1);
}

void saveMaxBrightness(byte brightness) {
  if (brightness != getMaxBrightness()) {
    EEPROMwrite(1, globalBrightness);
    eepromModified = true;
  }
}

void saveAutoplay(boolean value) {
  if (value != getAutoplay()) {
    EEPROMwrite(6, value);
    eepromModified = true;
  }
}

bool getAutoplay() {
  return EEPROMread(6) == 1;
}

void saveAutoplayTime(long value) {
  if (value != getAutoplayTime()) {
    EEPROMwrite(7, constrain(value / 1000L, 0, 255));
    eepromModified = true;
  }
}

long getAutoplayTime() {
  long time = EEPROMread(7) * 1000L;  
  if (time == 0) time = ((long)AUTOPLAY_PERIOD * 1000);
  return time;
}

void saveIdleTime(long value) {
  if (value != getIdleTime()) {
    EEPROMwrite(8, constrain(value / 60L / 1000L, 0, 255));
    eepromModified = true;
  }
}

long getIdleTime() {
  long time = EEPROMread(8) * 60 * 1000L;  
  if (time == 0) time = ((long)IDLE_TIME * 60L * 1000L);
  return time;
}

void saveEffectClock(byte effect, boolean overlay) {
  if (overlay != getEffectClock(effect)) {
    const int addr = EFFECT_EEPROM;  
    EEPROMwrite(addr + effect*3 + 1, overlay ? 1 : 0);             // По умолчанию оверлей часов для эффекта отключен  
    eepromModified = true;
  }
}

boolean getEffectClock(byte effect) {
  const int addr = EFFECT_EEPROM;
  return EEPROMread(addr + effect*3 + 1) == 1;
}

boolean getClockOverlayEnabled() {
  return EEPROMread(5) == 1;
}

void saveClockOverlayEnabled(boolean enabled) {
  if (enabled != getClockOverlayEnabled()) {
    EEPROMwrite(5, enabled ? 1 : 0);
    eepromModified = true;
  }
}

void saveUseNtp(boolean value) {
  if (value != getUseNtp()) {
    EEPROMwrite(9, value);
    eepromModified = true;
  }
}

bool getUseNtp() {
  return EEPROMread(9) == 1;
}

void saveNtpSyncTime(uint16_t value) {
  if (value != getNtpSyncTime()) {
    EEPROM_int_write(10, SYNC_TIME_PERIOD);
    eepromModified = true;
  }
}

uint16_t getNtpSyncTime() {
  uint16_t time = EEPROM_int_read(10);  
  if (time == 0) time = 60;
  return time;
}

void saveTimeZone(int8_t value) {
  if (value != getTimeZone()) {
    EEPROMwrite(12, (byte)value);
    eepromModified = true;
  }
}

int8_t getTimeZone() {
  return (int8_t)EEPROMread(12);
}

byte getClockOrientation() {
  return EEPROMread(13) == 1 ? 1 : 0;
}

void saveClockOrientation(byte orientation) {
  if (orientation != getClockOrientation()) {
    EEPROMwrite(13, orientation == 1 ? 1 : 0);
    eepromModified = true;
  }
}

byte getClockColorMode() {
  return EEPROMread(14);
}

void saveClockColorMode(byte ColorMode) {
  if (ColorMode != getClockColorMode()) {
    EEPROMwrite(14, COLOR_MODE);
    eepromModified = true;
  }
}

bool getShowDateInClock() {
  return EEPROMread(16) == 1;
}

void setShowDateInClock(boolean use) {  
  if (use != getShowDateInClock()) {
    EEPROMwrite(16, use ? 1 : 0);
    eepromModified = true;
  }
}

byte getShowDateDuration() {
  return EEPROMread(17);
}

void setShowDateDuration(byte Duration) {
  if (Duration != getShowDateDuration()) {
    EEPROMwrite(17, Duration);
    eepromModified = true;
  }
}

byte getShowDateInterval() {
  return EEPROMread(18);
}

void setShowDateInterval(byte Interval) {
  if (Interval != getShowDateInterval()) {
    EEPROMwrite(18, Interval);
    eepromModified = true;
  }
}

void saveAlarmParams(boolean alarmOnOff, byte alarmHour, byte alarmMinute, byte alarmWeekDay, byte dawnDuration, byte alarmEffect) {
  //   19 - Будильник вкл/выкл 1 - вкл; 0 -выкл
  //   20 - Будильник, время: часы
  //   21 - Будильник, время: минуты
  //   22 - Будильник, дни недели
  //   23 - Будильник, продолжительность "рассвета"
  //   24 - Будильник, эффект "рассвета"
  if (alarmOnOff != getAlarmOnOff()) {
    EEPROMwrite(19, alarmOnOff ? 1 : 0);
    eepromModified = true;
  }
  if (alarmHour != getAlarmHour()) {
    EEPROMwrite(20, alarmHour);
    eepromModified = true;
  }
  if (alarmMinute != getAlarmMinute()) {
    EEPROMwrite(21, alarmMinute);
    eepromModified = true;
  }
  if (alarmWeekDay != getAlarmWeekDay()) {
    EEPROMwrite(22, alarmWeekDay);
    eepromModified = true;
  }
  if (dawnDuration != getDawnDuration()) {
    EEPROMwrite(23, dawnDuration);
    eepromModified = true;
  }
  if (alarmEffect != getAlarmEffect()) {
    EEPROMwrite(24, alarmEffect);
    eepromModified = true;
  }
}

bool getAlarmOnOff() { 
  return EEPROMread(19) == 1;
}

byte getAlarmHour() { 
  return constrain(EEPROMread(20), 0, 23);
}


byte getAlarmMinute() { 
  return constrain(EEPROMread(21), 0, 59);
}

byte getAlarmWeekDay() { 
  return EEPROMread(22);
}

byte getDawnDuration() { 
  return constrain(EEPROMread(23),1,59);
}

byte getAlarmEffect() { 
  return EEPROMread(24);
}

void saveAlarmSounds(boolean useSound, byte duration, byte maxVolume, int8_t alarmSound, int8_t dawnSound) {
  //   25 - Будильник звук: вкл/выкл 1 - вкл; 0 -выкл
  //   26 - Будильник, длительностьзвука будильника, минут
  //   27 - Будильник, мелодия будильника
  //   28 - Будильник, мелодия рассвета
  //   29 - Будильник, максимальная громкость
  if (useSound != getUseAlarmSound()) {
    EEPROMwrite(25, useSound ? 1 : 0);
    eepromModified = true;
  }
  if (duration != getAlarmDuration()) {
    EEPROMwrite(26, duration);
    eepromModified = true;
  }
  if (maxVolume != getMaxAlarmVolume()) {
    EEPROMwrite(29, maxVolume);
    eepromModified = true;
  }
  if (alarmSound != getAlarmSound()) {
    EEPROMwrite(27, (byte)alarmSound);
    eepromModified = true;
  }
  if (dawnSound != getDawnSound()) {
    EEPROMwrite(28, (byte)dawnSound);
  }
  if (alarmEffect != getAlarmEffect()) {
    EEPROMwrite(24, alarmEffect);
    eepromModified = true;
  }
}

bool getUseAlarmSound() { 
  return EEPROMread(25) == 1;
}

byte getAlarmDuration() { 
  return constrain(EEPROMread(25),1,10);
}

byte getMaxAlarmVolume() { 
  return constrain(EEPROMread(29),0,30);
}

int8_t getAlarmSound() { 
  return constrain((int8_t)EEPROMread(27),-1,127);
}

int8_t getDawnSound() { 
  return constrain((int8_t)EEPROMread(28),-1,127);
}

bool getUseTextInDemo() {
  return EEPROMread(15) == 1;
}

void setUseTextInDemo(boolean use) {  
  if (use != getUseTextInDemo()) {
    EEPROMwrite(15, use ? 1 : 0);
    eepromModified = true;
  }
}

bool getUseSoftAP() {
  return EEPROMread(30) == 1;
}

void setUseSoftAP(boolean use) {  
  if (use != getUseSoftAP()) {
    EEPROMwrite(30, use ? 1 : 0);
    eepromModified = true;
  }
}

String getSoftAPName() {
  return EEPROM_string_read(48, 16);
}

void setSoftAPName(String SoftAPName) {
  if (SoftAPName != getSoftAPName()) {
    EEPROM_string_write(48, SoftAPName);
    eepromModified = true;
  }
}

String getSoftAPPass() {
  return EEPROM_string_read(64, 16);
}

void setSoftAPPass(String SoftAPPass) {
  if (SoftAPPass != getSoftAPPass()) {
    EEPROM_string_write(64, SoftAPPass);
    eepromModified = true;
  }
}

String getSsid() {
  return EEPROM_string_read(80, 24);
}

void setSsid(String Ssid) {
  if (Ssid != getSsid()) {
    EEPROM_string_write(80, Ssid);
    eepromModified = true;
  }
}

String getPass() {
  return EEPROM_string_read(104, 16);
}

void setPass(String Pass) {
  if (Pass != getPass()) {
    EEPROM_string_write(104, Pass);
    eepromModified = true;
  }
}

String getNtpServer() {
  return EEPROM_string_read(120, 30);
}

void setNtpServer(String server) {
  if (server != getNtpServer()) {
    EEPROM_string_write(120, server);
    eepromModified = true;
  }
}

bool getUseAutoBrightness() {
  return EEPROMread(31) == 1;
}

void setUseAutoBrightness(boolean use) {  
  if (use != getUseAutoBrightness()) {
    EEPROMwrite(31, use ? 1 : 0);
    eepromModified = true;
  }
}

byte getAutoBrightnessMin() {
  return EEPROMread(32);
}

void setAutoBrightnessMin(byte brightness) {
  if (brightness != getAutoBrightnessMin()) {
    EEPROMwrite(32, brightness);
    eepromModified = true;
  }
}

void setAM1params(byte hour, byte minute, int8_t effect) { 
  setAM1hour(hour);
  setAM1minute(minute);
  setAM1effect(effect);
}

byte getAM1hour() { 
  byte hour = EEPROMread(33);
  if (hour>23) hour = 0;
  return hour;
}

void setAM1hour(byte hour) {
  if (hour != getAM1hour()) {
    EEPROMwrite(33, hour);
    eepromModified = true;
  }
}

byte getAM1minute() {
  byte minute = EEPROMread(34);
  if (minute > 59) minute = 0;
  return minute;
}

void setAM1minute(byte minute) {
  if (minute != getAM1minute()) {
    EEPROMwrite(34, minute);
    eepromModified = true;
  }
}

int8_t getAM1effect() {
  return (int8_t)EEPROMread(35);
}

void setAM1effect(int8_t effect) {
  if (effect != getAM1minute()) {
    EEPROMwrite(35, (byte)effect);
    eepromModified = true;
  }
}

void setAM2params(byte hour, byte minute, int8_t effect) { 
  setAM2hour(hour);
  setAM2minute(minute);
  setAM2effect(effect);
}

byte getAM2hour() { 
  byte hour = EEPROMread(36);
  if (hour>23) hour = 0;
  return hour;
}

void setAM2hour(byte hour) {
  if (hour != getAM2hour()) {
    EEPROMwrite(36, hour);
    eepromModified = true;
  }
}

byte getAM2minute() {
  byte minute = EEPROMread(37);
  if (minute > 59) minute = 0;
  return minute;
}

void setAM2minute(byte minute) {
  if (minute != getAM2minute()) {
    EEPROMwrite(37, minute);
    eepromModified = true;
  }
}

int8_t getAM2effect() {
  return (int8_t)EEPROMread(38);
}

void setAM2effect(int8_t effect) {
  if (effect != getAM2minute()) {
    EEPROMwrite(38, (byte)effect);
    eepromModified = true;
  }
}

// ----------------------------------------------------------

byte EEPROMread(byte addr) {    
  return EEPROM.read(addr);
}

void EEPROMwrite(byte addr, byte value) {    
  EEPROM.write(addr, value);
}

// чтение uint16_t
uint16_t EEPROM_int_read(byte addr) {    
  byte raw[2];
  for (byte i = 0; i < 2; i++) raw[i] = EEPROMread(addr+i);
  uint16_t &num = (uint16_t&)raw;
  return num;
}

// запись uint16_t
void EEPROM_int_write(byte addr, uint16_t num) {
  byte raw[2];
  (uint16_t&)raw = num;
  for (byte i = 0; i < 2; i++) EEPROMwrite(addr+i, raw[i]);
}

// чтение стоки (макс 32 байта)
String EEPROM_string_read(byte addr, byte len) {
   if (len>32) len = 32;
   char buffer[len+1];
   memset(buffer,'\0',len+1);
   byte i = 0;
   while (i < len) {
     byte c = EEPROMread(addr+i);
     if (isAlphaNumeric(c) || isPunct(c))
        buffer[i++] = c;
     else
       break;
   }
   return String(buffer);
}

// запись строки (макс 32 байта)
void EEPROM_string_write(byte addr, String buffer) {
   uint16_t len = buffer.length();
   if (len>32) len = 32;
   byte i = 0;
   while (i < len) {
     EEPROMwrite(addr+i, buffer[i++]);
   }
   if (i < 32) EEPROMwrite(addr+i,0);
}

// ----------------------------------------------------------
