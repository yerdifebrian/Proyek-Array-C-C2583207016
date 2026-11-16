/*
 * Sistem Monitoring Stok dan Penjualan Kantin Kampus
 * Bahasa: C (console)
 *
 * Struktur data utama (array):
 *   - nama[MAX_PRODUK][PANJANG_NAMA] : array karakter 2 dimensi (nama produk)
 *   - harga[], stok[], terjual[], pendapatan[] : array 1 dimensi
 *   - jumlah : jumlah data aktif (indeks 0 .. jumlah-1)
 *
 * Kompilasi : gcc kantin.c -o kantin
 * Jalankan  : ./kantin   (Windows: kantin.exe)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUK   50
#define PANJANG_NAMA 30
#define BATAS_MENIPIS 5

/* ===== Data utama ===== */
char nama[MAX_PRODUK][PANJANG_NAMA];
int  harga[MAX_PRODUK];
int  stok[MAX_PRODUK];
int  terjual[MAX_PRODUK];
long pendapatan[MAX_PRODUK];   /* pendapatan per produk (akumulasi transaksi) */
int  jumlah = 0;               /* jumlah produk aktif */

/* ===== Fungsi bantu input ===== */

/* Membaca satu baris teks, membuang newline */
void bacaTeks(const char *prompt, char *buf, int ukuran) {
    printf("%s", prompt);
    if (fgets(buf, ukuran, stdin) == NULL) {
        /* input berakhir (EOF): hentikan program agar tidak berulang tanpa akhir */
        printf("\nInput berakhir. Program dihentikan.\n");
        exit(0);
    }
    size_t n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n') {
        buf[n - 1] = '\0';
    } else {
        /* baris terlalu panjang: buang sisa input */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
}

/* Membaca bilangan bulat; mengembalikan 1 jika valid, 0 jika bukan angka */
int bacaInt(const char *prompt, int *hasil) {
    char buf[64];
    char *akhir;
    long v;

    bacaTeks(prompt, buf, sizeof(buf));
    if (buf[0] == '\0') return 0;
    v = strtol(buf, &akhir, 10);
    while (*akhir == ' ' || *akhir == '\t') akhir++;
    if (*akhir != '\0' || v > 2000000000L || v < -2000000000L) return 0;
    *hasil = (int)v;
    return 1;
}

/* Ubah string menjadi huruf kecil (untuk pencarian) */
void keHurufKecil(const char *sumber, char *tujuan) {
    int i;
    for (i = 0; sumber[i] != '\0'; i++)
        tujuan[i] = (char)tolower((unsigned char)sumber[i]);
    tujuan[i] = '\0';
}

/* Cek apakah nama produk sudah ada (tanpa memedulikan huruf besar/kecil) */
int namaSudahAda(const char *cek) {
    char a[PANJANG_NAMA], b[PANJANG_NAMA];
    int i;
    keHurufKecil(cek, a);
    for (i = 0; i < jumlah; i++) {
        keHurufKecil(nama[i], b);
        if (strcmp(a, b) == 0) return 1;
    }
    return 0;
}

/* Pilih produk berdasarkan nomor; mengembalikan indeks atau -1 jika tidak valid */
int pilihProduk(const char *prompt) {
    int no;
    if (!bacaInt(prompt, &no)) {
        printf("[!] Input harus berupa angka.\n");
        return -1;
    }
    if (no < 1 || no > jumlah) {
        printf("[!] Nomor produk %d tidak ditemukan.\n", no);
        return -1;
    }
    return no - 1;
}

/* ===== Inisialisasi ===== */
void isiDataAwal(void) {
    const char *n[] = {"Nasi Goreng", "Mie Ayam", "Es Teh Manis",
                       "Gorengan", "Air Mineral", "Kopi Susu"};
    int h[] = {12000, 10000, 4000, 2000, 3000, 8000};
    int s[] = {20, 15, 30, 40, 4, 12};
    int i, total = 6;

    for (i = 0; i < total; i++) {
        strcpy(nama[i], n[i]);
        harga[i] = h[i];
        stok[i] = s[i];
        terjual[i] = 0;
        pendapatan[i] = 0;
    }
    jumlah = total;
}

