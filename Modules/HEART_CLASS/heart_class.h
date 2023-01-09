#ifndef DADM_HEART_CLASS_H
#define DADM_HEART_CLASS_H

#include <complex>
#include <iostream>
#include <valarray>
#include <vector>
#include <memory>
#include <numeric>

/* #include "../Waves/waves.h" */

/* Mozliwe typy aktywacji */
enum type{
    DIFF_DISEASE = 0,
    VENTRICULAR = 1,
    SUPRAVENTRICULAR = 2,
    ARTIFACT = 3,
};

/* Struktura zawierajaca index i typ klasyfikacji -> STRUKTURA WYJSCIOWA */
typedef struct activations{
    float Ridx;             /* index R piku */
    enum type actType;      /* typ aktywacji (enum type) */
}activations_t;

/* Glowna klasa modulu
 * Po wywolaniu konstruktora otrzymujemy wskaznik do wektora struktur aktywacji
 */
class heart_class {
private:
/*
 * Funkcja sprawdzajaca obecnosc dysocjacji AV.
 * IN:       waves - wektor cech z modulu WAVES
 * IN:       currR - aktualny pik R
 * OUT:      rodzaj aktywacji.
 */
    enum type CheckAVDissociation(std::vector<int> rPeaks, std::vector<int> P, int currR, std::vector<int> QRSonset1);

/*
 * Funkcja do klasyfikacji
 * IN:       rPeaks - wektor pozycji zalamkow R
 * IN:       waves - wektor cech z modulu WAVES
 * IN:       fs - czestotliwosc probkowania
 */
    void Classify_Type(std::vector<int> rPeaks, std::vector<int> P, std::vector<int> QRSend, std::vector<int> QRSonset, int fs);

    std::vector<activations_t> activations;
    std::vector <float> m_filteredSignal;
    std::vector <int> m_rPeaks;
    std::vector<int> m_rPeaksLocs;
    std::vector <int> m_QRSend;
    std::vector <int> m_QRSonset;
    std::vector <int> m_P;
public:

/* Konstruktor
 * IN:       rPeaks - wektor pozycji zalamkow R
 * IN:       waves - wektor cech z modulu WAVES
 * IN:       fs - czestotliwosc probkowania
 */
    heart_class(std::vector<int> rPeaks, std::vector<int> P, std::vector<int> QRSend, std::vector<int> QRSonset, int fs, std::vector <float> filteredSignal,
    std::vector<int> rPeaksLocs)
    {
            m_rPeaks = rPeaks;
            m_rPeaksLocs = rPeaksLocs;
            m_QRSend = QRSend;
            m_QRSonset = QRSonset;
            m_P = P;
            m_filteredSignal = filteredSignal;
            Classify_Type(rPeaks, P, QRSend, QRSonset, fs);
    }

/* Funkcja pobierajaca kolejne aktywacje
 * OUT:      wskaznik do wektora aktywacji.
 */
    std::shared_ptr<std::vector<activations_t>> heart_classGetActivations()
    {
        return std::make_shared<std::vector<activations_t>>(this->activations);
    }
};
#endif //DADM_HEART_CLASS_H