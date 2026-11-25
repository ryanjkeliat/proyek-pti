# Sistem Penyiram Tanaman Otomatis Berbasis ESP32-C3 Super Mini dan Sensor Kelembaban Tanah Kapasitif

Proyek praktikum **Program Studi D-3 Teknik Informatika, Fakultas Vokasi, Universitas Sumatera Utara** yang bertujuan mengotomatisasi proses penyiraman tanaman berdasarkan pembacaan kadar air media tanam secara *real-time*.

---

## Identitas Pembuat
* **Nama Mahasiswa 1:** [Isi Nama Lengkap] (`[Isi NIM]`)
* **Nama Mahasiswa 2:** [Isi Nama Lengkap] (`[Isi NIM]`)
* **Nama Mahasiswa 3:** [Isi Nama Lengkap] (`[Isi NIM]`)
* **Kelas / KOM:** KOM A2'26
* **Tahun:** 2026

---

## Daftar Komponen Perangkat Keras
1. **Mikrokontroler:** ESP32-C3 Super Mini (USB Type-C)
2. **Sensor:** Capacitive Soil Moisture Sensor v1.2
3. **Modul Pengendali:** Modul Relay 1 Channel 5V
4. **Aktuator:** Pompa Air Celup Mini (*Submersible Water Pump*) DC 3V–5V + Selang Air
5. **Kabel & Sumber Daya:** Kabel Jumper (*Female-to-Female* / *Male-to-Female*) dan Adaptor 5V 1A/2A

---

## Skema Pengkabelan (*Wiring*)

### 1. Sensor Kelembaban Tanah ke ESP32-C3 Super Mini
| Pin Sensor Tanah | Pin ESP32-C3 Super Mini | Keterangan |
| :--- | :--- | :--- |
| `VCC` | `3V3` | Tegangan daya 3,3V |
| `GND` | `GND` | Ground |
| `AOUT` | `GPIO 2` (Pin `2`) | Keluaran sinyal analog (ADC1) |

### 2. Modul Relay 5V ke ESP32-C3 Super Mini (Sisi Pin Input)
| Pin Modul Relay | Pin ESP32-C3 Super Mini | Keterangan |
| :--- | :--- | :--- |
| `DC+` | `5V` | Tegangan kerja koil relay 5V |
| `DC-` | `GND` | Ground |
| `IN` | `GPIO 4` (Pin `4`) | Sinyal pemicu saklar relay |

### 3. Terminal Output Relay ke Pompa Air DC 5V
| Terminal Relay | Keterangan Aksara | Sambungkan ke |
| :--- | :--- | :--- |
| `NO` (*Normally Open*) | **常开** | Kabel Merah (`+`) Pompa Air |
| `COM` (*Common*) | **公共** | Jalur `5V` dari ESP32-C3 / Adaptor |
| `NC` (*Normally Closed*) | **常闭** | *Tidak digunakan (kosong)* |

*(Catatan: Kabel Hitam (`-`) dari pompa air disambungkan langsung ke jalur `GND`).*

---

## Pengaturan Arduino IDE
Sebelum melakukan proses *upload* kode program (`.ino`) ke papan **ESP32-C3 Super Mini**, pastikan konfigurasi pada menu **Tools** di Arduino IDE diatur sebagai berikut agar pembacaan *Serial Monitor* berjalan lancar dan terhindar dari *boot loop*:

* **Board:** `ESP32C3 Dev Module`
* **USB CDC On Boot:** `Enabled`
* **Flash Frequency:** `40MHz`
* **Flash Mode:** `DIO`
* **Baud Rate Serial Monitor:** `115200`

---

## Cara Kalibrasi & Penggunaan
1. Unggah file program utama ke **ESP32-C3 Super Mini** menggunakan kabel data USB Type-C.
2. Buka **Serial Monitor** pada baud rate `115200` untuk melihat nilai pembacaan mentah (*raw ADC 12-bit*) sensor.
3. Catat nilai sensor saat kering di udara terbuka (contoh: `~2900`) dan saat bilah sensor dicelupkan ke dalam air hingga batas garis putih (contoh: `~1800`).
4. Sesuaikan variabel `batasKering` pada bagian atas kode program dengan nilai tengah di antara kedua hasil pengukuran tersebut, lalu unggah ulang kode.
5. Pindahkan sumber daya dari port USB laptop ke adaptor charger HP 5V atau *powerbank* saat menjalankan pompa air secara penuh.