/* ===== Fitur 1: Lihat daftar produk ===== */
void tampilDaftar(void) {
    int i;
    if (jumlah == 0) {
        printf("Belum ada produk.\n");
        return;
    }
    printf("\n%-4s %-30s %10s %6s\n", "No", "Nama Produk", "Harga", "Stok");
    printf("---------------------------------------------------------\n");
    for (i = 0; i < jumlah; i++)
        printf("%-4d %-30s %10d %6d\n", i + 1, nama[i], harga[i], stok[i]);
}

/* ===== Fitur 2: Tambah produk ===== */
void tambahProduk(void) {
    char namaBaru[PANJANG_NAMA];
    int hargaBaru, stokBaru;

    if (jumlah >= MAX_PRODUK) {
        printf("[!] Kapasitas penuh (maksimal %d produk).\n", MAX_PRODUK);
        return;
    }

    bacaTeks("Nama produk : ", namaBaru, sizeof(namaBaru));
    if (namaBaru[0] == '\0') {
        printf("[!] Nama produk tidak boleh kosong.\n");
        return;
    }
    if (namaSudahAda(namaBaru)) {
        printf("[!] Produk dengan nama tersebut sudah ada.\n");
        return;
    }
    if (!bacaInt("Harga       : ", &hargaBaru)) {
        printf("[!] Harga harus berupa angka.\n");
        return;
    }
    if (hargaBaru < 0) {
        printf("[!] Harga tidak boleh negatif.\n");
        return;
    }
    if (!bacaInt("Stok awal   : ", &stokBaru)) {
        printf("[!] Stok harus berupa angka.\n");
        return;
    }
    if (stokBaru < 0) {
        printf("[!] Stok tidak boleh negatif.\n");
        return;
    }

    strcpy(nama[jumlah], namaBaru);
    harga[jumlah] = hargaBaru;
    stok[jumlah] = stokBaru;
    terjual[jumlah] = 0;
    pendapatan[jumlah] = 0;
    jumlah++;
    printf("Produk \"%s\" berhasil ditambahkan.\n", namaBaru);
}

/* ===== Fitur 3: Ubah harga dan stok ===== */
void ubahProduk(void) {
    int idx, hargaBaru, stokBaru;

    tampilDaftar();
    if (jumlah == 0) return;
    idx = pilihProduk("\nNomor produk yang diubah: ");
    if (idx < 0) return;

    printf("Produk: %s (harga %d, stok %d)\n", nama[idx], harga[idx], stok[idx]);
    if (!bacaInt("Harga baru : ", &hargaBaru)) {
        printf("[!] Harga harus berupa angka.\n");
        return;
    }
    if (hargaBaru < 0) {
        printf("[!] Harga tidak boleh negatif.\n");
        return;
    }
    if (!bacaInt("Stok baru  : ", &stokBaru)) {
        printf("[!] Stok harus berupa angka.\n");
        return;
    }
    if (stokBaru < 0) {
        printf("[!] Stok tidak boleh negatif.\n");
        return;
    }

    harga[idx] = hargaBaru;
    stok[idx] = stokBaru;
    printf("Data produk berhasil diubah.\n");
}

