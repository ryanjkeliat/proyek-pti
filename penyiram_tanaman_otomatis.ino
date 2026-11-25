/*
  ============================================================
  SISTEM PENYIRAM TANAMAN OTOMATIS
  Mikrokontroler : ESP32-C3 Super Mini
  Board (IDE)    : ESP32C3 Dev Module, USB CDC On Boot: Enabled
  Sensor         : Capacitive Soil Moisture Sensor v1.2
  Aktuator       : Modul Relay 1 Channel 5V + Pompa Air Celup DC 5V

  WIRING:
    Sensor  : VCC -> 3V3 | GND -> GND | AOUT -> GPIO 2
    Relay   : DC+ -> 5V  | DC-  -> GND | IN   -> GPIO 4
    Pompa   : terminal COM & NO relay (seri dengan catu daya pompa)
  ============================================================
*/

// ===================== PENGATURAN YANG MUDAH DIUBAH =====================
int batasKering = 2500;          // Ambang batas ADC: nilai > batasKering = tanah kering (kalibrasi sesuai sensor Anda)

// Pilih tipe modul relay Anda dengan mengubah nilai di bawah ini:
//   true  -> Relay Active LOW  (relay menyala saat pin IN diberi LOW)  -> paling umum
//   false -> Relay Active HIGH (relay menyala saat pin IN diberi HIGH)
const bool RELAY_ACTIVE_LOW = true;
// ========================================================================

const int PIN_SENSOR = 2;        // Pin analog sensor kelembaban tanah (AOUT) terhubung ke GPIO 2
const int PIN_RELAY  = 4;        // Pin kontrol relay (IN) terhubung ke GPIO 4

const int JUMLAH_SAMPEL = 10;    // Jumlah pembacaan yang dirata-ratakan agar nilai lebih stabil
const unsigned long JEDA_LOOP = 1000; // Jeda tiap siklus 1000 ms (aman untuk watchdog ESP32-C3)

bool pompaMenyala = false;       // Menyimpan status pompa saat ini (true = menyala, false = mati)

// Fungsi untuk menyalakan atau mematikan relay sesuai tipe modul (Active LOW / HIGH)
void setRelay(bool nyala) {
  if (RELAY_ACTIVE_LOW) {                          // Jika relay bertipe Active LOW
    digitalWrite(PIN_RELAY, nyala ? LOW : HIGH);   // Nyala = LOW, Mati = HIGH
  } else {                                         // Jika relay bertipe Active HIGH
    digitalWrite(PIN_RELAY, nyala ? HIGH : LOW);   // Nyala = HIGH, Mati = LOW
  }
  pompaMenyala = nyala;                            // Simpan status pompa terbaru
}

// Fungsi membaca sensor dengan rata-rata beberapa sampel (raw ADC 12-bit: 0 - 4095)
int bacaSensor() {
  long total = 0;                                  // Variabel penampung jumlah seluruh sampel
  for (int i = 0; i < JUMLAH_SAMPEL; i++) {        // Ulangi pembacaan sebanyak JUMLAH_SAMPEL (loop terbatas, tidak blocking)
    total += analogRead(PIN_SENSOR);               // Baca nilai ADC dan tambahkan ke total
    delay(5);                                      // Jeda singkat 5 ms antar sampel agar pembacaan stabil
  }
  return (int)(total / JUMLAH_SAMPEL);             // Kembalikan nilai rata-rata
}

void setup() {
  Serial.begin(115200);                            // Mulai komunikasi serial dengan baud rate 115200
  unsigned long mulai = millis();                  // Catat waktu mulai untuk batas tunggu Serial
  while (!Serial && (millis() - mulai) < 3000) {   // Tunggu USB CDC maksimal 3 detik (tidak menunggu selamanya)
    delay(10);                                     // Jeda kecil agar watchdog tetap ter-reset
  }

  // Set level awal relay = MATI SEBELUM pin dijadikan output, mencegah pompa berkedip saat boot
  digitalWrite(PIN_RELAY, RELAY_ACTIVE_LOW ? HIGH : LOW); // Siapkan level pin pada kondisi relay mati
  pinMode(PIN_RELAY, OUTPUT);                      // Jadikan GPIO 4 sebagai output
  setRelay(false);                                 // Pastikan pompa dalam keadaan mati saat awal

  analogReadResolution(12);                        // Atur resolusi ADC 12-bit (nilai 0 - 4095)
  analogSetPinAttenuation(PIN_SENSOR, ADC_11db);   // Atenuasi 11 dB agar rentang baca sampai ~3,3 V
  pinMode(PIN_SENSOR, INPUT);                      // Jadikan GPIO 2 sebagai input analog

  Serial.println("=== Sistem Penyiram Tanaman Otomatis ===");           // Tampilkan judul program
  Serial.print("Batas kering (batasKering) = ");                         // Tampilkan label ambang batas
  Serial.println(batasKering);                                           // Tampilkan nilai ambang batas
  Serial.println(RELAY_ACTIVE_LOW ? "Tipe relay: Active LOW" : "Tipe relay: Active HIGH"); // Tampilkan tipe relay
  Serial.println("----------------------------------------");           // Garis pemisah
}

void loop() {
  int nilaiSensor = bacaSensor();                  // Baca nilai mentah (raw ADC) sensor kelembaban tanah

  if (nilaiSensor > batasKering) {                 // Jika nilai > batas, berarti tanah KERING
    setRelay(true);                                // Nyalakan pompa
  } else {                                         // Jika nilai <= batas, berarti tanah BASAH/LEMBAB
    setRelay(false);                               // Matikan pompa
  }

  Serial.print("Raw ADC: ");                       // Cetak label nilai sensor
  Serial.print(nilaiSensor);                       // Cetak nilai mentah ADC (untuk kalibrasi)
  Serial.print(" | Batas: ");                      // Cetak label batas
  Serial.print(batasKering);                       // Cetak nilai batas kering
  Serial.print(" | Kondisi tanah: ");              // Cetak label kondisi tanah
  Serial.print(nilaiSensor > batasKering ? "KERING" : "BASAH/LEMBAB"); // Cetak kondisi tanah
  Serial.print(" | Pompa: ");                      // Cetak label status pompa
  Serial.println(pompaMenyala ? "MENYALA" : "MATI"); // Cetak status pompa (MENYALA / MATI)

  delay(JEDA_LOOP);                                // Jeda 1000 ms; beri waktu CPU/watchdog agar tidak crash
}