/* ===== Fitur 4: Transaksi penjualan (beberapa produk) ===== */
void transaksiPenjualan(void) {
    int qtyBeli[MAX_PRODUK] = {0};  /* jumlah beli per produk dalam transaksi ini */
    int idx, qty, i;
    int adaItem = 0;
    long subtotal, total = 0;
    char konfirmasi[8];

    if (jumlah == 0) {
        printf("Belum ada produk.\n");
        return;
    }

    tampilDaftar();
    printf("\nMasukkan nomor produk (0 = selesai).\n");

    while (1) {
        int no;
        if (!bacaInt("\nNomor produk: ", &no)) {
            printf("[!] Input harus berupa angka.\n");
            continue;
        }
        if (no == 0) break;
        if (no < 1 || no > jumlah) {
            printf("[!] Nomor produk %d tidak ditemukan.\n", no);
            continue;
        }
        idx = no - 1;

        if (!bacaInt("Jumlah beli  : ", &qty)) {
            printf("[!] Jumlah harus berupa angka.\n");
            continue;
        }
        if (qty <= 0) {
            printf("[!] Jumlah pembelian harus lebih dari 0.\n");
            continue;
        }
        /* periksa stok, termasuk jumlah yang sudah ada di keranjang */
        if (qtyBeli[idx] + qty > stok[idx]) {
            printf("[!] Stok %s tidak cukup (tersedia %d, sudah dipilih %d).\n",
                   nama[idx], stok[idx], qtyBeli[idx]);
            continue;
        }
        qtyBeli[idx] += qty;
        adaItem = 1;
        printf("Ditambahkan: %s x %d\n", nama[idx], qty);
    }

    if (!adaItem) {
        printf("Transaksi dibatalkan (tidak ada produk dipilih).\n");
        return;
    }

    /* tampilkan rincian */
    printf("\n=========== RINCIAN TRANSAKSI ===========\n");
    printf("%-25s %5s %10s %12s\n", "Produk", "Qty", "Harga", "Subtotal");
    for (i = 0; i < jumlah; i++) {
        if (qtyBeli[i] > 0) {
            subtotal = (long)qtyBeli[i] * harga[i];
            total += subtotal;
            printf("%-25s %5d %10d %12ld\n", nama[i], qtyBeli[i], harga[i], subtotal);
        }
    }
    printf("-----------------------------------------------------\n");
    printf("%-25s %5s %10s %12ld\n", "TOTAL PEMBAYARAN", "", "", total);

    bacaTeks("\nProses transaksi? (y/n): ", konfirmasi, sizeof(konfirmasi));
    if (tolower((unsigned char)konfirmasi[0]) != 'y') {
        printf("Transaksi dibatalkan. Stok tidak berubah.\n");
        return;
    }

    /* transaksi berhasil: kurangi stok, tambah terjual & pendapatan */
    for (i = 0; i < jumlah; i++) {
        if (qtyBeli[i] > 0) {
            stok[i] -= qtyBeli[i];
            terjual[i] += qtyBeli[i];
            pendapatan[i] += (long)qtyBeli[i] * harga[i];
        }
    }
    printf("Transaksi berhasil. Total dibayar: %ld\n", total);
}

/* ===== Fitur 5: Tambah stok ===== */
void tambahStok(void) {
    int idx, tambah;

    tampilDaftar();
    if (jumlah == 0) return;
    idx = pilihProduk("\nNomor produk: ");
    if (idx < 0) return;

    if (!bacaInt("Jumlah stok yang ditambah: ", &tambah)) {
        printf("[!] Jumlah harus berupa angka.\n");
        return;
    }
    if (tambah <= 0) {
        printf("[!] Jumlah tambahan stok harus lebih dari 0.\n");
        return;
    }
    stok[idx] += tambah;
    printf("Stok %s sekarang: %d\n", nama[idx], stok[idx]);
}

/* ===== Fitur 6: Cari produk berdasarkan nama ===== */
void cariProduk(void) {
    char kata[PANJANG_NAMA], kataKecil[PANJANG_NAMA], namaKecil[PANJANG_NAMA];
    int i, ketemu = 0;

    bacaTeks("Masukkan nama produk yang dicari: ", kata, sizeof(kata));
    if (kata[0] == '\0') {
        printf("[!] Kata kunci tidak boleh kosong.\n");
        return;
    }
    keHurufKecil(kata, kataKecil);

    for (i = 0; i < jumlah; i++) {
        keHurufKecil(nama[i], namaKecil);
        if (strstr(namaKecil, kataKecil) != NULL) {
            if (!ketemu) {
                printf("\n%-4s %-30s %10s %6s\n", "No", "Nama Produk", "Harga", "Stok");
                printf("---------------------------------------------------------\n");
            }
            printf("%-4d %-30s %10d %6d\n", i + 1, nama[i], harga[i], stok[i]);
            ketemu++;
        }
    }
    if (!ketemu)
        printf("Produk \"%s\" tidak ditemukan.\n", kata);
    else
        printf("Ditemukan %d produk.\n", ketemu);
}

/* ===== Fitur 7: Urutkan produk ===== */

/* Urutkan indeks urut[] (bubble sort).
 * mode 1 : harga termurah -> termahal
 * mode 2 : jumlah terjual terbanyak -> tersedikit */
void urutkanIndeks(int urut[], int mode) {
    int i, j, tmp, tukar;
    for (i = 0; i < jumlah - 1; i++) {
        for (j = 0; j < jumlah - 1 - i; j++) {
            if (mode == 1)
                tukar = harga[urut[j]] > harga[urut[j + 1]];
            else
                tukar = terjual[urut[j]] < terjual[urut[j + 1]];
            if (tukar) {
                tmp = urut[j];
                urut[j] = urut[j + 1];
                urut[j + 1] = tmp;
            }
        }
    }
}

void urutkanProduk(void) {
    int urut[MAX_PRODUK];
    int pilihan, i;

    if (jumlah == 0) {
        printf("Belum ada produk.\n");
        return;
    }
    printf("\n1. Harga termurah\n2. Jumlah terjual terbanyak\n");
    if (!bacaInt("Pilih urutan: ", &pilihan) || (pilihan != 1 && pilihan != 2)) {
        printf("[!] Pilihan urutan tidak tersedia.\n");
        return;
    }

    for (i = 0; i < jumlah; i++) urut[i] = i;
    urutkanIndeks(urut, pilihan);

    printf("\n%-4s %-30s %10s %6s %8s\n", "No", "Nama Produk", "Harga", "Stok", "Terjual");
    printf("--------------------------------------------------------------\n");
    for (i = 0; i < jumlah; i++)
        printf("%-4d %-30s %10d %6d %8d\n", urut[i] + 1, nama[urut[i]],
               harga[urut[i]], stok[urut[i]], terjual[urut[i]]);
}

/* ===== Fitur 8: Laporan penjualan ===== */
void laporanPenjualan(void) {
    int i, maks = 0;
    long totalUnit = 0, totalPendapatan = 0;

    if (jumlah == 0) {
        printf("Belum ada produk.\n");
        return;
    }

    printf("\n================ LAPORAN PENJUALAN ================\n");
    printf("%-4s %-25s %8s %14s\n", "No", "Nama Produk", "Terjual", "Pendapatan");
    printf("---------------------------------------------------\n");
    for (i = 0; i < jumlah; i++) {
        printf("%-4d %-25s %8d %14ld\n", i + 1, nama[i], terjual[i], pendapatan[i]);
        totalUnit += terjual[i];
        totalPendapatan += pendapatan[i];
        if (terjual[i] > maks) maks = terjual[i];
    }
    printf("---------------------------------------------------\n");
    printf("Total unit terjual : %ld\n", totalUnit);
    printf("Total pendapatan   : %ld\n", totalPendapatan);

    if (maks == 0) {
        printf("Produk terlaris    : (belum ada penjualan)\n");
    } else {
        printf("Produk terlaris    : ");
        int pertama = 1;
        for (i = 0; i < jumlah; i++) {
            if (terjual[i] == maks) {
                printf("%s%s", pertama ? "" : ", ", nama[i]);
                pertama = 0;
            }
        }
        printf(" (%d unit)\n", maks);
    }
}

/* ===== Fitur 9: Cek stok menipis ===== */
void cekStokMenipis(void) {
    int i, ada = 0;

    printf("\nProduk dengan stok kurang dari %d:\n", BATAS_MENIPIS);
    for (i = 0; i < jumlah; i++) {
        if (stok[i] < BATAS_MENIPIS) {
            printf("  [PERINGATAN] %d. %-25s stok: %d\n", i + 1, nama[i], stok[i]);
            ada = 1;
        }
    }
    if (!ada) printf("  Semua stok aman.\n");
}

/* ===== Menu ===== */
void tampilMenu(void) {
    printf("\n====== KANTIN KAMPUS - MONITORING STOK & PENJUALAN ======\n");
    printf(" 1. Lihat Daftar Produk     6. Cari Produk\n");
    printf(" 2. Tambah Produk           7. Urutkan Produk\n");
    printf(" 3. Ubah Produk             8. Laporan Penjualan\n");
    printf(" 4. Transaksi Penjualan     9. Cek Stok Menipis\n");
    printf(" 5. Tambah Stok             0. Keluar\n");
    printf("=========================================================\n");
}

int main(void) {
    int pilihan;

    isiDataAwal();

    do {
        tampilMenu();
        if (!bacaInt("Pilih menu: ", &pilihan)) {
            printf("[!] Pilihan menu harus berupa angka.\n");
            pilihan = -1;
            continue;
        }

        switch (pilihan) {
            case 1: tampilDaftar();       break;
            case 2: tambahProduk();       break;
            case 3: ubahProduk();         break;
            case 4: transaksiPenjualan(); break;
            case 5: tambahStok();         break;
            case 6: cariProduk();         break;
            case 7: urutkanProduk();      break;
            case 8: laporanPenjualan();   break;
            case 9: cekStokMenipis();     break;
            case 0: printf("Terima kasih. Program selesai.\n"); break;
            default: printf("[!] Pilihan menu tidak tersedia.\n");
        }
    } while (pilihan != 0);

    return 0;
}
